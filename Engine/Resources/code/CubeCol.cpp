#include "CubeCol.h"

ENGINE::CCubeCol::CCubeCol(LPDIRECT3DDEVICE9 pDevice)
: ENGINE::CVIBuffer(pDevice)
{

}

ENGINE::CCubeCol::~CCubeCol(void)
{
	Release();
}

HRESULT ENGINE::CCubeCol::CreateBuffer(void)
{
	m_dwVtxSize  = sizeof(VTXCUBECOL);
	m_dwVtxCnt   = 8;
	m_dwTriCnt	 = 12;
	m_dwVtxFVF   = VTXFVF_CUBE_COL;

	m_dwIdxSize	 = sizeof(INDEX32);
	m_IdxFmt	 = D3DFMT_INDEX32;
	m_dwTriCnt	 = 12;

	FAILED_CHECK(CVIBuffer::CreateBuffer());

	VTXCUBECOL*		pVtxCubeCol = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxCubeCol, 0);

	pVtxCubeCol[0].vPos		= D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxCubeCol[0].dwColor	= D3DCOLOR_ARGB(255, 255, 255, 255);

	pVtxCubeCol[1].vPos		= D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxCubeCol[1].dwColor	= D3DCOLOR_ARGB(255, 255, 255, 255);

	pVtxCubeCol[2].vPos		= D3DXVECTOR3(1.f, -1.f, -1.f);
	pVtxCubeCol[2].dwColor	= D3DCOLOR_ARGB(255, 255, 255, 255);

	pVtxCubeCol[3].vPos		= D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxCubeCol[3].dwColor	= D3DCOLOR_ARGB(255, 255, 255, 255);

	pVtxCubeCol[4].vPos		= D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxCubeCol[4].dwColor	= D3DCOLOR_ARGB(255, 255, 255, 255);

	pVtxCubeCol[5].vPos		= D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxCubeCol[5].dwColor	= D3DCOLOR_ARGB(255, 255, 255, 255);

	pVtxCubeCol[6].vPos		= D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxCubeCol[6].dwColor	= D3DCOLOR_ARGB(255, 255, 255, 255);

	pVtxCubeCol[7].vPos		= D3DXVECTOR3(-1.f, -1.f, 1.f);
	pVtxCubeCol[7].dwColor	= D3DCOLOR_ARGB(255, 255, 255, 255);

	m_pVB->Unlock();

	INDEX32*		pIndex = NULL;

	m_pIB->Lock(0, 0 , (void**)&pIndex, 0);

	// x+ 
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 5;
	pIndex[0]._3 = 6;

	pIndex[1]._1 = 1;
	pIndex[1]._2 = 6;
	pIndex[1]._3 = 2;

	// x- 
	pIndex[2]._1 = 4;
	pIndex[2]._2 = 0;
	pIndex[2]._3 = 3;

	pIndex[3]._1 = 4;
	pIndex[3]._2 = 3;
	pIndex[3]._3 = 7;

	// y+ 
	pIndex[4]._1 = 4;
	pIndex[4]._2 = 5;
	pIndex[4]._3 = 1;

	pIndex[5]._1 = 4;
	pIndex[5]._2 = 1;
	pIndex[5]._3 = 0;

	// y- 
	pIndex[6]._1 = 3;
	pIndex[6]._2 = 2;
	pIndex[6]._3 = 6;

	pIndex[7]._1 = 3;
	pIndex[7]._2 = 6;
	pIndex[7]._3 = 7;

	// z+ 
	pIndex[8]._1 = 7;
	pIndex[8]._2 = 6;
	pIndex[8]._3 = 5;

	pIndex[9]._1 = 7;
	pIndex[9]._2 = 5;
	pIndex[9]._3 = 4;

	// z- 
	pIndex[10]._1 = 0;
	pIndex[10]._2 = 1;
	pIndex[10]._3 = 2;

	pIndex[11]._1 = 0;
	pIndex[11]._2 = 2;
	pIndex[11]._3 = 3;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT ENGINE::CCubeCol::CreateBuffer(const float& wItvX, 
									   const float& wItvY, 
									   const float& wItvZ,
									   const DWORD&	dwColor)
{
	m_tItv.m_wItvX	= wItvX;
	m_tItv.m_wItvY	= wItvY;
	m_tItv.m_wItvZ	= wItvZ;

	m_dwVtxSize  = sizeof(VTXCUBECOL);
	m_dwVtxCnt   = 8;
	m_dwTriCnt	 = 12;
	m_dwVtxFVF   = VTXFVF_CUBE_COL;

	m_dwIdxSize	 = sizeof(INDEX32);
	m_IdxFmt	 = D3DFMT_INDEX32;
	m_dwTriCnt	 = 12;

	FAILED_CHECK(CVIBuffer::CreateBuffer());

	VTXCUBECOL*		pVtxCubeCol = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxCubeCol, 0);

	pVtxCubeCol[0].vPos		= D3DXVECTOR3(-1.f * m_tItv.m_wItvX, 1.f * m_tItv.m_wItvY, -1.f * m_tItv.m_wItvZ);
	pVtxCubeCol[0].dwColor	= dwColor;

	pVtxCubeCol[1].vPos		= D3DXVECTOR3(1.f * m_tItv.m_wItvX, 1.f * m_tItv.m_wItvY, -1.f * m_tItv.m_wItvZ);
	pVtxCubeCol[1].dwColor	= dwColor;

	pVtxCubeCol[2].vPos		= D3DXVECTOR3(1.f * m_tItv.m_wItvX, -1.f * m_tItv.m_wItvY, -1.f * m_tItv.m_wItvZ);
	pVtxCubeCol[2].dwColor	= dwColor;

	pVtxCubeCol[3].vPos		= D3DXVECTOR3(-1.f * m_tItv.m_wItvX, -1.f * m_tItv.m_wItvY, -1.f * m_tItv.m_wItvZ);
	pVtxCubeCol[3].dwColor	= dwColor;

	pVtxCubeCol[4].vPos		= D3DXVECTOR3(-1.f * m_tItv.m_wItvX, 1.f * m_tItv.m_wItvY, 1.f * m_tItv.m_wItvZ);
	pVtxCubeCol[4].dwColor	= dwColor;

	pVtxCubeCol[5].vPos		= D3DXVECTOR3(1.f * m_tItv.m_wItvX, 1.f * m_tItv.m_wItvY, 1.f * m_tItv.m_wItvZ);
	pVtxCubeCol[5].dwColor	= dwColor;

	pVtxCubeCol[6].vPos		= D3DXVECTOR3(1.f * m_tItv.m_wItvX, -1.f * m_tItv.m_wItvY, 1.f * m_tItv.m_wItvZ);
	pVtxCubeCol[6].dwColor	= dwColor;

	pVtxCubeCol[7].vPos		= D3DXVECTOR3(-1.f * m_tItv.m_wItvX, -1.f * m_tItv.m_wItvY, 1.f * m_tItv.m_wItvZ);
	pVtxCubeCol[7].dwColor	= dwColor;

	m_pVB->Unlock();

	INDEX32*		pIndex = NULL;

	m_pIB->Lock(0, 0 , (void**)&pIndex, 0);

	// x+ 
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 5;
	pIndex[0]._3 = 6;

	pIndex[1]._1 = 1;
	pIndex[1]._2 = 6;
	pIndex[1]._3 = 2;

	// x- 
	pIndex[2]._1 = 4;
	pIndex[2]._2 = 0;
	pIndex[2]._3 = 3;

	pIndex[3]._1 = 4;
	pIndex[3]._2 = 3;
	pIndex[3]._3 = 7;

	// y+ 
	pIndex[4]._1 = 4;
	pIndex[4]._2 = 5;
	pIndex[4]._3 = 1;

	pIndex[5]._1 = 4;
	pIndex[5]._2 = 1;
	pIndex[5]._3 = 0;

	// y- 
	pIndex[6]._1 = 3;
	pIndex[6]._2 = 2;
	pIndex[6]._3 = 6;

	pIndex[7]._1 = 3;
	pIndex[7]._2 = 6;
	pIndex[7]._3 = 7;

	// z+ 
	pIndex[8]._1 = 7;
	pIndex[8]._2 = 6;
	pIndex[8]._3 = 5;

	pIndex[9]._1 = 7;
	pIndex[9]._2 = 5;
	pIndex[9]._3 = 4;

	// z- 
	pIndex[10]._1 = 0;
	pIndex[10]._2 = 1;
	pIndex[10]._3 = 2;

	pIndex[11]._1 = 0;
	pIndex[11]._2 = 2;
	pIndex[11]._3 = 3;

	m_pIB->Unlock();

	return S_OK;
}

void ENGINE::CCubeCol::Render(void)
{
	CVIBuffer::Render();
}

ENGINE::CCubeCol* ENGINE::CCubeCol::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCubeCol* pCubeCol = new CCubeCol(pDevice);

	if(FAILED(pCubeCol->CreateBuffer()))
		ENGINE::Safe_Delete(pCubeCol);

	return pCubeCol;
}

ENGINE::CCubeCol* ENGINE::CCubeCol::Create(LPDIRECT3DDEVICE9 pDevice, 
								   const float& wItvX, 
								   const float& wItvY, 
								   const float& wItvZ,
								   const DWORD&	dwColor)
{
	CCubeCol* pCubeCol = new CCubeCol(pDevice);

	if(FAILED(pCubeCol->CreateBuffer(wItvX, wItvY, wItvZ, dwColor)))
		ENGINE::Safe_Delete(pCubeCol);

	return pCubeCol;
}

void ENGINE::CCubeCol::Release(void)
{

}

