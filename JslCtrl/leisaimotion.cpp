#include "leisaimotion.h"

leisaiMotion::leisaiMotion()
	: m_HTTP_Interface(new HTTP_Interface)
{
	
	connect(m_HTTP_Interface->m_pManager, SIGNAL(finished(QNetworkReply *)), this, SLOT(onReply(QNetworkReply *)));
}

leisaiMotion::~leisaiMotion()
{
	//qDebug() << "!!";
	dmc_board_close();
}

void leisaiMotion::Init(char* configFilePath)
{
	emit sigWarnningLogAdd(QString::fromLocal8Bit("正在初始化"));
	emit sigLogAdd(QString::fromLocal8Bit("正在初始化"));
	CardNum = dmc_board_init();
	if (CardNum == 0)
	{
		emit sigWarnningLogAdd(QString::fromLocal8Bit("没有找到控制卡"));
		emit sigLogAdd(QString::fromLocal8Bit("没有找到控制卡"));
		return;
	}
	if (CardNum != 2)
	{
		emit sigWarnningLogAdd(QString::fromLocal8Bit("控制卡数量异常"));
		emit sigLogAdd(QString::fromLocal8Bit("控制卡数量异常"));
		return;
	}
	//qDebug() << dmc_download_configfile(1, configFilePath);
	for (int card = 0; card < CardNum; card++)
	{
		if (int errorCode = dmc_download_configfile(card, configFilePath))
		{
			emit sigWarnningLogAdd(QString::fromLocal8Bit("控制卡卡%1配置文件载入失败，错误代码：%2").arg(card).arg(errorCode));
			emit sigLogAdd(QString::fromLocal8Bit("控制卡卡%1配置文件载入失败，错误代码：%2").arg(card).arg(errorCode));
			return;
		}
		for (int axis = 0; axis < 8; axis++)
			if (dmc_write_sevon_pin(card, axis, 0))
			{
				emit sigWarnningLogAdd(QString::fromLocal8Bit("卡%1轴%2的伺服使能端口输出设置失败").arg(card).arg(axis));
				emit sigLogAdd(QString::fromLocal8Bit("卡%1轴%2的伺服使能端口输出设置失败").arg(card).arg(axis));
				return;
			}
	}
	
	/*if (!LoadConfig())
	{
		emit sigWarnningLogAdd(QString::fromLocal8Bit("载入配置失败"));
		emit sigLogAdd(QString::fromLocal8Bit("载入配置失败"));
		return;
	}*/
	
	QString strSend("{\"init\":\"1\"}");
	emit sigLogAdd(tr("sent init post"));
	for (int i = 0; i < 4; i++)
	{
		if (!bReceived)
		{
			m_HTTP_Interface->sendPost(strSend.toLatin1(), QUrl("http://127.0.0.1:6789/init"), 6789);
			mysleep(2500);
		}
	}
	if (!bReceived)
	{
		emit sigWarnningLogAdd(QString::fromLocal8Bit("初始化失败"));
		emit sigLogAdd(QString::fromLocal8Bit("初始化失败"));
		return;
	}

	emit sigWarnningLogAdd(QString::fromLocal8Bit("初始化成功"));
	emit sigLogAdd(QString::fromLocal8Bit("初始化成功"));
	bInited = true;
}

