#include <algorithm>
#include <cstdio>
#include <deque>
#include <queue>
#include <vector>
#include "Domains.h"
#include "BFS.h"
#include "AStar.h"

int main() {
	GridDomain domain(10,10);
	AStarSearcher<GridDomain, GridDomain::ManhattanDistance> searcher(&domain);
	int res = searcher.search(0,58);
	printf("%d %d %d\n", res, searcher.expanded, searcher.generated);
	return 0;
}