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
    plot widget mass oscillator plugin

\*---------------------------------------------------------------------------*/

#include "TmcMassOscillatorWidget.h"

#include <QDateTime>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QSplitter>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QGroupBox>
#include <QSettings>
#include <QMessageBox>
#include <QInputDialog>

#include <vtkTable.h>
#include <vtkNew.h>
#include <vtkContextView.h>
#include <vtkChartXY.h>
#include <vtkChartLegend.h>
#include <vtkPlot.h>
#include <vtkRenderer.h>
#include <vtkContextScene.h>
#include <vtkPen.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkAxis.h>
#include <vtkPNGWriter.h>
#include <vtkImageData.h>
#include <vtkDelimitedTextReader.h>
#include <vtkDelimitedTextWriter.h>
#include <vtkDoubleArray.h>
#include <vtkWindowToImageFilter.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <QVTKInteractor.h>
#include <QVTKRenderWidget.h>
#include <vtkMergeTables.h>
#include <vtkColorSeries.h>
#include <vtkNamedColors.h>

#include <cmath>
#include <cstring>
#include <sstream>
#include <iostream>
#include <tuple>

#include <common/math/TmcMath.h>
#include <common/utilities/TmcSystem.h>
#include <common/utilities/TmcTiming.h>

#include <numerics/structuralsolver/massoscillator/TmcMassOscillator.h>
#include <views/plot2d/TmcPlotWidget.h>

struct TmcMassOscillatorWidgetPrivate
{
    TmcMassOscillatorWidgetPrivate() {}

    QGridLayout *gridLayoutOscillator;
    QLabel *labelB;
    QLabel *labelM;
    QLabel *labelD;
    QLabel *labelK;
    QLabel *labelLoad;
    QLabel *labelAlpha;
    QLabel *labelBeta;
    QLabel *labelGamma;
    QLabel *labelTheta;
    QLabel *labelDeltaT;
    QLabel *labelTime;
    QLineEdit *textEditB;
    QLineEdit *textEditM;
    QLineEdit *textEditD;
    QLineEdit *textEditK;
    QLineEdit *textEditLoad;
    QLineEdit *textEditAlpha;
    QLineEdit *textEditBeta;
    QLineEdit *textEditGamma;
    QLineEdit *textEditTheta;
    QLineEdit *textEditDeltaT;
    QLineEdit *textEditTime;
    QPushButton *pushButtonInitialValues;
    QPushButton *pushButtonComputeStandard;
    QPushButton *pushButtonComputeNew;

    TmcMassOscillator *massOscillator;
    TmcPlotWidget *plotWidget;
};

