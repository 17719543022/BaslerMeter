
// BaslerMeterView.cpp: CBaslerMeterView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "BaslerMeter.h"
#endif

#include "BaslerMeterDoc.h"
#include "BaslerMeterView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "MainFrm.h"
#include "CCaptureSetting.h"


// CBaslerMeterView


CPen penInActive(PS_SOLID, 1, RGB(170, 0, 0));
CPen penActive(PS_SOLID, 1, RGB(255, 0, 0));
CPen penNull(PS_NULL, 1, RGB(0, 0, 0));
// pointTemp11、pointTemp12作为拖动手动画的那根线时用
CPoint pointTemp11 = CPoint(0, 0);
CPoint pointTemp12 = CPoint(0, 0);
// pointTemp21、pointTemp22作为拖动复制出来的那根线时用
CPoint pointTemp21 = CPoint(0, 0);
CPoint pointTemp22 = CPoint(0, 0);
CPoint pointBeCloseStart = CPoint(0, 0);

IMPLEMENT_DYNCREATE(CBaslerMeterView, CView)

BEGIN_MESSAGE_MAP(CBaslerMeterView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CAPTURE, &CBaslerMeterView::OnCapture)
	ON_COMMAND(ID_DISTANCE1, &CBaslerMeterView::OnDistance1)
	ON_COMMAND(ID_DISTANCE2, &CBaslerMeterView::OnDistance2)
	ON_COMMAND(ID_DISTANCE3, &CBaslerMeterView::OnDistance3)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_FILE_OPEN, &CBaslerMeterView::OnFileOpen)
	ON_COMMAND(ID_CAPTURE_SETTING, &CBaslerMeterView::OnCaptureSetting)
	ON_COMMAND(ID_VIDEO, &CBaslerMeterView::OnVideo)
END_MESSAGE_MAP()


void CBaslerMeterView::InitiateVariables()
{
	m_Point1 = CPoint(0, 0);
	m_Point2 = CPoint(0, 0);
	m_Point3 = CPoint(0, 0);
	m_PointParallel1 = CPoint(0, 0);
	m_PointParallel2 = CPoint(0, 0);
	m_DrawType = DrawType::Reserved;
	m_Distance3PointCount = 0;
	m_InActiveFlagForDistance2 = 1;
	m_PointBeCloseStart = CPoint(0, 0);
	m_IsPointClose = FALSE;
}


void CBaslerMeterView::InitiateBackboard()
{
	// 清空画板
	CClientDC dc(this);
	CBrush brush(RGB(255, 255, 255));
	dc.SelectObject(brush);
	dc.SelectObject(&penNull);

	CRect rect;
	GetClientRect(&rect);
	dc.Rectangle(rect);

	// 如果之前抓过图，把图保留下来
	BOOL isCaptured = IsFoundByDrawType(DrawType::Capture);
	if (isCaptured)
	{
		m_PaintedArray.clear();
		m_PaintedArray.push_back(CBaslerMeterRePaint(CPoint(0, 0), CPoint(0, 0), CPoint(0, 0), CPoint(0, 0), CPoint(0, 0), DrawType::Capture, 0, CRect(), 0));

		LoadGrabbedImage();
	}
	else {
		m_PaintedArray.clear();
	}
}


void CBaslerMeterView::GetParallelPoints(CPoint m_Point1, CPoint m_Point2, CPoint& pointParallel1, CPoint& pointParallel2)
{
	CRect rect;
	GetClientRect(&rect);

	if (abs(m_Point1.x - m_Point2.x) > abs(m_Point1.y - m_Point2.y))
	{
		if (m_Point1.y > rect.Height() / 2)
		{
			pointParallel1.x = m_Point1.x;
			pointParallel1.y = m_Point1.y - 50;

			pointParallel2.x = m_Point2.x;
			pointParallel2.y = m_Point2.y - 50;
		}
		else
		{
			pointParallel1.x = m_Point1.x;
			pointParallel1.y = m_Point1.y + 50;

			pointParallel2.x = m_Point2.x;
			pointParallel2.y = m_Point2.y + 50;
		}
	}
	else
	{
		if (m_Point1.x > rect.Width() / 2)
		{
			pointParallel1.x = m_Point1.x - 50;
			pointParallel1.y = m_Point1.y;

			pointParallel2.x = m_Point2.x - 50;
			pointParallel2.y = m_Point2.y;
		}
		else
		{
			pointParallel1.x = m_Point1.x + 50;
			pointParallel1.y = m_Point1.y;

			pointParallel2.x = m_Point2.x + 50;
			pointParallel2.y = m_Point2.y;
		}
	}
}


