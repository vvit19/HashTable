# Hash Table
## Описание проекта
Данный проект можно разбить на 2 части:
1) исследование различных хеш-функций и выбор наиболее оптимальной, то есть имеющей минимальный load factor и минимальную дисперсию
2) оптимизация работы функции поиска по ключу в хеш-таблице
## Что такое хеш-таблица?
Хеш-таблица - это структура данных, хранящая пары элементов (ключ, значение). Выполняет три основные операции:
добавляет ключ, удаляет ключ, выполняет поиск по ключу. Значение для ключа равно числу, которое возвращает хеш-функция (обычно значение зависит от ключа). Хранятся ключи в связном списке: хеш-таблица хранит массив связных списков, индекс списка в массиве равен значению хеша для ключа. Таким образом, если хеш-функция возвращает уникальные значения для разных ключей, поиск элемента (ключа) в хеш-таблице имеет асимптотическую сложность О(1).

<img src= "https://github.com/vvit19/HashTable/blob/master/data/hashtwiki.png" width="500px"/>

В рамках нашей задачи ключами являются слова из произведения У.Шекспира "Гамлет".

## 1 часть: исследование хеш-функций
### Хеш-функция возвращает 0

Очевидно, что данная хеш-функция является ужасной, но в образовательных целях исследована и она.

<img src= "https://github.com/vvit19/HashTable/blob/master/data/hash0.png" width="500px"/>

> Load factor = 4407

> Dispersion = 63056,63

### Хеш-функция возвращает ASCII-code первого символа слова

<img src= "https://github.com/vvit19/HashTable/blob/master/data/hash0ascii.png" width="500px"/>

> Load factor = 169,5

> Dispersion = 3389,68

Дяже при небольшом размере хеш-таблицы получен высокий load factor, то есть большое число коллизий. Поэтому данная хеш-функция неэффективна.

### Хеш-функция возвращает длину слова

<img src= "https://github.com/vvit19/HashTable/blob/master/data/strlen.png" width="500px"/>

> Load factor = 348,14

> Dispersion = 10271,12

Load factor и дисперсия даже больше, чем для предыдущей хеш-функции. Данная хеш-функция также неэффективна.

### Хеш-функция возвращает сумму ASCII-кодов символов слова

<img src= "https://github.com/vvit19/HashTable/blob/master/data/ascii300.png" width="500px"/>

> Load factor = 16,03

> Dispersion = 99,93

У этой хеш-фукнции довольно низкие значения load factor и дисперсии. Может показаться, что она оптимальна, но посмотрите на гистограмму при большем размере хеш-таблицы:

<img src= "https://github.com/vvit19/HashTable/blob/master/data/ascii2000.png" width="500px"/>

> Load factor = 5,56

> Dispersion = 21,73

Видно, что в общем случае она не такая эффективная из-за ограниченности суммы ASCII-кодов, многие значения хеш-функции просто не достигаются, при еще больших размерах хеш-таблицы будут увеличиваться дисперсия и load factor. Однако она очевидно эффективнее предложенных ранее хеш-функций.

### ROR hash

Принцип работы данной хеш-функции: создается цикл длины, равной длине ключа. В цикле выполняется битовый сдвиг значения хеша на 1 вправо. Затем делается XOR с текущим символом слова (ключа). Полученное значение присваивается хешу, цикл выполняется далее.

<details>

<summary>ROR hash implementation</summary>

~~~ C++

uint32_t HashRor (const char* word, size_t len)
{
    uint32_t hash_value = 0;
    for (size_t i = 0; i < len; i++) hash_value = Ror (hash_value, 1) ^ (uint32_t) word[i];

    return hash_value;
}

static inline uint32_t Ror (uint32_t num, int shift)
{
    return (num >> shift) | (num << ((int) sizeof (num) * 8 - shift));
}

~~~

</details>

<img src= "https://github.com/vvit19/HashTable/blob/master/data/ror.png" width="500px"/>

> Load factor = 3,12

> Dispersion = 6,79

Показатели load factor и дисперсии самые оптимальные среди всех перечисленных выше функций.

Интересный факт: ассемблерный листинг ```Ror```:

<img src= "https://github.com/vvit19/HashTable/blob/master/profilerdata/ror.png" width="500px"/>

С включенной оптимизацией -О3 компилятор распознал функцию ```Ror``` и подставил ассемблерный стандартный ```ror```.

### ROL hash

Алгоритм данной хеш-функции аналогичен алгоритму ROR hash, только здесь выполняется сдвиг влево, а не вправо.

<img src= "https://github.com/vvit19/HashTable/blob/master/data/rol.png" width="500px"/>

> Load factor = 2,93

> Dispersion = 4,17

Показатели еще лучше, чем у ROR hash, пока это - самая эффективная хеш-функция.

### Crc32 Hash

