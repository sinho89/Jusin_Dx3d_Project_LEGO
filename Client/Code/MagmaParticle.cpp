#include "stdafx.h"
#include "MagmaParticle.h"

CMagmaParticle::CMagmaParticle(LPDIRECT3DDEVICE9 pDevice)
:CParticleObject(pDevice)
{

}

CMagmaParticle::~CMagmaParticle()
{

}

CMagmaParticle* CMagmaParticle::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMagmaParticle*		pSkillParticle = new CMagmaParticle(pDevice);

	if(FAILED(pSkillParticle->Initialize()))
		ENGINE::Safe_Delete(pSkillParticle);

	return pSkillParticle;
}

HRESULT CMagmaParticle::Initialize(void)
{
	m_PrtN	= 10;
	m_PrtD	= new ENGINE::PARTICLE[m_PrtN];

	m_fBrightness = 0.6f;
	m_fMagmaY = 0.f;
	// 입자의 운동 출력용 설정
	m_iVtx	= m_PrtN * 2 * 3;
	m_pVtx	= new ENGINE::VTXPTC[ m_iVtx ];

	

	// Texture Load
	D3DXCreateTextureFromFileEx(m_pDevice
		, L"../../Texture/Particle/cycle.png"
		, D3DX_DEFAULT
		, D3DX_DEFAULT
		, D3DX_DEFAULT
		, 0
		, D3DFMT_UNKNOWN
		, D3DPOOL_MANAGED
		, (D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR)
		, (D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR)
		, 0x00FFFFFF
		, NULL
		, NULL
		, &m_pTx
		);

	m_dTimeCur	= timeGetTime();
	m_dTimeInt	= 60;

	return S_OK;
}

