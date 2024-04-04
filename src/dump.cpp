#include "hashtable.h"

void DumpHashCsv (HashTable* hash_t, const char* dump_filename)
{
    assert (hash_t);
    assert (dump_filename);

    FILE* dump_file = fopen (dump_filename, "w");

    for (size_t i = 0; i < hash_t->size; i++)
    {
        fprintf (dump_file, "%lu, %d \n", i, hash_t->content[i].size);
    }

    fclose (dump_file);
}

void DumpTableTxt (HashTable* hash_t, const char* dump_filename)
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

void PrintDispLoadFactor (HashTable* hash_t)
{
    assert (hash_t);

    size_t size = hash_t->size;
    int sum = 0;
    int no_zero_sizes_cnt = 0;

    for (size_t i = 0; i < size; i++)
    {
        int cur_list_size = hash_t->content[i].size;
        if (cur_list_size > 0)
        {
            ++no_zero_sizes_cnt;
            sum += cur_list_size;
        }
    }

    double mid_val = (double) sum / (double) size;
    double load_factor = (double) sum / (double) no_zero_sizes_cnt;

    double dispertion = 0;
    for (size_t i = 0; i < size; i++)
    {
        int cur_list_size = hash_t->content[i].size;
        dispertion += (cur_list_size - mid_val) * (cur_list_size - mid_val);
    }
    dispertion /= (double) size;

    printf ("load factor = %lf \n"
            "dispertion = %lf \n",
            load_factor, dispertion);
}
