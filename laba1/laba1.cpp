#include <io.h>
#include <string>
#include <iostream>

using namespace std;

class FileIterator
{
private:
    inline bool IsDirectory(const _finddata_t &FindData)
    {
        return ((FindData.attrib & _A_SUBDIR) != 0);
    }
    int compareToMask(const string& mask, const string& file)
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
                return 0;
            }
        return 1;
    }
public:
    FileIterator() {}
    void Next(const string& FileMask)
    {
        _finddata_t FindData;   // unique handle to call to _findnext and _findclose functions
        intptr_t    FindHandle; // structure that receives information about the found file or subdirectory

  // The _findfirst function searches a directory for a file or subdirectory
  // whose name matches the specified name
  // FileMask.c_str() - Pointer to a null-terminated string that specifies a valid directory
  //   or path and file name, which can contain wildcard characters (* and ?)
  // FindData - structure that receives information about the found file or subdirectory
  // FindHandle - unique handle to subsequent call to _findnext function (=-1L if the function fails)
        string defFileMask = FileMask.substr(0, FileMask.find_last_of('\\') + 1) + "*.*";
        FindHandle = _findfirst(defFileMask.c_str(), &FindData);
        if (FindHandle == -1L)
            return;
        /*if (FindData.name[0] == '.')
            if (_findnext(FindHandle, &FindData) == 0)
            {
                _findclose(FindHandle);
                return;
            }*/
        do
        {
            /*cout << (IsDirectory(FindData) ? "Directory" : "File") << ": " <<
            FindData.name << endl;*/
            if ((FindData.name != string(".")) && (FindData.name != string("..")))
                if (IsDirectory(FindData))
                {
                    //string newFileMask = FileMask.insert(FileMask.find_last_of('\\') + 1, FindData.name + '\\');
                    string newFileMask = FileMask;
                    newFileMask.insert(FileMask.find_last_of('\\') + 1, string(FindData.name) + '\\');
                    Next(newFileMask);
                }
                else if (compareToMask(FileMask.substr(FileMask.find_last_of('\\') + 1), string(FindData.name)))
                {
                    cout << "File :" << FindData.name << endl;
                    cout << "In directory: " << defFileMask.substr(0, defFileMask.find_last_of('\\')) << endl << endl;
                }
        }
  // The _findnext function continues a file search from a previous call to the _findfirst
  // FindHandle - search handle returned by a previous call to the _findfirst
  // FindData - structure that receives information about the found file or subdirectory
  // The function return 0 if succeded
        while (_findnext(FindHandle, &FindData) == 0);

  // The _findclose function closes the specified search handle
  // FindHandle - file search handle which must have been previously opened by the _findfirst function
        _findclose(FindHandle);
    }
};

/*inline bool IsDirectory(const _finddata_t &FindData)
{
  return ((FindData.attrib & _A_SUBDIR) != 0);
}

void DisplayFiles(const string& FileMask)
{
  _finddata_t FindData;   // unique handle to call to _findnext and _findclose functions
  intptr_t    FindHandle; // structure that receives information about the found file or subdirectory

  // The _findfirst function searches a directory for a file or subdirectory
  // whose name matches the specified name
  // FileMask.c_str() - Pointer to a null-terminated string that specifies a valid directory
  //   or path and file name, which can contain wildcard characters (* and ?)
  // FindData - structure that receives information about the found file or subdirectory
  // FindHandle - unique handle to subsequent call to _findnext function (=-1L if the function fails)
  FindHandle = _findfirst(FileMask.c_str(), &FindData);
  if (FindHandle == -1L)
    return;

  do
  {
    cout << (IsDirectory(FindData) ? "Directory" : "File") << ": " <<
      FindData.name << endl;
  }
  // The _findnext function continues a file search from a previous call to the _findfirst
  // FindHandle - search handle returned by a previous call to the _findfirst
  // FindData - structure that receives information about the found file or subdirectory
  // The function return 0 if succeded
  while (_findnext(FindHandle, &FindData) == 0);

  // The _findclose function closes the specified search handle
  // FindHandle - file search handle which must have been previously opened by the _findfirst function
  _findclose(FindHandle);
}*/

int main(int argc, char *argv[])
{
    string FileMask;
    FileIterator f;

    if (argc > 1)
        FileMask = argv[1];
    else
    {
        cout << "Enter a drive, directory, and/or file mask to list ([drive:][path]filemask): ";
        cin >> FileMask;
        cout << endl;
        if (!FileMask.length())
        return 1;
    }

    //DisplayFiles(FileMask);
    f.Next(FileMask);
}
