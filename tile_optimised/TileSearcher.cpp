#include <algorithm>
#include <cstdio>
#include "IDAStar.h"

int main() {
	const IDAStarTileSearcher::Tiles start {
		14, 0,12,15,
		 7, 9, 4, 6,
		 1, 3,11, 8,
		13, 5, 2,10
	};

	unsigned result = IDAStarTileSearcher::search(start);
	printf("%u\n", result);
	return 0;
}
