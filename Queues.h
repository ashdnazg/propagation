#pragma once

template <typename T, typename Comp>
class ClearableQueue: public std::priority_queue<T, std::vector<T>, Comp> {
public:
	void clear() { this->c.clear(); }
	std::vector<T>& get() { return this->c; }
	const std::vector<T>& get() const { return this->c; }
};

// Functors for priority queue ordering
template <typename T>
struct lessF: public std::binary_function<const T&, const T&, bool> {
	inline bool operator() (const T& x, const T& y) const {
		return (x.f > y.f);
	}
};

template <typename T>
struct lessFMoreG: public std::binary_function<const T&, const T&, bool> {
	inline bool operator() (const T& x, const T& y) const {
		return x.f == y.f ? (x.g < y.g) : (x.f > y.f);
	}
};

template <typename T>
struct lessFLessG: public std::binary_function<const T&, const T&, bool> {
	inline bool operator() (const T& x, const T& y) const {
		return x.f == y.f ? (x.g > y.g) : (x.f > y.f);
	}
};

template <typename T>
struct moreG: public std::binary_function<const T&, const T&, bool> {
	inline bool operator() (const T& x, const T& y) const {
		return x.g < y.g;
	}
};

template <typename T>
struct lessG: public std::binary_function<const T&, const T&, bool> {
	inline bool operator() (const T& x, const T& y) const {
		return x.g > y.g;
	}
};


template <typename T, class D, class F = lessFLessG<T>>
class PriorityQueue: public ClearableQueue<T, F> {
public:
	typename D::CostType getMinG(const T& node) const {
		typename D::CostType g = std::numeric_limits<typename D::CostType>::max();
		bool found = false;
		for (const T& other: this->c) {
			if (D::same(other.domainNode, node.domainNode)) {
				found = true;
				g = std::min(other.g, g);
			}
		}
		assert(found);
		return g;
	}
};

template <typename T, class D>
class BucketQueue {
public:
	struct Bucket {
		typename D::CostType f;
		PriorityQueue<T, D, moreG<T>> nodes;
	};
	inline bool empty() const { return buckets.empty(); }
	inline void clear() { buckets.clear(); }
	inline void push(const T& node) {
		for (Bucket& b: buckets) {
			if (b.f == node.f) {
				b.nodes.push(node);
				return;
			}
		}
		Bucket b;
		b.f = node.f;
		b.nodes.push(node);
		buckets.push_back(std::move(b));
		for (int pos = buckets.size() - 2; pos >= 0 && node.f < buckets[pos].f; --pos) {
			std::swap(buckets[pos], buckets[pos + 1]);
		}
	}
	inline void pop() {
		buckets.front().nodes.pop();
		if (buckets.front().nodes.empty())
			buckets.erase(buckets.begin());
	}
	inline const T& top() const {
		return buckets.front().nodes.top();
	}
	inline typename D::CostType getMinG(const T& node) const {
		for (const Bucket& b: buckets) {
			for (const T& other: b.nodes.get()) {
				if (D::same(other.domainNode, node.domainNode))
					return other.g;
			}
		}
		assert(false);
		return std::numeric_limits<typename D::CostType>::max();
	}
private:
	std::vector<Bucket> buckets;
};
