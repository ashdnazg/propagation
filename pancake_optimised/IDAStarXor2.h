#pragma once

class IDAStarXorPancakeSearcher2
{
public:


	struct Node {
		unsigned first;
		unsigned before_flip;
		unsigned after_flip;
		int flip;
		//unsigned h;
	};

	typedef unsigned Cache[N + 1][N + 1];

	static bool DFS(const Pancakes& start, const Pancakes& goal, unsigned th);
	static unsigned search(const Pancakes& start);
	static unsigned initCache(Cache& cache, const Pancakes& start, const Pancakes& goal);
	static std::uint64_t generated;
};

unsigned IDAStarXorPancakeSearcher2::initCache(Cache& cache, const Pancakes& start, const Pancakes& goal) {
	memset(cache, 0, (N + 1) * (N + 1) * sizeof(**cache));

	for (int pos = GAP_LEVEL; pos < N; ++pos) {
		cache[goal[pos]][goal[pos + 1]] = 1;
		cache[goal[pos + 1]][goal[pos]] = 1;
	}
	unsigned h = N - GAP_LEVEL;

	for (int pos = 0; pos < N; ++pos) {
		h -= cache[start[pos]][start[pos + 1]];
	}

	return h;
}


bool IDAStarXorPancakeSearcher2::DFS(const Pancakes& start, const Pancakes& goal, unsigned th) {
	Node stack[MAX_SOLUTION * MAX_DESCENDANTS];
	Pancakes neighbors;
	Cache hCache;


	unsigned h = initCache(hCache, start, goal);

	if (th < h) {
		//printf("breaking forward since initial h was: %u\n", h);
		return false;
	}

	Node* sp = stack;
	int g = -1;

	// Creating start node
	{
		neighbors[start[0]] = start[1];
		for (unsigned pos = 1; pos < N; ++pos) {
			neighbors[start[pos]] = start[pos - 1] ^ start[pos + 1];
		}
		neighbors[N] = start[N - 1];

		sp->first = start[0];
		sp->before_flip = start[0];
		sp->after_flip = start[1];
		sp->flip = 1;
	}


	// maxF = std::max(sp->h, maxF);

	while (sp >= stack) {
		const unsigned first = sp->first;
		const unsigned before_flip = sp->before_flip;
		const unsigned after_flip = sp->after_flip;
		const int flip = sp->flip;
		//printf("flip: %d, first: %u, before_flip: %u, after_flip: %u\n", flip ,first, before_flip, after_flip);

		// unroll
		if (sp->flip < 0) {
			h += hCache[after_flip][first];
			h -= hCache[after_flip][before_flip];

			neighbors[before_flip] ^= after_flip;
			neighbors[after_flip] ^= before_flip ^ first;
			neighbors[first] ^= after_flip;

			--sp;
			--g;
			//printf("unrolling, g: %d=>%d, flip: %d, h: %u\n", g + 1 ,g, flip, h);
			//printState(pancakes);
			//printf("\n");
			continue;
		}

		// commit
		{
			h -= hCache[after_flip][first];
			h += hCache[after_flip][before_flip];

			neighbors[before_flip] ^= after_flip;
			neighbors[after_flip] ^= before_flip ^ first;
			neighbors[first] ^= after_flip;

			++g;
			//printf("committing, g: %d=>%d, flip: %d, h: %u\n", g - 1 ,g, flip, h);
			// printState(pancakes);
			// printf("\n");
		}

		if (h == 0)
			return true;

		//mark as traversed
		sp->flip = -flip;



		// expand
		{
			unsigned temp_before_flip = neighbors[before_flip];
			unsigned temp_after_flip = neighbors[temp_before_flip] ^ before_flip;
			for (int pos = 2; pos <= N; ++pos) {
				//printf("generating, h:%d, g: %d, pos: %u\n", h, g, pos);
				if (pos != flip) {
					unsigned tempH = h;
					tempH -= hCache[temp_after_flip][before_flip];
					tempH += hCache[temp_after_flip][temp_before_flip];
					//printf("tempH: %d\n", tempH);
					if ((g + 1 + tempH) <= th) {

						++sp;
						sp->first = before_flip;
						sp->before_flip = temp_before_flip;
						sp->after_flip = temp_after_flip;
						sp->flip = pos;

						++generated;
					}
				}
				unsigned old_before_flip = temp_before_flip;
				temp_before_flip = temp_after_flip;
				temp_after_flip = neighbors[temp_after_flip] ^ old_before_flip;
			}
		}
	}

	return false;
}

unsigned IDAStarXorPancakeSearcher2::search(const Pancakes& start) {
	generated = 0;
	Pancakes goal;
	unsigned th = 0;
	for (unsigned i = 0; i < N + 1; ++i) {
		goal[i] = i;
	}
	while (true) {
		//printf("Starting iteration with thF: %u, thB: %u\n", thF, thB);
		bool found = DFS(start, goal, th);
		if (found) {
			return th;
		}

		++th;
	}
	return 0;
}

std::uint64_t IDAStarXorPancakeSearcher2::generated = 0;