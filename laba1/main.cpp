/****************************************************************************

ModuleName:
  FindFileExample.cpp

Abstract:
  This code demonstrates usage of _findfirst, _findnext and _findclose functions

Author:
  Konstantin Kuritsyn

Date:
  2005-04-01

****************************************************************************/

#include <io.h>
#include <string>
#include <iostream>

using namespace std;

inline bool IsDirectory(const _finddata_t &FindData)
{
  return ((FindData.attrib & _A_SUBDIR) != 0);
}

//
// The DisplayFiles function displays files and directories whose names match the FileMask specified
//
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
}

int main(int argc, char *argv[])
{
  string FileMask;

  if (argc > 1)
    FileMask = argv[1];
  else
  {
    cout << "Enter a drive, directory, and/or file mask to list ([drive:][path]filemask): ";
    cin >> FileMask;
    if (!FileMask.length())
      return 1;
  }

  DisplayFiles(FileMask);
}
