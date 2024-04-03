#include "hashtable.h"

int main ()
{
    uint32_t (*hash_functions[]) (const char*, size_t) =
    {
        HashZero, HashFirstLetter, HashStrlen,
        HashAsciiSum, HashAsciiSumDivStrlen, HashRor, HashRol
    };

    const char* csv_files[] =
    {
        "data/hash0.csv", "data/hash[0].csv", "data/strlen.csv",
        "data/ascii.csv", "data/asciidivlen.csv", "data/ror.csv", "data/rol.csv"
    };

    for (size_t i = 0; i < sizeof (hash_functions) / sizeof (*hash_functions); i++)
    {
        HashTable* hash_t = HashTableCtor (HASH_T_INITIAL_SIZE, hash_functions[i]);
        FillHashTable (hash_t, CONTENT_FILE);
        DumpHashCsv (hash_t, csv_files[i]);
        PrintDispLoadFactor (hash_t);
        HashTableDtor (hash_t);
    }

    return 0;
}
