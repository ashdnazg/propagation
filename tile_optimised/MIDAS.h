#pragma once

class IDAStarTileSearcher
{
public:
	struct Node {
		unsigned blankPos;
		unsigned h;
		unsigned hB;
		//unsigned char prevBlankPos;
		//unsigned char hF;
	};
	struct Operator {
		unsigned num;
		unsigned char nextBlankPos[MAX_DESCENDANTS];
	};

	static unsigned DFS(const Tiles& start, const Tiles& goal, unsigned& maxF);
	static unsigned search(const Tiles& start);
	static std::uint64_t generated[MAX_SOLUTION + 10];

};

unsigned IDAStarTileSearcher::DFS(const Tiles& start, const Tiles& goal, unsigned& maxF) {
	unsigned hFCache[N][N] = {{0}};
	unsigned hBCache[N][N] = {{0}};
	Operator opCache[N][N] = {{0}}; //Sparse, but fast
	std::uint64_t nodeCount[MAX_SOLUTION][MAX_SOLUTION] = {{0}};

	Node stack[MAX_SOLUTION * MAX_DESCENDANTS];
	unsigned blankPosStack[MAX_SOLUTION + 10];
	Tiles tiles;

	Node* sp = stack;
	unsigned* bpsp = blankPosStack;
	unsigned g = 0;


	// Initializing hFCache
	{
		std::array<char, N> startPos;
		std::array<char, N> goalPos;
		for (unsigned i = 0; i < N; ++i) {
			startPos[start[i]] = i;
			goalPos[goal[i]] = i;
		}

		for (unsigned i = 1; i < N; ++i) {
			char targetPosF = goalPos[i];
			char targetPosB = startPos[i];
			for (char pos = 0; pos < N; ++pos) {
				char dxF = std::abs(pos % ROW_SIZE - targetPosF % ROW_SIZE);
				char dyF = std::abs(pos / ROW_SIZE - targetPosF / ROW_SIZE);
				hFCache[i][(unsigned char)pos] = dxF + dyF;

				char dxB = std::abs(pos % ROW_SIZE - targetPosB % ROW_SIZE);
				char dyB = std::abs(pos / ROW_SIZE - targetPosB / ROW_SIZE);
				hBCache[i][(unsigned char)pos] = dxB + dyB;
			}
		}
	}

	// Initializing opCache (inefficient, but who cares?)
	{
		for (char blankPos = 0; blankPos < N; ++blankPos) {
			char blankPosX = blankPos % ROW_SIZE;
			char blankPosY = blankPos / ROW_SIZE;
			for (char nextBlankPos = 0; nextBlankPos < N; ++nextBlankPos) {
				char nextBlankPosX = nextBlankPos % ROW_SIZE;
				char nextBlankPosY = nextBlankPos / ROW_SIZE;
				if (std::abs(blankPosX - nextBlankPosX) + std::abs(blankPosY - nextBlankPosY) != 1)
					continue;

				for (char prevBlankPos = 0; prevBlankPos < N; ++prevBlankPos) {
					if (prevBlankPos == nextBlankPos)
						continue;

					auto& op = opCache[(unsigned char)prevBlankPos][(unsigned char)blankPos];
					op.nextBlankPos[op.num] = nextBlankPos;
					op.num++;
				}
			}
		}
	}


	// Creating start node
	{
		unsigned char blankPos;
		unsigned h = 0;
		for (unsigned pos = 0; pos < N; ++pos) {
			const unsigned char tile = start[pos];
			if (tile == 0) {
				blankPos = pos;
			} else {
				h += hFCache[tile][pos];
			}
			tiles[pos] = tile;
		}
		sp->blankPos = blankPos;
		sp->h = h;
		sp->hB = 0;
		*bpsp = blankPos;

	}

	if (maxF == 0)
		maxF = sp->h;

	printf("initial h: %u, maxF: %u\n", sp->h, maxF);
	bool found = false;

	while (sp >= stack) {
		Node& currentNode = *sp;
		const unsigned blankPos = currentNode.blankPos;

		if (currentNode.h == 0) {
			--bpsp;
			--sp;
			--g;
			// printf("unrolling: %u=>%u\n", currentNode.blankPos, tiles[*bpsp]);
			tiles[blankPos] = tiles[*bpsp];
			continue;
		}

		const unsigned prevBlankPos = *bpsp;
		const unsigned tile = tiles[blankPos];
		generated[g]++;
		const unsigned h = currentNode.h + hFCache[tile][prevBlankPos] - hFCache[tile][blankPos];
		const unsigned hB = currentNode.hB + hBCache[tile][prevBlankPos] - hBCache[tile][blankPos];
		// printf("expanding: %u=>%u\n", prevBlankPos, currentNode.blankPos);


		// current node is off limits
		if (g + h > maxF) {
			--sp;
			continue;
		}

		if ((g + g) == (maxF + 1) || (g + g) == maxF) {
			nodeCount[h][hB]++;
			--sp;
			continue;
		}

		// current node is the goal
		if (h == 0) {
			found = true;
			--sp;
			continue;
		}

		const Operator& currentOp = opCache[prevBlankPos][blankPos];
		++bpsp;
		++g;
		*bpsp = blankPos;
		tiles[prevBlankPos] = tile;


		switch (currentOp.num) {
			case 4: {
				++sp;
				sp->h = h;
				sp->hB = hB;
				sp->blankPos = currentOp.nextBlankPos[3];
				// printf("generating: %u=>%u\n", currentNode.blankPos, sp->blankPos);
				// printf("h: %u g: %u\n", sp->h, g);
			}
			case 3: {
				++sp;
				sp->h = h;
				sp->hB = hB;
				sp->blankPos = currentOp.nextBlankPos[2];
				// printf("generating: %u=>%u\n", currentNode.blankPos, sp->blankPos);
				// printf("h: %u g: %u\n", sp->h, g);
			}
			case 2: {
				++sp;
				sp->h = h;
				sp->hB = hB;
				sp->blankPos = currentOp.nextBlankPos[1];
				// printf("generating: %u=>%u\n", currentNode.blankPos, sp->blankPos);
				// printf("h: %u g: %u\n", sp->h, g);
			}
			default: {
				++sp;
				sp->h = h;
				sp->hB = hB;
				sp->blankPos = currentOp.nextBlankPos[0];
				// printf("generating: %u=>%u\n", currentNode.blankPos, sp->blankPos);
				// printf("h: %u g: %u\n", sp->h, g);
			}
		}

		// mark node as passed so it's not expanded again:
		currentNode.h = 0;
	}
	for (unsigned h = 0; h < MAX_SOLUTION; ++h) {
		for (unsigned hB = 0; hB < MAX_SOLUTION; ++hB) {
			std::uint64_t count = nodeCount[h][hB];
			if (count > 0)
				printf("hF: %u, hB: %u, count: %llu\n", h, hB, count);
		}
	}

	if (!found)
		return maxF + 2;

	return maxF;
}

unsigned IDAStarTileSearcher::search(const Tiles& start) {
	Tiles goal {
		1, 2, 3, 4,
		5, 6, 7, 8,
		9,10,11,12,
		13,14,15,0
	};
	unsigned maxF = 0;
	while (true) {
		unsigned result = DFS(start, goal, maxF);
		if (result == maxF)
			break;

		if (maxF == 52)
			break;

		maxF = result;
	}
	std::uint64_t total = 0;
	std::uint64_t half = 0;
	for (unsigned i = 0; i < MAX_SOLUTION + 1; ++i) {
		printf("generated(%u): %llu\n", i, generated[i]);
		total += generated[i];
		half += generated[i] * ((i * 2) < maxF);
	}
	printf("generated(half): %llu\n", half);
	printf("generated(total): %llu\n", total);

	return maxF;
}
std::uint64_t IDAStarTileSearcher::generated[MAX_SOLUTION + 10];