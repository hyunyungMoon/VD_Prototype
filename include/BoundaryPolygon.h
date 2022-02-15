#pragma once
#include "rg_Line2D.h"
#include "MyEdge.h"
#include "MyVertex.h"
#include <list>
class BoundaryPolygon
{
public :
	std::list<rg_Line2D*> m_linelist;
	double ContanerSize;

public : 
	inline BoundaryPolygon(std::list<rg_Line2D*> list, double size) { m_linelist = list, ContanerSize = size; }

	bool find_intersect(const MyEdge* edge, rg_Point2D& intersection_point) { 

		rg_Point2D existPoint;
		rg_Point2D endPoint;

		if (edge->end_vertex() == nullptr) {
			MyVertex * vertex = (MyVertex*)edge->start_vertex();
			existPoint = vertex->m_point;
		}
		else if (edge->start_vertex() == nullptr) {
			MyVertex* vertex = (MyVertex*)edge->end_vertex();
			existPoint = vertex->m_point;
		}

		if (edge->m_direction == true) {
			endPoint = edge->m_vector * ContanerSize + existPoint ;
		}
		else if (edge->m_direction == false) {
			endPoint = edge->m_vector * -ContanerSize + existPoint;
		}


		rg_Line2D curr_line = rg_Line2D(existPoint,endPoint);
		bool doesIntersect = true;
		rg_Line2D::IntersectionPointPosition position;

		std::list<rg_Line2D*>::iterator it;
		for (it = m_linelist.begin(); it != m_linelist.end(); it++) {
			intersection_point = (*it)->compute_intersection_btw_two_line_segments(curr_line, doesIntersect, position);
			if (doesIntersect)
				break;
		}

		if (it == m_linelist.end())
			return false;

		return true;
	}

};

