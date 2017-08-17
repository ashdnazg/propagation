#pragma once


template<int N>
class PancakeDomain
{
public:
	PancakeDomain();
	typedef struct {
		std::array<unsigned char, N> pancakes;
		unsigned lastFlip;
	} NodeType;
	typedef unsigned CostType;

	struct Hasher {
		static std::uint64_t get(const std::array<unsigned char, N>& pancakes, int n) {
			std::uint64_t out[2];
			MurmurHash3_x64_128(&pancakes, N, n * 12582917, out);

			return out[n % 2];
		}

		size_t operator()(const std::array<unsigned char, N>& pancakes) const {
			return get(pancakes, 0);
		}
	};


	class List {
	public:
		void insert(const NodeType& node);
		void erase(const NodeType& node);
		void clear();
		bool contains(const NodeType& node) const;
		bool empty() const;
		size_t size() const;
		std::unordered_set<std::array<unsigned char, N>, Hasher> nodeSet;
	};
	void getNeighbors(const NodeType& node, std::vector<Neighbor<NodeType, CostType>>& nodesVec) const;
	static bool same(const NodeType& node1, const NodeType& node2);

	static void createRandomState(NodeType& node);
	static void printState(const NodeType& node);

	template<int M>
	struct GAP {
		static inline unsigned get(const PancakeDomain& d, const NodeType& node1, const NodeType& node2) {
			unsigned h = N - M;
			std::array<unsigned char, N> legend;

			for (unsigned pos = 0; pos < N; ++pos) {
				legend[node2.pancakes[pos]] = pos;
			}

			for (int pos = 0; pos < N - 1; ++pos) {
				const unsigned char p1 = legend[node1.pancakes[pos]];
				const unsigned char p2 = legend[node1.pancakes[pos + 1]];

				if (std::min(p1,p2) < M)
					continue;

				h -= d.hCache[p1][p2];
			}
			if (M < N)
				h -= node1.pancakes[N - 1] == node2.pancakes[N - 1];

			return h;
		}
	};
	struct Dummy {
		static inline unsigned get(const PancakeDomain& d, const NodeType& node1, const NodeType& node2) {
			return 0;
		}
	};
	unsigned hCache[N][N];
	unsigned size;
};



template<int N>
void PancakeDomain<N>::createRandomState(NodeType& node) {
	for (unsigned i = 0; i < N; ++i) {
		node.pancakes[i] = i;
	}

	for (int i = N - 1; i >= 0; --i) {
		std::swap(node.pancakes[i], node.pancakes[std::rand() % (i + 1)]);
	}
	node.lastFlip = 0;
}

template<int N>
void PancakeDomain<N>::printState(const NodeType& node) {
	for (unsigned i = 0; i < N; ++i) {
		printf("%u, ", node.pancakes[i]);
	}
}



template<int N>
PancakeDomain<N>::PancakeDomain()
{
	memset(hCache, 0, N * N * sizeof(**hCache));

	for (int pos = 0; pos < N - 1; ++pos) {
		hCache[pos][pos + 1] = 1;
		hCache[pos + 1][pos] = 1;
	}
}


template<int N>
void PancakeDomain<N>::getNeighbors(const NodeType& node, std::vector<Neighbor<NodeType, CostType>>& nodesVec) const
{
	nodesVec.reserve(N);
	for (unsigned i = 2; i <= N; ++i) {
		if (i == node.lastFlip) {
			continue;
		}

		NodeType newNode;
		for (unsigned j = 0; j < i; ++j) {
			newNode.pancakes[j] = node.pancakes[i - j - 1];
		}
		for (unsigned j = i; j < N; ++j) {
			newNode.pancakes[j] = node.pancakes[j];
		}
		newNode.lastFlip = i;
		nodesVec.push_back({newNode, 1});
	}
}

template<int N>
bool PancakeDomain<N>::same(const NodeType& node1, const NodeType& node2)
{
	return node1.pancakes == node2.pancakes;
}

template<int N>
void PancakeDomain<N>::List::insert(const NodeType& node)
{
	nodeSet.insert(node.pancakes);
}

template<int N>
void PancakeDomain<N>::List::erase(const NodeType& node)
{
	nodeSet.erase(node.pancakes);
}

template<int N>
void PancakeDomain<N>::List::clear()
{
	nodeSet.clear();
}

template<int N>
bool PancakeDomain<N>::List::contains(const NodeType& node) const
{
	return (nodeSet.find(node.pancakes) != nodeSet.end());
}

template<int N>
bool PancakeDomain<N>::List::empty() const
{
	return nodeSet.empty();
}

template<int N>
size_t PancakeDomain<N>::List::size() const
{
	return nodeSet.size();
}