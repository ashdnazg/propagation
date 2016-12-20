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

	MMSearcher(const D* domain_) : expanded(0), generated(0), domain(domain_), bestFound(std::numeric_limits<DomainCost>::max()) { }
	void reset(DomainNode start_, DomainNode goal_);
	bool isEmpty() const;
	bool expand(Node& best);
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
	DomainNode start;
	DomainNode goal;
	typename D::List openList[2];
	typename D::List closedList[2];
	PriorityQueue<Node, std::vector<Node>, lessCost> q[2];
	const D* domain;
	DomainCost bestFound;
};

template <class D, class H>
void MMSearcher<D,H>::reset(DomainNode start_, DomainNode goal_)
{
	q[FORWARD].clear();
	q[BACK].clear();
	goal = goal_;
	start = start_;

	generate(start_, 0, FORWARD);
	generate(goal_, 0, BACK);
}

template <class D, class H>
void MMSearcher<D,H>::generate(DomainNode node, DomainCost distance, Direction dir)
{
	if (closedList[dir].contains(node))
		return;

	DomainCost h = H::get(domain, node, dir == FORWARD ? goal : start);
	Node n = {node, distance, std::max(distance + h, 2 * distance), dir};
	if (n.f < bestFound) {
		if (openList[!dir].contains(node)) {
			bool found = false;
			for (const Node& other: q[!dir].get()) {
				if (domain->same(other.domainNode, n.domainNode)) {
					found = true;
					bestFound = std::min(n.g + other.g, bestFound);
					break;
				}
			}
			assert(found);
		} else {
			openList[dir].insert(node);
			q[dir].push(n);
			++generated;
		}
	}
}

template <class D, class H>
void MMSearcher<D,H>::generate(std::vector<DomainNeighbor>& nodesVec, DomainCost distance, Direction dir)
{
	for (const DomainNeighbor& n: nodesVec) {
		generate(n.node, distance + n.cost, dir);
	}
}

template <class D, class H>
bool MMSearcher<D,H>::expand(Node& best)
{
	Direction d = (Direction) compareNodes(q[FORWARD].top(), q[BACK].top());
	best = q[d].top();
	q[d].pop();
	if (closedList[d].contains(best.domainNode))
		return false;

	++expanded;
	closedList[d].insert(best.domainNode);
	openList[d].remove(best.domainNode);
	return true;
}


template<class D, class H>
typename MMSearcher<D,H>::DomainCost MMSearcher<D,H>::search(DomainNode start_, DomainNode goal_) {
	std::vector<DomainNeighbor> tempNodes;
	reset(start_, goal_);

	while (!q[FORWARD].empty() && !q[BACK].empty()) {
		const float stopCond = std::max(q[FORWARD].top().f, q[BACK].top().f);
		if (stopCond >= bestFound - 0.01)
			return bestFound;
		Node best;
		if (!expand(best))
			continue;

		tempNodes.clear();
		domain->getNeighbors(best.domainNode, tempNodes);
		generate(tempNodes, best.g, best.dir);
	}
	return bestFound;
}
