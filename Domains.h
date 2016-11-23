#pragma once

class GridDomain
{
public:
	GridDomain(unsigned xSize_, unsigned ySize_);
	typedef unsigned NodeType;
	class List
	{
	public:
		void insert(unsigned node);
		bool contains(unsigned node) const;
		std::vector<bool> bitField;
	};
	void getNeighbors(unsigned node, std::vector<unsigned>& nodesVec) const;
	bool same(unsigned node1, unsigned node2) const;

	struct ManhattanDistance {
		static inline int get(const GridDomain* d, unsigned node1, unsigned node2) {
			unsigned x1 = node1 % d->xSize;
			unsigned y1 = node1 / d->xSize;
			unsigned x2 = node2 % d->xSize;
			unsigned y2 = node2 / d->xSize;
			return std::abs((int)(x1 - x2)) + std::abs((int)(y1 - y2));
		}
	};

	unsigned xSize;
	unsigned ySize;
};

class PancakeDomain
{
public:
	PancakeDomain(unsigned size_);
	typedef unsigned NodeType;
	class List
	{
	public:
		void insert(unsigned node);
		bool contains(unsigned node) const;
		std::vector<bool> bitField;
	};
	void getNeighbors(unsigned node, std::vector<unsigned>& nodesVec) const;
	bool same(unsigned node1, unsigned node2) const;

	struct GAP {
		static inline int get(const GridDomain* d, unsigned node1, unsigned node2) {
			unsigned x1 = node1 % d->xSize;
			unsigned y1 = node1 / d->xSize;
			unsigned x2 = node2 % d->xSize;
			unsigned y2 = node2 / d->xSize;
			return std::abs((int)(x1 - x2)) + std::abs((int)(y1 - y2));
		}
	};

	unsigned size;
};