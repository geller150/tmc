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
 Copyright (C) 2022-2023 Sebastian Geller

 This software is distributed WITHOUT ANY WARRANTY.

 License

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
    mass oscillator plugin widget

\*---------------------------------------------------------------------------*/

#ifndef TmcMassOscillatorWidget_H
#define TmcMassOscillatorWidget_H

#include <vector>
#include <tuple>

#include <QDockWidget>
#include <QScopedPointer>

#include <TmcMacroFile.h>

#include <vtkAutoInit.h>

class TmcPlotWidget;

struct TmcMassOscillatorWidgetPrivate;

class TMC_DLL_EXPORT TmcMassOscillatorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TmcMassOscillatorWidget(TmcPlotWidget *plotWidget, QWidget *parent = 0);
    virtual ~TmcMassOscillatorWidget();

    void setInitialOscillatorDefaultValues();
    void setValuesToMassOscillator();
    void compute(bool standard);

protected slots:
    void on_pushButtonInitialValues_clicked();
    void on_pushButtonComputeStandard_clicked();
    void on_pushButtonComputeNew_clicked();

private:
    Q_DISABLE_COPY(TmcMassOscillatorWidget)

    QScopedPointer<TmcMassOscillatorWidgetPrivate> d_ptr;
};

#endif //
