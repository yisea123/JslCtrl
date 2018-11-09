#pragma once
#ifndef LEISAIMOTION_H
#define LEISAIMOTION_H

#include <QObject>
#include "globaldefine.h"
#include <QApplication>
#include <qthread.h>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTIMER>
#include <QProcess>
#include <QMessageBox>
#include "interface.h"
#include "./rapidjson/rapidjson.h"
#include "./rapidjson/document.h"

using namespace rapidjson;

class leisaiMotion : public QThread
{
	Q_OBJECT

public:
	leisaiMotion();
	~leisaiMotion();

	void Init(char* configPath);
	bool LoadConfig(const QString& str);

	HTTP_Interface *m_HTTP_Interface;

	//short m_bBoardStatus = 1;
	int CardNum = 0;
	bool bInited = false;
	bool bStarting = false;//判断是否正在工作
	bool bNeedReset = true;//判断是否需要复位
	bool bES = false;//判断急停状态
	bool bReceived = false;//判断是否收到服务器返回信息
	
	bool statusCheck(int card, int axis);
	
	void ResetAxis(int card, int axis, int speed,int dir=1);//回零
	void set0pos(int card, int axis);
	void mysleep(unsigned int msec);
	void loose();

	//*******数据库配置内容*******
	int speed_c0a0;
	int speed_c0a1;
	int speed_c1a0;
	int speed_c1a1;
	int speed_c1a2;
	int speed_c1a3;
	int speed_c1a4;
	int speed_c1a5;
	int speed_c1a6;
	int speed_c1a7;
	int pos_c0a0;
	int pos_c0a0_2;
	int pos_c0a0_3;
	int pos_c0a1;
	int pos_c1a0;
	int pos_c1a1_check;
	int pos_c1a1_work;
	int pos_c1a2;
	int pos_c1a3;
	int pos_c1a4;
	int pos_c1a5;
	int pos_c1a6;
	int pos_c1a7;	
	int pos_c0a0_StepDist;//C0A0步距
	int pos_c0a1_StepDist;//C0A1步距
	int rowCMOS;//CMOS摆放行数
	int colCMOS;//CMOS摆放列数
	//**********************

	//*********数据库记录内容*********
	QStringList box_id;
	int pannel_id = 1;
	int sample_id = 0;
	QString machine_id="008";
	QString time;
	QString opreator = "9527";
	QString shift_id = QString::fromLocal8Bit("白班");
	QString md5;
	QString process_res;
	QString process_time;
	QString img_path = "img_path";
	QString raw_image_path = "raw_image_path";
	QString pos_ok = "pos_ok";
	QString config;
	//********************************

	//右上角为1，1，之后需要转换
	int row;
	int col;

	//左上角为1，1
	int currow;
	int curcol;

public slots:
	/*moveMode: 0 相对移动， 1 绝对移动*/
	void axis_Move(int card, int axis, int pos, int speed, int moveMode, bool bAck = false);
	void StopAxis(int card, int axis);
	void slot_EmergencyStopOn();
	void slot_EmergencyStopOff();
	void slot_ResetBtn();
	void slot_LoadGoods();
	void run();
	void slot_test();
	void slot_singleMove(int);
	void slot_MoveTestBtn();
	void slot_C0Work(int,int,int);//int1:行走列数; int2:列坐标计算; int3:下一步C0A0初始位置
	void CtlCam();
	QString onReply(QNetworkReply *);
	//void slot_ReleaseGoods();
	void areaCheck(bool);

signals:
	void sigSendRes(int,int,int,int, const QString &protime, const QString &md5, const  QString& rawpath);//int1:pannelid为0时重置界面
	void sigLogAdd(const QString &str);
	void sigWarnningLogAdd(const QString&);
	void sigSendImgPath(const QString &path);
	void sig_updateBoxID(const QString &str,int);//1(正在工作):yellow; 0(完成):lime
	void sig_resetBoxBtn();//更新盒子按钮，使UI弹夹队列接收ID
};

#endif // LEISAIMOTION_H