bool leisaiMotion::LoadConfig(const QString& str)
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
		return false;
	}
	else
	{
		QSqlQuery query;

		query.exec(QString("SELECT speed_c0a0,speed_c0a1,speed_c1a0,speed_c1a1,speed_c1a2,speed_c1a3,speed_c1a4,speed_c1a5,speed_c1a6,speed_c1a7,pos_c0a0,pos_c0a0_2,pos_c0a0_3,pos_c0a1,pos_c1a0,pos_c1a1_check,pos_c1a1_work,pos_c1a2,pos_c1a3,pos_c1a4,pos_c1a5,pos_c1a6,pos_c1a7,pos_c0a0_StepDist,pos_c0a1_StepDist,rowCMOS,colCMOS FROM CONFIG WHERE ConfigName='%1'").arg(str));
		query.next();
		speed_c0a0 = query.value(0).toInt();
		speed_c0a1 = query.value(1).toInt();
		speed_c1a0 = query.value(2).toInt();
		speed_c1a1 = query.value(3).toInt();
		speed_c1a2 = query.value(4).toInt();
		speed_c1a3 = query.value(5).toInt();
		speed_c1a4 = query.value(6).toInt();
		speed_c1a5 = query.value(7).toInt();
		speed_c1a6 = query.value(8).toInt();
		speed_c1a7 = query.value(9).toInt();
		pos_c0a0 = query.value(10).toInt();
		pos_c0a0_2 = query.value(11).toInt();
		pos_c0a0_3 = query.value(12).toInt();
		pos_c0a1 = query.value(13).toInt();
		pos_c1a0 = query.value(14).toInt();
		pos_c1a1_check = query.value(15).toInt();
		pos_c1a1_work = query.value(16).toInt();
		pos_c1a2 = query.value(17).toInt();
		pos_c1a3 = query.value(18).toInt();
		pos_c1a4 = query.value(19).toInt();
		pos_c1a5 = query.value(20).toInt();
		pos_c1a6 = query.value(21).toInt();
		pos_c1a7 = query.value(22).toInt();
		pos_c0a0_StepDist = query.value(23).toInt();
		pos_c0a1_StepDist = query.value(24).toInt();
		rowCMOS = query.value(25).toInt();
		colCMOS = query.value(26).toInt();

		db.close();
		return true;
	}
}

void leisaiMotion::axis_Move(int card,int axis,int pos,int speed,int moveMode,bool bAck){
	emit sigWarnningLogAdd(QString::fromLocal8Bit(" "));

	if (bES)
	{
		emit sigWarnningLogAdd(QString::fromLocal8Bit("急停中"));
		emit sigLogAdd(QString::fromLocal8Bit("急停中"));
		return;
	}


	/*if (m_bBoardStatus)
	{
		emit sigWarnningLogAdd(QString::fromLocal8Bit("初始化失败，错误代码：%1").arg(m_bBoardStatus));
		emit sigLogAdd(QString::fromLocal8Bit("初始化失败，错误代码：%1").arg(m_bBoardStatus));
		return;
	}*/
	

	if (bAck)
		if (!dmc_check_done(card, axis))
		{
			emit sigWarnningLogAdd(QString::fromLocal8Bit("卡%1轴%2正在运行").arg(card).arg(axis));
			emit sigLogAdd(QString::fromLocal8Bit("卡%1轴%2正在运行").arg(card).arg(axis));
			return;
		}

	if (int ErrorCode = dmc_set_profile(card, axis, 0, speed, 0.1, 0.1, 0))
	{
		emit sigWarnningLogAdd(QString::fromLocal8Bit("速度设置出错，错误代码:%1").arg(ErrorCode));
		emit sigLogAdd(QString::fromLocal8Bit("速度设置出错，错误代码:%1").arg(ErrorCode));
		return;
	}

	if (card == 0)
	{
		dmc_set_profile(0, axis, 0, speed, 0.001, 0.5, 0);
		dmc_set_s_profile(0, axis, 0, 0.3);
	}

	if(int ErrorCode = dmc_pmove(card,axis,pos,moveMode))
		if (card == 1)
		{
			emit sigWarnningLogAdd(QString::fromLocal8Bit("轴运动出错，错误代码:%1").arg(ErrorCode));
			emit sigLogAdd(QString::fromLocal8Bit("轴运动出错，错误代码:%1").arg(ErrorCode));
		}
	
	emit sigLogAdd(QString::fromLocal8Bit("【运动】卡%1，轴%2，速度%3, 目标%4, 模式%5")
											.arg(card)
											.arg(axis)
											.arg(speed)
											.arg(pos)
											.arg(moveMode));
}

void leisaiMotion::StopAxis(int card,int axis)
{
	dmc_stop(card, axis, 1);
	emit sigLogAdd(QString::fromLocal8Bit("【停止】卡%1，轴%2").arg(card).arg(axis));
}

bool leisaiMotion::statusCheck(int card, int axis){
	mysleep(10);
	while (!dmc_check_done(card, axis)) {
		//mysleep(10);
		if (bES) return false;//ES SIG
		QApplication::processEvents();
	}

	return true;
}

