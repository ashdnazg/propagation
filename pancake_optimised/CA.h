#pragma once

class CAPancakeSearcher
{
public:


	struct Node {
		int flip;
		//unsigned h;
	};

	typedef HashSet<Pancakes, PancakeHasher, 1 << 30> myHashSet;
	typedef unsigned Cache[N + 1][N + 1];

	static bool CASearch(const Pancakes& start, const Pancakes& goal, unsigned thF, unsigned thB);
	static unsigned backwardSearch(const Pancakes& start, const Pancakes& goal, unsigned maxF, unsigned thB, myHashSet& hashSet, Cache& hCache);
	static unsigned search(const Pancakes& start);
	static unsigned initCache(Cache& cache, const Pancakes& start, const Pancakes& goal);
	static std::uint64_t generated;
	static std::uint64_t memoryUsed;
};


unsigned CAPancakeSearcher::initCache(Cache& cache, const Pancakes& start, const Pancakes& goal) {
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


unsigned CAPancakeSearcher::backwardSearch(const Pancakes& start, const Pancakes& goal, unsigned maxF, unsigned thB, myHashSet& hashSet, Cache& hCache) {
	Node stack[MAX_SOLUTION * MAX_DESCENDANTS];
	Pancakes pancakes;
	Pancakes legend;

	Node* sp = stack;
	int g = -1;
	unsigned h = N - GAP_LEVEL;
	// Creating start node
	{
		for (unsigned pos = 0; pos < N + 1; ++pos) {
			pancakes[pos] = start[pos];
			legend[goal[pos]] = pos;
		}
		sp->flip = 1;
		//sp->h = h;
	}
	{
		for (int pos = 0; pos < N; ++pos) {
			h -= hCache[legend[start[pos]]][legend[start[pos + 1]]];
		}

		if (maxF < h) {
			//printf("breaking backward since initial h was: %u\n", h);
			return false;
		}
	}

	// maxF = std::max(sp->h, maxF);

	while (sp >= stack) {
		const int flip = sp->flip;

		// unroll
		if (flip < 0) {
			const int backflip = -flip;
			h -= hCache[legend[pancakes[backflip]]][legend[pancakes[0]]];
			h += hCache[legend[pancakes[backflip]]][legend[pancakes[backflip - 1]]];
			std::reverse(pancakes.begin(), pancakes.begin() + backflip);

			--sp;
			--g;
			// printf("unrolling, g: %d=>%d, flip: %d, h: %u\n", g + 1 ,g, flip, h);
			// printState(pancakes);
			// printf("\n");
			continue;
		}

		// commit
		{
			h -= hCache[legend[pancakes[flip]]][legend[pancakes[0]]];
			h += hCache[legend[pancakes[flip]]][legend[pancakes[flip - 1]]];

			std::reverse(pancakes.begin(), pancakes.begin() + flip);
			++g;
			// printf("committing, g: %d=>%d, flip: %d, h: %u\n", g - 1 ,g, flip, h);
			// printState(pancakes);
			// printf("\n");
		}

		//mark as traversed
		sp->flip *= -1;


		// don't expand if last layer (continue sends us straight to unroll)
		if (unsigned(g) == thB) {
			if (hashSet.contains(pancakes)) {
				return true;
			}
			continue;
		}

		// expand
		{
			for (int pos = 2; pos <= N; ++pos) {
				if (pos == flip)
					continue;

				unsigned tempH = h;
				tempH -= hCache[legend[pancakes[pos]]][legend[pancakes[0]]];
				tempH += hCache[legend[pancakes[pos]]][legend[pancakes[pos - 1]]];
				//printf("generating, pos: %u expected h: %d\n", pos, tempH);
				if ((g + 1 + tempH) > maxF) {
					continue;
				}

				(++sp)->flip = pos;
				++generated;
			}
		}
	}

	return false;
}


bool CAPancakeSearcher::CASearch(const Pancakes& start, const Pancakes& goal, unsigned thF, unsigned thB) {
	Node stack[MAX_SOLUTION * MAX_DESCENDANTS];
	Pancakes pancakes;
	Pancakes commonAncestor;
	Pancakes nextCommonAncestor;
	Cache hCache;
	myHashSet hashSet;


	unsigned h = initCache(hCache, start, goal);
	unsigned maxF = thF + thB;

	if (maxF < h) {
		//printf("breaking forward since initial h was: %u\n", h);
		return false;
	}

	Node* sp = stack;
	int g = -1;
	int caDepth = -2;
	int nextCADepth = -2;
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

		// unroll
		if (flip < 0) {
			const int backflip = -flip;
			const int pivot = backflip / 2;
			//const unsigned prevH = h;
			h -= hCache[pancakes[backflip]][pancakes[0]];
			h += hCache[pancakes[backflip]][pancakes[backflip - 1]];
			for (int pos = 0; pos < pivot; ++pos) {
				std::swap(pancakes[pos], pancakes[backflip - pos - 1]);
			}
			if (g == nextCADepth) {
				nextCommonAncestor = pancakes;
				--nextCADepth;
			}

			//const bool forceSearchCondition = ;
			const bool forceSearchCondition = nextCADepth < int(thF >> 1);

			if (forceSearchCondition && !hashSet.isEmpty()) {
				//printf("doing backward search with ca depth: %d\n", caDepth);
				memoryUsed = std::max(memoryUsed, hashSet.getCount());
				const bool found = backwardSearch(goal, commonAncestor, maxF - caDepth, thB, hashSet, hCache);
				if (found) {
					return true;
				}
				nextCADepth = -2;
				caDepth = -2;
				hashSet.clear();
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
			h -= hCache[pancakes[flip]][pancakes[0]];
			h += hCache[pancakes[flip]][pancakes[flip - 1]];
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
		if (unsigned(g) == thF) {
			//printf("match candidate: g: %u h: %u\n", g, h);
			if (hashSet.isEmpty()) {
				nextCommonAncestor = pancakes;
				nextCADepth = thF;
			}
			const bool isFull = hashSet.insert(pancakes);
			memoryUsed = std::max(memoryUsed, hashSet.getCount());
			commonAncestor = nextCommonAncestor;
			caDepth = nextCADepth;
			if (isFull) {
				//printf("doing backward search with ca depth: %d\n", caDepth);
				const bool found = backwardSearch(goal, commonAncestor, maxF - caDepth, thB, hashSet, hCache);
				if (found) {
					return true;
				}
				nextCADepth = -2;
				caDepth = -2;
				hashSet.clear();
			}
			continue;
		}

		// expand
		{
			for (int pos = 2; pos <= N; ++pos) {
				if (pos == flip)
					continue;

				unsigned tempH = h;
				tempH -= hCache[pancakes[pos]][pancakes[0]];
				tempH += hCache[pancakes[pos]][pancakes[pos - 1]];
				//printf("generating, pos: %u expected h: %d\n", pos, tempH);
				if ((g + 1 + tempH) > maxF) {
					continue;
				}

				(++sp)->flip = pos;
				++generated;
			}
		}
	}

	if (!hashSet.isEmpty()) {
		printf("doing final backward search with ca depth: %d\n", caDepth);
		return backwardSearch(goal, commonAncestor, maxF - caDepth, thB, hashSet, hCache);
	}

	return false;
}

unsigned CAPancakeSearcher::search(const Pancakes& start) {
	generated = 0;
	memoryUsed = 0;
	Pancakes goal;
	unsigned thF = 0;
	unsigned thB = 0;
	for (unsigned i = 0; i < N + 1; ++i) {
		goal[i] = i;
	}
	while (true) {
		// printf("Starting iteration with thF: %u, thB: %u\n", thF, thB);
		bool found = CASearch(start, goal, thF, thB);
		if (found) {
			return thF + thB;
		}

		if (thF > thB) {
			++thB;
		} else {
			++thF;
		}
	}
	return 0;
}

std::uint64_t CAPancakeSearcher::generated = 0;
std::uint64_t CAPancakeSearcher::memoryUsed = 0;