TmcMassOscillatorWidget::TmcMassOscillatorWidget(TmcPlotWidget *plotWidget, QWidget *parent) : QWidget(parent),
                                                                                               d_ptr(new TmcMassOscillatorWidgetPrivate)
{
    // vtkObject::GlobalWarningDisplayOff();
    d_ptr->massOscillator = new TmcMassOscillator();
    d_ptr->plotWidget = plotWidget;

    d_ptr->gridLayoutOscillator = new QGridLayout();
    d_ptr->labelB = new QLabel(this);
    d_ptr->labelB->setText(QApplication::translate("TmcMassOscillatorWidget", "B", Q_NULLPTR));
    d_ptr->labelM = new QLabel(this);
    d_ptr->labelM->setText(QApplication::translate("TmcMassOscillatorWidget", "M", Q_NULLPTR));
    d_ptr->labelD = new QLabel(this);
    d_ptr->labelD->setText(QApplication::translate("TmcMassOscillatorWidget", "D", Q_NULLPTR));
    d_ptr->labelK = new QLabel(this);
    d_ptr->labelK->setText(QApplication::translate("TmcMassOscillatorWidget", "K", Q_NULLPTR));
    d_ptr->labelLoad = new QLabel(this);
    d_ptr->labelLoad->setText(QApplication::translate("TmcMassOscillatorWidget", "Load", Q_NULLPTR));
    d_ptr->labelAlpha = new QLabel(this);
    d_ptr->labelAlpha->setText(QApplication::translate("TmcMassOscillatorWidget", "alpha", Q_NULLPTR));
    d_ptr->labelBeta = new QLabel(this);
    d_ptr->labelBeta->setText(QApplication::translate("TmcMassOscillatorWidget", "beta", Q_NULLPTR));
    d_ptr->labelGamma = new QLabel(this);
    d_ptr->labelGamma->setText(QApplication::translate("TmcMassOscillatorWidget", "gamma", Q_NULLPTR));
    d_ptr->labelTheta = new QLabel(this);
    d_ptr->labelTheta->setText(QApplication::translate("TmcMassOscillatorWidget", "theta", Q_NULLPTR));

    d_ptr->labelDeltaT = new QLabel(this);
    d_ptr->labelDeltaT->setText(QApplication::translate("TmcMassOscillatorWidget", "delta T", Q_NULLPTR));

    d_ptr->labelTime = new QLabel(this);
    d_ptr->labelTime->setText(QApplication::translate("TmcMassOscillatorWidget", "Time", Q_NULLPTR));

    d_ptr->textEditB = new QLineEdit(this);
    d_ptr->textEditM = new QLineEdit(this);
    d_ptr->textEditD = new QLineEdit(this);
    d_ptr->textEditK = new QLineEdit(this);
    d_ptr->textEditLoad = new QLineEdit(this);
    d_ptr->textEditAlpha = new QLineEdit(this);
    d_ptr->textEditBeta = new QLineEdit(this);
    d_ptr->textEditGamma = new QLineEdit(this);
    d_ptr->textEditTheta = new QLineEdit(this);
    d_ptr->textEditDeltaT = new QLineEdit(this);
    d_ptr->textEditTime = new QLineEdit(this);
    
    this->setInitialOscillatorDefaultValues();
    
    d_ptr->pushButtonInitialValues = new QPushButton(this);
    d_ptr->pushButtonInitialValues->setText(QApplication::translate("TmcMassOscillatorWidget", "Initial Values", Q_NULLPTR));
    d_ptr->pushButtonComputeStandard = new QPushButton(this);
    d_ptr->pushButtonComputeStandard->setText(QApplication::translate("TmcMassOscillatorWidget", "Standard", Q_NULLPTR));

    d_ptr->pushButtonComputeNew = new QPushButton(this);
    d_ptr->pushButtonComputeNew->setText(QApplication::translate("TmcMassOscillatorWidget", "With Jerk", Q_NULLPTR));

    d_ptr->gridLayoutOscillator->addWidget(d_ptr->pushButtonInitialValues, 0, 0, 1, 2);
    d_ptr->gridLayoutOscillator->addWidget(d_ptr->labelB, 1, 0);
    d_ptr->gridLayoutOscillator->addWidget(d_ptr->textEditB, 1, 1);
    d_ptr->gridLayoutOscillator->addWidget(d_ptr->labelM, 2, 0);
    d_ptr->gridLayoutOscillator->addWidget(d_ptr->textEditM, 2, 1);
    d_ptr->gridLayoutOscillator->addWidget(d_ptr->labelD, 3, 0);
    d_ptr->gridLayoutOscillator->addWidget(d_ptr->textEditD, 3, 1);
    d_ptr->gridLayoutOscillator->addWidget(d_ptr->labelK, 4, 0);
    d_ptr->gridLayoutOscillator->addWidget(d_ptr->textEditK, 4, 1);
    d_ptr->gridLayoutOscillator->addWidget(d_ptr->labelLoad, 5, 0);
    d_ptr->gridLayoutOscillator->addWidget(d_ptr->textEditLoad, 5, 1);
    d_ptr->gridLayoutOscillator->addWidget(d_ptr->labelAlpha, 6, 0);
    d_ptr->gridLayoutOscillator->addWidget(d_ptr->textEditAlpha, 6, 1);
    d_ptr->gridLayoutOscillator->addWidget(d_ptr->labelBeta, 7, 0);
    d_ptr->gridLayoutOscillator->addWidget(d_ptr->textEditBeta, 7, 1);
    d_ptr->gridLayoutOscillator->addWidget(d_ptr->labelGamma, 8, 0);
    d_ptr->gridLayoutOscillator->addWidget(d_ptr->textEditGamma, 8, 1);
    d_ptr->gridLayoutOscillator->addWidget(d_ptr->labelTheta, 9, 0);
    d_ptr->gridLayoutOscillator->addWidget(d_ptr->textEditTheta, 9, 1);
    d_ptr->gridLayoutOscillator->addWidget(d_ptr->labelDeltaT, 10, 0);
    d_ptr->gridLayoutOscillator->addWidget(d_ptr->textEditDeltaT, 10, 1);
    d_ptr->gridLayoutOscillator->addWidget(d_ptr->labelTime, 11, 0);
    d_ptr->gridLayoutOscillator->addWidget(d_ptr->textEditTime, 11, 1);
    d_ptr->gridLayoutOscillator->addWidget(d_ptr->pushButtonComputeStandard, 12, 0, 1, 1);
    d_ptr->gridLayoutOscillator->addWidget(d_ptr->pushButtonComputeNew, 12, 1, 1, 1);

    this->setLayout(d_ptr->gridLayoutOscillator);

    QObject::connect(d_ptr->pushButtonInitialValues, SIGNAL(clicked()), this, SLOT(on_pushButtonInitialValues_clicked()));
    QObject::connect(d_ptr->pushButtonComputeNew, SIGNAL(clicked()), this, SLOT(on_pushButtonComputeNew_clicked()));
    QObject::connect(d_ptr->pushButtonComputeStandard, SIGNAL(clicked()), this, SLOT(on_pushButtonComputeStandard_clicked()));
}

