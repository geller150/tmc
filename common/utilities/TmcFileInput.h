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
    the base for file input

\*---------------------------------------------------------------------------*/

#ifndef TMCFILEINPUT_H
#define TMCFILEINPUT_H

#include <fstream>
#include <iostream>
#include <string>

#include <cstdlib> //atoi
#include <cstring> //strstr

#include "TmcException.h"

class TmcFileInput
{
public:
    enum FILETYPE
    {
        ASCII,
        BINARY
    };

public:
    TmcFileInput() : filename(""), commentindicator('C') {}
    virtual ~TmcFileInput() { infile.close(); }

    virtual bool operator!() { return !(infile); }
    virtual bool isOpen() { return !(!(infile)); }

    virtual bool open(std::string filename) = 0;
    virtual void close() { infile.close(); }
    virtual int eof() { return infile.eof(); }
    std::fstream::pos_type tellg() { return infile.tellg(); }
    void seekg(std::fstream::off_type offset, std::ios::seekdir origin = std::ios::beg) { infile.seekg(offset, origin); }

    virtual bool skipLine() = 0; // Springt zur naechsten Zeile
    virtual void readLine() = 0;
    virtual std::string readStringLine() = 0;
    virtual int readInteger() = 0; // Liest einen int-Wert ein
    virtual long readLong() = 0;   // Liest einen long-Wert ein
    virtual std::size_t readSize_t() = 0;
    virtual double readDouble() = 0;                 // Liest einen double-Wert ein
    virtual float readFloat() = 0;                   // Liest einen float-Wert ein
    virtual bool readBool() = 0;                     // Liest einen bool-Wert ein
    virtual char readChar() = 0;                     // Liest einen char-Wert ein
    virtual std::string readString() = 0;            // Liest ein Wort ein
    virtual std::string readLineTill(char stop) = 0; // Liest gesamte Zeile ein bis zu einem bestimmten Zeichen
    virtual std::string parseString() = 0;           // Liest

    virtual void setCommentIndicator(char commentindicator) { this->commentindicator = commentindicator; }
    virtual char getCommentIndicator() { return this->commentindicator; }

    virtual bool containsString(const std::string &var) = 0;
    virtual void setPosAfterLineWithString(const std::string &var) = 0;
    virtual int readIntegerAfterString(const std::string &var) = 0;
    virtual double readDoubleAfterString(const std::string &var) = 0;
    virtual bool readBoolAfterString(const std::string &var) = 0;
    virtual std::string readStringAfterString(const std::string &var) = 0;

    virtual std::string getFileName() { return this->filename; }

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

    // returns "ASCII", "BINARY"
    virtual FILETYPE getFileType() = 0;

protected:
    std::ifstream infile;
    std::string filename;
    char commentindicator;
};

#endif // TMCFILEINPUT_H
