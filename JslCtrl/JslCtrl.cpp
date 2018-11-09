#include "JslCtrl.h"
#include <QtGui>  
#include <QApplication>  

JslCtrl::JslCtrl(QWidget *parent)
	: QMainWindow(parent),m_motion(new leisaiMotion())
{
	/****************************** Menu Bar **************************/
	QMenuBar *menuBar = new QMenuBar(this);
	QMenu *menu[10];
	menu[0] = menuBar->addMenu("Admin");
	QAction *Setting = new QAction(tr("Settings"), this);
	areaCheck = new QAction(QString::fromLocal8Bit("轮廓检测"), this);
	areaCheck->setCheckable(true);
	areaCheck->setChecked(true);
	menu[0]->addAction(Setting);
	menu[0]->addAction(areaCheck);
	menu[1] = menuBar->addMenu("Test");
	QAction *DebugMode = new QAction(tr("Debug Mode"), this);
	QAction *ShowIOTab = new QAction(tr("IO TAB"), this);
	QAction *loose = new QAction(QString::fromLocal8Bit("松开夹子"), this);
	QAction *testbtn = new QAction(tr("testbtn"), this);
	menu[1]->addAction(DebugMode);
	menu[1]->addAction(ShowIOTab);	
	menu[1]->addAction(loose);
	menu[1]->addAction(testbtn);
	this->setMenuBar(menuBar);

	connect(Setting, SIGNAL(triggered()), this, SLOT(slot_Setting()));
	connect(DebugMode, SIGNAL(triggered()), this, SLOT(slot_DebugMode()));
	connect(ShowIOTab, SIGNAL(triggered()), this, SLOT(slot_ShowIOTab()));
	connect(testbtn, SIGNAL(triggered()), this, SLOT(slot_test()));
	connect(loose, SIGNAL(triggered()), this, SLOT(slot_loose()));
	connect(areaCheck, SIGNAL(triggered()), this, SLOT(slot_areaCheck()));
	/*****************************************************************/

	/****************************** LogPannel **************************/
	myQTextBrowser = new QTextBrowser();
	myDockWidget = new QDockWidget("Log Panel", this);
	myDockWidget->setWidget(myQTextBrowser);
	addDockWidget(Qt::BottomDockWidgetArea, myDockWidget);
	connect(this, SIGNAL(sigLogAdd(const QString&)), this, SLOT(slot_LogAdd(const QString&)));
	connect(m_motion, SIGNAL(sigLogAdd(const QString&)), this, SLOT(slot_LogAdd(const QString&)));
	/*****************************************************************/


	/********************************** button *********************************/
	QHBoxLayout *hLayout2 = new QHBoxLayout();
	warnningLineEdit = new QLabel(this); 
	showCurConfig = new QLabel(this);
	
	QFont configFont;
	configFont.setBold(true);
	configFont.setPointSize(16);
	showCurConfig->setFont(configFont);


	QFont warnningFont;
	warnningFont.setBold(true);
	warnningFont.setPointSize(20);
	
	QPalette warnningColor;
	warnningColor.setColor(QPalette::WindowText, Qt::blue);
	warnningLineEdit->setFont(warnningFont);
	warnningLineEdit->setPalette(warnningColor);

	StartBtn = new QPushButton(QString::fromLocal8Bit("开始"), this);
	StartBtn->setFocusPolicy(Qt::NoFocus);
	EmergencyStopOnBtn = new QPushButton(QString::fromLocal8Bit("停止"), this);
	EmergencyStopOnBtn->setFocusPolicy(Qt::NoFocus);
	EmergencyStopOffBtn = new QPushButton(QString::fromLocal8Bit("急停关"), this);
	EmergencyStopOffBtn->setFocusPolicy(Qt::NoFocus);
	ResetBtn = new QPushButton(QString::fromLocal8Bit("复位"), this);
	ResetBtn->setFocusPolicy(Qt::NoFocus);
	
	hLayout2->addWidget(warnningLineEdit,5);
	hLayout2->addWidget(showCurConfig,2);
	hLayout2->addWidget(StartBtn,1);
	hLayout2->addWidget(EmergencyStopOnBtn,1);
	hLayout2->addWidget(EmergencyStopOffBtn,1);
	hLayout2->addWidget(ResetBtn,1);

	connect(this, SIGNAL(sigWarnningLogAdd(const QString&)), this, SLOT(slot_warnningLog(const QString&)));
	connect(m_motion, SIGNAL(sigWarnningLogAdd(const QString&)), this, SLOT(slot_warnningLog(const QString&)));
	/********************************** button  End **************************/
	


	/**********************************  IOTAB  **************************/
	InputIOStatus0 = new IOLabel(QString::fromLocal8Bit("0"), true, 0, this);
	InputIOStatus1 = new IOLabel(QString::fromLocal8Bit("1"), true, 1, this);
	InputIOStatus2 = new IOLabel(QString::fromLocal8Bit("2"), true, 2, this);
	InputIOStatus3 = new IOLabel(QString::fromLocal8Bit("3"), true, 3, this);
	InputIOStatus4 = new IOLabel(QString::fromLocal8Bit("4"), true, 4, this);
	InputIOStatus5 = new IOLabel(QString::fromLocal8Bit("5"), true, 5, this);
	InputIOStatus6 = new IOLabel(QString::fromLocal8Bit("6"), true, 6, this);
	InputIOStatus7 = new IOLabel(QString::fromLocal8Bit("7"), true, 7, this);
	InputIOStatus8 = new IOLabel(QString::fromLocal8Bit("8"), true, 8, this);
	InputIOStatus9 = new IOLabel(QString::fromLocal8Bit("9"), true, 9, this);
	InputIOStatus10 = new IOLabel(QString::fromLocal8Bit("10"), true, 10, this);
	InputIOStatus11 = new IOLabel(QString::fromLocal8Bit("11"), true, 11, this);
	InputIOStatus12 = new IOLabel(QString::fromLocal8Bit("12"), true, 12, this);
	InputIOStatus13 = new IOLabel(QString::fromLocal8Bit("13"), true, 13, this);
	OutputIOStatus0 = new IOLabel(QString::fromLocal8Bit("0"), false, 0);
	OutputIOStatus1 = new IOLabel(QString::fromLocal8Bit("1"), false, 1);
	OutputIOStatus2 = new IOLabel(QString::fromLocal8Bit("2"), false, 2);
	OutputIOStatus3 = new IOLabel(QString::fromLocal8Bit("3"), false, 3);
	OutputIOStatus4 = new IOLabel(QString::fromLocal8Bit("4"), false, 4);
	QTableWidget *tab = new QTableWidget(2, 15);
	tab->setCellWidget(0, 0, InputIOStatus0);
	tab->setCellWidget(0, 1, InputIOStatus1);
	tab->setCellWidget(0, 2, InputIOStatus2);
	tab->setCellWidget(0, 3, InputIOStatus3);
	tab->setCellWidget(0, 4, InputIOStatus4);
	tab->setCellWidget(0, 5, InputIOStatus5);
	tab->setCellWidget(0, 6, InputIOStatus6);
	tab->setCellWidget(0, 7, InputIOStatus7);
	tab->setCellWidget(0, 8, InputIOStatus8);
	tab->setCellWidget(0, 9, InputIOStatus9);
	tab->setCellWidget(0, 10, InputIOStatus10);
	tab->setCellWidget(0, 11, InputIOStatus11);
	tab->setCellWidget(0, 12, InputIOStatus12);
	tab->setCellWidget(0, 13, InputIOStatus13);
	tab->setCellWidget(1, 0, OutputIOStatus0);
	tab->setCellWidget(1, 1, OutputIOStatus1);
	tab->setCellWidget(1, 2, OutputIOStatus2);
	tab->setCellWidget(1, 3, OutputIOStatus3);
	tab->setCellWidget(1, 4, OutputIOStatus4);
	tab->resizeRowsToContents();
	tab->resizeColumnsToContents();
	tab->setFocusPolicy(Qt::NoFocus);
	//tab->horizontalHeader()->setStretchLastSection(true);
	tab->setHorizontalHeaderLabels(QStringList() << "0" << "1" << "2" << "3" << "4" << "5" << "6"\
		<< "7" << "8" << "9" << "10" << "11" << "12" << "13" << "");
	tab->setVerticalHeaderLabels(QStringList() << "In" << "Out");
	tab->setEditTriggers(QAbstractItemView::NoEditTriggers);
	tab->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectItems);
	tab->setSelectionMode(QAbstractItemView::SingleSelection);
	/*QDockWidget **/ioDockWidget = new QDialog(/*"IO Status", */this);
	QHBoxLayout *iolayout = new QHBoxLayout();
	iolayout->addWidget(tab);
	ioDockWidget->setLayout(iolayout);
	
	//addDockWidget(Qt::BottomDockWidgetArea, ioDockWidget);
	ioDockWidget->hide();	
	
	/*******************************  IOTAB END  **************************/
	
	/*******************************  Settings  *******************************/
	curSettings = new QSettings("./config/curSettings.ini", QSettings::IniFormat);
	iCurrentConfigNum = curSettings->value("iCurrentConfigNum").toInt();
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
		emit sigWarnningLogAdd(QString::fromLocal8Bit("不能打开数据库！"));
		emit sigLogAdd(QString::fromLocal8Bit("不能打开数据库！"));
	}
	QSqlQuery query;
	query.exec("SELECT ConfigName FROM CONFIG");

	strlistConfigName.clear();
	while (query.next())
	{
		strlistConfigName.append(query.value(0).toString());
	}

	if (!m_motion->LoadConfig(strlistConfigName.at(iCurrentConfigNum)))
	{
		emit sigWarnningLogAdd(QString::fromLocal8Bit("载入配置失败"));
		emit sigLogAdd(QString::fromLocal8Bit("载入配置失败"));
	}
	m_motion->config = strlistConfigName.at(iCurrentConfigNum);
	showCurConfig->setText(QString::fromLocal8Bit("物料编码 %1").arg(strlistConfigName.at(iCurrentConfigNum)));
	/***********************************************************************/

	/**********************************  cmosTAB  **************************/
	QFont ft;
	ft.setPointSize(12);
	labStrList = new QStringList;
	QWidget *idList = new QWidget();
	QGridLayout *idlayout = new QGridLayout();
	QLabel *tipl1 = new QLabel(QString::fromLocal8Bit("弹夹1："));
	QLabel *tipl2 = new QLabel(QString::fromLocal8Bit("弹夹2："));
	QLabel *tipl3 = new QLabel(QString::fromLocal8Bit("弹夹3："));
	tipl1->setFont(ft);
	tipl2->setFont(ft);
	tipl3->setFont(ft);
	QFont ft2;
	ft2.setPointSize(18);
	ft2.setBold(true);
	idlist1 = new QLabel(" ");
	idlist2 = new QLabel(" ");
	idlist3 = new QLabel(" ");
	idlist1->setMinimumWidth(200);
	idlist2->setMinimumWidth(200);
	idlist3->setMinimumWidth(200);
	idlist1->setFont(ft2);
	idlist2->setFont(ft2);
	idlist3->setFont(ft2);
	boxOkBtn = new QPushButton(QString::fromLocal8Bit("确   认"));
	boxOkBtn->setFocusPolicy(Qt::NoFocus);
	boxClearBtn = new QPushButton(QString::fromLocal8Bit("清 空"));
	boxClearBtn->setFocusPolicy(Qt::NoFocus);
	boxOkBtn->setMinimumSize(120, 30);
	boxClearBtn->setMinimumSize(60, 30);
	boxOkBtn->setFont(ft);
	boxClearBtn->setFont(ft);
	

	idlayout->addWidget(tipl1, 0, 0);
	idlayout->addWidget(tipl2, 1, 0);
	idlayout->addWidget(tipl3, 2, 0);

	idlayout->addWidget(idlist1, 0, 1, 1, 2);
	idlayout->addWidget(idlist2, 1, 1, 1, 2);
	idlayout->addWidget(idlist3, 2, 1, 1, 2);
	idlayout->addWidget(boxOkBtn,3, 0, 1, 2, Qt::AlignLeft);
	idlayout->addWidget(boxClearBtn,3,2);
	idList->setLayout(idlayout);

	cmosTAB = new QTableWidget(m_motion->rowCMOS, m_motion->colCMOS);
	for (int i = 0; i < m_motion->rowCMOS; i++)
	{
		for (int j = 0; j < m_motion->colCMOS; j++)
		{
			cmosTAB->setItem(i, j, new QTableWidgetItem(QString(" ")));
			cmosTAB->item(i, j)->setBackgroundColor(QColor(255, 255, 240));
			cmosTAB->item(i, j)->setFlags(0);
		}
	}

	for (int j = 0; j < m_motion->colCMOS; j++)
		cmosTAB->setColumnWidth(j, 30);
	cmosTAB->setEditTriggers(QAbstractItemView::NoEditTriggers);
	cmosTAB->setMaximumHeight(120);
	cmosTAB->setFocusPolicy(Qt::NoFocus);
	
	//cmosTAB->item(1, 20)->setBackgroundColor(QColor(0, 0, 0));
	QHBoxLayout *cmos_layout = new QHBoxLayout();
	cmos_layout->addWidget(idList);
	//cmos_layout->addSpacing(50);
	cmos_layout->addWidget(cmosTAB);	
	/*********************************************************************/

	/***************************  Pic Show  *******************************/
	QHBoxLayout *picLayout = new QHBoxLayout();
	QScrollArea *PicScroll = new QScrollArea(this);
	PicShow = new QLabel(this);
	img = new QImage("./1.png");
	PicShow->setAlignment(Qt::AlignLeft);
	PicShow->setPixmap(QPixmap::fromImage(*img));
	
	PicShow->setMinimumHeight(687);
	PicShow->setMinimumWidth(960);
	PicScroll->setMinimumHeight(500);
	PicScroll->setWidget(PicShow);

	QWidget *statusShow = new QWidget(this);
	QGridLayout *grouplayout = new QGridLayout();
	QFont labelfont;
	labelfont.setPointSize(24);
	labelfont.setFamily(QString::fromLocal8Bit("微软雅黑"));
	QFont labelfont1;
	labelfont1.setPointSize(14);
	labelfont1.setFamily(QString::fromLocal8Bit("微软雅黑"));

	QFont labelfont2;
	labelfont2.setPointSize(6);
	labelfont2.setFamily(QString::fromLocal8Bit("微软雅黑"));

	QLabel *showRes0 = new QLabel(QString::fromLocal8Bit("结果"));
	showRes = new QLabel(QString::fromLocal8Bit(" "));

	QLabel *showPannelid0 = new QLabel(QString::fromLocal8Bit("载板"));
	showPannelid = new QLabel(QString::fromLocal8Bit(" "));

	QLabel *curSampleID0 = new QLabel(QString::fromLocal8Bit("位置"));
	curSampleID = new QLabel(QString::fromLocal8Bit(" "));
	
	QLabel *processtime0 = new QLabel(QString::fromLocal8Bit("处理时间"));
	processtime = new QLabel(QString::fromLocal8Bit(" "));
	
	QLabel *pathLabel0 = new QLabel(QString::fromLocal8Bit("图片路径"));
	pathLabel = new QPushButton(QString::fromLocal8Bit(" "),this);
	pathLabel->setFocusPolicy(Qt::NoFocus);

	QLabel *md5label0 = new QLabel(QString::fromLocal8Bit("MD5"));
	md5label = new QPushButton(QString::fromLocal8Bit(" "),this);
	md5label->setFocusPolicy(Qt::NoFocus);

	pathLabel0->setMinimumHeight(80);
	md5label0->setMinimumHeight(80);

	grouplayout->addWidget(showRes0, 0, 0);
	grouplayout->addWidget(showRes, 0, 1);
	grouplayout->addWidget(showPannelid0, 1, 0);
	grouplayout->addWidget(showPannelid, 1, 1);
	grouplayout->addWidget(curSampleID0, 2, 0);
	grouplayout->addWidget(curSampleID, 2, 1);
	grouplayout->addWidget(processtime0, 3, 0);
	grouplayout->addWidget(processtime, 3, 1);
	grouplayout->addWidget(pathLabel0, 4, 0);
	grouplayout->addWidget(pathLabel, 4, 1);
	grouplayout->addWidget(md5label0, 5, 0);
	grouplayout->addWidget(md5label, 5, 1);

	showRes0->setFont(labelfont);
	showPannelid0->setFont(labelfont1);
	curSampleID0->setFont(labelfont1);
	processtime0->setFont(labelfont1);
	pathLabel0->setFont(labelfont1);
	md5label0->setFont(labelfont1);
	showRes->setFont(labelfont);
	showPannelid->setFont(labelfont1);
	curSampleID->setFont(labelfont1);
	processtime->setFont(labelfont1);
	pathLabel->setFont(labelfont2);
	md5label->setFont(labelfont1);

	pathLabel->setMaximumWidth(300);

	statusShow->setLayout(grouplayout);
	PicScroll->setMinimumWidth(800);
	picLayout->addWidget(PicScroll,4);
	picLayout->addWidget(statusShow,3);
	connect(pathLabel, SIGNAL(clicked()), this, SLOT(slot_savepath()));
	connect(md5label, SIGNAL(clicked()), this, SLOT(slot_savemd5()));
	/*********************************************************************/

	QVBoxLayout *m_layout = new QVBoxLayout();	
	m_layout->addLayout(hLayout2);
	m_layout->addLayout(cmos_layout);
	m_layout->addLayout(picLayout);
	m_layout->addStretch(10);
	QWidget *m_widget = new QWidget(this);
	m_widget->setLayout(m_layout);
	this->setCentralWidget(m_widget);
	
	
	connect(m_motion, SIGNAL(sigSendRes(int,int,int,int, const QString&,const  QString&, const  QString&)), this, SLOT(slot_RecRes(int,int,int,int, const QString&, const  QString&, const  QString&)));
	connect(m_motion,SIGNAL(sig_resetBoxBtn()),this,SLOT(slot_resetBoxBtn()));
	connect(boxClearBtn,SIGNAL(clicked()),this,SLOT(slot_boxClearBtn()));
	connect(boxOkBtn, SIGNAL(clicked()), this, SLOT(slot_boxOkBtn()));
	connect(StartBtn,	SIGNAL(clicked()), this, SLOT(slot_StartBtn()));
	connect(EmergencyStopOnBtn, SIGNAL(clicked()), this, SLOT(slot_StopBtn()));
	connect(EmergencyStopOffBtn, SIGNAL(clicked()), this, SLOT(slot_EmergencyStopOff()));
	connect(ResetBtn, SIGNAL(clicked()), this, SLOT(slot_ResetBtn()));
	connect(m_motion, SIGNAL(sigSendImgPath(const QString&)), this, SLOT(slot_RecImgPath(const QString&)));
	connect(InputIOStatus5, SIGNAL(sig_ZeroToOne()), this, SLOT(slot_IO5Error()));
	connect(m_motion, SIGNAL(sig_updateBoxID(const QString&,int)), this, SLOT(slot_updateBoxID(const QString&,int)));
	
	QTimer::singleShot(0, this, SLOT(slot_init()));
}

