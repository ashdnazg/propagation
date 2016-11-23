#pragma once

template <class D>
class BFSSearcher
{
public:
	typedef typename D::NodeType DomainNode;

	struct Node {
		DomainNode domainNode;
		int g;
	};

	BFSSearcher(const D* domain_) : expanded(0), generated(0), domain(domain_) { }
	void reset(DomainNode start_, DomainNode goal_);
	bool isEmpty() const;
	BFSSearcher<D>::Node expand();
	void generate(DomainNode node, int distance);
	void generate(std::vector<DomainNode>& nodesVec, int distance);
	int search(DomainNode start, DomainNode goal);


	unsigned expanded;
	unsigned generated;
private:
	DomainNode goal;
	typename D::List openList;
	typename D::List closedList;
	std::deque<Node> q;
	const D* domain;
};

template <class D>
void BFSSearcher<D>::reset(DomainNode start_, DomainNode goal_)
{
	q.clear();
	goal = goal_;

	generate(start_, 0);
}

template <class D>
void BFSSearcher<D>::generate(DomainNode node, int distance)
{
	if (openList.contains(node) || closedList.contains(node))
		return;

	++generated;
	openList.insert(node);
	Node n = {node, distance};
	q.push_back(n);
}

template <class D>
void BFSSearcher<D>::generate(std::vector<DomainNode>& nodesVec, int distance)
{
	for (const DomainNode& node: nodesVec) {
		generate(node, distance);
	}
}

template <class D>
typename BFSSearcher<D>::Node BFSSearcher<D>::expand()
{
	++expanded;
	Node best = q.front();
	q.pop_front();
	closedList.insert(best.domainNode);
	return best;
}


template<class D>
int BFSSearcher<D>::search(DomainNode start_, DomainNode goal_) {
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