#ifndef _HUMAN_MODEL_H_
#define _HUMAN_MODEL_H_

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include <vector>

class HumanModel
{
	
// Constructor/Destructor
public:
	HumanModel();
	~HumanModel();

// Methods
public:
	bool load(QString sHumanModelFilename);
	void firstLevelTransform(std::vector<double>);
	void secondLevelTransform();

	std::vector<double> getVertices();
	std::vector<double> getRigidMotion();
	std::vector<double> getFirstLevelMeanModel();
	std::vector<double> getFirstLevelCovarianceModel();
	std::vector<double> getSecondLevelMeanModel();
	std::vector<double> getSecondLevelCovarianceModel();
	int getNbPCAParameters();
	int getNbIntermediateParameters();
	int getNbVertices();
	bool getIsHumanModelLoaded();
	bool getIsCoordComputed();

	void setFirstLevelMeanModel(std::vector<double>);
	void setFirstLevelCovarianceModel(std::vector<double>);
	void setSecondLevelMeanModel(std::vector<double>);
	void setSecondLevelCovarianceModel(std::vector<double>);
	void setNbPCAParameters(int);
	void setNbIntermediateParameters(int);
	void setNbVertices(int);

protected:
	void resetVariables();
	
private:
	
// Variables
private:
	bool m_bIsHumanModelLoaded;
	bool m_bIsCoordComputed;

	int m_i32NbPCAParameters;
	int m_i32NbIntermediateParameters;
	int m_i32NbVertices;

	std::vector<double> m_vFirstLevelMeanModel;
	std::vector<double> m_vFirstLevelCovarianceModel;

	std::vector<double> m_vSecondLevelMeanModel;
	std::vector<double> m_vSecondLevelCovarianceModel;

	std::vector<double> m_vIntermediatePC;
	std::vector<double> m_vVertices;
	
	
};

#endif
