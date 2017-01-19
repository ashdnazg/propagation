#pragma once

template<typename T, class H, long long unsigned int M, int K>
class BloomFilter {
public:
	BloomFilter() : hashOffset(0), usingSet(true), onesCount(0), count(0) {
		filter.resize(M, false);
	}


	void bloomInsert(const T& object) {
		for (unsigned i = hashOffset; i < hashOffset + K; ++i) {
			const std::uint64_t b = H::get(object, i) % M;
			onesCount += 1 - filter[b];
			filter[b] = true;
		}
		++count;
	}

	void insert(const T& object) {
		if (usingSet) {
			hashSet.insert(object);
			// if we're way too big, switch to bloom filter
			if (hashSet.size() * sizeof(T) < (M >> 3)) {
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

		for (unsigned i = hashOffset; i < hashOffset + K; ++i) {
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

	std::uint64_t getCount() const {
		return usingSet ? hashSet.size() : count;
	}

	void clear() {
		++hashOffset;
		std::fill(filter.begin(), filter.end(), false);
		onesCount = 0;
		usingSet = true;
		hashSet.clear();
		count = 0;
	}

	unsigned hashOffset;
	bool usingSet;
	std::unordered_set<T, PancakeHasher> hashSet;
	std::vector<bool> filter;
	std::uint64_t onesCount;
	std::uint64_t count;
};
