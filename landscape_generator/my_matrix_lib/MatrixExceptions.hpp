#ifndef __MATRIXEXCEPTIONS_HPP__
#define __MATRIXEXCEPTIONS_HPP__

#include <cstdio>
#include <cstring>
#include <exception>

class MatrixExceptions : public std::exception
{
public:
    MatrixExceptions(const char *file_info, const char *class_name, const int line_info,
                     const char *err_msg = "No error message")
    {
        // формируем строку сообщения об ошибке с помощью функции snprintf()
        snprintf(_err_msg, sizeof(_err_msg), "Ошибка в классе %s в файле %s:%d: %s",
                 class_name, file_info, line_info, err_msg);
    };

    ~MatrixExceptions(){};

    virtual const char *what() const noexcept override
    {
        return _err_msg;
    };

protected:
    char _err_msg[256]; // статический буфер для хранения сообщения
};

class IndexError : public MatrixExceptions
{
public:
    IndexError(const char *file_info, const char *class_name, const int line_info,
               const char *err_msg = "No error message") : MatrixExceptions(file_info, class_name, line_info, err_msg)
    {
        strcat(_err_msg, " (error type: Index Error)");
    }
};

class MemoryError : public MatrixExceptions
{
public:
    MemoryError(const char *file_info, const char *class_name, const int line_info,
                const char *err_msg = "No error message") : MatrixExceptions(file_info, class_name, line_info, err_msg)
    {
        strcat(_err_msg, " (error type: Memory Error)");
    }
};

class InvalidArgument : public MatrixExceptions
{
public:
    InvalidArgument(const char *file_info, const char *class_name, const int line_info,
                    const char *err_msg = "No error message") : MatrixExceptions(file_info, class_name, line_info, err_msg)
    {
        strcat(_err_msg, " (error type: Invalid Argument)");
    }
};

class IncompatibleElements : public MatrixExceptions
{
public:
    IncompatibleElements(const char *file_info, const char *class_name, const int line_info,
                         const char *err_msg = "No error message") : MatrixExceptions(file_info, class_name, line_info, err_msg)
    {
        strcat(_err_msg, " (error type: Incompatible Elements)");
    }
};

class InvalidState : public MatrixExceptions
{
public:
    InvalidState(const char *file_info, const char *class_name, const int line_info,
                 const char *err_msg = "No error message") : MatrixExceptions(file_info, class_name, line_info, err_msg)
    {
        strcat(_err_msg, " (error type: Invalid State)");
    }
};

class IteratorIndexError : public MatrixExceptions
{
public:
    IteratorIndexError(const char *file_info, const char *class_name, const int line_info,
                       const char *err_msg = "No error message") : MatrixExceptions(file_info, class_name, line_info, err_msg)
    {
        strcat(_err_msg, " (error type: Iterator Index Error)");
    }
};

class IteratorValidationError : public MatrixExceptions
{
public:
    IteratorValidationError(const char *file_info, const char *class_name, const int line_info,
                            const char *err_msg = "No error message") : MatrixExceptions(file_info, class_name, line_info, err_msg)
    {
        strcat(_err_msg, " (error type: Iterator Validation Error)");
    }
};

#endif // __MATRIXEXCEPTIONS_HPP__