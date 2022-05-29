
// ObjToolView.cpp : CObjToolView 클래스의 구현
//

#include "stdafx.h"
#include "ObjTool.h"

#include "ObjToolDoc.h"
#include "ObjToolView.h"

#include "MainFrm.h"

#include "GraphicDev.h"
#include "Pipeline.h"
#include "Export_Function.h"
#include "Engine_Include.h"
#include "Transform.h"
#include "GameObject.h"
#include "StaticCamera.h"
#include "CameraObserver.h"
#include "CubeObj.h"
#include "CamTarget.h"
#include "CollisionMgr.h"
#include "CubeObj.h"
#include "MouseCol.h"
#include "MyForm.h"
#include "Component.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CObjToolView

IMPLEMENT_DYNCREATE(CObjToolView, CScrollView)

BEGIN_MESSAGE_MAP(CObjToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CObjToolView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_CREATE()
	ON_WM_MBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CObjToolView 생성/소멸


CObjToolView::CObjToolView()
: m_pGraphicDev(ENGINE::Get_GraphicDev())
, m_pManagement(ENGINE::Get_Management())
, m_pTimeMgr(ENGINE::Get_Time())
, m_pResourcesMgr(ENGINE::Get_ResourceMgr())
, m_pInfoSubject(ENGINE::Get_InfoSubject())
, m_pDevice(NULL)
, m_pInfo(NULL)
, m_pCamObserver(NULL)
, m_pOptionObject(NULL)
, m_bView(false)
, m_bSave(false)
, m_bLoad(false)
, m_bOptionCreate(false)
, m_bOptionDelete(false)
, m_bOptionTexture(false)
, m_bOptionMove(false)
, m_bOptionSize(false)
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_pInfo = ENGINE::CTransform::Create(ENGINE::g_vLook);
	ZeroMemory(szLoadFile, sizeof(szLoadFile));
}