void leisaiMotion::ResetAxis(int card, int axis,int speed,int dir)
{
	dmc_set_profile(card, axis, 0, speed, 0.1, 0.1, 0);
	dmc_set_homemode(card, axis, dir, 1, 0, 0);
	dmc_home_move(card, axis);
	emit sigLogAdd(QString::fromLocal8Bit("【回零】卡%1轴%2，速度%3，方向%4").arg(card).arg(axis).arg(speed).arg(dir));
}

void leisaiMotion::set0pos(int card, int axis)
{
	dmc_set_position(card, axis, 0);
}

void leisaiMotion::slot_test()
{
	
	//CtlCam();
	return;
}

void leisaiMotion::slot_EmergencyStopOn()
{
	for (int i = 0; i<CardNum; i++)
		dmc_emg_stop(i);
	bES = true;
	dmc_write_outbit(1, 4, 1);//关真空气阀
	bStarting = false;
	emit sigWarnningLogAdd(QString::fromLocal8Bit("急停已开启"));
	emit sigLogAdd(QString::fromLocal8Bit("急停已开启"));
}

void leisaiMotion::slot_EmergencyStopOff()
{
	bES = false;
	bNeedReset = true;
	emit sigWarnningLogAdd(QString::fromLocal8Bit("解除急停"));
	emit sigLogAdd(QString::fromLocal8Bit("解除急停"));
}

void leisaiMotion::slot_ResetBtn()
{
	box_id.clear();		

	if (dmc_read_inbit(1,4) == 0)
	{
		emit sigWarnningLogAdd(QString::fromLocal8Bit("左侧异物"));
		emit sigLogAdd(QString::fromLocal8Bit("左侧异物"));
		return;
	}

	if (dmc_read_inbit(1, 13) == 0)
	{
		emit sigWarnningLogAdd(QString::fromLocal8Bit("右侧异物"));
		emit sigLogAdd(QString::fromLocal8Bit("右侧异物"));
		return;
	}

	if ((dmc_read_inbit(1, 2) == 1)&& (dmc_read_outbit(1, 0) == 1))
	{
		emit sigWarnningLogAdd(QString::fromLocal8Bit("左侧压缸未松，请手扶弹夹后打开IOTAB点击OUTIO 0"));
		emit sigLogAdd(QString::fromLocal8Bit("左侧压缸未松，请手扶弹夹后打开IOTAB点击OUTIO 0"));
		return;
	}

	if ((dmc_read_inbit(1, 10) == 1)&& (dmc_read_outbit(1, 2) == 1))
	{	
		emit sigWarnningLogAdd(QString::fromLocal8Bit("右侧压缸未松，请手扶弹夹后打开IOTAB点击OUTIO 2"));
		emit sigLogAdd(QString::fromLocal8Bit("右侧压缸未松，请手扶弹夹后打开IOTAB点击OUTIO 2"));
		return;

	}

	dmc_write_outbit(1, 0, 0);
	dmc_write_outbit(1, 2, 0);
	dmc_write_outbit(1, 1, 1);
	dmc_write_outbit(1, 3, 1);
	dmc_write_outbit(1, 4, 1);

	axis_Move(0, 0, -400000, speed_c0a0, 0, true);
	axis_Move(0, 1, -400000, speed_c0a1, 0, true);
	ResetAxis(1, 1, speed_c1a1 / 2);
	ResetAxis(1, 2, speed_c1a2);
	ResetAxis(1, 3, speed_c1a3);
	ResetAxis(1, 4, speed_c1a4);
	ResetAxis(1, 5, speed_c1a5,0);
	ResetAxis(1, 7, speed_c1a7 / 2);
	statusCheck(1, 1);
	ResetAxis(1, 0, speed_c1a0 / 2);

	statusCheck(0, 0);
	statusCheck(0, 1);
	statusCheck(1, 0);
	statusCheck(1, 1);
	statusCheck(1, 2);
	statusCheck(1, 3);
	statusCheck(1, 4);
	statusCheck(1, 5);
	statusCheck(1, 7);

	set0pos(0, 0);
	set0pos(0, 1);
	set0pos(1, 0);
	set0pos(1, 1);
	set0pos(1, 2);
	set0pos(1, 3);
	set0pos(1, 4);
	set0pos(1, 5);
	set0pos(1, 7);

	
	sample_id = 0;
	bNeedReset = false;
	bStarting = false;
	bReceived = false;
	emit sigWarnningLogAdd(QString::fromLocal8Bit("复位完成"));
	emit sigLogAdd(QString::fromLocal8Bit("复位完成"));
}

