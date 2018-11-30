#ifndef _MESH_H_
#define _MESH_H_

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>

class Mesh
{
	
// Constructor/Destructor
public:
	Mesh();
	~Mesh();

// Methods
public:
	bool loadFaces(QString);
	bool loadTexture(QString);

	void setVertices(std::vector<double>);
	
protected:
	
private:
	void resetVariables();
	
// Variables
protected:
	bool m_bIsFaceMeshLoaded;
	bool m_bIsTextureMeshLoaded;

	std::vector<double> m_vVertices;
	std::vector<double> m_vTexture;
	std::vector<unsigned int> m_vFaces;
	
	int m_i32NbFaces;
	int m_i32NbVertices;
	QString m_sImageFilename;
};

#endif
