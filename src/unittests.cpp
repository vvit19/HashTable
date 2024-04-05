#include "hashtable.h"
#include "utils.h"
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

        for (int i = 0; i < TESTS_NUM; i++)
            FindWord (hash_t, word, len);

        if (FindWord (hash_t, word, len))
            ++finded_words;
    }

    free (buffer);
    free (text);

    return finded_words;
}

static String* ParseBuffer (char* buffer, size_t nlines)
{
    assert (buffer);

    String* text = (String*) calloc (nlines, sizeof (String));

    char word[MAX_WORD_LEN] = "";
    size_t i = 0;
    size_t len = 0;
    while (*buffer != '\0' && sscanf (buffer, "%s", word) != 0)
    {
        len = strlen (word);

        text[i].string = buffer;
        text[i].len = len;

        buffer += len;
        *buffer++ = '\0';
        ++i;
    }

    return text;
}
