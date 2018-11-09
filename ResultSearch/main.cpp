#include "ResultSearch.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ResultSearch w;
	w.show();
	return a.exec();
}
