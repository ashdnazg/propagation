#include <algorithm>
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
	OctileDomain domain(10,10);
	//BFSSearcher<GridDomain> searcher(&domain);
	//AStarSearcher<GridDomain, GridDomain::ManhattanDistance> searcher(&domain);
	//AStarSearcher<OctileDomain, OctileDomain::OctileDistance> searcher(&domain);
	MMSearcher<OctileDomain, OctileDomain::OctileDistance> searcher(&domain);
	float res = searcher.search(0,58);
	printf("%f %d %d\n", res, searcher.expanded, searcher.generated);
	return 0;
}