JslCtrl::~JslCtrl()
{
	
}

void JslCtrl::slot_init()
{
	StartBtn->setEnabled(false);
	boxOkBtn->setEnabled(false);
	boxClearBtn->setEnabled(false);
	m_motion->Init("./config/leisai_config.ini");
	
}

void JslCtrl::slot_warnningLog(const QString &str)
{
	warnningLineEdit->setText(str);
}

void JslCtrl::slot_LogAdd(const QString &str)
{
	QString time = QTime::currentTime().toString("hh:mm:ss:zzz");
	myQTextBrowser->append('[' + time + "] " + str);
}

void JslCtrl::slot_StartBtn()
{
	if (m_motion->bES)
	{
		emit sigWarnningLogAdd(QString::fromLocal8Bit("急停中，请关急停、复位后重新开始"));
		emit sigLogAdd(QString::fromLocal8Bit("急停中，请关急停、复位后重新开始"));
		return;
	}

	if (m_motion->bNeedReset)
	{
		emit sigWarnningLogAdd(QString::fromLocal8Bit("请先复位"));
		emit sigLogAdd(QString::fromLocal8Bit("程序开始或关急停后请先复位"));
		return;
	}

	StartBtn->setEnabled(false);

	if (m_motion->bStarting)
	{
		emit sigWarnningLogAdd(QString::fromLocal8Bit("已经开始工作，不要重复点击"));
		emit sigLogAdd(QString::fromLocal8Bit("已经开始工作，不要重复点击"));
		return;
	}
	m_motion->bStarting = true;
	m_motion->start();
}

