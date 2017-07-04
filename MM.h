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

	MMSearcher(const D& domain_) : expanded(0), generated(0), domain(domain_) { }
	void reset(const DomainNode& start_, const DomainNode& goal_);
	bool expand(Node& best);
	void generate(const DomainNode& node, DomainCost distance, Direction dir);
	void generate(std::vector<DomainNeighbor>& nodesVec, DomainCost distance, Direction dir);
	DomainCost search(const DomainNode& start, const DomainNode& goal);
	static inline bool compareNodes(const Node& x, const Node& y) {
		return x.f == y.f ? (x.g < y.g) : (x.f > y.f);
	}

	std::uint64_t expanded;
	std::uint64_t generated;
private:
	DomainNode start;
	DomainNode goal;
	typename D::List openList[2];
	typename D::List closedList[2];
	BucketQueue<Node, D> q[2];
	DomainCost fMin[2];
	const D& domain;
	DomainCost bestFound;
};

template <class D, class H>
void MMSearcher<D,H>::reset(const DomainNode& start_, const DomainNode& goal_)
{
	q[FORWARD].clear();
	q[BACK].clear();
	goal = goal_;
	start = start_;
	fMin[FORWARD] = 0;
	fMin[BACK] = 0;
	expanded = 0;
	generated = 0;
	bestFound = std::numeric_limits<DomainCost>::max();

	generate(start_, 0, FORWARD);
	generate(goal_, 0, BACK);
}

template <class D, class H>
void MMSearcher<D,H>::generate(const DomainNode& node, DomainCost distance, Direction dir)
{
	if (closedList[dir].contains(node))
		return;

	DomainCost h = H::get(domain, dir == FORWARD ? node : start, dir == FORWARD ? goal : node);
	Node n = {node, distance, distance + std::max(h, distance), dir};
	if (n.f < bestFound && n.f >= fMin[dir]) {
		if (openList[!dir].contains(node)) {
			bestFound = std::min(n.g + q[!dir].getMinG(n), bestFound);
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

	if (best.f > fMin[d]) {
		printf("new f: %u in dir: %d\n", best.f, d);
		fMin[d] = best.f;
		closedList[d].clear();
	}

	++expanded;
	closedList[d].insert(best.domainNode);
	openList[d].erase(best.domainNode);
	return true;
}


template<class D, class H>
typename MMSearcher<D,H>::DomainCost MMSearcher<D,H>::search(const DomainNode& start_, const DomainNode& goal_) {
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
		domain.getNeighbors(best.domainNode, tempNodes);
		generate(tempNodes, best.g, best.dir);
	}
	return bestFound;
}
