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
 Original Copyright (C) 2022-2023 Sebastian Geller

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
     execuatable - example simple mass oscillator

\*---------------------------------------------------------------------------*/

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#include <QApplication>
#include <QMainWindow>

#include <common/utilities/TmcFileOutputASCII.h>
#include <numerics/algebra/LaVector.h>
#include <numerics/algebra/LaScalar.h>
#include <numerics/algebra/LaSquareMatrix.h>
#include <numerics/algebra/LaLinearEquation.h>
#include <numerics/structuralsolver/massOscillator/views/TmcMassOscillatorWidget.h>

#include <views/plot2d/TmcPlotWidget.h>

int main(int argc, char **argv)
{
    try
    {
        std::cout << "/*===================================================*/" << std::endl;
        std::cout << "/*======           Mass Oscillator             ======*/" << std::endl;
        std::cout << "/*===================================================*/" << std::endl;
        QApplication app(argc, argv);
        app.setOrganizationName("TMC");
        app.setApplicationName("Plot");

        QMainWindow mw;
        TmcPlotWidget *pw = new TmcPlotWidget(&mw);
        TmcMassOscillatorWidget *mow = new TmcMassOscillatorWidget(pw);
        pw->addPluginWidget(mow);
        mw.setCentralWidget(pw);
        mw.resize(800, 600);

        mow->compute(true);

        mw.show();
        return app.exec();
    }
    catch (std::string &s)
    {
        std::cout << s << std::endl;
    }
    catch (std::string *s)
    {
        std::cout << *s << std::endl;
    }
    catch (...)
    {
        std::cout << "CRASHED for some unknown reason !" << std::endl;
    };
    return 0;
}
