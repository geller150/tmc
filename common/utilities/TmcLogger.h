/*---------------------------------------------------------------------------*\

        .----------------.  .----------------.  .----------------.
       | .--------------. || .--------------. || .--------------. |
       | |  _________   | || | ____    ____ | || |     ______   | |
       | | |  _   _  |  | || ||_   \  /   _|| || |   .' ___  |  | |
       | | |_/ | | \_|  | || |  |   \/   |  | || |  / .'   \_|  | |
       | |     | |      | || |  | |\  /| |  | || |  | |         | |
       | |    _| |_     | || | _| |_\/_| |_ | || |  \ `.___.'\  | |
       | |   |_____|    | || ||_____||_____|| || |   `._____.'  | |
       | |              | || |              | || |              | |
       | '--------------' || '--------------' || '--------------' |
        '----------------'  '----------------'  '----------------'

 ------------------------------------------------------------------------------
 Original Copyright (C) 2008-2011 Soeren Textor (VirtualFluids, TU Braunschweig, Germany)
   - based on the article from Dr. Dobbs Portal, Logging In C++, September 05, 2007
 Modifications Copyright (C) 2022-2023 Sebastian Geller

 This software is distributed WITHOUT ANY WARRANTY.

 License
    This file has been incorporated in TMC.

    TMC is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TMC is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License along
    with TMC (see LICENSE.txt). If not, see <http://www.gnu.org/licenses/>.

 Description
    logger

\*---------------------------------------------------------------------------*/

#ifndef TMCLOGGER_H
#define TMCLOGGER_H

#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <memory>

#if defined(__MINGW32__) || defined(__MINGW64__)
#include <sys/time.h>
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(__WIN64__)
#include <windows.h>
#else
#include <sys/time.h>
#endif

#if defined(TMC_BOOST) && !defined(NO_THREADSAFE_LOGGING)
#include <boost/thread.hpp>
#endif // TMC_BOOST && !NO_THREADSAFE_LOGGING

//////////////////////////////////////////////////////////////////////////
// TmcLogger
// C++ Logger
// Functionality:
// a TmcLogger object is created for each log entry, the logstring is passed and when "destroying" the logstring
// the logstring is written to a file or to the screen by means of the corresponding policy (=template paramter).
// or on the screen. Different log levels are supported
//
// Help macro:  UBLOG
// Example1:    UBLOG(logINFO) << "Klasse::foo entered"; //endl - no need
//              --> Entry:
//
// Bsp2: siehe Dateiende!
//

enum LogLevel
{
    logERROR,
    logWARNING,
    logINFO,
    logDEBUG,
    logDEBUG1,
    logDEBUG2,
    logDEBUG3,
    logDEBUG4,
    logDEBUG5
};

//////////////////////////////////////////////////////////////////////////
// template <typename OutputPolicy> class Log  - declaration
//////////////////////////////////////////////////////////////////////////
template <typename OutputPolicy>
class TmcLogger
{
public:
    typedef OutputPolicy output_policy;

public:
    TmcLogger();
    virtual ~TmcLogger();
    std::ostringstream &get(const LogLevel &level = logINFO);

public:
    // static, to avoid object creation later
    // in case level is smaller than Level
    static LogLevel &reportingLevel();
    static void setReportingLevel(const LogLevel &level) { TmcLogger::reportingLevel() = level; }

    static std::string logLevelToString(const LogLevel &level);
    static LogLevel logLevelFromString(const std::string &level);

    static std::string logTimeString();

protected:
    std::ostringstream os;

private:
    TmcLogger(const TmcLogger &);
    TmcLogger &operator=(const TmcLogger &);
};

