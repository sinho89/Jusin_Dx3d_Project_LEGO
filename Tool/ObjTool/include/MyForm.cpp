// MyForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ObjTool.h"
#include "MyForm.h"
#include "CubeObj.h"
#include "Transform.h"
#include "MainFrm.h"
#include "ObjToolView.h"

// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(CMyForm::IDD)
	, m_iOption(0)
	, fSize_X(0)
	, fSize_Y(0)
	, fSize_Z(0)
	, m_fPos_X(0)
	, m_fPos_Y(0)
	, m_fPos_Z(0)
	, m_iSlider_X(10000)
	, m_iSlider_Y(10000)
	, m_iSlider_Z(10000)
	, m_pClickCube(NULL)
	, m_bCubeSave(false)
	, m_bCubeLoad(false)
	, m_strSaveName(_T(""))
	, m_CubeType(_T(""))
	, m_iCubeType(0)
{
	
}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXTURE_LISTBOX, m_Tex_ListBox);
	DDX_Control(pDX, IDC_OBJ_LISTBOX, m_Obj_ListBox);
	DDX_Text(pDX, IDC_X_EDIT, fSize_X);
	DDX_Text(pDX, IDC_Y_EDIT, fSize_Y);
	DDX_Text(pDX, IDC_Z_EDIT, fSize_Z);
	DDX_Control(pDX, IDC_SLIDER_X, m_Slider_X);
	DDX_Control(pDX, IDC_SLIDER_Y, m_Slider_Y);
	DDX_Control(pDX, IDC_SLIDER_Z, m_Slider_Z);
	DDX_Text(pDX, IDC_POS_X, m_fPos_X);
	DDX_Text(pDX, IDC_POS_Y, m_fPos_Y);
	DDX_Text(pDX, IDC_POS_Z, m_fPos_Z);
	DDX_Text(pDX, IDC_DATANAME, m_strSaveName);
	DDX_Control(pDX, IDC_SAVETYPE, m_ComboSaveType);
	DDX_Text(pDX, IDC_CUBETYPE, m_CubeType);
	DDX_Control(pDX, IDC_RADIO1, m_optionButton);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_BN_CLICKED(IDC_OBJ_SVAE_BTN, &CMyForm::OnBnClickedObjSvaeBtn)
	ON_LBN_SELCHANGE(IDC_TEXTURE_LISTBOX, &CMyForm::OnLbnSelchangeTextureListbox)
	ON_LBN_SELCHANGE(IDC_OBJ_LISTBOX, &CMyForm::OnLbnSelchangeObjListbox)
	ON_BN_CLICKED(IDC_RADIO1, &CMyForm::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CMyForm::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CMyForm::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CMyForm::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO5, &CMyForm::OnBnClickedRadio5)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_X, &CMyForm::OnNMCustomdrawSliderX)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_Y, &CMyForm::OnNMCustomdrawSliderY)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_Z, &CMyForm::OnNMCustomdrawSliderZ)
	ON_EN_CHANGE(IDC_POS_X, &CMyForm::OnEnChangePosX)
	ON_EN_CHANGE(IDC_POS_Y, &CMyForm::OnEnChangePosY)
	ON_EN_CHANGE(IDC_POS_Z, &CMyForm::OnEnChangePosZ)
	ON_BN_CLICKED(IDC_OBJ_SVAE_BTN2, &CMyForm::OnBnClickedObjSvaeBtn2)
	ON_EN_CHANGE(IDC_DATANAME, &CMyForm::OnEnChangeDataname)
	ON_CBN_SELCHANGE(IDC_SAVETYPE, &CMyForm::OnCbnSelchangeSavetype)
	ON_EN_CHANGE(IDC_CUBETYPE, &CMyForm::OnEnChangeCubetype)
END_MESSAGE_MAP()


// CMyForm 진단입니다.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

void CMyForm::OnBnClickedObjSvaeBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);


	m_bCubeSave = true;

	//큐브저장성공
	
}

void CMyForm::OnLbnSelchangeTextureListbox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString strSelectName;

	int iSelNum = m_Tex_ListBox.GetCurSel();
	m_Tex_ListBox.GetText(iSelNum, strSelectName);

	map<CString, CImage*>::iterator iter = m_MapImage.find(strSelectName);

	if(iter == m_MapImage.end())
		return;

	/*m_Tex_Prev.SetBitmap(*iter->second);

	CRect Rect;
	m_Tex_Prev.GetClientRect(Rect);
	m_Tex_Prev.MoveWindow(220, 80, 100, 100);*/

	int iSel = m_Tex_ListBox.GetCurSel();

	m_Tex_ListBox.GetText(iSel, strSelectName);

	int i=0;
	for(i; i<strSelectName.GetLength(); ++i)
	{
		if(isdigit(strSelectName[i]) != 0)
			break;
	}

	strSelectName.Delete(0, i);

	m_iDrawID = _tstoi(strSelectName);

	UpdateData(FALSE);
}

