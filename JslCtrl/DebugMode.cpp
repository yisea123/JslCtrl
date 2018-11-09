#include "DebugMode.h"
#include "JslCtrl.h"

DebugMode::DebugMode(QWidget *parent)
	:QDialog(parent)
{
	setWindowTitle(tr("DebugMode"));
	QVBoxLayout *mainlayout = new QVBoxLayout();
	QHBoxLayout *hLayout1 = new QHBoxLayout();
	QLabel *label0 = new QLabel(QString::fromLocal8Bit("选择卡-->"), this);

	CardSelect = new QComboBox(this);
	CardSelect->setFocusPolicy(Qt::NoFocus);
	for (int i = 0; i < 2/*m_motion->CardNum*/; i++)
		CardSelect->addItem(QString::number(i));

	QLabel *label1 = new QLabel(QString::fromLocal8Bit("选择轴-->"), this);
	AxisSelect = new QComboBox(this);
	AxisSelect->setFocusPolicy(Qt::NoFocus);
	for (int i = 0; i < 8; i++)
		AxisSelect->addItem(QString::number(i));

	QLabel *label2 = new QLabel(QString::fromLocal8Bit("速度-->"), this);
	speed = new QSpinBox(this);
	speed->setMinimumWidth(80);
	speed->setRange(0, 200000);

	QLabel *label3 = new QLabel(QString::fromLocal8Bit("目标-->"), this);
	target = new QSpinBox(this);
	target->setMinimumWidth(80);
	target->setRange(-2000000, 2000000);
	
	QLabel *label4 = new QLabel(QString::fromLocal8Bit("运动模式-->"), this);
	MoveModeSelect = new QComboBox(this);
	MoveModeSelect->setFocusPolicy(Qt::NoFocus);
	MoveModeSelect->addItem(QString::fromLocal8Bit("0(相对运动)"));
	MoveModeSelect->addItem(QString::fromLocal8Bit("1(绝对运动)"));
	MoveModeSelect->setCurrentIndex(1);

	QPushButton *Run = new QPushButton(QString::fromLocal8Bit("运动"), this);
	Run->setFocusPolicy(Qt::NoFocus);
	QPushButton *SignleAxisStop = new QPushButton(QString::fromLocal8Bit("单轴停止"), this);	
	SignleAxisStop->setFocusPolicy(Qt::NoFocus);
	QPushButton *RealseFocus = new QPushButton(QString::fromLocal8Bit("释放焦点"), this);
	RealseFocus->setFocusPolicy(Qt::NoFocus);
	//QPushButton *start
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
	hLayout1->addWidget(RealseFocus);
	

	QHBoxLayout *hLayout2 = new QHBoxLayout();
	c0a0 = new QLabel("c0a0");
	c0a1 = new QLabel("c0a1");
	c1 = new QList<QLabel *>();
	for (int i = 0; i < 8; i++)
		c1->append(new QLabel(QString("c1a%1").arg(i)));
	hLayout2->addWidget(c0a0);
	hLayout2->addWidget(c0a1);
	for (int i = 0; i < 8; i++)
		hLayout2->addWidget(c1->at(i));

	mainlayout->addLayout(hLayout1);
	mainlayout->addLayout(hLayout2);

	this->setLayout(mainlayout);

	connect(Run, SIGNAL(clicked()), this, SLOT(slot_Run()));
	connect(SignleAxisStop, SIGNAL(clicked()), this, SLOT(slot_SignleAxisStop()));
	connect(RealseFocus, SIGNAL(clicked()), this, SLOT(slot_RealseFocus()));
	QTimer *timer = new QTimer();
	connect(timer, SIGNAL(timeout()), this, SLOT(slot_UpdatePos()));
	timer->start(10);
	this->setFocus();
}

DebugMode::~DebugMode()
{}

void DebugMode::slot_Run()
{
	axis_Move(CardSelect->currentIndex(), AxisSelect->currentIndex(), target->value(), speed->value(), MoveModeSelect->currentIndex(), true);
}

void DebugMode::slot_SignleAxisStop()
{
	StopAxis(CardSelect->currentIndex(), AxisSelect->currentIndex());
}

