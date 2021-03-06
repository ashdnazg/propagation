#pragma once

template <class D, class H>
class IDAStarSearcher
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

	IDAStarSearcher(const D& domain_) : expanded(0), generated(0), domain(domain_) { }
	void reset(DomainNode start_, DomainNode goal_);

	DomainCost DFS(const Node& root);
	DomainCost search(DomainNode start, DomainNode goal);

	std::uint64_t expanded;
	std::uint64_t generated;
private:
	DomainNode goal;
	const D& domain;
	DomainCost currentF;
};

template <class D, class H>
void IDAStarSearcher<D,H>::reset(DomainNode start_, DomainNode goal_)
{
	goal = goal_;
	currentF = H::get(domain, start_, goal_);
	expanded = 0;
	generated = 0; // the start is always generated
}

template<class D, class H>
typename IDAStarSearcher<D,H>::DomainCost IDAStarSearcher<D,H>::DFS(const Node& root) {
	DomainCost result = std::numeric_limits<DomainCost>::max();
	std::vector<DomainNeighbor> nodesVec;
	expanded++;
	// printf("expanding: ");
	// D::printState(root.domainNode);
	// printf("\n");
	domain.getNeighbors(root.domainNode, nodesVec);
	for (const DomainNeighbor& n: nodesVec) {
		DomainCost h = H::get(domain, n.node, goal);
		Node neighbor = {n.node, root.g + n.cost, root.g + n.cost + h};
		generated++;
		// printf("generating: ");
		// D::printState(n.node);
		// printf("in depth: %u\n", (unsigned) root.g + n.cost);
		DomainCost tempResult;
		if (neighbor.f > currentF) {
			tempResult = neighbor.f;
		} else if (D::same(n.node, goal)) {
			// printf("solution node:");
			// D::printState(n.node);
			// printf("in depth: %u\n", (unsigned) root.g + n.cost);
			tempResult = neighbor.g;
		} else {
			tempResult = DFS(neighbor);
		}
		if (tempResult == currentF) {
			// printf("solution node:");
			// D::printState(n.node);
			// printf("in depth: %u\n", (unsigned) root.g + n.cost);
			return currentF;
		}
		result = std::min(result, tempResult);
	}
	return result;
}

template<class D, class H>
typename IDAStarSearcher<D,H>::DomainCost IDAStarSearcher<D,H>::search(DomainNode start_, DomainNode goal_) {
	reset(start_, goal_);
	Node root = {start_, 0, currentF};
	++generated;
	while (true) {
		// printf("currentF: %u\n", currentF);
		DomainCost result = DFS(root);
		if (result == currentF || result == std::numeric_limits<DomainCost>::max())
			return result;

		currentF = result;
	}
	return std::numeric_limits<DomainCost>::max();
}
