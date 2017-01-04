#pragma once

class IDAStarTileSearcher
{
public:
	struct Node {
		unsigned char blankPos;
		unsigned char prevBlankPos;
		unsigned char g;
		unsigned char f;
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
	char hCache[N][N] = {{0}};
	Operator opCache[N][N] = {{0}}; //Sparse, but fast

	Node stack[MAX_SOLUTION * MAX_DESCENDANTS];
	Tiles tiles;

	int sp = 0;


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
		stack[0].blankPos = blankPos;
		stack[0].prevBlankPos = blankPos;
		stack[0].g = 0;
		stack[0].f = h;
		// printf("initial h: %u\n", h);
	}

	if (maxF == 0)
		maxF = stack[0].f;

	while (sp >= 0) {
		Node& currentNode = stack[sp];

		if (currentNode.f == 0) {
			--sp;
			tiles[currentNode.blankPos] = tiles[currentNode.prevBlankPos];
			tiles[currentNode.prevBlankPos] = 0;
			continue;
		}

		const Operator& currentOp = opCache[currentNode.prevBlankPos][currentNode.blankPos];
		const unsigned char tile = tiles[currentNode.blankPos];
		tiles[currentNode.blankPos] = 0;
		tiles[currentNode.prevBlankPos] = tile;
		currentNode.f += hCache[tile][currentNode.prevBlankPos] - hCache[tile][currentNode.blankPos];
		generated++;

		if (currentNode.f > maxF) {
			--sp;
			tiles[currentNode.blankPos] = tiles[currentNode.prevBlankPos];
			tiles[currentNode.prevBlankPos] = 0;
			continue;
		}

		// current node is the goal
		if (currentNode.f == currentNode.g)
			break;


		currentNode.g++;

		switch (currentOp.num) {
			case 4: {
				Node& newNode = stack[++sp];

				newNode = currentNode;
				newNode.f = currentNode.f + 1;
				newNode.blankPos = currentOp.nextBlankPos[3];
				newNode.prevBlankPos = currentNode.blankPos;
				// printf("generating: %u=>%u\n", currentNode.blankPos, newNode.blankPos);
				// printf("f: %u g: %u\n", newNode.f, newNode.g);
			}
			case 3: {
				Node& newNode = stack[++sp];

				newNode = currentNode;
				newNode.f = currentNode.f + 1;
				newNode.blankPos = currentOp.nextBlankPos[2];
				newNode.prevBlankPos = currentNode.blankPos;
				// printf("generating: %u=>%u\n", currentNode.blankPos, newNode.blankPos);
				// printf("f: %u g: %u\n", newNode.f, newNode.g);
			}
			case 2: {
				Node& newNode = stack[++sp];

				newNode = currentNode;
				newNode.f = currentNode.f + 1;
				newNode.blankPos = currentOp.nextBlankPos[1];
				newNode.prevBlankPos = currentNode.blankPos;
				// printf("generating: %u=>%u\n", currentNode.blankPos, newNode.blankPos);
				// printf("f: %u g: %u\n", newNode.f, newNode.g);
			}
			default: {
				Node& newNode = stack[++sp];

				newNode = currentNode;
				newNode.f = currentNode.f + 1;
				newNode.blankPos = currentOp.nextBlankPos[0];
				newNode.prevBlankPos = currentNode.blankPos;
				// printf("generating: %u=>%u\n", currentNode.blankPos, newNode.blankPos);
				// printf("f: %u g: %u\n", newNode.f, newNode.g);
			}
		}

		// mark node as passed so it's not expanded again:
		currentNode.f = 0;
	}

	if (sp < 0)
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

	return maxF;
}
