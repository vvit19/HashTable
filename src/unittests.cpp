#include "hashtable.h"

size_t RunUnitTests (HashTable* hash_t, const char* filename)
{
    assert (hash_t);
    assert (filename);

    Text* text = GetAlignedFileContent(filename);
    size_t words_num = text->nlines;

    size_t finded_words = 0;
    char* word = text->buffer;
    for (size_t i = 0; i < words_num; i++)
    {
        size_t len = text->words_len[i];

        for (size_t k = 0; k < TESTS_NUM; k++)
            SearchElemHT (hash_t, word, len);

        if (SearchElemHT (hash_t, word, len))
            ++finded_words;

        word += WORD_LEN;
    }

    TextFree (text);

    return finded_words;
}
