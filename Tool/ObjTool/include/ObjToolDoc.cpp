
// ObjToolDoc.cpp : CObjToolDoc 클래스의 구현
//

#include "stdafx.h"
#include "ObjTool.h"

#include "ObjToolDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CObjToolDoc

IMPLEMENT_DYNCREATE(CObjToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CObjToolDoc, CDocument)
END_MESSAGE_MAP()


// CObjToolDoc 생성/소멸

CObjToolDoc::CObjToolDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CObjToolDoc::~CObjToolDoc()
{
}

BOOL CObjToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CObjToolDoc serialization

void CObjToolDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}


// CObjToolDoc 진단

#ifdef _DEBUG
void CObjToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CObjToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CObjToolDoc 명령
