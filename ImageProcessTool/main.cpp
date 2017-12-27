#include "imageprocesstool.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ImageProcessTool w;
	w.show();
	return a.exec();
}
