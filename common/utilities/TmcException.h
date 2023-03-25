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
 Original Copyright (C) 2004-2011 Soeren Textor (VirtualFluids, TU Braunschweig, Germany)
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
    the base for exception handling.

\*---------------------------------------------------------------------------*/

#ifndef TMCEXCEPTION_H
#define TMCEXCEPTION_H

#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

#include "./TmcTuple.h"

/*
usage: throw TmcException("error message");
       throw TmcException(__FILE__, __LINE__,"error message");
       throw TmcException(__FILE__, __LINE__,UB_FUNCTION,"error message");
       throw TmcException(UB_EXARGS,"error"); //same as above
*/

// Macro UB_FUNCTION: figures out the method/function name (platform dependant)
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600))
#define UB_FUNCTION __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define UB_FUNCTION __PRETTY_FUNCTION__
#elif defined(__FUNCSIG__)
#define UB_FUNCTION __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define UB_FUNCTION __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define UB_FUNCTION __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define UB_FUNCTION __func__
#else
#define UB_FUNCTION "(unknown)"
#endif

// Helper Marco
#ifndef SWIG
#define UB_EXARGS __FILE__, __LINE__, UB_FUNCTION
#endif
class TmcException : public std::runtime_error
{
public:
    typedef TmcTuple<std::string, int, std::string, std::string> ExceptionData;

public:
    //////////////////////////////////////////////////////////////////////////
    // constructors
    TmcException()
        : std::runtime_error("")
    {
    }
    /*==========================================================*/
    TmcException(const std::string &str)
        : std::runtime_error("")
    {
        this->addInfo(str);
    }
    /*==========================================================*/
    TmcException(const std::string &file, const int &line, const std::string &err_str)
        : std::runtime_error("")
    {
        this->addInfo(file, line, "unknown", err_str);
    }
    /*==========================================================*/
    // TmcException(const char* file, const int& line, const char* function, const std::string& err_str)
    TmcException(const std::string &file, const int &line, const std::string &function, const std::string &err_str)
        : std::runtime_error("")
    {
        this->addInfo(file, line, function, err_str);
    }
    //////////////////////////////////////////////////////////////////////////
    // destructor
    virtual ~TmcException() throw() {}
    //////////////////////////////////////////////////////////////////////////
    // virtual public methods
    // returns  exception-string
    virtual const char *what() const throw()
    {
        exceptionString = this->toString();
        return exceptionString.c_str(); // ansonsten ist das Verhalten anschlie�end undefiniert!
    }
    /*==========================================================*/
    virtual void addInfo(const std::string &err_str)
    {
        exceptionData.push_back(makeTmcTuple((std::string) "-", 0, (std::string) "unknown", err_str));
    }
    /*==========================================================*/
    // add exception
    virtual void addInfo(const std::string &file, const int &line, const std::string &function, const std::string &err_str)
    {
        exceptionData.push_back(makeTmcTuple(file, line, function, err_str));
    }
    /*==========================================================*/
    // returns exception-string with all calles exceptions
    virtual const std::vector<std::string> getInfo() const
    {
        std::vector<std::string> tmp;
        for (std::size_t i = 0; i < exceptionData.size(); i++)
        {
            std::stringstream str;
            str << val<1>(exceptionData[i]) << ", "
                << val<2>(exceptionData[i]) << ", "
                << val<3>(exceptionData[i]) << ", "
                << val<4>(exceptionData[i]);
            tmp.push_back(str.str());
        }
        return tmp;
    }
    /*==========================================================*/
    // returns exception-string with all calles exceptions and detailes informations
    virtual std::string toString() const
    {
        std::stringstream str("TmcExeption");

        for (std::size_t i = 0; i < exceptionData.size(); i++)
            str << (std::string) "caller[" << i << "]\n"
                << "  - file:     " << val<1>(exceptionData[i]) << "\n"
                << "  - line:     " << val<2>(exceptionData[i]) << "\n"
                << "  - function: " << val<3>(exceptionData[i]) << "\n"
                << "  - what:     " << val<4>(exceptionData[i]) << std::endl;

        return str.str();
    }

protected:
    //////////////////////////////////////////////////////////////////////////
    // protected member
    std::vector<ExceptionData> exceptionData;
    mutable std::string exceptionString;
};

// overlading operator <<
inline std::ostream &operator<<(std::ostream &os, const TmcException &e)
{
    return os << e.toString();
}

#endif // TMCEXCEPTION_H