void CMyForm::OnLbnSelchangeObjListbox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString strSelectName;

	int iSelNum = m_Obj_ListBox.GetCurSel();
	m_Obj_ListBox.GetText(iSelNum, strSelectName);

	CMainFrame*		pMainFrame	= (CMainFrame*)AfxGetMainWnd();
	CObjToolView*	pMyObjView	= (CObjToolView*)pMainFrame->m_wndSplitter.GetPane(0, 1);

	//pMyObjView->szLoadFile = (TCHAR*)(LPCTSTR)strSelectName;

	TCHAR*	tSelectName = (TCHAR*)(LPCTSTR)strSelectName;

	lstrcpy(pMyObjView->szLoadFile, tSelectName);

	UpdateData(FALSE);
}

void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_ComboSaveType.AddString(_T("CUBE"));
	m_ComboSaveType.AddString(_T("RIGHT_ARM_UP"));
	m_ComboSaveType.AddString(_T("RIGHT_ARM_DOWN"));
	m_ComboSaveType.AddString(_T("LEFT_ARM_UP"));
	m_ComboSaveType.AddString(_T("LEFT_ARM_DOWN"));
	m_ComboSaveType.AddString(_T("RIGHT_LEG"));
	m_ComboSaveType.AddString(_T("LEFT_LEG"));
	m_ComboSaveType.AddString(_T("BODY"));
	m_ComboSaveType.AddString(_T("HEAD"));

	//Slider초기화
	m_Slider_X.SetRange(0, 110000);
	m_Slider_Y.SetRange(0, 110000);
	m_Slider_Z.SetRange(0, 110000);

	m_Slider_X.SetPos(m_iSlider_X);
	m_Slider_Y.SetPos(m_iSlider_Y);
	m_Slider_Z.SetPos(m_iSlider_Z);

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	TCHAR	szForderPath[MAX_PATH] = L"../../../Texture/Cube_dds/";
	char	szFullPath[MAX_PATH]  = "";

	WideCharToMultiByte(CP_ACP, NULL,
		szForderPath, -1, szFullPath, MAX_PATH, NULL, FALSE);

	_finddata_t		fd;
	long			handle;
	int				iResult = 1;
	
	strcat_s(szFullPath, "/*.*");
	handle = _findfirst(szFullPath, &fd);

	if(handle == -1)
		return;

	while(iResult != -1)
	{
		CString strPath = szForderPath;
		strPath += L"/";
		strPath += fd.name;

		CString strImageName;
		strImageName = PathFindFileName(strPath);
		PathRemoveExtension((LPWSTR)strImageName.operator LPCWSTR());

		if(strImageName != "" && strImageName != "." && strImageName != "..")
		{
			map<CString, CImage*>::iterator iter = m_MapImage.find(strImageName);
			if(iter == m_MapImage.end())
			{
				CImage* pImage = new CImage;

				pImage->Load(strPath);			
				m_MapImage.insert(map<CString, CImage*>::value_type(strImageName, pImage));
				m_Tex_ListBox.AddString(strImageName);
			}			
		}	
		iResult = _findnext(handle, &fd);
	}


	lstrcpy(szForderPath, L"../../../Data/Object_Data/");
	strcpy_s(szFullPath, "");

	WideCharToMultiByte(CP_ACP, NULL,
		szForderPath, -1, szFullPath, MAX_PATH, NULL, FALSE);

	iResult = 1;
	
	strcat_s(szFullPath, "/*.dat");
	handle = _findfirst(szFullPath, &fd);

	if(handle == -1)
		return;

	while(iResult != -1)
	{
		CString strPath = szForderPath;
		strPath += L"/";
		strPath += fd.name;

		CString strImageName;
		strImageName = PathFindFileName(strPath);
		PathRemoveExtension((LPWSTR)strImageName.operator LPCWSTR());

		if(strImageName != "" && strImageName != "." && strImageName != "..")
		{		
			m_Obj_ListBox.AddString(strImageName);						
		}	
		iResult = _findnext(handle, &fd);
	}	
}

void CMyForm::OnBnClickedRadio1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	
	CMainFrame*		pMainFrame	= (CMainFrame*)AfxGetMainWnd();
	CObjToolView*	pMyObjView	= (CObjToolView*)pMainFrame->m_wndSplitter.GetPane(0, 1);

	m_iOption = 1;

	m_pClickCube = NULL;

	UpdateData(FALSE);
}

