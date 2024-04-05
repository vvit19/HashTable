#include "hashtable.h"

static int CheckRepeat (List* list, const char* word);

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

    uint32_t hash_value = hash_t->hash_function (word, len);
    List* cur_list = &hash_t->content[hash_value];

    return (bool) CheckRepeat (cur_list, word);
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

    char word[MAX_WORD_LEN] = "";
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

    uint32_t hash_value = hash_t->hash_function (word, len);
    List* cur_list = &hash_t->content[hash_value];
    if (CheckRepeat (cur_list, word)) return;

    char* word_ptr = (char*) calloc (1, len + 1);
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
        if (strcmp (word, cur_node.value) == 0) return cur_node_index;
        cur_node_index = cur_node.next;
    }

    return 0;
}

void DeleteValue (HashTable* hash_t, const char* word, size_t len)
{
    assert (hash_t);
    assert (word);

    uint32_t hash_value = hash_t->hash_function (word, len);
    List* cur_list = &hash_t->content[hash_value];

    int position = CheckRepeat (cur_list, word);
    if (position) ListDelete (cur_list, position);
}
