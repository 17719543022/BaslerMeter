
// Painter5View.cpp: CPainter5View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Painter1.h"
#endif

#include "Painter1Doc.h"
#include "Painter1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "SettingSizeDialog.h"
#include <vector>


// CPainter5View

IMPLEMENT_DYNCREATE(CPainter5View, CScrollView)

BEGIN_MESSAGE_MAP(CPainter5View, CScrollView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_DRAW_LINE_SEGMENT, &CPainter5View::OnDrawLineSegment)
	ON_COMMAND(ID_DRAW_RECTANGLE, &CPainter5View::OnDrawRectangle)
	ON_COMMAND(ID_DRAW_CIRCLE, &CPainter5View::OnDrawCircle)
	ON_COMMAND(ID_DRAW_ELLIPSE, &CPainter5View::OnDrawEllipse)
	ON_COMMAND(ID_SET_SIZE, &CPainter5View::OnSetSize)
	ON_COMMAND(ID_SET_COLOR, &CPainter5View::OnSetColor)
	ON_COMMAND(ID_DRAW_TEXT, &CPainter5View::OnDrawText)
	ON_COMMAND(ID_DRAW_PENCIL, &CPainter5View::OnDrawPencil)
	ON_COMMAND(ID_DRAW_ERASER, &CPainter5View::OnDrawEraser)
	ON_COMMAND(ID_FILE_OPEN, &CPainter5View::OnFileOpen)
	ON_COMMAND(ID_DRAW_MAGNIFY, &CPainter5View::OnDrawMagnify)
END_MESSAGE_MAP()

// CPainter5View 构造/析构

CPainter5View::CPainter5View() noexcept
{
	// TODO: 在此处添加构造代码
	m_PenSize = 1;
	m_PenColor = RGB(0, 0, 0);
	m_BrushColor = RGB(0, 0, 0);
	m_PointBegin = CPoint(0, 0);
	m_PointEnd = CPoint(0, 0);
	m_DrawType = DrawType::LineSegment;
	m_TextId = 10086;
	m_Edit = nullptr;
	m_MagnifyCenter = CPoint(0, 0);
	m_MagnifyWidth = 600;
	m_MagnifyHeight = 400;
}

CPainter5View::~CPainter5View()
{
	if (m_Edit != nullptr)
	{
		delete m_Edit;
		m_Edit = nullptr;
	}
}

BOOL CPainter5View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CScrollView::PreCreateWindow(cs);
}

// CPainter5View 绘图

void CPainter5View::OnDraw(CDC* /*pDC*/)
{
	CPainter5Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
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


void CPainter5View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_PointBegin = point;
	m_PointEnd = point;

	switch (m_DrawType)
	{
	case DrawType::Magnify:
	{
		CClientDC dc(this);
		CPen pen(PS_SOLID, 1, RGB(0, 0, 0));

		dc.SelectStockObject(NULL_BRUSH);
		dc.SelectObject(&pen);

		dc.SetROP2(R2_NOTXORPEN);

		CRect rectUndraw = GetMagnifyRect(m_PointEnd);
		dc.Rectangle(rectUndraw);

		m_PointEnd = point;

		break;
	}
	default:
		break;
	}

	CScrollView::OnLButtonDown(nFlags, point);
}


