#pragma once

template<typename T, class H, int M, int K>
class BloomFilter {
public:
	BloomFilter() : usingSet(true) {
		filter.resize(M, false);
	}


	void bloomInsert(const T& object) {

		for (int i = 0; i < K; ++i) {
			const std::uint64_t b = H::get(object, i) % M;
			onesCount += 1 - filter[b];
			filter[b] = true;
		}
	}

	void insert(const T& object) {
		if (usingSet) {
			hashSet.insert(object);
			// if we're way too big, switch to bloom filter
			if (hashSet.size() * sizeof(T) < (M >> 5)) {
				return;
			}
			for (const T& o: hashSet) {
				bloomInsert(o);
			}
			hashSet.clear();
			usingSet = false;
		}

		bloomInsert(object);
	}

	bool contains(const T& object) const {
		if (usingSet) {
			return hashSet.find(object) != hashSet.end();
		}

		for (int i = 0; i < K; ++i) {
			const std::uint64_t b = H::get(object, i) % M;
			if (!filter[b]) {
				return false;
			}
		}
		return true;
	}

	double getSaturation() const {
		return double(onesCount) / double(M);
	}

	bool isUsingSet() const {
		return usingSet;
	}

	bool isSetEmpty() const {
		return hashSet.empty();
	}

	void clear() {
		std::fill(filter.begin(), filter.end(), false);
		usingSet = true;
		hashSet.clear();
	}

	bool usingSet;
	std::unordered_set<T, PancakeHasher> hashSet;
	std::vector<bool> filter;
	std::uint64_t onesCount;
};
