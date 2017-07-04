#include <algorithm>
#include <array>
#include <cassert>
#include <cinttypes>
#include <cstdio>
#include <cstring>
#include <unordered_set>
#include <vector>
#include "Domains.h"

GridDomain::GridDomain(unsigned xSize_, unsigned ySize_)
 : xSize(xSize_)
 , ySize(ySize_)
 , pathable(xSize * ySize, true)
 { }

void GridDomain::pushToVec(unsigned node, int cost, std::vector<Neighbor<unsigned, int>>& nodesVec) const
{
	if (pathable[node])
		nodesVec.push_back({node ,cost});
}
void GridDomain::getNeighbors(unsigned node, std::vector<Neighbor<unsigned, int>>& nodesVec) const
{
	unsigned x = node % ySize;
	if (x > 0) {
		pushToVec(node - 1, 1, nodesVec);
	}
	if (x < xSize - 1) {
		pushToVec(node + 1, 1, nodesVec);
	}
	if (node > xSize) {
		pushToVec(node - xSize, 1, nodesVec);
	}
	if (node < (ySize - 1) * xSize) {
		pushToVec(node + xSize, 1, nodesVec);
	}
}

bool GridDomain::same(unsigned node1, unsigned node2)
{
	return node1 == node2;
}

void GridDomain::List::insert(unsigned node)
{
	if (bitField.size() <= node)
		bitField.resize(node + 1, false);

	bitField[node] = true;
}

void GridDomain::List::remove(unsigned node)
{
	if (bitField.size() <= node)
		return;

	bitField[node] = false;
}

void GridDomain::List::clear()
{
	bitField.clear();
}


bool GridDomain::List::contains(unsigned node) const
{
	return (bitField.size() > node) && bitField[node];
}



OctileDomain::OctileDomain(unsigned xSize_, unsigned ySize_)
 : xSize(xSize_)
 , ySize(ySize_)
 , pathable(xSize * ySize, true)
 { }

void OctileDomain::pushToVec(unsigned node, float cost, std::vector<Neighbor<unsigned, float>>& nodesVec) const
{
	if (pathable[node])
		nodesVec.push_back({node, cost});
}
void OctileDomain::getNeighbors(unsigned node, std::vector<Neighbor<unsigned, float>>& nodesVec) const
{
	unsigned x = node % ySize;
	const bool bl = x > 0;
	const bool bu = node > xSize;
	const bool br = x < xSize - 1;
	const bool bd = x < (ySize - 1) * xSize;
	if (bl) {
		pushToVec(node - 1, 1.0f, nodesVec);
	}
	if (br) {
		pushToVec(node + 1, 1.0f, nodesVec);
	}
	if (bu) {
		pushToVec(node - xSize, 1.0f, nodesVec);
	}
	if (bd) {
		pushToVec(node + xSize, 1.0f, nodesVec);
	}
	if (bl && bu) {
		pushToVec(node - 1 - xSize, SQRT2 ,nodesVec);
	}
	if (bl && bd) {
		pushToVec(node - 1 + xSize, SQRT2 ,nodesVec);
	}
	if (br && bu) {
		pushToVec(node + 1 - xSize, SQRT2 ,nodesVec);
	}
	if (br && bd) {
		pushToVec(node + 1 + xSize, SQRT2 ,nodesVec);
	}
}

bool OctileDomain::same(unsigned node1, unsigned node2)
{
	return node1 == node2;
}

void OctileDomain::List::insert(unsigned node)
{
	if (bitField.size() <= node)
		bitField.resize(node + 1, false);

	bitField[node] = true;
}

void OctileDomain::List::remove(unsigned node)
{
	if (bitField.size() <= node)
		return;

	bitField[node] = false;
}

void OctileDomain::List::clear()
{
	bitField.clear();
}

bool OctileDomain::List::contains(unsigned node) const
{
	return (bitField.size() > node) && bitField[node];
}


Tile16Domain::Tile16Domain()
{ }