void DebugMode::axis_Move(int card, int axis, int pos, int speed, int moveMode, bool bAck) {

	dmc_set_profile(card, axis, 0, speed, 0.1, 0.1, 0);
	
	if (card == 0)
	{
		dmc_set_profile(0, axis, 0, speed, 0.001, 0.5, 0);
		dmc_set_s_profile(0, axis, 0, 0.3);
	}

	if (int ErrorCode = dmc_pmove(card, axis, pos, moveMode))
		if (card == 1)
		{
			qDebug()<<(QString::fromLocal8Bit("轴运动出错，错误代码:%1").arg(ErrorCode));
		}

	qDebug()<<(QString::fromLocal8Bit("【运动】卡%1，轴%2，速度%3, 目标%4, 模式%5")
		.arg(card)
		.arg(axis)
		.arg(speed)
		.arg(pos)
		.arg(moveMode));
	qDebug() << "test";
}

void DebugMode::StopAxis(int card, int axis)
{
	dmc_stop(card, axis, 1);
	qDebug()<<(QString::fromLocal8Bit("【停止】卡%1，轴%2").arg(card).arg(axis));
}

void DebugMode::slot_UpdatePos()
{
	c0a0->setText(QString("c0a0\n%1").arg(dmc_get_position(0, 0)));
	c0a1->setText(QString("c0a1\n%1").arg(dmc_get_position(0, 1)));
	for (int i = 0; i < 8; i++)
		c1->at(i)->setText(QString("c1a%1\n%2").arg(i).arg(dmc_get_position(1,i)));	
}

void DebugMode::keyPressEvent(QKeyEvent *keyValue)
{
	if (keyValue->key() == Qt::Key_Up)
	{
		if (!keyValue->isAutoRepeat())
			axis_Move(0, 1, 0, 40000, 0, true);
	}
	else if (keyValue->key() == Qt::Key_Down)
	{
		if (!keyValue->isAutoRepeat())
			axis_Move(0, 1, 400000, 40000, 0, true);
	}
	else if (keyValue->key() == Qt::Key_Left)
	{
		if (!keyValue->isAutoRepeat())
			axis_Move(0, 0, 0, 40000, 0, true);
	}
	else if (keyValue->key() == Qt::Key_Right)
	{
		if (!keyValue->isAutoRepeat())
			axis_Move(0, 0, 400000, 40000, 0, true);
	}
}

void DebugMode::keyReleaseEvent(QKeyEvent *keyValue)
{
	if (keyValue->key() == Qt::Key_Up)
	{
		if (!keyValue->isAutoRepeat())
			StopAxis(0, 1);
	}
	else if (keyValue->key() == Qt::Key_Down)
	{
		if (!keyValue->isAutoRepeat())
			StopAxis(0, 1);
	}
	else if (keyValue->key() == Qt::Key_Left)
	{
		if (!keyValue->isAutoRepeat())
			StopAxis(0, 0);
	}
	else if (keyValue->key() == Qt::Key_Right)
	{
		if (!keyValue->isAutoRepeat())
			StopAxis(0, 0);
	}
}

void DebugMode::slot_RealseFocus()
{
	static int n = 0;
	n++;
	this->setFocus();
	if (n == 1)
	{
		slot_singleMove(14500);
	}
	if (n == 2)
	{
		slot_singleMove(14500);
	}
	if (n == 3)
	{
		slot_singleMove(14000);
	}
	if (n == 4)
	{
		slot_singleMove(14000);
		n = 0;
	}
}

bool DebugMode::statusCheck(int card, int axis) {
	mysleep(10);
	while (!dmc_check_done(card, axis)) {
		QApplication::processEvents();
	}

	return true;
}

void DebugMode::mysleep(unsigned int msec)
{
	QTime reachTime = QTime::currentTime().addMSecs(msec);
	while (QTime::currentTime() < reachTime)
		QApplication::processEvents();
}

void DebugMode::slot_singleMove(int a4pos)
{
	statusCheck(1, 2);
	statusCheck(1, 3);
	statusCheck(1, 4);

	axis_Move(1, 2, -7000, 15000, 1, true);
	axis_Move(1, 3, -14500, 15000, 1, true);

	statusCheck(1, 2);
	statusCheck(1, 3);
	statusCheck(1, 5);
	axis_Move(1, 4, a4pos, 50000, 1, true);

	statusCheck(1, 4);
	ResetAxis(1, 2, 15000);
	ResetAxis(1, 3, 15000);

	statusCheck(1, 2);
	statusCheck(1, 3);
	axis_Move(1, 4, 0, 50000, 1, true);
}

void DebugMode::ResetAxis(int card, int axis, int speed, int dir)
{
	dmc_set_profile(card, axis, 0, speed, 0.1, 0.1, 0);
	dmc_set_homemode(card, axis, dir, 1, 0, 0);
	dmc_home_move(card, axis);
}