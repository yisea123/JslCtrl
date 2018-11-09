#include "IOLabel.h"

IOLabel::IOLabel(const QString &text, bool bInput, short port, QWidget *parent):
	m_bInput(bInput),m_port(port)
{
	setParent(parent);
	setText(text);
	setMinimumSize(60, 40);
	setAlignment(Qt::AlignCenter);

	if (m_bInput == true)
		lastStatus = dmc_read_inbit(1, m_port);
	else
		lastStatus = dmc_read_outbit(1, m_port);

	QTimer *Timer = new QTimer(this);
	Timer->start(10);
	connect(Timer, SIGNAL(timeout()), this, SLOT(slot_IOInit()));
}


IOLabel::~IOLabel()
{
	
}

void IOLabel::mouseReleaseEvent(QMouseEvent*)
{
	if (m_bInput == false)
	{
		m_on_off = !m_on_off;
		dmc_write_outbit(1, m_port, m_on_off);
	}
	
}

void IOLabel::slot_IOInit()
{
	if (m_bInput == true)
		m_on_off = dmc_read_inbit(1, m_port);
	else
		m_on_off = dmc_read_outbit(1, m_port);

	//IO状态改变
	if (lastStatus != m_on_off)
	{	
		if (lastStatus == 0)
			QTimer::singleShot(0, this, SIGNAL(sig_ZeroToOne()));//IO从0到1	
		else
			QTimer::singleShot(0, this, SIGNAL(sig_OneToZero()));//IO从1到0
	}	

	lastStatus = m_on_off;

	if (m_on_off == 0){
		setStyleSheet("background:lime");
	}
	else {
		setStyleSheet("background:red;height:3px;");
	}

		
}

short IOLabel::GetIOStatus()
{
	if (m_bInput == true)
		return dmc_read_inbit(1, m_port);		
	else
		return dmc_read_outbit(1, m_port);
}

void IOLabel::SetOutputStatus(short on_off)
{
	if (!bJerk)
		if (m_bInput == false)
			dmc_write_outbit(1, m_port, on_off);
}

void IOLabel::slot_IOJerkOn()
{
	bJerk = true;
}

void IOLabel::slot_IOJerkOff()
{
	bJerk = false;
}