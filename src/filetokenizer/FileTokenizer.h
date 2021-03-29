/*
    GL-117
    Copyright 2001-2004 Thomas A. Drexl aka heptargon

    This file is part of GL-117.

    GL-117 is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    GL-117 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GL-117; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef IS_FILETOKENIZER_H
#define IS_FILETOKENIZER_H

//#define _LARGEFILE_SOURCE
#define _LARGEFILE_SOURCE64
#define _FILE_OFFSET_BITS 64

// we should use __long here!!!
#define XLONG long

const int MAXCOMMENTS = 10;   ///< maximum number of comment strings
const int MAXLEN = 4096;      ///< maximum length of the file buffer

/**
* This is a low level library to split text files into tokens.
* Customize strings for quotes, whitespace and comments.
* Then parse one token after each other calling nextToken()
* or set the file cursor to a new position calling find(mystr)
*
* Documentation notes:
* - all int-functions return 0 on error/EOF, 1 on success
* - all documented functions are public, the rest is private
*
* Example to parse MEA-files:
*   char token [256];
*   setWhitespace (" \t\r\n-=");
*   setQuotes ("\'{}");
*   addComment ("$", "\n");
*   open ("test.mea");
*   while (find ("[MEA_"))         // search for string [MEA_
*   {
*     nextToken (token, 256);      // read the [MEA_ token
*     printf ("%s\n", token);           // and print it on stdout
*   }
*   rewind ();                     // again from the beginning
*   while (nextToken (token, 256)) // read every token
*   {
*     printf ("%s\n", token);           // print out every token
*   }
*/
class FileTokenizer
{
  public:

    FileTokenizer ();

    /// Init data structure
    void init ();
    /// Init only the comments
    void initComments ();
    /// Revert the file read order (forward/backward)
    void revert ();
    /// Get current position of the file pointer
    XLONG getPosition () const;
    /// Get current line of the file
    int getLine () const;
    /// Set the whitespace chars for this file
    /// Example: setWhitespace (" \n\t");
    void setWhitespace (const char *string);
    /// Set the separator chars for this file
    /// Example: setSeparator (",;");
    void setSeparator (const char *string);
    /// Add comment strings for this file
    /// Example language C++: addComment ("//", "\n");
    void addComment (const char *startstring, const char *endstring);
    /// Set the quote chars for string in this file
    /// Example: setQuotes ("\'\"");
    void setQuotes (const char *string);
    /// Rewind file to an initial state to restart parsing
    void rewind ();
    /// Open file for parsing
    int open (const char *filename);
    /// Close file
    int close ();
    /// Find the first occurance of string from the cursor and set cursor
    /// to the new position, this is quite fast!
    int find (const char *string);
    /// Find the next token from the current cursor position and fill the
    /// token string. Use tokenlen to define a maximum length for token.
    int nextToken (char *token, int tokenlen);
    /// Return the next character.
    int nextChar ();
    /// Return the file name
    const char *getFilename () const;

  protected:

    char filename [4096];
    int reverse;
    int quoteRemove;             ///< 1: remove quotes from tokens
    int ptr;                     ///< buffer pointer to current char = "file cursor"
    FILE *file;                  ///< FILE pointer
    char buffer [MAXLEN+1];      ///< file buffer in RAM
    char whitespace [256];       ///< string containing the whitespace chars
    char separator [256];        ///< string containing the separator chars
    int commentNum;              ///< number of comment strings
    char quotes [256];           ///< string containing the quote chars
    char commentStart [MAXCOMMENTS] [256]; ///< strings for comment beginnings
    char commentEnd [MAXCOMMENTS] [256];   ///< strings for corresponding comment ends
    int bufferLen;               ///< current length of the file buffer
    int line;
    char currentQuotes;
    char currentComment;
    XLONG bufferStart;
    int bufferRead;
    char lastChar;
    int tokenPos;

    void revertBuffer (int start, int end);
    int fillBuffer ();
    int setPosition (XLONG pointer, int whence);
    int isChar (const char *string) const;
    int isCommentStart ();
    int isCommentEnd ();
    int fillToken (char *token, int tokenlen);

};

#endif
