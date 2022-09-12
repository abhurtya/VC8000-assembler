//
//  Implementation of file access class.
//
#include "stdafx.h"
#include "FileAccess.h"


/**/
/*
FileAccess::FileAccess()
NAME
    FileAccess::FileAccess - Used in opening a file from which the code is to be read.
SYNOPSIS
    FileAccess::FileAccess(int argc, char *argv[]);
    argc	->	total number of command-line arguments.
    argv	->	User entered array of characters for file to be opened
DESCRIPTION
    This function is the constructor for FileAccess class that aims to open the file.
RETURN
    void

*/
/**/

FileAccess::FileAccess(int argc, char* argv[])
{
    // Check that there is exactly one run time parameter.
    if (argc != 2) {
        cerr << "Usage: Assem <FileName>" << endl;
        exit(1);
    }
    // Open the file.  One might question if this is the best place to open the file.
    // One might also question whether we need a file access class.
    m_sfile.open(argv[1], ios::in);

    // If the open failed, report the error and terminate.
    if (!m_sfile) {
        cerr << "Source file could not be opened, assembler terminated."
            << endl;
        exit(1);
    }
}
/*FileAccess::FileAccess(int argc, char* argv[])*/


/**/
/*
FileAccess::~FileAccess()
NAME
    FileAccess::FileAccess - Closes the file that we read from
SYNOPSIS
    FileAccess::~FileAccess();

DESCRIPTION
    Destructor for FileAccess class that was used to
    close the file to read the code from.

Returns
    No return value for a destructor.

*/
/**/
FileAccess::~FileAccess()
{
    // Not that necessary in that the file will be closed when the program terminates, but good form.
    m_sfile.close();
}
/*FileAccess::~FileAccess();*/

/**/
/*
FileAccess::GetNextLine()
NAME
    FileAccess::GetNextLine - Get the next line from the file.
SYNOPSIS
    bool FileAccess::GetNextLine(string &a_line);
    a_line -> Consists of the set of characters in the next line.
DESCRIPTION
    Checks if the next lines exists. If it does, then it reads the line
    and stores it in a_line.
Returns
    Returns true if next line exists, else it returns false.

/**/
// Get the next line from the file.
bool FileAccess::GetNextLine(string& a_line)
{
    // If there is no more data, return false.
    if (m_sfile.eof()) {

        return false;
    }
    getline(m_sfile, a_line);

    // Return indicating success.
    return true;
}
/* bool FileAccess::GetNextLine(string &a_line) */

/**/
/*
FileAccess::rewind()
NAME
    FileAccess::Rewind - It rewinds the pointer to beginnning of the file
SYNOPSIS
    void FileAccess::Rewind();
DESCRIPTION
    Cleans all the file flags and goes back to the beginning
    of the file.
RETURN
    void

*/
/**/
void FileAccess::rewind()
{
    // Clean all file flags and go back to the beginning of the file.
    m_sfile.clear();
    m_sfile.seekg(0, ios::beg);
}
/*void FileAccess::rewind()*/
