#include "fileiterator.h"
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    string FileMask;
    FileIterator* it = new FileIterator();

    if (argc > 1)
        fileMask = argv[1];
    else
    {
        cout << "Enter a drive, directory, and/or file mask to list ([drive:][path]filemask): ";
        cin >> fileMask;
        cout << endl;
        if (!fileMask.length())
            return 1;
    }

    it->fileMask(FileMask);

    while (it->hasMore())
        it->next()->show();

    return 0;
}