void Tile16Domain::getNeighbors(std::uint64_t node, std::vector<Neighbor<std::uint64_t, unsigned>>& nodesVec) const
{
	unsigned blankPos = 15;
	const std::uint64_t mask = 0xF;

	std::uint64_t lastTile = 0;


	// Reconstructing the original node
	for (unsigned pos = 0; pos < 15; ++pos) {
		const std::uint64_t tile = (node >> (pos * 4)) & mask;
		if (tile == 0) {
			blankPos = pos;
		} else {
			lastTile = lastTile + tile;
		}
	}
	lastTile = (120 - lastTile) & mask;
	std::uint64_t reconstructed = (node & 0x0FFFFFFFFFFFFFFF) + (lastTile << 60);
	std::uint64_t lastDir = (node >> 60);
	// printf("lastDir: 0x%016llx \n", lastDir);
	// printf("reconstructed: 0x%016llx \n", reconstructed);
	//printf("expanded: 0x%016llx\n", reconstructed);
	if (lastDir != 0x2 && ((blankPos & 0x3) != 0x0)) {
		unsigned movePos = blankPos - 1;
		std::uint64_t movedTile = reconstructed & (mask << (movePos * 4));
		std::uint64_t newNode = reconstructed - movedTile;
		newNode += (movedTile << 4);
		//printf("generated: 0x%016llx moving: %u\n", newNode, (unsigned) (movedTile >> (movePos * 4)));
		newNode = (newNode & 0x0FFFFFFFFFFFFFFF) | 0x1000000000000000;

		nodesVec.push_back({newNode, 1});
	}

	if (lastDir != 0x1 && ((blankPos & 0x3) != 0x3)) {
		unsigned movePos = blankPos + 1;
		std::uint64_t movedTile = reconstructed & (mask << (movePos * 4));
		std::uint64_t newNode = reconstructed - movedTile;
		newNode += (movedTile >> 4);
		//printf("generated: 0x%016llx moving: %u\n", newNode, (unsigned) (movedTile >> (movePos * 4)));
		newNode = (newNode & 0x0FFFFFFFFFFFFFFF) | 0x2000000000000000;
		nodesVec.push_back({newNode, 1});
	}

	if (lastDir != 0x4 && ((blankPos & 0xC) != 0x0)) {
		unsigned movePos = blankPos - 4;
		std::uint64_t movedTile = reconstructed & (mask << (movePos * 4));
		std::uint64_t newNode = reconstructed - movedTile;
		newNode += (movedTile << 16);
		//printf("generated: 0x%016llx moving: %u\n", newNode, (unsigned) (movedTile >> (movePos * 4)));
		newNode = (newNode & 0x0FFFFFFFFFFFFFFF) | 0x3000000000000000;
		nodesVec.push_back({newNode, 1});
	}

	if (lastDir != 0x3 && ((blankPos & 0xC) != 0xC)) {
		unsigned movePos = blankPos + 4;
		std::uint64_t movedTile = reconstructed & (mask << (movePos * 4));
		std::uint64_t newNode = reconstructed - movedTile;
		newNode += (movedTile >> 16);
		//printf("generated: 0x%016llx moving: %u\n", newNode, (unsigned) (movedTile >> (movePos * 4)));
		newNode = (newNode & 0x0FFFFFFFFFFFFFFF) | 0x4000000000000000;
		nodesVec.push_back({newNode, 1});
	}
}


bool Tile16Domain::same(std::uint64_t node1, std::uint64_t node2)
{
	return ((node1 ^ node2) & 0x0FFFFFFFFFFFFFFF) == 0;
}

void Tile16Domain::List::insert(std::uint64_t node)
{
	nodeSet.insert(node & 0x0FFFFFFFFFFFFFFF);
}

void Tile16Domain::List::remove(std::uint64_t node)
{
	nodeSet.erase(node & 0x0FFFFFFFFFFFFFFF);
}

void Tile16Domain::List::clear()
{
	nodeSet.clear();
}

bool Tile16Domain::List::contains(std::uint64_t node) const
{
	return (nodeSet.find(node & 0x0FFFFFFFFFFFFFFF) != nodeSet.end());
}
