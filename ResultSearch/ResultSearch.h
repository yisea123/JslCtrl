#pragma once

#include <QtWidgets/QMainWindow>
//#include "ui_ResultSearch.h"
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qdockwidget.h>
#include <qlayout.h>
#include <qtablewidget.h>
#include <qlist.h>
#include <qscrollarea.h>
#include <qdebug.h>


class ResultSearch : public QMainWindow
{
	Q_OBJECT


public:
	ResultSearch(QWidget *parent = Q_NULLPTR);
	~ResultSearch();

public slots:
	void slot_searchBtn();
	void slot_clearTable();

private:
	QLineEdit *boxidEdit;
	QList<QTableWidget *> *cmosTAB;
	QScrollArea *centralScroll;
	
	
	//Ui::ResultSearchClass ui;
};
