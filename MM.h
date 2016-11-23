#pragma once

template <typename T, typename Cont, typename Comp>
class PriorityQueue: public std::priority_queue<T, Cont, Comp> {
public:
	void clear() { this->c.clear(); }
};

template <class D, class H>
class MMSearcher
{
public:
	typedef typename D::NodeType DomainNode;

	struct Node {
		DomainNode domainNode;
		int g;
		int f;
	};

	MMSearcher(const D* domain_) : expanded(0), generated(0), domain(domain_) { }
	void reset(DomainNode start_, DomainNode goal_);
	bool isEmpty() const;
	Node expand();
	void generate(DomainNode node, int distance);
	void generate(std::vector<DomainNode>& nodesVec, int distance);
	int search(DomainNode start, DomainNode goal);

	// Functor for priority queue ordering
	struct lessCost: public std::binary_function<const Node&, const Node&, bool> {
		inline bool operator() (const Node& x, const Node& y) const {
			return (x.f ==  y.f) ? (x.g < y.g) : (x.f > y.f);
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
void MMSearcher<D,H>::reset(MMSearcher<D,H>::DomainNode start_, MMSearcher<D,H>::DomainNode goal_)
{
	q.clear();
	goal = goal_;

	generate(start_, 0);
}

template <class D, class H>
void MMSearcher<D,H>::generate(MMSearcher<D,H>::DomainNode node, int distance)
{
	if (openList.contains(node) || closedList.contains(node))
		return;

	++generated;
	openList.insert(node);
	int h = H::get(domain, node, goal);
	Node n = {node, distance, distance + h};
	q.push(n);
}

template <class D, class H>
void MMSearcher<D,H>::generate(std::vector<MMSearcher<D,H>::DomainNode>& nodesVec, int distance)
{
	for (const DomainNode& node: nodesVec) {
		generate(node, distance);
	}
}

template <class D, class H>
typename MMSearcher<D,H>::Node MMSearcher<D,H>::expand()
{
	++expanded;
	Node best = q.top();
	q.pop();
	closedList.insert(best.domainNode);
	return best;
}


template<class D, class H>
int MMSearcher<D,H>::search(MMSearcher<D,H>::DomainNode start_, MMSearcher<D,H>::DomainNode goal_) {
	std::vector<DomainNode> tempNodes;
	reset(start_, goal_);

	while (!q.empty()) {
		Node best = expand();

		if (domain->same(best.domainNode, goal))
			return best.g;

		tempNodes.clear();
		domain->getNeighbors(best.domainNode, tempNodes);
		generate(tempNodes, best.g + 1);
	}
	return -1;
}