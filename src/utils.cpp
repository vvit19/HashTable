#include "utils.h"
#include <cstddef>

const int CACHE_LINE_SIZE = 32;

char* GetFileContent(const char* filename)
{
    assert(filename);

    FILE* file = fopen(filename, "rb");

    int file_size = GetFileSize(file);

    char* buffer = (char*) calloc (file_size + 1, sizeof(char));
    assert(buffer);

    buffer[file_size] = '\0';
    size_t size = fread (buffer, sizeof(char), file_size, file);

    fclose(file);

    return buffer;
}

Text* GetAlignedFileContent (const char* filename)
{
    assert (filename);

    char* temp_buffer = GetFileContent(filename);
    char* start_buffer = temp_buffer;
    int nwords = CalcNlines(temp_buffer);

    Text* text = (Text*) calloc (1, sizeof (Text));

    char* buffer = (char*) aligned_alloc (CACHE_LINE_SIZE, nwords * CACHE_LINE_SIZE);
    size_t* words_len = (size_t*) calloc (nwords, sizeof (size_t));

    text->nlines = nwords;
    text->buffer = buffer;
    text->words_len = words_len;

    char* buffer_start = buffer;
    memset (buffer, 0, nwords * CACHE_LINE_SIZE);

    char word[CACHE_LINE_SIZE] = "";
    for (size_t cnt = 0; cnt < nwords; cnt++)
    {
        sscanf (temp_buffer, "%s", word);
        size_t len = strlen (word);
        strncpy (buffer, word, len);
        buffer += CACHE_LINE_SIZE;
        words_len[cnt] = len;
        temp_buffer += len + 1;
    }

    free (start_buffer);
    return text;
}

int GetFileSize(FILE* file)
{
    assert(file);

    fseek(file, 0, SEEK_END);
    int position = (int) ftell(file);
    fseek(file, 0, SEEK_SET);

    return position;
}

int CalcNlines(char* buffer)
{
    assert(buffer);

    int nlines = 0;
    char* temp = buffer;

    while ((temp = strchr(temp, '\n')) != nullptr)
    {
        nlines++;
        temp++;
    }

    return nlines;
}

bool IsEqual(double a, double b)
{
    const double EPS = 1e-10;
    return fabs(a - b) < EPS;
}

int SkipSpaces (char* buffer, int i)
{
    assert (buffer);

    while (isspace (buffer[i])) i++;

    return i;
}

int Factorial (int n)
{
    if (n == 1 || n == 0) return 1;
    return n * Factorial (n - 1);
}

bool IsInt (double n)
{
    return IsEqual (n, round (n));
}

void TextFree (Text* text)
{
    assert (text);

    free (text->buffer);
    text->buffer = nullptr;

    free (text->words_len);
    text->words_len = nullptr;

    text->nlines = 0;
    free (text);
}
