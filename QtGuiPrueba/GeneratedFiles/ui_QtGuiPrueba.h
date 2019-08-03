/********************************************************************************
** Form generated from reading UI file 'QtGuiPrueba.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTGUIPRUEBA_H
#define UI_QTGUIPRUEBA_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtGuiPruebaClass
{
public:
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QtGuiPruebaClass)
    {
        if (QtGuiPruebaClass->objectName().isEmpty())
            QtGuiPruebaClass->setObjectName(QString::fromUtf8("QtGuiPruebaClass"));
        QtGuiPruebaClass->resize(600, 400);
        centralWidget = new QWidget(QtGuiPruebaClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QtGuiPruebaClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(QtGuiPruebaClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        QtGuiPruebaClass->setStatusBar(statusBar);

        retranslateUi(QtGuiPruebaClass);

        QMetaObject::connectSlotsByName(QtGuiPruebaClass);
    } // setupUi

    void retranslateUi(QMainWindow *QtGuiPruebaClass)
    {
        QtGuiPruebaClass->setWindowTitle(QCoreApplication::translate("QtGuiPruebaClass", "QtGuiPrueba", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtGuiPruebaClass: public Ui_QtGuiPruebaClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTGUIPRUEBA_H
