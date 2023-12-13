#pragma once


enum class DrawType
{
	Capture,
	Distance1,
	Distance2,
	Distance3,
	Reserved,
};


class CBaslerMeterRePaint
{
public:
	CBaslerMeterRePaint();
	CBaslerMeterRePaint(CPoint m_Point1, CPoint m_Point2, CPoint m_Point3, CPoint pointParallel1, CPoint pointParallel2, DrawType m_DrawType, int m_Distance3PointCount, CRect rect, int diameter);
	~CBaslerMeterRePaint();

public:
	CPoint m_Point1;
	CPoint m_Point2;
	CPoint m_Point3;
	CPoint m_PointParallel1;
	CPoint m_PointParallel2;
	DrawType m_DrawType;
	int m_Distance3PointCount;
	CRect rect;
	int diameter;
};

