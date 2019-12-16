#pragma once

class BlocksSearcher
{
public:
	static void DFS();
	static std::uint64_t generated;
	static std::uint64_t found;
};

static constexpr size_t STACK_SIZE = 2200;

void BlocksSearcher::DFS() {
	std::array<State, STACK_SIZE> orStack;
	std::array<State, STACK_SIZE> xorStack;
	std::array<unsigned, 65> pieceCache;
	std::array<unsigned, NUM_PIECES> esp;
	pieceCache.fill(0);
	{
		unsigned count = 0;
		for (unsigned p = 0; p < NUM_PIECES; ++p) {
			count += __builtin_popcountll(placements[placementIndices[p][0]]);
			pieceCache[count] = p + 1;
		}
	}

	orStack[0] = 0;
	xorStack[0] = 0;
	unsigned sp = 1;

	while (true) {
		if(--sp > STACK_SIZE) {
			break;
		}
		if (orStack[sp] != xorStack[sp]) {
			continue;
		}
		int piece = pieceCache[__builtin_popcountll(orStack[sp])];
		if (piece == NUM_PIECES) {
			++found;
			//printf("%llu, %llu, %u, %u, %u\n", orStack[sp], xorStack[sp], sp, piece, __builtin_popcountll(orStack[sp]));
			printf("%u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u\n", esp[0], esp[1], esp[2], esp[3], esp[4], esp[5], esp[6], esp[7], esp[8], esp[9], sp);
			continue;
		}
		esp[piece] = sp;
		const unsigned placementsStart = placementIndices[piece][0];
		const unsigned placementsCount = placementIndices[piece][1];
		std::fill(orStack.begin() + sp + 1, orStack.begin() + sp + placementsCount, orStack[sp]);
		std::fill(xorStack.begin() + sp + 1, xorStack.begin() + sp + placementsCount, xorStack[sp]);
		//printf("%llu, %llu, %u, %d, %u, %u\n", orStack[sp], xorStack[sp], sp, piece, __builtin_popcountll(orStack[sp]), solution[piece - 1]);
		for (unsigned i = 0; i < placementsCount; ++i) {
			orStack[sp + i] |= placements[placementsStart + i];
			xorStack[sp + i] ^= placements[placementsStart + i];
		}
		generated += placementsCount;
		sp += placementsCount;

		if (generated % 1000000000L < (generated - placementsCount) % 1000000000L) {
			printf("generated : %llu found: %llu\n", generated, found);
		}
	}
	printf("generated : %llu found: %llu\n", generated, found);
}


std::uint64_t BlocksSearcher::generated = 0;

std::uint64_t BlocksSearcher::found = 0;