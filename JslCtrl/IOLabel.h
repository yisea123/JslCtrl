#pragma once
#include "globaldefine.h"
#include <QLabel>
#include <QTIMER>

class IOLabel : public QLabel
{
	Q_OBJECT

public:
	IOLabel(const QString &text, bool bInput, short port, QWidget *parent = Q_NULLPTR);
	~IOLabel();
	bool m_bInput;				//判断IO类别是IN还是OUT
	short m_port;
	short m_on_off;
	short lastStatus;			//之前IO状态，与当前IO状态比较，检测IO状态变化
	bool bJerk = false;			//判断是否急停

	//输出IO鼠标单击切换状态
	void mouseReleaseEvent(QMouseEvent*);

	short GetIOStatus();
	void SetOutputStatus(short on_off);
public slots:
	//更新IO状态，IO状态改变时发射状态变化信号
	void slot_IOInit();
	void slot_IOJerkOn();
	void slot_IOJerkOff();

signals:
	//IO状态变化：从0到1
	void sig_ZeroToOne();
	//IO状态变化：从1到0
	void sig_OneToZero();
	
};