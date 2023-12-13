#pragma once


// CSettingDialogView 对话框

class CSettingDialogView : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingDialogView)

public:
	CSettingDialogView(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSettingDialogView();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_size;
};
