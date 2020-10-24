#pragma once

class IDAStarSlimPancakeSearcher
{
public:


	struct Node {
		unsigned flip;
		//unsigned h;
	};

	typedef std::array<std::array<unsigned char, N + 1>, N + 1> Cache;

	static bool DFS(const Pancakes& start, const Pancakes& goal, unsigned th);
	static unsigned search(const Pancakes& start);
	static unsigned initCache(Cache& cache, const Pancakes& start, const Pancakes& goal);
	static std::uint64_t generated;
};

unsigned IDAStarSlimPancakeSearcher::initCache(Cache& cache, const Pancakes& start, const Pancakes& goal) {
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


bool IDAStarSlimPancakeSearcher::DFS(const Pancakes& start, const Pancakes& goal, unsigned th) {
	Node stack[MAX_SOLUTION * MAX_DESCENDANTS];
	Cache hCache;
	Pancakes pancakes = start;
	unsigned h = initCache(hCache, start, goal);

	if (th < h)
		return false;

	stack[0] = {N + 1};
	stack[1] = {N - 1};
	Node* sp = stack + 1;

	while (true) {
LOOP_START:
		if (sp == stack)
			return false;

		unsigned flip = sp->flip;
		while (true) {
			if (flip == 0) {
				const unsigned prevFlip = (sp - 1)->flip;
				//printf("rolling back\n");
				h -= hCache[pancakes[prevFlip + 1]][pancakes[0]];
				h += hCache[pancakes[prevFlip + 1]][pancakes[prevFlip]];
				std::reverse(pancakes.begin(), pancakes.begin() + prevFlip + 1);
				++th;
				--sp;
				sp->flip = prevFlip - 1;

				goto LOOP_START;
			}

			int tempTh = th - h;
			tempTh += hCache[pancakes[flip + 1]][pancakes[0]];
			tempTh -= hCache[pancakes[flip + 1]][pancakes[flip]];

			if (tempTh > 0)
				goto FOUND;

			--flip;
		}
FOUND:
		++generated;
		//printf("flip: %d\n", flip);


		h -= hCache[pancakes[flip + 1]][pancakes[0]];
		h += hCache[pancakes[flip + 1]][pancakes[flip]];

		if (h == 0)
			return true;

		std::reverse(pancakes.begin(), pancakes.begin() + flip + 1);
		--th;


		sp->flip = flip;

		++sp;
		sp->flip = N - 1;
	}
}

unsigned IDAStarSlimPancakeSearcher::search(const Pancakes& start) {
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

std::uint64_t IDAStarSlimPancakeSearcher::generated = 0;