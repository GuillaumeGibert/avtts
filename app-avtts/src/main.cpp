/**
* \file main.cpp
* \brief Defines the main
* \author Guillaume Gibert
* \date 14/11/2018
*/

#include "Application.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	Application app;
	MainWindow w;

	w.show();
	app.init(&w);

	return a.exec();
}