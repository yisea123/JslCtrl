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
	bool m_bInput;				//�ж�IO�����IN����OUT
	short m_port;
	short m_on_off;
	short lastStatus;			//֮ǰIO״̬���뵱ǰIO״̬�Ƚϣ����IO״̬�仯
	bool bJerk = false;			//�ж��Ƿ�ͣ

	//���IO��굥���л�״̬
	void mouseReleaseEvent(QMouseEvent*);

	short GetIOStatus();
	void SetOutputStatus(short on_off);
public slots:
	//����IO״̬��IO״̬�ı�ʱ����״̬�仯�ź�
	void slot_IOInit();
	void slot_IOJerkOn();
	void slot_IOJerkOff();

signals:
	//IO״̬�仯����0��1
	void sig_ZeroToOne();
	//IO״̬�仯����1��0
	void sig_OneToZero();
	
};