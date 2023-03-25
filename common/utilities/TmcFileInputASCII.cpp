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
    file input for ASCII files

\*---------------------------------------------------------------------------*/

#include "TmcFileInputASCII.h"
#include <cstring>

using namespace std;

TmcFileInputASCII::TmcFileInputASCII(string filename)
{
    this->filename = filename;
    this->commentindicator = 'C';

    infile.open(filename.c_str());
}
/*==========================================================*/
bool TmcFileInputASCII::open(string filename)
{
    infile.close();
    infile.clear(); // setzt flags zurueck

    this->filename = filename;
    infile.open(this->filename.c_str());

    return infile.is_open();
}
/*==========================================================*/
int TmcFileInputASCII::readInteger()
{
    int dummy;
    infile >> dummy;
    return dummy;
}
/*==========================================================*/
bool TmcFileInputASCII::parseInteger(int &value)
{
    int dummy;
    infile >> dummy;
    if (infile.fail())
    {
        value = -999;
        return false;
    }

    value = dummy;
    return true;
}
/*==========================================================*/
long TmcFileInputASCII::readLong()
{
    long dummy;
    infile >> dummy;
    return dummy;
}
/*==========================================================*/
string TmcFileInputASCII::getFileName()
{
    return this->filename;
}

/*==========================================================*/
bool TmcFileInputASCII::skipLine()
{
    string dummy;
    bool b = (bool)getline(infile, dummy);
    return b;
}
/*==========================================================*/
void TmcFileInputASCII::readLine()
{
    string dummy;
    getline(infile, dummy);
}
/*==========================================================*/
string TmcFileInputASCII::readStringLine()
{
    string dummy;
    getline(infile, dummy);
    return dummy;
}
/*==========================================================*/
string TmcFileInputASCII::readLineTill(char stop)
{
    string dummy;
    getline(infile, dummy, stop);
    return dummy;
}
/*==========================================================*/
string TmcFileInputASCII::parseString()
{
    string dummy;
    getline(infile, dummy, ' ');
    return dummy;
}
/*==========================================================*/
double TmcFileInputASCII::readDouble()
{
    double dummy;
    infile >> dummy;
    if (infile.fail())
        std::cout << "double failed" << std::endl;

    // if(infile.get() != '\n') std::cout<<"double failed - \n"<<std::endl;
    return dummy;
}
/*==========================================================*/
bool TmcFileInputASCII::parseDouble(double &value)
{
    double dummy;
    infile >> dummy;
    if (infile.fail())
    {
        value = -999.9;
        return false;
    }

    value = dummy;
    return true;
}
/*==========================================================*/
float TmcFileInputASCII::readFloat()
{
    float dummy;
    infile >> dummy;
    return dummy;
}
/*==========================================================*/
string TmcFileInputASCII::readString()
{
    string dummy;
    infile >> dummy;
    return dummy;
}
/*==========================================================*/
char TmcFileInputASCII::readChar()
{
    int dummy;
    infile >> dummy;
    return (char)dummy;
}
/*==========================================================*/
std::size_t TmcFileInputASCII::readSize_t()
{
    std::size_t dummy;
    infile >> dummy;
    return dummy;
}
/*==========================================================*/
void TmcFileInputASCII::setPosAfterLineWithString(const string &var)
{
    infile.seekg(0L, ios::beg); // set position pointer to beginning of file
    char line[512];
    do
    {
        infile.getline(line, 512);
        if (infile.eof())
            throw TmcException(UB_EXARGS, "error at reading in file \"" + filename + "\" -> string " + var + " wasn't found in " + this->filename);
    } while (strstr(line, var.c_str()) != line); // Ende Schleife, wenn varname ganz in zeile vorkommt
}
/*==========================================================*/
bool TmcFileInputASCII::containsString(const string &var)
{
    infile.clear(); // set the EOF-Status (not done via infile.seekg() !!!)

    infile.seekg(0L, ios::beg); // set position pointer to beginning of file
    char line[512];
    do
    {
        infile.getline(line, 512);
        if (infile.eof())
            return false;
    } while (strstr(line, var.c_str()) != line); // end of loop, if varname is in line

    return true;
}
/*==========================================================*/
int TmcFileInputASCII::readIntegerAfterString(const string &var)
// search in file for varname and gets the int-value behind
// e.g. timesteps 9
{
    infile.clear(); // set the EOF-Status (not done via infile.seekg() !!!)

    infile.seekg(0L, ios::beg); // set position pointer to beginning of file

    char line[512];

    do
    {
        infile.getline(line, 512);
        if (infile.eof())
            throw TmcException(UB_EXARGS, "error at reading in file \"" + filename + "\" -> " + var + " wasn't found in " + this->filename);
    } while (strstr(line, var.c_str()) != line);

    strcpy(line, (line + strlen(var.c_str()))); // shorten line for "varname"
    while ((line[0] == ' ') || (line[0] == '\t'))
        strcpy(line, (line + 1)); // remove Whitespaces

    return atoi(line); // convert to int
}
/*==========================================================*/
// search in file for varname and gets the double-value behind
// e.g. nue 9.5
double TmcFileInputASCII::readDoubleAfterString(const string &var)
{
    infile.clear();

    infile.seekg(0L, ios::beg);

    char line[512];

    do
    {
        infile.getline(line, 512);
        if (infile.eof())
            throw TmcException(UB_EXARGS, "error at reading in file \"" + filename + "\" -> " + var + " wasn't found in " + this->filename);
    } while (/*!strncmp(varname,line,sizeof(varname))==0*/ strstr(line, var.c_str()) != line);

    strcpy(line, (line + strlen(var.c_str())));
    while ((line[0] == ' ') || (line[0] == '\t'))
        strcpy(line, (line + 1));

    return (atof(line)); // convert to double
}
/*==========================================================*/
string TmcFileInputASCII::readStringAfterString(const string &var)
{
    infile.clear();

    infile.seekg(0L, ios::beg);

    char line[512];

    do
    {
        infile.getline(line, 512);
        if (infile.eof())
            throw TmcException(UB_EXARGS, "error at reading in file \"" + filename + "\" -> " + var + " wasn't found in " + this->filename);
    } while (strstr(line, var.c_str()) != line);

    strcpy(line, (line + strlen(var.c_str())));
    while ((line[0] == ' ') || (line[0] == '\t'))
        strcpy(line, (line + 1));

    char *p;
    p = strtok(line, " ");
    p = strtok(line, "\t");

    return static_cast<string>(p); // convert to string
}
/*==========================================================*/
bool TmcFileInputASCII::readBoolAfterString(const string &var)
{
    if (this->readStringAfterString(var) == "true")
        return true;
    else if (this->readStringAfterString(var) == "false")
        return false;
    else
        throw TmcException(UB_EXARGS, "error at reading in file \"" + filename + "\" -> expression after " + var + " is not equal to 'true' or 'false' in " + this->filename);
}
/*==========================================================*/
bool TmcFileInputASCII::readBool()
{
    string tmp = this->readString();
    if (tmp == "true")
        return true;
    else if (tmp == "false")
        return false;
    else
        throw TmcException(UB_EXARGS, "error at reading in file \"" + filename + "\" -> expression=\"" + tmp + "\" is not equal to 'true' or 'false' in " + this->filename);
}
/*==========================================================*/
int TmcFileInputASCII::getNumberOfLines()
{
    infile.clear();

    infile.seekg(0L, ios::beg);

    char line[512];
    int counter = 0;

    do
    {
        infile.getline(line, 512);
        counter++;
    } while (!infile.eof());

    counter -= 1;

    infile.clear();

    infile.seekg(0L, ios::beg);

    return counter;
}