void JslCtrl::slot_StopBtn()
{
	m_motion->slot_EmergencyStopOn();
	m_motion->quit();
}

void JslCtrl::slot_EmergencyStopOff()
{
	m_motion->slot_EmergencyStopOff();
}

void JslCtrl::resetUI()
{
	showRes->setText(tr(" "));
	showRes->setStyleSheet("background:rgb(240,240,240)");
	showPannelid->setText(tr(" "));
	curSampleID->setText(tr(" "));
	processtime->setText(" ");
	pathLabel->setText(" ");
	md5label->setText(" ");
	slot_RecImgPath("./1.png");

	for (int i = 0; i < m_motion->rowCMOS; i++)
	{
		for (int j = 0; j < m_motion->colCMOS; j++)
			cmosTAB->item(i, j)->setBackgroundColor(QColor(255, 255, 240));
	}
}

void JslCtrl::slot_ResetBtn()
{
	if (!m_motion->bInited)
	{
		emit sigWarnningLogAdd(QString::fromLocal8Bit("初始化失败"));
		emit sigLogAdd(QString::fromLocal8Bit("初始化失败"));
		return;
	}

	if (m_motion->bStarting)
	{
		emit sigWarnningLogAdd(QString::fromLocal8Bit("请先停止"));
		emit sigLogAdd(QString::fromLocal8Bit("请先停止"));
		return;
	}

	if (m_motion->bES)
	{
		emit sigWarnningLogAdd(QString::fromLocal8Bit("请先关急停"));
		emit sigLogAdd(QString::fromLocal8Bit("请先关急停"));
		return;
	}

	
	

	resetUI();

	m_motion->slot_ResetBtn();
	slot_boxClearBtn();

	boxOkBtn->setEnabled(true);
	boxClearBtn->setEnabled(true);
	StartBtn->setEnabled(true);
}

void JslCtrl::slot_RecImgPath(const QString &path)
{
	img->load(path);
	PicShow->setPixmap(QPixmap::fromImage(*img));
}

void JslCtrl::slot_IO5Error()
{
	if (m_motion->bES)
		return;
	slot_StopBtn();
	emit sigWarnningLogAdd(QString::fromLocal8Bit("上料推杆卡位"));
	emit sigLogAdd(QString::fromLocal8Bit("上料推杆卡位"));
}

void JslCtrl::keyPressEvent(QKeyEvent *keyValue)
{
	//emit sigLogAdd(QString::number(keyValue->key()));
	recentKey[recentKeyPointer] = keyValue->key();
	recentKeyPointer = recentKeyPointer + 1;
	if(recentKeyPointer==3)
		recentKeyPointer = 0;

	if (recentKey[0] == Qt::Key_Control && recentKey[1] == Qt::Key_Shift && recentKey[2] == Qt::Key_J) {
		strBoxNum.clear();
		prevTime = currTime;

		recentKeyPointer = 0;
		recentKey[0] = 0;
		recentKey[1] = 0;
		recentKey[2] = 0;

		return;
	}
	
	

	if (keyValue->key() == Qt::Key_Return)
	{	
		currTime = QTime::currentTime();
		recentKeyPointer = 0;
		recentKey[0] = 0;
		recentKey[1] = 0;
		recentKey[2] = 0;

		if (currTime.msecsTo(prevTime) < -100) {
			strBoxNum.clear();
			prevTime = currTime;
			return;
		}

		if (strBoxNum.length() < 1) {
			strBoxNum.clear();
			prevTime = currTime;
			emit sigLogAdd(QString::fromLocal8Bit("弹夹编号格式错误"));
			return;
		}

		if (bBoxWorking)
		{
			strBoxNum.clear();
			return;
		}
		if (labStrList->size() > 0)
		{
			for (int i = 0; i < labStrList->size(); i++)
			{
				if (strBoxNum == labStrList->at(i))
				{
					strBoxNum.clear();
					emit sigLogAdd(QString::fromLocal8Bit("此ID已被扫描"));
					return;
				}
			}
		}

		if (labStrList->size() > 2)
		{
			emit sigLogAdd(QString::fromLocal8Bit("工作队列已满"));
			strBoxNum.clear();
			return;
		}

		labStrList->append(strBoxNum);

		if (labStrList->size()>0)
			idlist1->setText(labStrList->at(0));
		if(labStrList->size()>1)
			idlist2->setText(labStrList->at(1));
		if (labStrList->size()>2)
			idlist3->setText(labStrList->at(2));


		strBoxNum.clear();
	}
	else
	{
		currTime = QTime::currentTime();
		if (strBoxNum.length() == 0) {
			strBoxNum += keyValue->key();
			prevTime = currTime;
			
		}
		else if (currTime.msecsTo(prevTime) > -100) {
			strBoxNum += keyValue->key();
			prevTime = currTime;
		}
		else {
			strBoxNum.clear();
			strBoxNum += keyValue->key();
			prevTime = currTime;

			recentKeyPointer = 0;
			recentKey[0] = 0;
			recentKey[1] = 0;
			recentKey[2] = 0;
			recentKey[recentKeyPointer] = keyValue->key();
			recentKeyPointer = recentKeyPointer + 1;
		}
		
	}
}

