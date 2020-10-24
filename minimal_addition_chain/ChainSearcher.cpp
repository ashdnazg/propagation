#include <chrono>
#include <array>
#include <vector>
#include <algorithm>

constexpr unsigned TARGET = 27719U;
constexpr unsigned MAX_CHAIN = 100;
constexpr unsigned DELAY = 3;

typedef std::array<unsigned, MAX_CHAIN> Chain;

bool find_chain_rec(unsigned chain_size, unsigned g, Chain& chain) {
	const unsigned last_number = chain[g];
	g += 1;

	for (unsigned idx1 = g - 1; idx1 != -1U; --idx1) {
		if (chain[idx1] <= last_number / 2) {
			break;
		}
		for (unsigned idx2 = idx1; idx2 != -1U; --idx2) {
			const unsigned next_number = chain[idx1] + chain[idx2];
			if (next_number == TARGET >> (chain_size - g) && next_number << (chain_size - g) == TARGET) {
				for (;g <= chain_size;++g) {
					chain[g] = TARGET >> (chain_size - g);
				}
				return true;
			}
			if (next_number < TARGET >> (chain_size - g)) {
				break;
			}
			if (next_number < last_number) {
				break;
			}
			if (g == chain_size) {
				continue;
			}

			chain[g] = next_number;
			if (find_chain_rec(chain_size, g, chain)) {
				return true;
			}
		}
	}

	return false;
}

bool find_chain_iter(unsigned chain_size) {
	Chain chain;
	chain[0] = 1;
	chain[1] = 2;

	bool ret = find_chain_rec(chain_size, 1, chain);
	if (ret) {
		for (unsigned i = 0; i < chain_size + 1; ++i) {
			printf("%u ", chain[i]);
		}
		printf("\n");
	}

	return ret;
}


unsigned find_chain() {
	for (unsigned chain_size = 1; chain_size <= MAX_CHAIN; ++chain_size) {
		if (1U << chain_size < TARGET) {
			continue;
		}

		if (find_chain_iter(chain_size)) {
			return chain_size;
		}
	}
	return 0;
}


bool find_chain_delay_rec(unsigned chain_size, unsigned g, unsigned last_number, Chain& chain) {
	// for (unsigned i = 0; i < g + 1; ++i) {
		// printf("%u ", chain[i]);
	// }
	// printf("\n");
	g += 1;

	const unsigned optimal_distance = (chain_size - g) / DELAY;

	for (unsigned idx1 = g - DELAY; idx1 != -1U; --idx1) {
		if (chain[idx1] == 0) {
			continue;
		}
		if (chain[idx1] < last_number / 2) {
			break;
		}
		for (unsigned idx2 = idx1; idx2 != -1U; --idx2) {
			if (chain[idx2] == 0) {
				continue;
			}
			const unsigned next_number = chain[idx1] + chain[idx2];
			if (next_number > TARGET) {
				continue;
			}
			if (next_number == TARGET >> optimal_distance && next_number << optimal_distance == TARGET) {
				for (unsigned gg = g;gg <= chain_size;gg += 1) {
					chain[gg] = 0;
				}
				for (;g <= chain_size;g += DELAY) {
					chain[g] = TARGET >> (chain_size - g);
				}
				return true;
			}
			if (next_number < TARGET >> optimal_distance) {
				break;
			}
			if (next_number <= last_number) {
				break;
			}
			if (g == chain_size) {
				continue;
			}

			chain[g] = next_number;
			if (find_chain_delay_rec(chain_size, g, next_number, chain)) {
				return true;
			}
		}
	}
	if (g == chain_size) {
		return false;
	}

	chain[g] = 0;
	return find_chain_delay_rec(chain_size, g, last_number, chain);
}


bool find_chain_delay_iter(unsigned chain_size) {
	Chain chain;
	chain.fill(0);
	chain[0] = 1;
	chain[DELAY] = 2;

	bool ret = find_chain_delay_rec(chain_size, 2 * DELAY - 1, 2, chain);
	if (ret) {
		for (unsigned i = 0; i < chain_size + 1; ++i) {
			printf("%u ", chain[i]);
		}
		printf("\n");
	}

	return ret;
}


unsigned find_chain_delay() {
	for (unsigned chain_size = 0; chain_size <= MAX_CHAIN; ++chain_size) {
		if ((1U << (chain_size / DELAY)) < TARGET) {
			continue;
		}

		if (find_chain_delay_iter(chain_size)) {
			return chain_size;
		}
	}
	return 0;
}

int main() {
	{
		printf("Target: %u\n", TARGET);
		unsigned long before = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		unsigned length = find_chain();
		unsigned long after = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		printf("Length = %u\n", length);
		printf("time: %lu\n", (unsigned long) (after - before));
	}

	{
		printf("Delay: %u\n", DELAY);
		unsigned long before = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		unsigned length = find_chain_delay();
		unsigned long after = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		printf("Length (delay) = %u\n", length);
		printf("time (delay): %lu\n", (unsigned long) (after - before));
	}

	return 0;
}
