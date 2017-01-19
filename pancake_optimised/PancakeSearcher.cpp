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
#include "BDIDAH.h"

int main() {
	// const Pancakes start {
		// 21, 1, 28, 38, 3, 33, 13, 30, 23, 19, 39, 25, 11, 7, 18, 8, 37, 32, 15, 12, 27, 6, 20, 22, 4, 10, 17, 36, 2, 0, 5, 14, 16, 31, 34, 26, 29, 35, 24, 9
	// };
	// const Pancakes start {
		// 0, 3, 4, 1, 2
	// };
	std::srand(std::time(0));
	Pancakes start;
	createRandomState(start);
	printf("Start: ");
	printState(start);
	printf("\n");

	unsigned result = BDIDAStarPancakeSearcher::search(start);
	printf("C* = %u\n", result);


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
