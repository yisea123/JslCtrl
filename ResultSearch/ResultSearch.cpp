#include "ResultSearch.h"
#include <QMessageBox>

ResultSearch::ResultSearch(QWidget *parent)
	: QMainWindow(parent)
{
	//ui.setupUi(this);

	/*************************** Search *******************************/
	QLabel *labelbox = new QLabel(QString::fromLocal8Bit("弹夹ID"), this);
	boxidEdit = new QLineEdit(this);

	QPushButton *searchBtn = new QPushButton(QString::fromLocal8Bit("搜索"), this);
	connect(searchBtn,SIGNAL(clicked()),this,SLOT(slot_searchBtn()));
	QPushButton *clearTable = new QPushButton(QString::fromLocal8Bit("清空结果"), this);
	connect(clearTable, SIGNAL(clicked()), this, SLOT(slot_clearTable()));

	QGridLayout *searchLayout = new QGridLayout();
	searchLayout->addWidget(labelbox, 0, 0);
	searchLayout->addWidget(boxidEdit, 0, 1);
	searchLayout->addWidget(searchBtn, 2, 0);
	searchLayout->addWidget(clearTable, 2, 1);

	QWidget *searchWidget = new QWidget(this);
	searchWidget->setLayout(searchLayout);
	QDockWidget *searchDockWidget = new QDockWidget(QString::fromLocal8Bit("操作区"),this);
	searchDockWidget->setWidget(searchWidget);
	searchDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
	this->addDockWidget(Qt::LeftDockWidgetArea, searchDockWidget);
	/********************************************************************/


	//***************************** central **********************************
	cmosTAB = new QList<QTableWidget *>();
	QList<QLabel*> *pannelLabel = new QList<QLabel*>();
	QFont ft;
	ft.setPointSize(20);
	ft.setBold(true);
	for (int n = 0; n < 10; n++)
	{
		pannelLabel->append(new QLabel(QString("%1").arg(n+1)));
		pannelLabel->at(n)->setFont(ft);
		pannelLabel->at(n)->setAlignment(Qt::AlignCenter);
		cmosTAB->append(new QTableWidget());
		cmosTAB->at(n)->setMinimumWidth(1200);
		//cmosTAB->at(n)->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		//cmosTAB->at(n)->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		
		/*for (int i = 0; i < cmosTAB->at(n)->rowCount(); i++)
		{
			for (int j = 0; j < cmosTAB->at(n)->columnCount(); j++)
			{
				cmosTAB->at(n)->setItem(i, j, new QTableWidgetItem(QString(" ")));
				cmosTAB->at(n)->item(i, j)->setBackgroundColor(QColor(255, 255, 240));
				cmosTAB->at(n)->item(i, j)->setFlags(0);
				cmosTAB->at(n)->setRowHeight(i, 42);
				cmosTAB->at(n)->setColumnWidth(j, 42);
			}
		}*/
	}

	QVBoxLayout *centralLayout = new QVBoxLayout();
	for (int n = 0; n < 10; n++)
	{
		centralLayout->addWidget(pannelLabel->at(n));
		centralLayout->addWidget(cmosTAB->at(n));
	}
	QWidget *centralWidget = new QWidget();
	centralWidget->setLayout(centralLayout);
	
	/*QScrollArea **/centralScroll = new QScrollArea();
	//centralScroll->setLayout(centralLayout);
	centralScroll->setWidget(centralWidget);
	//centralScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setCentralWidget(centralScroll);
	/*************************************************************************/
}

ResultSearch::~ResultSearch()
{ }

void ResultSearch::slot_searchBtn()
{
	if (boxidEdit->text() == NULL)
	{
		/*QMessageBox msgBox(QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("弹夹ID不能为空"), QMessageBox::Warning, QMessageBox::Ok,QMessageBox::Cancel,QMessageBox::NoButton,NULL);
		if (msgBox.exec() == QMessageBox::Cancel)
			NULL;*/
		QMessageBox::warning(boxidEdit, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("弹夹ID不能为空"));
		return;
	}
	
	QSqlDatabase db;
	if (QSqlDatabase::contains("qt_sql_default_connection"))
		db = QSqlDatabase::database("qt_sql_default_connection");
	else
	{
		db = QSqlDatabase::addDatabase("QSQLITE");
		db.setDatabaseName("./config/datebase.db");
	}
	if (!db.open())
	{
		QMessageBox::warning(this,"Error","Cannot opened database");
		return;
	}
	QSqlQuery query;

	//获取物料编号
	query.exec(QString("SELECT config,box_id FROM ITEM_RECORD WHERE box_id = '%1'").arg(boxidEdit->text()));
	if (!query.next())
	{
		QMessageBox::warning(this, "Error", "Cannot Find the ID");
		return;
	}
	QString config = query.value(0).toString();
	query.clear();

	//查询物料配置
	query.exec(QString("SELECT rowCMOS,colCMOS FROM CONFIG WHERE ConfigName='%1'").arg(config));
	if (!query.next())
	{
		QMessageBox::warning(this, "Error", QString::fromLocal8Bit("找不到此ID使用的配置:%1").arg(config));
		return;
	}
	int row = query.value(0).toInt();;
	int col = query.value(1).toInt();
	query.clear();

	for (int n = 0; n < 10; n++)
	{
		cmosTAB->at(n)->setRowCount(row);
		cmosTAB->at(n)->setColumnCount(col);
		
		for (int i = 0; i < cmosTAB->at(n)->rowCount(); i++)
		{
			for (int j = 0; j < cmosTAB->at(n)->columnCount(); j++)
			{
				cmosTAB->at(n)->setItem(i, j, new QTableWidgetItem(QString(" ")));
				//cmosTAB->at(n)->item(i, j)->setBackgroundColor(QColor(255, 255, 240));
				cmosTAB->at(n)->item(i, j)->setFlags(0);
				cmosTAB->at(n)->setRowHeight(i, 42);
				cmosTAB->at(n)->setColumnWidth(j, 42);
			}
		}
	}


	query.exec(QString("Select pannel_id,sample_id,process_res FROM ITEM_RECORD WHERE box_id = '%1'").arg(boxidEdit->text()));
	while (query.next())
	{
		int pannel_id = query.value(0).toInt();
		int sample_id = query.value(1).toInt();
		QString process_res = query.value(2).toString();

		int currow, curcol;			
		if (sample_id % col == 0) {
			currow = sample_id / col - 1;
		}
		else
		{
			currow = sample_id / col;
		}

		curcol = sample_id % col - 1;
		if (curcol == -1) {
			curcol = col - 1;
		}


		QColor color = QColor(255, 255, 240);
		if (process_res == "OK")
		{
			color = QColor(0, 238, 0);			
		}
		else if (process_res == "NG")
		{
			color = QColor(255, 64, 64);
		}
		else if (process_res == "NO")
		{
			color = QColor(255, 255, 0);
		}
		cmosTAB->at(pannel_id - 1)->item(currow, curcol)->setBackgroundColor(color);
			
	}
}

void ResultSearch::slot_clearTable()
{
	for (int i = 0; i < 10; i++)
	{
		cmosTAB->at(i)->setRowCount(0);
		cmosTAB->at(i)->setColumnCount(0);
	}
}