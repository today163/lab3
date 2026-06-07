Документация библиотеки контейнеров и последовательностей

## Происхождение и внесённые изменения

Код основан на материалах 2-й лабораторной работы (базовые структуры данных).

Внесённые изменения по сравнению с исходной лабораторной:

- Добавлена поддержка итераторов через `IEnumerable<T>` / `IEnumerator<T>` с адаптером для range-based for.
- Реализованы immutable последовательности (`ImmutableArraySequence`, `ImmutableListSequence`) – все модифицирующие методы возвращают новый объект.
- Созданы адаптеры контейнеров на основе `MutableListSequence`: `Stack`, `Queue`, `Deque`.
- Реализован класс `Vector<T>` на базе `ImmutableArraySequence` с арифметическими операциями и лексикографическими сравнениями.
- Написаны модульные тесты (Google Test) для всех компонентов (файлы `test.cpp`, `newtest.cpp`).
- Обновлён `Makefile` для сборки двух тестовых программ и основной программы.

## Общие замечания

- **Исключения:** все методы, выполняющие доступ по индексу или удаление из пустого контейнера, выбрасывают `IndexOutOfRange` (унаследовано от `std::out_of_range`).
- **Потокобезопасность:** не гарантируется.
- **Итераторы:** каждый контейнер предоставляет `begin()` / `end()` для использования в range-based for.

---

## Файл `IEnumerable.hpp`

Определяет интерфейсы для итерируемых коллекций.

### Класс `IEnumerator<T>`

| Метод                       | Описание                                                                                  | Исключения                                           |
| --------------------------- | ----------------------------------------------------------------------------------------- | ---------------------------------------------------- |
| `virtual bool MoveNext()`   | Перемещает итератор к следующему элементу. Возвращает `false`, если элементов больше нет. | Нет                                                  |
| `virtual T Current() const` | Возвращает текущий элемент.                                                               | `IndexOutOfRange` если состояние итератора невалидно |
| `virtual void Reset()`      | Сбрасывает итератор в начальное положение (перед первым элементом).                       | Нет                                                  |

### Класс `EnumeratorAdapter<T>`

Адаптирует `IEnumerator<T>` к STL-подобному интерфейсу итератора.

| Метод                                               | Описание                                                     |
| --------------------------------------------------- | ------------------------------------------------------------ |
| `EnumeratorAdapter(IEnumerator<T>* e)`              | Принимает владение итератором и сразу вызывает `MoveNext()`. |
| `EnumeratorAdapter()`                               | Создаёт «конечный» итератор (`end`).                         |
| `EnumeratorAdapter& operator++()`                   | Перемещает на следующий элемент.                             |
| `T operator*() const`                               | Возвращает текущий элемент.                                  |
| `bool operator!=(const EnumeratorAdapter& o) const` | Сравнивает валидность итераторов.                            |

### Класс `IEnumerable<T>`

| Метод                                           | Описание                                              | Исключения  |
| ----------------------------------------------- | ----------------------------------------------------- | ----------- |
| `virtual IEnumerator<T>* GetEnumerator() const` | Создаёт новый итератор (требует удаления вызывающим). | `bad_alloc` |
| `EnumeratorAdapter<T> begin() const`            | Возвращает итератор на первый элемент.                | `bad_alloc` |
| `EnumeratorAdapter<T> end() const`              | Возвращает конечный итератор.                         | Нет         |

---

## Файл `exceptions.hpp`

Определяет пользовательское исключение.

```cpp
class IndexOutOfRange : public std::out_of_range {
    explicit IndexOutOfRange(const std::string& msg);
};
```

---

## Файл `DynamicArray.hpp`

Динамический массив с изменяемым размером (`capacity` увеличивается по геометрическому принципу, начальная ёмкость = 8).

### Публичные методы

