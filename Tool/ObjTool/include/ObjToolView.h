
// ObjToolView.h : CObjToolView 클래스의 인터페이스
//


#pragma once

namespace ENGINE
{
	class CGraphicDev;
	class CResourcesMgr;
	class CTransform;
	class CTimeMgr;
	class CVIBuffer;
	class CManagement;

	class CComponent;
	class CGameObject;
	class CInfoSubject;	
}

class CObjToolDoc;
class CCameraObserver;
class CStaticCamera;
class CObjToolView : public CScrollView
{
protected: // serialization에서만 만들어집니다.
	CObjToolView();
	DECLARE_DYNCREATE(CObjToolView)

public:
	enum	OBJTYPE { OBJ_FOCUS_CENTER, OBJ_OBJECT, OBJ_TERRAIN, OBJ_LINE, OBJ_CAMERA, OBJ_ETC, OBJ_end };

// 특성입니다.
public:
	CObjToolDoc* GetDocument() const;
public:
	typedef list<ENGINE::CGameObject*>		OBJLIST;
	OBJLIST							m_ObjList[OBJ_end];

public:
	D3DXVECTOR3				XYZ_Line[6];

	ENGINE::CGraphicDev*	m_pGraphicDev;
	ENGINE::CManagement*	m_pManagement;
	LPDIRECT3DDEVICE9		m_pDevice;
	ENGINE::CResourcesMgr*	m_pResourcesMgr;
	ENGINE::CVIBuffer*		m_pBuffer;
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CTransform*		m_pInfo;
	ENGINE::CInfoSubject*	m_pInfoSubject;

	ENGINE::CGameObject*	m_pOptionObject;

	ENGINE::SAVEOBJID		m_eSaveCubeType;

	//선긋기
	//ID3DXLine*				m_pLine;

	//임시
	CCameraObserver*		m_pCamObserver;
	
	bool					m_bSave;
	bool					m_bLoad;

	bool					m_bView;

	TCHAR					szLoadFile[MAX_PATH];

	bool					m_bOptionCreate;
	bool					m_bOptionDelete;
	bool					m_bOptionTexture;
	bool					m_bOptionMove;
	bool					m_bOptionSize;
	
// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);


protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.

public:
	void	CubeSave(void);
	void	CubeLoad(void);
public:
	virtual ~CObjToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	HRESULT		Initialize(void);
	HRESULT		Add_GameLogic(void);
	void		SetDirection(void);
	void		SetTransform(void);
	void		Update(void);
	void		Render(void);
	void		Release(void);
	
	void		CreatePickingCube(ENGINE::PICKINGID ePickID, 
									//const ENGINE::ITVXYZ* tItvXYZ,
									D3DXVECTOR3 vPos);

protected:
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // ObjToolView.cpp의 디버그 버전
inline CObjToolDoc* CObjToolView::GetDocument() const
   { return reinterpret_cast<CObjToolDoc*>(m_pDocument); }
#endif

