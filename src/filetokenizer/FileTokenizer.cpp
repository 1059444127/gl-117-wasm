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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "FileTokenizer.h"



FileTokenizer::FileTokenizer ()
{
  quoteRemove = 0;
  file = NULL;
  ptr = MAXLEN;
  commentNum = 0;
  bufferLen = MAXLEN;
  line = 1;
  currentQuotes = 0;
  currentComment = 0;
  bufferStart = 0;
  bufferRead = 0;
  lastChar = 0;
  reverse = 0;
  tokenPos = 0;
}

void FileTokenizer::init ()
{
  memset (whitespace, 0, 256);
  memset (separator, 0, 256);
  memset (quotes, 0, 256);
  memset (commentStart, 0, MAXCOMMENTS * 256);
  memset (commentEnd, 0, MAXCOMMENTS * 256);
  commentNum = 0;
  quoteRemove = 0;
  currentQuotes = 0;
  currentComment = 0;
  reverse = 0;
  bufferRead = 0;
  bufferStart = 0;
  buffer [MAXLEN] = 0;
}

void FileTokenizer::initComments ()
{
  memset (commentStart, 0, MAXCOMMENTS * 256);
  memset (commentEnd, 0, MAXCOMMENTS * 256);
  commentNum = 0;
}

XLONG FileTokenizer::getPosition () const
{
  if (!reverse)
    return (XLONG) ftell (file) - MAXLEN + ptr;
  else
    return (XLONG) ftell (file) - ptr;
}

int FileTokenizer::getLine () const
{
  return line;
}

void FileTokenizer::setWhitespace (const char *string)
{
  strncpy (whitespace, string, 256);
  whitespace [255] = 0;
}

void FileTokenizer::setSeparator (const char *string)
{
  strncpy (separator, string, 256);
  separator [255] = 0;
}

void FileTokenizer::addComment (const char *startstring, const char *endstring)
{
  if (commentNum >= MAXCOMMENTS) return;
  strncpy (commentStart [commentNum], startstring, 256);
  commentStart [commentNum] [255] = 0;
  strncpy (commentEnd [commentNum], endstring, 256);
  commentEnd [commentNum] [255] = 0;
  commentNum ++;
}

void FileTokenizer::setQuotes (const char *string)
{
  strncpy (quotes, string, 256);
  quotes [255] = 0;
}

void FileTokenizer::revertBuffer (int start, int end)
{
  int i;
  char c;
  for (i = 0; i <= (end - start) / 2; i ++)
  {
    c = buffer [start + i];
    buffer [start + i] = buffer [end - i];
    buffer [end - i] = c;
  }
}

int FileTokenizer::fillBuffer ()
{
  int bytesread = 0;
  int n = MAXLEN - ptr;

  int bytestoread = ptr;

  if (ptr < MAXLEN)
    if (buffer [ptr] == 0)
      return 0;

  if (ptr <= MAXLEN / 2) return 1;

  memcpy (buffer, &buffer [ptr], n);

  if (reverse)
  {
    if (bufferStart - bytestoread - bufferRead >= 0)
    {
      fseek (file, -bytestoread - bufferRead, SEEK_CUR);
      bufferStart += -bytestoread - bufferRead;
    }
    else
    {
      bytestoread = (int) bufferStart - bufferRead;
      fseek (file, 0, SEEK_SET);
      bufferStart = 0;
      memset (&buffer [n + bytestoread], 0, MAXLEN - (n + bytestoread));
    }
  }

  bytesread = fread (&buffer [n], 1, bytestoread, file);
  bufferLen = bufferLen + bytesread - ptr;
  bufferRead = bytesread;

  bufferStart += bytesread;
  if (reverse)
  {
    if (bytesread > 0)
      revertBuffer (n, n + bytesread - 1);
  }

  if (bytesread != ptr)
  {
    if (bufferLen >= 0 && bufferLen < MAXLEN)
      buffer [bufferLen] = 0;
  }
  ptr = 0;
  return 1;
}

void FileTokenizer::revert ()
{
  if (reverse)
  {
    fseek (file, -bufferRead - ptr + bufferLen, SEEK_CUR);
    bufferStart += -bufferRead - ptr + bufferLen;
  }
  bufferRead = bufferLen - ptr;
  reverse = !reverse;
  ptr = MAXLEN;
  bufferLen = MAXLEN;
  fillBuffer ();
}

int FileTokenizer::setPosition (XLONG pointer, int whence)
{
  if (fseek (file, 0, whence))
    return 0;

  if (whence == 1)
  {
    if (!reverse)
      pointer += -MAXLEN + ptr;
    else
      pointer += -ptr;
  }

  while (pointer > 0x77777777)
  {
    if (fseek (file, 0x77777777, 1))
      return 0;
    pointer -= 0x77777777;
  }
  while (pointer < -0x77777777)
  {
    if (fseek (file, -0x77777777, 1))
      return 0;
    pointer += 0x77777777;
  }
  if (!fseek (file, (long) pointer, 1))
  {
    bufferStart = ftell (file);
    bufferRead = 0;
    ptr = MAXLEN;
    bufferLen = MAXLEN;
    fillBuffer ();
    return 1;
  }
  return 0;
}

