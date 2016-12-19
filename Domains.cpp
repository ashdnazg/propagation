#include <algorithm>
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



// PancakeDomain::PancakeDomain(unsigned xSize_, unsigned ySize_)
 // : size(size_) { }

// void PancakeDomain::getNeighbors(unsigned node, std::vector<unsigned>& nodesVec) const
// {
	// nodesVec.resize(size - 2, 0);
	// for (int numFlipped = 2; numFlipped < size; ++numFlipped) {
		// unsigned newNode = node;
		// for (int pancake = 0; pancake < numFlipped) {
			// newNode +=
		// }

	// }
// }

// bool PancakeDomain::same(unsigned node1, unsigned node2) const
// {
	// return node1 == node2;
// }

// void PancakeDomain::List::insert(unsigned node)
// {
	// if (bitField.size() <= node)
		// bitField.resize(node + 1, false);

	// bitField[node] = true;
// }

// bool PancakeDomain::List::contains(unsigned node) const
// {
	// return (bitField.size() > node) && bitField[node];
// }
