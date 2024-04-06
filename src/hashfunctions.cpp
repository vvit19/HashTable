#include "hashtable.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <immintrin.h>
#include <sys/types.h>

const uint32_t POLINOM = 0xEDB88320;

static inline uint32_t Ror (uint32_t num, int shift);
static inline uint32_t Rol (uint32_t num, int shift);

uint32_t HashZero (const char* /*word*/, size_t /*len*/)
{
    return 0;
}

uint32_t HashFirstLetter (const char* word, size_t /*len*/)
{
    assert (word);

    return (uint32_t) word[0];
}

uint32_t HashStrlen (const char* /*word*/, size_t len)
{
    return (uint32_t) len;
}

uint32_t HashAsciiSum (const char* word, size_t len)
{
    assert (word);

    uint32_t sum = 0;
    for (size_t i = 0; i < len; i++) sum += (uint32_t) word[i];

    return sum;
}

uint32_t HashAsciiSumDivStrlen (const char* word, size_t len)
{
    assert (word);

    uint32_t sum = HashAsciiSum (word, len);

    return sum / (uint32_t) len;
}

uint32_t HashRor (const char* word, size_t len)
{
    assert (word);

    uint32_t hash_value = 0;
    for (size_t i = 0; i < len; i++) hash_value = Ror (hash_value, 1) ^ (uint32_t) word[i];

    return hash_value;
}

uint32_t HashRol (const char* word, size_t len)
{
    assert (word);

    uint32_t hash_value = 0;
    for (size_t i = 0; i < len; i++) hash_value = Rol (hash_value, 1) ^ (uint32_t) word[i];

    return hash_value;
}

uint32_t HashCrc32 (const char* word, size_t len)
{
    assert (word);

	uint32_t crc_array[256] = {}, crc = 0;

	for (uint32_t i = 0; i < 256; ++i)
	{
		crc = i;
		for (size_t cnt = 0; cnt < 8; cnt++)
        {
			crc = crc & 1 ? (crc >> 1) ^ POLINOM : crc >> 1;
        }

		crc_array[i] = crc;
	};

	crc = 0xFFFFFFFFUL;

    for (size_t i = 0; i < len; ++i)
    {
        crc = crc_array[(crc ^ (uint32_t) word[i]) & 0xFF] ^ (crc >> 8);
    }

	return crc ^ 0xFFFFFFFFUL;
}

uint32_t IntrinsicHashCrc32 (const char* word, size_t len)
{
    __m256i crc_vector_table[32] = {}, one_vecor = _mm256_set1_epi32 (1);
    __m256i polinom_vector = _mm256_set1_epi32 (POLINOM);
    __m256i zero_vector = _mm256_set1_epi32 (0);

    int bit = 0;
    for (size_t i = 0; i < 32; ++i, bit += 8)
    {
        __m256i crc_vector = _mm256_set_epi32 (bit + 7, bit + 6, bit + 5, bit + 4,
                                               bit + 3, bit + 2, bit + 1, bit);

        for (size_t cnt = 0; cnt < 8; cnt++)
        {
            __m256i and_vector = _mm256_and_si256 (crc_vector, one_vecor);
            crc_vector = _mm256_srli_epi32 (crc_vector, 1);
            and_vector = _mm256_mullo_epi32 (and_vector, polinom_vector);
            crc_vector = _mm256_xor_si256 (crc_vector, and_vector);
        }

        crc_vector_table[i] = _mm256_loadu_si256 (&crc_vector);
    }

    uint32_t crc = 0xFFFFFFFFUL;
    uint32_t* crc_array = (uint32_t*) &crc_vector_table;

    for (size_t i = 0; i < len; ++i)
        crc = crc_array[(crc ^ (uint32_t) word[i]) & 0xFF] ^ (crc >> 8);


    return crc ^ 0xFFFFFFFFUL;
}

static inline uint32_t Ror (uint32_t num, int shift)
{
    return (num >> shift) | (num << ((int) sizeof (num) * 8 - shift));
}

static inline uint32_t Rol (uint32_t num, int shift)
{
    return (num << shift) | (num >> ((int) sizeof (num) * 8 - shift));
}
