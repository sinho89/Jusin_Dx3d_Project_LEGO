
// MapToolView.cpp : CMapToolView 클래스의 구현
//

#include "stdafx.h"
#include "MapTool.h"

#include "MapToolDoc.h"
#include "MapToolView.h"

#include "Export_Function.h"
#include "GraphicDev.h"
#include "MainFrm.h"
#include "GameObject.h"
#include "Terrain.h"
#include "ResourcesMgr.h"
#include "Transform.h"
#include "StaticCamera.h"
#include "CamTarget.h"
#include "MyForm.h"
#include "CollisionMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMapToolView

IMPLEMENT_DYNCREATE(CMapToolView, CScrollView)

BEGIN_MESSAGE_MAP(CMapToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMapToolView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CMapToolView 생성/소멸

CMapToolView::CMapToolView()
: m_pGraphicDev(ENGINE::Get_GraphicDev())
, m_pManagement(ENGINE::Get_Management())
, m_pResourcesMgr(ENGINE::Get_ResourceMgr())
, m_pDevice(NULL)
, m_bCreate(false)
, m_pGameObject(NULL)
, m_bView(false)
, m_bSave(false)
, m_bLoad(false)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMapToolView::~CMapToolView()
{
	ENGINE::Safe_Single_Destory(m_pGraphicDev);
	ENGINE::Safe_Single_Destory(m_pManagement);
	ENGINE::Safe_Single_Destory(m_pResourcesMgr);
}

BOOL CMapToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMapToolView 그리기

void CMapToolView::OnDraw(CDC* /*pDC*/)
{
	CMapToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	m_pGraphicDev->GetDevice()->Clear(0, NULL, 
		D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
		D3DCOLOR_ARGB(255, 0, 0, 0), 1.f, 0);
	m_pGraphicDev->GetDevice()->BeginScene();

	for(int i = 0; i < OBJ_end; ++i)
	{
		list<ENGINE::CGameObject*>::iterator iter = m_ObjList[i].begin();
		list<ENGINE::CGameObject*>::iterator iter_end = m_ObjList[i].end();
		for(; iter != iter_end; ++iter)
		{
			(*iter)->Update();
			if(i == OBJ_TERRAIN)
				(*iter)->Render();
		}
	}

	CMainFrame*	pFrame = (CMainFrame*)AfxGetMainWnd();
	MyForm*		pMyForm = (MyForm*)pFrame->GetActiveView();//pFrame->m_pMyForm;

	if(pMyForm->m_bCreate && m_bCreate == false)
	{
		pMyForm->m_bCreate = false;
		m_bCreate = true;
		
		m_pResourcesMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
				ENGINE::CVIBuffer::BUFFER_TERRAIN, L"Buffer_Terrain",  pMyForm->m_CntX, pMyForm->m_CntZ,  pMyForm->m_Itv);
			
		ENGINE::CGameObject*	pGameObject = NULL;

		//// terrain
		m_pTerrain = pGameObject = CTerrain::Create(m_pDevice, pMyForm->m_CntX, pMyForm->m_CntZ, pMyForm->m_Itv, L"Buffer_Terrain");
		m_ObjList[OBJ_TERRAIN].push_back(pGameObject);

		((CTerrain*)pGameObject)->m_VtxCntX = pMyForm->m_CntX;
		((CTerrain*)pGameObject)->m_VtxCntZ = pMyForm->m_CntZ;
		((CTerrain*)pGameObject)->m_Itv = pMyForm->m_Itv;

		m_pGameObject = pGameObject;
	}

	if(pMyForm->m_bHeight)
	{
		pMyForm->m_bHeight = false;
		m_fHeight = pMyForm->m_fHeight;
	}

	m_bSave = pMyForm->m_bSave;
	m_bLoad = pMyForm->m_bLoad;

	if(m_bSave)
	{
		m_bSave = false;
		pMyForm->m_bSave = false;
		Save();

	}

	if(m_bLoad)
	{
		m_bLoad = false;
		pMyForm->m_bLoad = false;
		Load();
	}
	m_pGraphicDev->GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);

	if(m_bView == true)
	{
		m_pGraphicDev->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pGraphicDev->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
	else if(m_bView == false)
		m_pGraphicDev->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pGraphicDev->GetDevice()->EndScene();
	m_pGraphicDev->GetDevice()->Present(NULL, NULL, NULL, NULL);

	Invalidate(FALSE);
}


