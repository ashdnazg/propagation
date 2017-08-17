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
	Pancakes start;
	createRandomState(start);
	printf("Start: ");
	printState(start);
	printf("\n");

	// unsigned result = CAPancakeSearcher::search(start);
	// printf("C* = %u\n", result);
	// printf("nodes generated = %llu\n", CAPancakeSearcher::generated);
	// printf("nodes memory used = %llu\n", CAPancakeSearcher::memoryUsed);
	// unsigned result2 = IDAStarPancakeSearcher::search(start);
	// printf("C*(2) = %u\n", result2);
	// printf("nodes generated(2) = %llu\n", IDAStarPancakeSearcher::generated);

	const int iterations = 1000;
	for (unsigned i = 0; i < iterations; ++i) {
		printf("Instance %u\n", i);
		Pancakes p;
		createRandomState(p);
		printState(p);
		printf("\n");
		unsigned long before = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		unsigned result = IDAStarPancakeSearcher::search(p);
		unsigned long after = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		printf("C* (IDA) = %u\n", result);
		printf("nodes generated (IDA) = %llu\n", IDAStarPancakeSearcher::generated);
		printf("time (IDA): %lu\n", (unsigned long) (after - before));

		unsigned long before2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		unsigned result2 = CAPancakeSearcher::search(p);
		unsigned long after2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		printf("C* = %u\n", result2);
		printf("nodes generated = %llu\n", CAPancakeSearcher::generated);
		printf("nodes memory used = %llu\n", CAPancakeSearcher::memoryUsed);
		if (result2 != result) {
			printf("ERRRORERRORERROR\n");
			exit(1);
		}

		printf("time: %lu\n", (unsigned long) (after2 - before2));
	}
	return 0;
}
