#include "TestTool.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	TestTool w;
	w.show();
	return a.exec();
}
