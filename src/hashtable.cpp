#include "hashtable.h"

static void SendWordToHashTable (HashTable* hash_t, const char* word, size_t len);
static bool CheckRepeat         (List* list, const char* word, size_t len);
static inline uint32_t Ror      (uint32_t num, int shift);
static inline uint32_t Rol      (uint32_t num, int shift);

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
        SendWordToHashTable (hash_t, word, len);
        buffer += len + 1;
    }

    free (buffer_ptr);
}

static void SendWordToHashTable (HashTable* hash_t, const char* word, size_t len)
{
    assert (word);

    uint32_t hash_value = hash_t->hash_function (word, len);
    List* hash_list = &hash_t->content[hash_value];
    if (CheckRepeat (hash_list, word, len)) return;

    char* word_ptr = (char*) calloc (1, len + 1);
    strncpy (word_ptr, word, len);

    InsertTail (&hash_t->content[hash_value], word_ptr);
}

static bool CheckRepeat (List* list, const char* word, size_t len)
{
    assert (list);
    assert (word);

    Node* nodes_array = list->nodes;
    int cur_node_index = list->head;

    while (cur_node_index != 0)
    {
        Node cur_node = nodes_array[cur_node_index];
        if (strncmp (word, cur_node.value, len) == 0) return true;
        cur_node_index = cur_node.next;
    }

    return false;
}

uint32_t HashZero (const char* /*word*/, size_t /*len*/)
{
    return 0;
}

uint32_t HashFirstLetter (const char* word, size_t /*len*/)
{
    assert (word);

    return word[0];
}

uint32_t HashStrlen (const char* /*word*/, size_t len)
{
    return (uint32_t) len;
}

uint32_t HashAsciiSum (const char* word, size_t len)
{
    assert (word);

    uint32_t sum = 0;
    for (size_t i = 0; i < len; i++) sum += word[i];

    return sum;
}

uint32_t HashAsciiSumDivStrlen (const char* word, size_t len)
{
    assert (word);

    uint32_t sum = HashAsciiSum (word, len);

    return sum / (uint32_t) len;
}

uint32_t HashRor (const char* word, size_t len)
{
    assert (word);

    uint32_t hash_value = 0;
    for (size_t i = 0; i < len; i++) hash_value = Ror (hash_value, 1) ^ word[i];

    return hash_value % HASH_T_INITIAL_SIZE;
}

uint32_t HashRol (const char* word, size_t len)
{
    assert (word);

    uint32_t hash_value = 0;
    for (size_t i = 0; i < len; i++) hash_value = Rol (hash_value, 1) ^ word[i];

    return hash_value % HASH_T_INITIAL_SIZE;
}

static inline uint32_t Ror (uint32_t num, int shift)
{
    return (num >> shift) | (num << (32 - shift));
}

static inline uint32_t Rol (uint32_t num, int shift)
{
    return (num << shift) | (num >> (32 - shift));
}

void DumpTable (HashTable* hash_t, const char* dump_filename)
{
    assert (hash_t);
    assert (dump_filename);

    FILE* dump_file = fopen (dump_filename, "w");

    for (size_t hash = 0; hash < hash_t->size; hash++)
    {
        fprintf (dump_file, "HASH = %lu: \n", hash);

        List cur_list = hash_t->content[hash];
        int cur_list_index = cur_list.head;
        while (cur_list_index != 0)
        {
            fprintf (dump_file, "%s \n", cur_list.nodes[cur_list_index].value);
            cur_list_index = cur_list.nodes[cur_list_index].next;
        }

        fprintf (dump_file, "------------------------------- \n");
    }
}