| Метод                                                | Описание                                                                              | Исключения                                       |
| ---------------------------------------------------- | ------------------------------------------------------------------------------------- | ------------------------------------------------ |
| `DynamicArray(const T* items, int count)`            | Создаёт массив из `count` элементов (если `items == nullptr`, инициализирует нулями). | `IndexOutOfRange` при `count < 0`                |
| `DynamicArray(const DynamicArray& other)`            | Копирующий конструктор.                                                               | `bad_alloc`                                      |
| `DynamicArray& operator=(const DynamicArray& other)` | Присваивание.                                                                         | `bad_alloc`                                      |
| `~DynamicArray()`                                    | Освобождает память.                                                                   | Нет                                              |
| `const T& Get(int index) const`                      | Доступ по индексу (чтение).                                                           | `IndexOutOfRange` если `index` вне `[0, size-1]` |
| `int GetSize() const`                                | Возвращает текущий размер.                                                            | Нет                                              |
| `void Set(int index, const T& value)`                | Присваивает значение элементу.                                                        | `IndexOutOfRange` если `index` вне диапазона     |
| `void Resize(int newSize)`                           | Изменяет размер. При увеличении новые элементы инициализируются по умолчанию.         | `IndexOutOfRange` при `newSize < 0`              |
| `void ShrinkToFit()`                                 | Уменьшает `capacity` до минимально возможного (не менее `INITIAL_CAPACITY`).          | `bad_alloc`                                      |
| `T& operator[](int index)`                           | Доступ с проверкой границ.                                                            | `IndexOutOfRange`                                |
| `const T& operator[](int index) const`               | То же для `const`.                                                                    | `IndexOutOfRange`                                |
| `IEnumerator<T>* GetEnumerator() const`              | Возвращает итератор.                                                                  | `bad_alloc`                                      |

### Нетривиальные особенности

- **Управление памятью:** при `Resize` увеличение размера вызывает реаллокацию с удвоением `capacity`, пока не будет достигнут `newSize`. При уменьшении размера память не освобождается (только `ShrinkToFit`).
- **Исключения:** конструктор от массива с `count < 0` выбрасывает `const char*` (не `IndexOutOfRange`) – это недостаток, сохранённый из исходной лабораторной.

---

## Файл `LinkedList.hpp`

Односвязный список с головой, хвостом и длиной.

### Публичные методы

| Метод                                                     | Описание                         | Исключения                                      |
| --------------------------------------------------------- | -------------------------------- | ----------------------------------------------- |
| `LinkedList(const T* items, int count)`                   | Создаёт список из массива.       | `bad_alloc`                                     |
| `LinkedList()`                                            | Пустой список.                   | Нет                                             |
| `LinkedList(const LinkedList& other)`                     | Копирование.                     | `bad_alloc`                                     |
| `LinkedList& operator=(const LinkedList& other)`          | Присваивание.                    | `bad_alloc`                                     |
| `~LinkedList()`                                           | Очищает список.                  | Нет                                             |
| `const T& GetFirst() const`                               | Первый элемент.                  | `IndexOutOfRange` если пуст                     |
| `const T& GetLast() const`                                | Последний элемент.               | `IndexOutOfRange` если пуст                     |
| `const T& Get(int index) const`                           | Элемент по индексу.              | `IndexOutOfRange`                               |
| `T& GetRef(int index)`                                    | Не-const доступ (для изменения). | `IndexOutOfRange`                               |
| `LinkedList<T>* GetSubList(int start, int end) const`     | Возвращает новый список-срез.    | `IndexOutOfRange` если границы неверны          |
| `int GetLength() const`                                   | Длина списка.                    | Нет                                             |
| `void Append(const T& item)`                              | Добавляет в конец.               | `bad_alloc`                                     |
| `void Prepend(const T& item)`                             | Добавляет в начало.              | `bad_alloc`                                     |
| `void InsertAt(const T& item, int index)`                 | Вставка по индексу.              | `IndexOutOfRange` при `index` вне `[0, length]` |
| `void RemoveFirst()`                                      | Удаляет первый элемент.          | `IndexOutOfRange` если пуст                     |
| `void RemoveLast()`                                       | Удаляет последний.               | `IndexOutOfRange` если пуст                     |
| `void RemoveAt(int index)`                                | Удаляет элемент по индексу.      | `IndexOutOfRange`                               |
| `LinkedList<T>* Concat(const LinkedList<T>* other) const` | Конкатенация (новый список).     | `bad_alloc`                                     |
| `IEnumerator<T>* GetEnumerator() const`                   | Итератор.                        | `bad_alloc`                                     |

### Детальная спецификация нетривиальных методов

```cpp
LinkedList<T>* GetSubList(int startIndex, int endIndex) const;
```

**Предусловие:** `0 <= startIndex <= endIndex < length`.

**Постусловие:** Возвращает указатель на новый `LinkedList`, содержащий элементы с `startIndex` по `endIndex` включительно. Вызывающий отвечает за удаление.

