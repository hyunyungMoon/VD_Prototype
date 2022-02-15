#pragma once
#include "rg_Circle2D.h"

#include <vector>

class Vertex2D
{
private:
	rg_Point2D* m_generator1 = nullptr;
	rg_Point2D* m_generator2 = nullptr;
	rg_Point2D* m_generator3 = nullptr;

	vector<rg_Point2D*> generatorList = {m_generator1, m_generator2, m_generator3};

	rg_Circle2D* m_center;


public:
	Vertex2D();

	Vertex2D(rg_Point2D& generator1, rg_Point2D& generator2, rg_Point2D& generator3, rg_Circle2D& center);

	Vertex2D(const Vertex2D& vertex);

	~Vertex2D();

	double getX();

	double getY();

	vector<rg_Point2D*> getGeneratorList();

	

	bool isAdjacentVertex(const vector<rg_Point2D*> generatorList);

	vector<rg_Point2D> findSharingGenerator(const Vertex2D* vertex);
};

