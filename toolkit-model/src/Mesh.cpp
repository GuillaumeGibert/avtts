#include "Mesh.h"

Mesh::Mesh()
{
	
}

Mesh::~Mesh()
{
	
}

void Mesh::resetVariables()
{
	m_bIsFaceMeshLoaded = false;
	m_bIsTextureMeshLoaded = false;
	
	m_vVertices.clear();
	m_vTexture.clear();
	m_vFaces.clear();
	
	m_i32NbFaces = 0;
	m_i32NbVertices = 0;
	
	m_sImageFilename = "";
}

bool Mesh::loadFaces(QString sFaceMeshFilename)
{
	QFile file(sFaceMeshFilename);
	
	// opens the file
	if(!file.open(QIODevice::ReadOnly)) 
	{
		qDebug() << "[ERROR] (Mesh::loadFaces) Cannot open the Face Mesh File!";
		m_bIsFaceMeshLoaded = false;
	}
	else
	{
		QTextStream in(&file);
		// 1st line
		QString line = in.readLine();
		QStringList list = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
		m_i32NbFaces = list[0].toInt();
		
		for (int l_i32Face = 0; l_i32Face < m_i32NbFaces; l_i32Face++)
		{
			line = in.readLine();
			list = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
			
			for (int l_i32Coord = 0; l_i32Coord < 3; l_i32Coord++)
			{
				m_vFaces.push_back(list[l_i32Coord].toInt());
			}
		}
		
		file.close();
		
		m_bIsFaceMeshLoaded = true;
	}
	
	return m_bIsFaceMeshLoaded;
}


bool Mesh::loadTexture(QString sTextureMeshFile)
{
	QFile file(sTextureMeshFile);
	
	// opens the file
	if(!file.open(QIODevice::ReadOnly)) 
	{
		qDebug() << "[ERROR] (Mesh::loadTexture) Cannot open the Texture Mesh File!";
		m_bIsTextureMeshLoaded = false;
	}
	else
	{
		QTextStream in(&file);
		// 1st line
		QString line = in.readLine();
		QStringList list = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
		m_sImageFilename = list[0];
		
		while(!in.atEnd()) 
		//for (int l_i32Vertex = 0; l_i32Vertex < m_i32NbVertices; l_i32Vertex++)
		{
			line = in.readLine();
			list = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
			
			for (int l_i32Coord = 0; l_i32Coord < 2; l_i32Coord++)
			{
				m_vTexture.push_back(list[l_i32Coord].toDouble());
			}
		}
		
		file.close();
		
		m_bIsTextureMeshLoaded = true;
	}
	
	return m_bIsFaceMeshLoaded;
}

void Mesh::setVertices(std::vector<double> vVertices)
{
	m_vVertices = vVertices;
	m_i32NbVertices = m_vVertices.size() / 3;
}