void CMagmaParticle::Update(void)
{
	if(!m_bAni)
		return;

	if(m_fBrightness >= 1.0f)
	{
		m_bBrightCheck = true;
	}
	else if(m_fBrightness <= 0.6f)
	{
		m_bBrightCheck = false;

	}

	if(m_bBrightCheck == false)
	{
		m_fMagmaY += 0.1f;
		m_fBrightness += 0.002f;
	}
	else if(m_bBrightCheck == true)
	{
		m_fMagmaY -= 0.1f;
		m_fBrightness -= 0.002f;
	}
	int		i;

	// 1. 운동을 갱신한다.
	FLOAT	ftime = m_fTimeAvg * 0.1f;

	D3DXMATRIX matX;
	D3DXMATRIX matY;
	D3DXMATRIX matZ;
	D3DXMATRIX matS;
	D3DXMATRIX matW;

	

	for(i=0; i<m_PrtN; ++i)
	{
		ENGINE::PARTICLE* pPrt = &m_PrtD[i];

		const ENGINE::CComponent* m_pPlayer1Info =	m_pLayerMgr->GetComponent(L"Player_Elond", L"Transform", ENGINE::LAYER_PLAYER);

		pPrt->CrnP.x = ((ENGINE::CTransform*)m_pPlayer1Info)->m_vPos.x;
		pPrt->CrnP.y = m_fMagmaY - 30;
		pPrt->CrnP.z = ((ENGINE::CTransform*)m_pPlayer1Info)->m_vPos.z;

		// 공기저항을 구한다.
		D3DXVECTOR3	vcAirR = pPrt->CrnV;					// 공기저항의 방향 벡터
		FLOAT		fLenV  = D3DXVec3LengthSq(&vcAirR);		// 속도의 제곱(Vx*Vx + Vy*Vy + Vz*Vz)크기 구함

		// 공기저항의 방향 벡터를 구한다.
		D3DXVec3Normalize(&vcAirR, &vcAirR);

		// 이동 속도와 반대로 설정
		vcAirR	*= -1.F;

		// 속력제곱 * 공기 저항 계수를 곱함.
		vcAirR	*= fLenV * pPrt->fDamp;


		// 1. 가속도에 공기저항을 더한다.
		pPrt->CrnA = pPrt->IntA + vcAirR;

		// 2. 현재 속도 갱신
		pPrt->CrnV += pPrt->CrnA * ftime;

		// 3. 현재 위치 갱신
		pPrt->CrnP += pPrt->CrnV * ftime;

		// 4. 경계값 설정. 벗어나면 죽은 상태로 설정.
		if(pPrt->CrnP.y<0.f)
		{
			//pPrt->bLive	= FALSE;
		}
		pPrt->dColor	=	D3DXCOLOR(0.2f, 0.f, 0.f, m_fBrightness);


		pPrt->PrsW = 100000.f;
		pPrt->PrsH = 100000.f;
	}


	// 2. 파티클의 생명을 갱신한다.
	for(i=0; i<m_PrtN; ++i)
	{
		/*ENGINE::PARTICLE* pPrt = &m_PrtD[i];

		if(FALSE == pPrt->bLive)
			continue;

		D3DXCOLOR	xc = pPrt->dColor;

		pPrt->fLife -=pPrt->fFade*ftime*100;

		if(pPrt->fLife<=0.f)
		{
			//pPrt->bLive	= FALSE;
			continue;
		}

		xc.a	= pPrt->fLife;
		pPrt->dColor	= xc;*/
	}


	// 3. 텍스처 애니메이션에 대한 파티클을 갱신한다.
	DWORD	dTimeCur	= timeGetTime();

	if(dTimeCur>=(m_dTimeCur+m_dTimeInt))
	{
		/*m_dTimeCur = dTimeCur;

		for(i=0; i<m_PrtN; ++i)
		{
			ENGINE::PARTICLE* pPrt = &m_PrtD[i];

			if(FALSE == pPrt->bLive)
				continue;

			++pPrt->m_PrsImg;
			pPrt->m_PrsImg %=22;
		}*/
	}



	// 3. 죽은 파티클을 재생한다.
	for(i=0; i<m_PrtN; ++i)
	{
		/*ENGINE::PARTICLE* pPrt = &m_PrtD[i];

		if(TRUE == pPrt->bLive)
			continue;*/

		//this->SetPart(i);
	}





	// 4. 출력을 설정한다.

	// 카메라의 정보
	D3DXMATRIX mtView = *(m_pCameraMgr->GetView());

	D3DXVECTOR3 vcCamX(mtView._11, mtView._21, mtView._31);
	D3DXVECTOR3 vcCamY(mtView._12, mtView._22, mtView._32);
	D3DXVECTOR3 vcCamZ(mtView._13, mtView._23, mtView._33);

	for(i=0; i<m_PrtN; ++i)
	{	
		ENGINE::PARTICLE* pPrt = &m_PrtD[i];

		D3DXVECTOR3	vcP	= pPrt->CrnP;

		// 카메라의 Z축과 파티클의 위치와 내적
		pPrt->PrsZ	= D3DXVec3Dot(&vcP, &vcCamZ);
	}

	// Sorting
	/*qsort (m_PrtD
		, m_PrtN
		, sizeof(ENGINE::PARTICLE)
		, (int(*) (const void *, const void *)) CMcParticle::SortFnc);
	*/

	for(i=0; i<m_PrtN; ++i)
	{	
		ENGINE::PARTICLE* pPrt = &m_PrtD[i];


		D3DXVECTOR3	vcP	= pPrt->CrnP;
		D3DXCOLOR	xcC	= pPrt->dColor;

		FLOAT		fW = pPrt->PrsW;
		FLOAT		fH = pPrt->PrsH;
		FLOAT		fD = min(fW, fH);

		ENGINE::VTXPTC* pVtx = &m_pVtx[i*6 + 0];

		int			nIdxX	= pPrt->m_PrsImg %4;
		int			nIdxY	= pPrt->m_PrsImg /8;
		D3DXVECTOR2	uv00( (nIdxX+0)/4.f, (nIdxY+0)/8.f);
		D3DXVECTOR2	uv11( (nIdxX+1)/4.f, (nIdxY+1)/8.f);

		//	// Rotation		
		D3DXMatrixRotationAxis(&matX, &D3DXVECTOR3(1.f,0.f,0.f),D3DXToRadian(pPrt->CrnR.x));
		D3DXMatrixRotationAxis(&matY, &D3DXVECTOR3(0.f,1.f,0.f),D3DXToRadian(pPrt->CrnR.y));
		D3DXMatrixRotationAxis(&matZ, &D3DXVECTOR3(0.f,0.f,1.f),D3DXToRadian(pPrt->CrnR.z));

		//	// Scaling
		D3DXMatrixScaling( &matS, pPrt->CrnS.x, pPrt->CrnS.y, pPrt->CrnS.z);

		//	// World matrix setup
		matW = matS; * matX * matY * matZ;
		matW._41 =pPrt->CrnP.x;
		matW._42 =pPrt->CrnP.y;
		matW._43 =pPrt->CrnP.z;

		//	// vertex setup
		m_pVtx[i*6 + 0] = ENGINE::VTXPTC( -fW, +fH, 0, 0, 0, xcC);
		m_pVtx[i*6 + 1] = ENGINE::VTXPTC( +fW, +fH, 0, 1, 0, xcC);
		m_pVtx[i*6 + 2] = ENGINE::VTXPTC( -fW, -fH, 0, 0, 1, xcC);
		m_pVtx[i*6 + 3] = ENGINE::VTXPTC( +fW, -fH, 0, 1, 1, xcC);

		D3DXVec3TransformCoord( &(pVtx+0)->vPos, &(pVtx+0)->vPos, &matW);
		D3DXVec3TransformCoord( &(pVtx+1)->vPos, &(pVtx+1)->vPos, &matW);
		D3DXVec3TransformCoord( &(pVtx+2)->vPos, &(pVtx+2)->vPos, &matW);
		D3DXVec3TransformCoord( &(pVtx+3)->vPos, &(pVtx+3)->vPos, &matW);

		(pVtx+0)->vPos.x	= vcP.x - (vcCamX.x - vcCamY.x) * fW;
		(pVtx+0)->vPos.y	= vcP.y /*- (vcCamX.y - vcCamY.y) * fH*/;
		(pVtx+0)->vPos.z	= vcP.z - (vcCamX.z - vcCamY.z) * fD;
		(pVtx+0)->fU		= uv00.x;
		(pVtx+0)->fV		= uv00.y;
		(pVtx+0)->dwColor	= xcC;

		(pVtx+1)->vPos.x	= vcP.x + (vcCamX.x + vcCamY.x) * fW;
		(pVtx+1)->vPos.y	= vcP.y /*+ (vcCamX.y + vcCamY.y) * fH */;
		(pVtx+1)->vPos.z	= vcP.z + (vcCamX.z + vcCamY.z) * fD;
		(pVtx+1)->fU		= uv11.x;
		(pVtx+1)->fV		= uv00.y;
		(pVtx+1)->dwColor	= xcC;

		(pVtx+2)->vPos.x	= vcP.x - (vcCamX.x + vcCamY.x) * fW;
		(pVtx+2)->vPos.y	= vcP.y /*- (vcCamX.y + vcCamY.y) * fH */;
		(pVtx+2)->vPos.z	= vcP.z - (vcCamX.z + vcCamY.z) * fD;
		(pVtx+2)->fU		= uv00.x;
		(pVtx+2)->fV		= uv11.y;
		(pVtx+2)->dwColor	= xcC;

		(pVtx+3)->vPos.x	= vcP.x + (vcCamX.x - vcCamY.x) * fW;
		(pVtx+3)->vPos.y	= vcP.y /*+ (vcCamX.y - vcCamY.y) * fH*/;
		(pVtx+3)->vPos.z	= vcP.z + (vcCamX.z - vcCamY.z) * fD;
		(pVtx+3)->fU		= uv11.x;
		(pVtx+3)->fV		= uv11.y;
		(pVtx+3)->dwColor	= xcC;

		m_pVtx[i*6 + 4] = m_pVtx[i*6 + 2];
		m_pVtx[i*6 + 5] = m_pVtx[i*6 + 1];
	}

	SetAvgTime(0.05f / m_pTimeMgr->GetTime());

}

