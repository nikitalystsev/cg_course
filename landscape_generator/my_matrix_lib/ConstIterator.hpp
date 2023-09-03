#ifndef __CONSTITERATOR_HPP__
#define __CONSTITERATOR_HPP__

#include <iostream>
#include <memory>

#include "Concepts.hpp"
#include "Iterator.hpp"
#include "MatrixExceptions.hpp"

using string = std::string;

template <MatrixType T> // объявление класса Matrix
class Matrix;

template <MatrixType T>
class ConstIterator
{
public:
    // определили алиасы типов
    using iterator_type = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = std::remove_const_t<T>;
    using pointer = T *;
    using reference = T &;
    using iterator = ConstIterator<T>;

    ConstIterator(Matrix<T> &matrix, const size_t index = 0);       // конструктор итератора
    ConstIterator(const Matrix<T> &matrix, const size_t index = 0); // конструктор итератора
    ConstIterator(const ConstIterator &it) = default;               // конструктор копирования
    ConstIterator(ConstIterator &&it) noexcept = default;           // конструктор перемещения
    ConstIterator(const Iterator<T> &other) noexcept;
    ConstIterator(Iterator<T> &&other) noexcept;

    ~ConstIterator() noexcept = default; // деструктор дефолтный

    // оператор присваивания
    iterator &operator=(const iterator &it);
    iterator &operator=(iterator &&it) noexcept;

    // сравнение
    bool operator!=(iterator const &other) const;
    bool operator==(iterator const &other) const;
    bool operator<(iterator const &other) const;
    bool operator<=(iterator const &other) const;
    bool operator>(iterator const &other) const;
    bool operator>=(iterator const &other) const;

    // перемещение
    iterator &operator++();
    iterator operator++(int);
    iterator &next() const;
    iterator &operator--();
    iterator operator--(int) const;
    iterator operator+(const size_t value) const;
    iterator operator-(const size_t value) const;
    iterator &operator+=(const size_t value);
    iterator &operator-=(const size_t value);

    difference_type operator-(const iterator &it);

    // // доступ
    const T &operator*() const;
    const T &value() const;
    const T *operator->() const;
    const T &operator[](const size_t index) const;

    operator bool() const;
    bool is_end() const;
    bool is_valid_data() const;

private:
    std::weak_ptr<typename Matrix<T>::MatrixRow[]> _data_iter;
    mutable size_t _index = 0; // индекс это номер элемента в матрице ка если бы все ее элементы построчно расположились бы на одной строки
    size_t _rows = 0;
    size_t _cols = 0;

    void _check_index(const char *hint = "") const;
    void _check_data(const char *hint = "") const;
};

// /////////////////////////////////////////////////////////////////////////////////////////////////

template <MatrixType T>
// конструктор итератора
ConstIterator<T>::ConstIterator(Matrix<T> &matrix, const size_t index)
{
    _index = index;
    _data_iter = matrix._data;
    _rows = matrix._rows;
    _cols = matrix._cols;
}

template <MatrixType T>
// конструктор итератора
ConstIterator<T>::ConstIterator(const Matrix<T> &matrix, const size_t index)
{
    _index = index;
    _data_iter = matrix._data;
    _rows = matrix._rows;
    _cols = matrix._cols;
}

template <MatrixType T>
ConstIterator<T>::ConstIterator(const Iterator<T> &other) noexcept
{
    _index = other._index;
    _data_iter = other._data_iter;
    _rows = other._rows;
    _cols = other._cols;
}

