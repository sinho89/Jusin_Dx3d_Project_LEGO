#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CMyForm 폼 뷰입니다.
namespace ENGINE
{
	class CGameObject;
}

class CMainFrame;
class CObjToolView;
class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

private:
	map<CString, CImage*>	m_MapImage;
public:
	int		m_iDrawID;
	int		m_iOption;

	ENGINE::CGameObject*	m_pClickCube;

	TCHAR*					m_strName;


protected:
	CMyForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMyForm();

public:
	bool	m_bCubeSave;
	bool	m_bCubeLoad;
public:
	enum { IDD = IDD_MYFORM };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	// 옵션 라디오
	CListBox m_TexListBox;
	CListBox m_ObjListBox;
	
	afx_msg void OnBnClickedObjSvaeBtn();
	afx_msg void OnLbnSelchangeTextureListbox();
	afx_msg void OnLbnSelchangeObjListbox();
	
	CListBox m_Tex_ListBox;
	CListBox m_Obj_ListBox;
	virtual void OnInitialUpdate();
	CStatic m_Tex_Prev;
	
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio5();
	
	float fSize_X;
	float fSize_Y;
	float fSize_Z;

	int m_iSlider_X;
	int m_iSlider_Y;
	int m_iSlider_Z;

	CSliderCtrl m_Slider_X;
	CSliderCtrl m_Slider_Y;
	CSliderCtrl m_Slider_Z;
	
	afx_msg void OnNMCustomdrawSliderX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderZ(NMHDR *pNMHDR, LRESULT *pResult);

	float m_fPos_X;
	float m_fPos_Y;
	float m_fPos_Z;
	afx_msg void OnEnChangePosX();
	afx_msg void OnEnChangePosY();
	afx_msg void OnEnChangePosZ();

	afx_msg void OnBnClickedObjSvaeBtn2();
	CString m_strSaveName;
	afx_msg void OnEnChangeDataname();
	CComboBox m_ComboSaveType;
	afx_msg void OnCbnSelchangeSavetype();

	CString m_CubeType;
	int		m_iCubeType;

	afx_msg void OnEnChangeCubetype();
	CButton m_optionButton;
};


