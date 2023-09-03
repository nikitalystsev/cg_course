#ifndef __MATRIX_HPP__
#define __MATRIX_HPP__

#include <iostream>
#include <memory>
#include <cmath>

#include "Concepts.hpp"
#include "MatrixBase.hpp" // класс матрицы будет наследоваться от базового класса
#include "Iterator.hpp"
#include "ConstIterator.hpp"
#include "MatrixExceptions.hpp"

// класс будет шаблонным, то есть тип элемента матрицы будет определяться при создании
template <MatrixType T>
class Matrix : public MatrixBase // наследуется от базового класса
{
public:
    class MatrixRow; // обьявляем класс MatrixRow
    friend Iterator<T>;
    friend ConstIterator<T>;

public:
    // определили алиасы типов
    using value_type = T;
    using size_type = size_t;
    using iterator = Iterator<T>;
    using difference_type = typename iterator::difference_type;
    using const_iterator = ConstIterator<T>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using reference = typename iterator::reference;
    using const_reference = typename const_iterator::reference;
    using pointer = typename iterator::pointer;
    using const_pointer = typename const_iterator::pointer;

    // различные конструкторы класса Matrix
    // Matrix() = default;
    explicit Matrix(const size_t rows = 0, const size_t cols = 0);
    explicit Matrix(const Matrix<T> &matrix); // конструктор копирования
    Matrix(Matrix<T> &&matrix) noexcept;      // конструктор перемещения

    Matrix(const size_t rows, const size_t cols, const T &filler);     // конструктор для заполнения матрицы filler-ом
    Matrix(const size_t rows, const size_t cols, T **matrix);          // создание матрицы на основе си матрицы
    Matrix(std::initializer_list<std::initializer_list<T>> init_list); // конструктор по списку инициализации

    template <MatrixType T2>
    explicit Matrix(const Matrix<T2> &matrix)
        requires PermittedType<T, T2>;

    template <typename Container>
        requires PermittedContainer<T, Container>
    explicit Matrix(const Container &matrix);

    template <std::input_iterator Iter, std::sentinel_for<Iter> Iter_e>
        requires std::constructible_from<T, typename std::iterator_traits<Iter>::reference>
    Matrix(const Iter begin, const Iter_e end, const size_t cols);

    ~Matrix() noexcept = default; // деструктор класса по умолчанию

    MatrixRow operator[](const size_t row);             // методы, возвращающие строку матрицы
    const MatrixRow operator[](const size_t row) const; // методы, возвращающие строку матрицы

    // // методы для итерации по матрицы (итерация по строкам)
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;
    reverse_iterator rbegin();
    reverse_iterator rend();
    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;
    const_reverse_iterator rcbegin() const;
    const_reverse_iterator rcend() const;

    template <MatrixType T2>
        requires MatrixEquality<T, T2>
    bool operator==(const Matrix<T2> &matrix) const;

    template <MatrixType T2>
        requires MatrixEquality<T, T2>
    bool operator!=(const Matrix<T2> &matrix) const;

    Matrix<T> &operator=(const Matrix<T> &matrix);
    Matrix<T> &operator=(Matrix<T> &&matrix) noexcept;
    Matrix<T> &operator=(std::initializer_list<std::initializer_list<T>> init_list);

    // математические операции с матрицами
    template <MatrixType T2>
        requires MatrixSum<T, T2>
    decltype(auto) operator+(const Matrix<T2> &matrix) const;
    template <MatrixType T2>
        requires MatrixSum<T, T2>
    decltype(auto) operator+(const T2 &elem) const;
    template <MatrixType T2>
        requires MatrixSum<T, T2>
    Matrix<T> &operator+=(const Matrix<T2> &matrix);
    template <MatrixType T2>
        requires MatrixSum<T, T2>
    Matrix<T> &operator+=(const T2 &elem);

    template <MatrixType T2>
        requires MatrixSum<T, T2>
    decltype(auto) add_matrix(const Matrix<T2> &matrix) const;
    template <MatrixType T2>
        requires MatrixSum<T, T2>
    decltype(auto) add_elem(const T2 &elem) const;
    template <MatrixType T2>
        requires MatrixSum<T, T2>
    Matrix<T> &add_eq_matrix(const Matrix<T2> &matrix);
    template <MatrixType T2>
        requires MatrixSum<T, T2>
    Matrix<T> &add_eq_elem(const T2 &elem);

    template <MatrixType T2>
        requires MatrixSub<T, T2>
    decltype(auto) operator-(const Matrix<T2> &matrix) const;
    template <MatrixType T2>
        requires MatrixSub<T, T2>
    decltype(auto) operator-(const T2 &elem) const;
    template <MatrixType T2>
        requires MatrixSub<T, T2>
    Matrix<T> &operator-=(const Matrix<T2> &matrix);
    template <MatrixType T2>
        requires MatrixSub<T, T2>
    Matrix<T> &operator-=(const T2 &elem);

    template <MatrixType T2>
        requires MatrixSub<T, T2>
    decltype(auto) sub_matrix(const Matrix<T2> &matrix) const;
    template <MatrixType T2>
        requires MatrixSub<T, T2>
    decltype(auto) sub_elem(const T2 &elem) const;
    template <MatrixType T2>
        requires MatrixSub<T, T2>
    Matrix<T> &sub_eq_matrix(const Matrix<T2> &matrix);
    template <MatrixType T2>
        requires MatrixSub<T, T2>
    Matrix<T> &sub_eq_elem(const T2 &elem);

