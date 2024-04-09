#include "hashtable.h"
#include "utils.h"
#include <cstddef>

struct String
{
    char* string;
    size_t len;
};

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
            FindWord (hash_t, word, len);

        if (FindWord (hash_t, word, len))
            ++finded_words;

        word += WORD_LEN;
    }

    free (text->buffer);
    free (text->words_len);
    free (text);
    text = nullptr;

    return finded_words;
}
