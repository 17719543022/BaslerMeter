
// Painter5View.cpp: CPainter5View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Painter5.h"
#endif

#include "Painter5Doc.h"
#include "Painter5View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "Graph.h"


// CPainter5View

IMPLEMENT_DYNCREATE(CPainter5View, CScrollView)

BEGIN_MESSAGE_MAP(CPainter5View, CScrollView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_COMMAND(IDM_POINT, &CPainter5View::OnPoint)
	ON_COMMAND(IDM_LINE, &CPainter5View::OnLine)
	ON_COMMAND(IDM_RECTANGLE, &CPainter5View::OnRectangle)
	ON_COMMAND(IDM_ELLIPSE, &CPainter5View::OnEllipse)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CPainter5View 构造/析构

CPainter5View::CPainter5View() noexcept
{
	// TODO: 在此处添加构造代码
	m_DrawType = 0;
	m_PointOrigin = CPoint(0, 0);
}

CPainter5View::~CPainter5View()
{
}

BOOL CPainter5View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CScrollView::PreCreateWindow(cs);
}

// CPainter5View 绘图

void CPainter5View::OnDraw(CDC* pDC)
{
	CPainter5Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	// 创建透明画刷，不加此条的话，画出来的矩形和圆填充的白色而不是透明的
	CBrush* pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	pDC->SelectObject(pBrush);

	// 将保存的图形对象取出来
	for (int i = 0; i < m_PtrArray.GetSize(); i++)
	{
		switch (((Graph*)m_PtrArray.GetAt(i))->m_DrawType)
		{
		case 1:
			pDC->SetPixel(((Graph*)m_PtrArray.GetAt(i))->m_PointEnd, RGB(0, 0, 0));
			break;
		case 2:
			pDC->MoveTo(((Graph*)m_PtrArray.GetAt(i))->m_PointOrigin);
			pDC->LineTo(((Graph*)m_PtrArray.GetAt(i))->m_PointEnd);
			break;
		case 3:
			pDC->Rectangle(CRect(((Graph*)m_PtrArray.GetAt(i))->m_PointOrigin
				, ((Graph*)m_PtrArray.GetAt(i))->m_PointEnd));
			break;
		case 4:
			pDC->Ellipse(CRect(((Graph*)m_PtrArray.GetAt(i))->m_PointOrigin
				, ((Graph*)m_PtrArray.GetAt(i))->m_PointEnd));
			break;
		default:
			break;
		}
	}
}

void CPainter5View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 计算此视图的合计大小
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CPainter5View 打印

BOOL CPainter5View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CPainter5View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CPainter5View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CPainter5View 诊断

#ifdef _DEBUG
void CPainter5View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CPainter5View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CPainter5Doc* CPainter5View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPainter5Doc)));
	return (CPainter5Doc*)m_pDocument;
}
#endif //_DEBUG


// CPainter5View 消息处理程序


void CPainter5View::OnPoint()
{
	// TODO: 在此添加命令处理程序代码
	m_DrawType = 1;
}


void CPainter5View::OnLine()
{
	// TODO: 在此添加命令处理程序代码
	m_DrawType = 2;
}


void CPainter5View::OnRectangle()
{
	// TODO: 在此添加命令处理程序代码
	m_DrawType = 3;
}


void CPainter5View::OnEllipse()
{
	// TODO: 在此添加命令处理程序代码
	m_DrawType = 4;
}


void CPainter5View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_PointOrigin = point;

	CScrollView::OnLButtonDown(nFlags, point);
}


void CPainter5View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
#if 0 // 用这种写法，当改变窗口大小时，图像仍然消失了
	// 注：此时graph是一个局部变量，为它分配的内存在栈中，当这个函数结束时，对象也会被析构
	Graph graph(m_DrawType, m_PointOrigin, point);
	// 将graph对象保存到集合类的对象m_PtrArray当中
	m_PtrArray.Add(&graph);
#endif

	// 这也是一个局部的指针变量，内存在栈中
	Graph* pGraph;
	// 这一句调用它的构造函数为pGraph指针类型的变量在堆中分配一个内存空间，构造一个Graph类
	// 的对象，因为new所分配的内存都是在堆中分配的，在堆中分配的对象的内存，如果不去显式的调用
	// delete去释放这个对象的内存，那么这个对象的生命周期是和应用程序保持一致的
	pGraph = new Graph(m_DrawType, m_PointOrigin, point);
	m_PtrArray.Add(pGraph);

	CClientDC dc(this);
	// 创建透明画刷，不加此条的话，画出来的矩形和圆填充的白色而不是透明的
	CBrush* pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	dc.SelectObject(pBrush);

	switch (m_DrawType)
	{
	case 1:
		dc.SetPixel(point, RGB(0, 0, 0));
		break;
	case 2:
		dc.MoveTo(m_PointOrigin);
		dc.LineTo(point);
		break;
	case 3:
		dc.Rectangle(CRect(m_PointOrigin, point));
		break;
	case 4:
		dc.Ellipse(CRect(m_PointOrigin, point));
		break;
	default:
		break;
	}

	CScrollView::OnLButtonUp(nFlags, point);
}
