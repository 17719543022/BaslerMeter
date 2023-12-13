
// Painter2View.cpp: CPainter2View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Painter2.h"
#endif

#include "Painter2Doc.h"
#include "Painter2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "CSettingDialogView.h"


// CPainter2View

IMPLEMENT_DYNCREATE(CPainter2View, CScrollView)

BEGIN_MESSAGE_MAP(CPainter2View, CScrollView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_COMMAND(ID_POINT, &CPainter2View::OnPoint)
	ON_COMMAND(ID_LINE, &CPainter2View::OnLine)
	ON_COMMAND(ID_RECTANGLE, &CPainter2View::OnRectangle)
	ON_COMMAND(ID_CHESS, &CPainter2View::OnChess)
	ON_COMMAND(ID_SETTING, &CPainter2View::OnSetting)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CPainter2View 构造/析构

CPainter2View::CPainter2View() noexcept
{
	// TODO: 在此处添加构造代码
    m_choice = 2;
    m_size = 50;
}

CPainter2View::~CPainter2View()
{
}

BOOL CPainter2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CScrollView::PreCreateWindow(cs);
}

// CPainter2View 绘图

void CPainter2View::OnDraw(CDC* /*pDC*/)
{
	CPainter2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}

void CPainter2View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 计算此视图的合计大小
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CPainter2View 打印

BOOL CPainter2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CPainter2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CPainter2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CPainter2View 诊断

#ifdef _DEBUG
void CPainter2View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CPainter2View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CPainter2Doc* CPainter2View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPainter2Doc)));
	return (CPainter2Doc*)m_pDocument;
}
#endif //_DEBUG


// CPainter2View 消息处理程序


void CPainter2View::OnPoint()
{
	// TODO: 在此添加命令处理程序代码
	m_choice = 1;
}


void CPainter2View::OnLine()
{
	// TODO: 在此添加命令处理程序代码
	m_choice = 2;
}


void CPainter2View::OnRectangle()
{
	// TODO: 在此添加命令处理程序代码
	m_choice = 3;
}


void CPainter2View::OnChess()
{
	// TODO: 在此添加命令处理程序代码
	m_choice = 4;
}


void CPainter2View::OnSetting()
{
	// TODO: 在此添加命令处理程序代码
    CSettingDialogView dialog;

    dialog.m_size = this->m_size;
    if (IDOK == dialog.DoModal())
    {
        UpdateData(TRUE);
        this->m_size = dialog.m_size;
        UpdateData(FALSE);
    }
}


void CPainter2View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_pointBegin = point;

	CScrollView::OnLButtonDown(nFlags, point);
}


void CPainter2View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
    m_pointEnd = point;
    CClientDC du(this);  //获得一个指向当前画板的句柄，du为自定义变量。
    int Ox = 200;			//棋盘左上角的x轴起点
    int Oy = 100;      //棋盘左上角的y轴起点
    int numx = 9;		//直线列数
    int numy = 9;		//直线行数
    int bulk = this->m_size;		//棋盘每个格子大小
    CBrush brush0(RGB(0, 0, 255));
    du.SelectObject(&brush0);

    switch (m_choice)
    {
    case 1:
        du.SetPixel(point, RGB(255, 0, 0));
        break;
    case 2:
        du.MoveTo(m_pointBegin);
        du.LineTo(m_pointEnd);
        break;
    case 3:
        du.Rectangle(CRect(m_pointBegin, m_pointEnd)); break;
    case 4:
        for (int i = 0; i < numx; i++)//画列线
        {
            du.MoveTo(Ox + bulk * i, Oy);
            du.LineTo(Ox + bulk * i, bulk * 8 + Oy);
        }
        for (int j = 0; j < numy; j++)//画行线
        {
            du.MoveTo(Ox, Oy + bulk * j);
            du.LineTo(Ox + bulk * 8, Oy + bulk * j);
        }
        CString str;
        for (int i = 8; i > 0; i--)
        {
            str.Format(_T("%d"), i);				//将数字转为CString类型
            du.TextOut(Ox - 30, bulk * 8 + 100 - bulk / 2 - (bulk * (i - 1)), str);     //绘制数字
        }
        char b[] = { 'A','B','C','D','E','F','G','H' };        //通过数组存储要绘制的字母
        for (int j = 1; j < 9; j++)
        {
            str.Format(_T("%c"), b[j - 1]);				//将char类型字母改为CString
            du.TextOut(Ox + bulk / 2 + bulk * (j - 1), Oy - 30, str);  //输出字母
        }
        for (int n = 0; n < 8; n = n + 1)
        {

            for (int m = 0; m < 8; m = m + 2)
            {
                CRect rc(Ox + bulk * m, Oy + n * bulk, Ox + bulk * m + bulk, Oy + n * bulk + bulk);
                if (n % 2 == 0)
                {
                    du.FillRect(rc, &brush0);
                }
            }
        }

        for (int n = 1; n < 9; n = n + 1)
        {

            for (int m = 1; m < 9; m = m + 2)
            {

                if (n % 2 != 0)
                {
                    CRect rc(Ox + bulk * m, Oy + n * bulk, Ox + bulk * m + bulk, Oy + n * bulk + bulk);
                    du.FillRect(rc, &brush0);
                }

            }
        }
        break;
    }

	CScrollView::OnLButtonUp(nFlags, point);
}
