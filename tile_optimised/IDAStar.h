#pragma once

class IDAStarTileSearcher
{
public:
	struct Node {
		unsigned blankPos;
		unsigned h;
	};
	struct Operator {
		unsigned num;
		unsigned char nextBlankPos[MAX_DESCENDANTS];
	};

	static unsigned DFS(const Tiles& start, unsigned& maxF);
	static unsigned search(const Tiles& start);
	static std::uint64_t generated;

};

unsigned IDAStarTileSearcher::DFS(const Tiles& start, unsigned& maxF) {
	unsigned hCache[N][N] = {{0}};
	Operator opCache[N][N] = {{0}}; //Sparse, but fast

	Node stack[MAX_SOLUTION * MAX_DESCENDANTS];
	unsigned blankPosStack[MAX_SOLUTION + 10];
	Tiles tiles;

	Node* sp = stack;
	unsigned* bpsp = blankPosStack;
	unsigned g = 0;


	// Initializing hCache
	{
		for (unsigned i = 1; i < N; ++i) {
			char targetPos = i - 1;
			for (char pos = 0; pos < N; ++pos) {
				char dx = std::abs(pos % ROW_SIZE - targetPos % ROW_SIZE);
				char dy = std::abs(pos / ROW_SIZE - targetPos / ROW_SIZE);
				hCache[i][(unsigned char)pos] = dx + dy;
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
				h += hCache[tile][pos];
			}
			tiles[pos] = tile;
		}
		sp->blankPos = blankPos;
		sp->h = h;
		*bpsp = blankPos;

		// printf("initial h: %u, maxF: %u\n", h, maxF);
	}

	if (maxF == 0)
		maxF = sp->h;

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
		generated++;
		const unsigned h = currentNode.h + hCache[tile][prevBlankPos] - hCache[tile][blankPos];
		// printf("expanding: %u=>%u\n", prevBlankPos, currentNode.blankPos);


		// current node is off limits
		if (g + h > maxF) {
			--sp;
			continue;
		}


		// current node is the goal
		if (h == 0)
			break;

		const Operator& currentOp = opCache[prevBlankPos][blankPos];
		++bpsp;
		++g;
		*bpsp = blankPos;
		tiles[prevBlankPos] = tile;


		switch (currentOp.num) {
			case 4: {
				++sp;
				sp->h = h;
				sp->blankPos = currentOp.nextBlankPos[3];
				// printf("generating: %u=>%u\n", currentNode.blankPos, sp->blankPos);
				// printf("h: %u g: %u\n", sp->h, g);
			}
			case 3: {
				++sp;
				sp->h = h;
				sp->blankPos = currentOp.nextBlankPos[2];
				// printf("generating: %u=>%u\n", currentNode.blankPos, sp->blankPos);
				// printf("h: %u g: %u\n", sp->h, g);
			}
			case 2: {
				++sp;
				sp->h = h;
				sp->blankPos = currentOp.nextBlankPos[1];
				// printf("generating: %u=>%u\n", currentNode.blankPos, sp->blankPos);
				// printf("h: %u g: %u\n", sp->h, g);
			}
			default: {
				++sp;
				sp->h = h;
				sp->blankPos = currentOp.nextBlankPos[0];
				// printf("generating: %u=>%u\n", currentNode.blankPos, sp->blankPos);
				// printf("h: %u g: %u\n", sp->h, g);
			}
		}

		// mark node as passed so it's not expanded again:
		currentNode.h = 0;
	}

	if (sp < stack)
		return maxF + 2;

	return maxF;
}

unsigned IDAStarTileSearcher::search(const Tiles& start) {

	unsigned maxF = 0;
	while (true) {
		unsigned result = DFS(start, maxF);
		if (result == maxF)
			break;
		maxF = result;
	}

	printf("generated: %llu\n", generated);
	return maxF;
}
std::uint64_t IDAStarTileSearcher::generated = 0;