BOOL CBaslerMeterView::IsFoundByDrawType(DrawType m_DrawType)
{
	BOOL isFound = FALSE;

	for (int i = 0; i < m_PaintedArray.size(); i++)
	{
		if (m_PaintedArray.at(i).m_DrawType == m_DrawType)
		{
			isFound = TRUE;
			break;
		}
	}

	return isFound;
}


BOOL CBaslerMeterView::IsPointCloseToLine(CPoint point, CPoint lineHead, CPoint lineTail)
{
	double distanceToHead = sqrt(pow((lineHead.x - point.x), 2) + pow((lineHead.y - point.y), 2));
	double distanceToTail = sqrt(pow((lineTail.x - point.x), 2) + pow((lineTail.y - point.y), 2));
	double lengthOfLine = sqrt(pow((lineTail.x - lineHead.x), 2) + pow((lineTail.y - lineHead.y), 2));

	return (distanceToHead + distanceToTail - lengthOfLine) < 0.8;
}


void CBaslerMeterView::SetPixelWithCross(CClientDC& dc, CPoint point)
{
	dc.SetPixel(point, RGB(255, 0, 0));
	dc.SetPixel(CPoint(point.x - 1, point.y), RGB(255, 0, 0));
	dc.SetPixel(CPoint(point.x + 1, point.y), RGB(255, 0, 0));
	dc.SetPixel(CPoint(point.x, point.y - 1), RGB(255, 0, 0));
	dc.SetPixel(CPoint(point.x, point.y + 1), RGB(255, 0, 0));
}


void CBaslerMeterView::SetPixelWithCross(CDC* pDC, CPoint point)
{
	pDC->SetPixel(point, RGB(255, 0, 0));
	pDC->SetPixel(CPoint(point.x - 1, point.y), RGB(255, 0, 0));
	pDC->SetPixel(CPoint(point.x + 1, point.y), RGB(255, 0, 0));
	pDC->SetPixel(CPoint(point.x, point.y - 1), RGB(255, 0, 0));
	pDC->SetPixel(CPoint(point.x, point.y + 1), RGB(255, 0, 0));
}


