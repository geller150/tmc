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
 Original Copyright (C) 2004-2011 Soeren Textor, Sebastian Geller (VirtualFluids, TU Braunschweig, Germany)
 Modifications Copyright (C) 2022-2023 Sebastian Geller

 Semantics and Namings are inspired by the java bauinformatik package (Markus Brueggemann, BTU Cottbus, Germany, 1997)

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
    static path map

\*---------------------------------------------------------------------------*/

#ifndef TMCSYSTEM_H
#define TMCSYSTEM_H

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <algorithm>
#include <typeinfo>
#include <cctype> //for toupper
#include <ctime>

#include <common/utilities/TmcException.h>
#include <common/utilities/TmcLogger.h>

#define TMC_BOOST = 1

#if defined(TMC_BOOST)
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include <boost/dll.hpp>
#include <boost/dll/runtime_symbol_info.hpp>
#include <boost/system/error_code.hpp>
#endif // TMC_BOOST

#if defined(_WIN32) || defined(_WIN64)
#define UBSYSTEM_WINDOWS
#include <process.h>
#include <io.h>
#include <direct.h>
#ifndef _WINSOCKAPI_ // else complains at #include "Windows.h" und ::Sleep()
#define _WINSOCKAPI_
#endif
#include <windows.h>
#elif defined(__APPLE__)
#define UBSYSTEM_APPLE
#include "dirent.h"
#include "sys/stat.h"
#include <sys/syscall.h>
#include <sys/stat.h>
#elif defined(__amd64) || defined(__amd64__) || defined(__unix__) || defined(__CYGWIN__)
#define UBSYSTEM_LINUX
#include "dirent.h"
#include "sys/stat.h"
#include <sys/syscall.h>
#include <sys/stat.h>
#include <unistd.h>
#else
#error "TmcSystem::UnknownMachine"
#endif

#if defined(min) || defined(max) // daruch kann man sich spaeter #undef min; #undef max erparen
#error add NOMINMAX to preprocessor defines
#endif

// allg.:
// const int * C1        -> C1 is variable pointer to a constant integer
// int const * C2        -> C2 is variable pointer to a constant integer (same as above)
// int * const C3        -> C3 is constant pointer to a variable integer
// int const * const C4  -> C4 is constant pointer to a constant integer

// DEFINE TO STRING
// e.g. #define FOO hallo
//      -> QUOTEME(FOO) == "hallo"
#define _QUOTEME(x) #x
#define QUOTEME(x) _QUOTEME(x)

//////////////////////////////////////////////////////////////////////////
// TmcSystem
//////////////////////////////////////////////////////////////////////////
namespace TmcSystem
{
    template <bool>
    struct ub_static_assert; // declaration (ub_xxx due to static_assert in C++0x will not be a keyword)
    template <>
    struct ub_static_assert<true>
    {
    }; // declaration + definition of specific  "true"
       // ub_static_assert<false> leads to compiler errors, as there is no implementation!  //UB_STATIC_ASSERT(false)