void  JslCtrl::slot_updateBoxID(const QString &str,int flag)
{
	QString myColor;
	if (flag == 1)
		myColor = "yellow";
	else if (flag == 0)
		myColor = "lime";


	int i = 0;
	for (i = 0; i < labStrList->size(); i++)
		if (labStrList->at(i) == str)
			break;
	switch (i)
	{
	case 0:idlist1->setStyleSheet(QString("background:%1").arg(myColor)); break;
	case 1:idlist2->setStyleSheet(QString("background:%1").arg(myColor)); break;
	case 2:idlist3->setStyleSheet(QString("background:%1").arg(myColor)); break;
	}
}

void JslCtrl::slot_RecRes(int pannelid, int currow,int curcol, int status, const QString& protime, const QString& md5, const QString& rawpath)
{
	if (pannelid == 0)
	{
		resetUI();
		return;
	}

	//cal(m_motion->rowCMOS, m_motion->colCMOS, sampleid);
	pathLabel->setText(rawpath);

	showPannelid->setText(QString::number(pannelid));
	curSampleID->setText(tr("[%1,%2]").arg(currow).arg(curcol));
	processtime->setText(protime);
	md5label->setText(md5);

	if (status == 1)//OK
	{
		cmosTAB->item(currow-1, curcol-1)->setBackgroundColor(QColor(0, 238, 0));
		showRes->setStyleSheet("background:lime");
		showRes->setText("OK");
	}
	else if (status == 0)//NG
	{
		cmosTAB->item(currow - 1, curcol - 1)->setBackgroundColor(QColor(255, 64, 64));
		showRes->setStyleSheet("background:red");
		showRes->setText("NG");
	}
	else if (status == 2)//NO
	{
		cmosTAB->item(currow - 1, curcol - 1)->setBackgroundColor(QColor(255, 255, 0));
		showRes->setStyleSheet("background:yellow");
		showRes->setText(QString::fromLocal8Bit("未检测到sensor"));
	}
}

void JslCtrl::cal(int ROW, int COL, int cnt_index)
{

		col_coord = int((cnt_index + ROW - 1) / ROW);

		if (col_coord % 2 == 1)
		{
			if (cnt_index % ROW == 0)
			{
				row_coord = ROW - 1;
			}
			else {
				row_coord = cnt_index % ROW - 1;
			}
		}
		else {
			if (cnt_index % ROW == 0)
			{
				row_coord = 0;
			}
			else {
				row_coord = ROW - cnt_index % ROW;
			}
		}

		col_coord = COL - col_coord;
	
}

void JslCtrl::slot_boxOkBtn()
{
	if (labStrList->size() == 0)
	{
		return;
	}

	boxOkBtn->setEnabled(false);
	boxClearBtn->setEnabled(false);
	bBoxWorking = true;
	

	for (int i = 0; i < labStrList->size(); i++)
	{
		m_motion->box_id.append(labStrList->at(i));
	}
	emit sigLogAdd(QString::fromLocal8Bit("队列已添加"));
}

void JslCtrl::slot_boxClearBtn()
{
	bBoxWorking = false;
	labStrList->clear();
	idlist1->setText(" ");
	idlist2->setText(" ");
	idlist3->setText(" ");
	idlist1->setStyleSheet("background:white");
	idlist2->setStyleSheet("background:white");
	idlist3->setStyleSheet("background:white");
}

void JslCtrl::slot_resetBoxBtn()
{
	slot_boxClearBtn();
	boxOkBtn->setEnabled(true);
	boxClearBtn->setEnabled(true);
}

void JslCtrl::slot_savepath()
{

	QClipboard *clipboard = QApplication::clipboard();
	QString strX = pathLabel->text();
	clipboard->setText(strX);
}

void JslCtrl::slot_savemd5()
{
	QClipboard *clipboard = QApplication::clipboard();
	QString strX = md5label->text();
	clipboard->setText(strX);
}

void JslCtrl::slot_ShowIOTab()
{
	if (ioDockWidget->isHidden())
		ioDockWidget->show();
	else
		ioDockWidget->hide();
}

