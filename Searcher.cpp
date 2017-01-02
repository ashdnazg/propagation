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

int main() {
	//GridDomain domain(10,10);
	const OctileDomain octile(10,10);
	const PancakeDomain pancake(11);
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
	//{
		//AStarSearcher<PancakeDomain, PancakeDomain::GAP<9>> searcher(&pancake);
		//MMSearcher<PancakeDomain, PancakeDomain::GAP<0>> searcher(&pancake);
		//BFSSearcher<PancakeDomain> searcher(&pancake);
		//unsigned res = searcher.search(0x0123456789Aull,0x492A8075316ull);
		//printf("%u %d %d\n", res, searcher.expanded, searcher.generated);
		//printf("compressCount: %llu\n", PancakeDomain::compressCount);
		// typedef typename PancakeDomain::NodeType DomainNode;
		// typedef typename PancakeDomain::CostType DomainCost;
		// typedef Neighbor<DomainNode, DomainCost> DomainNeighbor;
		// std::vector<DomainNeighbor> neighbors;
		// pancake.getNeighbors(0x428075316, neighbors);
		// printf("Neighbors of 0x%09llx : ", 0x428075316ull);
		// for (DomainNeighbor n: neighbors) {
			// printf("0x%09llx ", n.node);
		// }
		// printf("\n");
		// unsigned h = PancakeDomain::GAP::get(&pancake, 0x012345678ull,0x428075316ull);
		// printf("h: %u\n", h);
		// PancakeDomain::compressPancake(0x0123);
		// PancakeDomain::compressPancake(0x2130);
		// PancakeDomain::compressPancake(0x0213);
		// PancakeDomain::compressPancake(0x3210);
	//}
	{
		typedef typename Tile16Domain::NodeType DomainNode;
		typedef typename Tile16Domain::CostType DomainCost;
		typedef Neighbor<DomainNode, DomainCost> DomainNeighbor;
		std::vector<DomainNeighbor> neighbors;
		tile.getNeighbors(0x0FEDCBA980654321, neighbors);
		for (DomainNeighbor n: neighbors) {
			printf("0x%016llx\n", n.node);
			printf("h: %u\n", Tile16Domain::ManhattanDistance::get(&tile, n.node, 0x0FEDCBA980654321));
		}
		printf("\n");

		//AStarSearcher<Tile16Domain, Tile16Domain::ManhattanDistance> searcher(&tile);
		MMSearcher<Tile16Domain, Tile16Domain::ManhattanDistance> searcher(&tile);
		unsigned res = searcher.search(0x005E7C92836D4FA1,0x0FEDCBA987654321);
		printf("%u %d %d\n", res, searcher.expanded, searcher.generated);
		printf("h: %u\n", Tile16Domain::ManhattanDistance::get(&tile, 0x005E7C92836D4FA1, 0x0FEDCBA987654321));
	}
	return 0;
}