//////////////////////////////////////////////////////////////////////////
// template <typename OutputPolicy> class Log  - implementation
//////////////////////////////////////////////////////////////////////////
template <typename OutputPolicy>
TmcLogger<OutputPolicy>::TmcLogger()
{
}
/*==========================================================*/
template <typename OutputPolicy>
std::ostringstream &TmcLogger<OutputPolicy>::get(const LogLevel &level)
{
    os << logTimeString() << " "
       << std::setw(8) << std::left << TmcLogger<OutputPolicy>::logLevelToString(level) << ": "
       << std::string(level > logDEBUG ? 3 * (level - logDEBUG) : 0, ' '); //<kind of tree output :D

    return os;
}
/*==========================================================*/
template <typename OutputPolicy>
TmcLogger<OutputPolicy>::~TmcLogger()
{
    os << std::endl;
    OutputPolicy::output(os.str());
}
/*==========================================================*/
template <typename OutputPolicy>
LogLevel &TmcLogger<OutputPolicy>::reportingLevel()
{
    static LogLevel reportLevel = logINFO;
    return reportLevel;
}
/*==========================================================*/
template <typename OutputPolicy>
std::string TmcLogger<OutputPolicy>::logLevelToString(const LogLevel &level)
{
    static std::string const buffer[] = {"ERROR", "WARNING", "INFO", "DEBUG", "DEBUG1", "DEBUG2", "DEBUG3", "DEBUG4", "DEBUG5"};
    return buffer[level];
}
/*==========================================================*/
template <typename OutputPolicy>
LogLevel TmcLogger<OutputPolicy>::logLevelFromString(const std::string &level)
{
    if (level == "DEBUG5")
        return logDEBUG5;
    if (level == "DEBUG4")
        return logDEBUG4;
    if (level == "DEBUG3")
        return logDEBUG3;
    if (level == "DEBUG2")
        return logDEBUG2;
    if (level == "DEBUG1")
        return logDEBUG1;
    if (level == "DEBUG")
        return logDEBUG;
    if (level == "INFO")
        return logINFO;
    if (level == "WARNING")
        return logWARNING;
    if (level == "ERROR")
        return logERROR;

    TmcLogger<OutputPolicy>().get(logWARNING) << "TmcLogger<OutputPolicy>::logLevelFromString(level) - unknown logging level '" << level << "'. Using INFO level as default.";
    return logINFO;
}

//////////////////////////////////////////////////////////////////////////
// logTimeString
//////////////////////////////////////////////////////////////////////////
#if defined(__MINGW32__) || defined(__MINGW64__)
template <typename OutputPolicy>
inline std::string TmcLogger<OutputPolicy>::logTimeString()
{
    char buffer[11];
    time_t t;
    time(&t);
    // tm r = {0};
    // strftime(buffer, sizeof(buffer), "%X", localtime_r(&t, &r));
    strftime(buffer, sizeof(buffer), "%X", localtime(&t));
    struct timeval tv;
    gettimeofday(&tv, 0);
    char result[100] = {0};
    std::sprintf(result, "%s.%03ld", buffer, (long)tv.tv_usec / 1000);
    return result;
}
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(__WIN64__)
template <typename OutputPolicy>
inline std::string TmcLogger<OutputPolicy>::logTimeString()
{
    const int MAX_LEN = 200;
    char buffer[MAX_LEN];
    if (GetTimeFormatA(LOCALE_USER_DEFAULT, 0, 0, "HH':'mm':'ss", buffer, MAX_LEN) == 0)
    {
        return "Error in std::string TmcLogger<OutputPolicy>::logTimeString()";
    }

    char result[100] = {0};
    static DWORD first = GetTickCount();
    std::sprintf(result, "%s.%03ld", buffer, (long)(GetTickCount() - first) % 1000);
    return result;
}
#else
template <typename OutputPolicy>
inline std::string TmcLogger<OutputPolicy>::logTimeString()
{
    char buffer[11];
    time_t t;
    time(&t);
    tm r = {0};
    strftime(buffer, sizeof(buffer), "%X", localtime_r(&t, &r));
    struct timeval tv;
    gettimeofday(&tv, 0);
    char result[100] = {0};
    std::sprintf(result, "%s.%03ld", buffer, (long)tv.tv_usec / 1000);
    return result;
}
#endif