void CPainter5View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nFlags & MK_LBUTTON) {
		CClientDC dc(this);
		CPen cPen(PS_SOLID, m_PenSize, m_PenColor);
		dc.SelectObject(&cPen);

		switch (m_DrawType)
		{
		case DrawType::LineSegment:
			dc.SetROP2(R2_NOTXORPEN);
			
			dc.MoveTo(m_PointBegin);
			dc.LineTo(m_PointEnd);

			dc.MoveTo(m_PointBegin);
			dc.LineTo(point);

			m_PointEnd = point;
			break;
		case DrawType::Rectangle:
			dc.SetROP2(R2_NOTXORPEN);
			dc.SelectStockObject(NULL_BRUSH);

			dc.Rectangle(CRect(m_PointBegin, m_PointEnd));
			dc.Rectangle(CRect(m_PointBegin, point));

			m_PointEnd = point;
			break;
		case DrawType::Circle:
		{
			dc.SetROP2(R2_NOTXORPEN);
			dc.SelectStockObject(NULL_BRUSH);

			int diameter1 = m_PointEnd.y - m_PointBegin.y;

			if (m_PointEnd.x < m_PointBegin.x)
			{
				m_PointEnd.x = m_PointBegin.x - abs(diameter1);
			}
			else {
				m_PointEnd.x = m_PointBegin.x + abs(diameter1);
			}

			dc.Ellipse(CRect(m_PointBegin, m_PointEnd));

			int diameter2 = point.y - m_PointBegin.y;

			if (point.x < m_PointBegin.x)
			{
				m_PointEnd.x = m_PointBegin.x - abs(diameter2);
			}
			else {
				m_PointEnd.x = m_PointBegin.x + abs(diameter2);
			}
			m_PointEnd.y = point.y;

			dc.Ellipse(CRect(m_PointBegin, m_PointEnd));

			m_PointEnd = point;

			break;
		}
		case DrawType::Ellipse:
			dc.SetROP2(R2_NOTXORPEN);
			dc.SelectStockObject(NULL_BRUSH);

			dc.Ellipse(CRect(m_PointBegin, m_PointEnd));
			dc.Ellipse(CRect(m_PointBegin, point));

			m_PointEnd = point;

			break;
		case DrawType::Text:
		{
			if (nullptr != m_Edit)
			{
				delete m_Edit;
				m_Edit = nullptr;
			}

			CEdit* edit = new CEdit();
			edit->Create(WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(m_PointBegin, point), this, m_TextId);
			m_Edit = edit;

			m_Edit->ShowWindow(SW_SHOW);

			break;
		}
		case DrawType::Pencil:
		{
			m_PointBegin = m_PointEnd;
			m_PointEnd = point;

			dc.MoveTo(m_PointBegin);
			dc.LineTo(m_PointEnd);

			break;
		}
		case DrawType::Eraser:
		{
			COLORREF pColor = dc.GetBkColor();
			CPen pen(PS_SOLID, m_PenSize, pColor);
			dc.SelectObject(&pen);

			m_PointBegin = m_PointEnd;
			m_PointEnd = point;

			dc.MoveTo(m_PointBegin);
			dc.LineTo(m_PointEnd);

			break;
		}
		case DrawType::Magnify:
		{
			CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
			dc.SelectObject(&pen);

			dc.SetROP2(R2_NOTXORPEN);
			dc.SelectStockObject(NULL_BRUSH);

			CRect rectUndraw = GetMagnifyRect(m_PointEnd);
			dc.Rectangle(rectUndraw);

			CRect rectDraw = GetMagnifyRect(point);
			dc.Rectangle(rectDraw);

			m_PointEnd = point;

			break;
		}
		default:
			break;
		}
	}

	CScrollView::OnMouseMove(nFlags, point);
}


