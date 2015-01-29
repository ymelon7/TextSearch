#ifndef TRUNC_FILE_H
#define TRUNC_FILE_H

#include <string>
#include "RobustIO.h"
#include <boost/noncopyable.hpp>

class TruncFile : boost::noncopyable
{
public:
    TruncFile(const std::string &path);
    ~TruncFile();

    size_t writenBytes(const char *usrbuf, size_t count);
    void writeString(const std::string &s);
    void writeStringLine(const std::string &s);

    long seek() const;  //获取文件当前偏移量
    long seekBeg(long offset);  //从文件开头偏移指定长度
    long seekCur(long offset);  //从当前位置偏移
    long seekEnd(long offset);  //从文件末尾偏移
    
    operator bool() const
    { return fd_ != -1; }

private:
    const int fd_;
}

#endif //TRUNC_FILE_H
