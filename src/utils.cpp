#include "utils.h"

char* GetFileContent(const char* filename)
{
    assert(filename);

    FILE* file = fopen(filename, "rb");

    int file_size = GetFileSize(file);

    char* buffer = (char*) calloc(file_size + 1, sizeof(char));
    assert(buffer);

    buffer[file_size] = '\0';
    size_t size = fread (buffer, sizeof(char), file_size, file);

    fclose(file);

    return buffer;
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

int MyStrcmp (const char str1[WORD_LEN], const char str2[WORD_LEN])
{
    assert (str1);
    assert (str2);

    int res = 0;

    asm(".intel_syntax noprefix\n\t"
        "xor rax, rax\n\t"
        "vmovdqu ymm1, [%1]\n\t"
        "vpcmpeqb ymm2, ymm1, [%2]\n\t"
        "vpmovmskb ebx, ymm2\n\t"
        "cmp ebx, 0xffffffff\n\t"
        "je .EndLabel\n\t"
        "inc rax\n\t"

        ".EndLabel:\n\t"
        ".att_syntax\n"
        : "=r" (res)
        : "r" (str1), "r" (str2)
        : "ymm1", "ymm2", "ebx"
    );

    return res;
}
