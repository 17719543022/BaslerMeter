
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
private:
	unsigned int m_DrawType;
	CPoint m_PointOrigin;
public:
	afx_msg void OnPoint();
	afx_msg void OnLine();
	afx_msg void OnRectangle();
	afx_msg void OnEllipse();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
private:
	CPtrArray m_PtrArray;
};

#ifndef _DEBUG  // Painter5View.cpp 中的调试版本
inline CPainter5Doc* CPainter5View::GetDocument() const
   { return reinterpret_cast<CPainter5Doc*>(m_pDocument); }
#endif

