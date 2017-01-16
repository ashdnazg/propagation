#include <algorithm>
#include <array>
#include <cassert>
#include <cinttypes>
#include <cstdio>
#include <deque>
#include <queue>
#include <unordered_set>
#include <vector>
#include "Domains.h"
#include "Queues.h"
#include "BFS.h"
#include "AStar.h"
#include "MM.h"
#include "IDAStar.h"
#include "MIDAS.h"

int main() {
	//GridDomain domain(10,10);
	const OctileDomain octile(10,10);
	const PancakeDomain pancake(10);
	const Tile16Domain tile;
	//BFSSearcher<GridDomain> searcher(&domain);
	//AStarSearcher<GridDomain, GridDomain::ManhattanDistance> searcher(&domain);
	// {
		// AStarSearcher<OctileDomain, OctileDomain::OctileDistance> searcher(&octile);
		// float res = searcher.search(0,58);
		// printf("%f %d %d\n", res, searcher.expanded, searcher.generated);
	// }
	// printf("\n");
	// {
		// MMSearcher<OctileDomain, OctileDomain::OctileDistance> searcher(&octile);
		// float res = searcher.search(0,58);
		// printf("%f %d %d\n", res, searcher.expanded, searcher.generated);
	// }
	// printf("\n");
	{
		// AStarSearcher<PancakeDomain, PancakeDomain::Dummy> searcher(&pancake);
		MMSearcher<PancakeDomain, PancakeDomain::Dummy> searcher(&pancake);
		// BFSSearcher<PancakeDomain> searcher(&pancake);
		unsigned res = searcher.search(0x9876543210ull,0x6084715293ull);
		printf("%u %d %d\n", res, searcher.expanded, searcher.generated);
		printf("compressCount: %llu\n", PancakeDomain::compressCount);
		// typedef typename PancakeDomain::NodeType DomainNode;
		// typedef typename PancakeDomain::CostType DomainCost;
		// typedef Neighbor<DomainNode, DomainCost> DomainNeighbor;
	}
	// {
		// typedef typename Tile16Domain::NodeType DomainNode;
		// typedef typename Tile16Domain::CostType DomainCost;
		// typedef Neighbor<DomainNode, DomainCost> DomainNeighbor;
		// std::vector<DomainNeighbor> neighbors;
		// tile.getNeighbors(0x0FEDCBA980654321, neighbors);
		// for (DomainNeighbor n: neighbors) {
			// printf("0x%016llx\n", n.node);
			// printf("h: %u\n", Tile16Domain::ManhattanDistance::get(&tile, n.node, 0x0FEDCBA980654321));
		// }
		// printf("\n");

		// //AStarSearcher<Tile16Domain, Tile16Domain::ManhattanDistance> searcher(&tile);
		// //MMSearcher<Tile16Domain, Tile16Domain::ManhattanDistance> searcher(&tile);
		// //unsigned res = searcher.search(0x005E7C92836D4FA1,0x0FEDCBA987654321); // 35
		// //MIDASSearcher<Tile16Domain, Tile16Domain::ManhattanDistance> searcher(&tile);
		// IDAStarSearcher<Tile16Domain, Tile16Domain::ManhattanDistance> searcher(&tile);
		// //unsigned res = searcher.search(0x06845273EABFD9C0,0x0FEDCBA987654321); // 80
		// unsigned res = searcher.search(0x00F63295A8B4D7EC,0x0FEDCBA987654321); // 80
		// //printf("%u %d %d\n", res, searcher.expanded, searcher.generated);
		// printf("%u\n", res);
		// printf("h: %u\n", Tile16Domain::ManhattanDistance::get(&tile, 0x005E7C92836D4FA1, 0x0FEDCBA987654321));
	// }
	return 0;
}
