// SettingSizeDialog.cpp: 实现文件
//

#include "pch.h"
#include "Painter1.h"
#include "SettingSizeDialog.h"
#include "afxdialogex.h"


// SettingSizeDialog 对话框

IMPLEMENT_DYNAMIC(SettingSizeDialog, CDialogEx)

SettingSizeDialog::SettingSizeDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_POP, pParent)
	, m_Size(1)
{

}

SettingSizeDialog::~SettingSizeDialog()
{
}

void SettingSizeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Size);
}


BEGIN_MESSAGE_MAP(SettingSizeDialog, CDialogEx)
END_MESSAGE_MAP()


// SettingSizeDialog 消息处理程序
