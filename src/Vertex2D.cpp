
#include "Vertex2D.h"

Vertex2D::Vertex2D()
{

}

Vertex2D::Vertex2D(rg_Point2D& generator1, rg_Point2D& generator2, rg_Point2D& generator3, rg_Circle2D& center):
	m_generator1(&generator1), m_generator2(&generator2), m_generator3(&generator3), m_center(&center)
{
}

Vertex2D::Vertex2D(const Vertex2D& vertex)
{
	m_generator1 = vertex.m_generator1;
	m_generator2 = vertex.m_generator2;
	m_generator3 = vertex.m_generator3;

	m_center = vertex.m_center;
}


Vertex2D::~Vertex2D()
{
	if (m_generator1 != nullptr)
		m_generator1 = nullptr;

	if (m_generator2 != nullptr)
		m_generator2 = nullptr;

	if (m_generator3 != nullptr)
		m_generator3 = nullptr;

	if (m_center != nullptr)
		m_center = nullptr;

	delete m_generator1;
	delete m_generator2;
	delete m_generator3;
	delete m_center;
}


double Vertex2D::getX()
{
	return m_center->getCenterPt().getX();
}

double Vertex2D::getY()
{
	return m_center->getCenterPt().getY();
}

vector<rg_Point2D*> Vertex2D::getGeneratorList()
{
	return generatorList;
}

bool Vertex2D::isAdjacentVertex(const vector<rg_Point2D*> generatorList)
{
	int count = 0;
	for (int i = 0; i < generatorList.size(); i++)
	{

		if (generatorList[i] == m_generator1 || generatorList[i] == m_generator2 || generatorList[i] == m_generator3) {
			count++;
		}

	}
	if (count == 2)
	{
		return true;
	}
	else {
		return false;
	}
}


vector<rg_Point2D> Vertex2D::findSharingGenerator(const Vertex2D* vertex)
{
	vector<rg_Point2D> sharingVertex = {};
	for (int i = 0; i < vertex->generatorList.size(); i++) {
		if (vertex->generatorList[i] == m_generator1) {
			sharingVertex.push_back(*m_generator1);
			continue;
		}
		else if (vertex->generatorList[i] == m_generator2) {
			sharingVertex.push_back(*m_generator2);
			continue;
		}
		else if (vertex->generatorList[i] == m_generator3) {
			sharingVertex.push_back(*m_generator3);
			continue;
		}
	}
	return sharingVertex;
}