void CPainter5View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CClientDC dc(this);
	CPen cPen(PS_SOLID, m_PenSize, m_PenColor);
	dc.SelectObject(&cPen);

	switch (m_DrawType)
	{
	case DrawType::LineSegment:
		dc.MoveTo(m_PointBegin);
		dc.LineTo(point);

		break;
	case DrawType::Rectangle:
		dc.SelectStockObject(NULL_BRUSH);
		dc.Rectangle(CRect(m_PointBegin, m_PointEnd));
		break;
	case DrawType::Circle:
	{
		dc.SelectStockObject(NULL_BRUSH);

		int diameter = point.y - m_PointBegin.y;

		if (point.x < m_PointBegin.x)
		{
			m_PointEnd.x = m_PointBegin.x - abs(diameter);
		}
		else {
			m_PointEnd.x = m_PointBegin.x + abs(diameter);
		}
		m_PointEnd.y = point.y;

		dc.Ellipse(CRect(m_PointBegin, m_PointEnd));

		break;
	}
	case DrawType::Ellipse:
		dc.SelectStockObject(NULL_BRUSH);
		dc.Ellipse(CRect(m_PointBegin, m_PointEnd));
		break;
	case DrawType::Text:
	{
		if (nullptr != m_Edit)
		{
			delete m_Edit;
			m_Edit = nullptr;
		}

		CEdit* edit = new CEdit();
		edit->Create(WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(m_PointBegin, point), this, m_TextId);
		m_Edit = edit;

		m_Edit->ShowWindow(SW_SHOW);

		break;
	}
	case DrawType::Pencil:
	{
		m_PointBegin = m_PointEnd;
		m_PointEnd = point;

		dc.MoveTo(m_PointBegin);
		dc.LineTo(m_PointEnd);

		break;
	}
	case DrawType::Eraser:
	{
		COLORREF pColor = dc.GetBkColor();
		CPen pen(PS_SOLID, m_PenSize, pColor);
		dc.SelectObject(&pen);

		m_PointBegin = m_PointEnd;
		m_PointEnd = point;

		dc.MoveTo(m_PointBegin);
		dc.LineTo(m_PointEnd);

		break;
	}
	case DrawType::Magnify:
	{
		CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
		dc.SelectObject(&pen);

		dc.SetROP2(R2_NOTXORPEN);
		dc.SelectStockObject(NULL_BRUSH);

		CRect rectUndraw = GetMagnifyRect(m_PointEnd);
		dc.Rectangle(rectUndraw);

		m_PointEnd = point;

		CRect rect;
		GetClientRect(&rect);

		CDC* pCdc = GetDC();
		dc.StretchBlt(0, 0, rect.Width(), rect.Height(), pCdc, rectUndraw.left, rectUndraw.top, rectUndraw.Width(), rectUndraw.Height(), SRCCOPY);

		ReleaseDC(pCdc);

		break;
	}
	default:
		break;
	}

	CScrollView::OnLButtonUp(nFlags, point);
}


void CPainter5View::OnDrawLineSegment()
{
	// TODO: 在此添加命令处理程序代码
	m_DrawType = DrawType::LineSegment;
}


void CPainter5View::OnDrawRectangle()
{
	// TODO: 在此添加命令处理程序代码
	m_DrawType = DrawType::Rectangle;
}


void CPainter5View::OnDrawCircle()
{
	// TODO: 在此添加命令处理程序代码
	m_DrawType = DrawType::Circle;
}


void CPainter5View::OnDrawEllipse()
{
	// TODO: 在此添加命令处理程序代码
	m_DrawType = DrawType::Ellipse;
}


void CPainter5View::OnDrawText()
{
	// TODO: 在此添加命令处理程序代码
	m_DrawType = DrawType::Text;
}


void CPainter5View::OnSetSize()
{
	// TODO: 在此添加命令处理程序代码
	SettingSizeDialog dialog;
	dialog.m_Size = m_PenSize;

	if (IDOK == dialog.DoModal())
	{
		m_PenSize = dialog.m_Size;
	}
}


void CPainter5View::OnSetColor()
{
	// TODO: 在此添加命令处理程序代码
	CColorDialog dialog;

	if (IDOK == dialog.DoModal())
	{
		m_PenColor = dialog.GetColor();
	}
}




BOOL CPainter5View::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if (m_DrawType == DrawType::Text)
		{
			CString str;
			m_Edit->GetWindowTextW(str);
			delete m_Edit;
			m_Edit = nullptr;

			CClientDC dc(this);
			dc.TextOutW(m_PointBegin.x, m_PointBegin.y, str);

			return TRUE;
		}
	}

	return CScrollView::PreTranslateMessage(pMsg);
}


void CPainter5View::OnDrawPencil()
{
	// TODO: 在此添加命令处理程序代码
	m_DrawType = DrawType::Pencil;
}


void CPainter5View::OnDrawEraser()
{
	// TODO: 在此添加命令处理程序代码
	m_DrawType = DrawType::Eraser;
}


/*****最近邻算法实现图片的缩放*******此函数接受一个源位图和新的宽度和高度作为参数，返回一个新的位图，表示缩放后的结果****************/

