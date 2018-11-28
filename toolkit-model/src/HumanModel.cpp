#include "HumanModel.h"

HumanModel::HumanModel()
{
	resetVariables();
}

HumanModel::~HumanModel()
{
	
}

void HumanModel::resetVariables()
{
	m_bIsHumanModelLoaded = false;
	m_bIsCoordComputed = false;

	m_i32NbPCAParameters = 0;
	m_i32NbIntermediateParameters = 0;
	m_i32NbVertices = 0;

	m_vFirstLevelMeanModel.clear();
	m_vFirstLevelCovarianceModel.clear();

	m_vIntermediatePC.clear();

	m_vSecondLevelMeanModel.clear();
	m_vSecondLevelCovarianceModel.clear();

	m_vVertices.clear();

	
}

bool HumanModel::load(QString sHumanModelFilename)
{
	QFile file(sHumanModelFilename);
	
	// opens the file
	if(!file.open(QIODevice::ReadOnly)) 
	{
		qDebug() << "[ERROR] (HumanModel::load) Cannot open the Human Model File!";
		m_bIsHumanModelLoaded = false;
	}
	else
	{
		QTextStream in(&file);
		
		while(!in.atEnd()) 
		{
			QString line = in.readLine();    
			QStringList fields = line.split(" ");    
			QStringList list = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
		}
		
		file.close();
		
		m_bIsHumanModelLoaded = true;
	}
	
	// checks if the model is plausible
	// MeanModel.size() = 3(x,y,z)*m_i32NbVertices
	//CovarianceModel.size() = m_i32NbPCAParameters x 3*m_i32NbVertices
	
	
	return m_bIsHumanModelLoaded;
}

void HumanModel::firstLevelTransform(std::vector<double> vPCValues)
{
	m_vIntermediatePC.clear();

	for (int l_i32IntermediateParams = 0; l_i32IntermediateParams < m_i32NbIntermediateParameters; l_i32IntermediateParams++)
	{
		double l_dCovarianceResult = 0.0;
		for (int l_i32PCAParameter = 0; l_i32PCAParameter < m_i32NbPCAParameters; l_i32PCAParameter++)
		{
			l_dCovarianceResult += vPCValues[l_i32PCAParameter] * m_vFirstLevelCovarianceModel[m_i32NbPCAParameters*l_i32IntermediateParams + l_i32PCAParameter];
		}

		double l_dResult = 0.0;
		//if (LIN)
			l_dResult = l_dCovarianceResult + m_vFirstLevelMeanModel[l_i32IntermediateParams];
			l_dResult = cos(l_dCovarianceResult + m_vFirstLevelMeanModel[l_i32IntermediateParams]);
			l_dResult = sin(l_dCovarianceResult + m_vFirstLevelMeanModel[l_i32IntermediateParams]);


		m_vIntermediatePC.push_back(l_dResult);
	}
}

void HumanModel::secondLevelTransform()
{
	m_vVertices.clear();

	// 2nd level
	for (int l_i32coord = 0; l_i32coord < 3*m_i32NbVertices; l_i32coord++)
	{
		double l_dCovarianceResult = 0.0;
		for (int l_i32IntermediateParams = 0; l_i32IntermediateParams < m_i32NbIntermediateParameters; l_i32IntermediateParams++)
		{
			l_dCovarianceResult += m_vIntermediatePC[l_i32IntermediateParams] * m_vSecondLevelCovarianceModel[m_i32NbPCAParameters*l_i32coord + l_i32IntermediateParams];
		}
		
		m_vVertices.push_back(l_dCovarianceResult + m_vSecondLevelMeanModel[l_i32coord]);
	}
}

// Getters
std::vector<double> HumanModel::getVertices()
{
	return m_vVertices;
}

std::vector<double> HumanModel::getFirstLevelMeanModel()
{
	return m_vFirstLevelMeanModel;
}

std::vector<double> HumanModel::getFirstLevelCovarianceModel()
{
	return m_vFirstLevelCovarianceModel;
}

std::vector<double> HumanModel::getSecondLevelMeanModel()
{
	return m_vSecondLevelMeanModel;
}

std::vector<double> HumanModel::getSecondLevelCovarianceModel()
{
	return m_vSecondLevelCovarianceModel;
}

int HumanModel::getNbPCAParameters()
{
	return m_i32NbPCAParameters;
}

int HumanModel::getNbIntermediateParameters()
{
	return m_i32NbIntermediateParameters;
}

int HumanModel::getNbVertices()
{
	return m_i32NbVertices;
}

bool HumanModel::getIsHumanModelLoaded()
{
	return m_bIsHumanModelLoaded;
}

bool HumanModel::getIsCoordComputed()
{
	return m_bIsCoordComputed;
}

// Setters
void HumanModel::setFirstLevelMeanModel(std::vector<double> vMeanModel)
{
	m_vFirstLevelMeanModel = vMeanModel;
}

void HumanModel::setFirstLevelCovarianceModel(std::vector<double> vCovarianceModel)
{
	m_vFirstLevelCovarianceModel = vCovarianceModel;
}

void HumanModel::setSecondLevelMeanModel(std::vector<double> vMeanModel)
{
	m_vSecondLevelMeanModel = vMeanModel;
}

void HumanModel::setSecondLevelCovarianceModel(std::vector<double> vCovarianceModel)
{
	m_vSecondLevelCovarianceModel = vCovarianceModel;
}

void HumanModel::setNbPCAParameters(int i32NbPCAParameters)
{
	m_i32NbPCAParameters = i32NbPCAParameters;
}

void HumanModel::setNbIntermediateParameters(int i32NbIntermediateParameters)
{
	m_i32NbIntermediateParameters = i32NbIntermediateParameters;
}

void HumanModel::setNbVertices(int i32NbVertices)
{
	m_i32NbVertices = i32NbVertices;
}