void CMyForm::OnBnClickedRadio2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CMainFrame*		pMainFrame	= (CMainFrame*)AfxGetMainWnd();
	CObjToolView*	pMyObjView	= (CObjToolView*)pMainFrame->m_wndSplitter.GetPane(0, 1);
	m_iOption =2;

	m_pClickCube = NULL;

	UpdateData(FALSE);
}

void CMyForm::OnBnClickedRadio3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CMainFrame*		pMainFrame	= (CMainFrame*)AfxGetMainWnd();
	CObjToolView*	pMyObjView	= (CObjToolView*)pMainFrame->m_wndSplitter.GetPane(0, 1);

	m_iOption = 3;

	m_pClickCube = NULL;
	
	UpdateData(FALSE);
}

void CMyForm::OnBnClickedRadio4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CMainFrame*		pMainFrame	= (CMainFrame*)AfxGetMainWnd();
	CObjToolView*	pMyObjView	= (CObjToolView*)pMainFrame->m_wndSplitter.GetPane(0, 1);
	
	m_iOption = 4;

	m_pClickCube = NULL;

	UpdateData(FALSE);
}

void CMyForm::OnBnClickedRadio5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CMainFrame*		pMainFrame	= (CMainFrame*)AfxGetMainWnd();
	CObjToolView*	pMyObjView	= (CObjToolView*)pMainFrame->m_wndSplitter.GetPane(0, 1);

	pMyObjView->m_bOptionCreate = false;
	pMyObjView->m_bOptionDelete = false;
	pMyObjView->m_bOptionTexture = false;
	pMyObjView->m_bOptionMove = false;
	pMyObjView->m_bOptionSize = true;

	m_iOption = 5;

	m_pClickCube = NULL;
	
	UpdateData(FALSE);
}

void CMyForm::OnNMCustomdrawSliderX(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(TRUE);
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static int prePos = 0;
	int newPos = m_Slider_X.GetPos();

	if(prePos != newPos)
	{
		prePos = newPos;
		m_iSlider_X = m_Slider_X.GetRangeMax() - newPos + 1;
		m_Slider_X.SetPos(newPos);
		TRACE("슬라이더 : %d\r\n", m_iSlider_X);

		HWND hWnd = ::GetFocus();
		HWND NewxWnd;

		NewxWnd = ::GetNextDlgTabItem(ENGINE::g_hWnd, hWnd, TRUE);
		::SetFocus(NewxWnd);
	}

	fSize_X = (float(prePos)/10000.f)-1;
	
	if(m_pClickCube != NULL)
	{
		dynamic_cast<CCubeObj*>(m_pClickCube)->m_fItv_X = fSize_X;

		//for(int i=0; i<8; ++i)
		//{
		//	dynamic_cast<CCubeObj*>(m_pClickCube)->m_pVertex[i].vPos.x = dynamic_cast<CCubeObj*>(m_pClickCube)->m_pOriginPos[i].x;
		//}

		//for(size_t i = 0; i < dynamic_cast<CCubeObj*>(m_pClickCube)->m_dwVtxCnt; ++i)
		//{
		//	if(i == 0 || i == 4 || i == 7 || i == 3)
		//	{
		//		dynamic_cast<CCubeObj*>(m_pClickCube)->m_pVertex[i].vPos.x -= fSize_X;
		//	}
		//	else
		//	{
		//		dynamic_cast<CCubeObj*>(m_pClickCube)->m_pVertex[i].vPos.x += fSize_X;
		//	}
		//}

		//dynamic_cast<CCubeObj*>(m_pClickCube)->m_fItv_X = fSize_X - 1.f;
	}

	*pResult = 0;
	UpdateData(FALSE);
}