HBITMAP CPainter5View::NearestNeighborResize(HBITMAP hBmpSrc, int newWidth, int newHeight)
{
	// 获取源位图的信息
	BITMAP bmpSrc;
	GetObject(hBmpSrc, sizeof(BITMAP), &bmpSrc);

	// 创建目标位图
	HDC hdc = GetDC()->GetSafeHdc();
	HBITMAP hBmpDest = CreateCompatibleBitmap(hdc, newWidth, newHeight);

	// 获取源位图和目标位图的数据
	int srcWidth = bmpSrc.bmWidth;
	int srcHeight = bmpSrc.bmHeight;
	int srcBytesPerPixel = bmpSrc.bmBitsPixel / 8;
	int srcBytesPerLine = ((srcWidth * srcBytesPerPixel + 3) / 4) * 4;
	int destBytesPerLine = ((newWidth * srcBytesPerPixel + 3) / 4) * 4;
	std::vector<BYTE> srcData(srcBytesPerLine * srcHeight);
	std::vector<BYTE> destData(destBytesPerLine * newHeight);
	GetBitmapBits(hBmpSrc, srcData.size(), srcData.data());

	// 执行最近邻缩放
	for (int y = 0; y < newHeight; y++)
	{
		for (int x = 0; x < newWidth; x++)
		{
			int srcX = x * srcWidth / newWidth;
			int srcY = y * srcHeight / newHeight;
			for (int i = 0; i < srcBytesPerPixel; i++)
			{
				destData[y * destBytesPerLine + x * srcBytesPerPixel + i] =
					srcData[srcY * srcBytesPerLine + srcX * srcBytesPerPixel + i];
			}
		}
	}
	// 设置目标位图的数据
	SetBitmapBits(hBmpDest, destData.size(), destData.data());
	return hBmpDest;
}


void CPainter5View::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	m_DrawType = DrawType::Reserved;

	CString filter, path;
	filter = "bmp图片（*.bmp）||";
	CFileDialog dialog(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter);

	if (IDOK == dialog.DoModal())
	{
		path = dialog.GetPathName();
	}

	HBITMAP hBitMap = (HBITMAP)::LoadImage(
		NULL
		, path
		, IMAGE_BITMAP
		, 0
		, 0
		, LR_LOADFROMFILE
	);

	CImage image;
	image.Attach(hBitMap);

	HBITMAP bBitMapDownScale = NearestNeighborResize(hBitMap, image.GetWidth() / 4, image.GetHeight() / 4);

	CBitmap loadImageBitMap;
	loadImageBitMap.Attach(bBitMapDownScale);

	BITMAP bitMap;
	loadImageBitMap.GetBitmap(&bitMap);

	CBrush brush;
	brush.CreatePatternBrush(&loadImageBitMap);

	CPen pen(PS_NULL, 1, RGB(0, 0, 0));

	CClientDC dc(this);
	dc.SelectObject(&brush);
	dc.SelectObject(&pen);

	dc.Rectangle(0, 0, bitMap.bmWidth, bitMap.bmHeight);
}


CRect CPainter5View::GetMagnifyRect(CPoint point)
{
	CRect rect;
	GetClientRect(&rect);

	CPoint topLeft, bottomRight;
	topLeft.x = point.x - m_MagnifyWidth / 2;
	bottomRight.x = point.x + m_MagnifyWidth / 2;
	topLeft.y = point.y - m_MagnifyHeight / 2;
	bottomRight.y = point.y + m_MagnifyHeight / 2;

	if (topLeft.x < 0)
	{
		topLeft.x = 0;
		bottomRight.x = topLeft.x + m_MagnifyWidth;
	}
	else if (bottomRight.x > rect.right)
	{
		bottomRight.x = rect.right;
		topLeft.x = bottomRight.x - m_MagnifyWidth;
	}

	if (topLeft.y < 0)
	{
		topLeft.y = 0;
		bottomRight.y = topLeft.y + m_MagnifyHeight;
	}
	else if (bottomRight.y > rect.bottom)
	{
		bottomRight.y = rect.bottom;
		topLeft.y = bottomRight.y - m_MagnifyHeight;
	}


	return CRect(topLeft, bottomRight);
}


void CPainter5View::OnDrawMagnify()
{
	// TODO: 在此添加命令处理程序代码
	m_DrawType = DrawType::Magnify;
}
