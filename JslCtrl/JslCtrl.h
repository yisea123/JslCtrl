#pragma once

#include <QtWidgets/QMainWindow>
#include <QLayout>
#include <QLabel>
#include <qgroupbox.h>
#include <qradiobutton.h>
#include <QPushButton>
#include <QTextBrowser>
#include <QSpinBox>
#include <QComboBox>
#include <QTime>
#include <QDockWidget>
#include <QTableWidget>
#include <QMenu>
#include <QMenubar>
#include <qscrollarea.h>
#include <QLineEdit>
#include <qlistwidget.h>
#include <QKeyEvent>
#include <QButtonGroup>
#include "IOLabel.h"
#include "leisaimotion.h"
#include <QSettings>
#include "DebugMode.h"

class JslCtrl : public QMainWindow
{
	Q_OBJECT

public:
	JslCtrl(QWidget *parent = Q_NULLPTR);
	~JslCtrl();	
	
	leisaiMotion *m_motion;

	QLabel *warnningLineEdit;
	QLabel *showCurConfig;
	QPushButton *StartBtn;
	QPushButton *EmergencyStopOnBtn;
	QPushButton *EmergencyStopOffBtn;
	QPushButton *ResetBtn;

	void resetUI();
	//弹夹ID显示
	QLabel *idlist1;
	QLabel *idlist2;
	QLabel *idlist3;
	QPushButton *boxOkBtn;
	QPushButton *boxClearBtn;
	QStringList *labStrList;//弹夹ID准备队列
	bool bBoxWorking = false;

	QLabel *showRes;
	QLabel *showPannelid;
	QLabel *curSampleID;
	QLabel *processtime;
	QPushButton *pathLabel;
	QPushButton *md5label;
	QTime currTime = QTime::currentTime();
	QTime prevTime = QTime::currentTime();
	int recentKeyPointer = 0;
	int recentKey[3] = { 0 ,0, 0};

	QTableWidget *cmosTAB;

	IOLabel *InputIOStatus0;
	IOLabel *InputIOStatus1;
	IOLabel *InputIOStatus2;
	IOLabel *InputIOStatus3;
	IOLabel *InputIOStatus4;
	IOLabel *InputIOStatus5;
	IOLabel *InputIOStatus6;
	IOLabel *InputIOStatus7;
	IOLabel *InputIOStatus8;
	IOLabel *InputIOStatus9;
	IOLabel *InputIOStatus10;
	IOLabel *InputIOStatus11;
	IOLabel *InputIOStatus12;
	IOLabel *InputIOStatus13;
	IOLabel *OutputIOStatus0;
	IOLabel *OutputIOStatus1;
	IOLabel *OutputIOStatus2;
	IOLabel *OutputIOStatus3;
	IOLabel *OutputIOStatus4;

	QLabel *PicShow;
	QImage *img;

	QDockWidget *myDockWidget;
	QTextBrowser *myQTextBrowser;
	QDialog *ioDockWidget;

	QString strBoxNum;
	void keyPressEvent(QKeyEvent *);

	int row_coord;
	int col_coord;
	void cal(int ROW, int COL, int cnt_index);
	//Debug Mode Dialog
	QComboBox *CardSelect;
	QComboBox *AxisSelect;
	QSpinBox *speed;
	QSpinBox *target;
	QComboBox *MoveModeSelect;

public slots:
	void slot_LogAdd(const QString &str);
	void slot_warnningLog(const QString &str);
	void slot_Run();
	void slot_SignleAxisStop();
	void slot_StartBtn();
	void slot_StopBtn();
	void slot_test();
	void slot_RecImgPath(const QString &str);
	void slot_IO5Error();
	void slot_Setting();
	void slot_SaveDateBtn();
	void slot_DebugMode();
	void slot_updateBoxID(const QString&,int);
	void slot_RecRes(int,int,int,int, const  QString&, const QString&, const QString&);
	void slot_boxOkBtn();
	void slot_boxClearBtn();
	void slot_ResetBtn();
	void slot_resetBoxBtn();
	void slot_init();
	void slot_savepath();
	void slot_savemd5();
	void slot_ShowIOTab();
	void slot_areaCheck();
	void slot_EmergencyStopOff();
	void slot_dataSelect(int);//配置数据选择
	void slot_NewConfig();
	void slot_DelConfig();
	void slot_loose();

signals:
	void sigLogAdd(const QString &);
	void sigWarnningLogAdd(const QString&);
private:
	QSpinBox *speed_c0a0_box;
	QSpinBox *speed_c0a1_box;
	QSpinBox *speed_c1a0_box;
	QSpinBox *speed_c1a1_box;
	QSpinBox *speed_c1a2_box;
	QSpinBox *speed_c1a3_box;
	QSpinBox *speed_c1a4_box;
	QSpinBox *speed_c1a5_box;
	QSpinBox *speed_c1a6_box;
	QSpinBox *speed_c1a7_box;
	QSpinBox *pos_c0a0_box;
	QSpinBox *pos_c0a0_2_box;
	QSpinBox *pos_c0a0_3_box;
	QSpinBox *pos_c0a1_box;
	QSpinBox *pos_c1a0_box;
	QSpinBox *pos_c1a1_check_box;
	QSpinBox *pos_c1a1_work_box;
	QSpinBox *pos_c1a2_box;
	QSpinBox *pos_c1a3_box;
	QSpinBox *pos_c1a4_box;
	QSpinBox *pos_c1a5_box;
	QSpinBox *pos_c1a6_box;
	QSpinBox *pos_c1a7_box;
	QLineEdit *configNameEdit;
	QSpinBox *other_c0a0_StepDist_box;
	QSpinBox *other_c0a1_StepDist_box;
	QSpinBox *other_rowCMOS_box;
	QSpinBox *other_colCMOS_box;

	QAction *areaCheck;

	QDialog *dlgSetting;//配置对话框，保存后自动关闭
	QListWidget *configList;//配置列表
	
	QStringList strlistConfigName;
	int iCurrentConfigNum = 0;//当前配置序号，从来开始，在strlistConfigName中查找，存储在ini文件中
	QSettings *curSettings;
};