void leisaiMotion::run()
{
	bStarting = true;
	axis_Move(0, 0, pos_c0a0, speed_c0a0, 1, true);
	axis_Move(0, 1, pos_c0a1, speed_c0a1, 1, true);
	axis_Move(1, 0, pos_c1a0, speed_c1a0, 1, true);
	axis_Move(1, 7, pos_c1a7, speed_c1a7, 1, true);
	statusCheck(1, 0);
	statusCheck(1, 7);

	slot_LoadGoods();
}

void leisaiMotion::slot_LoadGoods()
{
	while (!bES)
	{
		if (box_id.size() == 0)
		{
			emit sigWarnningLogAdd(QString::fromLocal8Bit("弹夹ID空，请扫弹夹码"));
			emit sigLogAdd(QString::fromLocal8Bit("弹夹ID空，请扫弹夹码"));
		}
		while (box_id.size() == 0)
		{
			mysleep(10);
			if (bES) return;//ES SIG
		}

		emit sig_updateBoxID(box_id.first(),1);//发送正在工作ID

		
		if (dmc_read_inbit(1, 2) == 0)
		{
			emit sigWarnningLogAdd(QString::fromLocal8Bit("上料盒缺，请按扫码次序补充上料盒"));
			emit sigLogAdd(QString::fromLocal8Bit("上料盒缺，请按扫码次序补充上料盒"));
		}
		while (dmc_read_inbit(1,2) == 0)
		{
			mysleep(10);
			if (bES) return;//ES SIG
		}

		
		if (dmc_read_inbit(1, 11) == 1)
		{
			emit sigWarnningLogAdd(QString::fromLocal8Bit("下料盒缺，请补充空盒"));
			emit sigLogAdd(QString::fromLocal8Bit("下料盒缺，请补充空盒"));
		}
		while (dmc_read_inbit(1, 11) == 1)
		{
			mysleep(10);
			if (bES) return;//ES SIG
		}

		emit sigWarnningLogAdd(QString::fromLocal8Bit(" "));
		
		for (pannel_id = 1; pannel_id <= 10; pannel_id++)
		{
			statusCheck(1, 1);
			set0pos(1, 1);

			if (pannel_id != 1)
			{
				statusCheck(1, 7);
				if (dmc_read_inbit(1, 13) == 0)
				{
					emit sigWarnningLogAdd(QString::fromLocal8Bit("载板右侧未推到位，请处理"));
					emit sigLogAdd(QString::fromLocal8Bit("载板右侧未推到位，请处理"));
				}
				while (dmc_read_inbit(1, 13) == 0)
				{
					mysleep(10);
					if (bES) return;//ES SIG
				}
				emit sigWarnningLogAdd(QString::fromLocal8Bit(" "));
				axis_Move(1, 7, 60000, speed_c1a7, 0, true);
			}
			
			statusCheck(1, 0);
			axis_Move(1, 0, -60000, speed_c1a0, 0, true);
			statusCheck(1, 0);
			statusCheck(1, 7);

			statusCheck(1, 1);
			axis_Move(1, 1, pos_c1a1_check, speed_c1a1, 1, true);
			statusCheck(1, 1);

			if (bES) return;//ES SIG
			if (dmc_read_inbit(1, 4) == 0)
			{
				axis_Move(1, 1, pos_c1a1_work, speed_c1a1, 1, true);
				statusCheck(1, 1);
				ResetAxis(1, 1, speed_c1a1 / 2);
				slot_MoveTestBtn();				//进入下一步处理
			}
			else
			{
				if (bES) return;//ES SIG
				ResetAxis(1, 1, speed_c1a1 / 2);
				emit sigLogAdd(QString::fromLocal8Bit("载板空，切换下一工作位"));
			}

			if (bES) return;//ES SIG
			if (dmc_read_inbit(1, 4) == 0)
			{
				emit sigWarnningLogAdd(QString::fromLocal8Bit("载板未推到位，请处理"));
				emit sigLogAdd(QString::fromLocal8Bit("载板未推到位，请处理"));
			}
			while (dmc_read_inbit(1, 4) == 0)
			{
				mysleep(10);
				if (bES) return;//ES SIG
			}
			emit sigWarnningLogAdd(QString::fromLocal8Bit(" "));
		}

		emit sig_updateBoxID(box_id.first(), 0);//此盒子完成，颜色更新为绿色
		box_id.removeFirst();
		if (box_id.size() == 0)//操作队列用完后使主界面可重新添加队列
			emit sig_resetBoxBtn();

		statusCheck(1, 1);
		statusCheck(1, 0);
		axis_Move(1, 0, pos_c1a0 - 700000, speed_c1a0, 1, true);//头顶的盒子刚好处于工作位
		axis_Move(1, 7, pos_c1a7 - 160000, speed_c1a7, 1, true);
		statusCheck(1, 7);
		statusCheck(1, 0);
		mysleep(100);
		if (bES) return;//ES SIG
		dmc_write_outbit(1, 0, 1);//夹紧上料盒
		dmc_write_outbit(1, 2, 1);//夹紧下料盒
		mysleep(100);
		ResetAxis(1, 0, speed_c1a0 / 2);
		ResetAxis(1, 7, speed_c1a7 / 2);
		statusCheck(1, 0);
		statusCheck(1, 7);
		set0pos(1, 0);
		set0pos(1, 7);

		if (bES) return;//ES SIG
		if (dmc_read_inbit(1, 3) == 0)
		{
			emit sigWarnningLogAdd(QString::fromLocal8Bit("上料盒满，请拿走左边空盒"));
			emit sigLogAdd(QString::fromLocal8Bit("上料盒满，请拿走左边空盒"));
		}
		while (dmc_read_inbit(1, 3) == 0)
		{
			mysleep(10);
			if (bES) return;//ES SIG
		}
		emit sigWarnningLogAdd(QString::fromLocal8Bit(" "));

		if (bES) return;//ES SIG
		if (dmc_read_inbit(1, 12) == 0)
		{
			emit sigWarnningLogAdd(QString::fromLocal8Bit("下料盒满，请拿走右边空盒"));
			emit sigLogAdd(QString::fromLocal8Bit("下料盒满，请拿走右边空盒"));
		}
		while (dmc_read_inbit(1, 12) == 0)
		{
			mysleep(10);
			if (bES) return;//ES SIG
		}
		emit sigWarnningLogAdd(QString::fromLocal8Bit(" "));

		if (bES) return;//ES SIG
		dmc_write_outbit(1, 1, 0);//推上料盒
		dmc_write_outbit(1, 3, 0);//推走下料盒
		while (true)
		{
			mysleep(10);
			if (bES) return;//ES SIG
			if (dmc_read_inbit(1,1) == 0 && dmc_read_inbit(1, 8) == 0)
				break;
		}
		if (bES) return;//ES SIG
		dmc_write_outbit(1, 1, 1);
		dmc_write_outbit(1, 3, 1);

		axis_Move(1, 0, pos_c1a0, speed_c1a0, 1, true);
		axis_Move(1, 7, pos_c1a7 + 540000, speed_c1a7, 1, true);
		statusCheck(1, 7);
		statusCheck(1, 0);
		if (bES) return;//ES SIG
		dmc_write_outbit(1, 0, 0);
		dmc_write_outbit(1, 2, 0);
		axis_Move(1, 7, pos_c1a7, speed_c1a7, 1, true);
		statusCheck(1, 7);
	}
}

