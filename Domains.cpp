#include <algorithm>
#include <array>
#include <cinttypes>
#include <cstdio>
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

bool GridDomain::same(unsigned node1, unsigned node2) const
{
	return node1 == node2;
}

void GridDomain::List::insert(unsigned node)
{
	if (bitField.size() <= node)
		bitField.resize(node + 1, false);

	bitField[node] = true;
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

bool OctileDomain::same(unsigned node1, unsigned node2) const
{
	return node1 == node2;
}

void OctileDomain::List::insert(unsigned node)
{
	if (bitField.size() <= node)
		bitField.resize(node + 1, false);

	bitField[node] = true;
}

bool OctileDomain::List::contains(unsigned node) const
{
	return (bitField.size() > node) && bitField[node];
}



PancakeDomain::PancakeDomain(unsigned size_)
 : size(size_) {
	unsigned f = 1;
	factorials[0] = f;
	for (unsigned i = 1; i <= size; ++i) {
		f *= i;
		factorials[i] = f;
	}
 }

void PancakeDomain::getNeighbors(std::uint64_t node, std::vector<Neighbor<std::uint64_t, unsigned>>& nodesVec) const
{
	const std::uint64_t mask = 0xF;
	for (unsigned numFlipped = 2; numFlipped <= size; ++numFlipped) {
		std::uint64_t newNode = node;
		const unsigned maxShift = (numFlipped - 1) * 4;
		for (unsigned pancake = 0; pancake < numFlipped; ++pancake) {
			const unsigned shift = pancake * 4;
			const unsigned n = node & (mask << shift);
			newNode -= n;
			newNode += (n >> shift) << (maxShift - shift);
		}
		nodesVec.push_back({newNode, 1});
	}
}


unsigned PancakeDomain::compressPancake(std::uint64_t node)
{
	std::array<unsigned char, 13> positions;

	static const std::array<std::uint64_t, 13> add_masks {
		0x0,
		0x1,
		0x11,
		0x111,
		0x1111,
		0x11111,
		0x111111,
		0x1111111,
		0x11111111,
		0x111111111,
		0x1111111111,
		0x11111111111,
		0x111111111111
	};

	const std::uint64_t mask = 0xF;

	int hit_zero = 0;
	unsigned char pos = 0;
	while (true) {
		const unsigned shift = pos * 4;
		const unsigned pancake = ((node & (mask << shift)) >> shift);
		hit_zero += (pancake == 0);
		if (hit_zero > 1)
			break;

		positions[pancake] = pos;
		++pos;
	}

	std::uint64_t global_mask = ~std::uint64_t(0);
	std::uint64_t count_greater = 0; // bit mangling thing

	for (unsigned i = pos - 1; i > 0; --i) {
		const unsigned char p = positions[i];
		count_greater += add_masks[p] & global_mask;
		global_mask -= mask << p * 4;
	}

	unsigned compressed = 0;
	for (unsigned i = 0; i < pos; ++i) {
		const unsigned shift = (pos - i - 1) * 4;
		compressed *= i + 1;
		compressed += ((count_greater & (mask << shift)) >> shift);
	}
	// printf("%x compressed to %u from count: %x\n", (unsigned) node, compressed, (unsigned) count_greater);
	return compressed;
}

bool PancakeDomain::same(std::uint64_t node1, std::uint64_t node2) const
{
	return node1 == node2;
}

void PancakeDomain::List::insert(std::uint64_t node)
{
	const unsigned compressed = PancakeDomain::compressPancake(node);
	if (bitField.size() <= compressed)
		bitField.resize(compressed + 1, false);

	bitField[compressed] = true;
}

bool PancakeDomain::List::contains(std::uint64_t node) const
{
	const unsigned compressed = PancakeDomain::compressPancake(node);
	return (bitField.size() > compressed) && bitField[compressed];
}
