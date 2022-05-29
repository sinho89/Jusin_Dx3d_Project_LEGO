#include "VIBuffer.h"

ENGINE::CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pDevice)
: ENGINE::CResources(pDevice)
, m_pVB(NULL)
, m_pIB(NULL)
, m_dwVtxSize(0)
, m_dwVtxCnt(0)
, m_dwVtxFVF(0)
, m_dwTriCnt(0)
, m_dwIdxSize(0)
{

}

ENGINE::CVIBuffer::~CVIBuffer(void)
{
	Release();
}

HRESULT ENGINE::CVIBuffer::CreateBuffer(void)
{
	m_pDevice->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCnt, 
		0, 
		m_dwVtxFVF, 
		D3DPOOL_MANAGED, 
		&m_pVB, 
		NULL);


	m_pDevice->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt, 
		0, 
		m_IdxFmt, 
		D3DPOOL_MANAGED, 
		&m_pIB,
		NULL);

	return S_OK;
}

void ENGINE::CVIBuffer::Render(void)
{
	m_pDevice->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);

	m_pDevice->SetFVF(m_dwVtxFVF);

	m_pDevice->SetIndices(m_pIB);	

	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}

void ENGINE::CVIBuffer::Release(void)
{
	if((*m_pwRefCnt) == 0)
	{
		ENGINE::Safe_Release(m_pVB);
		ENGINE::Safe_Release(m_pIB);

		CResources::Release();
	}
	else
	{
		--(*m_pwRefCnt);
	}
}

ENGINE::CResources* ENGINE::CVIBuffer::CloneResource(void)
{
	++(*m_pwRefCnt);

	return new CVIBuffer(*this);	
}

void ENGINE::CVIBuffer::SetVtxInfo(void* pVertex)
{
	void*		pOriVertex = NULL;

	m_pVB->Lock(0, 0, &pOriVertex, 0);

	memcpy(pOriVertex, pVertex, m_dwVtxSize * m_dwVtxCnt);

	m_pVB->Unlock();
}

void ENGINE::CVIBuffer::GetVtxInfo(void* pVertex)
{
	void*		pOriVertex = NULL;

	m_pVB->Lock(0, 0, &pOriVertex, 0);

	memcpy(pVertex, pOriVertex, m_dwVtxSize * m_dwVtxCnt);

	m_pVB->Unlock();
}

DWORD* ENGINE::CVIBuffer::GetVtxSize()
{
	return &m_dwVtxSize;
}

DWORD* ENGINE::CVIBuffer::GetVtxCnt()
{
	return &m_dwVtxCnt;
}

DWORD* ENGINE::CVIBuffer::GetVtxFVF()
{
	return &m_dwVtxFVF;
}

DWORD* ENGINE::CVIBuffer::GetTriCnt()
{
	return &m_dwTriCnt;
}

const ENGINE::ITVXYZ* ENGINE::CVIBuffer::GetItv(void)
{
	return &m_tItv;
}


