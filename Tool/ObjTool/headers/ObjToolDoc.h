
// ObjToolDoc.h : CObjToolDoc Ŭ������ �������̽�
//


#pragma once


class CObjToolDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CObjToolDoc();
	DECLARE_DYNCREATE(CObjToolDoc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// �����Դϴ�.
public:
	virtual ~CObjToolDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};