void JslCtrl::slot_Setting()
{
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
		emit sigWarnningLogAdd(QString::fromLocal8Bit("不能打开数据库！"));
		emit sigLogAdd(QString::fromLocal8Bit("不能打开数据库！"));
		return;
	}
	emit sigWarnningLogAdd(QString::fromLocal8Bit(" "));
	QSqlQuery query;
	query.exec("SELECT id,ConfigName FROM CONFIG ORDER BY id");

	strlistConfigName.clear();
	while (query.next())
	{
		strlistConfigName.append(query.value(1).toString());
	}
	dlgSetting = new QDialog(this);
	dlgSetting->setWindowTitle(tr("Settings"));
	QHBoxLayout *set_hl = new QHBoxLayout();

	configList = new QListWidget();
	QList<QListWidgetItem*> *listConfigItem = new QList<QListWidgetItem*>();

	for (int i = 0; i < strlistConfigName.size(); i++)
	{
		listConfigItem->append(new QListWidgetItem(QString("%1").arg(strlistConfigName.at(i))));
		configList->addItem(listConfigItem->at(i));
		connect(configList, SIGNAL(currentRowChanged(int)), this, SLOT(slot_dataSelect(int)), Qt::UniqueConnection);
	}
	configList->setMaximumWidth(200);
	QVBoxLayout *set_vl_config = new QVBoxLayout();
	QPushButton *btnNewConfig = new QPushButton(QString::fromLocal8Bit("新建"));
	QPushButton *btnDelConfig = new QPushButton(QString::fromLocal8Bit("删除"));
	btnNewConfig->setFocusPolicy(Qt::NoFocus);
	btnDelConfig->setFocusPolicy(Qt::NoFocus);
	set_vl_config->addWidget(configList);
	set_vl_config->addWidget(btnNewConfig);
	set_vl_config->addWidget(btnDelConfig);
	connect(btnNewConfig, SIGNAL(clicked()), this, SLOT(slot_NewConfig()));
	connect(btnDelConfig, SIGNAL(clicked()), this, SLOT(slot_DelConfig()));

	QVBoxLayout *set_vl_speed = new QVBoxLayout();
	QVBoxLayout *set_vl_pos = new QVBoxLayout();
	QVBoxLayout *set_vl_other = new QVBoxLayout();

	QHBoxLayout *speed_c0a0_layout = new QHBoxLayout();
	QHBoxLayout *speed_c0a1_layout = new QHBoxLayout();
	QHBoxLayout *speed_c1a0_layout = new QHBoxLayout();
	QHBoxLayout *speed_c1a1_layout = new QHBoxLayout();
	QHBoxLayout *speed_c1a2_layout = new QHBoxLayout();
	QHBoxLayout *speed_c1a3_layout = new QHBoxLayout();
	QHBoxLayout *speed_c1a4_layout = new QHBoxLayout();
	QHBoxLayout *speed_c1a5_layout = new QHBoxLayout();
	QHBoxLayout *speed_c1a6_layout = new QHBoxLayout();
	QHBoxLayout *speed_c1a7_layout = new QHBoxLayout();

	QHBoxLayout *pos_c0a0_layout = new QHBoxLayout();
	QHBoxLayout *pos_c0a0_2_layout = new QHBoxLayout();
	QHBoxLayout *pos_c0a0_3_layout = new QHBoxLayout();
	QHBoxLayout *pos_c0a1_layout = new QHBoxLayout();
	QHBoxLayout *pos_c1a0_layout = new QHBoxLayout();
	QHBoxLayout *pos_c1a1_check_layout = new QHBoxLayout();
	QHBoxLayout *pos_c1a1_work_layout = new QHBoxLayout();
	QHBoxLayout *pos_c1a2_layout = new QHBoxLayout();
	QHBoxLayout *pos_c1a3_layout = new QHBoxLayout();
	QHBoxLayout *pos_c1a4_layout = new QHBoxLayout();
	QHBoxLayout *pos_c1a5_layout = new QHBoxLayout();
	QHBoxLayout *pos_c1a6_layout = new QHBoxLayout();
	QHBoxLayout *pos_c1a7_layout = new QHBoxLayout();

	QHBoxLayout *other_c0a0_StepDist_layout = new QHBoxLayout();
	QHBoxLayout *other_c0a1_StepDist_layout = new QHBoxLayout();
	QHBoxLayout *other_rowCMOS_layout = new QHBoxLayout();
	QHBoxLayout *other_colCMOS_layout = new QHBoxLayout();

	speed_c0a0_box = new QSpinBox(); speed_c0a0_box->setRange(0, 500000); //speed_c0a0_box->setValue(m_motion->speed_c0a0);
	speed_c0a1_box = new QSpinBox(); speed_c0a1_box->setRange(0, 500000); //speed_c0a1_box->setValue(m_motion->speed_c0a1);
	speed_c1a0_box = new QSpinBox(); speed_c1a0_box->setRange(0, 500000); //speed_c1a0_box->setValue(m_motion->speed_c1a0);
	speed_c1a1_box = new QSpinBox(); speed_c1a1_box->setRange(0, 500000); //speed_c1a1_box->setValue(m_motion->speed_c1a1);
	speed_c1a2_box = new QSpinBox(); speed_c1a2_box->setRange(0, 500000); //speed_c1a2_box->setValue(m_motion->speed_c1a2);
	speed_c1a3_box = new QSpinBox(); speed_c1a3_box->setRange(0, 500000); //speed_c1a3_box->setValue(m_motion->speed_c1a3);
	speed_c1a4_box = new QSpinBox(); speed_c1a4_box->setRange(0, 500000); //speed_c1a4_box->setValue(m_motion->speed_c1a4);
	speed_c1a5_box = new QSpinBox(); speed_c1a5_box->setRange(0, 500000); //speed_c1a5_box->setValue(m_motion->speed_c1a5);
	speed_c1a6_box = new QSpinBox(); speed_c1a6_box->setRange(0, 500000); //speed_c1a6_box->setValue(m_motion->speed_c1a6);
	speed_c1a7_box = new QSpinBox(); speed_c1a7_box->setRange(0, 500000); //speed_c1a7_box->setValue(m_motion->speed_c1a7);
	QLabel *speed_c0a0_label = new QLabel(tr("speed_c0a0"));
	QLabel *speed_c0a1_label = new QLabel(tr("speed_c0a1"));
	QLabel *speed_c1a0_label = new QLabel(tr("speed_c1a0"));
	QLabel *speed_c1a1_label = new QLabel(tr("speed_c1a1"));
	QLabel *speed_c1a2_label = new QLabel(tr("speed_c1a2"));
	QLabel *speed_c1a3_label = new QLabel(tr("speed_c1a3"));
	QLabel *speed_c1a4_label = new QLabel(tr("speed_c1a4"));
	QLabel *speed_c1a5_label = new QLabel(tr("speed_c1a5"));
	QLabel *speed_c1a6_label = new QLabel(tr("speed_c1a6"));
	QLabel *speed_c1a7_label = new QLabel(tr("speed_c1a7"));


	pos_c0a0_box = new QSpinBox(); pos_c0a0_box->setRange(-2000000, 2000000); //pos_c0a0_box->setValue(m_motion->pos_c0a0);
	pos_c0a0_2_box = new QSpinBox(); pos_c0a0_2_box->setRange(-2000000, 2000000);
	pos_c0a0_3_box = new QSpinBox(); pos_c0a0_3_box->setRange(-2000000, 2000000);
	pos_c0a1_box = new QSpinBox(); pos_c0a1_box->setRange(-2000000, 2000000);  //pos_c0a1_box->setValue(m_motion->pos_c0a1);
	pos_c1a0_box = new QSpinBox(); pos_c1a0_box->setRange(-2000000, 2000000); //pos_c1a0_box->setValue(m_motion->pos_c1a0);
	pos_c1a1_check_box = new QSpinBox(); pos_c1a1_check_box->setRange(-2000000, 2000000); //pos_c1a1_check_box->setValue(m_motion->pos_c1a1_check);
	pos_c1a1_work_box = new QSpinBox(); pos_c1a1_work_box->setRange(-2000000, 2000000);  //pos_c1a1_work_box->setValue(m_motion->pos_c1a1_work);
	pos_c1a2_box = new QSpinBox(); pos_c1a2_box->setRange(-2000000, 2000000); //pos_c1a2_box->setValue(m_motion->pos_c1a2);
	pos_c1a3_box = new QSpinBox(); pos_c1a3_box->setRange(-2000000, 2000000); //pos_c1a3_box->setValue(m_motion->pos_c1a3);
	pos_c1a4_box = new QSpinBox(); pos_c1a4_box->setRange(-2000000, 2000000); //pos_c1a4_box->setValue(m_motion->pos_c1a4);
	pos_c1a5_box = new QSpinBox(); pos_c1a5_box->setRange(-2000000, 2000000); //pos_c1a5_box->setValue(m_motion->pos_c1a5);
	pos_c1a6_box = new QSpinBox(); pos_c1a6_box->setRange(-2000000, 2000000); //pos_c1a6_box->setValue(m_motion->pos_c1a6);
	pos_c1a7_box = new QSpinBox(); pos_c1a7_box->setRange(-2000000, 2000000); //pos_c1a7_box->setValue(m_motion->pos_c1a7);

	QLabel *pos_c0a0_label = new QLabel(tr("pos_c0a0_1"));
	QLabel *pos_c0a0_2_label = new QLabel(tr("pos_c0a0_2"));
	QLabel *pos_c0a0_3_label = new QLabel(tr("pos_c0a0_3"));
	QLabel *pos_c0a1_label = new QLabel(tr("pos_c0a1"));
	QLabel *pos_c1a0_label = new QLabel(tr("pos_c1a0"));
	QLabel *pos_c1a1_check_label = new QLabel(tr("pos_c1a1_check"));
	QLabel *pos_c1a1_work_label = new QLabel(tr("pos_c1a1_work"));
	QLabel *pos_c1a2_label = new QLabel(tr("pos_c1a2"));
	QLabel *pos_c1a3_label = new QLabel(tr("pos_c1a3"));
	QLabel *pos_c1a4_label = new QLabel(tr("pos_c1a4"));
	QLabel *pos_c1a5_label = new QLabel(tr("pos_c1a5"));
	QLabel *pos_c1a6_label = new QLabel(tr("pos_c1a6"));
	QLabel *pos_c1a7_label = new QLabel(tr("pos_c1a7"));

	configNameEdit = new QLineEdit();
	other_c0a0_StepDist_box = new QSpinBox(); other_c0a0_StepDist_box->setRange(0, 500000); //other_c0a0_StepDist_box->setValue(m_motion->pos_c0a0_StepDist);
	other_c0a1_StepDist_box = new QSpinBox(); other_c0a1_StepDist_box->setRange(0, 500000); //other_c0a1_StepDist_box->setValue(m_motion->pos_c0a1_StepDist);
	other_rowCMOS_box = new QSpinBox(); //other_rowCMOS_box->setValue(m_motion->rowCMOS);
	other_colCMOS_box = new QSpinBox(); //other_colCMOS_box->setValue(m_motion->colCMOS);
	QLabel *other_c0a0_StepDist_label = new QLabel(tr("c0a0_StepDist"));
	QLabel *other_c0a1_StepDist_label = new QLabel(tr("c0a1_StepDist"));
	QLabel *other_rowCMOS_label = new QLabel(tr("rowCMOS"));
	QLabel *other_colCMOS_label = new QLabel(tr("colCMOS"));
	QPushButton *SaveDateBtn = new QPushButton(QString::fromLocal8Bit("保存并加载"), dlgSetting);
	SaveDateBtn->setFocusPolicy(Qt::NoFocus);

	speed_c0a0_layout->addWidget(speed_c0a0_box);
	speed_c0a1_layout->addWidget(speed_c0a1_box);
	speed_c1a0_layout->addWidget(speed_c1a0_box);
	speed_c1a1_layout->addWidget(speed_c1a1_box);
	speed_c1a2_layout->addWidget(speed_c1a2_box);
	speed_c1a3_layout->addWidget(speed_c1a3_box);
	speed_c1a4_layout->addWidget(speed_c1a4_box);
	speed_c1a5_layout->addWidget(speed_c1a5_box);
	speed_c1a6_layout->addWidget(speed_c1a6_box);
	speed_c1a7_layout->addWidget(speed_c1a7_box);

	speed_c0a0_layout->addWidget(speed_c0a0_label);
	speed_c0a1_layout->addWidget(speed_c0a1_label);
	speed_c1a0_layout->addWidget(speed_c1a0_label);
	speed_c1a1_layout->addWidget(speed_c1a1_label);
	speed_c1a2_layout->addWidget(speed_c1a2_label);
	speed_c1a3_layout->addWidget(speed_c1a3_label);
	speed_c1a4_layout->addWidget(speed_c1a4_label);
	speed_c1a5_layout->addWidget(speed_c1a5_label);
	speed_c1a6_layout->addWidget(speed_c1a6_label);
	speed_c1a7_layout->addWidget(speed_c1a7_label);


	pos_c0a0_layout->addWidget(pos_c0a0_box);
	pos_c0a0_2_layout->addWidget(pos_c0a0_2_box);
	pos_c0a0_3_layout->addWidget(pos_c0a0_3_box);
	pos_c0a1_layout->addWidget(pos_c0a1_box);
	pos_c1a0_layout->addWidget(pos_c1a0_box);
	pos_c1a1_check_layout->addWidget(pos_c1a1_check_box);
	pos_c1a1_work_layout->addWidget(pos_c1a1_work_box);
	pos_c1a2_layout->addWidget(pos_c1a2_box);
	pos_c1a3_layout->addWidget(pos_c1a3_box);
	pos_c1a4_layout->addWidget(pos_c1a4_box);
	pos_c1a5_layout->addWidget(pos_c1a5_box);
	pos_c1a6_layout->addWidget(pos_c1a6_box);
	pos_c1a7_layout->addWidget(pos_c1a7_box);
	pos_c0a0_layout->addWidget(pos_c0a0_label);
	pos_c0a0_2_layout->addWidget(pos_c0a0_2_label);
	pos_c0a0_3_layout->addWidget(pos_c0a0_3_label);
	pos_c0a1_layout->addWidget(pos_c0a1_label);
	pos_c1a0_layout->addWidget(pos_c1a0_label);
	pos_c1a1_check_layout->addWidget(pos_c1a1_check_label);
	pos_c1a1_work_layout->addWidget(pos_c1a1_work_label);
	pos_c1a2_layout->addWidget(pos_c1a2_label);
	pos_c1a3_layout->addWidget(pos_c1a3_label);
	pos_c1a4_layout->addWidget(pos_c1a4_label);
	pos_c1a5_layout->addWidget(pos_c1a5_label);
	pos_c1a6_layout->addWidget(pos_c1a6_label);
	pos_c1a7_layout->addWidget(pos_c1a7_label);


	other_c0a0_StepDist_layout->addWidget(other_c0a0_StepDist_box);
	other_c0a1_StepDist_layout->addWidget(other_c0a1_StepDist_box);
	other_rowCMOS_layout->addWidget(other_rowCMOS_box);
	other_colCMOS_layout->addWidget(other_colCMOS_box);
	other_c0a0_StepDist_layout->addWidget(other_c0a0_StepDist_label);
	other_c0a1_StepDist_layout->addWidget(other_c0a1_StepDist_label);
	other_rowCMOS_layout->addWidget(other_rowCMOS_label);
	other_colCMOS_layout->addWidget(other_colCMOS_label);

	set_vl_speed->addLayout(speed_c0a0_layout);
	set_vl_speed->addLayout(speed_c0a1_layout);
	set_vl_speed->addLayout(speed_c1a0_layout);
	set_vl_speed->addLayout(speed_c1a1_layout);
	set_vl_speed->addLayout(speed_c1a2_layout);
	set_vl_speed->addLayout(speed_c1a3_layout);
	set_vl_speed->addLayout(speed_c1a4_layout);
	set_vl_speed->addLayout(speed_c1a5_layout);
	set_vl_speed->addLayout(speed_c1a6_layout);
	set_vl_speed->addLayout(speed_c1a7_layout);

	set_vl_pos->addLayout(pos_c0a0_layout);
	set_vl_pos->addLayout(pos_c0a0_2_layout);
	set_vl_pos->addLayout(pos_c0a0_3_layout);
	set_vl_pos->addLayout(pos_c0a1_layout);
	set_vl_pos->addLayout(pos_c1a0_layout);
	set_vl_pos->addLayout(pos_c1a1_check_layout);
	set_vl_pos->addLayout(pos_c1a1_work_layout);
	set_vl_pos->addLayout(pos_c1a2_layout);
	set_vl_pos->addLayout(pos_c1a3_layout);
	set_vl_pos->addLayout(pos_c1a4_layout);
	set_vl_pos->addLayout(pos_c1a5_layout);
	set_vl_pos->addLayout(pos_c1a6_layout);
	set_vl_pos->addLayout(pos_c1a7_layout);

	set_vl_other->addWidget(configNameEdit);
	set_vl_other->addLayout(other_c0a0_StepDist_layout);
	set_vl_other->addLayout(other_c0a1_StepDist_layout);
	set_vl_other->addLayout(other_rowCMOS_layout);
	set_vl_other->addLayout(other_colCMOS_layout);
	set_vl_other->addWidget(SaveDateBtn);

	set_hl->addLayout(set_vl_config);
	set_hl->addLayout(set_vl_speed);
	set_hl->addLayout(set_vl_pos);
	set_hl->addLayout(set_vl_other);
	dlgSetting->setLayout(set_hl);

	connect(SaveDateBtn, SIGNAL(clicked()), this, SLOT(slot_SaveDateBtn()));
	configList->setCurrentRow(iCurrentConfigNum);
	dlgSetting->show();

}

