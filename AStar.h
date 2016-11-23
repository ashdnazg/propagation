#pragma once

template <typename T, typename Cont, typename Comp>
class PriorityQueue: public std::priority_queue<T, Cont, Comp> {
public:
	void clear() { this->c.clear(); }
};

template <class D, class H>
class AStarSearcher
{
public:
	typedef typename D::NodeType DomainNode;
	typedef typename D::CostType DomainCost;
	typedef Neighbor<DomainNode, DomainCost> DomainNeighbor;

	struct Node {
		DomainNode domainNode;
		DomainCost g;
		DomainCost f;
	};

	AStarSearcher(const D* domain_) : expanded(0), generated(0), domain(domain_) { }
	void reset(DomainNode start_, DomainNode goal_);
	bool isEmpty() const;
	Node expand();
	void generate(DomainNode node, DomainCost distance);
	void generate(std::vector<DomainNeighbor>& nodesVec, DomainCost distance);
	DomainCost search(DomainNode start, DomainNode goal);

	// Functor for priority queue ordering
	struct lessCost: public std::binary_function<const Node&, const Node&, bool> {
		inline bool operator() (const Node& x, const Node& y) const {
			return x.f == y.f ? (x.g < y.g) : (x.f > y.f);
		}
	};

	unsigned expanded;
	unsigned generated;
private:
	DomainNode goal;
	typename D::List openList;
	typename D::List closedList;
	PriorityQueue<Node, std::vector<Node>, lessCost> q;
	const D* domain;
};

template <class D, class H>
void AStarSearcher<D,H>::reset(DomainNode start_, DomainNode goal_)
{
	q.clear();
	goal = goal_;

	generate(start_, 0);
}

template <class D, class H>
void AStarSearcher<D,H>::generate(DomainNode node, DomainCost distance)
{
	if (openList.contains(node) || closedList.contains(node))
		return;

	++generated;
	openList.insert(node);
	DomainCost h = H::get(domain, node, goal);
	Node n = {node, distance, distance + h};
	q.push(n);
}

template <class D, class H>
void AStarSearcher<D,H>::generate(std::vector<DomainNeighbor>& nodesVec, DomainCost distance)
{
	for (const DomainNeighbor& n: nodesVec) {
		generate(n.node, distance + n.cost);
	}
}

template <class D, class H>
typename AStarSearcher<D,H>::Node AStarSearcher<D,H>::expand()
{
	++expanded;
	Node best = q.top();
	q.pop();
	closedList.insert(best.domainNode);
	return best;
}


template<class D, class H>
typename AStarSearcher<D,H>::DomainCost AStarSearcher<D,H>::search(DomainNode start_, DomainNode goal_) {
	std::vector<DomainNeighbor> tempNodes;
	reset(start_, goal_);

	while (!q.empty()) {
		Node best = expand();

		if (domain->same(best.domainNode, goal))
			return best.g;

		tempNodes.clear();
		domain->getNeighbors(best.domainNode, tempNodes);
		generate(tempNodes, best.g);
	}
	return -1;
}