    template <MatrixType T2>
        requires MatrixMul<T, T2>
    decltype(auto) operator*(const Matrix<T2> &matrix) const;
    template <MatrixType T2>
        requires MatrixMul<T, T2>
    decltype(auto) operator*(const T2 &elem) const;
    template <MatrixType T2>
        requires MatrixMul<T, T2>
    Matrix<T> &operator*=(const Matrix<T2> &matrix);
    template <MatrixType T2>
        requires MatrixMul<T, T2>
    Matrix<T> &operator*=(const T2 &elem);

    template <MatrixType T2>
        requires MatrixMul<T, T2>
    decltype(auto) mul_matrix(const Matrix<T2> &matrix) const;
    template <MatrixType T2>
        requires MatrixMul<T, T2>
    decltype(auto) mul_elem(const T2 &elem) const;
    template <MatrixType T2>
        requires MatrixMul<T, T2>
    Matrix<T> &mul_eq_matrix(const Matrix<T2> &matrix);
    template <MatrixType T2>
        requires MatrixMul<T, T2>
    Matrix<T> &mul_eq_elem(const T2 &elem);

    template <MatrixType T2>
        requires MatrixDiv<T, T2>
    decltype(auto) operator/(const Matrix<T2> &matrix) const;
    template <MatrixType T2>
        requires MatrixDiv<T, T2>
    decltype(auto) operator/(const T2 &elem) const;
    template <MatrixType T2>
        requires MatrixDiv<T, T2>
    Matrix<T> &operator/=(const Matrix<T2> &matrix);
    template <MatrixType T2>
        requires MatrixDiv<T, T2>
    Matrix<T> &operator/=(const T2 &elem);

    template <MatrixType T2>
        requires MatrixDiv<T, T2>
    decltype(auto) div_matrix(const Matrix<T2> &matrix) const;
    template <MatrixType T2>
        requires MatrixDiv<T, T2>
    decltype(auto) div_elem(const T2 &elem) const;
    template <MatrixType T2>
        requires MatrixDiv<T, T2>
    Matrix<T> &div_eq_matrix(const Matrix<T2> &matrix);
    template <MatrixType T2>
        requires MatrixDiv<T, T2>
    Matrix<T> &div_eq_elem(const T2 &elem);

    template <MatrixType T2>
        requires MatrixSum<T, T2>
    friend Matrix<T> operator+(const T2 &elem, const Matrix<T> &matrix)
    {
        return matrix + elem;
    }

    template <MatrixType T2>
        requires MatrixSub<T, T2>
    friend Matrix<T> operator-(const T2 &elem, const Matrix<T> &matrix)
    {
        return matrix - elem;
    }

    template <MatrixType T2>
        requires MatrixMul<T, T2>
    friend Matrix<T> operator*(const T2 &elem, const Matrix<T> &matrix)
    {
        return matrix * elem;
    }

    template <MatrixType T2>
        requires MatrixDiv<T, T2>
    friend Matrix<T> operator/(const T2 &elem, const Matrix<T> &matrix)
    {
        Matrix tmp(matrix);

        tmp.inverse();

        return tmp * elem;
    }

    Matrix<T> operator-()
        requires NumericMatrix<T>;
    Matrix<T> neg()
        requires NumericMatrix<T>;

    // более сложные математические операции
    value_type determinant() const
        requires MatrixFloatPoint<T>;
    Matrix<T> transpose()
        requires MatrixFloatPoint<T>;
    Matrix<T> identity();
    Matrix<T> inverse()
        requires MatrixFloatPoint<T>;
    bool is_square() const; // квадратная ли матрица
    size_t size() const;

    void fill(Iterator<T> start, const Iterator<T> &end, const T &value); // заполнение матрицы значениями

    T &get_elem(size_t row, size_t col);
    const T &get_elem(size_t row, size_t col) const;

private:
    // // атрибуты _rows и _cols не объявляю, поскольку они есть в базовом классе
    std::shared_ptr<typename Matrix<T>::MatrixRow[]> _data;                                               // собственно сами данные (массив указателей на строки)
    std::shared_ptr<typename Matrix<T>::MatrixRow[]> _matrix_alloc(const size_t rows, const size_t cols); // метод выделяет память под матрицы

    void _check_index(size_t pos, size_t limit) const;
    template <MatrixType T2>
    void _check_sizes(const Matrix<T2> &matrix) const;
    template <MatrixType T2>
    void _check_mult_sizes(const Matrix<T2> &matrix) const;

    void _move_row(size_t from, size_t to);
    void _move_col(size_t from, size_t to);

public: // для описания строки матрицы как подкласса самой матрицы
    class MatrixRow
    {
    public:
        explicit MatrixRow();                                         // конструктор без параметров
        explicit MatrixRow(const T *data_row, const size_t size_row); // конструктор создания строки матрицы

        ~MatrixRow() noexcept = default; // деструктор класса строки по умолчанию

