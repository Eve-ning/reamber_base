#pragma once 
#include <exception>
#include <QString>

#ifdef AMBER_BASE_EX                                                // Declare this when compiling the library!
#define AMBER_BASE __declspec(dllexport)
#else
#define AMBER_BASE __declspec(dllimport)
#endif

// Handles all of reamber's exceptions

class AMBER_BASE ReamberException : public std::exception
{
public:
    ReamberException(const char* msg);
    ReamberException& operator=(const ReamberException& o);
    ReamberException(const ReamberException& o);

    virtual ~ReamberException() noexcept;

    const char* what() const noexcept;

protected:
    QString m;

private:
    ReamberException();
};
