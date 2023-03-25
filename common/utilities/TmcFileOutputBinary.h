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

#ifndef TMCFILEOUTPUTBINARY_H
#define TMCFILEOUTPUTBINARY_H

#include <iomanip>
#include <fstream>
#include <iostream>

#include "TmcException.h"
#include "TmcFileOutput.h"

#include <TmcMacroFile.h>

class TMC_DLL_EXPORT TmcFileOutputBinary : public TmcFileOutput
{
public:
    TmcFileOutputBinary() : TmcFileOutput() {}
    TmcFileOutputBinary(const std::string &filename, const bool &createPath = true);
    TmcFileOutputBinary(const std::string &filename, TmcFileOutput::CREATEOPTION opt, const bool &createPath);

    bool open(const std::string &filename, TmcFileOutput::CREATEOPTION opt = OUTFILE);

    void writeInteger(const int &value, const int &width = 0);
    void writeLong(const long &value, const int &width = 0);
    void writeDouble(const double &value, const int &width = 0);
    void writeFloat(const float &value, const int &width = 0);
    void writeBool(const bool &value, const int &width = 0);
    void writeChar(const char &value, const int &width = 0);
    void writeSize_t(const std::size_t &value, const int &width = 0);
    void writeString(const std::string &value, const int &width = 0);
    void writeStringOnly(const std::string &value);
    void writeLine(const std::string &value, const int &width = 0);
    void writeLine();
    void writeCommentLine(const std::string &line);
    void writeCommentLine(char indicator, const std::string &line);
    void writeCopyOfFile(const std::string &filename);

    void setPrecision(const int &precision);
    int getPrecision();

    FILETYPE getFileType() { return BINARY; }

    template <typename T>
    friend inline TmcFileOutputBinary &operator<<(TmcFileOutputBinary &file, const T &data)
    {
        file.outfile.write((char *)&data, sizeof(T));
        return file;
    }
};

#endif
