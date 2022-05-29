#include "stdafx.h"
#include "Petal.h"

CPetal::CPetal(LPDIRECT3DDEVICE9 pDevice)
: CParticleObject(pDevice)
{
	m_PrtN		= 0;
	m_PrtD		= NULL;

	m_fTimeAvg	= 0;
	m_bAni		= FALSE;

	m_pVtx		= NULL;
	m_pTx		= NULL;
}

CPetal::~CPetal()
{

}
CPetal* CPetal::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPetal*		pPetal = new CPetal(pDevice);

	if(FAILED(pPetal->Initialize()))
		ENGINE::Safe_Delete(pPetal);

	return pPetal;
}
HRESULT CPetal::Initialize(void)
{
	m_PrtN	= 1000;
	m_PrtD	= new ENGINE::PARTICLE[m_PrtN];

	// 입자의 운동 출력용 설정
	m_iVtx	= m_PrtN * 2 * 3;
	m_pVtx	= new ENGINE::VTXPTC[ m_iVtx ];

	// Texture Load
	D3DXCreateTextureFromFileEx(m_pDevice
		, L"../../Texture/Particle/Petal.png"
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


	return S_OK;
}
void CPetal::Update(void)
{
	if(!m_bAni)
		return;

	INT i;

	float fWindL = 2.f;
	m_vcWind = D3DXVECTOR3(1.f, 0, 1.f) * fWindL;

	D3DXMATRIX matX;
	D3DXMATRIX matY;
	D3DXMATRIX matZ;
	D3DXMATRIX matS;
	D3DXMATRIX matW;

	D3DXVECTOR3 vcWind;
	// 1. 운동을 갱신한다.
	FLOAT	ftime = m_fTimeAvg * 0.1f;

	for(i=0; i<m_PrtN; ++i)
	{
		ENGINE::PARTICLE* pPrt = &m_PrtD[i];

		// 공기저항을 구한다.
		D3DXVECTOR3	vcAirR = pPrt->CrnV;					// 공기저항의 방향 벡터
		FLOAT		fLenV  = D3DXVec3LengthSq(&vcAirR);		// 속도의 제곱(Vx*Vx + Vy*Vy + Vz*Vz)크기 구함

		// 공기저항의 방향 벡터를 구한다.
		D3DXVec3Normalize(&vcAirR, &vcAirR);

		// 이동 속도와 반대로 설정
		vcAirR	*= -1.f;

		// 속력제곱 * 공기 저항 계수를 곱함.
		vcAirR	*= fLenV * pPrt->fDamp;


		// 바람에 의한 perturbation을 더한다.
		vcWind.x = m_vcWind.x * (10 + rand()%11)/20.f * (1+sinf(D3DXToRadian(pPrt->CrnR.x)));
		vcWind.y = m_vcWind.y * (10 + rand()%11)/20.f * (1+sinf(D3DXToRadian(pPrt->CrnR.y)));
		vcWind.z = m_vcWind.z * (10 + rand()%11)/20.f * (1+sinf(D3DXToRadian(pPrt->CrnR.z)));

		// 회전 요소를 첨가 한다.
		vcWind.x += 1.8F* sinf(D3DXToRadian(pPrt->CrnR.x));
		vcWind.y += 1.8F* sinf(D3DXToRadian(pPrt->CrnR.y));
		vcWind.z += 1.8F* sinf(D3DXToRadian(pPrt->CrnR.z));

		vcWind *=.8F;

		// 1. 가속도에 공기저항을 더한다.
		pPrt->CrnA = pPrt->IntA + vcAirR;

		// 2. 현재 속도 갱신
		pPrt->CrnV += pPrt->CrnA * ftime;

		// 3. 현재 위치 갱신
		pPrt->CrnP += pPrt->CrnV * ftime;
		pPrt->CrnP += vcWind * ftime;

		// 회전
		pPrt->CrnR +=pPrt->CrnRv * ftime;

		// 4. 죽은 파티클을 재생한다.
		if(pPrt->CrnP.y<0.f)
		{
			this->SetPart(i);
		}
	}

	// 4. 출력을 설정한다.
	// 카메라의 정보
	D3DXMATRIX mtView = *(m_pCameraMgr->GetView());
	//m_pDevice->GetTransform(D3DTS_VIEW, &mtView);

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

	//// Sorting
	/*qsort (m_PrtD
		, m_PrtN
		, sizeof(ENGINE::PARTICLE)
		, (int(*)(const void*, const void*)) ENGINE::SortFnc);
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

	//	// Rotation		
		D3DXMatrixRotationAxis(&matX, &D3DXVECTOR3(1.f,0.f,0.f),D3DXToRadian(pPrt->CrnR.x));
		D3DXMatrixRotationAxis(&matY, &D3DXVECTOR3(0.f,1.f,0.f),D3DXToRadian(pPrt->CrnR.y));
		D3DXMatrixRotationAxis(&matZ, &D3DXVECTOR3(0.f,0.f,1.f),D3DXToRadian(pPrt->CrnR.z));

	//	// Scaling
		D3DXMatrixScaling( &matS, pPrt->CrnS.x, pPrt->CrnS.y, pPrt->CrnS.z);

	//	// World matrix setup
		matW = matS * matX * matY * matZ;
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

		m_pVtx[i*6 + 4] = m_pVtx[i*6 + 2];
		m_pVtx[i*6 + 5] = m_pVtx[i*6 + 1];
	}

	SetAvgTime(0.005f / m_pTimeMgr->GetTime());
}

void CPetal::Render(void)
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

D3DXMATRIX* CPetal::GetWorldMatrix(void)
{
	return NULL;
}

D3DXMATRIX* CPetal::GetViewMatrix(void)
{
	return NULL;
}

D3DXMATRIX* CPetal::GetProjMatrix(void)
{
	return NULL;
}

LPDIRECT3DDEVICE9* CPetal::GetDevice(void)
{
	return NULL;
}

/*int CPetal::SortFnc(const ENGINE::PARTICLE* p1, const ENGINE::PARTICLE* p2)
{
	FLOAT	v1, v2;

	v1 = p1->PrsZ;
	v2 = p2->PrsZ;

	if(v1 < v2)
		return 1;

	else if(v1 == v2)
		return 0;

	else 
		return -1;
}*/

void CPetal::SetAni(BOOL bAni/*=TRUE*/)
{
	m_bAni = bAni;

	if(!m_bAni)
		return;

	for(int i=0; i<m_PrtN; ++i)
	{
		SetPart(i);
	}
}

void CPetal::SetAvgTime(FLOAT fTime)
{
	m_fTimeAvg = fTime;
}

void CPetal::SetPart(int nIdx)
{
	ENGINE::PARTICLE* pPrt = &m_PrtD[nIdx];

	// 초기 가속도
	pPrt->IntA = D3DXVECTOR3(0, -0.003F, 0);

	// 초기 속도
	pPrt->IntV.x	= rand()%10 /10.f *5.f;
	pPrt->IntV.y	=-rand()%10 /10.f *5.f;
	pPrt->IntV.z	= rand()%10 /10.f *5.f;

	pPrt->IntV	*=0.1F;

	// 초기 위치		
	pPrt->IntP.x	= 100.f - rand()%100;
	pPrt->IntP.y	=   10.f + rand()%40;
	pPrt->IntP.z	= 100.f - rand()%100;

	// 초기 회전
	pPrt->CrnR.x	= 0.f + rand()%360;
	pPrt->CrnR.y	= 0.f + rand()%360;
	pPrt->CrnR.z	= 0.f + rand()%360;

	// 회전 속도
	pPrt->CrnRv.x	=(rand()%91-45.5f) *1.2f;
	pPrt->CrnRv.y	=(rand()%91-45.5f) *1.2f;
	pPrt->CrnRv.z	=(rand()%91-45.5f) *1.2f;

	pPrt->CrnRv	*=0.1F;

	// 초기 스케일
	pPrt->CrnS.x	= ( 5 + rand()%101) /800.f;
	pPrt->CrnS.y	= (10 + rand()%101) /1600.f;
	pPrt->CrnS.z	= ( 5 + rand()%101) /800.f;

	// 탄성 계수 설정
	pPrt->fElst= 1.0f;

	// 공기저항 계수
	pPrt->fDamp= (100 + rand()%101)*0.00001F;


	// 초기 위치, 속도, 가속도를 현재의 값들의 초기 값으로 설정
	pPrt->CrnP = pPrt->IntP;
	pPrt->CrnV = pPrt->IntV;
	pPrt->CrnA = pPrt->IntA;

	// 입자의 생명 요소
	pPrt->bLive	= TRUE;
	pPrt->fLife	= 30.f + rand()%71;
	pPrt->fLife	= 30.f + rand()%71;
	pPrt->fLife	*=0.01f;

	pPrt->fFade	=( 100 + rand()%101  ) *0.0001f;

	D3DXCOLOR	dColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	/*dColor.r += (rand()%16)/15.f;
	dColor.g += (rand()%16)/15.f;
	dColor.b += (rand()%16)/15.f;*/
	pPrt->dColor =	dColor;



	// 입자의 표현 요소
	pPrt->PrsW = 50.f + rand()%101;
	pPrt->PrsW *= 0.05f;

	pPrt->PrsH = 50.f + rand()%101;
	pPrt->PrsH *= 0.1f;

	//	pPrt->PrsW = 1.6f;
	//	pPrt->PrsH = 3.2f;
}

void CPetal::Release(void)
{

}
