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
		void remove(unsigned node);
		void clear();
		bool contains(unsigned node) const;
		std::vector<bool> bitField;
	};
	void getNeighbors(unsigned node, std::vector<Neighbor<unsigned, int>>& nodesVec) const;
	static bool same(unsigned node1, unsigned node2);

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
		void remove(unsigned node);
		void clear();
		bool contains(unsigned node) const;
		std::vector<bool> bitField;
	};
	void getNeighbors(unsigned node, std::vector<Neighbor<unsigned, float>>& nodesVec) const;
	static bool same(unsigned node1, unsigned node2);

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
	typedef std::uint64_t NodeType;
	typedef unsigned CostType;
	class List
	{
	public:
		void insert(std::uint64_t node);
		void remove(std::uint64_t node);
		void clear();
		bool contains(std::uint64_t node) const;
		std::vector<bool> bitField;
	};
	void getNeighbors(std::uint64_t node, std::vector<Neighbor<std::uint64_t, unsigned>>& nodesVec) const;
	static bool same(std::uint64_t node1, std::uint64_t node2);

	template<int M>
	struct GAP {
		static inline unsigned get(const PancakeDomain* d, std::uint64_t node1, std::uint64_t node2) {
			const std::uint64_t mask = 0xF;
			std::array<unsigned char, 13> legend;
			for (unsigned char pancake = 0; pancake < d->size; ++pancake) {
				const unsigned shift = pancake * 4;
				legend[(node1 >> shift) & mask] = pancake;
			}

			unsigned h = 0;
			unsigned char prevPancake = legend[(node2 & mask)];
			for (unsigned pancake = 1; pancake < d->size; ++pancake) {
				const unsigned shift = pancake * 4;
				unsigned char nextPancake = legend[((node2 & (mask << shift)) >> shift)];
				h += (prevPancake >= M) * ((nextPancake + 1u - prevPancake) > 2u); // unsigned values, so this checks if abs(next - prev) > 1
				prevPancake = nextPancake;
			}

			return h;
		}
	};
	struct Dummy {
		static inline unsigned get(const PancakeDomain* d, std::uint64_t node1, std::uint64_t node2) {
			return 0;
		}
	};

	static unsigned compressPancake(std::uint64_t node);
	static std::uint64_t compressCount;

	unsigned size;
};

class Tile16Domain
{
public:
	Tile16Domain();
	typedef std::uint64_t NodeType; //16 4 bit numbers, first 15 are the state and last one is bbRR where bb is the last direction of movement
	typedef unsigned CostType;
	class List
	{
	public:
		void insert(std::uint64_t node);
		void remove(std::uint64_t node);
		void clear();
		bool contains(std::uint64_t node) const;
		std::unordered_set<std::uint64_t> nodeSet;
	};

	struct ManhattanDistance {
		static inline int get(const Tile16Domain* d, std::uint64_t node1, std::uint64_t node2) {
			const std::uint64_t mask = 0xF;
			std::array<unsigned char, 16> legend;
			std::uint64_t lastTile = 0;

			for (unsigned char pos = 0; pos < 15; ++pos) {
				const unsigned shift = pos * 4;
				const std::uint64_t tile = (node1 >> shift) & mask;
				legend[tile] = pos;
				lastTile = lastTile + tile;
			}

			lastTile = (120 - lastTile) & mask;
			legend[lastTile] = 15;

			lastTile = 0;
			unsigned h = 0;
			for (unsigned char pos = 0; pos < 15; ++pos) {
				const unsigned shift = pos * 4;
				const std::uint64_t tile = (node2 >> shift) & mask;
				if (tile == 0)
					continue;

				const unsigned char targetPos = legend[tile];
				lastTile = lastTile + tile;
				h += std::abs((targetPos & 0x3) - (pos & 0x3));
				h += std::abs(((targetPos >> 2) & 0x3) - ((pos >> 2) & 0x3));
			}

			lastTile = (120 - lastTile) & mask;
			if (lastTile != 0) {
				const unsigned char pos = 15;
				const unsigned char targetPos = legend[lastTile];
				h += std::abs((targetPos & 0x3) - (pos & 0x3));
				h += std::abs(((targetPos >> 2) & 0x3) - ((pos >> 2) & 0x3));
			}

			return h;
		}
	};

	void getNeighbors(std::uint64_t node, std::vector<Neighbor<std::uint64_t, unsigned>>& nodesVec) const;
	static bool same(std::uint64_t node1, std::uint64_t node2);
};
