#ifndef HASHTABLE_H
#define HASHTABLE_H

// #define HASH_FUNCTIONS_CMP
#include <cstddef>
#define NDEBUG

#include "list.h"
#include "utils.h"

#include <cstdint>
#include <x86intrin.h>

const char* const CONTENT_FILE        = "hamlet.txt";
const char* const TXT_DUMP_FILE       = "dump.txt";
const char* const UNIT_TESTS_FILE     = "unittests.txt";
const size_t      TESTS_NUM           = 100;
const uint32_t    HASH_T_SIZE         = 2003;
const size_t      MAX_WORD_LEN        = 100;

struct HashTable
{
    List* content;
    uint32_t (*hash_function) (const char*, size_t);
    size_t size;
};

HashTable* HashTableCtor (size_t hash_t_size, uint32_t (*hash_function) (const char*, size_t));
void       FillHashTable (HashTable* hash_t, const char* filename);
void       InsertValue   (HashTable* hash_t, const char* word, size_t len);
void       DeleteValue   (HashTable* hash_t, const char* word, size_t len);
bool       FindWord      (HashTable* hash_t, const char* word, size_t len);
void       HashTableDtor (HashTable* hash_t);

uint32_t HashZero              (const char* word, size_t len);
uint32_t HashFirstLetter       (const char* word, size_t len);
uint32_t HashStrlen            (const char* word, size_t len);
uint32_t HashAsciiSum          (const char* word, size_t len);
uint32_t HashAsciiSumDivStrlen (const char* word, size_t len);
uint32_t HashRor               (const char* word, size_t len);
uint32_t HashRol               (const char* word, size_t len);

extern "C" uint32_t AsmHashCrc32 (const char* word, size_t len);
uint32_t HashCrc32               (const char* word, size_t len);
uint32_t IntrinsicHashCrc32      (const char* word, size_t len);

void DumpTableTxt        (HashTable* hash_t, const char* dump_filename);
void DumpHashCsv         (HashTable* hash_t, const char* dump_filename);
void PrintDispLoadFactor (HashTable* hash_t);

size_t RunUnitTests      (HashTable* hash_t, const char* filename);

#endif