void CMagmaParticle::Render(void)
{
	if(!m_bAni)
		return;


	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	m_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pDevice->SetTexture(0, m_pTx);
	m_pDevice->SetFVF(ENGINE::VTXPTC::FVF);

	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_PrtN * 2, m_pVtx, sizeof(ENGINE::VTXPTC));

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	m_pDevice->SetTexture(0, NULL);

	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

}

D3DXMATRIX* CMagmaParticle::GetWorldMatrix(void)
{
	return NULL;
}

D3DXMATRIX* CMagmaParticle::GetViewMatrix(void)
{
	return NULL;
}

D3DXMATRIX* CMagmaParticle::GetProjMatrix(void)
{
	return NULL;
}

LPDIRECT3DDEVICE9* CMagmaParticle::GetDevice(void)
{
	return NULL;
}

void CMagmaParticle::SetAni(BOOL bAni/*=TRUE*/)
{
	m_bAni = bAni;

	if(!m_bAni)
		return;

	for(int i=0; i<m_PrtN; ++i)
	{
		SetPart(i);
	}
}

void CMagmaParticle::SetAvgTime(FLOAT fTime)
{
	m_fTimeAvg = fTime;
}

void CMagmaParticle::Release(void)
{

}

void CMagmaParticle::SetPart(int nIdx)
{
	ENGINE::PARTICLE* pPrt = &m_PrtD[nIdx];

	FLOAT	fTheta;		// 각도 θ
	FLOAT	fPhi;		// 각도 φ
	FLOAT	fSpdR;		// 속도 크기

	// 초기 가속도
	pPrt->IntA = D3DXVECTOR3(0, -0.2F,0);

	//초기 속도와 위치를 설정하기 위한 변수
	fTheta	= float(rand()%61);
	fTheta	-=30.f;

	fPhi	= float(rand()%360);

	fSpdR = 100.f + rand()%101;
	fSpdR *=0.1f;

	// 라디안으로 변경
	fTheta	= D3DXToRadian(fTheta);
	fPhi	= D3DXToRadian(fPhi);

	// 초기 속도
	pPrt->IntV.x = fSpdR * sinf(fTheta) * sinf(fPhi);
	pPrt->IntV.y = fSpdR * cosf(fTheta);
	pPrt->IntV.z = fSpdR * sinf(fTheta) * cosf(fPhi);

	// 초기 위치		
	pPrt->IntP.x = 50.f;
	pPrt->IntP.y = 0.f;
	pPrt->IntP.z = 50.f;

	pPrt->CrnS.x	= ( 5 + rand()%101) /800000.f;
	pPrt->CrnS.y	= (10 + rand()%101) /1600000.f;
	pPrt->CrnS.z	= ( 5 + rand()%101) /800000.f;
	// 탄성 계수 설정
	pPrt->fElst= (50 + rand()%51)*0.01f;

	// 공기저항 계수
	pPrt->fDamp= (100 + rand()%101)*0.000005F;

	// 초기 위치, 속도, 가속도를 현재의 값들의 초기 값으로 설정
	pPrt->CrnP = pPrt->IntP;
	pPrt->CrnV = pPrt->IntV;
	pPrt->CrnA = pPrt->IntA;

	// 입자의 생명 요소
	pPrt->bLive	= TRUE;
	pPrt->fLife	= 10000;//30.f + rand()%71;
	pPrt->fLife	= 10000;//30.f + rand()%71;


	pPrt->fFade	=( 100 + rand()%101  ) *0.0001f;


	// 입자의 표현 요소
	pPrt->PrsW = 50.f + rand()%101;
	pPrt->PrsW *= 0.03f;

	pPrt->PrsH = 50.f + rand()%101;
	pPrt->PrsH *= 0.03f;

	pPrt->PrsW = 100000.f;
	pPrt->PrsH = 100000.f;

	pPrt->m_PrsImg	= rand()%22;


}
