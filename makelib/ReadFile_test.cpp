#include "ReadFile.h"
#include <iostream>
#include <muduo/base/Logging.h>

using namespace std;

int main(int argc, const char *argv[])
{
    ReadFile rf("/home/reubun/xiaosong/test/test.txt");
    if(!rf)
    {
        LOG_ERROR << "open file failed.";
    }

    string line;
    while((line = rf.readLineAsString()) != "")
    {
        cout << line.size() << endl;
        cout << line << endl;
    }
    
    cout <<rf.seek() << endl;
}
