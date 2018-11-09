#include "JslCtrl.h"
#include <QtWidgets/QApplication>
#include <qobject.h>

int main(int argc, char *argv[])
{
	QApplication *a = new QApplication(argc, argv);
	
	JslCtrl *w = new JslCtrl();
	w->show();
	return a->exec();
}
