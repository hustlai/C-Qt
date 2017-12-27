/********************************************************************************
** Form generated from reading UI file 'imageprocesstool.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGEPROCESSTOOL_H
#define UI_IMAGEPROCESSTOOL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ImageProcessToolClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ImageProcessToolClass)
    {
        if (ImageProcessToolClass->objectName().isEmpty())
            ImageProcessToolClass->setObjectName(QStringLiteral("ImageProcessToolClass"));
        ImageProcessToolClass->resize(600, 400);
        menuBar = new QMenuBar(ImageProcessToolClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        ImageProcessToolClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ImageProcessToolClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        ImageProcessToolClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(ImageProcessToolClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        ImageProcessToolClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(ImageProcessToolClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ImageProcessToolClass->setStatusBar(statusBar);

        retranslateUi(ImageProcessToolClass);

        QMetaObject::connectSlotsByName(ImageProcessToolClass);
    } // setupUi

    void retranslateUi(QMainWindow *ImageProcessToolClass)
    {
        ImageProcessToolClass->setWindowTitle(QApplication::translate("ImageProcessToolClass", "ImageProcessTool", 0));
    } // retranslateUi

};

namespace Ui {
    class ImageProcessToolClass: public Ui_ImageProcessToolClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGEPROCESSTOOL_H
