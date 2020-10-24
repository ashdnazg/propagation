#include <algorithm>
#include <array>
#include <cassert>
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
#include "HashSet.h"
#include "IDAStar.h"
#include "IDAStarXor.h"
#include "IDAStarXor2.h"
#include "IDAStarSlim.h"
#include "IDAStarXorSlim.h"
#include "BDIDAH.h"
#include "BDIDAHREF.h"
//#include "BDIDAHREFAnalyzer.h"
#include "CA.h"

int main() {
	// const Pancakes start {
		// 21, 1, 28, 38, 3, 33, 13, 30, 23, 19, 39, 25, 11, 7, 18, 8, 37, 32, 15, 12, 27, 6, 20, 22, 4, 10, 17, 36, 2, 0, 5, 14, 16, 31, 34, 26, 29, 35, 24, 9
	// };
	// const Pancakes start {
		// 0, 3, 4, 1, 2
	// };
	// const Pancakes start {
		// 23, 21, 9, 33, 15, 42, 39, 19, 41, 43, 2, 47, 30, 27, 31, 46, 11, 20, 38, 44, 48, 0, 8, 25, 34, 3, 22, 12, 5, 7, 18, 24, 10, 14, 29, 45, 4, 37, 32, 40, 49, 35, 26, 28, 16, 17, 1, 13, 6, 36
	// };
	// const Pancakes start {
		// 5, 0, 3, 1, 2, 4
	// };

	//std::srand(std::time(0));
	// Pancakes start;
	// createRandomState(start);
	// printf("Start: ");
	// printState(start);
	// printf("\n");

	// unsigned result = CAPancakeSearcher::search(start);
	// printf("C* = %u\n", result);
	// printf("nodes generated = %llu\n", CAPancakeSearcher::generated);
	// printf("nodes memory used = %llu\n", CAPancakeSearcher::memoryUsed);
	// unsigned result2 = IDAStarPancakeSearcher::search(start);
	// printf("C*(2) = %u\n", result2);
	// printf("nodes generated(2) = %llu\n", IDAStarPancakeSearcher::generated);

	const int iterations = 20;
	long unsigned total1 = 0;
	long unsigned total2 = 0;
	for (unsigned i = 0; i < iterations; ++i) {
		printf("Instance %u\n", i);
		Pancakes p;// = { 35, 25, 6, 14, 31, 36, 24, 37, 34, 30, 38, 23, 20, 17, 27, 33, 45, 40, 12, 26, 48, 32, 41, 49, 18, 28, 7, 0, 2, 21, 13, 22, 19, 39, 29, 3, 42, 4, 43, 5, 16, 10, 44, 8, 46, 11, 15, 1, 47, 9, 50};
		createRandomState(p);
		printState(p);
		printf("\n");
		unsigned long before = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		unsigned result = IDAStarSlimPancakeSearcher::search(p);
		unsigned long after = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		printf("C* (IDASlim) = %u\n", result);
		printf("nodes generated (IDASlim) = %llu\n", IDAStarSlimPancakeSearcher::generated);
		printf("time (IDASlim): %lu\n", (unsigned long) (after - before));
		total1 += (after - before);

		unsigned long before2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		unsigned result2 = IDAStarXorSlimPancakeSearcher::search(p);
		unsigned long after2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		printf("C* (IDAXorSlim) = %u\n", result2);
		printf("nodes generated (IDAXorSlim) = %llu\n", IDAStarXorSlimPancakeSearcher::generated);
		//printf("nodes memory used (IDA) = %llu\n", IDAStarPancakeSearcher::memoryUsed);
		printf("time (IDAXorSlim): %lu\n", (unsigned long) (after2 - before2));
		total2 += (after2 - before2);

		if (result2 != result) {
			printf("ERRRORERRORERROR\n");
			exit(1);
		}
	}
	printf("total (IDASlim): %lu\n", total1);
	printf("total (IDAXorSlim): %lu\n", total2);
	return 0;
}
