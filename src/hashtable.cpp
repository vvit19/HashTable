#include "hashtable.h"
#include "utils.h"
#include <cstddef>

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

    FREE (hash_t->content);

    hash_t->hash_function = nullptr;
    hash_t->size = 0;

    FREE (hash_t);
}

void FillHashTable (HashTable* hash_t, Text* text)
{
    assert (hash_t);
    assert (filename);

    size_t words_num = text->nlines;
    char* buffer = text->buffer;

    for (size_t i = 0; i < words_num; ++i)
    {
        InsertValue (hash_t, buffer, text->words_len[i]);
        buffer += WORD_LEN;
    }
}

void InsertValue (HashTable* hash_t, const char* word, size_t len)
{
    assert (hash_t);
    assert (word);

    uint32_t hash_value = hash_t->hash_function (word, len) % hash_t->size;

    List* cur_list = &hash_t->content[hash_value];
    if (CheckRepeat (cur_list, word)) return;

    InsertTail (cur_list, word);
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
         "vmovdqa ymm1, YMMWORD PTR [%1]\n"
         "vpcmpeqb ymm0, ymm1, YMMWORD PTR [%2]\n"
         "vpmovmskb %0, ymm0\n"
         ".att_syntax prefix\n"
         : "=r" (res) : "r" (str1), "r" (str2) : "ymm0", "ymm1", "cc");

    return res;     // returns -1 if equal
}
