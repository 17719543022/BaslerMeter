
// Painter4View.cpp: CPainter4View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Painter4.h"
#endif

#include "Painter4Doc.h"
#include "Painter4View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "MainFrm.h"


// CPainter4View

IMPLEMENT_DYNCREATE(CPainter4View, CView)

BEGIN_MESSAGE_MAP(CPainter4View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CPainter4View 构造/析构

CPainter4View::CPainter4View() noexcept
{
	// TODO: 在此处添加构造代码

}

CPainter4View::~CPainter4View()
{
}

BOOL CPainter4View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CPainter4View 绘图

void CPainter4View::OnDraw(CDC* /*pDC*/)
{
	CPainter4Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CPainter4View 打印

BOOL CPainter4View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CPainter4View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CPainter4View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CPainter4View 诊断

#ifdef _DEBUG
void CPainter4View::AssertValid() const
{
	CView::AssertValid();
}

void CPainter4View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPainter4Doc* CPainter4View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPainter4Doc)));
	return (CPainter4Doc*)m_pDocument;
}
#endif //_DEBUG


// CPainter4View 消息处理程序


void CPainter4View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar* pBar = &(pFrame->m_wndStatusBar);

	CString str;
	str.Format(_T("x的坐标是%d, y的坐标是%d"), point.x, point.y);

	pBar->SetPaneText(3, str);

	CView::OnMouseMove(nFlags, point);
}