void CBaslerMeterView::ThreePointsCircle(CPoint m_Point1, CPoint m_Point2, CPoint m_Point3, CRect& rect, double& diameter)
{
	double vertical12_x = (m_Point1.x + m_Point2.x) / 2.0f;
	double vertical12_y = (m_Point1.y + m_Point2.y) / 2.0f;
	double vertical13_x = (m_Point1.x + m_Point3.x) / 2.0f;
	double vertical13_y = (m_Point1.y + m_Point3.y) / 2.0f;
	double verticalTemp_x = 0.0f;
	double verticalTemp_y = 0.0f;

	// 斜率的负倒数
	double negativeReciprocalSlope12 = 1.0f;
	double negativeReciprocalSlope13 = 1.0f;
	// 平行于X轴或者Y轴的情况
	BOOL isInfinitySlope12 = FALSE;
	BOOL isInfinitySlope13 = FALSE;

	if (m_Point2.y == m_Point1.y)
	{
		isInfinitySlope12 = TRUE;
	}
	else
	{
		negativeReciprocalSlope12 = -((1.0f * (m_Point2.x - m_Point1.x)) / (1.0f * (m_Point2.y - m_Point1.y)));
	}

	if (m_Point3.y == m_Point1.y)
	{
		isInfinitySlope13 = TRUE;
	}
	else
	{
		negativeReciprocalSlope13 = -((1.0f * (m_Point3.x - m_Point1.x)) / (1.0f * (m_Point3.y - m_Point1.y)));
	}

	double errorDistance = sqrt(pow((vertical12_x - vertical13_x), 2) + pow((vertical12_y - vertical13_y), 2));
	double errorDistanceTemp = 0.0f;
	double errorDistanceThrs = 1.2f;
	double step = 10.0f;

	int iterationCount = 0;

	do
	{
		if (errorDistance > 50)
		{
			step = 10.0f;
			iterationCount = 0;
		}
		else if (errorDistance > 5)
		{
			step = 1.0f;
			iterationCount = 0;
		}
		else
		{
			step = 0.1f;
		}

		iterationCount = iterationCount + 1;

		if (abs(m_Point2.x - m_Point1.x) > abs(m_Point2.y - m_Point1.y))
		{
			verticalTemp_x = isInfinitySlope12 ? vertical12_x : (vertical12_x + step * (1 / negativeReciprocalSlope12));
			verticalTemp_y = vertical12_y + step;
		}
		else
		{
			verticalTemp_x = vertical12_x + step;
			verticalTemp_y = isInfinitySlope12 ? vertical12_y : (vertical12_y + step * negativeReciprocalSlope12);
		}

		errorDistanceTemp = sqrt(pow((verticalTemp_x - vertical13_x), 2) + pow((verticalTemp_y - vertical13_y), 2));

		if (errorDistanceTemp < errorDistance)
		{
			vertical12_x = verticalTemp_x;
			vertical12_y = verticalTemp_y;

			errorDistance = errorDistanceTemp;
		}
		else
		{
			if (abs(m_Point2.x - m_Point1.x) > abs(m_Point2.y - m_Point1.y))
			{
				vertical12_x = isInfinitySlope12 ? vertical12_x : (vertical12_x - step * (1 / negativeReciprocalSlope12));
				vertical12_y = vertical12_y - step;
			}
			else
			{
				vertical12_x = vertical12_x - step;
				vertical12_y = isInfinitySlope12 ? vertical12_y : (vertical12_y - step * negativeReciprocalSlope12);
			}

			errorDistanceTemp = sqrt(pow((vertical12_x - vertical13_x), 2) + pow((vertical12_y - vertical13_y), 2));

			errorDistance = errorDistanceTemp;
		}

		if (abs(m_Point3.x - m_Point1.x) > abs(m_Point3.y - m_Point1.y))
		{
			verticalTemp_x = isInfinitySlope13 ? vertical13_x : (vertical13_x + step * (1 / negativeReciprocalSlope13));
			verticalTemp_y = vertical13_y + step;
		}
		else
		{
			verticalTemp_x = vertical13_x + step;
			verticalTemp_y = isInfinitySlope13 ? vertical13_y : (vertical13_y + step * negativeReciprocalSlope13);
		}

		errorDistanceTemp = sqrt(pow((verticalTemp_x - vertical12_x), 2) + pow((verticalTemp_y - vertical12_y), 2));

		if (errorDistanceTemp < errorDistance)
		{
			vertical13_x = verticalTemp_x;
			vertical13_y = verticalTemp_y;

			errorDistance = errorDistanceTemp;
		}
		else
		{
			if (abs(m_Point3.x - m_Point1.x) > abs(m_Point3.y - m_Point1.y))
			{
				vertical13_x = isInfinitySlope13 ? vertical13_x : (vertical13_x - step * (1 / negativeReciprocalSlope13));
				vertical13_y = vertical13_y - step;
			}
			else
			{
				vertical13_x = vertical13_x - step;
				vertical13_y = isInfinitySlope13 ? vertical13_y : (vertical13_y - step * negativeReciprocalSlope13);
			}

			errorDistanceTemp = sqrt(pow((vertical13_x - vertical12_x), 2) + pow((vertical13_y - vertical12_y), 2));

			errorDistance = errorDistanceTemp;
		}

	} while (errorDistance > errorDistanceThrs || (iterationCount < 200));

	CPoint point;
	point.x = round(vertical12_x);
	point.y = round(vertical12_y);

	double radius = sqrt(pow((vertical12_x - m_Point1.x), 2) + pow((vertical12_y - m_Point1.y), 2));

	rect.SetRect(CPoint(round(vertical12_x - radius), round(vertical12_y - radius)), CPoint(round(vertical12_x + radius), round(vertical12_y + radius)));
	diameter = 2 * radius;
}


// CBaslerMeterView 构造/析构

CBaslerMeterView::CBaslerMeterView() noexcept
{
	// TODO: 在此处添加构造代码
	InitiateVariables();

	m_ScaleForDisplay = 0.25;
	m_Magnification = 6.0f;
	m_Scale = 2.2f;
}

CBaslerMeterView::~CBaslerMeterView()
{
}

BOOL CBaslerMeterView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CBaslerMeterView 绘图

