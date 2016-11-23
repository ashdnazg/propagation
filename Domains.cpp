#include <algorithm>
#include <vector>
#include "Domains.h"

GridDomain::GridDomain(unsigned xSize_, unsigned ySize_)
 : xSize(xSize_)
 , ySize(ySize_) { }

void GridDomain::getNeighbors(unsigned node, std::vector<unsigned>& nodesVec) const
{
	unsigned x = node % ySize;
	if (x > 0) {
		nodesVec.push_back(node - 1);
	}
	if (x < xSize - 1) {
		nodesVec.push_back(node + 1);
	}
	if (node > xSize) {
		nodesVec.push_back(node - xSize);
	}
	if (node < (ySize - 1) * xSize) {
		nodesVec.push_back(node + xSize);
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


PancakeDomain::PancakeDomain(unsigned xSize_, unsigned ySize_)
 : size(size_) { }

void PancakeDomain::getNeighbors(unsigned node, std::vector<unsigned>& nodesVec) const
{
	nodesVec.resize(size - 2, 0);
	for (int numFlipped = 2; numFlipped < size; ++numFlipped) {
		unsigned newNode = node;
		for (int pancake = 0; pancake < numFlipped) {
			newNode +=
		}

	}
}

bool PancakeDomain::same(unsigned node1, unsigned node2) const
{
	return node1 == node2;
}

void PancakeDomain::List::insert(unsigned node)
{
	if (bitField.size() <= node)
		bitField.resize(node + 1, false);

	bitField[node] = true;
}

bool PancakeDomain::List::contains(unsigned node) const
{
	return (bitField.size() > node) && bitField[node];
}