**Исключения:** `IndexOutOfRange` при нарушении предусловия.

---

## Файл `Sequence.hpp`

Абстрактный класс, определяющий контракт последовательности. Наследует `IEnumerable<T>`.

### Чисто виртуальные методы (должны быть реализованы в потомках)

| Метод                                                   | Описание                           |
| ------------------------------------------------------- | ---------------------------------- |
| `const T& GetFirst() const`                             | Первый элемент.                    |
| `const T& GetLast() const`                              | Последний элемент.                 |
| `const T& Get(int index) const`                         | Элемент по индексу.                |
| `int GetLength() const`                                 | Размер.                            |
| `Sequence<T>* GetSubsequence(int start, int end) const` | Срез.                              |
| `Sequence<T>* Append(const T& item)`                    | Добавление в конец.                |
| `Sequence<T>* Prepend(const T& item)`                   | Добавление в начало.               |
| `Sequence<T>* InsertAt(const T& item, int index)`       | Вставка.                           |
| `Sequence<T>* RemoveFirst()`                            | Удаление первого.                  |
| `Sequence<T>* RemoveLast()`                             | Удаление последнего.               |
| `Sequence<T>* RemoveAt(int index)`                      | Удаление по индексу.               |
| `Sequence<T>* Clone() const`                            | Глубокое копирование.              |
| `Sequence<T>* CreateEmpty() const`                      | Фабрика пустой последовательности. |

### Реализованные методы (не виртуальные)

| Метод                                             | Описание                                                              | Исключения              |
| ------------------------------------------------- | --------------------------------------------------------------------- | ----------------------- |
| `Sequence<T>* Concat(const Sequence<T>* other)`   | Конкатенирует `*this` и `other`, возвращает новую последовательность. | `bad_alloc`             |
| `T2 Reduce(T2 (*f)(T2, T), T2 initial) const`     | Агрегация (fold left).                                                | любое исключение из `f` |
| `Sequence<T>* operator+(Sequence<T>* other)`      | Вызывает `Concat(other)`.                                             | –                       |
| `bool operator==(const Sequence<T>& other) const` | Позлементное сравнение.                                               | –                       |
| `bool IsEmpty() const`                            | Проверка на пустоту.                                                  | –                       |

**Важное замечание для immutable последовательностей:** методы `Append`, `Prepend`, `InsertAt`, `Remove...` возвращают новую последовательность, а исходная не изменяется. В mutable версиях они изменяют текущий объект и возвращают указатель на него же (для удобства цепочек вызовов).

---

## Файл `ArraySequence.hpp`

Реализует последовательность на основе `DynamicArray`. Содержит три класса: `ArraySequence` (абстрактная база), `MutableArraySequence`, `ImmutableArraySequence`.

### Класс `MutableArraySequence<T>`

#### Конструкторы

| Конструктор                                                  | Описание                   |
| ------------------------------------------------------------ | -------------------------- |
| `MutableArraySequence()`                                     | Пустая последовательность. |
| `MutableArraySequence(const T* items, int count)`            | Из массива.                |
| `MutableArraySequence(const MutableArraySequence<T>& other)` | Копирование.               |

#### Методы (помимо унаследованных от `Sequence`)

| Метод                                  | Описание                                         | Исключения        |
| -------------------------------------- | ------------------------------------------------ | ----------------- |
| `T& operator[](int index)`             | Прямой доступ к элементу (с проверкой).          | `IndexOutOfRange` |
| `const T& operator[](int index) const` | То же для `const`.                               | `IndexOutOfRange` |
| `Sequence<T>* Clone() const`           | Возвращает `new MutableArraySequence<T>(*this)`. | `bad_alloc`       |
| `Sequence<T>* CreateEmpty() const`     | Возвращает `new MutableArraySequence<T>()`.      | `bad_alloc`       |

### Класс `ImmutableArraySequence<T>`

В отличие от mutable, его `Instance()` возвращает `Clone()`, а все модифицирующие методы возвращают новый объект.

#### Пример поведения

```cpp
ImmutableArraySequence<int> seq(arr, 3);
auto* seq2 = seq.Append(4);  // seq не изменяется, seq2 – новый объект
delete seq2;
```

#### Особенности

