
// MapToolView.h : CMapToolView Ŭ������ �������̽�
//


#pragma once

class CMapToolDoc;

namespace ENGINE
{
	class CGraphicDev;
	class CManagement;
	class CResourcesMgr;
	class CGameObject;
	class CComponent;
}

class CMapToolView : public CScrollView
{
public:
	enum	OBJTYPE { OBJ_FOCUS_CENTER, OBJ_OBJECT, OBJ_TERRAIN, OBJ_CAMERA, OBJ_ETC, OBJ_end };

public:
	typedef list<ENGINE::CGameObject*>		OBJLIST;
	OBJLIST							m_ObjList[OBJ_end];

public:
	ENGINE::CGraphicDev*		m_pGraphicDev;
	ENGINE::CManagement*		m_pManagement;
	ENGINE::CResourcesMgr*		m_pResourcesMgr;
	ENGINE::CGameObject*		m_pGameObject;
	ENGINE::CGameObject*		m_pTerrain;
	LPDIRECT3DDEVICE9			m_pDevice;

	int						m_pCntX;
	int						m_pCntZ;
	int						m_pItv;
	float					m_fHeight;
	bool					m_bCreate;
	bool					m_bView;
	bool					m_bSave;
	bool					m_bLoad;

private:
	void	Save();
	void	Load();


protected: // serialization������ ��������ϴ�.
	CMapToolView();
	DECLARE_DYNCREATE(CMapToolView)

// Ư���Դϴ�.
public:
	CMapToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CMapToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	HRESULT		Initialize(void);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // MapToolView.cpp�� ����� ����
inline CMapToolDoc* CMapToolView::GetDocument() const
   { return reinterpret_cast<CMapToolDoc*>(m_pDocument); }
#endif

