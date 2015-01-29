#include "RobustIO.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

#define ERR_EXIT(m) \
    do{ \
        perror(m); \
        exit(EXIT_FAILURE); \
    }while(0)


RobustIO::RobustIO(int fd)
    :fd_(fd), cnt_(0), ptr_(buffer_)
{
    ::memset(buffer_, 0, kBufferSize); 
}

ssize_t RobustIO::read(char *usrbuf, size_t n)
{
    while(cnt_ <= 0)
    {
        ssize_t nread = ::read(fd_, buffer_, kBufferSize);
        if(nread == -1)
        {
            if(errno == EINTR)
            {
                continue;
            }

            return -1;
        }
        else if(nread == 0)
        {
            return 0;
        }

        cnt_ = nread;
        ptr_ = buffer_;
    }

    int cnt = (cnt_ < n) ? cnt_ : n;
    ::memcpy(usrbuf, ptr_, cnt);
    ptr_ += cnt;
    cnt_ -= cnt;

    return cnt;
}

ssize_t RobustIO::readn(char *usrbuf, size_t count)
{
   size_t nleft = count;
   size_t nread;
   char *bufp = usrbuf;

   while(nleft > 0)
   {
       nread = this->read(bufp, nleft);
       if(nread == -1)
       {
           return -1;
       }
       else if(nread == 0)
       {
           break; 
       }

       nleft -= nread;
       bufp += nread;
   }

   return (count - nleft);
}

ssize_t RobustIO::readLine(char *usrbuf, size_t maxlen)
{
    int i;
    int nread;
    char *bufp = usrbuf;
    char c;

    for(i = 0; i< maxlen - 1; i++)
    {
        if((nread = this->read(&c, 1)) == -1)
        {
            return -1;
        }
        else if(nread == 0)
        {
            if(i == 0)
            {
                return 0;
            }

            break;
        }

        *bufp++ = c;
        if(c == '\n')
        {
            break;
        }
    }

    *bufp = '\0';
    return i;
}

ssize_t RobustIO::writen(int fd, const void *buf, size_t count)
{
    size_t nleft = count;
    ssize_t nwrite;
    const char *bufp = (const char *)buf;
    
    while(nleft > 0)
    {
        nwrite = write(fd, bufp, nleft);
        if(nwrite <= 0)
        {
            if(nwrite == -1 && errno == EINTR)
            {   
                continue;
            }

            return -1;
        }

        nleft -= nwrite;
        bufp += nwrite;
    }

    return count;
}

