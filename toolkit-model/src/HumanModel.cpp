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
	m_bIsCenterOfRotationSet = false;

	m_i32NbPCAParameters = 0;
	m_i32NbRotoTranslationParameters = 6;
	m_i32NbTransformations = 0;
	m_i32NbVertices = 0;

	m_vFirstLevelMeanModel.clear();
	m_vFirstLevelCovarianceModel.clear();

	m_vTransformations.clear();

	m_vSecondLevelMeanModel.clear();
	m_vSecondLevelCovarianceModel.clear();

	m_vVertices.clear();

	
}

bool HumanModel::updateGeometry(std::vector<double> vPCValues)
{
	bool l_bIsFunctionSuccessfull = false;
	
	if (pc2Transformations(vPCValues))
	{

		if (transformations2Vertices())
		{
			l_bIsFunctionSuccessfull = true;
		}
		else
		{
			l_bIsFunctionSuccessfull = false;
			qDebug() << "[ERROR] (HumanModel::updateGeometry) Cannot compute from PC values to Vertices!";
		}
	}
	else
	{
		l_bIsFunctionSuccessfull = false;
		qDebug() << "[ERROR] (HumanModel::updateGeometry) Cannot compute transformations from PC values!";
	}
	
	return l_bIsFunctionSuccessfull;
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
		
		// 1st line
		QString line = in.readLine();
		QStringList list = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
		m_i32NbTransformations = list[0].toInt();

		// 2nd line
		line = in.readLine();
		list = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
		m_i32NbPCAParameters = list[0].toInt();
		for (int l_i32PCAParameter = 0; l_i32PCAParameter < m_i32NbPCAParameters; l_i32PCAParameter++)
		{
			m_vPCANames.push_back(list[l_i32PCAParameter+1]);
		}

		// 1st level model
		for (int l_i32Transformation = 0; l_i32Transformation < m_i32NbTransformations; l_i32Transformation++)
		{
			line = in.readLine();
			list = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
			m_vTransformationNames.push_back(list[0]);
			// Center of rotation
			if (list[0].contains("ROT", Qt::CaseSensitive) && !m_bIsCenterOfRotationSet)
			{
				QStringList crotList = list[0].split("_");
				for (int l_i32Coord = 0; l_i32Coord < 3; l_i32Coord++)
					m_vCenterOfRotation.push_back(crotList[1+l_i32Coord].toDouble());

				m_bIsCenterOfRotationSet = true;
			}
			m_vTransformationTypes.push_back(list[1]);
			m_vFirstLevelMeanModel.push_back(list[2].toDouble());

			for (int l_i32PCAParameter = 0; l_i32PCAParameter < m_i32NbPCAParameters; l_i32PCAParameter++)
			{
				m_vFirstLevelCovarianceModel.push_back(list[3 + l_i32PCAParameter].toDouble());
			}
		}

		// Nb vertex
		line = in.readLine();
		list = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
		m_i32NbVertices = list[0].toInt();

		// 2nd level model
		for (int l_i32Vertices = 0; l_i32Vertices < m_i32NbVertices; l_i32Vertices++)
		{
			line = in.readLine(); // vertex number

			for (int l_i32Coord = 0; l_i32Coord < 3; l_i32Coord++)
			{
				line = in.readLine();
				list = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);

				m_vSecondLevelMeanModel.push_back(list[0].toDouble());

				for (int l_i32Transformation = 0; l_i32Transformation < m_i32NbTransformations; l_i32Transformation++)
				{
					m_vSecondLevelCovarianceModel.push_back(list[1 + l_i32Transformation].toDouble());
				}
			}
		}

		file.close();
		
		m_bIsHumanModelLoaded = true;
	}
	
	// checks if the model is plausible
	if (m_vFirstLevelMeanModel.size() != m_i32NbTransformations)
	{
		m_bIsHumanModelLoaded = false;
		qDebug() << "[ERROR] (HumanModel::load) First level mean model is corrupted!";

	}
	if (m_vFirstLevelCovarianceModel.size() != 3 * m_i32NbTransformations * m_i32NbPCAParameters)
	{
		m_bIsHumanModelLoaded = false;
		qDebug() << "[ERROR] (HumanModel::load) First level covariance model is corrupted!";
	}
	if (m_vSecondLevelMeanModel.size() != m_i32NbVertices)
	{
		m_bIsHumanModelLoaded = false;
		qDebug() << "[ERROR] (HumanModel::load) Second level mean model is corrupted!";

	}
	if (m_vSecondLevelCovarianceModel.size() != 3 * m_i32NbTransformations * m_i32NbVertices)
	{
		m_bIsHumanModelLoaded = false;
		qDebug() << "[ERROR] (HumanModel::load) Second level covariance model is corrupted!";
	}

	// separate Principal Components from Roto-Translation parameters
	m_i32NbPCAParameters -= m_i32NbRotoTranslationParameters;
	m_i32NbTransformations -= m_i32NbRotoTranslationParameters;

	return m_bIsHumanModelLoaded;
}

