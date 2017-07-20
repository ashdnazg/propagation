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

	//P::NodeType n1;// = {{4, 12, 15, 8, 14, 1, 9, 6, 16, 11, 7, 17, 2, 5, 18, 3, 19, 0, 10, 13}, 0};
	T16::NodeType n1; //= 0x0261C4E58F7BA930ULL;


	//P::NodeType n2;// = {{3, 15, 5, 19, 8, 14, 9, 12, 17, 1, 0, 11, 13, 10, 16, 4, 6, 7, 2, 18}, 0};
	T16::NodeType n2; //= 0x0FEDCBA987654321ULL;

	// for (int i=0; i < 1000; ++i) {
		// printf("Instance %d\n", i);
		T16::createRandomState(n1);
		T16::createRandomState(n2);

		unsigned idares;
		unsigned cares;

		// {
			// MMSearcher<P, P::GAP> mmsearcher(pancake);
			// unsigned res = mmsearcher.search(n1,n2);
			// printf("%u %ull %ull\n", res, mmsearcher.expanded, mmsearcher.generated);
		// }
		{
			IDAStarSearcher<T16, T16::ManhattanDistance> idasearcher(tiles);
			idares = idasearcher.search(n1,n2);
			printf("%u %llu %llu\n", idares, idasearcher.expanded, idasearcher.generated);
		}
		// {
			// AStarSearcher<P, P::GAP> asearcher(pancake);
			// unsigned res = asearcher.search(n1,n2);
			// printf("%u %llu %llu\n", res, asearcher.expanded, asearcher.generated);
		// }
		{
			CAStarSearcher<T16, T16::ManhattanDistance> casearcher(tiles);
			cares = casearcher.search(n1,n2);
			printf("%u %llu %llu\n", cares, casearcher.expanded, casearcher.generated);
		}
		// if (idares != cares) {
			// printf("Discrepancy found!\n");
			// P::printState(n1);
			// printf("\n");
			// P::printState(n2);
			// printf("\n");
			// exit(1);
		// }
	// }
	return 0;
}
