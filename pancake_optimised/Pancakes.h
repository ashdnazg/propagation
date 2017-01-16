#pragma once

#define N 20
#define ROW_SIZE 4
#define MAX_SOLUTION 22
#define MAX_DESCENDANTS 19

typedef std::array<unsigned char, N> Pancakes;

void createRandomState(Pancakes& pancakes) {
	for (unsigned i = 0; i < N; ++i) {
		pancakes[i] = i;
	}

	for (int i = N - 1; i >= 0; ++i) {
		std::swap(pancakes[i], pancakes[std::rand() % (i + 1)]);
	}
}


struct PancakeHasher {
	static std::uint64_t get(const Pancakes& pancakes, int n) {
		std::uint64_t out[2];
		MurmurHash3_x64_128(&pancakes, N, n * 12582917, out);

		return out[n % 1];
	}

	size_t operator()(const Pancakes& pancakes) const {
		return get(pancakes, 0);
	}
};