//////////////////////////////////////////////////////////////////////////
// Output2Stream (=implementation of OutputPolicy)
//////////////////////////////////////////////////////////////////////////
// Note: the first version with auto_ptr for the stream led to
//       exceptions when using the logger in dtor's stat. global
//       objects. But even the pointer solution is not yet the
//       optimal solution.
class Output2Stream // implementation of OutputPolicy
{
public:
    static std::ostream *&getStream();
    static void output(const std::string &msg);

    // creates output-file-stream (of file opening fails -> stream is set to std::cerr)
    static void setStream(const std::string &filename);

    // direct set outputstream, gcControl = true -> object will be deleted by Output2Stream
    static void setStream(std::ostream *pStream, const bool &gcControl = false);

protected:
#if defined(TMC_BOOST) && !defined(NO_MT_LOGGING)
    static boost::mutex mtx;
#endif
};
/*==========================================================*/
inline std::ostream *&Output2Stream::getStream()
{
    static std::ostream *pStream = &std::clog;
    return pStream;
}
/*==========================================================*/
inline void Output2Stream::setStream(std::ostream *pFile, const bool &gcControl)
{
#if defined(TMC_BOOST) && !defined(NO_MT_LOGGING)
    boost::mutex::scoped_lock lock(mtx);
#endif
    static bool s_gcControl = false;

    if (s_gcControl && Output2Stream::getStream())
    {
        delete Output2Stream::getStream();
    }

    s_gcControl = gcControl;

    Output2Stream::getStream() = pFile;
}
/*==========================================================*/
inline void Output2Stream::setStream(const std::string &filename)
{
    std::ofstream *file = new std::ofstream(filename.c_str());
    if (!(*file))
    {
        delete file;
        Output2Stream::setStream(&std::cerr, false);
        TmcLogger<Output2Stream>().get(logERROR) << " Output2Stream::setStream(const std::string& filename) could not open file "
                                                 << filename << " -> std::cerr is used instead " << std::endl;
        return;
    }
    std::cout << "TmcLog writes to " << filename << std::endl;
    Output2Stream::setStream(file, true);
}
/*==========================================================*/
inline void Output2Stream::output(const std::string &msg)
{
#if defined(TMC_BOOST) && !defined(NO_MT_LOGGING)
    boost::mutex::scoped_lock lock(mtx);
#endif
    std::ostream *pStream = getStream();
    if (!pStream)
        return;
    (*pStream) << msg << std::flush;
}

//////////////////////////////////////////////////////////////////////////
// TmcLog
//////////////////////////////////////////////////////////////////////////
class TmcLog : public TmcLogger<Output2Stream>
{
public:
    typedef Output2Stream output_policy;
};

// Macro to limit maxLevel from compiler side
#ifndef UBLOG_MAX_LEVEL
#define UBLOG_MAX_LEVEL logDEBUG5
#endif

//////////////////////////////////////////////////////////////////////////
// main macro for logging
//  example UBLOG(logINFO) << "this is a log entry";
//////////////////////////////////////////////////////////////////////////
#define UBLOG(level, logtext)                                                                       \
    if (level > UBLOG_MAX_LEVEL || level > TmcLog::reportingLevel() || !Output2Stream::getStream()) \
        ;                                                                                           \
    else                                                                                            \
        TmcLog().get(level) << logtext;

// why this macro
// e.g. UBLOG(logDEBUG2) << "I am sooo great " << username;
// so what does the praeprocessor make of this?:
//  if(level > UBLOG_MAX_LEVEL || level > TmcLog::reportingLevel() || !Output2Stream::getStream()) ;
//  else // Log().Get(logINFO) << "I am sooo great " << username;
// so: the principle of logging is based on: Creating a log object and writing it out when destroying it.
//     -> if the level to be logged is < than the level given in the UBLOG, you can save the
//        object creation -> optimal performance -> according to Petru Marginean (the author of the
//        original logger, the performance difference is hardly measurable when NOT logging!