- Методы `Append`, `Prepend`, `InsertAt`, `Remove...` создают новый экземпляр `ImmutableArraySequence`.
- Требуют от вызывающего управления временем жизни возвращаемых указателей.
- `Clone()` и `CreateEmpty()` также возвращают новые объекты.

### Детальная спецификация `GetSubsequence`

```cpp
Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override;
```

**Предусловие:** `0 <= startIndex <= endIndex < GetLength()`.

**Постусловие:** Возвращает новую последовательность того же типа (mutable или immutable), содержащую элементы с `startIndex` по `endIndex`.

**Исключения:** `IndexOutOfRange` при нарушении предусловия.

---

## Файл `ListSequence.hpp`

Аналогично `ArraySequence`, но на базе `LinkedList`. Классы: `ListSequence` (база), `MutableListSequence`, `ImmutableListSequence`.

### Методы аналогичны `ArraySequence`, за исключением поведения

- `Get(index)` – линейный доступ.
- `InsertAt(index)`, `RemoveAt(index)` – линейные.
- `RemoveLast()` – линейный (из-за односвязности).
- `Prepend()` – константный (в отличие от массивовой версии).

### Дополнительные возможности

`MutableListSequence` предоставляет `operator[]` (возвращает ссылку).

---

## Файлы адаптеров: `StackListSequence.hpp`, `QueueListSequence.hpp`, `DequeListSequence.hpp`

Все адаптеры используют `MutableListSequence<T>` как внутреннее хранилище.

### Класс `Stack<T>`

| Метод                      | Описание                                 | Исключения                       |
| -------------------------- | ---------------------------------------- | -------------------------------- |
| `void push(const T& item)` | Добавляет элемент на вершину стека.      | `bad_alloc`                      |
| `T pop()`                  | Удаляет и возвращает верхний элемент.    | `IndexOutOfRange` если стек пуст |
| `const T& top() const`     | Возвращает верхний элемент без удаления. | `IndexOutOfRange` если пуст      |
| `bool isEmpty() const`     | Проверка на пустоту.                     | Нет                              |
| `int size() const`         | Количество элементов.                    | Нет                              |

### Класс `Queue<T>`

| Метод                                      | Описание                                | Исключения                   |
| ------------------------------------------ | --------------------------------------- | ---------------------------- |
| `void enqueue(const T& item)`              | Добавляет в конец очереди.              | `bad_alloc`                  |
| `T dequeue()`                              | Удаляет из начала и возвращает элемент. | `IndexOutOfRange` если пуста |
| `const T& front() const`                   | Возвращает первый элемент.              | `IndexOutOfRange` если пуста |
| `bool isEmpty() const`, `int size() const` | –                                       | Нет                          |

### Класс `Deque<T>` (double-ended queue)

| Метод                                             | Описание                                | Исключения                  |
| ------------------------------------------------- | --------------------------------------- | --------------------------- |
| `void pushFront(const T& item)`                   | Добавляет в начало.                     | `bad_alloc`                 |
| `void pushBack(const T& item)`                    | Добавляет в конец.                      | `bad_alloc`                 |
| `T popFront()`                                    | Удаляет и возвращает первый элемент.    | `IndexOutOfRange` если пуст |
| `T popBack()`                                     | Удаляет и возвращает последний элемент. | `IndexOutOfRange` если пуст |
| `const T& front() const`, `const T& back() const` | Доступ без удаления.                    | `IndexOutOfRange` если пуст |
| `bool isEmpty() const`, `int size() const`        | –                                       | Нет                         |

**Примечание:** `popBack` в текущей реализации `LinkedList` имеет сложность `O(n)` из-за односвязности. Это следует учитывать при использовании в больших деках.

---

## Файл `VectorArraySequence.hpp`

Класс `Vector<T>` на основе `ImmutableArraySequence<T>`. Поддерживает математические операции над векторами.

### Конструкторы

| Конструктор                         | Описание                                                                |
| ----------------------------------- | ----------------------------------------------------------------------- |
| `Vector()`                          | Пустой вектор (длина 0).                                                |
| `Vector(int count)`                 | Вектор из `count` элементов, инициализированных значением по умолчанию. |
| `Vector(const T* items, int count)` | Из массива.                                                             |
| `Vector(const Vector<T>& other)`    | Копирование.                                                            |

### Основные методы

