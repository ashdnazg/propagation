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
#include "MM.h"
#include "IDAStar.h"
#include "MIDAS.h"

int main() {
	//GridDomain domain(10,10);
	//const OctileDomain octile(10,10);
	std::srand(std::time(0));
	typedef PancakeDomain<40> P;
	const P pancake;

	P::NodeType n1;
	P::NodeType n2;
	P::createRandomState(n1);
	P::createRandomState(n2);
	// {
		// MMSearcher<P, P::GAP> mmsearcher(pancake);
		// unsigned res = mmsearcher.search(n1,n2);
		// printf("%u %ull %ull\n", res, mmsearcher.expanded, mmsearcher.generated);
	// }
	{
		IDAStarSearcher<P, P::GAP> idasearcher(pancake);
		unsigned res = idasearcher.search(n1,n2);
		printf("%u %llu %llu\n", res, idasearcher.expanded, idasearcher.generated);
	}
	{
		AStarSearcher<P, P::GAP> asearcher(pancake);
		unsigned res = asearcher.search(n1,n2);
		printf("%u %llu %llu\n", res, asearcher.expanded, asearcher.generated);
	}
	return 0;
}
