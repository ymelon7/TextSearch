#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <muduo/base/Logging.h>

using namespace std;

string integerToString(int digit)
{
    char text[100] = {0};
    snprintf(text, sizeof text, "%d", digit);
    return text;
}

int g_count = 1;
ofstream libfile;
ofstream indexfile;

void processFile(const string &filename)
{
    ifstream in(filename.c_str());
    if(!in)
    {
        perror("open file error");
        exit(EXIT_FAILURE); 
    }

    string doc = "";
    doc += "<doc>";
    doc += "<docid>";
    int docid = g_count ++;
    doc += integerToString(docid);
    doc += "</docid>";
   
    string tittle;        //using first line of text as tittle
    getline(in, tittle);
    doc += "<tittle>";
    doc += tittle;
    doc += "</tittle>";

    string content;
    string tmp;
    while(getline(in, tmp))
    {
        content += tmp;
        content += "\n";
        tmp.clear();
    }
    doc += "<content>";
    doc += content;
    doc += "</content>";

    doc += "</doc>";
    
    indexfile << docid << " " << libfile.tellp() << " " << doc.size() << endl;
    libfile << doc << endl;

}


void traversDirRecur(const string &path)
{
    DIR *dir = opendir(path.c_str());
    if(dir == NULL)
        LOG_FATAL << "open dir error" << path;
    
    chdir(path.c_str()); 

    struct dirent *pd; 
    while((pd = readdir(dir)) != NULL)
    {
        if(pd->d_name[0] == '.')
            continue;

        struct stat buf;
        if(lstat(pd->d_name, &buf) == -1)
        {
            LOG_ERROR << "lstat file: " << pd->d_name << "error";
            continue;
        }

        if(S_ISDIR(buf.st_mode))
            traversDirRecur(pd->d_name);
        if(S_ISREG(buf.st_mode))
            processFile(pd->d_name);     
    }

    closedir(dir);
    chdir("..");
}


void traversDir(const string &path)
{
    char tmp[100] = {0};
    if(getcwd(tmp, sizeof tmp) == NULL)
        LOG_FATAL << "getcwd error";

    traversDirRecur(path);

    chdir(tmp);
}

int main(int argc, char *argv[])
{
    libfile.open("../data/libpage.xml");
    indexfile.open("../data/page.index");

    traversDir("/home/melon/TextSearch/data");
    
    libfile.close();
    indexfile.close();
}