// CMapToolView 인쇄

void CMapToolView::Save()
{
	DWORD	dwByte = 0;
	HANDLE	hFile = CreateFile(L"../../../Data/Stage2.dat", GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile == NULL)
	{
		return;
	}

	ENGINE::TERRAIN_INFO* pSaveTerrain = new ENGINE::TERRAIN_INFO;

	pSaveTerrain->m_dwVtxCnt	= dynamic_cast<CTerrain*>(m_pTerrain)->m_dwVtxCnt;
	pSaveTerrain->m_Itv			= dynamic_cast<CTerrain*>(m_pTerrain)->m_Itv;
	pSaveTerrain->m_VtxCntX		= dynamic_cast<CTerrain*>(m_pTerrain)->m_VtxCntX;
	pSaveTerrain->m_VtxCntZ		= dynamic_cast<CTerrain*>(m_pTerrain)->m_VtxCntZ;

	WriteFile(hFile, pSaveTerrain, sizeof(ENGINE::TERRAIN_INFO), &dwByte, NULL);

	pSaveTerrain->m_pVertex			= new ENGINE::VTXTEX[dynamic_cast<CTerrain*>(m_pTerrain)->m_dwVtxCnt];
	pSaveTerrain->m_pConvertVertex	= new ENGINE::VTXTEX[dynamic_cast<CTerrain*>(m_pTerrain)->m_dwVtxCnt];

	for(DWORD i=0; i<dynamic_cast<CTerrain*>(m_pTerrain)->m_dwVtxCnt; ++i)
	{
		pSaveTerrain->m_pVertex[i]			= dynamic_cast<CTerrain*>(m_pTerrain)->m_pVertex[i];
		pSaveTerrain->m_pConvertVertex[i]	= dynamic_cast<CTerrain*>(m_pTerrain)->m_pConvertVertex[i];

		WriteFile(hFile, &pSaveTerrain->m_pVertex[i], sizeof(ENGINE::VTXTEX), &dwByte, NULL);
		WriteFile(hFile, &pSaveTerrain->m_pConvertVertex[i], sizeof(ENGINE::VTXTEX), &dwByte, NULL);
	}	

	CloseHandle(hFile);
}


void CMapToolView::Load()
{
	DWORD	dwByte = 0;

	HANDLE hFile = CreateFile(L"../../../Data/Stage2.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	if(m_ObjList[OBJ_TERRAIN].size())
	{
		ENGINE::Safe_Delete((*m_ObjList[OBJ_TERRAIN].begin()));
		
		m_ObjList[OBJ_TERRAIN].clear();
	}

	ENGINE::TERRAIN_INFO* pSaveTerrain = new ENGINE::TERRAIN_INFO;

	ReadFile(hFile, pSaveTerrain, sizeof(ENGINE::TERRAIN_INFO), &dwByte, NULL);

	pSaveTerrain->m_pVertex			= new ENGINE::VTXTEX[pSaveTerrain->m_dwVtxCnt];
	pSaveTerrain->m_pConvertVertex	= new ENGINE::VTXTEX[pSaveTerrain->m_dwVtxCnt];
	
	for(DWORD i=0; i<pSaveTerrain->m_dwVtxCnt; ++i)
	{
		ReadFile(hFile, &pSaveTerrain->m_pVertex[i], sizeof(ENGINE::VTXTEX), &dwByte, NULL);
		ReadFile(hFile, &pSaveTerrain->m_pConvertVertex[i], sizeof(ENGINE::VTXTEX), &dwByte, NULL);
	}

	m_pResourcesMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::CVIBuffer::BUFFER_TERRAIN, L"Buffer_Terrain_Load",  
		pSaveTerrain->m_VtxCntX, 
		pSaveTerrain->m_VtxCntZ,  
		pSaveTerrain->m_Itv);

	m_pTerrain = CTerrain::Create(m_pDevice, 
								pSaveTerrain->m_VtxCntX, 
								pSaveTerrain->m_VtxCntZ, 
								pSaveTerrain->m_Itv,
								L"Buffer_Terrain_Load");

	for(DWORD i=0; i<dynamic_cast<CTerrain*>(m_pTerrain)->m_dwVtxCnt; ++i)
	{
		((CTerrain*)m_pTerrain)->m_pVertex[i]			= pSaveTerrain->m_pVertex[i];
		((CTerrain*)m_pTerrain)->m_pConvertVertex[i]	= pSaveTerrain->m_pConvertVertex[i];
	}

	((CTerrain*)m_pTerrain)->m_VtxCntX = pSaveTerrain->m_VtxCntX;
	((CTerrain*)m_pTerrain)->m_VtxCntZ = pSaveTerrain->m_VtxCntZ;
	((CTerrain*)m_pTerrain)->m_Itv = pSaveTerrain->m_Itv;

	m_pGameObject = m_pTerrain;

	m_ObjList[OBJ_TERRAIN].push_back(m_pGameObject);

	CloseHandle(hFile);

	Invalidate(FALSE);
}

