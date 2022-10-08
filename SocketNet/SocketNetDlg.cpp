
// SocketNetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SocketNet.h"
#include "SocketNetDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSocketNetDlg �Ի���
CSocketNetDlg::CSocketNetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SOCKETNET_DIALOG, pParent)
	, m_nServer(0)
	, sPort(0)
	, m_nSend(0)
	, m_nRecv(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSocketNetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_IPAddress(pDX, IDC_IPADDRESS, ServerIP);
	DDX_Text(pDX, IDC_EDIT_PORT, sPort);
	DDX_Control(pDX, IDC_IPADDRESS, ServerIP);
	DDX_Control(pDX, IDC_COMBO_TCP, m_comboTcp);
}

BEGIN_MESSAGE_MAP(CSocketNetDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CSocketNetDlg::OnBnClickedConnect)
	ON_BN_CLICKED(IDC_BTN_DISCONNECT, &CSocketNetDlg::OnBnClickedBtnDisconnect)
	ON_BN_CLICKED(IDC_BTN_SEND, &CSocketNetDlg::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_BTN_DIS_LISTEN, &CSocketNetDlg::OnBnClickedBtnDisListen)
	ON_CBN_SELCHANGE(IDC_COMBO_TCP, &CSocketNetDlg::OnCbnSelchangeComboTcp)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CSocketNetDlg::OnBnClickedBtnClear)
END_MESSAGE_MAP()


// CSocketNetDlg ��Ϣ�������

BOOL CSocketNetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_ServerSocket = NULL;
	m_ListenSocket = NULL;
	m_ClientSocket = NULL;

	m_comboTcp.Clear();
	m_comboTcp.InsertString(0, _T("TCP �ͻ���"));
	m_comboTcp.InsertString(1, _T("TCP �����"));
	m_comboTcp.SetCurSel(0);

	GetDlgItem(IDC_BTN_CONNECT)->SetWindowText("����");
	GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_DISCONNECT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_DIS_LISTEN)->EnableWindow(FALSE);

	GetDlgItem(IDC_EDIT_SEND)->SetWindowText("TEST");

	CString strIP = "127.0.0.1";
	DWORD dwAddress = ntohl(inet_addr(strIP));
	ServerIP.SetAddress(dwAddress);
	GetDlgItem(IDC_EDIT_PORT)->SetWindowText("7600");

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSocketNetDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSocketNetDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSocketNetDlg::OnBnClickedConnect()
{
	if (!AfxSocketInit()) {
		MessageBox("WindowSocket init failed!", "Receive", MB_ICONSTOP);
		return;
	}

	if (1 == m_nServer)
	{
		m_ListenSocket = new MSocket;
		m_ListenSocket->SetDlg(this);
		BYTE nFild[4];
		CString sIP, sP;

		ServerIP.GetAddress(nFild[0], nFild[1], nFild[2], nFild[3]);
		sIP.Format("%d.%d.%d.%d", nFild[0], nFild[1], nFild[2], nFild[3]);
		sP.Format("%d", sPort);

		sIP = "127.0.0.1";
		sPort = 7600;

		m_ListenSocket->Create(sPort, 1, FD_ACCEPT, sIP);//��������������Socket
		m_ListenSocket->Listen(20);
		GetDlgItem(IDC_EDIT_RECV)->SetWindowText("��ʼ����");

		GetDlgItem(IDC_IPADDRESS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_DISCONNECT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_DIS_LISTEN)->EnableWindow(TRUE);

	}
	else
	{
		m_ClientSocket = new MSocket;
		m_ClientSocket->SetDlg(this);
		//���ӷ�����//
		BYTE nFile[4];
		CString sIP;
		ServerIP.GetAddress(nFile[0], nFile[1], nFile[2], nFile[3]);
		sIP.Format("%d.%d.%d.%d", nFile[0], nFile[1], nFile[2], nFile[3]);

		sIP = "127.0.0.1";
		sPort = 7600;

		m_ClientSocket->Create();
		m_ClientSocket->Connect(sIP, sPort);
		GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);
	}
}



void CSocketNetDlg::OnAccept()
{
	if (1 == m_nServer)
	{
		m_ServerSocket = new MSocket;
		m_ServerSocket->SetDlg(this);
		m_ListenSocket->Accept(*m_ServerSocket);
		m_ServerSocket->AsyncSelect(FD_READ | FD_CLOSE);
		GetDlgItem(IDC_EDIT_RECV)->SetWindowText("�յ���������");
		//���ӳɹ�//
		GetDlgItem(IDC_BTN_SEND)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_DISCONNECT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_DIS_LISTEN)->EnableWindow(TRUE);
	}
}

void CSocketNetDlg::OnConnect()
{
	//���ӳɹ�//
	GetDlgItem(IDC_BTN_SEND)->EnableWindow(TRUE);
	GetDlgItem(IDC_IPADDRESS)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_DISCONNECT)->EnableWindow(TRUE);

	if (1 == m_nServer)
	{
		GetDlgItem(IDC_BTN_DIS_LISTEN)->EnableWindow(TRUE);
	}

	GetDlgItem(IDC_EDIT_RECV)->SetWindowText("���ӷ������ɹ�");
}