    /*==========================================================*/
    inline void sleepMs(const unsigned int &msec)
    {
#if defined UBSYSTEM_WINDOWS
        ::Sleep((msec == 0) ? 1 : msec); // +1 here causes a context switch if SleepMSec(0) is called
#elif defined(UBSYSTEM_LINUX) || defined(UBSYSTEM_APPLE)
        ::usleep(1000 * msec);
#else
#error "TmcSystem::sleepMSec - UnknownMachine"
#endif
    }
    /*==========================================================*/
    inline void sleepS(const unsigned int &sec)
    {
#if defined UBSYSTEM_WINDOWS
        ::Sleep((sec == 0) ? 1 : sec * 1000); // +1 here causes a context switch if sleepS(0) is called
#elif defined(UBSYSTEM_LINUX) || defined(UBSYSTEM_APPLE)
        ::sleep(sec);
#else
#error "TmcSystem::sleepS - UnknownMachine"
#endif
    }
    /*==========================================================*/
    // checks if the bits of bitmask are set in value
    template <typename T>
    inline bool bitCheck(const T &value, const T &bitmask)
    {
        return ((value & bitmask) == bitmask);
    }
    /*==========================================================*/
    // checks if the bits of bitmask are set in value
    template <typename T>
    inline void setBit(T &value, const T &bitmask)
    {
        value |= bitmask;
    }
    /*==========================================================*/
    template <typename T>
    inline void unsetBit(T &value, const T &bitmask)
    {
        value &= ~bitmask;
    }
    /*==========================================================*/
    // returns bitmask as string e.g. 0001 0100 1101
    template <typename T>
    inline std::string getBitString(const T &value)
    {
        std::stringstream text;
        for (int i = sizeof(value) * 8 - 1 /*8 bits per byte*/; i >= 0; i--)
        {
            text << (char)(((value >> i) & 1) + '0');
            if (i % 4 == 0 && i > 0)
                text << ' ';
        }
        return text.str();
    }
    /*==========================================================*/
    // converts string to type T
    //  usage: int x = stringTo<int>("123");
    template <typename T>
    inline T stringTo(const std::string &s)
    {
        std::istringstream iss(s);
        T x;
        iss >> x;
        if (!iss)
            throw TmcException(UB_EXARGS, " cannot convert \"" + s + "\" to type <" + static_cast<std::string>(typeid(x).name()) + ">");

        return x;
    }
    /*==========================================================*/
    // usage: string s = toString(x);
    template <typename T>
    inline std::string toString(const T &x, int precision = 15)
    {
        std::ostringstream oss;
        oss << std::setprecision(precision);
        oss << x;
        return oss.str();
    }
    /*==========================================================*/
    // e.g. str="iHcsnW" -> "IHCSNW"
    inline std::string toUpperString(const std::string &str)
    {
        std::string tmp(str);
        std::transform(tmp.begin(), tmp.end(), tmp.begin(), static_cast<int (*)(int)>(std::toupper));

        return tmp;
    }
    /*==========================================================*/
    // e.g. str="iHcsnW" -> "ihcsnw"
    inline std::string toLowerString(const std::string &str)
    {
        std::string tmp(str);
        std::transform(tmp.begin(), tmp.end(), tmp.begin(), static_cast<int (*)(int)>(std::tolower));

        return tmp;
    }
    /*==========================================================*/
    // usage: std::string s = replaceInString(str,"\\","/");
    //        std::string s = replaceInString(str,"ich","du");
    static std::string replaceInString(std::string original, const std::string &replace, const std::string &replaceWith)
    {
        size_t pos = 0;
        while ((pos = original.find(replace, pos)) != std::string::npos)
        {
            original.replace(pos, replace.size(), replaceWith);
            pos += replaceWith.size();
        }
        return original;
    }
    /*==========================================================*/
    // returns content of an enviroment variable
    inline std::string getEnv(const std::string &var)
    {
        char *str = getenv(var.c_str());
        if (str == NULL)
        {
            return std::string("");
        }

        return static_cast<std::string>(str);
    }
    /*==========================================================*/
    inline bool isDirectory(const std::string &dir, const unsigned &attemptions = 3)
    {
        if (dir.empty())
            throw TmcException(UB_EXARGS, "dir is empty");

        std::string path = TmcSystem::replaceInString(dir, "\\", "/");

#if defined UBSYSTEM_WINDOWS
#ifndef _UNICODE
        if (_access(path.c_str(), 0) == -1)
            return false;
#else
        if (_waccess(path.c_str(), 0) == -1)
            return false;
#endif
#elif defined(UBSYSTEM_LINUX) || defined(UBSYSTEM_APPLE)
        struct stat stFileInfo;
        if (stat(path.c_str(), &stFileInfo) != 0)
        {
            return false;
        }
#endif

        return true;
    }

