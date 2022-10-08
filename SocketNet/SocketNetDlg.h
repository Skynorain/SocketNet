
// SocketNetDlg.h : 头文件
//

#pragma once
#include "MSocket.h"
#include "afxcmn.h"
#include "afxwin.h"

// CSocketNetDlg 对话框
class CSocketNetDlg : public CDialogEx
{
// 构造
public:
	CSocketNetDlg(CWnd* pParent = NULL);	// 标准构造函数
	MSocket* m_ServerSocket;
	MSocket* m_ListenSocket;
	MSocket* m_ClientSocket;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SOCKETNET_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	int m_nServer;	//0: Client  1: Server
	int m_nSend;
	int m_nRecv;
	void OnAccept();
	void OnConnect();
	void OnReceive();
	void OnClose();
	void SocketReset();

	afx_msg void OnBnClickedConnect();
	afx_msg void OnBnClickedBtnDisconnect();
	afx_msg void OnBnClickedBtnSend();
	afx_msg void OnBnClickedBtnDisListen();
	afx_msg void OnCbnSelchangeComboTcp();
	afx_msg void OnBnClickedBtnClear();

	int sPort;
	CIPAddressCtrl ServerIP;
	CComboBox m_comboTcp;
};
