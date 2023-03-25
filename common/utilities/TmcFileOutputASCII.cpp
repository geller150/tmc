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
    file output for ASCII files

\*---------------------------------------------------------------------------*/

#include "TmcFileOutputASCII.h"
#include <common/utilities/TmcSystem.h>
#include <common/math/TmcMath.h>
#include <common/math/TmcEqual.h>
#include <common/math/TmcInfinity.h>
#include <cstring>

using namespace std;

TmcFileOutputASCII::TmcFileOutputASCII(const string &filename, const bool &createPath, const int &precision)
    : TmcFileOutput(filename)
{
    this->commentindicator = 'C';
    this->setPrecision(20);

    outfile.open(filename.c_str(), ios::out);

    if (!outfile && createPath)
    {
        outfile.clear();
        string path = TmcSystem::getPathFromString(filename);
        if (path.size() > 0)
        {
            TmcSystem::makeDirectory(path);
            outfile.open(filename.c_str(), ios::out);
        }
    }

    if (!outfile)
        throw TmcException(UB_EXARGS, "couldn't open file:\n " + filename);
}
/*==========================================================*/
TmcFileOutputASCII::TmcFileOutputASCII(const std::string &filename, CREATEOPTION opt, const bool &createPath, const int &precision)
    : TmcFileOutput(filename)
{
    this->commentindicator = 'C';
    this->setPrecision(precision);

    if (!this->open(filename, opt) && createPath)
    {
        outfile.clear();
        string path = TmcSystem::getPathFromString(filename);
        if (path.size() > 0)
            TmcSystem::makeDirectory(path);

        this->open(filename, opt);
    }

    if (!outfile)
        throw TmcException(UB_EXARGS, "couldn't open file:\n " + filename);
}
/*==========================================================*/
bool TmcFileOutputASCII::open(const std::string &filename, CREATEOPTION opt)
{
    outfile.close();
    outfile.clear();
    this->filename = filename;

    if (opt == TmcFileOutput::OUTFILE)
        outfile.open(this->filename.c_str(), ios::out);
    else if (opt == TmcFileOutput::INANDOUTFILE)
        outfile.open(this->filename.c_str(), ios::out | ios::in);
    else if (opt == TmcFileOutput::APPENDFILE)
        outfile.open(this->filename.c_str(), ios::app);
    else
        throw TmcException(UB_EXARGS, "undefined CREATEOPTION");

    return outfile.is_open();
}
/*==========================================================*/
void TmcFileOutputASCII::writeBool(const bool &value, const int &width)
{
    outfile.width(width);
    if (value)
        outfile << "true ";
    else
        outfile << "false ";
}
/*==========================================================*/
void TmcFileOutputASCII::writeDouble(const double &value, const int &width)
{
    outfile.width(width);
    // Problem: Tmc::inf rounded
    //          -> by reading the value might to big and it creates wrong output
    //          -> display max length
    if (TmcMath::equal(value, (double)Tmc::inf))
    {
        ios_base::fmtflags flags = outfile.flags();
        outfile << setprecision(std::numeric_limits<double>::digits10 + 2);
        outfile << value << " ";
        outfile.flags(flags);
        return;
    }
    outfile << value << " ";
}
/*==========================================================*/
void TmcFileOutputASCII::writeFloat(const float &value, const int &width)
{
    outfile.width(width);
    // Problem: Tmc::inf rounded
    //          -> by reading the value might to big and it creates wrong output
    //          -> display max length
    if (TmcMath::equal(value, (float)Tmc::inf))
    {
        ios_base::fmtflags flags = outfile.flags();
        outfile << setprecision(std::numeric_limits<float>::digits10 + 2);
        outfile << value << " ";
        outfile.flags(flags);
        return;
    }
    outfile << value << " ";
}
/*==========================================================*/
void TmcFileOutputASCII::setPrecision(const int &precision)
{
    outfile << setprecision(precision);
}
/*==========================================================*/
void TmcFileOutputASCII::writeInteger(const int &value, const int &width)
{
    outfile.width(width);
    outfile << value << " ";
}
/*==========================================================*/
void TmcFileOutputASCII::writeLong(const long &value, const int &width)
{
    outfile.width(width);
    outfile << value << " ";
}
/*==========================================================*/
void TmcFileOutputASCII::writeSize_t(const std::size_t &value, const int &width)
{
    outfile.width(width);
    outfile << value << " ";
}
/*==========================================================*/
void TmcFileOutputASCII::writeChar(const char &value, const int &width)
{
    outfile.width(width);
    outfile << (int)value << " ";
}
/*==========================================================*/
void TmcFileOutputASCII::writeString(const string &value, const int &width)
{
    outfile.width(width);
    outfile << value.c_str() << " ";
}
/*==========================================================*/
void TmcFileOutputASCII::writeStringOnly(const string &value)
{
    outfile << value.c_str();
}

/*==========================================================*/
void TmcFileOutputASCII::writeLine(const string &value, const int &width)
{
    outfile.width(width);
    outfile << value.c_str() << endl;
}
/*==========================================================*/
void TmcFileOutputASCII::writeLine()
{
    outfile << endl;
}
/*==========================================================*/
void TmcFileOutputASCII::writeCommentLine(const string &line)
{
    this->writeCommentLine(this->commentindicator, line);
}
/*==========================================================*/
void TmcFileOutputASCII::writeCommentLine(char indicator, const string &line)
{
    this->outfile << indicator << line << endl;
}
/*==========================================================*/
void TmcFileOutputASCII::writeCopyOfFile(const string &filename)
{
    ifstream infile(filename.c_str());
    if (!infile)
        throw TmcException(UB_EXARGS, "couldn't open file:\n " + filename);

    try
    {
        char c;
        while (infile.get(c))
        {
            outfile.put(c); //=out<<c;
        }
        outfile.flush();
        infile.close();
    }
    catch (std::exception &e)
    {
        throw TmcException(UB_EXARGS, "catched std::exception, error: " + (std::string)e.what());
    }
    catch (...)
    {
        throw TmcException(UB_EXARGS, "unknown error");
    }
}
