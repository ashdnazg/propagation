#pragma once

#define N 16
#define ROW_SIZE 4
#define MAX_SOLUTION 80
#define MAX_DESCENDANTS 4

typedef unsigned char Tiles[N];

bool verifyState(const Tiles& tiles) {
	std::vector<unsigned> nums(N, 0);
	std::vector<unsigned> rev(N, 0);
	bool parity = true;

	for (unsigned i = 0; i < N; ++i) {
		nums[i] = tiles[i];
		rev[tiles[i]] = i;
		if (tiles[i] == 0) {
			unsigned dx = ROW_SIZE - (i % ROW_SIZE) - 1;
			unsigned dy = ROW_SIZE - (i / ROW_SIZE) - 1;
			parity = parity ^ ((dx + dy) % 2);
		}
	}

	for (unsigned i = 0; i < (N - 1); ++i) {
		if (nums[i] == i + 1)
			continue;

		unsigned pos = rev[i + 1];
		rev[nums[i]] = pos;
		std::swap(nums[i], nums[pos]);
		parity = !parity;
	}
	return parity;
}


void createRandomState(Tiles& tiles) {
	std::vector<unsigned> nums(N, 0);
	for (unsigned i = 0; i < N; ++i) {
		nums[i] = i;
	}

	for (unsigned i = 0; i < N; ++i) {
		unsigned char idx = std::rand() % (N - i);
		tiles[i] = nums[idx];
		nums.erase(nums.begin() + idx);
	}

	if (!verifyState(tiles)) {
		int i = 0;
		while (tiles[i] == 0) { ++i; }
		int j = i + 1;
		while (tiles[j] == 0) { ++j; }
		std::swap(tiles[i], tiles[j]);
	}

}