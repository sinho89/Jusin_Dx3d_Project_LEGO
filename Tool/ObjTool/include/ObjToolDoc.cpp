
// ObjToolDoc.cpp : CObjToolDoc Ŭ������ ����
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


// CObjToolDoc ����/�Ҹ�

CObjToolDoc::CObjToolDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

CObjToolDoc::~CObjToolDoc()
{
}

BOOL CObjToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CObjToolDoc serialization

void CObjToolDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}


// CObjToolDoc ����

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


// CObjToolDoc ���
