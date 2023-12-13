
#include "pch.h"
#include "Graph.h"


Graph::Graph()
{
}

Graph::Graph(unsigned int m_DrawType, CPoint m_PointOrigin, CPoint m_PointEnd)
{
	this->m_DrawType = m_DrawType;
	this->m_PointOrigin = m_PointOrigin;
	this->m_PointEnd = m_PointEnd;
}

Graph::~Graph()
{
}