void leisaiMotion::slot_MoveTestBtn()
{
	if (bES) return;//ES SIG

	//判断3步中每一步行走列数策略
	int c0work = colCMOS / 3;
	int c0work_mid = colCMOS / 3;
	int c0work_last = colCMOS / 3;
	if (colCMOS % 3 == 1)
	{
		c0work +=1;		
	}
	else if (colCMOS % 3 == 2)
	{
		c0work += 1;
		c0work_mid += 1;
	}

	statusCheck(1, 7);
	emit sigSendRes(0,0, 0, 0, " "," ", " ");
	slot_singleMove(pos_c1a4);
	slot_singleMove(pos_c1a4);
	if (bES) return;//ES SIG
	slot_C0Work(c0work,0,pos_c0a0_2);

	slot_singleMove(14000);
	if (bES) return;//ES SIG
	slot_C0Work(c0work_mid, c0work,pos_c0a0_3);

	slot_singleMove(14000);
	if (bES) return;//ES SIG
	slot_C0Work(c0work_last, c0work+c0work_mid,pos_c0a0);
	sample_id = 0;
	

	slot_singleMove(pos_c1a4);
	slot_singleMove(pos_c1a4);
	slot_singleMove(pos_c1a4);
}

void leisaiMotion::slot_singleMove(int a4pos)
{
	if (bES) return;//ES SIG

	statusCheck(1, 2);
	statusCheck(1, 3);
	statusCheck(1, 4);
	set0pos(1, 2);
	set0pos(1, 3);
	set0pos(1, 4);

	axis_Move(1, 2, pos_c1a2, speed_c1a2, 1, true);
	axis_Move(1, 3, pos_c1a3, speed_c1a3, 1, true);

	statusCheck(1, 2);
	statusCheck(1, 3);
	statusCheck(1, 5);
	axis_Move(1, 4, a4pos, speed_c1a4, 1, true);

	statusCheck(1, 4);
	ResetAxis(1, 2, speed_c1a2);
	ResetAxis(1, 3, speed_c1a3);

	statusCheck(1, 2);
	statusCheck(1, 3);
	//ResetAxis(1, 4, speed_c1a4);
	axis_Move(1, 4, 0, speed_c1a4, 1, true);
}