bool HumanModel::pc2Transformations(std::vector<double> vPCValues)
{
	bool l_bIsFunctionSuccessfull = true;
	m_vTransformations.clear();

	for (int l_i32IntermediateParams = 0; l_i32IntermediateParams < m_i32NbTransformations; l_i32IntermediateParams++)
	{
		double l_dCovarianceResult = 0.0;
		for (int l_i32PCAParameter = 0; l_i32PCAParameter < m_i32NbPCAParameters; l_i32PCAParameter++)
		{
			l_dCovarianceResult += vPCValues[l_i32PCAParameter] * m_vFirstLevelCovarianceModel[m_i32NbPCAParameters*l_i32IntermediateParams + l_i32PCAParameter];
		}

		double l_dResult = 0.0;

		if (m_vTransformationTypes[l_i32IntermediateParams].compare("LIN") == 0)
		{
			l_dResult = l_dCovarianceResult + m_vFirstLevelMeanModel[l_i32IntermediateParams];
		}
		else if (m_vTransformationTypes[l_i32IntermediateParams].compare("COS") == 0)
		{
			l_dResult = cos(l_dCovarianceResult + m_vFirstLevelMeanModel[l_i32IntermediateParams]);
		}
		else if (m_vTransformationTypes[l_i32IntermediateParams].compare("SIN") == 0)
		{
			l_dResult = sin(l_dCovarianceResult + m_vFirstLevelMeanModel[l_i32IntermediateParams]);
		}
		else
		{
			l_bIsFunctionSuccessfull = false;
			qDebug() << "[ERROR] (FaceModel::pc2Transformations) Transformation type" + m_vTransformationTypes[l_i32IntermediateParams] + "is not supported!";
		}

		m_vTransformations.push_back(l_dResult);
	}

	return l_bIsFunctionSuccessfull;
}

bool HumanModel::transformations2Vertices()
{
	bool l_bIsFunctionSuccessfull = true;
	m_vVertices.clear();

	// 2nd level
	for (int l_i32coord = 0; l_i32coord < 3 * m_i32NbVertices; l_i32coord++)
	{
		double l_dCovarianceResult = 0.0;
		for (int l_i32IntermediateParams = 0; l_i32IntermediateParams < m_i32NbTransformations; l_i32IntermediateParams++)
		{
			l_dCovarianceResult += m_vTransformations[l_i32IntermediateParams] * m_vSecondLevelCovarianceModel[m_i32NbPCAParameters*l_i32coord + l_i32IntermediateParams];
		}

		m_vVertices.push_back(l_dCovarianceResult + m_vSecondLevelMeanModel[l_i32coord]);
	}

	return l_bIsFunctionSuccessfull;
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

std::vector<double> HumanModel::getRigidMotion()
{
	return m_vRigidMotion;
}

int HumanModel::getNbPCAParameters()
{
	return m_i32NbPCAParameters;
}

int HumanModel::getNbTransformations()
{
	return m_i32NbTransformations;
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

void HumanModel::setNbTransformations(int i32NbIntermediateParameters)
{
	m_i32NbTransformations = i32NbIntermediateParameters;
}

void HumanModel::setNbVertices(int i32NbVertices)
{
	m_i32NbVertices = i32NbVertices;
}

