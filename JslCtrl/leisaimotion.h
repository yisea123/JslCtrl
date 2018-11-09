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
	bool bStarting = false;//�ж��Ƿ����ڹ���
	bool bNeedReset = true;//�ж��Ƿ���Ҫ��λ
	bool bES = false;//�жϼ�ͣ״̬
	bool bReceived = false;//�ж��Ƿ��յ�������������Ϣ
	
	bool statusCheck(int card, int axis);
	
	void ResetAxis(int card, int axis, int speed,int dir=1);//����
	void set0pos(int card, int axis);
	void mysleep(unsigned int msec);
	void loose();

	//*******���ݿ���������*******
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
	int pos_c0a0_StepDist;//C0A0����
	int pos_c0a1_StepDist;//C0A1����
	int rowCMOS;//CMOS�ڷ�����
	int colCMOS;//CMOS�ڷ�����
	//**********************

	//*********���ݿ��¼����*********
	QStringList box_id;
	int pannel_id = 1;
	int sample_id = 0;
	QString machine_id="008";
	QString time;
	QString opreator = "9527";
	QString shift_id = QString::fromLocal8Bit("�װ�");
	QString md5;
	QString process_res;
	QString process_time;
	QString img_path = "img_path";
	QString raw_image_path = "raw_image_path";
	QString pos_ok = "pos_ok";
	QString config;
	//********************************

	//���Ͻ�Ϊ1��1��֮����Ҫת��
	int row;
	int col;

	//���Ͻ�Ϊ1��1
	int currow;
	int curcol;

public slots:
	/*moveMode: 0 ����ƶ��� 1 �����ƶ�*/
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
	void slot_C0Work(int,int,int);//int1:��������; int2:���������; int3:��һ��C0A0��ʼλ��
	void CtlCam();
	QString onReply(QNetworkReply *);
	//void slot_ReleaseGoods();
	void areaCheck(bool);

signals:
	void sigSendRes(int,int,int,int, const QString &protime, const QString &md5, const  QString& rawpath);//int1:pannelidΪ0ʱ���ý���
	void sigLogAdd(const QString &str);
	void sigWarnningLogAdd(const QString&);
	void sigSendImgPath(const QString &path);
	void sig_updateBoxID(const QString &str,int);//1(���ڹ���):yellow; 0(���):lime
	void sig_resetBoxBtn();//���º��Ӱ�ť��ʹUI���ж��н���ID
};

#endif // LEISAIMOTION_H
