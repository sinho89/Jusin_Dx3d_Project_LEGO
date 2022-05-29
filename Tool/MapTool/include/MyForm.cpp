// MyForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "MyForm.h"
#include "GameObject.h"
#include "Terrain.h"
#include "MainFrm.h"
#include "MapToolView.h"
#include "ResourcesMgr.h"
#include "Export_Function.h"


// MyForm

IMPLEMENT_DYNCREATE(MyForm, CFormView)

MyForm::MyForm()
	: CFormView(MyForm::IDD)
	, m_strMapKey(_T(""))
	, m_CntX(0)
	, m_CntZ(0)
	, m_Itv(0)
	, m_fHeight(0)
	, m_pResourcesMgr(ENGINE::Get_ResourceMgr())
	, m_pObject(NULL)
	, m_pGraphicDev(ENGINE::Get_GraphicDev())
	, m_bCreate(false)
	, m_bHeight(false)
	, m_bSave(false)
	, m_bLoad(false)
{
	m_CntX = 0;
	m_CntZ = 0;
	m_Itv = 0;
	m_fHeight = 0;
}

MyForm::~MyForm()
{
}

void MyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_KEY, m_strMapKey);
	DDX_Text(pDX, IDC_CNTX, m_CntX);
	DDX_Text(pDX, IDC_CNTZ, m_CntZ);
	DDX_Text(pDX, IDC_ITV, m_Itv);
	DDX_Text(pDX, IDC_HEIGHT, m_fHeight);
	DDX_Control(pDX, IDC_MAPTEX, m_Maplist);
}

BEGIN_MESSAGE_MAP(MyForm, CFormView)
	ON_BN_CLICKED(IDC_CREATE, &MyForm::OnBnClickedCreate)
	ON_BN_CLICKED(IDC_HEIGHT_OK, &MyForm::OnBnClickedHeightOk)
	ON_BN_CLICKED(IDC_MAPTEX_OK, &MyForm::OnBnClickedMaptexOk)
	ON_BN_CLICKED(IDC_MAP_SAVE, &MyForm::OnBnClickedMapSave)
	ON_BN_CLICKED(IDC_MAP_LOAD, &MyForm::OnBnClickedMapLoad)
	ON_EN_CHANGE(IDC_CNTX, &MyForm::OnEnChangeCntx)
	ON_EN_KILLFOCUS(IDC_CNTX, &MyForm::OnEnKillfocusCntx)
END_MESSAGE_MAP()


// MyForm 진단입니다.

#ifdef _DEBUG
void MyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void MyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// MyForm 메시지 처리기입니다.

void MyForm::OnBnClickedCreate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_bCreate = true;

}

void MyForm::OnBnClickedHeightOk()
{
	UpdateData(TRUE);

	m_bHeight = true;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void MyForm::OnBnClickedMaptexOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void MyForm::OnBnClickedMapSave()
{
	UpdateData(TRUE);

	m_bSave = true;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString	strPath;
	strPath = L"../../../Data/Terrain_Data/";

	CMainFrame*				pMainFrame	= (CMainFrame*)AfxGetMainWnd();
	CMapToolView*			pMainView	= (CMapToolView*)pMainFrame->m_wndSplitter.GetPane(0, 1);
	ENGINE::CGameObject*	pTerrain	= pMainView->m_pTerrain;
}

void MyForm::OnBnClickedMapLoad()
{
	UpdateData(TRUE);

	m_bLoad = true;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void MyForm::OnEnChangeCntx()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void MyForm::OnEnKillfocusCntx()
{

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
