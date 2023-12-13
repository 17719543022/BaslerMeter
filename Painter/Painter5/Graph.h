
#pragma once


class Graph
{
public:
	Graph();
	Graph(unsigned int m_DrawType, CPoint m_PointOrigin, CPoint m_PointEnd);
	~Graph();

public:
	CPoint m_PointOrigin;
	CPoint m_PointEnd;
	unsigned int m_DrawType;
};