        T &operator[](const size_t index);             // получение элемента строки через индексацию
        const T &operator[](const size_t index) const; // получение элемента строки через индексацию (константное)

        void reset(T *data_row, const size_t size_row); // переустановка новых данных
        void reset();                                   // сброс данных
    private:
        std::shared_ptr<T[]> _data_row{nullptr}; // элементы строки
        size_t _size_row = 0;                    // количество элементов в строке

        T *get_address() { return _data_row.get(); }
        const T *get_address() const { return _data_row.get(); }
    };
};

//////////////////////////////////////////////////////////////////////////////////////////////////////

// все конструкторы будут шаблонными, чтобы элементом матрицы мог быть любой тип данных
template <MatrixType T>
// первый конструктор с параметрами. Вызывается конструктор базового класса
Matrix<T>::Matrix(const size_t rows, const size_t cols) : MatrixBase(rows, cols)
{
    this->_data = _matrix_alloc(rows, cols); // выделяем память под матрицу
}

template <MatrixType T>
// конструктор копирования. Вызывается конструктор бзового класса
Matrix<T>::Matrix(const Matrix<T> &matrix) : MatrixBase(matrix._rows, matrix._cols)
{
    this->_data = _matrix_alloc(matrix._rows, matrix._cols); // выделяем память под матрицу

    for (size_t i = 0; i < _rows; ++i)
        for (size_t j = 0; j < _cols; ++j)
            this->_data[i][j] = matrix[i][j];
}

template <MatrixType T>
// конструктор перемещения. Вызывается конструктор бзового класса
Matrix<T>::Matrix(Matrix<T> &&matrix) noexcept : MatrixBase(matrix._rows, matrix._cols)
{
    this->_data = matrix._data;
}

template <MatrixType T>
// конструктор заполнения матрицы переданным значением. Вызывается конструктор базового класса
Matrix<T>::Matrix(const size_t rows, const size_t cols, const T &filler) : MatrixBase(rows, cols)
{
    this->_data = _matrix_alloc(rows, cols); // выделяем память под матрицу

    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            this->_data[i][j] = filler;
}

template <MatrixType T>
void _check_ptr(T ptr)
{
    if (ptr)
        return;

    throw InvalidArgument(__FILE__, "Non-class", __LINE__, "nullptr как указатель c-матрицы");
}

template <MatrixType T>
// конструктор создания матрицы на основе си матрицы. Вызывается конструктор базового класса
Matrix<T>::Matrix(const size_t rows, const size_t cols, T **matrix) : MatrixBase(rows, cols)
{
    _check_ptr(matrix); // проверка указателя

    this->_data = _matrix_alloc(rows, cols); // выделяем память под матрицу

    for (size_t i = 0; i < rows; ++i)
    {
        // позже добавить проверку указателя
        for (size_t j = 0; j < cols; ++j)
            this->_data[i][j] = matrix[i][j];
    }
}

template <MatrixType T>
// конструктор создания матрицы на основе списка инициализации.
// Конструктор базового класса не вызывается
Matrix<T>::Matrix(std::initializer_list<std::initializer_list<T>> init_list)
{
    size_t rows = init_list.size();          // количество строк
    size_t cols = init_list.begin()->size(); // количество столбцов

    for (const auto &ilist : init_list)
        if (ilist.size() != cols)
        {
            throw InvalidArgument(__FILE__, typeid(*this).name(),
                                  __LINE__, "Некорректный список инициализации");
        }

    this->_data = _matrix_alloc(rows, cols); // выделяем память под матрицу

    this->_rows = rows;
    this->_cols = cols;

    size_t i = 0;

    for (const auto &i_list : init_list)
        for (const auto &elem : i_list)
        {
            this->_data[i / cols][i % cols] = elem;
            ++i;
        }
}

template <MatrixType T>
template <MatrixType T2>
Matrix<T>::Matrix(const Matrix<T2> &matrix)
    requires PermittedType<T, T2>
    : MatrixBase(matrix.get_rows(), matrix.get_cols())
{
    this->_data = _matrix_alloc(matrix.get_rows(), matrix.get_cols()); // выделяем память под матрицу

    for (size_t i = 0; i < _rows; ++i)
        for (size_t j = 0; j < _cols; ++j)
            this->_data[i][j] = matrix[i][j];
}

template <MatrixType T>
template <typename Container>
    requires PermittedContainer<T, Container>
Matrix<T>::Matrix(const Container &container) : MatrixBase(container.get_rows(), container.get_cols())
{
    this->_matrix_alloc(container.get_rows(), container.get_cols());

    for (size_t i = 0; i < _rows; ++i)
        for (size_t j = 0; j < _cols; ++j)
            this->_data[i][j] = container[i][j];
}

template <MatrixType T>
template <std::input_iterator Iter, std::sentinel_for<Iter> Iter_e>
    requires std::constructible_from<T, typename std::iterator_traits<Iter>::reference>