    /*=====================================================*/
    inline std::string getApplicationRootFolder()
    {
        boost::system::error_code ec;
        boost::filesystem::path fullPath = boost::dll::this_line_location(ec);
        boost::filesystem::path absPath = fullPath.normalize();
        boost::filesystem::path pp = absPath.parent_path();

        // if env PATH is set, then binary can be called without absolute path or relative path, so need to detect for such a situation and treat it differently
        if (pp.string().empty())
        {
            fullPath = boost::dll::program_location(ec);
            absPath = fullPath.normalize();
            pp = absPath.parent_path();
        }
        boost::filesystem::path installationPath = pp.parent_path();

        // std::cout << " full path : " << fullPath << std::endl;
        // std::cout << " abs path : " << absPath << std::endl;
        // std::cout << " parent path : " << pp << std::endl;
        // std::cout << " installation path: " << installationPath << std::endl;

        return installationPath.string();
    }
/*==========================================================*/
// usage:  makeDirectory("c:/temp");
//         makeDirectory("c:/temp/");
// return: true  -> successful
//         false -> failed
#if defined(TMC_BOOST)
    static boost::mutex mtx_makeDirectory;
#endif
    inline bool makeDirectory(const std::string &dir, const unsigned &attemptions = 3)
    {
        UBLOG(logDEBUG5, "TmcSystem::makeDirectory - start, dir=" << dir << " #attemptions=" << attemptions);

        if (dir.empty())
            throw TmcException(UB_EXARGS, "dir is empty");
        std::string path = TmcSystem::replaceInString(dir, "\\", "/");

        bool dirCreated = true;

#ifdef TMC_BOOST
        dirCreated = boost::filesystem::exists(dir);
        if (!dirCreated)
            dirCreated = boost::filesystem::create_directories(dir);
#elif defined UBSYSTEM_WINDOWS
        path = TmcSystem::replaceInString(dir, "//", "\\\\");
        if (path[path.size() - 1] != '/')
            path += "/";
        size_t pos = 0;
        // check for networkdrive
        if (path.size() > 1 && path[0] == '\\' && path[1] == '\\')
        {
            pos = path.find("/", pos); //_access-check always fails check with network drives
            pos = path.find("/", pos); // einfach netzlaufwerksnamen überspringen
        }

        while ((pos = path.find("/", pos + 1)) != std::string::npos)
        {
            std::string tmpdir = path.substr(0, pos);
#if defined(TMC_BOOST)
            boost::mutex::scoped_lock lock(mtx_makeDirectory);
#endif
            if (
#ifndef _UNICODE
                _access(tmpdir.c_str(), 0) == -1 && _mkdir(tmpdir.c_str()) == -1
#else
                _waccess(tmpdir.c_str(), 0) == -1 && _wmkdir(tmpdir.c_str()) == -1
#endif
            )
            {
                UBLOG(logDEBUG5, "TmcSystem::makeDirectory-  dir=\"" << tmpdir << "\" doesn't exist or makedir failed");
                dirCreated = false;
                break;
            }
        }
#elif defined(UBSYSTEM_LINUX) || defined(UBSYSTEM_APPLE)
        std::string command = "mkdir -p \"" + path + "\"";
        {
#if defined(TMC_BOOST)
            boost::mutex::scoped_lock lock(mtx_makeDirectory);
#endif
            if (system(command.c_str()) != 0)
            {
                UBLOG(logDEBUG5, "TmcSystem::makeDirectory-  dir=\"" << path << "\" doesn't exit or makedir failed");
                dirCreated = false;
            }
        }
#else
#error "TmcSystem::makeDirectory - UnknownMachine"
#endif

        if (!dirCreated && attemptions > 1)
        {
            UBLOG(logDEBUG5, "TmcSystem::makeDirectory - internal call of TmcSystem::makeDirectory");
            TmcSystem::sleepMs(500);
            dirCreated = TmcSystem::makeDirectory(path, attemptions - 1);
        }

        UBLOG(logDEBUG5, "TmcSystem::makeDirectory - end (success=" << dirCreated << ", attemptions = " << attemptions << ")");
        return dirCreated;
    }
    /*==========================================================*/
#if defined(TMC_BOOST)
    static boost::mutex mtx_removeDirectory;
#endif
    inline int removeDirectory(const std::string &dir)
    {
#if defined(TMC_BOOST)
        boost::mutex::scoped_lock lock(mtx_removeDirectory);
#endif
        std::string command = "rmdir \"" + dir + "\"";
        return std::system(command.c_str());
    }
    /*==========================================================*/
    // usage  : getPathFromString("c:/temp/foo.txt");
    // returns: "c:/temp"
    // usage  : getPathFromString("c:\\temp\\foo.txt");
    // returns: "c:/temp"
    // usage  : getPathFromString("foo.txt");
    // returns: ""
    inline std::string getPathFromString(const std::string &fileStringWithPath)
    {
        std::string tmp = TmcSystem::replaceInString(fileStringWithPath, "\\", "/");
        std::size_t last = tmp.rfind("/");
        if (last != std::string::npos)
            tmp.resize(last);
        else
            tmp = "";
        return tmp;
    }
    /*==========================================================*/
    // usage  : getFilenameFromString("c:/temp/foo.txt");
    // returns: "foo.txt"
    // usage  : getFilenameFromString("c:/temp/foo.txt",false);
    // returns: "foo"
    // usage  : getFilenameFromString("c:/temp/");
    // returns: ""
    inline std::string getFilenameFromString(const std::string &fileStringWithPath, bool withExtension = true)
    {
        std::string tmp = TmcSystem::replaceInString(fileStringWithPath, "\\", "/");

        // remove path
        std::size_t last = tmp.rfind("/");
        if (last != std::string::npos && (last + 1) < tmp.size())
            tmp.erase(0, last + 1);

        // remove extension
        if (!withExtension)
        {
            last = tmp.rfind(".");
            if (last != std::string::npos)
                tmp.erase(last);
        }

        return tmp;
    }
    /*==========================================================*/
    inline int getProcessID()
    {
#if defined UBSYSTEM_WINDOWS
        return _getpid();
#elif defined(UBSYSTEM_LINUX) || defined(UBSYSTEM_APPLE)
        return getpid();
#else
#error "int TmcSystem::getProcessID() - UnknownMachine"
#endif
    }
    /*==========================================================*/
    inline unsigned long getCurrentThreadID()
    {
#if defined UBSYSTEM_WINDOWS
        return (unsigned long)GetCurrentThreadId();
#elif defined(UBSYSTEM_LINUX) || defined(UBSYSTEM_APPLE)
        return (unsigned long)syscall(SYS_gettid);
#else
#error "unsigned long TmcSystem::getCurrentThreadID() - UnknownMachine"
#endif
    }
    /*==========================================================*/
    inline bool isBigEndian()
    {
        short word = 0x4321;
        if ((*(char *)&word) != 0x21)
            return true;
        else
            return false;
    }
    /*==========================================================*/
    inline bool isLittleEndian()
    {
        return !isBigEndian();
    }
    /*==========================================================*/
    inline std::string getTimeStamp()
    {
        time_t t = time(NULL);
        tm *localTime = localtime(&t);

        std::stringstream tmp;
        tmp.fill('0');

        tmp << localTime->tm_year + 1900
            << "." << std::setw(2) << localTime->tm_mon + 1
            << "." << std::setw(2) << localTime->tm_mday
            << "@" << std::setw(2) << localTime->tm_hour
            << "." << std::setw(2) << localTime->tm_min
            << "." << std::setw(2) << localTime->tm_sec;

        return tmp.str();
    }
    /*==========================================================*/
    // swap Byte Order
    // usage: int test = 8;
    //        swapByteOrder((unsigned char*)& test, sizeof(int))
    //#define ByteSwap5(x) ByteSwap((unsigned char *) &x,sizeof(x))
    inline void swapByteOrder(unsigned char *toSwap, int length)
    {
        register int i = 0;
        register int j = length - 1;
        while (i < j)
        {
            std::swap(toSwap[i], toSwap[j]);
            i++, j--;
        }
    }
    //////////////////////////////////////////////////////////////////////////
    // generic IfThenElse - start
    //////////////////////////////////////////////////////////////////////////
    // primary template: yield second or third argument depending on first argument
    template <bool C, typename Ta, typename Tb>
    class IfThenElse;