//////////////////////////////////////////////////////////////////////////
// makro 2 for correct multi line output (expensive!!)
//  example1: UBLOGML(logINFO, "line1"<<endl<<"line2"<<endl<<"line3" )
//  example2: UBLOGML(logINFO, "line1\nline2\nendl\nline3" )
//////////////////////////////////////////////////////////////////////////
#define UBLOGML(level, multiline)                                                                   \
    if (level > UBLOG_MAX_LEVEL || level > TmcLog::reportingLevel() || !Output2Stream::getStream()) \
        ;                                                                                           \
    else                                                                                            \
    {                                                                                               \
        std::ostringstream output;                                                                  \
        output << multiline;                                                                        \
        std::istringstream input(output.str());                                                     \
        while (!input.eof())                                                                        \
        {                                                                                           \
            std::string dummy;                                                                      \
            getline(input, dummy, '\n');                                                            \
            TmcLog().get(level) << dummy;                                                           \
        }                                                                                           \
    }
//////////////////////////////////////////////////////////////////////////
// macro3, in case of screen output
//    -> it is written to the logfile as well as to the stream
//  if reporting level and level match
// example1: UBLOG2ML(logINFO, std::cout,  "line1"<<endl<<"line2"<<endl<<"line3" )
// example2: UBLOG2ML(logINFO, std::cout,  "line1\nline2\nendl\nline3" )
//////////////////////////////////////////////////////////////////////////
#define UBLOG2(level, stream, text)                                                                 \
    if (level > UBLOG_MAX_LEVEL || level > TmcLog::reportingLevel() || !Output2Stream::getStream()) \
        ;                                                                                           \
    else                                                                                            \
    {                                                                                               \
        stream << text << std::endl;                                                                \
        TmcLog().get(level) << text;                                                                \
    }

//////////////////////////////////////////////////////////////////////////
// macro4, wie 3 just with multiline
// example: UBLOG2(logINFO, std::cout,  "test" )
//////////////////////////////////////////////////////////////////////////
#define UBLOG2ML(level, stream, multiline)                                                          \
    if (level > UBLOG_MAX_LEVEL || level > TmcLog::reportingLevel() || !Output2Stream::getStream()) \
        ;                                                                                           \
    else                                                                                            \
    {                                                                                               \
        stream << multiline << std::endl;                                                           \
        std::ostringstream output;                                                                  \
        output << multiline;                                                                        \
        std::istringstream input(output.str());                                                     \
        while (!input.eof())                                                                        \
        {                                                                                           \
            std::string dummy;                                                                      \
            getline(input, dummy, '\n');                                                            \
            TmcLog().get(level) << dummy;                                                           \
        }                                                                                           \
    }

//////////////////////////////////////////////////////////////////////////
// example 2
//////////////////////////////////////////////////////////////////////////
// try
// {
//    TmcLog::reportingLevel() = TmcLog::logLevelFromString("DEBUG3");
//    //TmcLog::output_policy::setStream(&std::cerr); //<- clog is stdandard
//    TmcLog::output_policy::setStream("c:/temp/out.txt");  //if openening is not possible -> error message-> Log goes to cerr
//
//    int count = 3;
//    UBLOG(logINFO, "A loop with " << count << " iterations");
//    for (int i = 0; i != count; ++i)
//    {
//        UBLOG(logERROR , "error  - the counter i = " << i );
//        UBLOG(logDEBUG1, "debug1 - the counter i = " << i );
//        UBLOG(logDEBUG2, "debug2 - the counter i = " << i );
//        UBLOG(logDEBUG3, "debug3 - the counter i = " << i );
//        //fuer MultiLine Eintraege: --> koerrekte formatierung im logfile
//        UBLOGML(logDEBUG3, "debug3 - the counter i = "<<endl<<" 2 zeile "<< "3. Zeile" << i);
//        UBLOGML(logDEBUG3, "debug3 - the counter i = "<<endl<<" 2 zeile "<< "3. Zeile" << i);
//        UBLOG2ML(logDEBUG3,std:cout,"debug3 - the counter i = "<<endl<<" 2 zeile "<< "3. Zeile" << i);
//    }
//    return 0;
// }
// catch(const std::exception& e)
// {
//    UBLOG(logERROR) << e.what();
// }

#endif // UBLOGGER_H
