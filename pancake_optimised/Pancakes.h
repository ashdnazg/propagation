#pragma once

#define N 55
#define MAX_SOLUTION (2 * N)
#define MAX_DESCENDANTS (N - 1)

#define GAP_LEVEL 0

typedef std::array<unsigned char, N + 1> Pancakes;

void createRandomState(Pancakes& pancakes) {
	for (unsigned i = 0; i < N + 1; ++i) {
		pancakes[i] = i;
	}

	for (int i = N - 1; i >= 0; --i) {
		std::swap(pancakes[i], pancakes[std::rand() % (i + 1)]);
	}
}

void printState(const Pancakes& pancakes) {
	for (unsigned i = 0; i < N; ++i) {
		printf("%u, ", pancakes[i]);
	}
}


struct PancakeHasher {
	static std::uint64_t get(const Pancakes& pancakes, int n) {
		std::uint64_t out[2];
		MurmurHash3_x64_128(pancakes.data(), N, n * 12582917, out);

		return out[n % 2];
	}

	size_t operator()(const Pancakes& pancakes) const {
		return get(pancakes, 0);
	}
};
