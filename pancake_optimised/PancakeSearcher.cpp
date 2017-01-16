#include <algorithm>
#include <array>
#include <chrono>
#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <vector>
#include <unordered_set>
#include "MurmurHash3.h"
#include "Pancakes.h"
#include "BloomFilter.h"
#include "IDAStar.h"
#include "BDIDA.h"

int main() {
	const Pancakes start {
		3, 9, 2, 5, 1, 7, 4, 8, 0, 6
	};

	unsigned result = BDIDAStarPancakeSearcher::search(start);
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