void FileTokenizer::rewind ()
{
  fseek (file, 0, SEEK_SET);
  reverse = 0;
  ptr = MAXLEN;
  bufferLen = MAXLEN;
  bufferStart = 0;
  fillBuffer ();
  line = 1;
}

int FileTokenizer::open (const char *filename)
{
  file = fopen (filename, "rb");
  if (!file) return 0;
  strncpy (this->filename, filename, 4096);
  rewind ();
  return 1;
}

int FileTokenizer::close ()
{
  if (file)
    return !fclose (file);
  else
    return 1;
}

int FileTokenizer::isChar (const char *string) const
{
  int i = 0;
  while (string [i] != 0 && i < 256)
  {
    if (buffer [ptr] == string [i])
      return 1;
    i ++;
  }
  return 0;
}

int FileTokenizer::isCommentStart ()
{
  int i, i2;
  for (i = 0; i < commentNum; i ++)
  {
    for (i2 = 0; i2 < (int) strlen (commentStart [i]); i2 ++)
      if (buffer [ptr + i2] != commentStart [i] [i2])
        break;
    if (i2 == (int) strlen (commentStart [i]))
    {
      currentComment = i;
      return 1;
    }
  }
  return 0;
}

int FileTokenizer::isCommentEnd ()
{
  int i, i2;
  i = currentComment;
  for (i2 = 0; i2 < (int) strlen (commentEnd [i]); i2 ++)
    if (buffer [ptr + i2] != commentEnd [i] [i2])
      break;
  if (i2 == (int) strlen (commentEnd [i]))
  {
    for (i = 0; i < i2; i ++)
      (void) nextChar ();
    return 1;
  }
  return 0;
}

int FileTokenizer::find (const char *string)
{
  char *result = NULL;
  int stringlen = strlen (string);

  if (stringlen >= MAXLEN / 2)
    return 0;

  while (result == NULL)
  {
    result = strstr (&buffer [ptr], string);
    if (result == NULL)
    {
      ptr = bufferLen - stringlen;
      if (bufferLen < MAXLEN)
        return 0;
      if (!fillBuffer ())
        return 0;
    }
    else
    {
      ptr += (int) (result - (&buffer [ptr]));
      fillBuffer ();
      return 1;
    }
  }
  return 1;
}

int FileTokenizer::nextChar ()
{
  if (buffer [ptr] == '\n' && lastChar != 92)
    line ++;
  lastChar = buffer [ptr];
  ptr ++;
  return fillBuffer ();
}

int FileTokenizer::fillToken (char *token, int tokenlen)
{
  if (tokenlen == 0)
    return 0;
  if (tokenPos >= tokenlen - 1)
  {
    token [tokenlen - 1] = 0;
    return 0;
  }
  token [tokenPos ++] = buffer [ptr];
  token [tokenPos] = 0;
  return 1;
}

int FileTokenizer::nextToken (char *token, int tokenlen)
{
  char last = 0;
  tokenPos = 0;
  if (tokenlen <= 0) return 1;
  token [0] = '\0';

  if (buffer [ptr] == 0)
  {
    return 0;
  }

  while (1)
  {
    if (isChar (whitespace))
    {
      if (!nextChar ())
        return 0;
    }
    else if (isChar (separator))
    {
      if (!fillToken (token, tokenlen))
        return 1;
      if (!nextChar ())
        return 0;
      return 1;
    }
    else if (isCommentStart ())
    {
      do
      {
        if (!nextChar ())
          return 0;
      } while (!isCommentEnd ());
    }
    else if (isChar (quotes))
    {
      currentQuotes = buffer [ptr];
      if (!quoteRemove)
        if (!fillToken (token, tokenlen)) /* opening quotes */
          return 1;
      last = 0;
      while (1)
      {
        last = buffer [ptr];
        if (!nextChar ())
          return 0;
        if (buffer [ptr] == currentQuotes && last != '\\')
        {
          if (!quoteRemove)
            if (!fillToken (token, tokenlen)) /* closing quotes */
              return 1;
          if (!nextChar ())
            return 0;
          return 1;
        }
        if (buffer [ptr] != '\\')
        {
          if (last != '\\')
          {
            if (!fillToken (token, tokenlen))
              return 1;
          }
          else
          {
            if (buffer [ptr] == 'n') buffer [ptr] = '\n';
            else if (buffer [ptr] == 't') buffer [ptr] = '\t';
            else if (buffer [ptr] == 'r') buffer [ptr] = '\r';
            else if (buffer [ptr] == 'a') buffer [ptr] = '\a';
            else if (buffer [ptr] == '\\') buffer [ptr] = '\\';
            if (!fillToken (token, tokenlen))
              return 1;
          }
        }
      }
    }
    else
    {
      while (1)
      {
        if (!fillToken (token, tokenlen))
          return 1;
        if (!nextChar ())
          return 0;
        if (isChar (whitespace))
          return 1;
        if (isChar (separator))
          return 1;
        if (isCommentStart ())
          return 1;
        if (isChar (quotes))
          return 1;
      }
    }
  }
  return 1;
}

const char *FileTokenizer::getFilename () const
{
  return filename;
}

#endif
