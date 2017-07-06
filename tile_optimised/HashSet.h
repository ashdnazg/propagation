#pragma once

template<typename T, typename H, long long unsigned int M>
class HashSet {
public:
	HashSet() { }

	bool insert(const T& object) {
		hashSet.insert(object);
		return hashSet.size() >= M;
	}

	bool contains(const T& object) const {
		return hashSet.find(object) != hashSet.end();
	}

	bool isEmpty() const {
		return hashSet.empty();
	}

	std::uint64_t getCount() const {
		return hashSet.size();
	}

	void clear() {
		hashSet.clear();
	}

	std::unordered_set<T, H> hashSet;
};
