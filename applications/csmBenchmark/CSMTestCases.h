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
 Original Copyright (C) 2003-2011 Sebastian Geller (VirtualFluids, TU Braunschweig, Germany)
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
     CSM benchmark test cases

\*---------------------------------------------------------------------------*/

#ifndef CSMTESTCASES_H
#define CSMTESTCASES_H

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#include <common/utilities/TmcFileOutputASCII.h>

class LaSquareMatrix;
class LaVector;

class CSMTestCases
{
public:
    CSMTestCases(){};
    ~CSMTestCases(){};

    // CSM
    static void createCSM1BBenchmark();
    static void createCSM2BBenchmark();
    static void createCSM3BBenchmark(std::vector<std::tuple<double, double, double, double, double>> &data);

    static TmcFileOutputASCII *createFoldersAndGetLogfile(std::string pathname)
    {

        // cout<<"mkdir -p works not all times on Windows !!!"<<endl;
        std::stringstream ss;
        // ss<<"mkdir -p "<<pathname;
        // const char* pat =  ss.str().c_str();     system(pat);
        // ss.str("");
        // ss<<"mkdir "<<pathname<<"steps/";
        // pat =  ss.str().c_str();     system(pat);
        // ss.str("");
        // ss<<"mkdir "<<pathname<<"validate/";
        // pat =  ss.str().c_str();     system(pat);
        ss.str("");
        ss << pathname << "logfile.txt";
        TmcFileOutputASCII *logfile = new TmcFileOutputASCII(ss.str());
        logfile->setPrecision(15);
        return logfile;
    }

    /*============================================*/
};
/*============================================*/
#endif