void JslCtrl::slot_SaveDateBtn()
{
	/*slot_dataSelect(configList->currentRow());
	return;*/

	if (m_motion->bStarting)
	{
		QMessageBox::warning(this, "Warning", QString::fromLocal8Bit("机器正在工作，停止以后方可操作 "));
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
		emit sigWarnningLogAdd(QString::fromLocal8Bit("不能打开数据库！"));
		emit sigLogAdd(QString::fromLocal8Bit("不能打开数据库！"));
		return;
	}
	QString curConfig = strlistConfigName.at(configList->currentRow());
	QSqlQuery query;
	if (configNameEdit->text() != curConfig)
	{
		for (int i = 0; i < strlistConfigName.size(); i++)
		{
			if (configNameEdit->text() == strlistConfigName.at(i))
			{
				QMessageBox::warning(this, "Error", QString::fromLocal8Bit("配置名重复！"));
				configNameEdit->setText(curConfig);
				return;
			}
		}
	}

	if (speed_c0a0_box->value() != m_motion->speed_c0a0)
		query.exec(QString("UPDATE CONFIG SET speed_c0a0=%1 WHERE ConfigName='%2'").arg(speed_c0a0_box->value()).arg(curConfig));
	if (speed_c0a1_box->value() != m_motion->speed_c0a1)
		query.exec(QString("UPDATE CONFIG SET speed_c0a1=%1 WHERE ConfigName='%2'").arg(speed_c0a1_box->value()).arg(curConfig));
	if (speed_c1a0_box->value() != m_motion->speed_c1a0)
		query.exec(QString("UPDATE CONFIG SET speed_c1a0=%1 WHERE ConfigName='%2'").arg(speed_c1a0_box->value()).arg(curConfig));
	if (speed_c1a1_box->value() != m_motion->speed_c1a1)
		query.exec(QString("UPDATE CONFIG SET speed_c1a1=%1 WHERE ConfigName='%2'").arg(speed_c1a1_box->value()).arg(curConfig));
	if (speed_c1a2_box->value() != m_motion->speed_c1a2)
		query.exec(QString("UPDATE CONFIG SET speed_c1a2=%1 WHERE ConfigName='%2'").arg(speed_c1a2_box->value()).arg(curConfig));
	if (speed_c1a3_box->value() != m_motion->speed_c1a3)
		query.exec(QString("UPDATE CONFIG SET speed_c1a3=%1 WHERE ConfigName='%2'").arg(speed_c1a3_box->value()).arg(curConfig));
	if (speed_c1a4_box->value() != m_motion->speed_c1a4)
		query.exec(QString("UPDATE CONFIG SET speed_c1a4=%1 WHERE ConfigName='%2'").arg(speed_c1a4_box->value()).arg(curConfig));
	if (speed_c1a5_box->value() != m_motion->speed_c1a5)
		query.exec(QString("UPDATE CONFIG SET speed_c1a5=%1 WHERE ConfigName='%2'").arg(speed_c1a5_box->value()).arg(curConfig));
	if (speed_c1a6_box->value() != m_motion->speed_c1a6)
		query.exec(QString("UPDATE CONFIG SET speed_c1a6=%1 WHERE ConfigName='%2'").arg(speed_c1a6_box->value()).arg(curConfig));
	if (speed_c1a7_box->value() != m_motion->speed_c1a7)
		query.exec(QString("UPDATE CONFIG SET speed_c1a7=%1 WHERE ConfigName='%2'").arg(speed_c1a7_box->value()).arg(curConfig));
	if (pos_c0a0_box->value() != m_motion->pos_c0a0)
		query.exec(QString("UPDATE CONFIG SET pos_c0a0=%1 WHERE ConfigName='%2'").arg(pos_c0a0_box->value()).arg(curConfig));
	if (pos_c0a0_2_box->value() != m_motion->pos_c0a0_2)
		query.exec(QString("UPDATE CONFIG SET pos_c0a0_2=%1 WHERE ConfigName='%2'").arg(pos_c0a0_2_box->value()).arg(curConfig));
	if (pos_c0a0_3_box->value() != m_motion->pos_c0a0_3)
		query.exec(QString("UPDATE CONFIG SET pos_c0a0_3=%1 WHERE ConfigName='%2'").arg(pos_c0a0_3_box->value()).arg(curConfig));
	if (pos_c0a1_box->value() != m_motion->pos_c0a1)
		query.exec(QString("UPDATE CONFIG SET pos_c0a1=%1 WHERE ConfigName='%2'").arg(pos_c0a1_box->value()).arg(curConfig));
	if (pos_c1a0_box->value() != m_motion->pos_c1a0)
		query.exec(QString("UPDATE CONFIG SET pos_c1a0=%1 WHERE ConfigName='%2'").arg(pos_c1a0_box->value()).arg(curConfig));
	if (pos_c1a1_check_box->value() != m_motion->pos_c1a1_check)
		query.exec(QString("UPDATE CONFIG SET pos_c1a1_check=%1 WHERE ConfigName='%2'").arg(pos_c1a1_check_box->value()).arg(curConfig));
	if (pos_c1a1_work_box->value() != m_motion->pos_c1a1_work)
		query.exec(QString("UPDATE CONFIG SET pos_c1a1_work=%1 WHERE ConfigName='%2'").arg(pos_c1a1_work_box->value()).arg(curConfig));
	if (pos_c1a2_box->value() != m_motion->pos_c1a2)
		query.exec(QString("UPDATE CONFIG SET pos_c1a2=%1 WHERE ConfigName='%2'").arg(pos_c1a2_box->value()).arg(curConfig));
	if (pos_c1a3_box->value() != m_motion->pos_c1a3)
		query.exec(QString("UPDATE CONFIG SET pos_c1a3=%1 WHERE ConfigName='%2'").arg(pos_c1a3_box->value()).arg(curConfig));
	if (pos_c1a4_box->value() != m_motion->pos_c1a4)
		query.exec(QString("UPDATE CONFIG SET pos_c1a4=%1 WHERE ConfigName='%2'").arg(pos_c1a4_box->value()).arg(curConfig));
	if (pos_c1a5_box->value() != m_motion->pos_c1a5)
		query.exec(QString("UPDATE CONFIG SET pos_c1a5=%1 WHERE ConfigName='%2'").arg(pos_c1a5_box->value()).arg(curConfig));
	if (pos_c1a6_box->value() != m_motion->pos_c1a6)
		query.exec(QString("UPDATE CONFIG SET pos_c1a6=%1 WHERE ConfigName='%2'").arg(pos_c1a6_box->value()).arg(curConfig));
	if (pos_c1a7_box->value() != m_motion->pos_c1a7)
		query.exec(QString("UPDATE CONFIG SET pos_c1a7=%1 WHERE ConfigName='%2'").arg(pos_c1a7_box->value()).arg(curConfig));
	if (other_c0a0_StepDist_box->value() != m_motion->pos_c0a0_StepDist)
		query.exec(QString("UPDATE CONFIG SET pos_c0a0_StepDist=%1 WHERE ConfigName='%2'").arg(other_c0a0_StepDist_box->value()).arg(curConfig));
	if (other_c0a1_StepDist_box->value() != m_motion->pos_c0a1_StepDist)
		query.exec(QString("UPDATE CONFIG SET pos_c0a1_StepDist=%1 WHERE ConfigName='%2'").arg(other_c0a1_StepDist_box->value()).arg(curConfig));
	if ((other_rowCMOS_box->value() != m_motion->rowCMOS) || (other_colCMOS_box->value() != m_motion->colCMOS))
	{
		if (other_rowCMOS_box->value() != m_motion->rowCMOS)
			query.exec(QString("UPDATE CONFIG SET rowCMOS=%1 WHERE ConfigName='%2'").arg(other_rowCMOS_box->value()).arg(curConfig));
		if (other_colCMOS_box->value() != m_motion->colCMOS)
			query.exec(QString("UPDATE CONFIG SET colCMOS=%1 WHERE ConfigName='%2'").arg(other_colCMOS_box->value()).arg(curConfig));
		/*StartBtn->setEnabled(false);
		EmergencyStopOnBtn->setEnabled(false);
		EmergencyStopOffBtn->setEnabled(false);
		ResetBtn->setEnabled(false);
		emit sigWarnningLogAdd(QString::fromLocal8Bit("请重启程序"));
		emit sigLogAdd("Please reboot program");*/
		cmosTAB->setRowCount(other_rowCMOS_box->value());
		cmosTAB->setColumnCount(other_colCMOS_box->value());
		for (int i = 0; i < other_rowCMOS_box->value(); i++)
		{
			for (int j = 0; j < other_colCMOS_box->value(); j++)
			{
				cmosTAB->setItem(i, j, new QTableWidgetItem(QString(" ")));
				cmosTAB->item(i, j)->setBackgroundColor(QColor(255, 255, 240));
				cmosTAB->item(i, j)->setFlags(0);
			}
		}
		for (int j = 0; j <other_colCMOS_box->value(); j++)
			cmosTAB->setColumnWidth(j, 30);

	}
	if (configNameEdit->text() != curConfig)
	{
		query.exec(QString("UPDATE CONFIG SET ConfigName='%1' WHERE ConfigName='%2'").arg(configNameEdit->text()).arg(curConfig));
		strlistConfigName.replace(configList->currentRow(), configNameEdit->text());
	}

	db.close();

	iCurrentConfigNum = configList->currentRow();

	m_motion->LoadConfig(strlistConfigName.at(iCurrentConfigNum));
	curSettings->setValue("iCurrentConfigNum", iCurrentConfigNum);

	emit sigLogAdd("completed");
	m_motion->config = strlistConfigName.at(iCurrentConfigNum);
	showCurConfig->setText(QString::fromLocal8Bit("物料编码 %1").arg(strlistConfigName.at(iCurrentConfigNum)));
	dlgSetting->close();
}

