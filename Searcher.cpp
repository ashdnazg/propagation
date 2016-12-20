#include <algorithm>
#include <array>
#include <cassert>
#include <cinttypes>
#include <cstdio>
#include <deque>
#include <queue>
#include <vector>
#include "Domains.h"
#include "BFS.h"
#include "AStar.h"
#include "MM.h"

int main() {
	//GridDomain domain(10,10);
	const OctileDomain octile(10,10);
	const PancakeDomain pancake(4);
	//BFSSearcher<GridDomain> searcher(&domain);
	//AStarSearcher<GridDomain, GridDomain::ManhattanDistance> searcher(&domain);
	{
		AStarSearcher<OctileDomain, OctileDomain::OctileDistance> searcher(&octile);
		float res = searcher.search(0,58);
		printf("%f %d %d\n", res, searcher.expanded, searcher.generated);
	}
	printf("\n");
	{
		MMSearcher<OctileDomain, OctileDomain::OctileDistance> searcher(&octile);
		float res = searcher.search(0,58);
		printf("%f %d %d\n", res, searcher.expanded, searcher.generated);
	}
	printf("\n");
	{
		MMSearcher<PancakeDomain, PancakeDomain::GAP> searcher(&pancake);
		unsigned res = searcher.search(0x3120,0x0123);
		printf("%u %d %d\n", res, searcher.expanded, searcher.generated);
		// PancakeDomain::compressPancake(0x0123);
		// PancakeDomain::compressPancake(0x2130);
		// PancakeDomain::compressPancake(0x0213);
		// PancakeDomain::compressPancake(0x3210);
	}
	return 0;
}
