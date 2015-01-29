#ifndef READ_FILE_H
#define READ_FILE_H

#include <string>
#include <boost/noncopyable.hpp>
#include "RobustIO.h"

//class RobustIO;

class ReadFile : boost::noncopyable
{
public:
    ReadFile(const std::string &path);
    ~ReadFile();

    size_t readn(char *usrbuf, size_t count);
    size_t readLine(char *usrbuf, size_t maxlen);
    std::string readnBytesAsString(size_t count);
    std::string readLineAsString();

    long seek() const;
    void seekBeg(long offset);
    void seekCur(long offset);
    void seekEnd(long offset);

    operator bool() const
    { return fd_ != -1; }

private:
    const int fd_;
    RobustIO robustIO_;
};

#endif //READ_FILE_H