void CBaslerMeterView::OnDraw(CDC* pDC)
{
	CBaslerMeterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	int sizeOfPainted = m_PaintedArray.size();

	// TODO: 在此处为本机数据添加绘制代码
	// 将保存的图形对象取出来
	for (int i = 0; i < m_PaintedArray.size(); i++)
	{
		if (m_PaintedArray.at(i).m_DrawType == DrawType::Capture)
		{
			LoadGrabbedImage();

			break;
		}
	}

	// 创建透明画刷，不加此条的话，画出来的矩形和圆填充的白色而不是透明的
	CBrush* pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	pDC->SelectObject(pBrush);
	pDC->SelectObject(&penActive);

	for (int i = 0; i < m_PaintedArray.size(); i++)
	{
		if (m_PaintedArray.at(i).m_DrawType == DrawType::Distance1)
		{
			SetPixelWithCross(pDC, m_Point1);
			pDC->MoveTo(m_PaintedArray.at(i).m_Point1);
			pDC->LineTo(m_PaintedArray.at(i).m_Point2);
			SetPixelWithCross(pDC, m_Point2);
		}

		if (m_PaintedArray.at(i).m_DrawType == DrawType::Distance2)
		{
			if (m_InActiveFlagForDistance2 == 1)
			{
				pDC->SelectObject(penInActive);
				SetPixelWithCross(pDC, m_PaintedArray.at(i).m_Point1);
				pDC->MoveTo(m_PaintedArray.at(i).m_Point1);
				pDC->LineTo(m_PaintedArray.at(i).m_Point2);
				SetPixelWithCross(pDC, m_PaintedArray.at(i).m_Point2);

				pDC->SelectObject(penActive);
				SetPixelWithCross(pDC, m_PaintedArray.at(i).m_PointParallel1);
				pDC->MoveTo(m_PaintedArray.at(i).m_PointParallel1);
				pDC->LineTo(m_PaintedArray.at(i).m_PointParallel2);
				SetPixelWithCross(pDC, m_PaintedArray.at(i).m_PointParallel2);
			}
			else
			{
				pDC->SelectObject(penActive);
				SetPixelWithCross(pDC, m_PaintedArray.at(i).m_Point1);
				pDC->MoveTo(m_PaintedArray.at(i).m_Point1);
				pDC->LineTo(m_PaintedArray.at(i).m_Point2);
				SetPixelWithCross(pDC, m_PaintedArray.at(i).m_Point2);

				pDC->SelectObject(penInActive);
				SetPixelWithCross(pDC, m_PaintedArray.at(i).m_PointParallel1);
				pDC->MoveTo(m_PaintedArray.at(i).m_PointParallel1);
				pDC->LineTo(m_PaintedArray.at(i).m_PointParallel2);
				SetPixelWithCross(pDC, m_PaintedArray.at(i).m_PointParallel2);
				pDC->SelectObject(penActive);
			}
		}

		if (m_PaintedArray.at(i).m_DrawType == DrawType::Distance3)
		{
			SetPixelWithCross(pDC, m_PaintedArray.at(i).m_Point1);
			SetPixelWithCross(pDC, m_PaintedArray.at(i).m_Point2);
			SetPixelWithCross(pDC, m_PaintedArray.at(i).m_Point3);

			pDC->Ellipse(m_PaintedArray.at(i).rect);
		}
	}
}


// CBaslerMeterView 打印

BOOL CBaslerMeterView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CBaslerMeterView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CBaslerMeterView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CBaslerMeterView 诊断

#ifdef _DEBUG
void CBaslerMeterView::AssertValid() const
{
	CView::AssertValid();
}

void CBaslerMeterView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBaslerMeterDoc* CBaslerMeterView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBaslerMeterDoc)));
	return (CBaslerMeterDoc*)m_pDocument;
}
#endif //_DEBUG


//extern int GrubAndSave();

void CBaslerMeterView::LoadGrabbedImage()
{
	HBITMAP hBitMap = (HBITMAP)::LoadImage(
		NULL
		, m_Path
		, IMAGE_BITMAP
		, 0
		, 0
		, LR_LOADFROMFILE
	);

	CImage image;
	image.Attach(hBitMap);

	HDC hdc = GetDC()->GetSafeHdc();
	//HBITMAP bBitMapDownScale = NearestNeighborResize(hdc, hBitMap, image.GetWidth() * m_ScaleForDisplay, image.GetHeight() * m_ScaleForDisplay);
	//HBITMAP bBitMapDownScale = hBitMap;

	CBitmap cBitMap;
	cBitMap.Detach();
	cBitMap.Attach(hBitMap);

	BITMAP bitMap;
	cBitMap.GetBitmap(&bitMap);

	CBrush brush;
	brush.CreatePatternBrush(&cBitMap);

	CClientDC dc(this);
	dc.SelectObject(&brush);
	dc.SelectObject(&penNull);

	dc.Rectangle(0, 0, bitMap.bmWidth, bitMap.bmHeight);
}


//extern HBITMAP NearestNeighborResize(HDC hdc, HBITMAP hBmpSrc, int newWidth, int newHeight);

