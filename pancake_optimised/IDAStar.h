#pragma once

class IDAStarPancakeSearcher
{
public:
	struct Node {
		int flip;
		//unsigned h;
	};

	static unsigned DFS(const Pancakes& start, unsigned& maxF);
	static unsigned search(const Pancakes& start);
	static std::uint64_t generated;

};

unsigned IDAStarPancakeSearcher::DFS(const Pancakes& start, unsigned& maxF) {
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

		// goal check
		{
			bool isGoal = true;
			for (int pos = 0; pos < N; ++pos) {
				isGoal &= pancakes[pos] == pos;
				// printf("pancakes[%d] : %d, isGoal: %d\n", pos, pancakes[pos], isGoal);
			}
			if (isGoal)
				break;
		}

		//mark as traversed
		sp->flip *= -1;


		// don't expand if last layer (continue sends us straight to unroll)
		if (unsigned(g) == maxF)
			continue;

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

unsigned IDAStarPancakeSearcher::search(const Pancakes& start) {

	unsigned maxF = 1;
	while (true) {
		unsigned result = DFS(start, maxF);
		if (result == maxF)
			break;
		maxF = result;
		printf("Starting iteration with maxF: %u\n", maxF);
	}

	printf("generated: %llu\n", generated);
	return maxF;
}
std::uint64_t IDAStarPancakeSearcher::generated = 0;