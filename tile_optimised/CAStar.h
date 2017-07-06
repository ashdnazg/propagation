#pragma once

class CAStarTileSearcher
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

	typedef HashSet<Tiles, TilesHasher, 1 << 31> myHashSet;
	typedef unsigned HCache[N][N];
	typedef Operator OPCache[N][N];

	static void initHCache(const Tiles& goal, HCache& hCache);
	static void initOPCache(OPCache& opCache);
	static unsigned CASearch(const Tiles& start, const Tiles& goal, unsigned& thF, unsigned& thB);
	static bool backwardSearch(const Tiles& start, const Tiles& goal, unsigned maxF, unsigned thB, myHashSet& hashSet, const OPCache& opCache);
	static unsigned search(const Tiles& start);
	static std::uint64_t generated;
	static std::uint64_t memoryUsed;
};

void CAStarTileSearcher::initHCache(const Tiles& goal, HCache& hCache) {
	memset(hCache, 0, N * N * sizeof(**hCache));

	Tiles legend;

	for (unsigned char pos = 0; pos < N; ++pos) {
		legend[goal[pos]] = pos;
	}

	for (unsigned i = 1; i < N; ++i) {
		char targetPos = legend[i];
		for (char pos = 0; pos < N; ++pos) {
			char dx = std::abs(pos % ROW_SIZE - targetPos % ROW_SIZE);
			char dy = std::abs(pos / ROW_SIZE - targetPos / ROW_SIZE);
			hCache[i][(unsigned char)pos] = dx + dy;
		}
	}
}

void CAStarTileSearcher::initOPCache(OPCache& opCache) {
	memset(opCache, 0, N * N * sizeof(**opCache));

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


bool CAStarTileSearcher::backwardSearch(const Tiles& start, const Tiles& goal, unsigned maxF, unsigned thB, myHashSet& hashSet, const OPCache& opCache) {
	HCache hCache;

	Node stack[MAX_SOLUTION * MAX_DESCENDANTS];
	unsigned blankPosStack[MAX_SOLUTION + 10];
	Tiles tiles;

	Node* sp = stack;
	unsigned* bpsp = blankPosStack;
	unsigned g = 0;

	initHCache(goal, hCache);


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
		if (g == thB) {
			if (hashSet.contains(tiles)) {
				return true;
			}
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
	return false;
}


unsigned CAStarTileSearcher::CASearch(const Tiles& start, const Tiles& goal, unsigned& thF, unsigned& thB) {
	HCache hCache;
	OPCache opCache; //Sparse, but fast
	myHashSet hashSet;

	Node stack[MAX_SOLUTION * MAX_DESCENDANTS];
	unsigned blankPosStack[MAX_SOLUTION + 10];
	Tiles tiles;
	Tiles commonAncestor;
	Tiles nextCommonAncestor;

	Node* sp = stack;
	unsigned* bpsp = blankPosStack;
	unsigned g = 0;
	int caDepth = -2;
	int nextCADepth = -2;


	initHCache(goal, hCache);

	initOPCache(opCache);


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

	if (thF == 0 && thB == 0) {
		thF = (sp->h + 1) / 2;
		thB = (sp->h) / 2;
	}

	unsigned maxF = thF + thB;

	while (sp >= stack) {
		Node& currentNode = *sp;
		const unsigned blankPos = currentNode.blankPos;

		if (currentNode.h == 0) {
			--bpsp;
			--sp;
			printf("unrolling: %u=>%u\n", currentNode.blankPos, tiles[*bpsp]);
			tiles[blankPos] = tiles[*bpsp];

			if (int(g) == nextCADepth) {
				nextCommonAncestor = tiles;
				nextCommonAncestor[*bpsp] = 0;
				--nextCADepth;
			}

			if ((nextCADepth < int(thF >> 1)) && !hashSet.isEmpty()) {
				printf("doing backward search with ca depth: %d\n", caDepth);
				const bool found = backwardSearch(goal, commonAncestor, maxF - caDepth, thB, hashSet, opCache);
				if (found) {
					return true;
				}
				nextCADepth = -2;
				caDepth = -2;
				memoryUsed = std::max(memoryUsed, hashSet.getCount());
				hashSet.clear();
			}

			--g;

			continue;
		}

		const unsigned prevBlankPos = *bpsp;
		const unsigned tile = tiles[blankPos];
		generated++;
		const unsigned h = currentNode.h + hCache[tile][prevBlankPos] - hCache[tile][blankPos];
		printf("expanding: %u=>%u\n", prevBlankPos, currentNode.blankPos);


		// current node is off limits
		if (g + h > maxF) {
			--sp;
			continue;
		}


		if (g == thF) {
			printf("match candidate: g: %u h: %u\n", g, h);
			tiles[prevBlankPos] = tile;
			tiles[blankPos] = 0;
			currentNode.h = 0;
			if (hashSet.isEmpty()) {
				nextCommonAncestor = tiles;
				nextCADepth = thF;
			}
			const bool isFull = hashSet.insert(tiles);
			commonAncestor = nextCommonAncestor;
			caDepth = nextCADepth;
			if (isFull) {
				printf("doing backward search with ca depth: %d\n", caDepth);
				const bool found = backwardSearch(goal, commonAncestor, maxF - caDepth, thB, hashSet, opCache);
				if (found) {
					return true;
				}
				nextCADepth = -2;
				caDepth = -2;
				memoryUsed = std::max(memoryUsed, hashSet.getCount());
				hashSet.clear();
			}
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
	return false;
}

unsigned CAStarTileSearcher::search(const Tiles& start) {
	Tiles goal {
		1, 2, 3, 4,
		5, 6, 7, 8,
		9,10,11,12,
		13,14,15,0
	};
	unsigned thF = 0;
	unsigned thB = 0;
	while (true) {
		bool found = CASearch(start, goal, thF, thB);

		if (found) {
			break;
		}

		++thF;
		++thB;
	}

	printf("generated: %llu\n", generated);
	return thF + thB;
}
std::uint64_t CAStarTileSearcher::generated = 0;
std::uint64_t CAStarTileSearcher::memoryUsed = 0;