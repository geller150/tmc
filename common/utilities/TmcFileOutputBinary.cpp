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
    file output for binary files

\*---------------------------------------------------------------------------*/

#include "TmcFileOutputBinary.h"
#include <common/utilities/TmcSystem.h>
#include <cstring>

using namespace std;

/*==========================================================*/
TmcFileOutputBinary::TmcFileOutputBinary(const string &filename, const bool &createPath)
{
    this->filename = filename;
    this->commentindicator = 'C';

    outfile.open(filename.c_str(), ios::out | ios::binary);

    if (!outfile && createPath)
    {
        string path = TmcSystem::getPathFromString(filename);
        if (path.size() > 0)
        {
            outfile.clear();
            TmcSystem::makeDirectory(path);
            outfile.open(filename.c_str(), ios::out | ios::binary);
        }
    }

    if (!outfile)
        throw TmcException(UB_EXARGS, "couldn't open file:\n " + filename);
}
/*==========================================================*/
TmcFileOutputBinary::TmcFileOutputBinary(const string &filename, TmcFileOutput::CREATEOPTION opt, const bool &createPath)
{
    this->filename = filename;
    this->commentindicator = 'C';

    this->open(filename, opt);

    if (!this->open(filename, opt) && createPath)
    {
        string path = TmcSystem::getPathFromString(filename);
        if (path.size() > 0)
        {
            outfile.clear();
            TmcSystem::makeDirectory(path, 20);

            this->open(filename, opt);
        }
    }

    if (!outfile)
        throw TmcException(UB_EXARGS, "couldn't open file:\n " + filename);
}
/*==========================================================*/
bool TmcFileOutputBinary::open(const string &filename, TmcFileOutput::CREATEOPTION opt)
{
    outfile.close();
    outfile.clear();

    this->filename = filename;

    if (opt == TmcFileOutput::OUTFILE)
        outfile.open(this->filename.c_str(), ios::out | ios::binary);
    else if (opt == TmcFileOutput::APPENDFILE)
        outfile.open(this->filename.c_str(), ios::app | ios::binary);
    else if (opt == TmcFileOutput::INANDOUTFILE)
        throw TmcException(UB_EXARGS, "undefined CREATEOPTION - INANDOUTFILE not possible for BINARY files");
    else
        throw TmcException(UB_EXARGS, "undefined CREATEOPTION");

    return outfile.is_open();
}
/*==========================================================*/
void TmcFileOutputBinary::writeBool(const bool &value, const int &width)
{
    outfile.write((char *)&value, sizeof(bool));
}
/*==========================================================*/
void TmcFileOutputBinary::writeDouble(const double &value, const int &width)
{
    outfile.write((char *)&value, sizeof(double));
}
/*==========================================================*/
void TmcFileOutputBinary::writeFloat(const float &value, const int &width)
{
    outfile.write((char *)&value, sizeof(float));
}
/*==========================================================*/
void TmcFileOutputBinary::setPrecision(const int &precision)
{
    throw TmcException(UB_EXARGS, "no way");
}
/*==========================================================*/
int TmcFileOutputBinary::getPrecision()
{
    throw TmcException(UB_EXARGS, "no way");
}
/*==========================================================*/
void TmcFileOutputBinary::writeInteger(const int &value, const int &width)
{
    outfile.write((char *)&value, sizeof(value));
}
/*==========================================================*/
void TmcFileOutputBinary::writeLong(const long &value, const int &width)
{
    outfile.write((char *)&value, sizeof(value));
}
/*==========================================================*/
void TmcFileOutputBinary::writeSize_t(const std::size_t &value, const int &width)
{
    outfile.write((char *)&value, sizeof(value));
}
/*==========================================================*/
void TmcFileOutputBinary::writeChar(const char &value, const int &width)
{
    outfile.write((char *)&value, sizeof(value));
}
/*==========================================================*/
void TmcFileOutputBinary::writeString(const string &value, const int &width)
{
    char c = '\0';
    unsigned int length = (unsigned)value.length();

    unsigned pos;
    // whitespaces and tabs to overtake at beginning of string
    for (pos = 0; pos < length; pos++)
        if (value[pos] != ' ' && value[pos] != '\t')
            break;

    while (pos < length)
    {
        while (pos < length && value[pos] != ' ' && value[pos] != '\t' && value[pos] != '\0')
        {
            outfile.write((char *)&(value[pos++]), sizeof(char));
        }

        outfile.write(&c, sizeof(char));
        pos++;

        while (pos < length && (value[pos] == ' ' || value[pos] == '\t' || value[pos] == '\0'))
        {
            pos++;
        }
    }
}
/*==========================================================*/
void TmcFileOutputBinary::writeStringOnly(const string &value)
{
    TmcException(UB_EXARGS, "no way... causes to many errors");
}
/*==========================================================*/
void TmcFileOutputBinary::writeLine(const std::string &value, const int &width)
{
    this->writeString(value);
    char c = '\n';
    outfile.write(&c, sizeof(char));
}
/*==========================================================*/
void TmcFileOutputBinary::writeLine()
{
    char c = '\n';
    outfile.write(&c, sizeof(char));
}
/*==========================================================*/
void TmcFileOutputBinary::writeCommentLine(const string &line)
{
    try
    {
        this->writeCommentLine(this->commentindicator, line);
    }
    catch (...)
    {
        throw TmcException(UB_EXARGS, "unknown error");
    }
}
/*==========================================================*/
void TmcFileOutputBinary::writeCommentLine(char indicator, const string &line)
{
    string dummy = indicator + line;
    this->writeLine(dummy);
}
/*==========================================================*/
void TmcFileOutputBinary::writeCopyOfFile(const string &filename)
{
    ifstream infile(filename.c_str(), ios::in | ios::binary);
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
    catch (...)
    {
        throw TmcException(UB_EXARGS, "unknown error");
    }
}
