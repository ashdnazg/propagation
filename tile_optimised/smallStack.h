#pragma once

#define N 16
#define ROW_SIZE 4
#define MAX_SOLUTION 80
#define MAX_DESCENDANTS 4

class IDAStarTileSearcher
{
public:
	typedef unsigned char Tiles[N];
	struct Node {
		Tiles tiles;
		unsigned char g;
		unsigned char f;
	};
	struct Operator {
		unsigned num;
		unsigned char nextBlankPos[MAX_DESCENDANTS];
	};
	struct StackState {
		unsigned opIndex;
		unsigned char blankPos;
	};

	static unsigned DFS(const Tiles& start, unsigned& maxF);
	static unsigned search(const Tiles& start);


};

unsigned IDAStarTileSearcher::DFS(const Tiles& start, unsigned& maxF) {
	char hCache[N][N] = {{0}};
	Operator opCache[N][N] = {{0}}; //Sparse, but fast

	Node currentNode;
	StackState stack[MAX_SOLUTION + 10] = {{0}};

	int sp = 1;


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
			currentNode.tiles[pos] = tile;
		}
		stack[0].blankPos = blankPos;
		stack[0].opIndex = 0;
		stack[1].blankPos = blankPos;
		stack[0].opIndex = 0;
		currentNode.g = 0;
		currentNode.f = h;
	}

	if (maxF == 0)
		maxF = currentNode.f;

	while (sp > 0) {
		// current node is the goal
		if (currentNode.f == currentNode.g)
			break;

		StackState& ss = stack[sp];
		const unsigned char prevBlankPos = stack[sp - 1].blankPos;
		const unsigned char blankPos = ss.blankPos;
		const Operator& currentOp = opCache[prevBlankPos][blankPos];
		if (ss.opIndex == currentOp.num || currentNode.f > maxF) {
			--sp;
			const unsigned char tile = currentNode.tiles[prevBlankPos];
			currentNode.tiles[prevBlankPos] = 0;
			currentNode.tiles[blankPos] = tile;
			currentNode.g--;
			currentNode.f -= hCache[tile][prevBlankPos] + 1 - hCache[tile][blankPos];
		} else {
			StackState& ns = stack[++sp];
			const unsigned char nextBlankPos = currentOp.nextBlankPos[ss.opIndex++];
			ns.blankPos = nextBlankPos;
			ns.opIndex = 0;

			const unsigned char tile = currentNode.tiles[nextBlankPos];
			currentNode.tiles[nextBlankPos] = 0;
			currentNode.tiles[blankPos] = tile;
			currentNode.g++;
			currentNode.f += hCache[tile][blankPos] + 1 - hCache[tile][nextBlankPos];
		}

		// mark node as passed so it's not expanded again:
		//currentNode.f = maxF + 1;
	}

	if (sp == 0)
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
