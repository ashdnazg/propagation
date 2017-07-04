#pragma once

class IDAStarPancakeSearcher
{
public:


	struct Node {
		int flip;
		//unsigned h;
	};

	typedef unsigned Cache[N][N];

	static bool DFS(const Pancakes& start, const Pancakes& goal, unsigned th);
	static unsigned search(const Pancakes& start);
	static unsigned initCache(Cache& cache, const Pancakes& start, const Pancakes& goal);
	static std::uint64_t generated;
};


unsigned IDAStarPancakeSearcher::initCache(Cache& cache, const Pancakes& start, const Pancakes& goal) {
	memset(cache, 0, N * N * sizeof(**cache));

	for (int pos = 0; pos < N - 1; ++pos) {
		cache[goal[pos]][goal[pos + 1]] = 1;
		cache[goal[pos + 1]][goal[pos]] = 1;
	}
	unsigned h = N;

	for (int pos = 0; pos < N - 1; ++pos) {
		h -= cache[start[pos]][start[pos + 1]];
	}

	h -= start[N - 1] == goal[N - 1];

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
		for (unsigned pos = 0; pos < N; ++pos) {
			pancakes[pos] = start[pos];
		}
		sp->flip = 1;
		//sp->h = h;
	}


	// maxF = std::max(sp->h, maxF);

	while (sp >= stack) {
		const int flip = sp->flip;

		// unroll
		if (flip < 0) {
			const int backflip = -flip;
			const int pivot = backflip / 2;
			if (backflip < N) {
				h -= hCache[pancakes[backflip]][pancakes[0]];
				h += hCache[pancakes[backflip]][pancakes[backflip - 1]];
			} else {
				h -= pancakes[0] == goal[N - 1];
				h += pancakes[N - 1] == goal[N - 1];
			}
			for (int pos = 0; pos < pivot; ++pos) {
				std::swap(pancakes[pos], pancakes[backflip - pos - 1]);
			}
			--sp;
			--g;
			// printf("unrolling, g: %d=>%d, flip: %d, h: %u\n", g + 1 ,g, flip, h);
			// printState(pancakes);
			// printf("\n");
			continue;
		}

		// commit
		{
			if (flip < N) {
				h -= hCache[pancakes[flip]][pancakes[0]];
				h += hCache[pancakes[flip]][pancakes[flip - 1]];
			} else {
				h -= pancakes[0] == goal[N - 1];
				h += pancakes[N - 1] == goal[N - 1];
			}
			const int pivot = flip / 2;
			for (int pos = 0; pos < pivot; ++pos) {
				std::swap(pancakes[pos], pancakes[flip - pos - 1]);
			}
			++g;
			// printf("committing, g: %d=>%d, flip: %d, h: %u\n", g - 1 ,g, flip, h);
			// printState(pancakes);
			// printf("\n");
		}

		//mark as traversed
		sp->flip *= -1;


		// don't expand if last layer (continue sends us straight to unroll)
		if (unsigned(g) == th) {
			return true;
		}

		// expand
		{
			for (int pos = 2; pos <= N; ++pos) {
				if (pos == flip)
					continue;

				unsigned tempH = h;
				if (pos < N) {
					tempH -= hCache[pancakes[pos]][pancakes[0]];
					tempH += hCache[pancakes[pos]][pancakes[pos - 1]];
				} else {
					tempH -= pancakes[0] == goal[N - 1];
					tempH += pancakes[N - 1] == goal[N - 1];
				}
				//printf("generating, pos: %u expected h: %d\n", pos, tempH);
				if ((g + 1 + tempH) > th) {
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
	for (unsigned i = 0; i < N; ++i) {
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