void leisaiMotion::slot_C0Work(int c0work,int offset,int nextc0a0pos)
{
	statusCheck(0, 1);
	statusCheck(0, 0);
	statusCheck(1, 5);
	set0pos(1, 5);
	axis_Move(1, 5, pos_c1a5, speed_c1a5, 1, true);
	statusCheck(1, 5);
	if (bES) return;//ES SIG
	dmc_write_outbit(1, 4, 0);//开真空气阀

	//emit sigLogAdd("[1,1]");
	
	row = 1;			//以右上角为1,1坐标的行号
	col = 1 + offset;	//以右上角为1,1坐标的列号
	CtlCam();

	int i = 1;
	int j = 1;

	for (i = 1; i <= c0work; i++)
	{
		for (j = 1; j <= rowCMOS - 1; j++)
		{
			if (bES) return;//ES SIG
			if (i % 2 != 0)
				axis_Move(0, 1, pos_c0a1_StepDist, speed_c0a1, 0, true);
			else
				axis_Move(0, 1, pos_c0a1_StepDist*-1, speed_c0a1, 0, true);

			statusCheck(0, 1);
			statusCheck(0, 0);

			if (bES) return;//ES SIG
			if (i % 2 != 0)
			{
				//emit sigLogAdd(tr("[%1,%2]").arg(j + 1).arg(i));
				row = j + 1;
				col = i + offset;
			}
			else
			{
				//emit sigLogAdd(tr("[%1,%2]").arg(rowCMOS - j).arg(i));
				row = rowCMOS - j;
				col = i + offset;
			}
			
			CtlCam();
		}

		if (i != c0work)
		{
			if (bES) return;//ES SIG
			axis_Move(0, 0, pos_c0a0_StepDist*-1, speed_c0a0, 0, true);
			statusCheck(0, 0);
			statusCheck(0, 1);
			if (bES) return;//ES SIG
			if (i % 2 != 0)
			{
				//emit sigLogAdd(tr("[%1,%2]").arg(rowCMOS).arg(i + 1));
				row = rowCMOS;
				col = i + 1 + offset;
			}
			else
			{
				//emit sigLogAdd(tr("[%1,%2]").arg(1).arg(i + 1));
				row = 1;
				col = i + 1 + offset;
			}
			
			CtlCam();
		}

		statusCheck(0, 1);
		statusCheck(0, 0);
	}

	if (bES) return;//ES SIG
	dmc_write_outbit(1, 4, 1);//关真空电阀

	axis_Move(0, 0, nextc0a0pos, speed_c0a0, 1, true);
	axis_Move(0, 1, pos_c0a1, speed_c0a1, 1, true);

	if (bES) return;//ES SIG
	ResetAxis(1, 5, speed_c1a5, 0);
}

