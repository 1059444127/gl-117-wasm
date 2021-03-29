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

#ifndef IS_LOGGING_H
#define IS_LOGGING_H

#include <iostream>

#define DISPLAY(msg,loglevel) Logging::display(msg,loglevel,__FILE__,__LINE__);
#define DISPLAY_FATAL(msg) Logging::display(msg,LOG_FATAL,__FILE__,__LINE__);
#define DISPLAY_ERROR(msg) Logging::display(msg,LOG_ERROR,__FILE__,__LINE__);
#define DISPLAY_WARN(msg) Logging::display(msg,LOG_WARN,__FILE__,__LINE__);
#define DISPLAY_INFO(msg) Logging::display(msg,LOG_INFO,__FILE__,__LINE__);
#define DISPLAY_DEBUG(msg) Logging::display(msg,LOG_DEBUG,__FILE__,__LINE__);

enum LogLevel
{
  LOG_NONE,  ///< do not log anything
  LOG_FATAL, ///< only show fatal (GL-117 will exit) errors (stderr)
  LOG_ERROR, ///< show errors too (stderr)
  LOG_WARN,  ///< show warnings too (stdout)
  LOG_INFO,  ///< show important messages too (stdout)
  LOG_DEBUG  ///< log everything (stdout)
};

// exit codes: call exit() with one of these values!
enum ExitCode
{
  EXIT_NORMAL,
  EXIT_LOADFILE,
  EXIT_INIT,
  EXIT_COMMAND,
  EXIT_ALLOC,
  EXIT_CONTEXT
};

/**
* This class does all logging activities to stdout/stderr and logfile.
* A message is only logged if its loglevel is lower (has higher priority)
* than the overall loglevel.
*/
class Logging
{
  public:
    static FILE *displayOut;
    static std::string filename;
    static LogLevel loglevel; ///< current log/debug level (set to constants LOG_*)

    Logging ();
    ~Logging ();

    static void setLevel (LogLevel loglevel);
    static void setFile (const std::string &filename);
    static void display (const std::string &str, LogLevel level, const char *file, int line);

  protected:

    static void displayStream (FILE *stream, const std::string &str, LogLevel level,
                               const char *filename, int line);
};

#endif