Принцип работы этой хеш-функции можно узнать здесь: [CRC32_wikipedia](https://ru.wikipedia.org/wiki/%D0%A6%D0%B8%D0%BA%D0%BB%D0%B8%D1%87%D0%B5%D1%81%D0%BA%D0%B8%D0%B9_%D0%B8%D0%B7%D0%B1%D1%8B%D1%82%D0%BE%D1%87%D0%BD%D1%8B%D0%B9_%D0%BA%D0%BE%D0%B4).

<details>

<summary>Crc32 hash implementation</summary>

~~~ C++
uint32_t HashCrc32(const char* word, size_t len)
{
    uint32_t crc = 0;

    crc = 0xFFFFFFFF;

    for (size_t i = 0; i < len; i++)
    {
        crc = crc32_tab[(crc ^ word[i]) & 0xFF] ^ (crc >> 8);
    }

    return crc ^ 0xFFFFFFFF;
}
~~~

</details>

<img src= "https://github.com/vvit19/HashTable/blob/master/data/crc32.png" width="500px"/>

> Load factor = 2,69

> Dispersion = 2,50

Получены впечатляющие показатели load factor и дисперсии (самые низкие по всем рассматриваемым хеш-функциям).

### Вывод

Идеальный показатель load factor вычисляется как отношение числа ключей к размеру хеш-таблицы (в таком случае имеем дисперсию, равную 0). Так, он равен: $\frac{4407}{2003} = 2,20$. Благодаря Crc32 Hash получен наиболее близкий к этому значению результат (load factor = 2,69), а также минимальная дисперсия (dispersion = 2,50). При рассмотрении вопросов оптимизации будем использовать именно эту хеш-функцию.

## 2 часть: оптимизации функции поиска элемента в хеш-таблице

### Параметры системы и методика тестирования

- **OS**: Ubuntu 22.04.4 LTS
- **CPU**: Intel(R) Core(TM) i7-10510U CPU @ 1.80GHz
- **Compiler**: gcc 11.4.0
- **Compiler flags**: -g -O3 -mavx2
- **CPU temperature**: 40 $\pm$ 3 °C

Тестирование заключалось в поиске в хеш-таблице слов из текста. Для расчета времени исполнения функции поиска использован профилировщик ```callgrind```.

Примечание: температура процессора указана, чтобы показать, что в процессе измерений не возникало троттлинга - эффекта пропуска части тактов процессора при его перегреве.

### Использование памяти в программе

В программе хранится много указателей на строки (для заполнения хеш-таблицы и для тестирования). Будем хранить слова из текстов в памяти подряд, выделяя под них по 32 байта (поскольку обычно длина слов явно меньше 32 букв) и выравнивая их. Это выгодно по 2 причинам. 1 причина: для архитектуры x86-64 кеш-линии имеют размер 64 байта. Таким образом, если размещать по 2 32-байтовых выровненных слова подряд, процессор c большей вероятностью переместит их в кеш. 2 причина заключается в том, что такое хранение поможет в дальнейшем применять для сравнения строк YMM регистры процессора, используя AVX инструкции.

Отчет ```callgrind``` об использовании кеша:

<img src= "https://github.com/vvit19/HashTable/blob/master/profilerdata/cache.png" width="500px"/>

Показания промахов кеша очень низкие, что показывает, что в программе эффективно используется память.

### Базовая версия

Отчет профилировщика по времени работы функции поиска:

<img src= "https://github.com/vvit19/HashTable/blob/master/profilerdata/base.png" width="500px"/>

| Оптимизации | Время <br> CPU cycles    | Абсолютное ускорение | Относительное ускорение |
| :---------: | :------: | :----------: | :------------: |
| Baseline    | 3 015 051 475 | 1 | 1 |

Самой затратной по времени фукнцией является ```HashCrc32```, она будет оптимизирована в первую очередь.

### HashCrc32 optimization

Вообще говоря, хеш-функция ```Crc32``` была подобрана неслучайно. Дело в том, что можно использовать встроенную ассемблерную функцию, которая считает этот хеш:

~~~ asm
global AsmHashCrc32

section .text

AsmHashCrc32:
        mov eax, 0xEDB88320
        push rdi
        add rdi, rsi    ; rdi += strlen
        lea rsi, [rdi]
        pop rdi
.While:
        crc32 eax, byte [rdi]
        inc rdi
        cmp rdi, rsi
        jl .While
        ret

~~~

Отчет профилировщика:

<img src= "https://github.com/vvit19/HashTable/blob/master/profilerdata/asmhash.png" width="500px"/>

| Оптимизации | Время <br> CPU cycles   | Абсолютное ускорение | Относительное ускорение |
| :---------: | :------: | :----------: | :------------: |
| Baseline    | 3 015 051 475 | 1 | 1 |
| Assembly Hash | 2 549 774 952 | 1,18 | 1,18 |

Непосредственно функция расчета хеша ускорилась на 54 %, и теперь самыми затратными по времени являются непосредственно функция поиска и ```strcmp```. Можно оптимизировать функцию ```strcmp```, используя ассемблерную вставку, и сделать саму функцию ```inline```. Таким образом, также ускорится и ```SearchElemHT```, ведь теперь в ней не будет тратиться много времени на вызов ```strcmp```.


### Inline assembly strcmp optimization

Здесь как раз и пригодится, что под слова в памяти выделено по 32 байта. Благодаря этому можно использовать ```AVX``` инструкции:

~~~ C++

inline int InlineAsmStrcmp (const char str1[WORD_LEN], const char str2[WORD_LEN])
{
    int res = 0;

    asm (".intel_syntax noprefix\n"
         "vmovdqa ymm1, YMMWORD PTR [%1]\n"
         "vpcmpeqb ymm0, ymm1, YMMWORD PTR [%2]\n"
         "vpmovmskb %0, ymm0\n"
         ".att_syntax prefix\n"
         : "=r" (res) : "r" (str1), "r" (str2) : "ymm0", "ymm1", "cc");

    return res;
}

~~~

Отчет профилировщика:

<img src= "https://github.com/vvit19/HashTable/blob/master/profilerdata/inlinecmp.png" width="500px"/>

| Оптимизации | Время <br> CPU cycles   | Абсолютное ускорение | Относительное ускорение |
| :---------: | :------: | :----------: | :------------: |
| Baseline    | 3 015 051 475 | 1 | 1 |
| Assembly Hash | 2 549 774 952 | 1,18 | 1,18 |
| Inline asm strcmp | 1 654 515 435 | 1,82 | 1,54 |

Использование ассемблерной вставки позволило оптимизировать 2 функции сразу. Однако в функции ```SearchElemHT``` все еще тратится много времени на вызов ```AsmHashCrc32```. Она определена в другом ассемблерном файле, и компилятор не может ее заинлайнить. Но это исправимо: существует интринскик, считающий ```Crc32```. Тогда можно использовать этот интринсик и переписать ```AsmHashCrc32``` уже на Си, чтобы заинлайнить ее вручную.

### Intrinsic HashCrc32 optimization

~~~ C++

inline uint32_t IntrinsicHashCrc32 (const char* word, size_t len)
{
	uint32_t hash = 0xEDB88320;

	for (size_t i = 0; i < len; i++)
		hash = _mm_crc32_u8 (hash, word[i]);

	return hash;
}

~~~

Отчет профилировщика:

<img src= "https://github.com/vvit19/HashTable/blob/master/profilerdata/inlinehash.png" width="500px"/>

| Оптимизации | Время <br> CPU cycles   | Абсолютное ускорение | Относительное ускорение |
| :---------: | :------: | :----------: | :------------: |
| Baseline    | 3 015 051 475 | 1 | 1 |
| Assembly Hash | 2 549 774 952 | 1,18 | 1,18 |
| Inline asm strcmp | 1 654 515 435 | 1,82 | 1,54 |
| Intrinsic Hash | 1 392 389 225 | 2,17 | 1,19 |

Дальше оптимизировать функцию поиска не получится, а следующая после нее по времени функция выполняется в 6 раз быстрее. А следовательно, ускорять ее смысла нет, ведь на общее время программы ее оптимизация повлияет слабо.

### Отчет профилировщика по времени работы всей программы

| Оптимизации | Время <br> CPU cycles   | Абсолютное ускорение | Относительное ускорение |
| :---------: | :------: | :----------: | :------------: |
| Baseline    | 3 431 395 456 | 1 | 1 |
| Assembly Hash | 2 965 409 775 | 1,16 | 1,16 |
| Inline asm strcmp | 2 069 252 958  | 1,66 | 1,43 |
| Intrinsic Hash | 1 807 040 171 | 1,90 | 1,14 |

### Вывод

Благодаря профилировщику можно искать узкие места в программе, которые можно оптимизировать. Таким образом, точечными низкоуровневыми оптимизациями функция поиска была ускорена в 2,17 раз (а вся программа в 1,90 раз). Это показывает, что использование только флагов оптимизаций недостаточно.

При этом всегда стоит помнить, что большое число ассемблерных оптимизаций вредит переносимости кода и его читаемости. Поэтому в заключении посчитаем коэффициент [Деда](https://github.com/ded32):

DED_coeff = $\frac{\text{ускорение программы}}{число ассемблерных строк} * 1000 = \frac{2170}{11} = 197,27$

## Список литературы и ресурсов

- Randal Bryant, David O'Hallaron - Computer Systems: A Programmer's Perspective 3rd Edition
- Valgrind - https://valgrind.org/
- Compiler Explorer - https://godbolt.org/
- Mirror of Intel® Intrinsics Guide - https://www.laruence.com/sse
- Wikipedia - [HashTable](https://en.wikipedia.org/wiki/Hash_table), [Crc32](https://ru.wikipedia.org/wiki/%D0%A6%D0%B8%D0%BA%D0%BB%D0%B8%D1%87%D0%B5%D1%81%D0%BA%D0%B8%D0%B9_%D0%B8%D0%B7%D0%B1%D1%8B%D1%82%D0%BE%D1%87%D0%BD%D1%8B%D0%B9_%D0%BA%D0%BE%D0%B4)