void JslCtrl::slot_dataSelect(int n)
{
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
		emit sigWarnningLogAdd(QString::fromLocal8Bit("不能打开数据库！"));
		emit sigLogAdd(QString::fromLocal8Bit("不能打开数据库！"));
		return;
	}
	QSqlQuery query;
	query.exec(QString("SELECT speed_c0a0,speed_c0a1,speed_c1a0,speed_c1a1,speed_c1a2,speed_c1a3,speed_c1a4,speed_c1a5,speed_c1a6,speed_c1a7,pos_c0a0,pos_c0a0_2,pos_c0a0_3,pos_c0a1,pos_c1a0,pos_c1a1_check,pos_c1a1_work,pos_c1a2,pos_c1a3,pos_c1a4,pos_c1a5,pos_c1a6,pos_c1a7,pos_c0a0_StepDist,pos_c0a1_StepDist,rowCMOS,colCMOS,ConfigName FROM CONFIG WHERE ConfigName='%1'").arg(strlistConfigName.at(n)));
	query.next();

	speed_c0a0_box->setValue(query.value(0).toInt());
	speed_c0a1_box->setValue(query.value(1).toInt());
	speed_c1a0_box->setValue(query.value(2).toInt());
	speed_c1a1_box->setValue(query.value(3).toInt());
	speed_c1a2_box->setValue(query.value(4).toInt());
	speed_c1a3_box->setValue(query.value(5).toInt());
	speed_c1a4_box->setValue(query.value(6).toInt());
	speed_c1a5_box->setValue(query.value(7).toInt());
	speed_c1a6_box->setValue(query.value(8).toInt());
	speed_c1a7_box->setValue(query.value(9).toInt());
	pos_c0a0_box->setValue(query.value(10).toInt());
	pos_c0a0_2_box->setValue(query.value(11).toInt());
	pos_c0a0_3_box->setValue(query.value(12).toInt());
	pos_c0a1_box->setValue(query.value(13).toInt());
	pos_c1a0_box->setValue(query.value(14).toInt());
	pos_c1a1_check_box->setValue(query.value(15).toInt());
	pos_c1a1_work_box->setValue(query.value(16).toInt());
	pos_c1a2_box->setValue(query.value(17).toInt());
	pos_c1a3_box->setValue(query.value(18).toInt());
	pos_c1a4_box->setValue(query.value(19).toInt());
	pos_c1a5_box->setValue(query.value(20).toInt());
	pos_c1a6_box->setValue(query.value(21).toInt());
	pos_c1a7_box->setValue(query.value(22).toInt());
	other_c0a0_StepDist_box->setValue(query.value(23).toInt());
	other_c0a1_StepDist_box->setValue(query.value(24).toInt());
	other_rowCMOS_box->setValue(query.value(25).toInt());
	other_colCMOS_box->setValue(query.value(26).toInt());
	configNameEdit->setText(query.value(27).toString());
}

