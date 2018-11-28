#ifndef _HAND_MODEL_H_
#define _HAND_MODEL_H_

#include "HumanModel.h"

class HandModel: public HumanModel
{
// Constructor/Destructor
public:
	HandModel();
	~HandModel();

// Methods
public:
	int getNbAngles();

	void setNbAngles(int);

protected:
	
private:
	
// Variables
private:
	int m_i32NbAngles;

};

#endif
