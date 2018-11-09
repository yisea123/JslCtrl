/********************************************************************************
** Form generated from reading UI file 'ResultSearch.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RESULTSEARCH_H
#define UI_RESULTSEARCH_H

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

class Ui_ResultSearchClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ResultSearchClass)
    {
        if (ResultSearchClass->objectName().isEmpty())
            ResultSearchClass->setObjectName(QStringLiteral("ResultSearchClass"));
        ResultSearchClass->resize(600, 400);
        menuBar = new QMenuBar(ResultSearchClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        ResultSearchClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ResultSearchClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        ResultSearchClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(ResultSearchClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        ResultSearchClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(ResultSearchClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ResultSearchClass->setStatusBar(statusBar);

        retranslateUi(ResultSearchClass);

        QMetaObject::connectSlotsByName(ResultSearchClass);
    } // setupUi

    void retranslateUi(QMainWindow *ResultSearchClass)
    {
        ResultSearchClass->setWindowTitle(QApplication::translate("ResultSearchClass", "ResultSearch", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ResultSearchClass: public Ui_ResultSearchClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RESULTSEARCH_H
