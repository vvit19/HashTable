#include "hashtable.h"

int main ()
{
    HashTable* hash_t = HashTableCtor (HASH_T_INITIAL_SIZE, HashAsciiSum);
    FillHashTable (hash_t, CONTENT_FILE);
    HashTableDtor (hash_t);

    return 0;
}
