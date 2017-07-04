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
	typedef PancakeDomain<70> P;
	const P pancake;

	P::NodeType n1;// = {{4, 12, 15, 8, 14, 1, 9, 6, 16, 11, 7, 17, 2, 5, 18, 3, 19, 0, 10, 13}, 0};

	P::NodeType n2;// = {{3, 15, 5, 19, 8, 14, 9, 12, 17, 1, 0, 11, 13, 10, 16, 4, 6, 7, 2, 18}, 0};

	// for (int i=0; i < 1000; ++i) {
		// printf("Instance %d\n", i);
		P::createRandomState(n1);
		P::createRandomState(n2);

		unsigned idares;
		unsigned cares;

		// {
			// MMSearcher<P, P::GAP> mmsearcher(pancake);
			// unsigned res = mmsearcher.search(n1,n2);
			// printf("%u %ull %ull\n", res, mmsearcher.expanded, mmsearcher.generated);
		// }
		{
			IDAStarSearcher<P, P::GAP> idasearcher(pancake);
			idares = idasearcher.search(n1,n2);
			printf("%u %llu %llu\n", idares, idasearcher.expanded, idasearcher.generated);
		}
		// {
			// AStarSearcher<P, P::GAP> asearcher(pancake);
			// unsigned res = asearcher.search(n1,n2);
			// printf("%u %llu %llu\n", res, asearcher.expanded, asearcher.generated);
		// }
		{
			CAStarSearcher<P, P::GAP> casearcher(pancake);
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