void CSocketNetDlg::OnReceive()
{
	char szTemp[2000];
	if (1 == m_nServer)
	{
		int n = m_ServerSocket->Receive(szTemp, 200);
		szTemp[n] = '\0';
	}
	else
	{
		int n = m_ClientSocket->Receive(szTemp, 200);
		szTemp[n] = '\0';
	}
	CString sTemp;
	sTemp.Format("�յ�: %s", szTemp);
	GetDlgItem(IDC_EDIT_RECV)->SetWindowText(sTemp);

	CString strRecv;
	strRecv.Format("����: %d", ++m_nRecv);
	GetDlgItem(IDC_STATIC_RECV)->SetWindowText(strRecv);
}

void CSocketNetDlg::OnClose()
{
	if (1 == m_nServer)
	{
		m_ServerSocket->Close();
		if (m_ServerSocket != NULL) {
			delete m_ServerSocket;
			m_ServerSocket = NULL;
		}

		GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_DISCONNECT)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_RECV)->SetWindowText("�Ͽ�����");
	}
	else
	{
		m_ClientSocket->Close();
		SocketReset();
	}

	GetDlgItem(IDC_EDIT_RECV)->SetWindowText("�ر�����");
}

void CSocketNetDlg::SocketReset()
{
	if (1 == m_nServer)
	{
		if (m_ServerSocket != NULL) {
			delete m_ServerSocket;
			m_ServerSocket = NULL;
		}
		if (m_ListenSocket != NULL) {
			delete m_ListenSocket;
			m_ListenSocket = NULL;
		}
	}
	else
	{
		if (m_ClientSocket != NULL) {
			delete m_ClientSocket;
			m_ClientSocket = NULL;
		}
	}

	GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_DISCONNECT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_DIS_LISTEN)->EnableWindow(FALSE);

	GetDlgItem(IDC_EDIT_RECV)->SetWindowText("�Ͽ����ӣ��Զ���λ");
}

void CSocketNetDlg::OnBnClickedBtnDisconnect()
{
	if (1 == m_nServer)
	{
		m_ServerSocket->Close();
		if (m_ServerSocket != NULL) {
			delete m_ServerSocket;
			m_ServerSocket = NULL;
		}
	}
	else
	{
		m_ClientSocket->Close();
		SocketReset();
		GetDlgItem(IDC_IPADDRESS)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(TRUE);
	}

	GetDlgItem(IDC_EDIT_PORT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_DISCONNECT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);

	GetDlgItem(IDC_EDIT_RECV)->SetWindowText("�Ͽ�");
}


void CSocketNetDlg::OnBnClickedBtnSend()
{
	CString str;
	GetDlgItem(IDC_EDIT_SEND)->GetWindowText(str);
	if ("" == str)
	{
		GetDlgItem(IDC_EDIT_RECV)->SetWindowText("����Ϊ�գ���ȷ��");
		return;
	}
	
	if (1 == m_nServer)
	{
		m_ServerSocket->Send(str, str.GetLength());
	}
	else
	{
		m_ClientSocket->Send(str, str.GetLength());
	}
	CString strSend;
	strSend.Format("����: %d", ++m_nSend);
	GetDlgItem(IDC_STATIC_SEND)->SetWindowText(strSend);
	GetDlgItem(IDC_EDIT_RECV)->SetWindowText("����: " + str);
}


void CSocketNetDlg::OnBnClickedBtnDisListen()
{
	if (1 == m_nServer)
	{
		m_ListenSocket->Close();
		if (m_ListenSocket != NULL) {
			delete m_ListenSocket;
			m_ListenSocket = NULL;
		}

		SocketReset();

		GetDlgItem(IDC_EDIT_RECV)->SetWindowText("�رռ���");
		GetDlgItem(IDC_IPADDRESS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_DISCONNECT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_DIS_LISTEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);
	}
}


void CSocketNetDlg::OnCbnSelchangeComboTcp()
{
	int n = ((CComboBox*)GetDlgItem(IDC_COMBO_TCP))->GetCurSel();//��ǰѡ�е��С�
	((CComboBox*)GetDlgItem(IDC_COMBO_TCP))->SetCurSel(n);//���õ�n������Ϊ��ʾ�����ݡ�
	if (0 == n)
	{
		GetDlgItem(IDC_IPADDRESS)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_DIS_LISTEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CONNECT)->SetWindowText("����");
		m_nServer = 0;
	}
	else
	{
		GetDlgItem(IDC_IPADDRESS)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_DIS_LISTEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CONNECT)->SetWindowText("����");
		m_nServer = 1;
	}
}

void CSocketNetDlg::OnBnClickedBtnClear()
{
	GetDlgItem(IDC_STATIC_SEND)->SetWindowText("����: 0");
	GetDlgItem(IDC_STATIC_RECV)->SetWindowText("����: 0");
	m_nSend = 0;
	m_nRecv = 0;
}