Matrix<T>::Matrix(const Iter begin, const Iter_e end, const size_t cols)
{
    size_t size_rows = 0;

    for (auto iter = begin; iter != end; iter++, size_rows++)
        ;

    if (size_rows == 0 || size_rows % cols != 0)
        throw IndexError(__FILE__, typeid(*this).name(), __LINE__, "Неверный индекс строки");

    this->_cols = cols;
    this->_rows = size_rows / cols;
    this->_matrix_alloc(this->_rows, this->_cols);

    size_t i = 0;
    size_t j = 0;

    for (auto iter = begin; iter != end; iter++, j++)
    {
        this->_data[i][j] = *iter;

        if (j % this->_cols == 0)
            i++;
    }
}
// ///////////////////////////////////////////////////////////////////////////////////////////////////

template <MatrixType T>
Matrix<T>::MatrixRow Matrix<T>::operator[](const size_t row)
{
    if (row >= get_rows())
        throw IndexError(__FILE__, typeid(*this).name(), __LINE__, "Неверный индекс строки");

    return _data[row];
}

template <MatrixType T>
const Matrix<T>::MatrixRow Matrix<T>::operator[](const size_t row) const
{
    if (row >= get_rows())
        throw IndexError(__FILE__, typeid(*this).name(), __LINE__, "Неверный индекс строки");

    return _data[row];
}

// ///////////////////////////////////////////////////////////////////////////////////////////////////

template <MatrixType T>
std::shared_ptr<typename Matrix<T>::MatrixRow[]> Matrix<T>::_matrix_alloc(const size_t rows, const size_t cols)
{
    std::shared_ptr<typename Matrix<T>::MatrixRow[]> data = nullptr;

    try
    {
        data.reset(new MatrixRow[rows]);

        for (size_t i = 0; i < rows; i++)
            data[i].reset(new T[cols], cols);
    }
    catch (std::bad_alloc &err)
    {
        throw MemoryError(__FILE__, typeid(*this).name(), __LINE__, "Ошибка выделения памяти в функции _matrix_alloc");
    }

    return data;
}

// ///////////////////////////////////////////////////////////////////////////////////////////////////

template <MatrixType T>
Matrix<T>::iterator Matrix<T>::begin()
{
    return Iterator<T>(*this, 0);
}

template <MatrixType T>
Matrix<T>::iterator Matrix<T>::end()
{
    return Iterator<T>(*this, this->size());
}

template <MatrixType T>
Matrix<T>::const_iterator Matrix<T>::begin() const
{
    return ConstIterator<T>(*this, 0);
}

template <MatrixType T>
Matrix<T>::const_iterator Matrix<T>::end() const
{
    return ConstIterator<T>(*this, this->size());
}

template <MatrixType T>
Matrix<T>::const_iterator Matrix<T>::cbegin() const
{
    return ConstIterator<T>(*this, 0);
}

template <MatrixType T>
Matrix<T>::const_iterator Matrix<T>::cend() const
{
    return ConstIterator<T>(*this, this->size());
}

template <MatrixType T>
Matrix<T>::reverse_iterator Matrix<T>::rend()
{
    return std::reverse_iterator(Iterator<T>(*this, 0));
}

template <MatrixType T>
Matrix<T>::reverse_iterator Matrix<T>::rbegin()
{
    return std::reverse_iterator(Iterator<T>(*this, this->size()));
}

template <MatrixType T>
Matrix<T>::const_reverse_iterator Matrix<T>::rend() const
{
    return std::reverse_iterator(Iterator<T>(*this, 0));
}

template <MatrixType T>
Matrix<T>::const_reverse_iterator Matrix<T>::rbegin() const
{
    return std::reverse_iterator(Iterator<T>(*this, this->size()));
}

template <MatrixType T>
Matrix<T>::const_reverse_iterator Matrix<T>::rcend() const
{
    return std::reverse_iterator(ConstIterator<T>(*this, 0));
}

template <MatrixType T>
Matrix<T>::const_reverse_iterator Matrix<T>::rcbegin() const
{
    return std::reverse_iterator(ConstIterator<T>(*this, this->size()));
}

// ///////////////////////////////////////////////////////////////////////////////////////////////////

template <MatrixType T>
template <MatrixType T2>
    requires MatrixEquality<T, T2>
bool Matrix<T>::operator==(const Matrix<T2> &matrix) const
{
    if ((this->_rows != matrix.get_rows()) || (this->_cols != matrix.get_cols()))
        return false;

    for (size_t i = 0; i < this->_rows; ++i)
        for (size_t j = 0; j < this->_cols; ++j)
            if (this->_data[i][j] != matrix[i][j])
                return false;

    return true;
}

template <MatrixType T>
template <MatrixType T2>
    requires MatrixEquality<T, T2>
bool Matrix<T>::operator!=(const Matrix<T2> &matrix) const
{
    return !operator==(matrix);
}

// ///////////////////////////////////////////////////////////////////////////////////////////////////

template <MatrixType T>
// операторы присваивания
Matrix<T> &Matrix<T>::operator=(const Matrix<T> &matrix)
{
    this->_data = _matrix_alloc(matrix._rows, matrix._cols); // выделяем память под матрицу

    this->_rows = matrix._rows, this->_cols = matrix._cols;

    for (size_t i = 0; i < _rows; ++i)
        for (size_t j = 0; j < _cols; ++j)
            this->_data[i][j] = matrix[i][j];

    return *this;
}

template <MatrixType T>
Matrix<T> &Matrix<T>::operator=(Matrix<T> &&matrix) noexcept
{
    this->_data = matrix._data;
    this->_rows = matrix._rows;
    this->_cols = matrix._cols;

    return *this;
}

