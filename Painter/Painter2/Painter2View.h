
// Painter2View.h: CPainter2View 类的接口
//

#pragma once


class CPainter2View : public CScrollView
{
protected: // 仅从序列化创建
	CPainter2View() noexcept;
	DECLARE_DYNCREATE(CPainter2View)

// 特性
public:
	CPainter2Doc* GetDocument() const;

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
	virtual ~CPainter2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
private:
	int m_choice;
	int m_size;
	CPoint m_pointBegin;
	CPoint m_pointEnd;
public:
	afx_msg void OnPoint();
	afx_msg void OnLine();
	afx_msg void OnRectangle();
	afx_msg void OnChess();
	afx_msg void OnSetting();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // Painter2View.cpp 中的调试版本
inline CPainter2Doc* CPainter2View::GetDocument() const
   { return reinterpret_cast<CPainter2Doc*>(m_pDocument); }
#endif

