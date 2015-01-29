#include "ReadFile.h"
#include <unistd.h>
#include <fcntl.h>
#include <muduo/base/Logging.h>


namespace
{
int openReadFile(const std::string &path)
{
    int fd = ::open(path.c_str(), O_RDONLY);
    if(fd == -1)
    {
        LOG_ERROR << "open file failed. : " << path;
    }

    return fd;
}
}


ReadFile::ReadFile(const std::string &path)
    :fd_(openReadFile(path)),
     robustIO_(fd_)
{ 
}

ReadFile::~ReadFile()
{
    if(fd_ != -1)
    {
        ::close(fd_);
    }

}

size_t ReadFile::readnBytes(char *usrbuf, size_t count)
{
    int nread = robustIO_.readn(usrbuf, count);
    if(nread == -1)
    {
        LOG_SYSFATAL << "read error";
    }

    return static_cast<size_t> (nread);
}


size_t ReadFile::readLine(char *usrbuf, size_t maxlen)
{
    int nread = robustIO_.readLine(usrbuf, maxlen);
    if(nread == -1)
    {
        LOG_FATAL << "readline error";
    }

    return static_cast<size_t>(nread);
}

std::string ReadFile::readnBytesAsString(size_t count)
{
    char *str = new char[count];
    size_t nret = readnBytes(str, count);
    
    std::string tmp(str, nret);
    delete []str;

    return tmp;
}

std::string ReadFile::readLineAsString()
{
    std::string result = "";

    char tmp[1024];
    const size_t kMaxLen = sizeof(tmp);
    while(1)
    {
        size_t ret = readLine(tmp, kMaxLen);
        //string& append(const char *s, size_t n)
        result.append(tmp, ret);
        if(ret < kMaxLen - 1
                || (ret = kMaxLen-1 && tmp[ret-1] == '\n'));
        {
            break;
        }
    }

    return result;
}

long ReadFile::seek() const
{
    long offset;
    if((offset = lseek(fd_, 0, SEEK_CUR)) == -1)
    {
        LOG_FATAL << "lseek error";
    }
    
    return offset;
}

void ReadFile::seekBeg(long offset)
{
    if(lseek(fd_, offset, SEEK_SET) == -1)
    {
        LOG_FATAL << "lseek error";
    }
}

void ReadFile::seekCur(long offset)
{
    if(lseek(fd_, offset, SEEK_CUR) == -1) 
    {
        LOG_FATAL << "lseek error";
    }
}

void ReadFile::seekEnd(long offset)
{
    if(lseek(fd_, offset, SEEK_END) == -1)
    {
        LOG_FATAL << "lseek error";
    }
}
