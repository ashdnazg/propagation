#pragma once

class IDAStarXorPancakeSearcher
{
public:

	typedef unsigned Cache[N + 1][N + 1];

	static bool DFS(const Pancakes& start, const Pancakes& goal, unsigned th);
	static bool DFS2(unsigned th, int flip, int first, int g, int h, Pancakes& neighbors, const Cache& hCache);
	static unsigned search(const Pancakes& start);
	static unsigned initCache(Cache& cache, const Pancakes& start, const Pancakes& goal);
	static std::uint64_t generated;
};

unsigned IDAStarXorPancakeSearcher::initCache(Cache& cache, const Pancakes& start, const Pancakes& goal) {
	memset(cache, 0, (N + 1) * (N + 1) * sizeof(**cache));

	for (int pos = GAP_LEVEL; pos < N; ++pos) {
		cache[goal[pos]][goal[pos + 1]] = 1;
		cache[goal[pos + 1]][goal[pos]] = 1;
	}
	unsigned h = N - GAP_LEVEL;

	for (int pos = 0; pos < N; ++pos) {
		h -= cache[start[pos]][start[pos + 1]];
	}

	return h;
}

bool IDAStarXorPancakeSearcher::DFS2(unsigned th, int flip, int first, int g, int h, Pancakes& neighbors, const Cache& hCache) {
	unsigned before_flip = neighbors[first];
	unsigned after_flip = neighbors[before_flip] ^ first;
	for (int pos = 2; pos <= N; ++pos) {
		if (pos != flip) {

			unsigned tempH = h;
			tempH -= hCache[after_flip][first];
			tempH += hCache[after_flip][before_flip];

			if (tempH == 0)
				return true;

			//printf("th: %u, g: %u, pos: %u, h: %u, expected h: %d, first: %u, before_flip: %u, after_flip:%u\n", th, g, pos, h, tempH, first, before_flip, after_flip);
			if ((g + 1 + tempH) <= th) {
				// printf("flipping: before_flip: %d, after_flip: %d\n", before_flip, after_flip);
				// printf("generating, pos: %u expected h: %d\n", pos, tempH);

				neighbors[before_flip] ^= after_flip;
				neighbors[after_flip] ^= before_flip ^ first;
				neighbors[first] ^= after_flip;

				bool ret = DFS2(th, pos, before_flip, g + 1, tempH, neighbors, hCache);
				if (ret)
					return true;

				neighbors[before_flip] ^= after_flip;
				neighbors[after_flip] ^= before_flip ^ first;
				neighbors[first] ^= after_flip;

				++generated;
			}
		}
		unsigned old_before_flip = before_flip;
		before_flip = after_flip;
		after_flip = neighbors[after_flip] ^ old_before_flip;
	}

	return false;
}


bool IDAStarXorPancakeSearcher::DFS(const Pancakes& start, const Pancakes& goal, unsigned th) {
	Pancakes neighbors;
	Cache hCache;


	unsigned h = initCache(hCache, start, goal);

	if (h == 0)
		return true;

	if (th < h) {
		//printf("breaking forward since initial h was: %u\n", h);
		return false;
	}

	// Creating start node
	int first = start[0];

	{
		neighbors[start[0]] = start[1];
		for (unsigned pos = 1; pos < N; ++pos) {
			neighbors[start[pos]] = start[pos - 1] ^ start[pos + 1];
		}
		neighbors[N] = start[N - 1];
	}

	return DFS2(th, 0, first, 0, h, neighbors, hCache);
}

unsigned IDAStarXorPancakeSearcher::search(const Pancakes& start) {
	generated = 0;
	Pancakes goal;
	unsigned th = 0;
	for (unsigned i = 0; i < N + 1; ++i) {
		goal[i] = i;
	}
	while (true) {
		//printf("Starting iteration with thF: %u, thB: %u\n", thF, thB);
		bool found = DFS(start, goal, th);
		if (found) {
			return th;
		}

		++th;
	}
	return 0;
}

std::uint64_t IDAStarXorPancakeSearcher::generated = 0;