void leisaiMotion::CtlCam()
{
	emit sigWarnningLogAdd(QString::fromLocal8Bit(" "));
	if (bES) return;//ES SIG
	bReceived = false;
	//QString strSend("{\"action\":\"1\"}");
	currow = row;					//以左上角为1,1坐标的行号
	curcol = colCMOS - col + 1;		//以左上角为1,1坐标的列号
	sample_id = curcol + colCMOS*(row - 1);//从左到右从上到下计数
	time = QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz");//发送拍照指令时间
	QString s = box_id.first() + QString::number(pannel_id) + QString::number(sample_id) + time;//boxid+pannelid+sampleid+time合并用以生成MD5
	QByteArray b = HTTP_Interface::getMD5(s.toLatin1());
	md5 = HTTP_Interface::UnicodeToString(b);
	
	QString strSend(tr("{\"box_id\":\"%1\",\"pannel_id\":\"%2\",\"sample_id\":\"%3\",\"machine_id\":\"%4\",\"time\":\"%5\",\"operator\":\"%6\",\"shift_id\":\"%7\",\"md5\":\"%8\",\"config\":\"%9\"}")
		.arg(box_id.first())
		.arg(QString::number(pannel_id))
		.arg(QString::number(sample_id))
		.arg(machine_id)
		.arg(time)
		.arg(opreator)
		.arg(shift_id)
		.arg(md5)
		.arg(config)
	);
	m_HTTP_Interface->sendPost(strSend.toUtf8(), QUrl("http://127.0.0.1:6789/predict"), 6789);
	
	QTime outtime = QTime::currentTime().addMSecs(4000);//超时时间
	while (!bReceived)
	{
		if (bES) return;//ES SIG
		if (QTime::currentTime() > outtime)
		{
			emit sigWarnningLogAdd(QString::fromLocal8Bit("服务器超时"));
			emit sigLogAdd(QString::fromLocal8Bit("服务器超时"));
			slot_EmergencyStopOn();
			return;
		}
		QApplication::processEvents();
	}
	
	//RecordResult
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
	else
	{
		QSqlQuery query;
		if (!query.exec(QString("INSERT INTO ITEM_RECORD VALUES (NULL,'%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11','%12','%13','%14')")
				.arg(box_id.first())
				.arg(QString::number(pannel_id))
				.arg(QString::number(sample_id))
				.arg(machine_id)
				.arg(time)
				.arg(opreator)
				.arg(shift_id)
				.arg(md5)
				.arg(process_res)
				.arg(process_time)
				.arg(img_path)
				.arg(raw_image_path)
				.arg(pos_ok)
				.arg(config)))
		{
			//emit sigLogAdd(query.lastError());
			emit sigWarnningLogAdd(QString::fromLocal8Bit("插入数据数据失败"));
			emit sigLogAdd(QString::fromLocal8Bit("插入数据数据失败"));
		}

		db.close();
	}	
}

QString leisaiMotion::onReply(QNetworkReply *pReply) {
	emit sigWarnningLogAdd(" ");
	if (pReply->error());
	
	QByteArray array = pReply->readAll();
	pReply->close();

	QString str = HTTP_Interface::UnicodeToString(array);
	
	if (str == NULL)
	{
		emit sigWarnningLogAdd(QString::fromLocal8Bit("服务器无响应，请开启服务器"));
		emit sigLogAdd("Server ERROR:NO REPLY");
		return str;
	}
	emit sigLogAdd(str);

	Document doc;
	doc.Parse(str.toLocal8Bit().data());

	//初始化分支
	if (doc.HasMember("init_result"))
	{
		if (doc["init_result"] == "0")
		{
			emit sigWarnningLogAdd(QString::fromLocal8Bit("相机或者检测模块初始化错误"));
			emit sigLogAdd(QString::fromLocal8Bit("相机或者检测模块初始化错误"));
		}
		else
		{
			bReceived = true;
		}
		return str;
	}

	//轮廓检测分支
	if (doc.HasMember("detec_en"))
	{
		if (doc["detec_en"] == "1")
			emit sigLogAdd(QString::fromLocal8Bit("轮廓检测已打开"));
		else if (doc["detec_en"] == "0")
			emit sigLogAdd(QString::fromLocal8Bit("轮廓检测已关闭"));
		//bReceived = true;
		return str;
	}

	//拍照分支
	if (doc.HasMember("img_result"))
	{
		process_res = QString::fromLocal8Bit(doc["img_result"].GetString());
		process_time = QString::fromLocal8Bit(doc["img_proc_time"].GetString());
		img_path = QString::fromLocal8Bit(doc["img_path"].GetString());
		raw_image_path = QString::fromLocal8Bit(doc["img_raw_path"].GetString());
		pos_ok = QString::fromLocal8Bit(doc["img_pos_res"].GetString());

		int gridColor;//界面格子颜色flag
		if (process_res == "OK")
			gridColor = 1;
		else if (process_res == "NG")
			gridColor = 0;
		else if (process_res == "NO")
			gridColor = 2;

		emit sigSendRes(pannel_id, currow, curcol, gridColor, process_time, md5, raw_image_path);

		Value &path = doc["img_path"];
		if (path.GetString())
			emit sigSendImgPath(QString(path.GetString()));
		bReceived = true;
		return str;
	}
	emit sigWarnningLogAdd(QString::fromLocal8Bit("服务器返回信息无法识别"));
	emit sigLogAdd("Server ERROR");
	return str;
}

