
// BaslerMeterView.h: CBaslerMeterView 类的接口
//

#pragma once


#include "CBaslerMeterRePaint.h"
#include <vector>


class CBaslerMeterView : public CView
{
protected: // 仅从序列化创建
	CBaslerMeterView() noexcept;
	DECLARE_DYNCREATE(CBaslerMeterView)

// 特性
public:
	CBaslerMeterDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CBaslerMeterView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	void InitiateVariables();
	void InitiateBackboard();
	void GetParallelPoints(CPoint m_Point1, CPoint m_Point2, CPoint& pointParallel1, CPoint& pointParallel2);
	void SetPixelWithCross(CClientDC& dc, CPoint point);
	void SetPixelWithCross(CDC* pDC, CPoint point);
	void ThreePointsCircle(CPoint m_Point1, CPoint m_Point2, CPoint m_Point3, CRect& rect, double& diameter);
	BOOL IsFoundByDrawType(DrawType m_DrawType);
	BOOL IsPointCloseToLine(CPoint point, CPoint lineHead, CPoint lineTail);
	void LoadGrabbedImage();
	int ConverToStatusBar1();
	int ConverToStatusBar2();
	int ConverToStatusBar3(double diameter);
	afx_msg void OnCapture();
	afx_msg void OnDistance1();
	afx_msg void OnDistance2();
	afx_msg void OnDistance3();
private:
	CPoint m_Point1;
	CPoint m_Point2;
	CPoint m_Point3;
	CPoint m_PointParallel1;
	CPoint m_PointParallel2;
	CPoint m_PointBeCloseStart;
	BOOL m_IsPointClose;
	DrawType m_DrawType;
	int m_Distance3PointCount;
	// 此值要么为1（手动画的那根InActive），要么为2（复制出来的那根Active）
	int m_InActiveFlagForDistance2;
	//CBitmap m_Bitmap;
	CString m_Path;
	std::vector<CBaslerMeterRePaint> m_PaintedArray;
	double m_ScaleForDisplay;
	int m_Magnification;
	double m_Scale;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnFileOpen();
	afx_msg void OnCaptureSetting();
	afx_msg void OnVideo();
};

#ifndef _DEBUG  // BaslerMeterView.cpp 中的调试版本
inline CBaslerMeterDoc* CBaslerMeterView::GetDocument() const
   { return reinterpret_cast<CBaslerMeterDoc*>(m_pDocument); }
#endif