TmcMassOscillatorWidget::~TmcMassOscillatorWidget()
{
}
/*===========================================================*/
void TmcMassOscillatorWidget::setInitialOscillatorDefaultValues()
{
    d_ptr->textEditB->setText("0.0");
    d_ptr->textEditM->setText("100000.0");
    d_ptr->textEditD->setText("100000.0");
    d_ptr->textEditK->setText("2000000.0");
    d_ptr->textEditLoad->setText("200000.0");
    d_ptr->textEditAlpha->setText("0.5");
    d_ptr->textEditBeta->setText("0.25");
    d_ptr->textEditGamma->setText("1.0");
    d_ptr->textEditTheta->setText("1.0");
    d_ptr->textEditDeltaT->setText("0.1");
    d_ptr->textEditTime->setText("12.0");
    this->setValuesToMassOscillator();
}
/*===========================================================*/
void TmcMassOscillatorWidget::setValuesToMassOscillator()
{
    double g = d_ptr->textEditB->text().toDouble();
    double m = d_ptr->textEditM->text().toDouble();
    double d = d_ptr->textEditD->text().toDouble();
    double k = d_ptr->textEditK->text().toDouble();
    double load = d_ptr->textEditLoad->text().toDouble();
    double alpha = d_ptr->textEditAlpha->text().toDouble();
    double beta = d_ptr->textEditBeta->text().toDouble();
    double gamma = d_ptr->textEditGamma->text().toDouble();
    double theta = d_ptr->textEditTheta->text().toDouble();
    double deltaT = d_ptr->textEditDeltaT->text().toDouble();
    double time = d_ptr->textEditTime->text().toDouble();

    d_ptr->massOscillator->setValues(g, m, d, k, load, alpha, beta, gamma, theta, deltaT, time);
}
/*===========================================================*/
void TmcMassOscillatorWidget::compute(bool standard)
{
    this->setValuesToMassOscillator();
    std::cout << "Solver:\n"
              << d_ptr->massOscillator->toString() << std::endl;
    std::vector<std::tuple<double, double, double, double, double>> data;
    TmcTimer timer;
    timer.start();
    d_ptr->massOscillator->compute(data, standard);
    timer.stop();
    std::cout << "Elapsed time: " << timer.getTotalTime() << " s" << std::endl;
    d_ptr->plotWidget->updateData(data);
}
/*===========================================================*/
void TmcMassOscillatorWidget::on_pushButtonInitialValues_clicked()
{
    this->setInitialOscillatorDefaultValues();
}
/*===========================================================*/
void TmcMassOscillatorWidget::on_pushButtonComputeNew_clicked()
{
    this->compute(false);
}
/*===========================================================*/
void TmcMassOscillatorWidget::on_pushButtonComputeStandard_clicked()
{
    this->compute(true);
}
/*===========================================================*/
