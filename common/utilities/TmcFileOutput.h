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
    the base for file output

\*---------------------------------------------------------------------------*/

#ifndef TMCFILEOUTPUT_H
#define TMCFILEOUTPUT_H

#include <iomanip>
#include <fstream>
#include <iostream>
#include <string>

#include "TmcException.h"

class TmcFileOutput
{
public:
    enum CREATEOPTION
    {
        OUTFILE = 0,
        INANDOUTFILE = 1,
        APPENDFILE = 2
    };
    enum FILETYPE
    {
        ASCII,
        BINARY
    };

public:
    TmcFileOutput() : filename(""), commentindicator('C') {}
    TmcFileOutput(const std::string &filename) : filename(filename), commentindicator('C') {}
    virtual ~TmcFileOutput()
    {
        outfile.flush();
        outfile.close();
    }

    virtual bool open(const std::string &filename, CREATEOPTION opt = OUTFILE) = 0;

    virtual bool operator!() { return !(outfile); }
    virtual bool isOpen() { return !(!(outfile)); }

    virtual void flush() { outfile.flush(); }
    virtual void close() { outfile.close(); }

    std::fstream::pos_type tellp() { return outfile.tellp(); }
    void seekp(std::fstream::off_type offset, std::ios::seekdir origin = std::ios::beg) { outfile.seekp(offset, origin); }

    virtual void writeInteger(const int &value, const int &width = 0) = 0;
    virtual void writeLong(const long &value, const int &width = 0) = 0;
    virtual void writeDouble(const double &value, const int &width = 0) = 0;
    virtual void writeFloat(const float &value, const int &width = 0) = 0;
    virtual void writeBool(const bool &value, const int &width = 0) = 0;
    virtual void writeSize_t(const std::size_t &value, const int &width = 0) = 0;
    virtual void writeChar(const char &value, const int &width = 0) = 0;
    virtual void writeString(const std::string &value, const int &width = 0) = 0;
    virtual void writeStringOnly(const std::string &value) = 0;
    virtual void writeLine(const std::string &value, const int &width = 0) = 0;
    virtual void writeLine() = 0;

    virtual void writeCommentLine(const std::string &line) = 0;
    virtual void writeCommentLine(char indicator, const std::string &line) = 0;
    virtual void writeCopyOfFile(const std::string &filename) = 0;

    virtual void setCommentIndicator(char commentindicator) { this->commentindicator = commentindicator; }

    virtual void setPrecision(const int &precision) = 0;
    virtual int getPrecision() = 0;

    // returns "ASCII", "BINARY"
    virtual FILETYPE getFileType() = 0;

    // returns file extension:
    // e.g. "./../test/ich.inp" -> "inp", "./../test/ich" -> ""
    virtual std::string getFileExtension()
    {
        std::size_t pos1 = filename.rfind("/");
        if (pos1 == std::string::npos)
            pos1 = 0;
        std::size_t pos2 = filename.rfind(".");
        if (pos2 != std::string::npos && pos2 > pos1)
            return filename.substr(pos2 + 1);

        return "";
    }

    virtual std::string getFileName() { return this->filename; }

protected:
    std::ofstream outfile;
    std::string filename;
    char commentindicator;
};

#endif // TMCFILEOUTPUT_H
