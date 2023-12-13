// CSettingDialogView.cpp: 实现文件
//

#include "pch.h"
#include "Painter3.h"
#include "CSettingDialogView.h"
#include "afxdialogex.h"


// CSettingDialogView 对话框

IMPLEMENT_DYNAMIC(CSettingDialogView, CDialogEx)

CSettingDialogView::CSettingDialogView(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_size(0)
{

}

CSettingDialogView::~CSettingDialogView()
{
}

void CSettingDialogView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_size);
}


BEGIN_MESSAGE_MAP(CSettingDialogView, CDialogEx)
END_MESSAGE_MAP()


// CSettingDialogView 消息处理程序
