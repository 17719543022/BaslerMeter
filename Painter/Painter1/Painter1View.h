
// Painter5View.h: CPainter5View 类的接口
//

#pragma once


class CPainter5View : public CScrollView
{
protected: // 仅从序列化创建
	CPainter5View() noexcept;
	DECLARE_DYNCREATE(CPainter5View)

	// 特性
public:
	CPainter5Doc* GetDocument() const;

	// 操作
public:

	// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// 实现
public:
	virtual ~CPainter5View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	int m_PenSize;
	COLORREF m_PenColor;
	COLORREF m_BrushColor;
	CPoint m_PointBegin;
	CPoint m_PointEnd;
	int m_TextId;
	CEdit* m_Edit;
	CPoint m_MagnifyCenter;
	int m_MagnifyWidth;
	int m_MagnifyHeight;
	enum class DrawType {
		LineSegment,
		Circle,
		Rectangle,
		Ellipse,
		Text,
		Pencil,
		Eraser,
		Magnify,
		Reserved,
	}m_DrawType;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDrawLineSegment();
	afx_msg void OnDrawRectangle();
	afx_msg void OnDrawCircle();
	afx_msg void OnDrawEllipse();
	afx_msg void OnSetSize();
	afx_msg void OnSetColor();
	afx_msg void OnDrawText();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDrawPencil();
	afx_msg void OnDrawEraser();
	HBITMAP NearestNeighborResize(HBITMAP hBmpSrc, int newWidth, int newHeight);
	afx_msg void OnFileOpen();
	afx_msg void OnDrawMagnify();

	CRect GetMagnifyRect(CPoint point);
};

#ifndef _DEBUG  // Painter5View.cpp 中的调试版本
inline CPainter5Doc* CPainter5View::GetDocument() const
{
	return reinterpret_cast<CPainter5Doc*>(m_pDocument);
}
#endif

