#ifndef ROBUST_IO_H
#define ROBUST_IO_H

#include <unistd.h>
#include <boost/noncopyable.hpp>

//简易的小型IO系统
class RobustIO : boost::noncopyable
{
public:
    RobustIO(int fd);

    ssize_t readn(char *usrbuf, size_t count);
    ssize_t writen(int fd, const void *usrbuf, size_t count);
    ssize_t readLine(char *buf, size_t maxlen);
    ssize_t read(char *usrbuf, size_t n);

private:
    static const int kBufferSize = 65536;

    int fd_;  //fd
    int cnt_; //缓冲区可用的字节数
    char *ptr_; //指向缓冲区可用的第一个字节
    char buffer_[kBufferSize]; //缓冲区
};


#endif //ROBUST_IO_H

