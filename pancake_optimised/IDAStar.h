#pragma once

class IDAStarPancakeSearcher
{
public:


	struct Node {
		int flip;
		//unsigned h;
	};

	typedef std::array<std::array<unsigned char, N + 1>, N + 1> Cache;

	static bool DFS(const Pancakes& start, const Pancakes& goal, unsigned th);
	static unsigned search(const Pancakes& start);
	static unsigned initCache(Cache& cache, const Pancakes& start, const Pancakes& goal);
	static std::uint64_t generated;
};

unsigned IDAStarPancakeSearcher::initCache(Cache& cache, const Pancakes& start, const Pancakes& goal) {
	memset(cache.data()->data(), 0, (N + 1) * (N + 1) * sizeof(cache[0][0]));

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


bool IDAStarPancakeSearcher::DFS(const Pancakes& start, const Pancakes& goal, unsigned th) {
	Node stack[MAX_SOLUTION * MAX_DESCENDANTS];
	Pancakes pancakes;
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
		for (unsigned pos = 0; pos < N + 1; ++pos) {
			pancakes[pos] = start[pos];
		}
		sp->flip = 1;
		//sp->h = h;
	}


	// maxF = std::max(sp->h, maxF);

	while (sp >= stack) {
		const int flip = sp->flip;
		//printf("flip: %d, first: %u, before_flip: %u, after_flip: %u\n", flip ,pancakes[0], pancakes[std::abs(flip) - 1], pancakes[std::abs(flip)]);

		// unroll
		if (flip < 0) {
			const int backflip = -flip;
			h -= hCache[pancakes[backflip]][pancakes[0]];
			h += hCache[pancakes[backflip]][pancakes[backflip - 1]];
			std::reverse(pancakes.begin(), pancakes.begin() + backflip);
			--sp;
			--g;
			//printf("unrolling, g: %d=>%d, flip: %d, h: %u\n", g + 1 ,g, flip, h);
			// printState(pancakes);
			// printf("\n");
			continue;
		}

		// commit
		{
			h -= hCache[pancakes[flip]][pancakes[0]];
			h += hCache[pancakes[flip]][pancakes[flip - 1]];
			std::reverse(pancakes.begin(), pancakes.begin() + flip);
			++g;
			//printf("committing, g: %d=>%d, flip: %d, h: %u\n", g - 1 ,g, flip, h);
			// printState(pancakes);
			// printf("\n");
		}

		if (h == 0)
			return true;

		//mark as traversed
		sp->flip *= -1;

		// expand
		{
			for (int pos = 2; pos <= N; ++pos) {
				//printf("generating, h:%d, g: %d, pos: %u\n", h, g, pos);
				if (pos == flip)
					continue;

				unsigned tempH = h;
				tempH -= hCache[pancakes[pos]][pancakes[0]];
				tempH += hCache[pancakes[pos]][pancakes[pos - 1]];
				//printf("tempH: %d\n", tempH);
				if ((g + tempH) >= th) {
					continue;
				}

				(++sp)->flip = pos;
				++generated;
			}
		}
	}

	return false;
}

unsigned IDAStarPancakeSearcher::search(const Pancakes& start) {
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

std::uint64_t IDAStarPancakeSearcher::generated = 0;