void CObjToolView::CubeSave(void)
{
	CMainFrame* pMainFrame	= (CMainFrame*)AfxGetMainWnd();
	CMyForm*	pMyForm		= (CMyForm*)pMainFrame->m_wndSplitter.GetPane(0, 0);

	TCHAR	szForderPath[MAX_PATH] = L"../../../Data/Object_Data/";
	//TCHAR	szFileName[MAX_PATH] = L"";

	if(pMyForm->m_strSaveName == L"")
	{
		MessageBox(L"저장할 파일 이름을 입력하시오.");
		return;
	}

	lstrcat(szForderPath, pMyForm->m_strSaveName);
	lstrcat(szForderPath, L".dat");

	HANDLE	hFile = CreateFile(szForderPath, GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD dwByte = 0;

	list<ENGINE::CGameObject*>::iterator iter = m_ObjList[OBJ_OBJECT].begin();
	list<ENGINE::CGameObject*>::iterator iter_end = m_ObjList[OBJ_OBJECT].end();

	ENGINE::SAVECUBE* pSaveCube = new ENGINE::SAVECUBE;

	for(; iter != iter_end; ++iter)
	{
		pSaveCube->m_SaveType	=	((CCubeObj*)(*iter))->m_eCubeSaveType;

		pSaveCube->m_fAngle[ENGINE::ANGLE_X] = ((CCubeObj*)(*iter))->m_pInfo->m_fAngle[ENGINE::ANGLE_X];
		pSaveCube->m_fAngle[ENGINE::ANGLE_Y] = ((CCubeObj*)(*iter))->m_pInfo->m_fAngle[ENGINE::ANGLE_Y];
		pSaveCube->m_fAngle[ENGINE::ANGLE_Z] = ((CCubeObj*)(*iter))->m_pInfo->m_fAngle[ENGINE::ANGLE_Z];

		pSaveCube->m_vPos = ((CCubeObj*)(*iter))->m_pInfo->m_vPos;
		pSaveCube->m_vDir = ((CCubeObj*)(*iter))->m_pInfo->m_vDir;
		pSaveCube->m_matWorld = ((CCubeObj*)(*iter))->m_pInfo->m_matWorld;

		pSaveCube->m_fItv_X = ((CCubeObj*)(*iter))->m_fItv_X;
		pSaveCube->m_fItv_Y = ((CCubeObj*)(*iter))->m_fItv_Y;
		pSaveCube->m_fItv_Z = ((CCubeObj*)(*iter))->m_fItv_Z;
		
		WriteFile(hFile, pSaveCube, sizeof(ENGINE::SAVECUBE), &dwByte, NULL);
	}

	pMyForm->m_Obj_ListBox.AddString(pMyForm->m_strSaveName);
	pMyForm->Invalidate(FALSE);

	CloseHandle(hFile);

}

void CObjToolView::CubeLoad(void)
{
	if(szLoadFile == L"")
	{
		MessageBox(L"로드할 파일을 선택하시오.");
		return;
	}

	lstrcat(szLoadFile, L".dat");

	TCHAR	szForderPath[MAX_PATH] = L"../../../Data/Object_Data/";

	lstrcat(szForderPath, szLoadFile);

	HANDLE hFile = CreateFile(szForderPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD	dwByte = 0;
	
	list<ENGINE::CGameObject*>::iterator iter = m_ObjList[OBJ_OBJECT].begin();
	list<ENGINE::CGameObject*>::iterator iter_end = m_ObjList[OBJ_OBJECT].end();


	for(iter; iter != iter_end; ++iter)
	{
		//m_ObjList[OBJ_OBJECT].erase(iter);
		delete (*iter);
	}

	m_ObjList[OBJ_OBJECT].clear();	

	while(true)
	{
		ENGINE::SAVECUBE* pCube = new ENGINE::SAVECUBE;

		ENGINE::CGameObject* pGameObj = CCubeObj::Create(m_pDevice, L"Buffer_CubeTex");

		ReadFile(hFile, pCube, sizeof(ENGINE::SAVECUBE), &dwByte, NULL);

		if(dwByte == 0)
		{
			delete pCube;
			break;
		}

		((CCubeObj*)(pGameObj))->m_pInfo->m_fAngle[ENGINE::ANGLE_X]	= pCube->m_fAngle[ENGINE::ANGLE_X];
		((CCubeObj*)(pGameObj))->m_pInfo->m_fAngle[ENGINE::ANGLE_Y]	= pCube->m_fAngle[ENGINE::ANGLE_Y];
		((CCubeObj*)(pGameObj))->m_pInfo->m_fAngle[ENGINE::ANGLE_Z]	= pCube->m_fAngle[ENGINE::ANGLE_Z];

		((CCubeObj*)(pGameObj))->m_pInfo->m_matWorld = pCube->m_matWorld;
		((CCubeObj*)(pGameObj))->m_pInfo->m_vDir = pCube->m_vDir;
		((CCubeObj*)(pGameObj))->m_pInfo->m_vPos = pCube->m_vPos;

		((CCubeObj*)(pGameObj))->m_fItv_X = pCube->m_fItv_X;
		((CCubeObj*)(pGameObj))->m_fItv_Y = pCube->m_fItv_Y;
		((CCubeObj*)(pGameObj))->m_fItv_Z = pCube->m_fItv_Z;

		((CCubeObj*)(pGameObj))->m_eCubeSaveType = pCube->m_SaveType;

		m_ObjList[OBJ_OBJECT].push_back(pGameObj);
	}

	CloseHandle(hFile);

	Invalidate(FALSE);





	//CFileDialog	FileDlg(FALSE, szLoadFile, NULL,
	//	OFN_EXPLORER, NULL, NULL);
	//TCHAR	szCurDir[MAX_PATH] = L"";

	//GetCurrentDirectory(MAX_PATH - 1, szCurDir);
	//PathRemoveFileSpec(szCurDir);
	//FileDlg.m_ofn.lpstrInitialDir = szCurDir;
	//CString strPathName;

	//if(FileDlg.DoModal() == IDOK)
	//{
	//	DWORD	dwByte = 0;

	//	strPathName = FileDlg.GetPathName();

	//	HANDLE hFile = CreateFile(strPathName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	//	list<ENGINE::CGameObject*>::iterator iter = m_ObjList[OBJ_OBJECT].begin();
	//	list<ENGINE::CGameObject*>::iterator iter_end = m_ObjList[OBJ_OBJECT].end();


	//	for(iter; iter != iter_end; ++iter)
	//	{
	//		//m_ObjList[OBJ_OBJECT].erase(iter);
	//		delete (*iter);
	//	}

	//	m_ObjList[OBJ_OBJECT].clear();	

	//	while(true)
	//	{
	//		ENGINE::SAVECUBE* pCube = new ENGINE::SAVECUBE;

	//		ENGINE::CGameObject* pGameObj = CCubeObj::Create(m_pDevice, L"Buffer_CubeTex");

	//		ReadFile(hFile, pCube, sizeof(ENGINE::SAVECUBE), &dwByte, NULL);

	//		if(dwByte == 0)
	//		{
	//			delete pCube;
	//			break;
	//		}

	//		((CCubeObj*)(pGameObj))->m_pInfo->m_fAngle[ENGINE::ANGLE_X]	= pCube->m_fAngle[ENGINE::ANGLE_X];
	//		((CCubeObj*)(pGameObj))->m_pInfo->m_fAngle[ENGINE::ANGLE_Y]	= pCube->m_fAngle[ENGINE::ANGLE_Y];
	//		((CCubeObj*)(pGameObj))->m_pInfo->m_fAngle[ENGINE::ANGLE_Z]	= pCube->m_fAngle[ENGINE::ANGLE_Z];

	//		((CCubeObj*)(pGameObj))->m_pInfo->m_matWorld = pCube->m_matWorld;
	//		((CCubeObj*)(pGameObj))->m_pInfo->m_vDir = pCube->m_vDir;
	//		((CCubeObj*)(pGameObj))->m_pInfo->m_vPos = pCube->m_vPos;

	//		m_ObjList[OBJ_OBJECT].push_back(pGameObj);
	//	}

	//	CloseHandle(hFile);

	//	Invalidate(FALSE);
	//}
}



CObjToolView::~CObjToolView()
{
	Release();
}

BOOL CObjToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CObjToolView 그리기

void CObjToolView::OnDraw(CDC* /*pDC*/)
{
	CObjToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CMainFrame* pMainFrame	= (CMainFrame*)AfxGetMainWnd();
	CMyForm*	pMyForm		= (CMyForm*)pMainFrame->m_wndSplitter.GetPane(0, 0);//GetActiveView();

	m_pGraphicDev->GetDevice()->Clear(0,NULL, 
		D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
		D3DCOLOR_ARGB(255, 0, 0, 0), 1.f, 0);
	m_pGraphicDev->GetDevice()->BeginScene();


	for(int i = 0; i < OBJ_end; ++i)
	{
		list<ENGINE::CGameObject*>::iterator iter = m_ObjList[i].begin();
		list<ENGINE::CGameObject*>::iterator iter_end = m_ObjList[i].end();

		for(; iter != iter_end; ++iter)
		{
			if(i == OBJ_FOCUS_CENTER || i == OBJ_CAMERA)
			{
				(*iter)->Update();
			}
			else
			{
				(*iter)->Update();
				(*iter)->Render();
			}
		}
	}

	if(m_bView)
	{
		m_pGraphicDev->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pGraphicDev->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
	else
	{
		m_pGraphicDev->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}

	m_bSave = pMyForm->m_bCubeSave;

	if(m_bSave == true)
	{
		CubeSave();
		pMyForm->m_bCubeSave = false;
		m_bSave = false;
	}

	m_bLoad = pMyForm->m_bCubeLoad;
	if(m_bLoad == true)
	{
		CubeLoad();
		pMyForm->m_bCubeLoad = false;
		m_bLoad = false;
	}

	m_pGraphicDev->GetDevice()->EndScene();
	m_pGraphicDev->GetDevice()->Present(NULL, NULL, NULL, NULL);

	Invalidate(FALSE);
}


// CObjToolView 인쇄


void CObjToolView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CObjToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CObjToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CObjToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CObjToolView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CObjToolView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CObjToolView 진단

#ifdef _DEBUG
void CObjToolView::AssertValid() const
{
	CView::AssertValid();
}

void CObjToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CObjToolDoc* CObjToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CObjToolDoc)));
	return (CObjToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CObjToolView 메시지 처리기
void CObjToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	ENGINE::g_hWnd = m_hWnd;

	ENGINE::CGameObject*	pGameObject = NULL;
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if(FAILED(m_pGraphicDev->InitGraphicDev(ENGINE::CGraphicDev::MODE_WIN,
		ENGINE::g_hWnd, 
		ENGINE::WINCX, 
		ENGINE::WINCY)))
	{
		//MSG_BOX(L"Device 초기화 실패");
		AfxMessageBox(L"Device 초기화 실패");
	}

	SetScrollSizes(MM_TEXT, CSize(ENGINE::WINCX, ENGINE::WINCY));

	CMainFrame*	pFrame = (CMainFrame*)AfxGetMainWnd();

	RECT	rcWindow;
	pFrame->GetWindowRect(&rcWindow);

	SetRect(&rcWindow, 0, 0, 
		rcWindow.right - rcWindow.left,
		rcWindow.bottom - rcWindow.top);

	RECT	rcMainView;
	GetClientRect(&rcMainView);

	float fRowFrm	=	float(rcWindow.right - rcMainView.right);
	float fColFrm	=	float(rcWindow.bottom - rcMainView.bottom);

	pFrame->SetWindowPos(NULL, 0, 0, int(ENGINE::WINCX + fRowFrm), int(ENGINE::WINCY + fColFrm), SWP_NOZORDER);

	Initialize();
}

