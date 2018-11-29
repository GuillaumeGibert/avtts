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
	virtual void pc2Transformations(std::vector<double>)=0;
	void transformations2Vertices();

	std::vector<double> getVertices();
	std::vector<double> getRigidMotion();
	std::vector<double> getFirstLevelMeanModel();
	std::vector<double> getFirstLevelCovarianceModel();
	std::vector<double> getSecondLevelMeanModel();
	std::vector<double> getSecondLevelCovarianceModel();
	int getNbPCAParameters();
	int getNbTransformations();
	int getNbVertices();
	bool getIsHumanModelLoaded();
	bool getIsCoordComputed();

	void setFirstLevelMeanModel(std::vector<double>);
	void setFirstLevelCovarianceModel(std::vector<double>);
	void setSecondLevelMeanModel(std::vector<double>);
	void setSecondLevelCovarianceModel(std::vector<double>);
	void setNbPCAParameters(int);
	void setNbTransformations(int);
	void setNbVertices(int);

protected:
	void resetVariables();
	
private:
	
// Variables
protected:
	bool m_bIsHumanModelLoaded;
	bool m_bIsCoordComputed;

	int m_i32NbPCAParameters;
	int m_i32NbTransformations;
	int m_i32NbVertices;

	std::vector<QString> m_vPCANames;
	std::vector<QString> m_vTransformationNames;
	std::vector<QString> m_vTransformationTypes;

	std::vector<double> m_vFirstLevelMeanModel;
	std::vector<double> m_vFirstLevelCovarianceModel;

	std::vector<double> m_vSecondLevelMeanModel;
	std::vector<double> m_vSecondLevelCovarianceModel;

	std::vector<double> m_vTransformations;
	std::vector<double> m_vVertices;

	std::vector<double> m_vRigidMotion;
	
	
};

#endif