void CMapToolView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CMapToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMapToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMapToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CMapToolView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMapToolView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CMapToolView 진단

#ifdef _DEBUG
void CMapToolView::AssertValid() const
{
	CView::AssertValid();
}

void CMapToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMapToolDoc* CMapToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapToolDoc)));
	return (CMapToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMapToolView 메시지 처리기

void CMapToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	ENGINE::g_hWnd = m_hWnd;

	m_pGraphicDev->InitGraphicDev(
		ENGINE::CGraphicDev::MODE_WIN, ENGINE::g_hWnd, ENGINE::WINCX, ENGINE::WINCY);

	SetScrollSizes(MM_TEXT, CSize( ENGINE::WINCX,  ENGINE::WINCY));
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CMainFrame*	pFrame = (CMainFrame*)AfxGetMainWnd();

	RECT	rcWindow;
	pFrame->GetWindowRect(&rcWindow);

	SetRect(&rcWindow, 0, 0, rcWindow.right - rcWindow.left,
		rcWindow.bottom - rcWindow.top);

	RECT	rcMainView;
	GetClientRect(&rcMainView);

	float fRowFrm = float(rcWindow.right - rcMainView.right);
	float fColFrm = float(rcWindow.bottom - rcMainView.bottom);

	pFrame->SetWindowPos(NULL, 0, 0, int(ENGINE::WINCX + fRowFrm), int(ENGINE::WINCY + fColFrm), SWP_NOZORDER);

	Initialize();
}

void CMapToolView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nChar)
	{
	case 'V':
		m_bView = true;
		break;
	case 'B':
		m_bView = false;
		break;
	}


	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

HRESULT CMapToolView::Initialize(void)
{
	m_pDevice = m_pGraphicDev->GetDevice();	

	m_fHeight = 0;

	HRESULT		hr = NULL;

	hr = m_pManagement->InitManagement(m_pDevice);

	CCollisionMgr::GetInstance()->AddColObject(CCollisionMgr::COL_MOUSE);

	m_pResourcesMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::TEX_NORMAL, L"Texture Terrain", 
		L"../../../Texture/terrain/lava_d.jpg", 1);

	ENGINE::CGameObject*	pGameObject = NULL;

	pGameObject = CCamTarget::Create(m_pDevice);
	m_ObjList[OBJ_FOCUS_CENTER].push_back(pGameObject);

	const ENGINE::CComponent*		pTransform = ((CCamTarget*)pGameObject)->m_pInfo;

	pGameObject = CStaticCamera::Create(m_pDevice, dynamic_cast<const ENGINE::CTransform*>(pTransform));
	m_ObjList[OBJ_CAMERA].push_back(pGameObject);

	return S_OK;
}



void CMapToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if(m_bCreate)
	{
		((CTerrain*)(*m_ObjList[OBJ_TERRAIN].begin()))->PickTerrain(point, m_fHeight);
	}
	CScrollView::OnLButtonDown(nFlags, point);
}

void CMapToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.


	if(GetAsyncKeyState(VK_LBUTTON))
	{
		OnLButtonDown(nFlags, point);
	}

	CScrollView::OnMouseMove(nFlags, point);
}
