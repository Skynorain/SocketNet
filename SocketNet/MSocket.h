#pragma once
#include <afxsock.h>            // MFC 套接字扩展

class CSocketNetDlg;
class MSocket : public CAsyncSocket
{
public:
	MSocket();
	CSocketNetDlg* m_dlg;
	virtual ~MSocket();
	virtual void OnAccept(int nErrorCode);	//Server
	virtual void OnConnect(int nErrorCode);	//Client
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	void SetDlg(CSocketNetDlg* dlg);
};


