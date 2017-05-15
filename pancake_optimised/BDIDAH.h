#pragma once

class BloomPancakeSearcher
{
public:


	struct Node {
		int flip;
		//unsigned h;
	};

	typedef BloomFilter<Pancakes, PancakeHasher, 1LLU << 32, 1> myBloomFilter;

	static unsigned DFS(const Pancakes& start, const Pancakes& goal, unsigned maxF, unsigned maxG, myBloomFilter& writeBloomFilter, myBloomFilter& readBloomFilter, bool firstRun);
	static unsigned search(const Pancakes& start);
	static std::uint64_t generated;


};



unsigned BloomPancakeSearcher::DFS(const Pancakes& start, const Pancakes& goal, unsigned maxF, unsigned maxG, myBloomFilter& writeBloomFilter, myBloomFilter& readBloomFilter, bool firstRun) {
	Node stack[MAX_SOLUTION * MAX_DESCENDANTS];
	Pancakes pancakes;
	unsigned hCache[N][N];


	memset(hCache, 0, N * N * sizeof(**hCache));

	for (int pos = 0; pos < N - 1; ++pos) {
		hCache[goal[pos]][goal[pos + 1]] = 1;
		hCache[goal[pos + 1]][goal[pos]] = 1;
	}

	unsigned h = N;

	for (int pos = 0; pos < N - 1; ++pos) {
		h -= hCache[start[pos]][start[pos + 1]];
	}
	h -= start[N - 1] == goal[N - 1];

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
		//printf("breaking since initial h was: %u\n", h);
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
		if (unsigned(g) == maxG) {
			//printf("match candidate: g: %u h: %u\n", g, h);
			if (firstRun || readBloomFilter.contains(pancakes)) {
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

	return maxF;
}

unsigned BloomPancakeSearcher::search(const Pancakes& start) {
	generated = 0;
	Pancakes goal;
	unsigned maxF = 0;
	myBloomFilter forwardFilter;
	myBloomFilter backwardFilter;
	backwardFilter.hashOffset = 1610612741;

	for (unsigned i = 0; i < N; ++i) {
		goal[i] = i;
	}
	std::uint64_t IDAGen;
	while (true) {
		// printf("Starting iterations with maxF: %u\n", maxF);
		std::uint64_t preGenerated = generated;
		DFS(start, goal, maxF, maxF / 2, forwardFilter, backwardFilter, true);
		IDAGen = generated - preGenerated;
		bool found = false;
		bool reverse = true;
		while (true) {
			if (reverse) {
				// printf("Doing reverse with %llu items and saturation %f\n", forwardFilter.getCount(), forwardFilter.getSaturation());
				backwardFilter.clear();
				DFS(goal, start, maxF, (maxF + 1) / 2, backwardFilter, forwardFilter, false);
			} else {
				// printf("Doing forward with %llu items and saturation %f\n", backwardFilter.getCount(), backwardFilter.getSaturation());
				forwardFilter.clear();
				DFS(start, goal, maxF, maxF / 2, forwardFilter, backwardFilter, false);
			}
			if (forwardFilter.isUsingSet() && backwardFilter.isUsingSet()) {
				if (!forwardFilter.isSetEmpty() && !backwardFilter.isSetEmpty()) {
					// myBloomFilter& iterFilter = forwardFilter.getCount() > backwardFilter.getCount() ? backwardFilter : forwardFilter;
					// myBloomFilter& testFilter = forwardFilter.getCount() > backwardFilter.getCount() ? forwardFilter : backwardFilter;
					// int count = 0;
					// for (const Pancakes& p: iterFilter.hashSet) {
						// if (testFilter.hashSet.find(p) != testFilter.hashSet.end()) {
							// printState(p);
							// printf("\n");
							// count++;
						// }
						// if (count > 10) {
							// break;
						// }
					// }
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
	printf("IDA*: %llu\n", IDAGen);
	//printf("generated: %llu\n", generated);
	return maxF;
}
std::uint64_t BloomPancakeSearcher::generated = 0;