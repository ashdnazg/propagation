#pragma once

template <class D, class H>
class MIDASSearcher
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

	MIDASSearcher(const D* domain_) : domain(domain_) { }
	void reset(DomainNode start_, DomainNode goal_);

	DomainCost DFS(const Node& root);
	DomainCost search(DomainNode start, DomainNode goal);

private:
	std::vector<std::uint64_t> hCounts;
	DomainNode goal;
	DomainNode start;
	const D* domain;
	DomainCost currentF;
};

template <class D, class H>
void MIDASSearcher<D,H>::reset(DomainNode start_, DomainNode goal_)
{
	goal = goal_;
	start = start_;
	currentF = H::get(domain, start_, goal_);
}

template<class D, class H>
typename MIDASSearcher<D,H>::DomainCost MIDASSearcher<D,H>::DFS(const Node& root) {
	DomainCost result = std::numeric_limits<DomainCost>::max();
	std::vector<DomainNeighbor> nodesVec;
	assert(root.g + root.g <= currentF);
	domain->getNeighbors(root.domainNode, nodesVec);
	for (const DomainNeighbor& n: nodesVec) {
		DomainCost h = H::get(domain, n.node, goal);
		DomainCost h2 = H::get(domain, start, n.node);
		DomainCost g = root.g + n.cost;

		Node neighbor = {n.node, g, g + std::max(g, h)};
		DomainCost tempResult;
		if (g + g == currentF && g >= h)
			hCounts[h * (currentF + 1) + h2]++;

		if (neighbor.f > currentF) {
			tempResult = neighbor.f;
		} else if (D::same(n.node, goal)) {
			tempResult = neighbor.g;
		} else {
			tempResult = DFS(neighbor);
		}
		result = std::min(result, tempResult);
	}
	return result;
}

template<class D, class H>
typename MIDASSearcher<D,H>::DomainCost MIDASSearcher<D,H>::search(DomainNode start_, DomainNode goal_) {
	std::vector<DomainNeighbor> tempNodes;
	reset(start_, goal_);
	Node root = {start_, 0, currentF};
	while (true) {
		hCounts.clear();
		hCounts.resize((currentF + 1) * (currentF + 1), 0);
		DomainCost result = DFS(root);
		// if (result == currentF || result == std::numeric_limits<DomainCost>::max())
			// return result;

		for (unsigned i = 0; i <= currentF; ++i) {
			for (unsigned j = 0; j <= currentF; ++j) {
				if (hCounts[i * (currentF + 1) + j] != 0)
					printf("h_F: %u, h_B: %u, count: %llu\n", i, j, hCounts[i * (currentF + 1) + j]);
			}
		}

		currentF = result;
		printf("currentF: %u\n", currentF);
	}
	return std::numeric_limits<DomainCost>::max();
}
