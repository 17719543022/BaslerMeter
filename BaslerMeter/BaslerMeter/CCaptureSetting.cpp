// CCaptureSetting.cpp: 实现文件
//

#include "pch.h"
#include "BaslerMeter.h"
#include "CCaptureSetting.h"
#include "afxdialogex.h"


// CCaptureSetting 对话框

IMPLEMENT_DYNAMIC(CCaptureSetting, CDialogEx)

CCaptureSetting::CCaptureSetting(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SETTING, pParent)
	, m_Magnification(CString())
	, m_Scale(CString())
{

}

CCaptureSetting::~CCaptureSetting()
{
}

void CCaptureSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MAGNIFICATION, m_Magnification);
	DDX_Text(pDX, IDC_SCALE, m_Scale);
}


BEGIN_MESSAGE_MAP(CCaptureSetting, CDialogEx)
END_MESSAGE_MAP()


// CCaptureSetting 消息处理程序
