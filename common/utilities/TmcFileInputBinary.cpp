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
    file input for Binary files

\*---------------------------------------------------------------------------*/

#include "TmcFileInputBinary.h"
#include <cstring>

using namespace std;

/*==========================================================*/
TmcFileInputBinary::TmcFileInputBinary(string filename)
{
    this->filename = filename;
    infile.open(filename.c_str(), ios::in | ios::binary);
}
/*==========================================================*/
bool TmcFileInputBinary::open(string filename)
{
    infile.close();
    infile.clear();

    this->filename = filename;
    infile.open(this->filename.c_str(), ios::in | ios::binary);

    return infile.is_open();
}
/*==========================================================*/
int TmcFileInputBinary::readInteger()
{
    int dummy;
    infile.read((char *)&dummy, sizeof(int));
    return dummy;
}
/*==========================================================*/
uint16_t TmcFileInputBinary::readUnsignedInteger16()
{
    uint16_t dummy;
    infile.read((char *)&dummy, sizeof(uint16_t));
    return dummy;
}
/*==========================================================*/
long TmcFileInputBinary::readLong()
{
    long dummy;
    infile.read((char *)&dummy, sizeof(long));
    return dummy;
}
/*==========================================================*/
std::size_t TmcFileInputBinary::readSize_t()
{
    std::size_t dummy;
    infile.read((char *)&dummy, sizeof(std::size_t));
    return dummy;
}
/*==========================================================*/
double TmcFileInputBinary::readDouble()
{
    double dummy;
    infile.read((char *)&dummy, sizeof(double));
    return dummy;
}
/*==========================================================*/
float TmcFileInputBinary::readFloat()
{
    float dummy;
    infile.read((char *)&dummy, sizeof(float));
    return dummy;
}
/*==========================================================*/
char TmcFileInputBinary::readChar()
{
    char dummy;
    infile.read((char *)&dummy, sizeof(char));
    return dummy;
}
/*==========================================================*/
string TmcFileInputBinary::readString()
{
    char c;
    infile.read(&c, sizeof(char));
    while (c == ' ' || c == '\t')
        infile.read(&c, sizeof(char));

    string dummy;
    dummy += c;

    infile.read(&c, sizeof(char));
    while (c != '\0' && c != ' ' && c != '\t' && c != '\n')
    {
        dummy += c;
        infile.read(&c, sizeof(char));
    }
    return dummy;
}
/*==========================================================*/
bool TmcFileInputBinary::readBool()
{
    bool dummy;
    infile.read((char *)&dummy, sizeof(bool));
    return dummy;
}
/*==========================================================*/
bool TmcFileInputBinary::skipLine()
{
    char c;
    do
    {
        infile.read(&c, sizeof(char));
    } while (c != '\n');
    return true;
}
/*==========================================================*/
void TmcFileInputBinary::readLine()
{
    char c;
    infile.read(&c, sizeof(char));
    while (c != '\n')
        infile.read(&c, sizeof(char));
}
/*==========================================================*/
string TmcFileInputBinary::readStringLine()
{
    char c;
    string dummy;
    infile.read(&c, sizeof(char));
    while (c != '\n')
    {
        dummy += c;
        infile.read(&c, sizeof(char));
    }
    return dummy;
}
/*==========================================================*/
string TmcFileInputBinary::readLineTill(char stop)
{
    throw TmcException(UB_EXARGS, "method makes no sense for binary streams");
}
/*==========================================================*/
string TmcFileInputBinary::parseString()
{
    throw TmcException(UB_EXARGS, "method makes no sense for binary streams");
}
/*==========================================================*/
bool TmcFileInputBinary::containsString(const string &var)
{
    throw TmcException(UB_EXARGS, "method makes no sense for binary streams");
}
/*==========================================================*/
void TmcFileInputBinary::setPosAfterLineWithString(const string &var)
{
    throw TmcException(UB_EXARGS, "method makes no sense for binary streams");
}
/*==========================================================*/
int TmcFileInputBinary::readIntegerAfterString(const string &var)
{
    throw TmcException(UB_EXARGS, "method makes no sense for binary streams");
}
/*==========================================================*/
double TmcFileInputBinary::readDoubleAfterString(const string &var)
{
    throw TmcException(UB_EXARGS, "method makes no sense for binary streams");
}
/*==========================================================*/
string TmcFileInputBinary::readStringAfterString(const string &var)
{
    throw TmcException(UB_EXARGS, "method makes no sense for binary streams");
}
/*==========================================================*/
bool TmcFileInputBinary::readBoolAfterString(const string &var)
{
    throw TmcException(UB_EXARGS, "method makes no sense for binary streams");
}