| Метод                                               | Описание                  | Исключения                            |
| --------------------------------------------------- | ------------------------- | ------------------------------------- |
| `T& operator[](int index)`                          | Доступ для записи.        | `IndexOutOfRange`                     |
| `const T& operator[](int index) const`              | Доступ для чтения.        | `IndexOutOfRange`                     |
| `int GetLength() const`                             | Размерность вектора.      | Нет                                   |
| `Vector<T> operator+(const Vector<T>& other) const` | Покомпонентное сложение.  | `IndexOutOfRange` если длины не равны |
| `Vector<T> operator-(const Vector<T>& other) const` | Покомпонентное вычитание. | `IndexOutOfRange` если длины не равны |

### Операторы сравнения

Все операторы (`==`, `!=`, `<`, `>`, `<=`, `>=`) выполняют лексикографическое сравнение:

- Сначала сравниваются первые различные элементы.
- Если все элементы равны, то более короткий вектор считается меньшим.

| Оператор                                        | Описание                     |
| ----------------------------------------------- | ---------------------------- |
| `bool operator==(const Vector<T>& other) const` | Равенство поэлементное.      |
| `bool operator!=(const Vector<T>& other) const` | `!(*this == other)`.         |
| `bool operator<(const Vector<T>& other) const`  | Лексикографическое «меньше». |
| `bool operator>(const Vector<T>& other) const`  | `other < *this`.             |
| `bool operator<=(const Vector<T>& other) const` | `!(other < *this)`.          |
| `bool operator>=(const Vector<T>& other) const` | `!(*this < other)`.          |

### Итераторы

`Vector` предоставляет `begin()` и `end()` (пробрасывает итераторы внутренней `ImmutableArraySequence`). Позволяет использовать range-based for.

### Пример использования

```cpp
Vector<int> v1({1,2,3}, 3);
Vector<int> v2({4,5,6}, 3);
Vector<int> v3 = v1 + v2;  // {5,7,9}
if (v1 < v3) { /* ... */ }
for (int x : v1) std::cout << x << ' ';
```

---

## Файлы тестов (`test.cpp`, `newtest.cpp`)

Содержат тесты Google Test.

Покрытие:

- `test.cpp`: `DynamicArray`, `LinkedList`, `MutableArraySequence`, `MutableListSequence`, `ImmutableArraySequence`, `ImmutableListSequence`.
- `newtest.cpp`: `Stack`, `Queue`, `Deque`, `Vector`.

Запуск через `make test` и `make newtest` (см. `Makefile`).

---

## Makefile

| Цель       | Описание                                                 |
| ---------- | -------------------------------------------------------- |
| `tests`    | Сборка исполняемого файла `tests` (тесты из `test.cpp`). |
| `newtests` | Сборка `newtests` (тесты из `newtest.cpp`).              |
| `program`  | Сборка основной программы `program` (из `main.cpp`).     |
| `clean`    | Удаление всех объектных файлов и исполняемых.            |
| `run`      | Запуск `program`.                                        |
| `test`     | Запуск `tests`.                                          |
| `newtest`  | Запуск `newtests`.                                       |

**Примечание:** используются флаги `-std=c++17 -stdlib=libc++` (для Clang с libc++). При необходимости адаптируйте под свой компилятор.

---

## Инструкция по редактированию README

Вы можете редактировать этот файл в любом текстовом редакторе, поддерживающем Markdown.

### Локальное редактирование

- VS Code, Sublime Text, Notepad++ – есть подсветка синтаксиса Markdown.
- Сохраняйте с расширением `.md` (например, `README.md`).

### Онлайн-редакторы

- GitHub / GitLab – нажмите на файл, затем иконку карандаша.
- StackEdit, Dillinger, HackMD – удобно для предпросмотра.

### Что можно изменять

- Заголовки, списки, таблицы, примеры кода.
- Добавлять новые разделы или пояснения.
- Исправлять описание функций.

### Предпросмотр

- В VS Code: `Ctrl+Shift+V`.
- На GitHub: вкладка `Preview`.
- В других редакторах – часто есть режим предпросмотра.

### Сохранение изменений

- Если файл в репозитории – сделайте commit и push.
- Если локальный – просто сохраните.

---

## Заключение

Данная документация покрывает все публичные интерфейсы предоставленного кода.

Для углублённого изучения рекомендуется обращаться к исходным файлам и тестам.

Если вы хотите дополнить README примерами использования, просто добавьте блоки кода с тройными бэктиками и указанием языка `cpp`.