void CObjToolView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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

	CMainFrame* pMainFrame	= (CMainFrame*)AfxGetMainWnd();
	CMyForm*	pMyForm		= (CMyForm*)pMainFrame->m_wndSplitter.GetPane(0, 0);

	if(pMyForm->m_iOption == 4)
	{
		if(dynamic_cast<CCubeObj*>(m_pOptionObject)->m_bMove == true)
		{
			switch(nChar)
			{
			case VK_NUMPAD8:
				dynamic_cast<CCubeObj*>(m_pOptionObject)->m_pInfo->m_vPos.y += 0.02f;
				pMyForm->OnEnChangePosY();
				break;

			case VK_NUMPAD5:
				dynamic_cast<CCubeObj*>(m_pOptionObject)->m_pInfo->m_vPos.y -= 0.02f;
				pMyForm->OnEnChangePosY();
				break;

			case VK_NUMPAD4:
				dynamic_cast<CCubeObj*>(m_pOptionObject)->m_pInfo->m_vPos.x -= 0.02f;
				pMyForm->OnEnChangePosX();
				break;

			case VK_NUMPAD6:
				dynamic_cast<CCubeObj*>(m_pOptionObject)->m_pInfo->m_vPos.x += 0.02f;
				pMyForm->OnEnChangePosX();
				break;

			case VK_NUMPAD7:
				dynamic_cast<CCubeObj*>(m_pOptionObject)->m_pInfo->m_vPos.z += 0.02f;
				pMyForm->OnEnChangePosZ();
				break;

			case VK_NUMPAD9:
				dynamic_cast<CCubeObj*>(m_pOptionObject)->m_pInfo->m_vPos.z -= 0.02f;
				pMyForm->OnEnChangePosZ();
				break;

			case 'I':
				dynamic_cast<CCubeObj*>(m_pOptionObject)->m_pInfo->m_fAngle[ENGINE::ANGLE_X] += 0.02f;
				break;

			case 'K':
				dynamic_cast<CCubeObj*>(m_pOptionObject)->m_pInfo->m_fAngle[ENGINE::ANGLE_X] -= 0.02f;
				break;

			case 'J':
				dynamic_cast<CCubeObj*>(m_pOptionObject)->m_pInfo->m_fAngle[ENGINE::ANGLE_Y] += 0.02f;
				break;

			case 'L':
				dynamic_cast<CCubeObj*>(m_pOptionObject)->m_pInfo->m_fAngle[ENGINE::ANGLE_Y] -= 0.02f;
				break;

			case 'O':
				dynamic_cast<CCubeObj*>(m_pOptionObject)->m_pInfo->m_fAngle[ENGINE::ANGLE_Z] += 0.02f;
				break;

			case 'P':
				dynamic_cast<CCubeObj*>(m_pOptionObject)->m_pInfo->m_fAngle[ENGINE::ANGLE_Z] -= 0.02f;
				break;
			}
		}		
	}

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CObjToolView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.


}

