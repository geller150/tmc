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

#ifndef TMCFILEINPUTBINARY_H
#define TMCFILEINPUTBINARY_H

#include <fstream>
#include <iostream>
#include <string>

#include <cstdint>

#include "TmcException.h"
#include "TmcFileInput.h"

#include <TmcMacroFile.h>

class TMC_DLL_EXPORT TmcFileInputBinary : public TmcFileInput
{
public:
    TmcFileInputBinary() : TmcFileInput() {}
    TmcFileInputBinary(std::string filename);

    bool open(std::string filename);

    bool skipLine();
    void readLine();
    std::string readStringLine();
    std::size_t readSize_t();
    int readInteger();
    uint16_t readUnsignedInteger16();
    long readLong();
    double readDouble();
    float readFloat();
    bool readBool();
    char readChar();
    std::string readString();
    std::string readLineTill(char stop);
    std::string parseString();

    bool containsString(const std::string &var);
    void setPosAfterLineWithString(const std::string &var);
    int readIntegerAfterString(const std::string &var);
    double readDoubleAfterString(const std::string &var);
    bool readBoolAfterString(const std::string &var);
    std::string readStringAfterString(const std::string &var);

    FILETYPE getFileType() { return BINARY; }

    template <typename T>
    friend inline TmcFileInputBinary &operator>>(TmcFileInputBinary &file, T &data)
    {
        file.infile.read((char *)&data, sizeof(T));
        return file;
    }
};

#endif
