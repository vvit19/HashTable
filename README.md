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
Очевидно, это - ужасная хеш-функция, но в образовательных целях исследуем и ее.

<img src= "https://github.com/vvit19/HashTable/blob/master/data/hash0.png" width="500px"/>

> Load factor = 4407

> Dispersion = 63056,63

### Хеш-функция возвращает ASCII-code первого символа слова

<img src= "https://github.com/vvit19/HashTable/blob/master/data/hash0ascii.png" width="500px"/>

> Load factor = 169,5

> Dispersion = 3389,68

Дяже при небольшом размере хеш-таблицы имеем высокий load-factor, то есть большое число коллизий. Поэтому данная хеш-функция неэффективна.

### Хеш-функция возвращает длину слова

<img src= "https://github.com/vvit19/HashTable/blob/master/data/strlen.png" width="500px"/>

> Load factor = 348,14

> Dispersion = 10271,12

Load factor и дисперсия даже больше, чем для предыдущей хеш-функции. Данная хеш-функция также неэффективна.

### Хеш-функция возвращает сумму ASCII-кодов символов слова

<img src= "https://github.com/vvit19/HashTable/blob/master/data/ascii300.png" width="500px"/>

> Load factor = 16,03

> Dispersion = 99,93

У данной хеш-фукнции довольно низкие значения load factor и дисперсии. Может показаться, что она оптимальна, но давайте на гистограмму при большем размере хеш-таблицы:

<img src= "https://github.com/vvit19/HashTable/blob/master/data/ascii2000.png" width="500px"/>

> Load factor = 5,56

> Dispersion = 21,73

Видно, что в общем случае она не такая эффективная из-за ограниченности суммы ASCII-кодов, многие значения хеш-функции просто не достигаются, при еще больших размерах хеш-таблицы будет увеличиваться дисперсия. Однако она очевидно эффективнее предложенных ранее хеш-функций.

### ROR hash

Принцип работы данной хеш-функции: создается цикл длины, равной длине ключа. В цикле выполняется битовый сдвиг значения хеша на 1 вправо. Затем делается XOR с текущим символом слова (ключа). Полученное значение присваивается хешу, цикл выполняется далее.

<details>

<summary>ROR hash implementation</summary>

~~~ C++

uint32_t HashRor (const char* word, size_t len)
{
    assert (word);

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

Имеем впечатляющие показатели load factor и дисперсии (самые низкие по всем рассматриваемым хеш-функциям).

### Вывод

Идеальный показатель load factor вычисляется как отношение числа ключей к размеру хеш-таблицы (в таком случае имеем дисперсию, равную 0). Так, он равен: $\frac{4407}{2003} = 2,20$. Благодаря Crc32 Hash получен наиболее близкий к этому значению результат (load factor = 2,69), а также минимальная дисперсия (dispersion = 2,50). При рассмотрении вопросов оптимизации будем использовать именно эту хеш-функцию.

## 2 часть: оптимизации функции поиска элемента в хеш-таблице
### Параметры системы

### Методика измерений
Все версии программы собраны с флагами -g -O3 -mavx2. Тестирование программы заключалось в поиске в хеш-таблице слов из текста. Для расчета времени исполнения функции поиска использован профилировщик ```callgrind```. Время указано в циклах процессора.

### Baseline

Отчет профилировщика:

<img src= "https://github.com/vvit19/HashTable/blob/master/profilerdata/base.png" width="500px"/>

| Оптимизации | Время    | Относительное ускорение |
| :---------: | :------: | :----------: |
| Baseline    | 30'598'051 | 1 |

Самыми затратными по времени фукнциями, которые вызывает функция поиска, являются ```HashCrc32``` и ```strcmp```. Их я и буду оптимизировать.

### HashCrc32 optimization

Вообще говоря, хеш-функция ```CRC32``` была подобрана неслучайно. Дело в том, что существует ассемблерная функция, которая считает этот хеш. Можно использовать AVX инструкции:

<details>

<summary>Intrinsic Crc32 hash implementation</summary>

~~~ C++
uint32_t IntrinsicHashCrc32 (const char* word, size_t len)
{
	uint32_t hash = 0xEDB88320;

	for (size_t i = 0; i < len; i++)
		hash = _mm_crc32_u8 (hash, word[i]);

	return hash;
}

~~~

</details>

Отчет профилировщика:

<img src= "https://github.com/vvit19/HashTable/blob/master/profilerdata/hash.png" width="500px"/>

| Оптимизации | Время    | Абсолютное ускорение | Относительное ускорение |
| :---------: | :------: | :----------: | :------------: |
| Baseline    | 30'598'051 | 1 | 1 |
| Intrinsic Hash | 26'162'333 | 1,17 | 1,17 |

Теперь большую часть времени работы функции поиска занимает ```strcmp```. Дальше будем ускорять ее.

### Strcmp optimization

Ускорим ```strcmp```, написав ее на ассемблере. Обычно слова имеют меньше 32 букв, поэтому в программе под них выделено по 32 байта памяти. Это сделано для того, чтобы сейчас написать ассемблерный код, используя AVX инструкции (YMM регистры занимают 32 байта).

<details>

<summary>Assembly strcmp</summary>

~~~ asm
global AsmStrcmp

section .text
AsmStrcmp:
        vmovdqu ymm1, yword [rdi]
        vpcmpeqb ymm0, ymm1, yword [rsi]
        vpmovmskb rax, ymm0
        ret

~~~

</details>

Отчет профилировщика:

<img src= "https://github.com/vvit19/HashTable/blob/master/profilerdata/asmcmp.png" width="500px"/>

| Оптимизации | Время    | Абсолютное ускорение | Относительное ускорение |
| :---------: | :------: | :----------: | :------------: |
| Baseline    | 30'598'051 | 1 | 1 |
| Intrinsic Hash | 26'162'333 | 1,17 | 1,17 |
| Assembly strcmp |