    // partial specialization: true yields second argument
    template <typename Ta, typename Tb>
    class IfThenElse<true, Ta, Tb>
    {
    public:
        typedef Ta ResultT;
    };

    // partial specialization: false yields third argument
    template <typename Ta, typename Tb>
    class IfThenElse<false, Ta, Tb>
    {
    public:
        typedef Tb ResultT;
    };
    //////////////////////////////////////////////////////////////////////////
    // generic IfThenElse - end
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    // help struct for overloading methods in template classes for specific types
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct type2type
    {
        typedef T type;
    };

    //////////////////////////////////////////////////////////////////////////
    // pair selector
    //////////////////////////////////////////////////////////////////////////
    template <typename Pair>
    struct select1st
    {
        typedef Pair argument_type;
        typedef typename Pair::first_type result_type;

        const result_type &operator()(const argument_type &p) const
        {
            return p.first;
        }
    };

    template <typename Pair>
    struct select2nd
    {
        typedef Pair argument_type;
        typedef typename Pair::second_type result_type;

        const result_type &operator()(const argument_type &p) const
        {
            return p.second;
        }
    };

};

#define UB_STATIC_ASSERT(expr) static_cast<void>(sizeof(TmcSystem::ub_static_assert<expr>));
    // to check static expressions during compile time
    //--> they must be evaluateable during compilation !!!!
    // apply e.g. bei TmcMath::getNegativeInfinity<double>();
    //
    // the reason for the macro is that is easier to apply else it would look like:
    //      TmcSystem::ub_static_assert< aaa == 1 > test();
    //     da ist  TMC_STATIC_ASSERT(aaa == 1); schoener
    //
    // to avoid this this it will be used static_cast<void>(sizeof(...) )
    // Code-Snippet:
    //  struct Test { static const bool m_const_bool = true; bool m_bool; };
    //  int main() {
    //   UB_STATIC_ASSERT( Test::m_const_bool == true );
    //   --> okay, assert bestanden
    //   UB_STATIC_ASSERT( Test::m_const_bool == false); //:
    //   --> assert nicht bestanden z.B. error C2027: use of undefined type 'TmcSystem::ub_static_assert<__formal> with __formal = false --> funzt nicht. fehler im code
    //   UB_STATIC_ASSERT( Test::m_bool == true );
    //   --> nicht erlaubt, da m_bool nicht statisch und nicht const ist.
    // }

#endif // TMCSYSTEM_H