void CBaslerMeterView::OnCapture()
{
//	// TODO: 在此添加命令处理程序代码
//	m_DrawType = DrawType::Capture;
//
//	// 使用brush方式
//	int exitCode = GrubAndSave();
//
//	if (exitCode != 0)
//		return;
//
//	InitiateVariables();
//	InitiateBackboard();
//
//	HBITMAP hBitMap = (HBITMAP)::LoadImage(
//		NULL
//		, _T("GrabbedImage.bmp")
//		, IMAGE_BITMAP
//		, 0
//		, 0
//		, LR_LOADFROMFILE
//	);
//
//	CImage image;
//	image.Attach(hBitMap);
//
//	HDC hdc = GetDC()->GetSafeHdc();
//	HBITMAP bBitMapDownScale = NearestNeighborResize(hdc, hBitMap, image.GetWidth() * m_ScaleForDisplay, image.GetHeight() * m_ScaleForDisplay);
//
//	m_Bitmap.Detach();
//	m_Bitmap.Attach(bBitMapDownScale);
//
//	LoadGrabbedImage();
//
//	// TODO: 在此添加命令处理程序代码
//	m_PaintedArray.push_back(CBaslerMeterRePaint(CPoint(0, 0), CPoint(0, 0), CPoint(0, 0), CPoint(0, 0), CPoint(0, 0), DrawType::Capture, 0, CRect(), 0));
}


void CBaslerMeterView::OnDistance1()
{
	// TODO: 在此添加命令处理程序代码
	if (!IsFoundByDrawType(DrawType::Capture))
	{
		MessageBox(_T("请确认已经成功捕获图像后开始测量！"));
	}
	else
	{
		InitiateVariables();
		InitiateBackboard();

		m_DrawType = DrawType::Distance1;
	}
}


void CBaslerMeterView::OnDistance2()
{
	// TODO: 在此添加命令处理程序代码
	if (!IsFoundByDrawType(DrawType::Capture))
	{
		MessageBox(_T("请确认已经成功捕获图像后开始测量！"));
	}
	else
	{
		InitiateVariables();
		InitiateBackboard();

		m_DrawType = DrawType::Distance2;
	}
}


void CBaslerMeterView::OnDistance3()
{
	// TODO: 在此添加命令处理程序代码
	if (!IsFoundByDrawType(DrawType::Capture))
	{
		MessageBox(_T("请确认已经成功捕获图像后开始测量！"));
	}
	else
	{
		InitiateVariables();
		InitiateBackboard();

		m_DrawType = DrawType::Distance3;
	}
}


void CBaslerMeterView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_DrawType == DrawType::Distance1)
	{
		if (IsFoundByDrawType(DrawType::Distance1))
		{
			InitiateVariables();
			InitiateBackboard();

			m_DrawType = DrawType::Distance1;
		}
		m_Point1 = point;
		m_Point2 = point;
	}

	if (m_DrawType == DrawType::Distance2)
	{
		if (IsFoundByDrawType(DrawType::Distance2))
		{
			m_DrawType = DrawType::Distance2;

			CClientDC dc(this);

			if (IsPointCloseToLine(point, m_Point1, m_Point2))
			{
				m_InActiveFlagForDistance2 = 2;
				m_PointBeCloseStart = point;
				pointBeCloseStart = point;
				m_IsPointClose = TRUE;
				pointTemp11 = m_Point1;
				pointTemp12 = m_Point2;

				dc.SelectObject(penActive);
				SetPixelWithCross(dc, m_Point1);
				dc.MoveTo(m_Point1);
				dc.LineTo(m_Point2);
				SetPixelWithCross(dc, m_Point2);

				dc.SelectObject(penInActive);
				SetPixelWithCross(dc, m_PointParallel1);
				dc.MoveTo(m_PointParallel1);
				dc.LineTo(m_PointParallel2);
				SetPixelWithCross(dc, m_PointParallel2);
				dc.SelectObject(penActive);
			}
			else if (IsPointCloseToLine(point, m_PointParallel1, m_PointParallel2))
			{
				m_InActiveFlagForDistance2 = 1;
				m_PointBeCloseStart = point;
				pointBeCloseStart = point;
				m_IsPointClose = TRUE;
				pointTemp21 = m_PointParallel1;
				pointTemp22 = m_PointParallel2;

				dc.SelectObject(penInActive);
				SetPixelWithCross(dc, m_Point1);
				dc.MoveTo(m_Point1);
				dc.LineTo(m_Point2);
				SetPixelWithCross(dc, m_Point2);

				dc.SelectObject(penActive);
				SetPixelWithCross(dc, m_PointParallel1);
				dc.MoveTo(m_PointParallel1);
				dc.LineTo(m_PointParallel2);
				SetPixelWithCross(dc, m_PointParallel2);
			}
			else
			{
				InitiateVariables();
				InitiateBackboard();
				m_DrawType = DrawType::Distance2;

				m_Point1 = point;
				m_Point2 = point;
			}
		}
		else
		{
			m_Point1 = point;
			m_Point2 = point;
		}
	}

	if (m_DrawType == DrawType::Distance3)
	{
		// 以点下后松开的位置为最终打点位置
		if (m_Distance3PointCount == 3)
		{
			InitiateVariables();
			InitiateBackboard();

			m_DrawType = DrawType::Distance3;
		}
		if (m_Distance3PointCount == 2)
		{
			m_Point3 = point;
			m_Distance3PointCount = 3;
		}
		if (m_Distance3PointCount == 1)
		{
			m_Point2 = point;
			m_Distance3PointCount = 2;
		}
		if (m_Distance3PointCount == 0)
		{
			m_Point1 = point;
			m_Distance3PointCount = 1;
		}
	}

	CView::OnLButtonDown(nFlags, point);
}


