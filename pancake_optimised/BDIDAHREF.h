#pragma once

class BDIDAStarPancakeSearcher
{
public:


	struct Node {
		int flip;
		//unsigned h;
	};
	typedef unsigned Cache[N][N];

	typedef BloomFilter<Pancakes, PancakeHasher, 1LLU << 28, 1> myBloomFilter;

	static unsigned DFS(const Pancakes& start, const Pancakes& goal, const Pancakes& reference, unsigned maxF, unsigned maxG, myBloomFilter& writeBloomFilter, myBloomFilter& readBloomFilter, bool firstRun);
	static unsigned search(const Pancakes& start);
	static unsigned initCache(Cache& cache, const Pancakes& start);
	static std::uint64_t generated;
	static std::uint64_t refHCounts[MAX_SOLUTION];
	static std::uint64_t hitRefHCounts[MAX_SOLUTION];


};

unsigned BDIDAStarPancakeSearcher::initCache(Cache& cache, const Pancakes& start) {
	unsigned h = N;
	return h;
}

unsigned BDIDAStarPancakeSearcher::DFS(const Pancakes& start, const Pancakes& goal, const Pancakes& reference, unsigned maxF, unsigned maxG, myBloomFilter& writeBloomFilter, myBloomFilter& readBloomFilter, bool firstRun) {
	Node stack[MAX_SOLUTION * MAX_DESCENDANTS];
	Pancakes pancakes;
	Cache hCache;
	Cache refHCache;

	memset(hCache, 0, N * N * sizeof(**hCache));
	memset(refHCache, 0, N * N * sizeof(**hCache));
	memset(refHCounts, 0, MAX_SOLUTION * sizeof(*refHCounts));
	memset(hitRefHCounts, 0, MAX_SOLUTION * sizeof(*hitRefHCounts));

	for (int pos = 0; pos < N - 1; ++pos) {
		hCache[goal[pos]][goal[pos + 1]] = 1;
		hCache[goal[pos + 1]][goal[pos]] = 1;
	}

	for (int pos = 0; pos < N - 1; ++pos) {
		refHCache[reference[pos]][reference[pos + 1]] = 1;
		refHCache[reference[pos + 1]][reference[pos]] = 1;
	}

	unsigned h = N;
	unsigned refH = N;

	for (int pos = 0; pos < N - 1; ++pos) {
		h -= hCache[start[pos]][start[pos + 1]];
		refH -= refHCache[start[pos]][start[pos + 1]];
	}
	h -= start[N - 1] == goal[N - 1];
	refH -= start[N - 1] == reference[N - 1];

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
	if (maxF < h) {
		printf("breaking since initial h was: %u\n", h);
		return maxF;
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
				refH -= refHCache[pancakes[backflip]][pancakes[0]];
				refH += refHCache[pancakes[backflip]][pancakes[backflip - 1]];
			} else {
				h -= pancakes[0] == goal[N - 1];
				h += pancakes[N - 1] == goal[N - 1];
				refH -= pancakes[0] == reference[N - 1];
				refH += pancakes[N - 1] == reference[N - 1];
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
				refH -= refHCache[pancakes[flip]][pancakes[0]];
				refH += refHCache[pancakes[flip]][pancakes[flip - 1]];
			} else {
				h -= pancakes[0] == goal[N - 1];
				h += pancakes[N - 1] == goal[N - 1];
				refH -= pancakes[0] == reference[N - 1];
				refH += pancakes[N - 1] == reference[N - 1];
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
		if (unsigned(g) == maxG) {
			++refHCounts[refH];
			//printf("match candidate: g: %u h: %u\n", g, h);
			if (firstRun || readBloomFilter.contains(pancakes)) {
				++hitRefHCounts[refH];
				writeBloomFilter.insert(pancakes);
			}
			continue;
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
				if ((g + 1 + tempH) > maxF) {
					continue;
				}

				(++sp)->flip = pos;
				++generated;
			}
		}
	}

	for (unsigned i = 0; i < MAX_SOLUTION; ++i) {
		if (refHCounts[i] != 0) {
			printf("%u: %llu, %llu\n", i, refHCounts[i], hitRefHCounts[i]);
		}
	}

	return maxF;
}

unsigned BDIDAStarPancakeSearcher::search(const Pancakes& start) {
	Pancakes goal;
	unsigned maxF = 0;
	myBloomFilter forwardFilter;
	myBloomFilter backwardFilter;
	backwardFilter.hashOffset = 1610612741;

	for (unsigned i = 0; i < N; ++i) {
		goal[i] = i;
	}
	Pancakes reference;
	createRandomState(reference);

	while (true) {
		printf("Starting iterations with maxF: %u\n", maxF);
		DFS(start, goal, reference, maxF, maxF / 2, forwardFilter, backwardFilter, true);
		bool found = false;
		bool reverse = true;
		while (true) {
			if (reverse) {
				printf("Doing reverse with %llu items and saturation %f\n", forwardFilter.getCount(), forwardFilter.getSaturation());
				backwardFilter.clear();
				DFS(goal, start, reference, maxF, (maxF + 1) / 2, backwardFilter, forwardFilter, false);
			} else {
				printf("Doing forward with %llu items and saturation %f\n", backwardFilter.getCount(), backwardFilter.getSaturation());
				forwardFilter.clear();
				DFS(start, goal, reference, maxF, maxF / 2, forwardFilter, backwardFilter, false);
			}
			if (forwardFilter.isUsingSet() && backwardFilter.isUsingSet()) {
				if (!forwardFilter.isSetEmpty() && !backwardFilter.isSetEmpty()) {
					myBloomFilter& iterFilter = forwardFilter.getCount() > backwardFilter.getCount() ? backwardFilter : forwardFilter;
					myBloomFilter& testFilter = forwardFilter.getCount() > backwardFilter.getCount() ? forwardFilter : backwardFilter;
					int count = 0;
					for (const Pancakes& p: iterFilter.hashSet) {
						if (testFilter.hashSet.find(p) != testFilter.hashSet.end()) {
							printState(p);
							printf("\n");
							count++;
						}
						if (count > 10) {
							break;
						}
					}
					found = true;
				}
				break;
			}
			reverse = !reverse;
		}

		if (found || maxF > N + 1) {
			break;
		}

		maxF++;
		forwardFilter.clear();
	}

	printf("generated: %llu\n", generated);
	return maxF;
}
std::uint64_t BDIDAStarPancakeSearcher::generated = 0;
std::uint64_t BDIDAStarPancakeSearcher::refHCounts[MAX_SOLUTION];
std::uint64_t BDIDAStarPancakeSearcher::hitRefHCounts[MAX_SOLUTION];