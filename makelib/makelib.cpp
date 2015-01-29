#include <iosteam>
#include <string>
#include <fstream>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <muduo/base/Logging.h>

using namespace std;
using namespace muduo;

int g_count = 0;
ofstream libfile;
ofstream indexfile;


std::string integerToString(int num)
{
    char tmp[100] = {0};
    snprintf(tmp, sizeof tmp, "%d", num);

    return tmp;
}

void processFile(const std::string &filename)
{
    ifstream infile(filename.c_str());
    std::string doc = "";
    doc += "<doc>";

    int docid = g_count ++;
    doc += "<docid>";
    doc += integerToString(docid);
    doc += "</docid>";

    std::string title;
    getline(infile, title);
    doc += "<title>";
    doc += title;
    doc += "</title>";

    std::string content;
    std::string tmp;
    while(getline(infile, tmp))
    {
        content += tmp;
        conten += "\n";
    }

    doc += "<conten>";
    doc += content;
    doc += "</content>";

    doc += "</doc>";

    indexfile << docid << " " << libfile.tellp() << " " << doc.size() << endl;
    libfile << doc << endl;

    infile.close();
}


void traversDirectoryWithRecursion(const std::string &path)
{
    DIR *dir = opendir(path.c_str());
    if(dir == NULL)
    {
        LOG_FATAL << "open dir error: " << path;
    }
    
    chdir(path.c_str());

    struct dirent *pd;
    while((pd = readdir(dir)) != NULL)
    {
        if(pd->d_name[0] == '.')
        {
            continue;
        }
        
        //int lstat(const char *path, struct stat *buf)
        struct stat buf;
        if(lstat(pd->d_name, &buf) == -1)
        {
            LOG_ERROR < "lstat file :" << pd->d_name << "error";
            continue;
        }
        

        if(S_ISDIR(buf.st_mode))
        {
            traversDirectoryWithRecursion(pd->d_name);
        }
        else if(S_IDREG(buf.st_mode))
        {
            processFile(pd->d_name);
        }
    }
    
    closedir(dir);
    chdir("..");
}

void traversDirectory(const std::string &path)
{
    char tmp[1024];
    if(getcwd(tmp, sizeof tmp) == NULL)
    {
        LOG_FATAL << "getcwd error";
    }
    
    traversDirectoryWithRecursion(path);
    chdir(tmp);
}


int main(int argc, const char *argv[])
{
    libfile.open("../data/pagelib.xml");
    indexfile.open("../data/page.index");

    traversDirectory("/home/reubun/xiaosong/MyItem/TextSearch/data/");

    libfile.close();
    indexfile.close();
}