template <MatrixType T>
Matrix<T> &Matrix<T>::operator=(std::initializer_list<std::initializer_list<T>> init_list)
{
    size_t cols = init_list.begin()->size();

    for (const auto &ilist : init_list)
        if (ilist.size() != cols)
            throw InvalidArgument(__FILE__, typeid(*this).name(), __LINE__, "Некорректный список инициализации");

    size_t i = 0;
    for (const auto &ilist : init_list)
        for (const auto &elem : ilist)
        {
            this->_data[i / cols][i % cols] = elem;
            ++i;
        }

    return *this;
}

// ///////////////////////////////////////////////////////////////////////////////////////////////////

template <MatrixType T>
template <MatrixType T2>
    requires MatrixSum<T, T2>
decltype(auto) Matrix<T>::operator+(const Matrix<T2> &matrix) const
{
    _check_sizes(matrix);

    Matrix<T> tmp(_rows, _cols);

    for (size_t i = 0; i < _rows; ++i)
        for (size_t j = 0; j < _cols; ++j)
            tmp[i][j] = _data[i][j] + matrix[i][j];

    return tmp;
}

template <MatrixType T>
template <MatrixType T2>
    requires MatrixSum<T, T2>
decltype(auto) Matrix<T>::operator+(const T2 &elem) const
{
    Matrix<T> tmp(_rows, _cols);

    for (size_t i = 0; i < _rows; ++i)
        for (size_t j = 0; j < _cols; ++j)
            tmp[i][j] = this->_data[i][j] + elem;

    return tmp;
}

template <MatrixType T>
template <MatrixType T2>
    requires MatrixSum<T, T2>
Matrix<T> &Matrix<T>::operator+=(const Matrix<T2> &matrix)
{
    _check_sizes(matrix);

    for (size_t i = 0; i < _rows; ++i)
        for (size_t j = 0; j < _cols; ++j)
            this->_data[i][j] += matrix[i][j];

    return *this;
}

template <MatrixType T>
template <MatrixType T2>
    requires MatrixSum<T, T2>
Matrix<T> &Matrix<T>::operator+=(const T2 &elem)
{
    for (size_t i = 0; i < _rows; ++i)
        for (size_t j = 0; j < _cols; ++j)
            this->_data[i][j] += elem;

    return *this;
}

template <MatrixType T>
template <MatrixType T2>
    requires MatrixSum<T, T2>
decltype(auto) Matrix<T>::add_matrix(const Matrix<T2> &matrix) const
{
    return operator+(matrix);
}

template <MatrixType T>
template <MatrixType T2>
    requires MatrixSum<T, T2>
decltype(auto) Matrix<T>::add_elem(const T2 &elem) const
{
    return operator+(elem);
}

template <MatrixType T>
template <MatrixType T2>
    requires MatrixSum<T, T2>
Matrix<T> &Matrix<T>::add_eq_matrix(const Matrix<T2> &matrix)
{
    return operator+=(matrix);
}

template <MatrixType T>
template <MatrixType T2>
    requires MatrixSum<T, T2>
Matrix<T> &Matrix<T>::add_eq_elem(const T2 &elem)
{
    return operator+=(elem);
}

// ///////////////////////////////////////////////////////////////////////////////////////////////////

template <MatrixType T>
template <MatrixType T2>
    requires MatrixSub<T, T2>
decltype(auto) Matrix<T>::operator-(const Matrix<T2> &matrix) const
{
    _check_sizes(matrix);

    Matrix<T> tmp(_rows, _cols);

    for (size_t i = 0; i < _rows; ++i)
        for (size_t j = 0; j < _cols; ++j)
            tmp[i][j] = _data[i][j] - matrix[i][j];

    return tmp;
}

template <MatrixType T>
template <MatrixType T2>
    requires MatrixSub<T, T2>
decltype(auto) Matrix<T>::operator-(const T2 &elem) const
{
    Matrix<T> tmp(_rows, _cols);

    for (size_t i = 0; i < _rows; ++i)
        for (size_t j = 0; j < _cols; ++j)
            tmp[i][j] = _data[i][j] - elem;

    return tmp;
}

template <MatrixType T>
template <MatrixType T2>
    requires MatrixSub<T, T2>
Matrix<T> &Matrix<T>::operator-=(const Matrix<T2> &matrix)
{
    _check_sizes(matrix);

    for (size_t i = 0; i < _rows; ++i)
        for (size_t j = 0; j < _cols; ++j)
            _data[i][j] -= matrix[i][j];

    return *this;
}

template <MatrixType T>
template <MatrixType T2>
    requires MatrixSub<T, T2>
Matrix<T> &Matrix<T>::operator-=(const T2 &elem)
{
    for (size_t i = 0; i < _rows; ++i)
        for (size_t j = 0; j < _cols; ++j)
            this->_data[i][j] -= elem;

    return *this;
}

template <MatrixType T>
template <MatrixType T2>
    requires MatrixSub<T, T2>
decltype(auto) Matrix<T>::sub_matrix(const Matrix<T2> &matrix) const
{
    return operator-(matrix);
}

template <MatrixType T>
template <MatrixType T2>
    requires MatrixSub<T, T2>
