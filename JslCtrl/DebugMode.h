#pragma once
#include "globaldefine.h"
#include "qobject.h"
#include <qdialog.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include "qkeyeventtransition.h"

class DebugMode :public QDialog
{
	Q_OBJECT;

public:
	DebugMode(QWidget *parent = Q_NULLPTR);
	~DebugMode();
private:
	QComboBox *CardSelect;
	QComboBox *AxisSelect;
	QComboBox *MoveModeSelect;
	QSpinBox *speed;
	QSpinBox *target;
	QLabel *c0a0;
	QLabel *c0a1;
	QList<QLabel *> *c1;
	void keyPressEvent(QKeyEvent *);
	void keyReleaseEvent(QKeyEvent *);
	bool keyPressed = false;
	void axis_Move(int card, int axis, int pos, int speed, int moveMode, bool bAck);
	void StopAxis(int card, int axis);
	bool statusCheck(int card, int axis);
	void mysleep(unsigned int);
	void ResetAxis(int card, int axis, int speed, int dir=1);
	void slot_singleMove(int a4pos);
private slots :
	void slot_Run();
	void slot_SignleAxisStop();
	void slot_UpdatePos();
	void slot_RealseFocus();	
};