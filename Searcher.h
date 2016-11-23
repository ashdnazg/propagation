#pragma once

// D=Domain P=PQueue<D>
template <class D, class P>
class Searcher
{
public:
	Searcher(const D* domain_, P* pQueue_) : domain(domain_) , pQueue(pQueue_) { }
	int search(typename D::NodeType start, typename D::NodeType end);

private:
	const D* domain;
	P* pQueue;
};

template<class D, class P>
int Searcher<D, P>::search(typename D::NodeType start, typename D::NodeType end) {
	std::vector<typename D::NodeType> tempNodes;
	pQueue->reset(start, end);

	while (!pQueue->isEmpty()) {
		int res = pQueue->expand();
		if (res != -1)
			return res;

		tempNodes.clear();
		domain->getNeighbors(bestNode, tempNodes);
		pQueue->generate(tempNodes);
	}
	return -1;
}