decltype(auto) Matrix<T>::sub_elem(const T2 &elem) const
{
    return operator-(elem);
}

template <MatrixType T>
template <MatrixType T2>
    requires MatrixSub<T, T2>
Matrix<T> &Matrix<T>::sub_eq_matrix(const Matrix<T2> &matrix)
{
    return operator-=(matrix);
}

template <MatrixType T>
template <MatrixType T2>
    requires MatrixSub<T, T2>
Matrix<T> &Matrix<T>::sub_eq_elem(const T2 &elem)
{
    return operator-=(elem);
}

// ///////////////////////////////////////////////////////////////////////////////////////////////////

template <MatrixType T>
template <MatrixType T2>
    requires MatrixMul<T, T2>
decltype(auto) Matrix<T>::operator*(const Matrix<T2> &matrix) const
{
    _check_mult_sizes(matrix);

    Matrix<decltype((*this)[0][0] * matrix[0][0])> tmp(_rows, matrix._cols);

    for (size_t i = 0; i < _rows; ++i)
        for (size_t j = 0; j < matrix._cols; ++j)
            for (size_t k = 0; k < _cols; ++k)
                tmp[i][j] += _data[i][k] * matrix[k][j];

    return tmp;
}

template <MatrixType T>
template <MatrixType T2>
    requires MatrixMul<T, T2>
decltype(auto) Matrix<T>::operator*(const T2 &elem) const
{
    Matrix<decltype((*this)[0][0] * elem)> tmp(_rows, _cols);

    for (size_t i = 0; i < _rows; ++i)
        for (size_t j = 0; j < _cols; ++j)
            tmp[i][j] = _data[i][j] * elem;

    return tmp;
}

template <MatrixType T>
template <MatrixType T2>
    requires MatrixMul<T, T2>
Matrix<T> &Matrix<T>::operator*=(const Matrix<T2> &matrix)
{
    _check_mult_sizes(matrix);

    Matrix<T> tmp(_rows, _cols);

    for (size_t i = 0; i < _rows; ++i)
        for (size_t j = 0; j < _cols; ++j)
            for (size_t k = 0; k < _rows; ++k)
                tmp[i][j] += _data[i][k] * matrix[k][j];

    *this = tmp;

    return *this;
}

template <MatrixType T>
template <MatrixType T2>
    requires MatrixMul<T, T2>
Matrix<T> &Matrix<T>::operator*=(const T2 &elem)
{
    for (size_t i = 0; i < _rows; ++i)
        for (size_t j = 0; j < _cols; ++j)
            this->_data[i][j] *= elem;

    return *this;
}

template <MatrixType T>
template <MatrixType T2>
    requires MatrixMul<T, T2>
decltype(auto) Matrix<T>::mul_matrix(const Matrix<T2> &matrix) const
{
    return operator*(matrix);
}

template <MatrixType T>
template <MatrixType T2>
    requires MatrixMul<T, T2>
decltype(auto) Matrix<T>::mul_elem(const T2 &elem) const
{
    return operator*(elem);
}

template <MatrixType T>
template <MatrixType T2>
    requires MatrixMul<T, T2>
Matrix<T> &Matrix<T>::mul_eq_matrix(const Matrix<T2> &matrix)
{
    return operator*=(matrix);
}

template <MatrixType T>
template <MatrixType T2>
    requires MatrixMul<T, T2>
Matrix<T> &Matrix<T>::mul_eq_elem(const T2 &elem)
{
    return operator*=(elem);
}

template <MatrixType T>
template <MatrixType T2>
    requires MatrixDiv<T, T2>
decltype(auto) Matrix<T>::operator/(const Matrix<T2> &matrix) const
{
    Matrix<T> tmp(matrix);

    tmp.inverse();

    return operator*(tmp);
}

template <MatrixType T>
template <MatrixType T2>
    requires MatrixDiv<T, T2>
decltype(auto) Matrix<T>::operator/(const T2 &elem) const
{
    if (elem == 0)
        throw InvalidArgument(__FILE__, typeid(*this).name(), __LINE__, "Нулевой делитель");

    Matrix<T> tmp(_rows, _cols);

    for (size_t i = 0; i < _rows; ++i)
        for (size_t j = 0; j < _cols; ++j)
            tmp[i][j] = _data[i][j] / elem;

    return operator*(tmp);
}

template <MatrixType T>
template <MatrixType T2>
    requires MatrixDiv<T, T2>
Matrix<T> &Matrix<T>::operator/=(const Matrix<T2> &matrix)
{
    Matrix<T> tmp = operator/(matrix);

    *this = tmp;

    return *this;
}

template <MatrixType T>
template <MatrixType T2>
    requires MatrixDiv<T, T2>
Matrix<T> &Matrix<T>::operator/=(const T2 &elem)
{
    if (elem == 0)
        throw InvalidArgument(__FILE__, typeid(*this).name(), __LINE__, "Нулевой делитель");

    for (size_t i = 0; i < _rows; ++i)
        for (size_t j = 0; j < _cols; ++j)
            _data[i][j] /= elem;

    return *this;
}

template <MatrixType T>
template <MatrixType T2>
    requires MatrixDiv<T, T2>
