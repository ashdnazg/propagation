#pragma once

class IDAStarXorSlimPancakeSearcher
{
public:


	struct Node {
		unsigned first;
		unsigned before_flip;
		unsigned after_flip;
		unsigned flip;
		//unsigned h;
	};

	typedef std::array<std::array<unsigned char, N + 1>, N + 1> Cache;

	static bool DFS(const Pancakes& start, const Pancakes& goal, unsigned th);
	static unsigned search(const Pancakes& start);
	static unsigned initCache(Cache& cache, const Pancakes& start, const Pancakes& goal);
	static std::uint64_t generated;
};

unsigned IDAStarXorSlimPancakeSearcher::initCache(Cache& cache, const Pancakes& start, const Pancakes& goal) {
	memset(cache.data()->data(), 0, (N + 1) * (N + 1) * sizeof(cache[0][0]));

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


bool IDAStarXorSlimPancakeSearcher::DFS(const Pancakes& start, const Pancakes& goal, unsigned th) {
	Node stack[MAX_SOLUTION * MAX_DESCENDANTS];
	Cache hCache;
	Pancakes neighbors;
	unsigned h = initCache(hCache, start, goal);

	if (th < h)
		return false;

	neighbors[start[0]] = start[1];
	for (unsigned pos = 1; pos < N; ++pos) {
		neighbors[start[pos]] = start[pos - 1] ^ start[pos + 1];
	}
	neighbors[N] = start[N - 1];

	stack[0] = Node{start[0], 0, 0, N + 1};
	stack[1] = Node{start[0], start[N - 1], N, N - 1};
	Node* sp = stack + 1;

	while (true) {
LOOP_START:
		if (sp == stack)
			return false;

		const unsigned first = sp->first;
		unsigned before_flip = sp->before_flip;
		unsigned after_flip = sp->after_flip;
		unsigned flip = sp->flip;
		//printf("flip: %d, first: %u, before_flip: %u, after_flip: %u\n", flip ,first, before_flip, after_flip);
		while (true) {
			if (flip == 0) {
				//printf("rolling back\n");
				const Node prevNode = *(sp - 1);
				h += hCache[prevNode.after_flip][prevNode.first];
				h -= hCache[prevNode.after_flip][prevNode.before_flip];

				neighbors[prevNode.before_flip] ^= prevNode.after_flip;
				neighbors[prevNode.after_flip] ^= prevNode.before_flip ^ prevNode.first;
				neighbors[prevNode.first] ^= prevNode.after_flip;
				++th;
				--sp;
				sp->flip = prevNode.flip - 1;
				sp->after_flip = prevNode.before_flip;
				sp->before_flip = neighbors[prevNode.before_flip] ^ prevNode.after_flip;

				goto LOOP_START;
			}

			int tempTh = th - h;
			tempTh += hCache[after_flip][first];
			tempTh -= hCache[after_flip][before_flip];
			if (tempTh > 0)
				goto FOUND;

			--flip;
			unsigned old_after_flip = after_flip;
			after_flip = before_flip;
			before_flip = neighbors[before_flip] ^ old_after_flip;
		}
FOUND:
		++generated;
		//printf("flip: %d\n", flip);


		h -= hCache[after_flip][first];
		h += hCache[after_flip][before_flip];

		if (h == 0)
			return true;

		neighbors[before_flip] ^= after_flip;
		neighbors[after_flip] ^= before_flip ^ first;
		neighbors[first] ^= after_flip;
		--th;


		*sp = Node{first, before_flip, after_flip, flip};

		++sp;
		*sp = Node{before_flip, neighbors[N], N, N - 1};
	}
}

unsigned IDAStarXorSlimPancakeSearcher::search(const Pancakes& start) {
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

std::uint64_t IDAStarXorSlimPancakeSearcher::generated = 0;