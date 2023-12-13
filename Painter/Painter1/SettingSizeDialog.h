#pragma once


// SettingSizeDialog 对话框

class SettingSizeDialog : public CDialogEx
{
	DECLARE_DYNAMIC(SettingSizeDialog)

public:
	SettingSizeDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~SettingSizeDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_POP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_Size;
};