decltype(auto) Matrix<T>::div_matrix(const Matrix<T2> &matrix) const
{
    return operator/(matrix);
}

template <MatrixType T>
template <MatrixType T2>
    requires MatrixDiv<T, T2>
decltype(auto) Matrix<T>::div_elem(const T2 &elem) const
{
    return operator/(elem);
}

template <MatrixType T>
template <MatrixType T2>
    requires MatrixDiv<T, T2>
Matrix<T> &Matrix<T>::div_eq_matrix(const Matrix<T2> &matrix)
{
    return operator/=(matrix);
}

template <MatrixType T>
template <MatrixType T2>
    requires MatrixDiv<T, T2>
Matrix<T> &Matrix<T>::div_eq_elem(const T2 &elem)
{
    return operator/=(elem);
}

// ///////////////////////////////////////////////////////////////////////////////////////////////////

template <MatrixType T>
Matrix<T> Matrix<T>::operator-()
    requires NumericMatrix<T>
{
    Matrix<T> tmp(_rows, _cols);

    for (size_t i = 0; i < _rows; ++i)
        for (size_t j = 0; j < _cols; ++j)
            tmp[i][j] = -_data[i][j];

    return tmp;
}

template <MatrixType T>
Matrix<T> Matrix<T>::neg()
    requires NumericMatrix<T>
{
    return operator-();
}

// ///////////////////////////////////////////////////////////////////////////////////////////////////

template <MatrixType T>
T Matrix<T>::determinant() const
    requires MatrixFloatPoint<T>
{
    if (!is_square())
        throw InvalidState(__FILE__, typeid(*this).name(), __LINE__, "Матрица должна быть квадратной для получения определителя;");

    if (this->get_rows() == 2)
        return _data[0][0] * _data[1][1] - _data[1][0] * _data[0][1];

    if (this->get_rows() == 1)
        return _data[0][0];

    Matrix<T> tmp(*this); // вызываю конструктор копирования

    double det = 1;

    for (size_t i = 0; i < tmp.get_rows(); ++i)
    {
        double mx = fabs(tmp[i][i]);
        size_t idx = i;

        for (size_t j = i + 1; j < tmp.get_rows(); ++j)
            if (mx < fabs(tmp[i][j]))
            {
                mx = fabs(tmp[i][j]);
                idx = j;
            }

        if (idx != i)
        {
            for (size_t j = i; j < tmp.get_rows(); ++j)
            {
                T t = tmp[j][i];
                tmp[j][i] = tmp[j][idx];
                tmp[j][idx] = t;
            }
            det = -det;
        }

        for (size_t k = i + 1; k < tmp.get_rows(); ++k)
        {
            T t = tmp[k][i] / tmp[i][i];

            for (size_t j = i; j < tmp.get_rows(); ++j)
                tmp[k][j] -= tmp[i][j] * t;
        }
    }

    for (size_t i = 0; i < tmp.get_rows(); ++i)
        det *= tmp[i][i];

    return det;
}

template <MatrixType T>
Matrix<T> Matrix<T>::transpose()
    requires MatrixFloatPoint<T>
{
    if (!is_square())
        throw InvalidState(__FILE__, typeid(*this).name(), __LINE__, "Матрица должна быть квадратной для получения транспонированной матрицы;");

    Matrix<T> tmp(_cols, _rows);

    for (size_t i = 0; i < _rows; ++i)
        for (size_t j = 0; j < _cols; ++j)
            tmp[j][i] = _data[i][j];

    return tmp;
}

template <MatrixType T>
Matrix<T> Matrix<T>::identity()
{
    if (!is_square())
        throw InvalidState(__FILE__, typeid(*this).name(), __LINE__, "Матрица должна быть квадратной для получения единичной матрицы;");

    Matrix<T> tmp(_cols, _rows);

    for (size_t i = 0; i < tmp.get_rows(); i++)
        for (size_t j = 0; j < tmp.get_cols(); j++)
            tmp[i][j] = i == j;

    return tmp;
}

template <MatrixType T>
Matrix<T> Matrix<T>::inverse()
    requires MatrixFloatPoint<T>
{
    if (!is_square())
        throw InvalidState(__FILE__, typeid(*this).name(), __LINE__, "Матрица должна быть квадратной для получения обратной матрицы;");

    T temp;

    Matrix<T> iden = identity();
    Matrix<double> tmp(*this);

    for (size_t k = 0; k < tmp.get_rows(); k++)
    {
        temp = tmp[k][k];

        for (size_t j = 0; j < tmp.get_rows(); j++)
        {
            tmp[k][j] /= temp;
            iden[k][j] /= temp;
        }

        for (size_t i = k + 1; i < tmp.get_rows(); i++)
        {
            temp = tmp[i][k];

            for (size_t j = 0; j < tmp.get_rows(); j++)
            {
                tmp[i][j] -= tmp[k][j] * temp;
                iden[i][j] -= iden[k][j] * temp;
            }
        }
    }

    for (int k = tmp.get_rows() - 1; k > 0; k--)
    {
        for (int i = k - 1; i >= 0; i--)
        {
            temp = tmp[i][k];

            for (size_t j = 0; j < tmp.get_rows(); j++)
            {
                tmp[i][j] -= tmp[k][j] * temp;
                iden[i][j] -= iden[k][j] * temp;
            }
        }
    }

    for (size_t i = 0; i < tmp.get_rows(); i++)
        for (size_t j = 0; j < tmp.get_rows(); j++)
            tmp[i][j] = iden[i][j];

    return tmp;
}

