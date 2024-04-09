#ifndef UTILS_H
#define UTILS_H

#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <cctype>

#define RED_COLOR   "\033[91m"
#define RESET_COLOR "\x1b[0m"

#define FREE(ptr) \
    free (ptr); \
    ptr = nullptr;

const size_t      WORD_LEN            = 32;

struct Text
{
    char* buffer;
    size_t* words_len;
    size_t nlines;
};

enum ExitCodes
{
    NO_ERR,
    ERR
};

#define NO_PROPER_CASE_FOUND \
    fprintf (stderr, "Default case reached in file: %s, function: %s, line: %d\n", \
                          __FILE__, __PRETTY_FUNCTION__, __LINE__)

#define SYNT_ASSERT(exit_code, ...) \
    if (!(__VA_ARGS__)) { \
        fprintf (stderr, "%s""Syntax assertion failed in file: %s:%d, function: %s\n" \
                         "Statement (" #__VA_ARGS__ ") is false\n\n %s",  \
                         RED_COLOR, __FILE__, __LINE__, __PRETTY_FUNCTION__, RESET_COLOR); *exit_code = ERR; }

char*  GetFileContent (const char* filename);
int    CalcNlines     (char* buffer);
int    GetFileSize    (FILE* file);
bool   IsEqual        (double a, double b);
int    SkipSpaces     (char* buffer, int i);
int    Factorial      (int n);
bool   IsInt          (double n);
Text*  GetAlignedFileContent (const char* filename);
void   TextFree              (Text* text);

#endif
