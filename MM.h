#pragma once

template <class D, class H>
class MMSearcher
{
public:
	typedef typename D::NodeType DomainNode;
	typedef typename D::CostType DomainCost;
	typedef Neighbor<DomainNode, DomainCost> DomainNeighbor;

	enum Direction {
		FORWARD = 0,
		BACK = 1
	};

	struct Node {
		DomainNode domainNode;
		DomainCost g;
		DomainCost f;
		Direction dir;
	};

	MMSearcher(const D* domain_) : expanded(0), generated(0), domain(domain_) { }
	void reset(DomainNode start_, DomainNode goal_);
	bool isEmpty() const;
	Node expand();
	void generate(DomainNode node, DomainCost distance, Direction dir);
	void generate(std::vector<DomainNeighbor>& nodesVec, DomainCost distance, Direction dir);
	DomainCost search(DomainNode start, DomainNode goal);
	static inline bool compareNodes(const Node& x, const Node& y) {
		return x.f == y.f ? (x.g < y.g) : (x.f > y.f);
	}
	// Functor for priority queue ordering
	struct lessCost: public std::binary_function<const Node&, const Node&, bool> {
		inline bool operator() (const Node& x, const Node& y) const {
			return compareNodes(x, y);
		}
	};

	unsigned expanded;
	unsigned generated;
private:
	DomainNode goal;
	typename D::List openList[2];
	typename D::List closedList[2];
	PriorityQueue<Node, std::vector<Node>, lessCost> q[2];
	const D* domain;
};

template <class D, class H>
void MMSearcher<D,H>::reset(DomainNode start_, DomainNode goal_)
{
	q[FORWARD].clear();
	q[BACK].clear();
	goal = goal_;

	generate(start_, 0, FORWARD);
	generate(goal_, 0, BACK);
}

template <class D, class H>
void MMSearcher<D,H>::generate(DomainNode node, DomainCost distance, Direction dir)
{
	if (openList[dir].contains(node) || closedList[dir].contains(node))
		return;

	++generated;
	openList[dir].insert(node);
	DomainCost h = H::get(domain, node, goal);
	Node n = {node, distance, distance + h, dir};
	q[dir].push(n);
}

template <class D, class H>
void MMSearcher<D,H>::generate(std::vector<DomainNeighbor>& nodesVec, DomainCost distance, Direction dir)
{
	for (const DomainNeighbor& n: nodesVec) {
		generate(n.node, distance + n.cost, dir);
	}
}

template <class D, class H>
typename MMSearcher<D,H>::Node MMSearcher<D,H>::expand()
{
	++expanded;
	Direction d = (Direction) compareNodes(q[FORWARD].top(), q[BACK].top());
	Node best = q[d].top();
	q[d].pop();
	closedList[d].insert(best.domainNode);
	return best;
}


template<class D, class H>
typename MMSearcher<D,H>::DomainCost MMSearcher<D,H>::search(DomainNode start_, DomainNode goal_) {
	std::vector<DomainNeighbor> tempNodes;
	reset(start_, goal_);

	while (!q[FORWARD].empty() && !q[BACK].empty()) {
		Node best = expand();

		if (closedList[!best.dir].contains(best.domainNode))
			return best.g;

		tempNodes.clear();
		domain->getNeighbors(best.domainNode, tempNodes);
		generate(tempNodes, best.g, best.dir);
	}
	return -1;
}
