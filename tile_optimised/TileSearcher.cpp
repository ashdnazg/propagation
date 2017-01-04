#include <algorithm>
#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "Tiles.h"
#include "IDAStar.h"

std::uint64_t IDAStarTileSearcher::generated = 0;

int main() {
	std::srand(std::time(0));
	// const Tiles start {
		 // 0, 3, 9,10,
		// 11, 7,15, 8,
		 // 5,14, 4,12,
		 // 1, 6, 2,13
	// };

	// const Tiles start {
		 // 1, 2, 3,4,
		// 5, 6,7, 8,
		 // 9,0, 11,12,
		 // 13, 10,14,15
	// };

	// const Tiles start {
		 // 15, 11, 9,12,
		// 1, 2,6, 14,
		 // 4,8, 7,10,
		 // 3, 0, 13,5
	// };

	// unsigned result = IDAStarTileSearcher::search(start);
	// printf("C* = %u\n", result);
	// printf("generated: %llu\n", IDAStarTileSearcher::generated);
	for (unsigned i = 0; i < 10; ++i) {
		Tiles t;
		createRandomState(t);
		unsigned result = IDAStarTileSearcher::search(t);
		printf("C* = %u\n", result);
		printf("generated: %llu\n", IDAStarTileSearcher::generated);
		IDAStarTileSearcher::generated = 0;
	}
	return 0;
}
