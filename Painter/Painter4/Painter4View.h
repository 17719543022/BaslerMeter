
// Painter4View.h: CPainter4View 类的接口
//

#pragma once


class CPainter4View : public CView
{
protected: // 仅从序列化创建
	CPainter4View() noexcept;
	DECLARE_DYNCREATE(CPainter4View)

// 特性
public:
	CPainter4Doc* GetDocument() const;

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
	virtual ~CPainter4View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // Painter4View.cpp 中的调试版本
inline CPainter4Doc* CPainter4View::GetDocument() const
   { return reinterpret_cast<CPainter4Doc*>(m_pDocument); }
#endif

