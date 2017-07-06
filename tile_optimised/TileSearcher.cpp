#include <algorithm>
#include <array>
#include <chrono>
#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <unordered_set>
#include <vector>
#include "MurmurHash3.h"
#include "Tiles.h"
#include "HashSet.h"
//#include "MIDAS.h"
#include "IDAStar.h"
#include "CAStar.h"

int main() {
	const Tiles start {
		 0, 3, 9,10,
		11, 7,15, 8,
		 5,14, 4,12,
		 1, 6, 2,13
	};

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

	unsigned result = CAStarTileSearcher::search(start);
	printf("C* = %u\n", result);


	std::srand(std::time(0));
	// for (unsigned i = 0; i < 1000; ++i) {
		// Tiles t;
		// createRandomState(t);
		// unsigned long before = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		// unsigned result = IDAStarTileSearcher::search(t);
		// unsigned long after = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		// printf("C* = %u\n", result);
		// printf("generated: %llu\n", IDAStarTileSearcher::generated);
		// printf("time: %lu\n", (unsigned long) (after - before));
		// IDAStarTileSearcher::generated = 0;
	// }
	return 0;
}
