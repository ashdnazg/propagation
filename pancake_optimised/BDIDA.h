#pragma once

class BDIDAStarPancakeSearcher
{
public:


	struct Node {
		int flip;
		//unsigned h;
	};

	typedef BloomFilter<Pancakes, PancakeHasher, 1 << 24, 4> myBloomFilter;

	static unsigned DFS(const Pancakes& start, unsigned& maxF, myBloomFilter& writeBloomFilter, myBloomFilter& readBloomFilter, bool firstRun);
	static unsigned search(const Pancakes& start);
	static std::uint64_t generated;


};



unsigned BDIDAStarPancakeSearcher::DFS(const Pancakes& start, unsigned& maxF, myBloomFilter& writeBloomFilter, myBloomFilter& readBloomFilter, bool firstRun) {
	Node stack[MAX_SOLUTION * MAX_DESCENDANTS];
	Pancakes pancakes;

	Node* sp = stack;
	int g = -1;
	// Creating start node
	{
		for (unsigned pos = 0; pos < N; ++pos) {
			pancakes[pos] = start[pos];
		}
		sp->flip = 1;
		//sp->h = h;
		// printf("initial h: %u, maxF: %u\n", h, maxF);
	}

	//maxF = std::max(sp->h, maxF);

	while (sp >= stack) {
		const int flip = sp->flip;

		// unroll
		if (flip < 0) {
			const int backflip = -flip;
			const int pivot = backflip / 2;
			for (int pos = 0; pos < pivot; ++pos) {
				std::swap(pancakes[pos], pancakes[backflip - pos - 1]);
			}
			--sp;
			--g;
			// printf("unrolling, g: %d=>%d, flip: %d\n", g + 1 ,g, flip);
			continue;
		}

		// commit
		{
			const int pivot = flip / 2;
			for (int pos = 0; pos < pivot; ++pos) {
				std::swap(pancakes[pos], pancakes[flip - pos - 1]);
			}
			++g;
			// printf("committing, g: %d=>%d, flip: %d\n", g - 1 ,g, flip);
		}

		//mark as traversed
		sp->flip *= -1;


		// don't expand if last layer (continue sends us straight to unroll)
		if (unsigned(g) == maxF) {
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

				(++sp)->flip = pos;
				++generated;
				// printf("generating, pos: %d\n", pos);
			}
		}
	}

	if (sp < stack)
		return maxF + 1;

	return maxF;
}

unsigned BDIDAStarPancakeSearcher::search(const Pancakes& start) {
	Pancakes goal;
	unsigned forwardMaxF = 0;
	unsigned backwardMaxF = 0;
	myBloomFilter forwardFilter;
	myBloomFilter backwardFilter;

	for (unsigned i = 0; i < N; ++i) {
		goal[i] = i;
	}

	unsigned result;
	while (true) {
		printf("Starting iterations with forwardMaxF: %u backwardMaxF: %u \n", forwardMaxF, backwardMaxF);
		DFS(start, forwardMaxF, forwardFilter, backwardFilter, true);
		bool found = false;
		bool reverse = true;
		while (true) {
			if (reverse) {
				printf("Doing reverse\n");
				backwardFilter.clear();
				DFS(goal, backwardMaxF, backwardFilter, forwardFilter, false);

			} else {
				printf("Doing forward\n");
				forwardFilter.clear();
				DFS(start, forwardMaxF, forwardFilter, backwardFilter, false);
			}
			if (forwardFilter.isUsingSet() && backwardFilter.isUsingSet()) {
				if (!forwardFilter.isSetEmpty() && !backwardFilter.isSetEmpty()) {
					found = true;
					result = backwardMaxF + forwardMaxF;
				}
				break;
			}
			reverse = !reverse;
		}

		if (found) {
			break;
		}

		if (forwardMaxF == backwardMaxF) {
			forwardMaxF++;
		} else {
			backwardMaxF++;
		}
		forwardFilter.clear();
	}

	printf("generated: %llu\n", generated);
	return result;
}
std::uint64_t BDIDAStarPancakeSearcher::generated = 0;