// ///////////////////////////////////////////////////////////////////////////////////////////////////

template <MatrixType T>
// квадратная ли матрица
bool Matrix<T>::is_square() const
{
    return _rows == _cols;
}

template <MatrixType T>
// размер матрицы
size_t Matrix<T>::size() const
{
    return _rows * _cols;
}

template <MatrixType T>
// заполнить часть матрицы значениями
void Matrix<T>::fill(Iterator<T> start, const Iterator<T> &end, const T &value)
{
    for (Iterator<T> it = start; it < end; ++it)
        *it = value;
}

template <MatrixType T>
// получить элемент матрицы
T &Matrix<T>::get_elem(size_t row, size_t col)
{
    return _data[row][col];
}

template <MatrixType T>
// получить элемент матрицы
const T &Matrix<T>::get_elem(size_t row, size_t col) const
{
    return _data[row][col];
}

// ///////////////////////////////////////////////////////////////////////////////////////////////////

template <MatrixType T>
void Matrix<T>::_check_index(size_t pos, size_t limit) const
{
    if (pos <= limit)
        return;

    throw IndexError(__FILE__, typeid(*this).name(), __LINE__, "Индекс больше, чем размеры");
}

template <MatrixType T>
template <MatrixType T2>
void Matrix<T>::_check_sizes(const Matrix<T2> &matrix) const
{
    if (this->_rows == matrix.get_rows() && this->_cols == matrix.get_cols())
        return;

    throw IncompatibleElements(__FILE__, typeid(*this).name(), __LINE__, "Размеры матриц различны");
}

template <MatrixType T>
template <MatrixType T2>
void Matrix<T>::_check_mult_sizes(const Matrix<T2> &matrix) const
{
    if (_cols == matrix.get_rows())
        return;

    throw IncompatibleElements(__FILE__, typeid(*this).name(), __LINE__,
                               "Некорректные размеры матрицы для операции умножения");
}

// ///////////////////////////////////////////////////////////////////////////////////////////////////

template <MatrixType T>
void Matrix<T>::_move_row(size_t from, size_t to)
{
    auto tmp = _data[from];

    for (size_t i = from; i > to; --i)

        _data[i] = _data[i - 1];

    for (size_t i = from; i < to; ++i)
        _data[i] = _data[i + 1];

    _data[to] = tmp;
}

template <MatrixType T>
void Matrix<T>::_move_col(size_t from, size_t to)
{
    for (size_t j = 0; j < _rows; ++j)
    {
        auto tmp = _data[j][from];

        for (size_t i = from; i > to; --i)
            _data[j][i] = _data[j][i - 1];

        for (size_t i = from; i < to; ++i)
            _data[j][i] = _data[j][i + 1];

        _data[j][to] = tmp;
    }
}

// ///////////////////////////////////////////////////////////////////////////////////////////////////

template <MatrixType T>
// конструктро без параметров
Matrix<T>::MatrixRow::MatrixRow() : _data_row(nullptr), _size_row(0)
{
}

template <MatrixType T>
// конструктор создания строки матрицы
Matrix<T>::MatrixRow::MatrixRow(const T *data_row, const size_t size_row) : _data_row(data_row), _size_row(size_row)
{
}

// ///////////////////////////////////////////////////////////////////////////////////////////////////

template <MatrixType T>
// получение элемента строки через индексацию
T &Matrix<T>::MatrixRow::operator[](const size_t index)
{
    if (index >= _size_row)
        throw IndexError(__FILE__, typeid(*this).name(), __LINE__, "Неверный индекс столбца");

    return _data_row[index];
}

template <MatrixType T>
// получение элемента строки через индексацию (константное)
const T &Matrix<T>::MatrixRow::operator[](const size_t index) const
{
    if (index >= _size_row)
        throw IndexError(__FILE__, typeid(*this).name(), __LINE__, "Неверный индекс столбца");

    return _data_row[index];
}

// ///////////////////////////////////////////////////////////////////////////////////////////////////

template <MatrixType T>
// переустановка новых данных
void Matrix<T>::MatrixRow::reset(T *data_row, const size_t size_row)
{
    _size_row = size_row;
    _data_row.reset(data_row);
}

template <MatrixType T>
// сброс данных
void Matrix<T>::MatrixRow::reset()
{
    _size_row = 0;
    _data_row.reset();
}

// ///////////////////////////////////////////////////////////////////////////////////////////////////

template <MatrixType T>
// вывод матрицы на экран
std::ostream &operator<<(std::ostream &out, const Matrix<T> &matrix)
{
    bool first_row = true;
    bool first_col = true;

    for (size_t i = 0; i < matrix.get_rows(); ++i)
    {
        first_col = true;
        if (!first_row)
            out << "\n";
        first_row = false;

        for (size_t j = 0; j < matrix.get_cols(); ++j)
        {
            if (!first_col)
                out << '\t';
            first_col = false;
            out << matrix[i][j];
        }
    }

    return out;
}

#endif // __MATRIX_HPP__