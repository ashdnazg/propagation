#pragma once

#define SQRT2 1.41421356237f
#define SQRT2M1 0.41421356237f

template<typename T, typename P>
struct Neighbor
{
	T node;
	P cost;
};


class GridDomain
{
public:
	GridDomain(unsigned xSize_, unsigned ySize_);
	typedef unsigned NodeType;
	typedef int CostType;
	class List
	{
	public:
		void insert(unsigned node);
		bool contains(unsigned node) const;
		std::vector<bool> bitField;
	};
	void getNeighbors(unsigned node, std::vector<Neighbor<unsigned, int>>& nodesVec) const;
	bool same(unsigned node1, unsigned node2) const;

	struct ManhattanDistance {
		static inline int get(const GridDomain* d, unsigned node1, unsigned node2) {
			const unsigned dx = std::abs((int) (node1 % d->xSize) - (int) (node2 % d->xSize));
			const unsigned dy = std::abs((int) (node1 / d->xSize) - (int) (node2 / d->xSize));
			return dx + dy;
		}
	};

	void pushToVec(unsigned node, int cost, std::vector<Neighbor<unsigned, int>>& nodesVec) const;

	unsigned xSize;
	unsigned ySize;
	std::vector<bool> pathable;
};


class OctileDomain
{
public:
	OctileDomain(unsigned xSize_, unsigned ySize_);
	typedef unsigned NodeType;
	typedef float CostType;
	class List
	{
	public:
		void insert(unsigned node);
		bool contains(unsigned node) const;
		std::vector<bool> bitField;
	};
	void getNeighbors(unsigned node, std::vector<Neighbor<unsigned, float>>& nodesVec) const;
	bool same(unsigned node1, unsigned node2) const;

	struct OctileDistance {
		static inline float get(const OctileDomain* d, unsigned node1, unsigned node2) {
			const unsigned dx = std::abs((int)(node1 % d->xSize) - (int)(node2 % d->xSize));
			const unsigned dy = std::abs((int)(node1 / d->xSize) - (int)(node2 / d->xSize));
			const unsigned dMax = std::max(dx, dy);
			const unsigned dMin = std::min(dx, dy);
			return dMax + SQRT2M1 * dMin;
		}
	};

	void pushToVec(unsigned node, float cost, std::vector<Neighbor<unsigned, float>>& nodesVec) const;

	unsigned xSize;
	unsigned ySize;
	std::vector<bool> pathable;
};



class PancakeDomain
{
public:
	PancakeDomain(unsigned size_);
	typedef unsigned NodeType;
	typedef unsigned CostType;
	class List
	{
	public:
		void insert(unsigned node);
		bool contains(unsigned node) const;
		std::vector<bool> bitField;
	};
	void getNeighbors(unsigned node, std::vector<Neighbor<unsigned, unsigned>>& nodesVec) const;
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

private:
	std::vector<unsigned> factorials;
};