//void leisaiMotion::slot_ReleaseGoods()
//{	
//	statusCheck(1, 7);
//
//	if (bES) return;//ES SIG
//	if (dmc_read_inbit(1, 13) == 0)
//		emit sigLogAdd(QString::fromLocal8Bit("载板未推到位，请处理"));
//	while (dmc_read_inbit(1, 13) == 0)
//	{
//		if (bES) return;//ES SIG
//		QApplication::processEvents();
//	}
//
//	if ((count_c1a7 % 10) == 0)
//	{
//		if (bES) return;//ES SIG
//		axis_Move(1, 7, pos_c1a7 - 700000, speed_c1a7, 1, true);
//		statusCheck(1, 7);
//		mysleep(200);
//		if (bES) return;//ES SIG
//		dmc_write_outbit(1, 2, 1);//夹紧下料盒
//		ResetAxis(1, 7, speed_c1a7/2);
//		statusCheck(1, 7);
//		set0pos(1, 7);
//		
//		if (bES) return;//ES SIG
//		if (dmc_read_inbit(1, 12) == 0)
//			emit sigLogAdd(QString::fromLocal8Bit("下料盒满，请拿走右边空盒"));
//		while (dmc_read_inbit(1, 12) == 0)
//		{
//			if (bES) return;//ES SIG
//			QApplication::processEvents();
//		}
//		if (bES) return;//ES SIG
//		dmc_write_outbit(1, 3, 0);//推走下料盒
//		while (true)
//		{
//			if (bES) return;//ES SIG
//			if (dmc_read_inbit(1,8) == 0)
//				break;
//			QApplication::processEvents();
//		}
//		if (bES) return;//ES SIG
//		dmc_write_outbit(1, 3, 1);
//		axis_Move(1, 7, pos_c1a7, speed_c1a7, 1, true);
//		statusCheck(1, 7);
//		if (bES) return;//ES SIG
//		dmc_write_outbit(1, 2, 0);
//	}
//	else
//	{
//		axis_Move(1, 7, -60000, speed_c1a7, 0, true);
//	}
//	count_c1a7++;
//}

void leisaiMotion::mysleep(unsigned int msec)
{
	QTime reachTime = QTime::currentTime().addMSecs(msec);
	while (QTime::currentTime() < reachTime)
		QApplication::processEvents();
}

void leisaiMotion::areaCheck(bool onoff)
{
	if (onoff)
	{
		QString strSend(tr("{\"detec_en\":\"1\"}"));
		m_HTTP_Interface->sendPost(strSend.toUtf8(), QUrl("http://127.0.0.1:6789/settings"), 6789);
	}
	else
	{
		QString strSend(tr("{\"detec_en\":\"0\"}"));
		m_HTTP_Interface->sendPost(strSend.toUtf8(), QUrl("http://127.0.0.1:6789/settings"), 6789);
	}
}

void leisaiMotion::loose()
{
	ResetAxis(1, 2, speed_c1a2);
	ResetAxis(1, 3, speed_c1a3);
	ResetAxis(1, 5, speed_c1a5, 0);
}