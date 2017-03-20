#include "MyNoteBook.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MyNoteBook w;
	w.show();
	return a.exec();
}
