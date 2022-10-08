// MSocket.cpp: 实现文件

#include "stdafx.h"
#include "MSocket.h"
#include "SocketNet.h"
#include "SocketNetDlg.h"

MSocket::MSocket()
{

}

MSocket::~MSocket()
{
}

// MSocket 成员函数
void MSocket::OnAccept(int nErrorCode)
{
	if (nErrorCode) {
		AfxMessageBox("连接失败，请重试");
		return;
	}
	m_dlg->OnAccept();
	CAsyncSocket::OnAccept(nErrorCode);
}

void MSocket::OnConnect(int nErrorCode)
{
	if (nErrorCode) {
		AfxMessageBox("连接失败，请重试");
		return;
	}
	m_dlg->OnConnect();
	CAsyncSocket::OnConnect(nErrorCode);
}

void MSocket::OnReceive(int nErrorCode)
{
	m_dlg->OnReceive();
	CAsyncSocket::OnReceive(nErrorCode);
}

void MSocket::OnClose(int nErrorCode)
{
	m_dlg->OnClose();
	CAsyncSocket::OnClose(nErrorCode);
}

void MSocket::SetDlg(CSocketNetDlg * dlg)
{
	m_dlg = dlg;
}