void CBaslerMeterView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nFlags & MK_LBUTTON)
	{
		if (m_DrawType == DrawType::Distance1)
		{
			CClientDC dc(this);

			dc.SelectStockObject(NULL_BRUSH);
			dc.SelectObject(&penActive);

			dc.SetROP2(R2_NOTXORPEN);

			SetPixelWithCross(dc, m_Point1);
			dc.MoveTo(m_Point1);
			dc.LineTo(m_Point2);
			SetPixelWithCross(dc, m_Point2);

			SetPixelWithCross(dc, m_Point1);
			dc.MoveTo(m_Point1);
			dc.LineTo(point);
			SetPixelWithCross(dc, point);

			m_Point2 = point;
		}

		if (m_DrawType == DrawType::Distance2)
		{
			if (m_IsPointClose)
			{
				CClientDC dc(this);
				dc.SelectStockObject(NULL_BRUSH);

				if (m_PointBeCloseStart == pointBeCloseStart)
				{
					InitiateBackboard();

					if (m_InActiveFlagForDistance2 == 1)
					{
						dc.SelectObject(&penInActive);
						SetPixelWithCross(dc, m_Point1);
						dc.MoveTo(m_Point1);
						dc.LineTo(m_Point2);
						SetPixelWithCross(dc, m_Point2);
					}
					else
					{
						dc.SelectObject(&penInActive);
						SetPixelWithCross(dc, m_PointParallel1);
						dc.MoveTo(m_PointParallel1);
						dc.LineTo(m_PointParallel2);
						SetPixelWithCross(dc, m_PointParallel2);
					}
				}

				dc.SetROP2(R2_NOTXORPEN);
				dc.SelectObject(&penActive);

				if (m_PointBeCloseStart == pointBeCloseStart)
				{
					if (m_InActiveFlagForDistance2 == 1)
					{
						SetPixelWithCross(dc, m_PointParallel1);
						dc.MoveTo(m_PointParallel1);
						dc.LineTo(m_PointParallel2);
						SetPixelWithCross(dc, m_PointParallel2);
					}
					else
					{
						SetPixelWithCross(dc, m_Point1);
						dc.MoveTo(m_Point1);
						dc.LineTo(m_Point2);
						SetPixelWithCross(dc, m_Point2);
					}
				}

				if (m_InActiveFlagForDistance2 == 1)
				{
					CPoint pointParallel1 = CPoint(0, 0);
					CPoint pointParallel2 = CPoint(0, 0);

					pointParallel1 = pointTemp21 + point - m_PointBeCloseStart;
					pointParallel2 = pointTemp22 + point - m_PointBeCloseStart;

					SetPixelWithCross(dc, pointTemp21);
					dc.MoveTo(pointTemp21);
					dc.LineTo(pointTemp22);
					SetPixelWithCross(dc, pointTemp22);

					SetPixelWithCross(dc, pointParallel1);
					dc.MoveTo(pointParallel1);
					dc.LineTo(pointParallel2);
					SetPixelWithCross(dc, pointParallel2);

					pointTemp21 = pointParallel1;
					pointTemp22 = pointParallel2;
				}
				else
				{
					CPoint point1 = CPoint(0, 0);
					CPoint point2 = CPoint(0, 0);

					point1 = pointTemp11 + point - m_PointBeCloseStart;
					point2 = pointTemp12 + point - m_PointBeCloseStart;

					SetPixelWithCross(dc, pointTemp11);
					dc.MoveTo(pointTemp11);
					dc.LineTo(pointTemp12);
					SetPixelWithCross(dc, pointTemp12);

					SetPixelWithCross(dc, point1);
					dc.MoveTo(point1);
					dc.LineTo(point2);
					SetPixelWithCross(dc, point2);

					pointTemp11 = point1;
					pointTemp12 = point2;
				}

				m_PointBeCloseStart = point;
			}
			else
			{
				CClientDC dc(this);

				dc.SelectStockObject(NULL_BRUSH);
				dc.SelectObject(&penActive);
				dc.SetROP2(R2_NOTXORPEN);

				SetPixelWithCross(dc, m_Point1);
				dc.MoveTo(m_Point1);
				dc.LineTo(m_Point2);
				SetPixelWithCross(dc, m_Point2);

				SetPixelWithCross(dc, m_Point1);
				dc.MoveTo(m_Point1);
				dc.LineTo(point);
				SetPixelWithCross(dc, point);

				m_Point2 = point;
			}
		}

		if (m_DrawType == DrawType::Distance3)
		{
			// 以点下后松开的位置为最终打点位置
			// 因此，只在OnLButtonUp中处理即可
		}
	}

	CView::OnMouseMove(nFlags, point);
}


