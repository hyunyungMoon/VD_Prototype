#pragma once
#include <WingedEdgeDataStructure.h>
#include <rg_Point2D.h>

class MyVertex :
    public WingedEdgeDataStructure::Vertex
{
public:
    int b = 0;
    rg_Point2D m_point;

    inline MyVertex() { }
    inline MyVertex(const int ID) { m_ID = ID; }
    inline MyVertex(const rg_Point2D& point) { m_point = point; }

};

