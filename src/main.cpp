#include "hashtable.h"

int main ()
{
    #ifdef HASH_FUNCTIONS_CMP

        uint32_t (*hash_functions[]) (const char*, size_t) =
        {
            HashZero, HashFirstLetter, HashStrlen, HashAsciiSum,
            HashAsciiSumDivStrlen, HashRor, HashRol, HashCrc32
        };

        const char* csv_files[] =
        {
            "data/hash0.csv", "data/hash[0].csv", "data/strlen.csv", "data/ascii.csv",
            "data/asciidivlen.csv", "data/ror.csv", "data/rol.csv", "data/crc32.csv"
        };

        for (size_t i = 0; i < sizeof (hash_functions) / sizeof (*hash_functions); i++)
        {
            HashTable* hash_t = HashTableCtor (HASH_T_SIZE, hash_functions[i]);
            FillHashTable (hash_t, CONTENT_FILE);
            DumpHashCsv (hash_t, csv_files[i]);
            printf ("\n%lu) \n", i);
            PrintDispLoadFactor (hash_t);
            HashTableDtor (hash_t);
        }

    #else

        HashTable* hash_t = HashTableCtor(HASH_T_SIZE, AsmHashCrc32);
        FillHashTable (hash_t, CONTENT_FILE);

        size_t finded_words = RunUnitTests (hash_t, UNIT_TESTS_FILE);
        printf ("FINDED_WORDS = %lu \n", finded_words);

        HashTableDtor (hash_t);

    #endif

    return 0;
}
