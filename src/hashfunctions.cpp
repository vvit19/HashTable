#include "hashtable.h"

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

static inline uint32_t Ror (uint32_t num, int shift)
{
    return (num >> shift) | (num << ((int) sizeof (num) * 8 - shift));
}

static inline uint32_t Rol (uint32_t num, int shift)
{
    return (num << shift) | (num >> ((int) sizeof (num) * 8 - shift));
}
