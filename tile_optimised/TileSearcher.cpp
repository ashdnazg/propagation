#include <algorithm>
#include <cstdio>
#include "IDAStar.h"

int main() {
	const IDAStarTileSearcher::Tiles start {
		 0, 3, 9,10,
		11, 7,15, 8,
		 5,14, 4,12,
		 1, 6, 2,13
	};

	unsigned result = IDAStarTileSearcher::search(start);
	printf("%u\n", result);
	return 0;
}
