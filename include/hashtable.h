#ifndef HASHTABLE_H
#define HASHTABLE_H

// #define HASH_FUNCTIONS_CMP
#define NDEBUG

#include "list.h"
#include "utils.h"

#include <cstdint>
#include <x86intrin.h>

const char* const CONTENT_FILE        = "hamlet.txt";
const char* const TXT_DUMP_FILE       = "dump.txt";
const char* const UNIT_TESTS_FILE     = "unittests.txt";
const size_t      TESTS_NUM           = 10000;
const uint32_t    HASH_T_SIZE         = 2003;

struct HashTable
{
    List* content;
    uint32_t (*hash_function) (const char*, size_t);
    size_t size;
};

HashTable* HashTableCtor (size_t hash_t_size, uint32_t (*hash_function) (const char*, size_t));
void       FillHashTable (HashTable* hash_t, Text* text);
void       InsertValue   (HashTable* hash_t, const char* word, size_t len);
void       DeleteValue   (HashTable* hash_t, const char* word, size_t len);
bool       SearchElemHT  (HashTable* hash_t, const char* word, size_t len);
void       HashTableDtor (HashTable* hash_t);
int        CheckRepeat   (List* list, const char* word);

extern "C" uint32_t AsmStrcmp  (const char* str1, const char* str2);

extern "C" uint32_t AsmHashCrc32 (const char* word, size_t len);

uint32_t HashZero              (const char* word, size_t len);
uint32_t HashFirstLetter       (const char* word, size_t len);
uint32_t HashStrlen            (const char* word, size_t len);
uint32_t HashAsciiSum          (const char* word, size_t len);
uint32_t HashAsciiSumDivStrlen (const char* word, size_t len);
uint32_t HashRor               (const char* word, size_t len);
uint32_t HashRol               (const char* word, size_t len);
uint32_t HashCrc32             (const char* word, size_t len);

inline uint32_t IntrinsicHashCrc32 (const char* word, size_t len);
inline uint32_t IntrinsicHashCrc32 (const char* word, size_t len)
{
	uint32_t hash = 0xEDB88320;

	for (size_t i = 0; i < len; i++)
		hash = _mm_crc32_u8 (hash, word[i]);

	return hash;
}

inline uint32_t HashFunction (HashTable* hash_t, const char* word, size_t len)
{
    #ifdef HASH_FUNCTIONS_CMP
        return hash_t->hash_function (word, len);
    #else
        return IntrinsicHashCrc32 (word, len);
    #endif
}

void DumpTableTxt        (HashTable* hash_t, const char* dump_filename);
void DumpHashCsv         (HashTable* hash_t, const char* dump_filename);
void PrintDispLoadFactor (HashTable* hash_t);

size_t RunUnitTests      (HashTable* hash_t, const char* filename);

#endif