void CMyForm::OnNMCustomdrawSliderY(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(TRUE);
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static int prePos = 0;
	int newPos = m_Slider_Y.GetPos();

	if(prePos != newPos)
	{
		prePos = newPos;
		m_iSlider_Y = m_Slider_Y.GetRangeMax() - newPos + 1;
		m_Slider_Y.SetPos(newPos);
		TRACE("슬라이더 : %d\r\n", m_iSlider_Y);

		HWND hWnd = ::GetFocus();
		HWND NewxWnd;

		NewxWnd = ::GetNextDlgTabItem(ENGINE::g_hWnd, hWnd, TRUE);
		::SetFocus(NewxWnd);
	}

	fSize_Y = (float(prePos)/10000.f)-1;
	
	if(m_pClickCube != NULL)
	{
		dynamic_cast<CCubeObj*>(m_pClickCube)->m_fItv_Y = fSize_Y;

		//for(int i=0; i<8; ++i)
		//{
		//	dynamic_cast<CCubeObj*>(m_pClickCube)->m_pVertex[i].vPos.y = dynamic_cast<CCubeObj*>(m_pClickCube)->m_pOriginPos[i].y;
		//}

		//for(size_t i = 0; i < dynamic_cast<CCubeObj*>(m_pClickCube)->m_dwVtxCnt; ++i)
		//{
		//	if(i == 0 || i == 4 || i == 5 || i == 1)
		//	{
		//		dynamic_cast<CCubeObj*>(m_pClickCube)->m_pVertex[i].vPos.y += fSize_Y;
		//	}
		//	else
		//	{
		//		dynamic_cast<CCubeObj*>(m_pClickCube)->m_pVertex[i].vPos.y -= fSize_Y;
		//	}
		//}

		//dynamic_cast<CCubeObj*>(m_pClickCube)->m_fItv_Y = fSize_Y - 1.f;
	}

	*pResult = 0;
	UpdateData(FALSE);
}

void CMyForm::OnNMCustomdrawSliderZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static int prePos = 0;
	int newPos = m_Slider_Z.GetPos();

	if(prePos != newPos)
	{
		prePos = newPos;
		m_iSlider_Z = m_Slider_Z.GetRangeMax() - newPos + 1;
		m_Slider_Z.SetPos(newPos);
		TRACE("슬라이더 : %d\r\n", m_iSlider_Z);

		HWND hWnd = ::GetFocus();
		HWND NewxWnd;

		NewxWnd = ::GetNextDlgTabItem(ENGINE::g_hWnd, hWnd, TRUE);
		::SetFocus(NewxWnd);
	}

	fSize_Z = (float(prePos)/10000.f)-1;
	
	if(m_pClickCube != NULL)
	{
		dynamic_cast<CCubeObj*>(m_pClickCube)->m_fItv_Z = fSize_Z;

		/*for(int i=0; i<8; ++i)
		{
			dynamic_cast<CCubeObj*>(m_pClickCube)->m_pVertex[i].vPos.z = dynamic_cast<CCubeObj*>(m_pClickCube)->m_pOriginPos[i].z;
		}

		for(size_t i = 0; i < dynamic_cast<CCubeObj*>(m_pClickCube)->m_dwVtxCnt; ++i)
		{
			if(i >= 0 && i < 4)
			{
				dynamic_cast<CCubeObj*>(m_pClickCube)->m_pVertex[i].vPos.z -= fSize_Z;
			}
			else
			{
				dynamic_cast<CCubeObj*>(m_pClickCube)->m_pVertex[i].vPos.z += fSize_Z;
			}
		}

		dynamic_cast<CCubeObj*>(m_pClickCube)->m_fItv_Z = fSize_Z - 1.f;*/
	}

	*pResult = 0;
	UpdateData(FALSE);
}


void CMyForm::OnEnChangePosX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_fPos_X = dynamic_cast<CCubeObj*>(m_pClickCube)->m_pInfo->m_vPos.x;
	UpdateData(FALSE);
}

void CMyForm::OnEnChangePosY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_fPos_Y = dynamic_cast<CCubeObj*>(m_pClickCube)->m_pInfo->m_vPos.y;
	UpdateData(FALSE);
}

void CMyForm::OnEnChangePosZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_fPos_Z = dynamic_cast<CCubeObj*>(m_pClickCube)->m_pInfo->m_vPos.z;
	UpdateData(FALSE);
}

void CMyForm::OnBnClickedObjSvaeBtn2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	m_bCubeLoad = true;

	UpdateData(FALSE);
}

void CMyForm::OnEnChangeDataname()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	
	UpdateData(FALSE);
}
void CMyForm::OnCbnSelchangeSavetype()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	int index = m_ComboSaveType.GetCurSel();
	UpdateData(FALSE);
}

void CMyForm::OnEnChangeCubetype()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	switch(m_iCubeType)
	{
	case 0:
		m_CubeType	=	L"Cube";
		break;

	case 1:
		m_CubeType	=	L"Right_Arm_Up";
		break;

	case 2:
		m_CubeType	=	L"Right_Arm_Down";
		break;

	case 3:
		m_CubeType	=	L"Left_Arm_Up";
		break;

	case 4:
		m_CubeType	=	L"Left_Arm_Down";
		break;

	case 5:
		m_CubeType	=	L"Right_Leg";
		break;

	case 6:
		m_CubeType	=	L"Left_Leg";
		break;

	case 7:
		m_CubeType	=	L"Body";
		break;

	case 8:
		m_CubeType	=	L"Head";
		break;
	}

	UpdateData(FALSE);
}