int CBaslerMeterView::ConverToStatusBar1()
{
	double distance = sqrt(pow((m_Point2.x - m_Point1.x), 2) + pow((m_Point2.y - m_Point1.y), 2));

	return round(1 / m_ScaleForDisplay * distance * m_Scale / m_Magnification);
}


int CBaslerMeterView::ConverToStatusBar2()
{
	double distanceToHead = sqrt(pow((m_PointParallel1.x - m_Point1.x), 2) + pow((m_PointParallel1.y - m_Point1.y), 2));
	double distanceToTail = sqrt(pow((m_PointParallel2.x - m_Point1.x), 2) + pow((m_PointParallel2.y - m_Point1.y), 2));
	double lengthOfLine = sqrt(pow((m_PointParallel2.x - m_PointParallel1.x), 2) + pow((m_PointParallel2.y - m_PointParallel1.y), 2));

	// 通过海伦公式求三角形的面积
	double p = (distanceToHead + distanceToTail + lengthOfLine) * 0.5;
	double area = sqrt(p * (p - distanceToHead) * (p - distanceToTail) * (p - lengthOfLine));
	double distance = area / lengthOfLine * 2;

	return round(1 / m_ScaleForDisplay * distance * m_Scale / m_Magnification);
}


int CBaslerMeterView::ConverToStatusBar3(double diameter)
{
	return round(1 / m_ScaleForDisplay * diameter * m_Scale / m_Magnification);
}


