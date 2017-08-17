#pragma once

template <class D, class H>
class CAStarSearcher
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

	CAStarSearcher(const D& domain_) : expanded(0), generated(0), domain(domain_) { }
	void reset(DomainNode start_, DomainNode goal_);

	DomainCost backDFS(const Node& root);
	DomainCost DFS(const Node& root);
	DomainCost backSearch();
	DomainCost search(DomainNode start, DomainNode goal);

	std::uint64_t expanded;
	std::uint64_t generated;
	std::uint64_t memoryUsed;
private:
	DomainNode start;
	DomainNode goal;
	const D& domain;
	DomainCost thF;
	DomainCost thB;
	DomainCost fF;
	DomainCost fB;
	typename D::List perimeter;
	DomainNode nextCommonAncestor;
	DomainNode commonAncestor;
	DomainCost caDepth;
	DomainCost nextCADepth;
};

template <class D, class H>
void CAStarSearcher<D,H>::reset(DomainNode start_, DomainNode goal_)
{
	goal = goal_;
	DomainCost h = H::get(domain, start_, goal_);
	thF = (h + 1) / 2;
	thB = h / 2;
	expanded = 0;
	generated = 0; // the start is always generated
	memoryUsed = 0;
}


template<class D, class H>
typename CAStarSearcher<D,H>::DomainCost CAStarSearcher<D,H>::backDFS(const Node& root) {
	DomainCost result = std::numeric_limits<DomainCost>::max();
	std::vector<DomainNeighbor> nodesVec;
	expanded++;
	// printf("backward expanding: ");
	// D::printState(root.domainNode);
	// printf("\n");
	domain.getNeighbors(root.domainNode, nodesVec);
	generated += nodesVec.size();
	for (const DomainNeighbor& n: nodesVec) {
		DomainCost h = H::get(domain, n.node, commonAncestor);
		Node neighbor = {n.node, root.g + n.cost, root.g + n.cost + h};
		// printf("backward generating: ");
		// D::printState(n.node);
		// printf("in depth: %u\n", (unsigned) root.g + n.cost);
		DomainCost tempResult;
		if (neighbor.f > fB) {
			tempResult = neighbor.f;
		} else if (neighbor.g == thB) {
			if (perimeter.contains(n.node)) {
				return fB;
			}
			tempResult = std::numeric_limits<DomainCost>::max();
		} else {
			tempResult = backDFS(neighbor);
			if (tempResult == fB) {
				return fB;
			}
		}

		result = std::min(result, tempResult);
	}
	return result;
}


template<class D, class H>
typename CAStarSearcher<D,H>::DomainCost CAStarSearcher<D,H>::backSearch() {
	if (thB == 0) {
		if (perimeter.contains(goal)) {
			return 0;
		} else {
			return std::numeric_limits<DomainCost>::max();
		}
	}

	Node root = {goal, 0, thF + thB};
	++generated;
	return backDFS(root);
}


template<class D, class H>
typename CAStarSearcher<D,H>::DomainCost CAStarSearcher<D,H>::DFS(const Node& root) {
	DomainCost result = std::numeric_limits<DomainCost>::max();
	std::vector<DomainNeighbor> nodesVec;
	expanded++;
	// printf("expanding: ");
	// D::printState(root.domainNode);
	// printf("\n");
	domain.getNeighbors(root.domainNode, nodesVec);
	generated += nodesVec.size();
	for (const DomainNeighbor& n: nodesVec) {
		DomainCost h = H::get(domain, n.node, goal);
		Node neighbor = {n.node, root.g + n.cost, root.g + n.cost + h};
		// printf("generating: ");
		// D::printState(n.node);
		// printf("in depth: %u h: %u\n", (unsigned) root.g + n.cost, h);
		DomainCost tempResult;
		if (neighbor.f > fF) {
			tempResult = neighbor.f;
		} else if (neighbor.g == thF) {
			if (perimeter.empty()) {
				nextCommonAncestor = n.node;
				nextCADepth = neighbor.g;
			}
			// printf("is candidate!\n");
			perimeter.insert(n.node);
			memoryUsed = std::max(perimeter.size(), memoryUsed);
			commonAncestor = nextCommonAncestor;
			caDepth = nextCADepth;
			// printf("caDepth is now: %u\n", caDepth);
			tempResult = fF + 1; // bad hack, sorry
		} else {
			tempResult = DFS(neighbor);
			if (tempResult == fF) {
				return fF;
			}
		}
		if (!perimeter.empty() && root.g < nextCADepth) {
			nextCommonAncestor = root.domainNode;
			nextCADepth = root.g;

			if (root.g <= (thF >> 1)) {
				//printf("doing backward search with ca depth: %d\n", caDepth);
				fB = fF - caDepth;
				// printf("doing back search to: ");
				// D::printState(commonAncestor);
				// printf("with fB: %u\n", (unsigned) fB);
				DomainCost backResult = backSearch();

				if (backResult == fB) {
					return fF;
				}
				perimeter.clear();
			}
		}
		result = std::min(result, tempResult);
	}
	return result;
}

template<class D, class H>
typename CAStarSearcher<D,H>::DomainCost CAStarSearcher<D,H>::search(DomainNode start_, DomainNode goal_) {
	reset(start_, goal_);
	Node root = {start_, 0, thF + thB};
	++generated;
	while (true) {
		fF = thF + thB;
		// printf("currentF: %u, thF: %u, thB: %u\n", fF, thF, thB);
		DomainCost result = DFS(root);
		if (result == std::numeric_limits<DomainCost>::max())
			return result;

		if (result == fF)
			return fF;

		if (thF > thB) {
			thB++;
		} else {
			thF++;
		}
	}
	return std::numeric_limits<DomainCost>::max();
}
