#pragma once
#include "afxwin.h"



// MyForm �� ���Դϴ�.
namespace ENGINE
{
	class CResourcesMgr;
	class CGameObject;
	class CGraphicDev;
}
class MyForm : public CFormView
{
	DECLARE_DYNCREATE(MyForm)
public:
	bool	m_bCreate;
	bool	m_bHeight;
	bool	m_bSave;
	bool	m_bLoad;
public:
	ENGINE::CGameObject*		m_pObject;
protected:
	MyForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~MyForm();

public:
	ENGINE::CResourcesMgr*		m_pResourcesMgr;
	ENGINE::CGraphicDev*		m_pGraphicDev;
public:
	enum { IDD = IDD_MYFORM };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strMapKey;
	int m_CntX;
	int m_CntZ;
	int m_Itv;
	float m_fHeight;
	CListBox m_Maplist;
	afx_msg void OnBnClickedCreate();
	afx_msg void OnBnClickedHeightOk();
	afx_msg void OnBnClickedMaptexOk();
	afx_msg void OnBnClickedMapSave();
	afx_msg void OnBnClickedMapLoad();
	afx_msg void OnEnChangeCntx();
	afx_msg void OnEnKillfocusCntx();
};