void CBaslerMeterView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CClientDC dc(this);

	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar* pBar = &(pFrame->m_wndStatusBar);
	CString str;

	dc.SelectStockObject(NULL_BRUSH);
	dc.SelectObject(&penActive);

	if (m_DrawType == DrawType::Distance1)
	{
		SetPixelWithCross(dc, m_Point1);
		dc.MoveTo(m_Point1);
		dc.LineTo(m_Point2);
		SetPixelWithCross(dc, m_Point2);

		int distance1 = ConverToStatusBar1();
		str.Format(_T("两点の距：%d"), distance1);

		pBar->SetPaneText(1, str);
	}

	if (m_DrawType == DrawType::Distance2)
	{
		if (m_IsPointClose)
		{
			if (m_InActiveFlagForDistance2 == 1)
			{
				m_PointParallel1 = pointTemp21;
				m_PointParallel2 = pointTemp22;
			}
			else
			{
				m_Point1 = pointTemp11;
				m_Point2 = pointTemp12;
			}
		}
		else
		{
			GetParallelPoints(m_Point1, m_Point2, m_PointParallel1, m_PointParallel2);
		}

		if (m_InActiveFlagForDistance2 == 1)
		{
			dc.SelectObject(penInActive);
			SetPixelWithCross(dc, m_Point1);
			dc.MoveTo(m_Point1);
			dc.LineTo(m_Point2);
			SetPixelWithCross(dc, m_Point2);

			dc.SelectObject(penActive);
			SetPixelWithCross(dc, m_PointParallel1);
			dc.MoveTo(m_PointParallel1);
			dc.LineTo(m_PointParallel2);
			SetPixelWithCross(dc, m_PointParallel2);
		}
		else
		{
			dc.SelectObject(penActive);
			SetPixelWithCross(dc, m_Point1);
			dc.MoveTo(m_Point1);
			dc.LineTo(m_Point2);
			SetPixelWithCross(dc, m_Point2);

			dc.SelectObject(penInActive);
			SetPixelWithCross(dc, m_PointParallel1);
			dc.MoveTo(m_PointParallel1);
			dc.LineTo(m_PointParallel2);
			SetPixelWithCross(dc, m_PointParallel2);
			dc.SelectObject(penActive);
		}

		CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
		CStatusBar* pBar = &(pFrame->m_wndStatusBar);

		CString str;
		int distance2 = ConverToStatusBar2();
		str.Format(_T("平行线距：%d"), distance2);

		pBar->SetPaneText(1, str);
	}

	if (m_DrawType == DrawType::Distance3)
	{
		if (m_Distance3PointCount == 3)
		{
			m_Point3 = point;
		}
		if (m_Distance3PointCount == 2)
		{
			m_Point2 = point;
		}
		if (m_Distance3PointCount == 1)
		{
			m_Point1 = point;
		}

		if (m_Distance3PointCount == 1)
		{
			SetPixelWithCross(dc, m_Point1);
		}
		if (m_Distance3PointCount == 2)
		{
			SetPixelWithCross(dc, m_Point2);
		}
		if (m_Distance3PointCount == 3)
		{
			SetPixelWithCross(dc, m_Point3);

			CRect rect;
			double diameter = 0.0f;
			ThreePointsCircle(m_Point1, m_Point2, m_Point3, rect, diameter);

			InitiateVariables();
			InitiateBackboard();
			m_DrawType = DrawType::Distance3;
			m_Distance3PointCount = 3;

			dc.Ellipse(rect);

			str.Format(_T("圆周直径：%d"), ConverToStatusBar3(diameter));
			pBar->SetPaneText(1, str);

			m_PaintedArray.push_back(CBaslerMeterRePaint(m_Point1, m_Point2, m_Point3, CPoint(0, 0), CPoint(0, 0), DrawType::Distance3, 3, rect, diameter));
		}
	}

	m_PaintedArray.push_back(CBaslerMeterRePaint(m_Point1, m_Point2, m_Point3, m_PointParallel1, m_PointParallel2, m_DrawType, m_Distance3PointCount, CRect(), 0));

	CView::OnLButtonUp(nFlags, point);
}


void CBaslerMeterView::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	CString filter;
	filter = "bmp图片（*.bmp）||";
	CFileDialog dialog(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter);

	if (IDOK == dialog.DoModal())
	{
		m_Path = dialog.GetPathName();
	}
	else
	{
		return;
	}

	HBITMAP hBitMap = (HBITMAP)::LoadImage(
		NULL
		, m_Path
		, IMAGE_BITMAP
		, 0
		, 0
		, LR_LOADFROMFILE
	);

	CImage image;
	image.Attach(hBitMap);

	HDC hdc = GetDC()->GetSafeHdc();
	//HBITMAP bBitMapDownScale = NearestNeighborResize(hdc, hBitMap, image.GetWidth() * m_ScaleForDisplay, image.GetHeight() * m_ScaleForDisplay);
	//HBITMAP bBitMapDownScale = hBitMap;

	CBitmap cBitMap;
	cBitMap.Detach();
	cBitMap.Attach(hBitMap);

	BITMAP bitMap;
	cBitMap.GetBitmap(&bitMap);

	CBrush brush;
	brush.CreatePatternBrush(&cBitMap);

	CClientDC dc(this);
	dc.SelectObject(&brush);
	dc.SelectObject(&penNull);

	dc.Rectangle(0, 0, bitMap.bmWidth, bitMap.bmHeight);

	// TODO: 在此添加命令处理程序代码
	m_PaintedArray.push_back(CBaslerMeterRePaint(CPoint(0, 0), CPoint(0, 0), CPoint(0, 0), CPoint(0, 0), CPoint(0, 0), DrawType::Capture, 0, CRect(), 0));
}


void CBaslerMeterView::OnCaptureSetting()
{
	// TODO: 在此添加命令处理程序代码
	CCaptureSetting dialog;
	CString str;

	str.Format(_T("%d"), m_Magnification);
	dialog.m_Magnification = str;
	str.Format(_T("%.1lf"), m_Scale);
	dialog.m_Scale = str;

	if (IDOK == dialog.DoModal())
	{
		m_Magnification = _ttoi(dialog.m_Magnification);
		m_Scale = _tstof((LPCTSTR)dialog.m_Scale);
	}
}

//extern int GrubAndShow(HDC hdc, CClientDC& dc);

void CBaslerMeterView::OnVideo()
{
	//CClientDC dc(this);

	//HDC hdc = GetDC()->GetSafeHdc();
	//GrubAndShow(hdc, dc);
}
