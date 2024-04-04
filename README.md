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

uint32_t HashRol (const char* word, size_t len)
{
    assert (word);

    uint32_t hash_value = 0;
    for (size_t i = 0; i < len; i++) hash_value = Rol (hash_value, 1) ^ (uint32_t) word[i];

    return hash_value % HASH_T_INITIAL_SIZE;
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
uint32_t HashCRC32 (const char* word, size_t len)
{
    assert (word);

	uint32_t crc_table[256] = {}, crc = 0;

	for (uint32_t i = 0; i < 256; ++i)
	{
		crc = i;
		for (size_t cnt = 0; cnt < 8; cnt++)
			crc = crc & 1 ? (crc >> 1) ^ 0xEDB88320UL : crc >> 1;

		crc_table[i] = crc;
	};
	crc = 0xFFFFFFFFUL;

    for (size_t i = 0; len > 0; --len, ++i)
    {
        crc = crc_table[(crc ^ (uint32_t) word[i]) & 0xFF] ^ (crc >> 8);
    }

	return (crc ^ 0xFFFFFFFFUL) % HASH_T_SIZE;
}
~~~

</details>

<img src= "https://github.com/vvit19/HashTable/blob/master/data/crc32.png" width="500px"/>

> Load factor = 2,69

> Dispersion = 2,50

Имеем впечатляющие показатели load factor и дисперсии (самые низкие по всем рассматриваемым хеш-функциям).

### Вывод

Идеальным показателем load factor является:
load factor $=$ Число ключей $/$ Размер хеш-таблицы $= 4407 / 2003 = 2,20 $

Благодаря Crc32 Hash получен наиболее близкий к этому значению результат (load factor = 2,69), а также минимальная дисперсия (dispersion = 2,50). При рассмотрении вопросов оптимизации будем использовать именно эту хеш-функцию.
