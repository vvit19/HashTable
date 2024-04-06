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

int mystrcmp (const char* str1, const char* str2)
{
    assert (str1);
    assert (str2);

    int res = 0;

    asm(".intel_syntax noprefix\n\t"

        "vmovdqu ymm1, [%1]\n\t"
        "vmovdqu ymm2, [%2]\n\t"
        "vpcmpeqb ymm3, ymm1, ymm2\n\t"
        "vpmovmskb eax, ymm3\n\t"
        "cmp eax, 0xffffffff\n\t"
        "jne .not_equal\n\t"
        "xor rax, rax\n\t"
        "jmp .EndLabel\n\t"
        ".not_equal:\n\t"
        "mov rax, 1\n\t"
        ".EndLabel:\n\t"

        ".att_syntax\n"
        : "=r" (res)
        : "r" (str1), "r" (str2)
        : "ymm1", "ymm2", "ymm3"
    );

    return res;
}