HRESULT CObjToolView::Initialize(void)
{
	m_pGraphicDev->GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	HRESULT		hr = NULL;

	m_pDevice = m_pGraphicDev->GetDevice();
	//NULL_CHECK_RETURN(m_pDevice, E_FAIL);

	hr = m_pManagement->InitManagement(m_pDevice);
	//FAILED_CHECK_MSG(hr, L"Management Init Failed");

	/*hr = m_pResourcesMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC,
	ENGINE::CVIBuffer::BUFFER_CUBETEX, L"Buffer_CubeTex");*/

	hr = m_pResourcesMgr->AddObjBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::CVIBuffer::BUFFER_CUBETEX_CUSTOM, L"Buffer_CubeTex", 1.f, 1.f, 1.f);

	hr = m_pResourcesMgr->AddObjBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::CVIBuffer::BUFFER_CUBECOL_CUSTOM, L"Buffer_X_Line", 30.f, 0.01f, 0.01f, D3DCOLOR_ARGB(255, 255, 0, 0));

	hr = m_pResourcesMgr->AddObjBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::CVIBuffer::BUFFER_CUBECOL_CUSTOM, L"Buffer_Y_Line", 0.01f, 30.f, 0.01f, D3DCOLOR_ARGB(255, 0, 255, 0));

	hr = m_pResourcesMgr->AddObjBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::CVIBuffer::BUFFER_CUBECOL_CUSTOM, L"Buffer_Z_Line", 0.01f, 0.01f, 30.f, D3DCOLOR_ARGB(255, 0, 0, 255));

	hr = m_pResourcesMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::TEX_CUBE, L"Texture_Cube",
		L"../../../Texture/Cube_dds/adesert_example_%d.dds", 1);


	hr = CCollisionMgr::GetInstance()->AddColObject(CCollisionMgr::COL_MOUSE);

	ENGINE::CGameObject*	pGameObject = NULL;

	pGameObject = CCubeObj::Create(m_pDevice, L"Buffer_CubeTex");
	dynamic_cast<CCubeObj*>(pGameObject)->SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));
	m_ObjList[OBJ_OBJECT].push_back(pGameObject);

	pGameObject = CCubeObj::Create(m_pDevice, L"Buffer_X_Line");
	dynamic_cast<CCubeObj*>(pGameObject)->SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));
	m_ObjList[OBJ_LINE].push_back(pGameObject);

	pGameObject = CCubeObj::Create(m_pDevice, L"Buffer_Y_Line");
	dynamic_cast<CCubeObj*>(pGameObject)->SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));
	m_ObjList[OBJ_LINE].push_back(pGameObject);

	pGameObject = CCubeObj::Create(m_pDevice, L"Buffer_Z_Line");
	dynamic_cast<CCubeObj*>(pGameObject)->SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));
	m_ObjList[OBJ_LINE].push_back(pGameObject);

	pGameObject = CCamTarget::Create(m_pDevice);
	m_ObjList[OBJ_FOCUS_CENTER].push_back(pGameObject);

	const ENGINE::CComponent*	pTransform = dynamic_cast<CCamTarget*>(pGameObject)->m_pInfo;

	pGameObject = CStaticCamera::Create(m_pDevice, dynamic_cast<const ENGINE::CTransform*>(pTransform));
	m_ObjList[OBJ_CAMERA].push_back(pGameObject);

	return S_OK;
}

