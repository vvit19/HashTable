#include "hashtable.h"

static int        CheckRepeat     (List* list, const char* word);
static inline int IntrinsicStrcmp (const char str1[WORD_LEN], const char str2[WORD_LEN]);
static inline int InlineAsmStrcmp (const char str1[WORD_LEN], const char str2[WORD_LEN]);

HashTable* HashTableCtor (size_t hash_t_size, uint32_t (*hash_function) (const char*, size_t))
{
    HashTable* hash_t = (HashTable*) calloc (1, sizeof (HashTable));
    hash_t->content = (List*) calloc (hash_t_size, sizeof (List));
    hash_t->hash_function = hash_function;
    hash_t->size = hash_t_size;

    for (size_t i = 0; i < hash_t_size; i++)
        ListCtor (&hash_t->content[i], MIN_CAPACITY);

    return hash_t;
}

bool FindWord (HashTable* hash_t, const char* word, size_t len)
{
    assert (hash_t);
    assert (word);

    uint32_t hash_value = hash_t->hash_function (word, len) % hash_t->size;

    return (bool) CheckRepeat (&hash_t->content[hash_value], word);
}

void HashTableDtor (HashTable* hash_t)
{
    assert (hash_t);

    for (size_t i = 0; i < hash_t->size; i++)
        ListDtor (&hash_t->content[i]);

    free (hash_t->content);
    hash_t->content = nullptr;

    hash_t->hash_function = nullptr;
    hash_t->size = 0;

    free (hash_t);
    hash_t = nullptr;
}

void FillHashTable (HashTable* hash_t, const char* filename)
{
    assert (hash_t);
    assert (filename);

    char* buffer = GetFileContent (filename);
    char* buffer_ptr = buffer;

    char word[WORD_LEN] = "";
    size_t len = 0;
    while (*buffer != '\0' && sscanf (buffer, "%s", word) != 0)
    {
        len = strlen (word);
        InsertValue (hash_t, word, len);
        buffer += len + 1;
    }

    free (buffer_ptr);
}

void InsertValue (HashTable* hash_t, const char* word, size_t len)
{
    assert (hash_t);
    assert (word);

    uint32_t hash_value = hash_t->hash_function (word, len) % hash_t->size;

    List* cur_list = &hash_t->content[hash_value];
    if (CheckRepeat (cur_list, word)) return;

    char* word_ptr = (char*) calloc (1, WORD_LEN);
    strncpy (word_ptr, word, len);

    InsertTail (cur_list, word_ptr);
}

static int CheckRepeat (List* list, const char* word)
{
    assert (list);
    assert (word);

    Node* nodes_array = list->nodes;
    int cur_node_index = list->head;

    while (cur_node_index != 0)
    {
        Node cur_node = nodes_array[cur_node_index];
        if (InlineAsmStrcmp (word, cur_node.value) == -1) return cur_node_index;
        // if (AsmStrcmp (word, cur_node.value) == -1) return cur_node_index;
        // if (strcmp (word, cur_node.value) == 0) return cur_node_index;
        cur_node_index = cur_node.next;
    }

    return 0;
}

void DeleteValue (HashTable* hash_t, const char* word, size_t len)
{
    assert (hash_t);
    assert (word);

    uint32_t hash_value = hash_t->hash_function (word, len) % hash_t->size;
    List* cur_list = &hash_t->content[hash_value];

    int position = CheckRepeat (cur_list, word);
    if (position) ListDelete (cur_list, position);
}

static inline int InlineAsmStrcmp (const char str1[WORD_LEN], const char str2[WORD_LEN])
{
    assert (str1);
    assert (str2);

    int res = 0;

    asm (".intel_syntax noprefix\n"
         "vmovdqu ymm1, YMMWORD PTR [%1]\n"
         "vpcmpeqb ymm0, ymm1, YMMWORD PTR [%2]\n"
         "vpmovmskb %0, ymm0\n"
         ".att_syntax prefix\n"
         : "=&r" (res) : "r" (str1), "r" (str2) : "ymm0", "ymm1", "cc");

    return res;     // returns -1 if equal
}

// --------------------------------------------------------
//                      Unit Tests
//---------------------------------------------------------

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

        for (size_t k = 0; k < TESTS_NUM; k++)
            FindWord (hash_t, word, len);

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
    return 0;
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
