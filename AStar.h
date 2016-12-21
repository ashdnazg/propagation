#pragma once

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

	AStarSearcher(const D* domain_) : expanded(0), generated(0), domain(domain_), bestFound(std::numeric_limits<DomainCost>::max()) { }
	void reset(DomainNode start_, DomainNode goal_);
	bool isEmpty() const;
	bool expand(Node& best);
	void generate(DomainNode node, DomainCost distance);
	void generate(std::vector<DomainNeighbor>& nodesVec, DomainCost distance);
	DomainCost search(DomainNode start, DomainNode goal);

	unsigned expanded;
	unsigned generated;
private:
	DomainNode goal;
	typename D::List closedList;
	BucketQueue<Node, D> q;
	const D* domain;
	DomainCost bestFound;
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
	if (closedList.contains(node))
		return;

	DomainCost h = H::get(domain, node, goal);
	Node n = {node, distance, distance + h};
	if (n.f < bestFound) {
		if (D::same(node, goal)) {
			bestFound = std::min(n.g, bestFound);
		} else {
			q.push(n);
			++generated;
		}
	}
}

template <class D, class H>
void AStarSearcher<D,H>::generate(std::vector<DomainNeighbor>& nodesVec, DomainCost distance)
{
	for (const DomainNeighbor& n: nodesVec) {
		generate(n.node, distance + n.cost);
	}
}

template <class D, class H>
bool AStarSearcher<D,H>::expand(Node& best)
{
	best = q.top();
	q.pop();
	if (closedList.contains(best.domainNode))
		return false;

	++expanded;
	closedList.insert(best.domainNode);
	return true;
}


template<class D, class H>
typename AStarSearcher<D,H>::DomainCost AStarSearcher<D,H>::search(DomainNode start_, DomainNode goal_) {
	std::vector<DomainNeighbor> tempNodes;
	reset(start_, goal_);

	while (!q.empty()) {
		if (q.top().f >= bestFound)
			return bestFound;

		Node best;
		if (!expand(best))
			continue;

		tempNodes.clear();
		domain->getNeighbors(best.domainNode, tempNodes);
		generate(tempNodes, best.g);
	}
	return bestFound;
}
