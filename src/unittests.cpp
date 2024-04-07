#include "hashtable.h"
#include <cstddef>

struct String
{
    char* string;
    size_t len;
};

static String* ParseBuffer (char* buffer, size_t nlines);

size_t RunUnitTests (HashTable* hash_t, const char* filename)
{
    assert (hash_t);
    assert (filename);

    char* buffer = GetFileContent (filename);
    size_t words_num = CalcNlines (buffer);
    String* text  = ParseBuffer (buffer, words_num);

    size_t finded_words = 0;
    for (size_t i = 0; i < words_num; i++)
    {
        char* word = text[i].string;
        size_t len = text[i].len;

        // for (size_t k = 0; k < TESTS_NUM; k++)
        //     FindWord (hash_t, word, len);

        if (FindWord (hash_t, word, len))
            ++finded_words;
    }

    free (buffer);
    for (size_t i = 0; i < words_num; i++)
    {
        free (text[i].string);
        text[i].string = nullptr;
    }

    free (text);
    text = nullptr;

    return finded_words;
}

static String* ParseBuffer (char* buffer, size_t nlines)
{
    assert (buffer);

    String* text = (String*) calloc (nlines, sizeof (String));

    char word[WORD_LEN] = "";
    size_t i = 0;
    size_t len = 0;
    while (*buffer != '\0' && sscanf (buffer, "%s", word) != 0)
    {
        len = strlen (word);

        char* str = (char*) calloc (1, WORD_LEN);
        strncpy (str, word, len);

        text[i].string = str;
        text[i].len = len;

        buffer += len + 1;
        ++i;
    }

    return text;
}
