#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "list.h"
#include "utils.h"

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cassert>

const char* const CONTENT_FILE     = "content.txt";
const char* const DUMP_FILE        = "dump.txt";
const uint32_t HASH_T_INITIAL_SIZE = 100001;
const size_t MAX_WORD_LEN          = 100;

struct HashTable
{
    List* content;
    uint32_t (*hash_function) (const char*, size_t);
    size_t size;
};

void       FillHashTable (HashTable* hash_t, const char* filename);
HashTable* HashTableCtor (size_t hash_t_size, uint32_t (*hash_function) (const char*, size_t));
void       HashTableDtor (HashTable* hash_t);

uint32_t HashZero (const char* word, size_t len);
uint32_t HashFirstLetter (const char* word, size_t len);
uint32_t HashStrlen (const char* word, size_t len);
uint32_t HashAsciiSum (const char* word, size_t len);
uint32_t HashAsciiSumDivStrlen (const char* word, size_t len);
uint32_t HashRor (const char* word, size_t len);
uint32_t HashRol (const char* word, size_t len);

void DumpTable (HashTable* hash_t, const char* dump_filename);

#endif