void CObjToolView::Update(void)
{

}

void CObjToolView::Render(void)
{

}

void CObjToolView::Release(void)
{
	ENGINE::Safe_Single_Destory(m_pGraphicDev);
	ENGINE::Safe_Single_Destory(m_pManagement);
	ENGINE::Safe_Single_Destory(m_pTimeMgr);
	ENGINE::Safe_Single_Destory(m_pResourcesMgr);
	ENGINE::Safe_Single_Destory(m_pInfoSubject);
	ENGINE::Safe_Delete(m_pInfo);
}

HRESULT CObjToolView::Add_GameLogic(void)
{
	return S_OK;
}

void CObjToolView::SetDirection(void)
{
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &ENGINE::g_vLook, &m_pInfo->m_matWorld);
	D3DXVec3Normalize(&m_pInfo->m_vDir, &m_pInfo->m_vDir);
}

void CObjToolView::SetTransform(void)
{

}

int CObjToolView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}

void CObjToolView::CreatePickingCube(ENGINE::PICKINGID ePickID, 
									 //const ENGINE::ITVXYZ* tItvXYZ,
									 D3DXVECTOR3 OriCube)
{
	D3DXVECTOR3 NewCubeVec = OriCube;

	switch(ePickID)
	{
	case ENGINE::PICK_Z_P:
		NewCubeVec.z += (2);
		break;

	case ENGINE::PICK_Z_M:
		NewCubeVec.z += (-2);
		break;

	case ENGINE::PICK_Y_P:
		NewCubeVec.y += (2 );
		break;

	case ENGINE::PICK_Y_M:
		NewCubeVec.y += (-2);
		break;

	case ENGINE::PICK_X_P:
		NewCubeVec.x += (2);
		break;

	case ENGINE::PICK_X_M:
		NewCubeVec.x += (-2);
		break;
	}

	CMainFrame* pMainFrame	= (CMainFrame*)AfxGetMainWnd();
	CMyForm*	pMyForm		= (CMyForm*)pMainFrame->m_wndSplitter.GetPane(0, 0);

	ENGINE::CGameObject*	pGameObject = NULL;

	pGameObject = CCubeObj::Create(m_pDevice, L"Buffer_CubeTex");
	dynamic_cast<CCubeObj*>(pGameObject)->SetPos(NewCubeVec);
	dynamic_cast<CCubeObj*>(pGameObject)->m_eCubeSaveType = m_eSaveCubeType;

	pMyForm->m_Slider_X.SetPos(1000);
	pMyForm->m_Slider_Y.SetPos(1000);
	pMyForm->m_Slider_Z.SetPos(1000);

	pMyForm->Invalidate(FALSE);

	m_ObjList[OBJ_OBJECT].push_back(pGameObject);
}

void CObjToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CMainFrame* pMainFrame	= (CMainFrame*)AfxGetMainWnd();
	CMyForm*	pMyForm		= (CMyForm*)pMainFrame->m_wndSplitter.GetPane(0, 0);

	if(pMyForm->m_iOption == 1)
	{
		ENGINE::CGameObject* pGameObj = NULL;

		if(m_ObjList[OBJ_OBJECT].size() == 0)
		{
			pGameObj = CCubeObj::Create(m_pDevice, L"Buffer_CubeTex");
			dynamic_cast<CCubeObj*>(pGameObj)->SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));
			m_ObjList[OBJ_OBJECT].push_back(pGameObj);
		}
		else
		{
			OBJLIST::iterator iter		=	m_ObjList[OBJ_OBJECT].begin();
			OBJLIST::iterator iter_end	=	m_ObjList[OBJ_OBJECT].end();

			float fDistance = 9999999999.f;

			for(; iter != iter_end; ++iter)
			{ 
				if(dynamic_cast<CCubeObj*>((*iter))->m_wstrResourceKey == L"Buffer_CubeTex")
				{
					dynamic_cast<CCubeObj*>((*iter))->PickCube(point);

					if(fDistance > dynamic_cast<CCubeObj*>(*iter)->m_fDist)
					{
						fDistance = dynamic_cast<CCubeObj*>(*iter)->m_fDist;

						pGameObj = (*iter);
					}
				}
			}

			if(pGameObj != NULL)
			{
				if(pMyForm->m_ComboSaveType.GetCurSel() == -1)
				{
					return;
				}			

				switch(pMyForm->m_ComboSaveType.GetCurSel())
				{
				case 0://Cube
					m_eSaveCubeType = ENGINE::SOBJ_CUBE;
					break;

				case 1://Right_Arm_Up
					m_eSaveCubeType = ENGINE::SOBJ_R_ARM_UP;
					break;

				case 2://Right_Arm_Down
					m_eSaveCubeType = ENGINE::SOBJ_R_ARM_DOWN;
					break;

				case 3://Left_Arm_Up
					m_eSaveCubeType = ENGINE::SOBJ_L_ARM_UP;
					break;

				case 4://Left_Arm_Down
					m_eSaveCubeType = ENGINE::SOBJ_L_ARM_DOWN;
					break;

				case 5://Right_Leg
					m_eSaveCubeType = ENGINE::SOBJ_R_LEG;
					break;

				case 6://Left_Leg
					m_eSaveCubeType = ENGINE::SOBJ_L_LEG;
					break;

				case 7://Body
					m_eSaveCubeType = ENGINE::SOBJ_BODY;
					break;

				case 8://Head
					m_eSaveCubeType = ENGINE::SOBJ_HEAD;
					break;

				default:
					m_eSaveCubeType = ENGINE::SOBJ_END;
					break;
				}

				if(m_eSaveCubeType == ENGINE::SOBJ_END)
				{
					return;
				}

				CreatePickingCube(dynamic_cast<CCubeObj*>(pGameObj)->m_ePickID, 
				//dynamic_cast<CCubeObj*>(pGameObj)->m_pBuffer->GetItv(), 
				dynamic_cast<CCubeObj*>(pGameObj)->m_pInfo->m_vPos);
			}
		}		
	}
	else if(pMyForm->m_iOption == 2)
	{
		ENGINE::CGameObject* pGameObj = NULL;

		OBJLIST::iterator iter		=	m_ObjList[OBJ_OBJECT].begin();
		OBJLIST::iterator iter_end	=	m_ObjList[OBJ_OBJECT].end();

		float fDistance = 9999999999.f;

		for(; iter != iter_end; ++iter)
		{ 
			if(dynamic_cast<CCubeObj*>((*iter))->m_wstrResourceKey == L"Buffer_CubeTex")
			{
				dynamic_cast<CCubeObj*>((*iter))->PickCube(point);

				if(fDistance > dynamic_cast<CCubeObj*>(*iter)->m_fDist)
				{
					m_ObjList[OBJ_OBJECT].erase(iter);
					break;					
				}
			}
		}
	}
	else if(pMyForm->m_iOption == 3)
	{
		CMainFrame* pMainFrame	= (CMainFrame*)AfxGetMainWnd();
		CMyForm*	pMyForm		= (CMyForm*)pMainFrame->m_wndSplitter.GetPane(0, 0);

		int iSel = pMyForm->m_iDrawID;

		ENGINE::CGameObject* pGameObj = NULL;

		OBJLIST::iterator iter		=	m_ObjList[OBJ_OBJECT].begin();
		OBJLIST::iterator iter_end	=	m_ObjList[OBJ_OBJECT].end();

		float fDistance = 9999999999.f;

		for(; iter != iter_end; ++iter)
		{ 
			if(dynamic_cast<CCubeObj*>((*iter))->m_wstrResourceKey == L"Buffer_CubeTex")
			{
				dynamic_cast<CCubeObj*>((*iter))->PickCube(point);

				if(fDistance > dynamic_cast<CCubeObj*>(*iter)->m_fDist)
				{
					fDistance = dynamic_cast<CCubeObj*>(*iter)->m_fDist;

					pGameObj = (*iter);
				}
			}
		}

		if(pGameObj != NULL)
			dynamic_cast<CCubeObj*>(pGameObj)->SetTexture(iSel);
	}
	else if(pMyForm->m_iOption == 4)
	{
		ENGINE::CGameObject* pGameObj = NULL;

		OBJLIST::iterator iter		=	m_ObjList[OBJ_OBJECT].begin();
		OBJLIST::iterator iter_end	=	m_ObjList[OBJ_OBJECT].end();

		float fDistance = 9999999999.f;

		for(; iter != iter_end; ++iter)
		{ 
			if(dynamic_cast<CCubeObj*>((*iter))->m_wstrResourceKey == L"Buffer_CubeTex")
			{
				dynamic_cast<CCubeObj*>((*iter))->m_bMove = false;
				dynamic_cast<CCubeObj*>((*iter))->PickCube(point);

				if(fDistance > dynamic_cast<CCubeObj*>(*iter)->m_fDist)
				{
					fDistance = dynamic_cast<CCubeObj*>(*iter)->m_fDist;
					pGameObj = (*iter);				
				}
			}
		}	

		CMainFrame* pMainFrame	= (CMainFrame*)AfxGetMainWnd();
		CMyForm*	pMyForm		= (CMyForm*)pMainFrame->m_wndSplitter.GetPane(0, 0);

		if(pGameObj != NULL)
		{
			dynamic_cast<CCubeObj*>(pGameObj)->m_bMove = true;

			pMyForm->m_pClickCube = pGameObj;
			pMyForm->m_iCubeType = dynamic_cast<CCubeObj*>(pGameObj)->m_eCubeSaveType;
			pMyForm->OnEnChangeCubetype();
			m_pOptionObject = pGameObj;
		}
	}
	else if(pMyForm->m_iOption == 5)
	{
		pMyForm->m_pClickCube = NULL;

		ENGINE::CGameObject* pGameObj = NULL;

		OBJLIST::iterator iter		=	m_ObjList[OBJ_OBJECT].begin();
		OBJLIST::iterator iter_end	=	m_ObjList[OBJ_OBJECT].end();

		float fDistance = 9999999999.f;

		for(; iter != iter_end; ++iter)
		{ 
			if(dynamic_cast<CCubeObj*>((*iter))->m_wstrResourceKey == L"Buffer_CubeTex")
			{
				dynamic_cast<CCubeObj*>((*iter))->m_bMove = false;
				dynamic_cast<CCubeObj*>((*iter))->PickCube(point);

				if(fDistance > dynamic_cast<CCubeObj*>(*iter)->m_fDist)
				{
					fDistance = dynamic_cast<CCubeObj*>(*iter)->m_fDist;
					pGameObj = (*iter);				
				}
			}
		}
		
		if(pGameObj != NULL)
		{
			pMyForm->m_Slider_X.SetPos(int((dynamic_cast<CCubeObj*>(pGameObj)->m_fItv_X + 1.f) * 10000.f)); 
			pMyForm->m_Slider_Y.SetPos(int((dynamic_cast<CCubeObj*>(pGameObj)->m_fItv_Y + 1.f) * 10000.f));
			pMyForm->m_Slider_Z.SetPos(int((dynamic_cast<CCubeObj*>(pGameObj)->m_fItv_Z + 1.f) * 10000.f));

			for(int i=0; i<8; ++i)
			{
				dynamic_cast<CCubeObj*>(pGameObj)->m_pVertex[i].vPos = dynamic_cast<CCubeObj*>(pGameObj)->m_pOriginPos[i];
			}

			pMyForm->m_pClickCube = pGameObj;
			m_pOptionObject	= pGameObj;

			pMyForm->m_iCubeType = dynamic_cast<CCubeObj*>(pGameObj)->m_eCubeSaveType;
			pMyForm->OnEnChangeCubetype();

			/*for(size_t i = 0; i < dynamic_cast<CCubeObj*>(pGameObj)->m_dwVtxCnt; ++i)
			{
				if(i >= 0 && i < 4)
				{
					dynamic_cast<CCubeObj*>(pGameObj)->m_pVertex[i].vPos.z -= pMyForm->fSize_Z;
				}
				else
				{
					dynamic_cast<CCubeObj*>(pGameObj)->m_pVertex[i].vPos.z += pMyForm->fSize_Z;
				}
			}

			for(size_t i = 0; i < dynamic_cast<CCubeObj*>(pGameObj)->m_dwVtxCnt; ++i)
			{
				if(i == 0 || i == 4 || i == 7 || i == 3)
				{
					dynamic_cast<CCubeObj*>(pGameObj)->m_pVertex[i].vPos.x -= pMyForm->fSize_X;
				}
				else
				{
					dynamic_cast<CCubeObj*>(pGameObj)->m_pVertex[i].vPos.x += pMyForm->fSize_X;
				}
			}

			for(size_t i = 0; i < dynamic_cast<CCubeObj*>(pGameObj)->m_dwVtxCnt; ++i)
			{
				if(i == 0 || i == 4 || i == 5 || i == 1)
				{
					dynamic_cast<CCubeObj*>(pGameObj)->m_pVertex[i].vPos.y += pMyForm->fSize_Y;
				}
				else
				{
					dynamic_cast<CCubeObj*>(pGameObj)->m_pVertex[i].vPos.y -= pMyForm->fSize_Y;
				}
			}*/

			/*dynamic_cast<CCubeObj*>(pGameObj)->m_fItv_X = pMyForm->fSize_X;
			dynamic_cast<CCubeObj*>(pGameObj)->m_fItv_Y = pMyForm->fSize_Y;
			dynamic_cast<CCubeObj*>(pGameObj)->m_fItv_Z = pMyForm->fSize_Z;*/
		}		
	}	

	CScrollView::OnLButtonDown(nFlags, point);
}
