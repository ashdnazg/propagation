#include <chrono>
#include <cstdio>
#include <cstdint>
#include <x86intrin.h>
#include <cstdlib>
#include <cstring>

constexpr uint64_t ITERATIONS = 100000000;

void print256_num(__m256i &var) {
    alignas(16) uint32_t val[8];
    _mm256_storeu_si256((__m256i*) val, var);
    printf("Numerical: %u %u %u %u %u %u %u %u \n",
           val[0], val[1], val[2], val[3], val[4], val[5],
           val[6], val[7]);
}

int main() {
	std::srand(std::time(0));
	__m256i vperm = _mm256_set_epi32(std::rand(), std::rand(), std::rand(), std::rand(), std::rand(), std::rand(), std::rand(), std::rand());
	print256_num(vperm);
	{
		unsigned long before = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		__m256i vi = vperm;
		for (uint64_t i = 0; i < ITERATIONS; i++) {
			__m256i vp3 = _mm256_shuffle_epi8(vi, vi); // 2
			vp3 = _mm256_shuffle_epi8(vp3, vi);           // 3 (+1)
			__m256i vi2 = _mm256_shuffle_epi8(vp3, vp3);      // 6
			vi2 = _mm256_shuffle_epi8(vi2, vi2);                // 12
			vi2 = _mm256_shuffle_epi8(vi2, vi2);                // 24
			vi2 = _mm256_shuffle_epi8(vi2, vp3);               // 27 (+3)
			vi2 = _mm256_shuffle_epi8(vi2, vi2);                // 54
			vi2 = _mm256_shuffle_epi8(vi2, vi2);                // 108
			vi2 = _mm256_shuffle_epi8(vi2, vi2);                // 216
			vi2 = _mm256_shuffle_epi8(vi2, vi2);                // 432
			vi2 = _mm256_shuffle_epi8(vi2, vi);             // 433 (+1)
			vi2 = _mm256_shuffle_epi8(vi2, vi2);                // 866
			vi2 = _mm256_shuffle_epi8(vi2, vi2);                // 1732
			vi2 = _mm256_shuffle_epi8(vi2, vi2);                // 3464
			vi2 = _mm256_shuffle_epi8(vi2, vi2);                // 6928
			vi2 = _mm256_shuffle_epi8(vi2, vi2);                // 13856
			vi2 = _mm256_shuffle_epi8(vi2, vp3);               // 13859 (+3)
			vi2 = _mm256_shuffle_epi8(vi2, vi2);                // 27718
			vi = _mm256_shuffle_epi8(vi2, vi);             // 27719 (+1)
		}
		unsigned long after = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		print256_num(vi);
		printf("time (No delay): %lu\n", (unsigned long) (after - before));
	}

	{
		unsigned long before = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		__m256i vp27719 = vperm;
		for (uint64_t i = 0; i < ITERATIONS; i++) {
			const __m256i vp2      = _mm256_shuffle_epi8(vp27719, vp27719);
			const __m256i vp4      = _mm256_shuffle_epi8(vp2, vp2);
			const __m256i vp8      = _mm256_shuffle_epi8(vp4, vp4);
			const __m256i vp16     = _mm256_shuffle_epi8(vp8, vp8);
			const __m256i vp32     = _mm256_shuffle_epi8(vp16, vp16);
			const __m256i vp64     = _mm256_shuffle_epi8(vp32, vp32);
			const __m256i vp128    = _mm256_shuffle_epi8(vp64, vp64);
			const __m256i vp256    = _mm256_shuffle_epi8(vp128, vp128);
			const __m256i vp512    = _mm256_shuffle_epi8(vp256, vp256);
			const __m256i vp1024   = _mm256_shuffle_epi8(vp512, vp512);
			const __m256i vp2048   = _mm256_shuffle_epi8(vp1024, vp1024);
			const __m256i vp2049   = _mm256_shuffle_epi8(vp2048, vp27719);
			const __m256i vp2064   = _mm256_shuffle_epi8(vp2048, vp16);
			const __m256i vp3073   = _mm256_shuffle_epi8(vp2049, vp1024);
			const __m256i vp4098   = _mm256_shuffle_epi8(vp2049, vp2049);
			const __m256i vp5137   = _mm256_shuffle_epi8(vp3073, vp2064);
			const __m256i vp7171   = _mm256_shuffle_epi8(vp3073, vp4098);
			const __m256i vp10274  = _mm256_shuffle_epi8(vp5137, vp5137);
			const __m256i vp20548  = _mm256_shuffle_epi8(vp10274, vp10274);
			vp27719 = _mm256_shuffle_epi8(vp20548, vp7171);
		}
		unsigned long after = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		print256_num(vp27719);
		printf("time (delay): %lu\n", (unsigned long) (after - before));
	}

	return 0;
}
