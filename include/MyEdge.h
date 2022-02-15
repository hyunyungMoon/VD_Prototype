#pragma once
#include <WingedEdgeDataStructure.h>
#include <rg_Point2D.h>

class MyEdge :
    public WingedEdgeDataStructure::Edge
{
public:
    bool m_direction;
    rg_Point2D m_vector;

public:
    inline MyEdge() { m_direction = true; m_vector = rg_Point2D(); }
    inline MyEdge(bool direction, rg_Point2D vector) { m_direction = direction; m_vector = vector; }
    inline bool get_direction() {        return m_direction;    }
    inline rg_Point2D get_vector() {        return m_vector;    }

};

