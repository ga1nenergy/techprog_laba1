#include <string>
#include <io.h>

using namespace std;

class FileItem
{
    std::string name;
    std::string path;
public:
    FileItem();
    FileItem(const std::string& name, const std::string& path);
    void show();
};

class FileIterator
{
    FileItem* cache;
    FileItem* search(const string& fileMask);

    FileIterator* subIterator;

    string fileMask;

    inline bool IsDirectory(const _finddata_t &FindData);
    bool compareToMask(const string& mask, const string& file);

public:
    FileIterator();
    void setFileMask(const string& mask);
    bool hasMore();
    FileItem* next();

};
