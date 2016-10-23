#include "fileiterator.h"
#include <io.h>
#include <string>
#include <iostream>

using namespace std;

//FileIterator::FileIterator() {};

FileIterator::FileIterator(const string& fileMask)
{
    this->fileMask = fileMask;
}

inline bool FileIterator::IsDirectory(const _finddata_t &FindData)
    {
        return ((FindData.attrib & _A_SUBDIR) != 0);
    }
bool FileIterator::compareToMask(const string& mask, const string& file)
{
    int pos = 0;
    for (int i = 0; i < mask.length(); i++)
        for (int j = pos; j < file.length(); j++)
        {
            pos = j;
            if (mask[i] == '*')
            {
                if (file[j] == '.')
                {
                    break;
                }
                else
                    continue;
            }
            else if ((mask[i] == '?') || (mask[i] == file[j]))
            {
                pos++;
                break;
            }
            return false;
        }
    return true;
    }

void FileIterator::setFileMask(const string& mask)
{
    fileMask = mask;
}

FileItem* FileIterator::next()
{
    FileItem* res = cache;
    cache = NULL;
    return res;
}

bool FileIterator::hasMore()
{
    cache = search(fileMask);
    if (this->cache != NULL)
        return true;
    else
        return false;
}

FileItem* FileIterator::search(const string& fileMask)
{
    _finddata_t FindData;

    string defFileMask = fileMask.substr(0, fileMask.find_last_of('\\') + 1) + "*.*";
    if (FindHandle == NULL)
    {
        FindHandle = _findfirst(defFileMask.c_str(), &FindData);
        if (FindHandle == -1L)
            return NULL;
    }
    else
        if (_findnext(FindHandle, &FindData) != 0)
        {
            if ((FindData.name != string(".")) && (FindData.name != string("..")))
            if (IsDirectory(FindData))
            {
                string newFileMask = fileMask;
                newFileMask.insert(fileMask.find_last_of('\\') + 1, string(FindData.name) + '\\');
                this->subIterator = new FileIterator(newFileMask);
                while (this->subIterator->hasMore())
                    this->subIterator->next();
            }
            else if (compareToMask(fileMask.substr(fileMask.find_last_of('\\') + 1), string(FindData.name)))
            {
                string name = string(FindData.name);
                string path = defFileMask.substr(0, defFileMask.find_last_of('\\'));
                if (path == "*.*")
                    path = "root";
                cache = new FileItem(name, path);
                return cache;
            }
        }
        else
        {
            _findclose(FindHandle);
            return NULL;
        }
}

FileItem::FileItem() {};

FileItem::FileItem(const string& name, const string& path)
{
    this->name = name;
    this->path = path;
}

void FileItem::show()
{
    cout << "File :" << name << endl;
    cout << "In directory: " << path << endl << endl;
}