template <MatrixType T>
ConstIterator<T>::ConstIterator(Iterator<T> &&other) noexcept
{
    _index = other._index;
    _data_iter = other._data_iter;
    _rows = other._rows;
    _cols = other._cols;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

template <MatrixType T>
// переопределили оператор присваивания =
ConstIterator<T>::iterator &ConstIterator<T>::operator=(const ConstIterator<T>::iterator &it)
{
    this->_data_iter = it._data;
    this->_index = it._index;
    this->_rows = it._rows;
    this->_cols = it._cols;

    return *this;
}

template <MatrixType T>
// переопределили оператор присваивания =
ConstIterator<T>::iterator &ConstIterator<T>::operator=(ConstIterator<T>::iterator &&it) noexcept
{
    this->_data_iter = it._data;
    this->_index = it._index;
    this->_rows = it._rows;
    this->_cols = it._cols;

    return *this;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////

template <MatrixType T>
// переопределили !=
bool ConstIterator<T>::operator!=(ConstIterator<T>::iterator const &other) const
{
    return this->_index != other._index;
}

template <MatrixType T>
// переопределили ==
bool ConstIterator<T>::operator==(ConstIterator<T>::iterator const &other) const
{
    return this->_index == other._index;
}

template <MatrixType T>
// переопределили <
bool ConstIterator<T>::operator<(ConstIterator<T>::iterator const &other) const
{
    return this->_index < other._index;
}

template <MatrixType T>
// переопределили <=
bool ConstIterator<T>::operator<=(ConstIterator<T>::iterator const &other) const
{
    return this->_index <= other._index;
}

template <MatrixType T>
// переопределили >
bool ConstIterator<T>::operator>(ConstIterator<T>::iterator const &other) const
{
    return this->_index > other._index;
}

template <MatrixType T>
// переопределили >=
bool ConstIterator<T>::operator>=(ConstIterator<T>::iterator const &other) const
{
    return this->_index >= other._index;
}

// ///////////////////////////////////////////////////////////////////////////////////////////////////

template <MatrixType T>
// переопределили префиксный инкремент
ConstIterator<T>::iterator &ConstIterator<T>::operator++()
{
    if (this->_index < this->_cols * this->_rows)
        ++_index;

    return *this;
}

template <MatrixType T>
// переопределили постфиксный инкремент
ConstIterator<T>::iterator ConstIterator<T>::operator++(int)
{
    iterator it(*this);

    ++(*this);

    return it;
}

template <MatrixType T>
// next
ConstIterator<T>::iterator &ConstIterator<T>::next() const
{
    return operator++();
}

template <MatrixType T>
ConstIterator<T>::iterator &ConstIterator<T>::operator--()
{
    if (_index > 0)
        --_index;

    return *this;
}

template <MatrixType T>
ConstIterator<T>::iterator ConstIterator<T>::operator--(int) const
{
    iterator it(*this);

    --(*this);

    return it;
}

template <MatrixType T>
// переопределили оператор +
ConstIterator<T>::iterator ConstIterator<T>::operator+(const size_t value) const
{
    iterator it(*this);

    if (value < 0 && it._index < -value)
        it._index = 0;
    else
        it._index += value;

    if (it._index < 0)
        it._index = 0;
    else if (it._index > this->_rows * this->_cols)
        it._index = this->_rows * this->_cols;

    return it;
}

template <MatrixType T>
// переопределили оператор -
ConstIterator<T>::iterator ConstIterator<T>::operator-(const size_t value) const
{
    return operator+(-value);
}

template <MatrixType T>
// переопределили оператор +=
ConstIterator<T>::iterator &ConstIterator<T>::operator+=(const size_t value)
{
    if (value < 0 && this->_index < -value)
        this->_index = 0;
    else
        this->_index += value;

    if (this->_index < 0)
        this->_index = 0;
    else if (this->_index > this->_rows * this->_cols)
        this->_index = this->_rows * this->_cols;

    return *this;
}

template <MatrixType T>
// переопределили оператор -=
ConstIterator<T>::iterator &ConstIterator<T>::operator-=(const size_t value)
{
    return operator+=(-value);
}

template <MatrixType T>
ConstIterator<T>::difference_type ConstIterator<T>::operator-(const iterator &it)
{
    return std::max(this->_index, it._index) - std::min(this->_index, it._index);
}

// ///////////////////////////////////////////////////////////////////////////////////////////////////

template <MatrixType T>
// переопределили оператор разыменования *
const T &ConstIterator<T>::operator*() const
{
    this->_check_data("Const Итератор указывает на nullptr\n");
    // this->_check_index("Const Итератор не находится в границах данных при выполнении const оператора *");

    std::shared_ptr<typename Matrix<T>::MatrixRow[]> data_ptr = this->_data_iter.lock();

    return data_ptr[this->_index / this->_cols][this->_index % this->_cols];
}

template <MatrixType T>
const T &ConstIterator<T>::value() const
{
    return operator*();
}

// ///////////////////////////////////////////////////////////////////////////////////////////////////

template <MatrixType T>
// переопределили ->
const T *ConstIterator<T>::operator->() const
{
    this->_check_data("Const Итератор указывает на nullptr\n");
    this->_check_index("Const Итератор не находится в границах данных при выполнении const оператора ->");

    std::shared_ptr<typename Matrix<T>::MatrixRow[]> data_ptr = this->_data_iter.lock();

    return data_ptr[_index / _cols].get_address() + (_index % _cols);
}

template <MatrixType T>
// переопределили []
const T &ConstIterator<T>::operator[](const size_t index) const
{
    this->_check_data("Const Итератор указывает на nullptr\n");

    size_t tmp_index = this->_index;
    this->_index = index;

    this->_check_index("Const Итератор не находится в границах данных при выполнении const оператора []");

    this->_index = tmp_index;

    std::shared_ptr<typename Matrix<T>::MatrixRow[]> data_ptr = this->_data_iter.lock();

    return data_ptr[index / this->_cols][index % this->_cols];
}

// ///////////////////////////////////////////////////////////////////////////////////////////////////

template <MatrixType T>
ConstIterator<T>::operator bool() const
{
    return this->_data_iter.expired();
}

template <MatrixType T>
bool ConstIterator<T>::is_end() const
{
    return this->_index == this->_rows * this->_cols;
}

template <MatrixType T>
bool ConstIterator<T>::is_valid_data() const
{
    return !this->_data_iter.expired();
}

// ///////////////////////////////////////////////////////////////////////////////////////////////////

template <MatrixType T>
// метод для проверки индекса в итераторе
void ConstIterator<T>::_check_index(const char *hint) const
{
    if (this->_index < this->_rows * this->_cols)
        return;

    throw IteratorIndexError(__FILE__, typeid(*this).name(), __LINE__, hint);
}

template <MatrixType T>
// метод проверяет данные на валидность
void ConstIterator<T>::_check_data(const char *hint) const
{
    if (is_valid_data())
        return;

    throw IteratorValidationError(__FILE__, typeid(*this).name(), __LINE__, hint);
}

#endif // __CONSTITERATOR_HPP__