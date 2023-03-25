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
     executable for CSM benchmark

\*---------------------------------------------------------------------------*/

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <map>

#include <QApplication>
#include <QMainWindow>

#include <common/utilities/TmcException.h>
#include <applications/csmBenchmark/CSMTestCases.h>

#include <views/plot2d/TmcPlotWidget.h>

int main(int argc, char **argv)
{
    try
    {
        std::cout << "**** CSM 1B *****\n";
        CSMTestCases::createCSM1BBenchmark();
        std::cout << "**** CSM 2B *****\n";
        CSMTestCases::createCSM2BBenchmark();
        std::cout << "**** CSM 3B *****\n";
        std::vector<std::tuple<double, double, double, double, double>> data;
        CSMTestCases::createCSM3BBenchmark(data);

        QApplication app(argc, argv);
        app.setOrganizationName("TMC");
        app.setApplicationName("Plot");

        QMainWindow mw;
        TmcPlotWidget *pw = new TmcPlotWidget(&mw);
        mw.setCentralWidget(pw);
        mw.resize(800, 600);

        pw->updateData(data);

        mw.show();
        return app.exec();
    }
    catch (TmcException &s)
    {
        std::cout << s.toString() << std::endl;
    }
    catch (...)
    {
        std::cout << "Something went wrong!" << std::endl;
    };
    return 0;
}
