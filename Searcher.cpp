#include <algorithm>
#include <array>
#include <cassert>
#include <cinttypes>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <deque>
#include <queue>
#include <unordered_set>
#include <vector>
#include "Domains.h"
#include "MurmurHash3.h"
#include "PancakeDomain.h"
#include "Queues.h"
#include "BFS.h"
#include "AStar.h"
#include "CAStar.h"
#include "MM.h"
#include "IDAStar.h"
#include "MIDAS.h"

int main() {
	//GridDomain domain(10,10);
	//const OctileDomain octile(10,10);
	std::srand(std::time(0));
	typedef PancakeDomain<10> P;
	typedef Tile16Domain T16;
	const P pancake;
	const T16 tiles;

	P::NodeType n1 = {{3, 9, 1, 7, 4, 5, 8, 0, 2, 6}, 0};
	// T16::NodeType n1; //= 0x0261C4E58F7BA930ULL;


	P::NodeType n2 = {{1, 7, 9, 3, 6, 2, 5, 8, 4, 0}, 0};
	// T16::NodeType n2; //= 0x0FEDCBA987654321ULL;

	for (int i=0; i < 1000; ++i) {
		printf("Instance %d\n", i);
		P::createRandomState(n1);
		P::printState(n1);
		printf("\n");
		P::createRandomState(n2);
		P::printState(n2);
		printf("\n");

		unsigned idares;
		unsigned cares;

		#define SEARCH(GAP_LEVEL)                                                                                                   \
		{                                                                                                                           \
			IDAStarSearcher<P, P::GAP<GAP_LEVEL>> idasearcher(pancake);                                                             \
			idares = idasearcher.search(n1,n2);                                                                                     \
			printf("IDA(" #GAP_LEVEL "): %u %llu %llu\n", idares, idasearcher.expanded, idasearcher.generated);                     \
		}                                                                                                                           \
		{                                                                                                                           \
			CAStarSearcher<P, P::GAP<GAP_LEVEL>> casearcher(pancake);                                                               \
			cares = casearcher.search(n1,n2);                                                                                       \
			printf("CA(" #GAP_LEVEL "): %u %llu %llu %llu\n", cares, casearcher.expanded, casearcher.generated, casearcher.memoryUsed);\
		}                                                                                                                           \
		if (idares != cares) {                                                                                                      \
			printf("Discrepancy found!\n");                                                                                         \
			P::printState(n1);                                                                                                      \
			printf("\n");                                                                                                           \
			P::printState(n2);                                                                                                      \
			printf("\n");                                                                                                           \
			exit(1);                                                                                                                \
		}

		SEARCH(0)
		SEARCH(1)
		SEARCH(2)
		SEARCH(3)
		SEARCH(4)
		SEARCH(5)
		SEARCH(6)
		SEARCH(7)
		SEARCH(8)
		SEARCH(9)
		SEARCH(10)
	}
	return 0;
}

