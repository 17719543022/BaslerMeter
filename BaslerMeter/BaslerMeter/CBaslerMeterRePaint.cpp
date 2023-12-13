#include "pch.h"
#include "CBaslerMeterRePaint.h"



CBaslerMeterRePaint::CBaslerMeterRePaint()
{

}


CBaslerMeterRePaint::CBaslerMeterRePaint(CPoint m_Point1, CPoint m_Point2, CPoint m_Point3, CPoint pointParallel1, CPoint pointParallel2, DrawType m_DrawType, int m_Distance3PointCount, CRect rect, int diameter)
{
	this->m_Point1 = m_Point1;
	this->m_Point2 = m_Point2;
	this->m_Point3 = m_Point3;
	this->m_PointParallel1 = pointParallel1;
	this->m_PointParallel2 = pointParallel2;
	this->m_DrawType = m_DrawType;
	this->m_Distance3PointCount = m_Distance3PointCount;
	this->rect = rect;
	this->diameter = diameter;
}


CBaslerMeterRePaint::~CBaslerMeterRePaint()
{

}