void JslCtrl::slot_NewConfig()
{
	if (m_motion->bStarting)
	{
		QMessageBox::warning(this, "Warning", QString::fromLocal8Bit("机器正在工作，停止以后方可操作 "));
		return;
	}
	QString name = QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz");
	strlistConfigName.append(name);
	configList->addItem(name);

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
		emit sigWarnningLogAdd(QString::fromLocal8Bit("不能打开数据库！"));
		emit sigLogAdd(QString::fromLocal8Bit("不能打开数据库！"));
		return;
	}
	QSqlQuery query;
	query.exec(QString("INSERT INTO CONFIG VALUES (NULL,%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15,%16,%17,%18,%19,%20,%21,%22,%23,%24,%25,%26,%27,%28)")
		.arg(name)
		.arg(m_motion->speed_c0a0)
		.arg(m_motion->speed_c0a1)
		.arg(m_motion->speed_c1a0)
		.arg(m_motion->speed_c1a1)
		.arg(m_motion->speed_c1a2)
		.arg(m_motion->speed_c1a3)
		.arg(m_motion->speed_c1a4)
		.arg(m_motion->speed_c1a5)
		.arg(m_motion->speed_c1a6)
		.arg(m_motion->speed_c1a7)
		.arg(m_motion->pos_c0a0)
		.arg(m_motion->pos_c0a0_2)
		.arg(m_motion->pos_c0a0_3)
		.arg(m_motion->pos_c0a1)
		.arg(m_motion->pos_c1a0)
		.arg(m_motion->pos_c1a1_check)
		.arg(m_motion->pos_c1a1_work)
		.arg(m_motion->pos_c1a2)
		.arg(m_motion->pos_c1a3)
		.arg(m_motion->pos_c1a4)
		.arg(m_motion->pos_c1a5)
		.arg(m_motion->pos_c1a6)
		.arg(m_motion->pos_c1a7)
		.arg(m_motion->pos_c0a0_StepDist)
		.arg(m_motion->pos_c0a1_StepDist)
		.arg(m_motion->rowCMOS)
		.arg(m_motion->colCMOS)
		);
	configList->setCurrentRow(configList->count() - 1);
}

void JslCtrl::slot_DelConfig()
{
	if (m_motion->bStarting)
	{
		QMessageBox::warning(this, "Warning", QString::fromLocal8Bit("机器正在工作，停止以后方可操作 "));
		return;
	}
	if (configList->count() == 1)
	{
		QMessageBox::warning(dlgSetting, QString::fromLocal8Bit("Error"), QString::fromLocal8Bit("至少留一项"));
		return;
	}

	if (configList->currentRow() == iCurrentConfigNum)
	{
		QMessageBox::warning(dlgSetting, QString::fromLocal8Bit("Error"), QString::fromLocal8Bit("当前配置项，禁止删除"));
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
		emit sigWarnningLogAdd(QString::fromLocal8Bit("不能打开数据库！"));
		emit sigLogAdd(QString::fromLocal8Bit("不能打开数据库！"));
		return;
	}
	QSqlQuery query;
	query.exec(QString("DELETE FROM CONFIG WHERE ConfigName = '%1'").arg(strlistConfigName.at(configList->currentRow())));


	int n = configList->currentRow();
	if (configList->currentRow() < iCurrentConfigNum)
	{
		iCurrentConfigNum--;
		curSettings->setValue("iCurrentConfigNum", iCurrentConfigNum);
	}


	if (n == configList->count() - 1)
	{
		configList->setCurrentRow(n - 1);

	}
	else
	{
		configList->setCurrentRow(n + 1);
	}
	strlistConfigName.removeAt(n);
	configList->takeItem(n);
}

void JslCtrl::slot_areaCheck()
{
	if (areaCheck->isChecked())
		m_motion->areaCheck(true);
	else
		m_motion->areaCheck(false);
}

void JslCtrl::slot_DebugMode()
{
	DebugMode *debugMode = new DebugMode();
	debugMode->show();
	return;
	QDialog *dlgDebugMode = new QDialog();

	dlgDebugMode->setWindowTitle(tr("DebugMode"));
	QHBoxLayout *hLayout1 = new QHBoxLayout();
	QLabel *label0 = new QLabel(QString::fromLocal8Bit("选择卡-->"), dlgDebugMode);
	CardSelect = new QComboBox(dlgDebugMode);
	for (int i = 0; i < m_motion->CardNum; i++)
		CardSelect->addItem(QString::number(i));

	QLabel *label1 = new QLabel(QString::fromLocal8Bit("选择轴-->"), dlgDebugMode);
	AxisSelect = new QComboBox(dlgDebugMode);
	for (int i = 0; i < 8; i++)
		AxisSelect->addItem(QString::number(i));

	QLabel *label2 = new QLabel(QString::fromLocal8Bit("速度-->"), dlgDebugMode);
	speed = new QSpinBox(dlgDebugMode);
	speed->setMinimumWidth(80);
	speed->setRange(0, 200000);

	QLabel *label3 = new QLabel(QString::fromLocal8Bit("目标-->"), dlgDebugMode);
	target = new QSpinBox(dlgDebugMode);
	target->setMinimumWidth(80);
	target->setRange(-2000000, 2000000);

	QLabel *label4 = new QLabel(QString::fromLocal8Bit("运动模式-->"), dlgDebugMode);
	MoveModeSelect = new QComboBox(dlgDebugMode);
	MoveModeSelect->addItem(QString::fromLocal8Bit("0(相对运动)"));
	MoveModeSelect->addItem(QString::fromLocal8Bit("1(绝对运动)"));
	MoveModeSelect->setCurrentIndex(1);

	QPushButton *Run = new QPushButton(QString::fromLocal8Bit("运动"), dlgDebugMode);
	QPushButton *SignleAxisStop = new QPushButton(QString::fromLocal8Bit("单轴停止"), dlgDebugMode);
	hLayout1->addWidget(label0);
	hLayout1->addWidget(CardSelect);
	hLayout1->addWidget(label1);
	hLayout1->addWidget(AxisSelect);
	hLayout1->addWidget(label2);
	hLayout1->addWidget(speed);
	hLayout1->addWidget(label3);
	hLayout1->addWidget(target);
	hLayout1->addWidget(label4);
	hLayout1->addWidget(MoveModeSelect);
	hLayout1->addWidget(Run);
	hLayout1->addWidget(SignleAxisStop);
	dlgDebugMode->setLayout(hLayout1);

	connect(Run, SIGNAL(clicked()), this, SLOT(slot_Run()));
	connect(SignleAxisStop, SIGNAL(clicked()), this, SLOT(slot_SignleAxisStop()));

	dlgDebugMode->show();
}

void JslCtrl::slot_Run()
{
	m_motion->axis_Move(CardSelect->currentIndex(), AxisSelect->currentIndex(), target->value(), speed->value(), MoveModeSelect->currentIndex(), true);
}

void JslCtrl::slot_SignleAxisStop()
{
	m_motion->StopAxis(CardSelect->currentIndex(), AxisSelect->currentIndex());
}

void JslCtrl::slot_loose()
{
	if (m_motion->bStarting)
	{
		QMessageBox::warning(this, "Warning", QString::fromLocal8Bit("机器正在工作，停止以后方可操作 "));
		return;
	}
	m_motion->loose();
}

void JslCtrl::slot_test()
{

}