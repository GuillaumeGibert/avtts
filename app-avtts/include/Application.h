/**
* \file Application.h
* \brief Defines Application
* \author Guillaume Gibert
* \date 14/11/2018
*/

#ifndef _APPLICATION_H
#define _APPLICATION_H

// Window
#include "MainWindow.h"

// Qt
#include <QThread>

Q_DECLARE_METATYPE(std::vector<float>);
Q_DECLARE_METATYPE(std::vector<int>);
Q_DECLARE_METATYPE(std::string);


/**
* \class Application
* \brief The object which represents the application and manages it.
*/
class Application: public QObject
{
	Q_OBJECT

public:
	/**
	* \brief The constructor of the object.
	*/
	Application();

	/**
	* \brief The delete of the object.
	*/
	~Application();

	/**
	* \brief The function that initialize the application.
	*/
	void init(MainWindow* window);

private:
	

signals:

	// -------------------- SIGNALS FOR THE WINDOW --------------------

	

public slots:

	// -------------------- WINDOW SLOTS --------------------	

	


public:
	
private:
	MainWindow* m_window = nullptr;						/**< The main window. */

	
};

#endif