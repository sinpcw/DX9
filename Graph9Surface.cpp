#include"owlEngine9.hpp"

//***********************************************************************************************************************************************************************
//
//		Class Constructor / Destructor
//
//***********************************************************************************************************************************************************************
//		SURFACE9::SURFACE9()
//***********************************************************************************************************************************************************************
owl::engine::SURFACE9::SURFACE9(){
	pTexture	= NULL;
	pSurface	= NULL;
	pZBuffer	= NULL;
	vXY			= core::VECT2f(0);
	vUV			= core::VECT2f(0);
}

//***********************************************************************************************************************************************************************
//		SURFACE9::~SURFACE9()
//***********************************************************************************************************************************************************************
owl::engine::SURFACE9::~SURFACE9(){
	SafeRelease(pZBuffer);
	SafeRelease(pSurface);
	SafeRelease(pTexture);
}

//***********************************************************************************************************************************************************************
//
//		GRAPH9 Function
//
//***********************************************************************************************************************************************************************
//		GRAPH9::InitSurface(SURFACE9 &)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::InitSurface(SURFACE9 &_Surface, const owl::core::VECT2i &_Size){
	owl::core::VECT2i	vSize;
	if(_Size.x <= 0 || _Size.y <= 0){
		vSize.x	= cD3Dpps.BackBufferWidth;
		vSize.y	= cD3Dpps.BackBufferHeight;
	}else{
		vSize	= _Size;
	}
	UINT	N	= core::MinSquared(std::max<unsigned int>(vSize.x, vSize.y));
	if(FAILED(pDevice->CreateTexture(N, N, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &_Surface.pTexture, NULL))){
		return (false);
	}
	if(FAILED(_Surface.pTexture->GetSurfaceLevel(0, &_Surface.pSurface))){
		SafeRelease(_Surface.pTexture);
		return (false);
	}
	if(FAILED(pDevice->CreateDepthStencilSurface(N, N, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE, &_Surface.pZBuffer, NULL))){
		SafeRelease(_Surface.pSurface);
		SafeRelease(_Surface.pTexture);
		return (false);
	}
	if(FAILED(pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pSurface))){
		return (false);
	}
	if(FAILED(pDevice->GetDepthStencilSurface(&pZBuffer))){
		return (false);
	}
	if(FAILED(pDevice->SetRenderTarget(0, _Surface.pSurface)) || FAILED(pDevice->SetDepthStencilSurface(_Surface.pZBuffer))){
		SafeRelease(_Surface.pZBuffer);
		SafeRelease(_Surface.pSurface);
		SafeRelease(_Surface.pTexture);
		return (false);
	}
	pDevice->BeginScene();
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	pDevice->EndScene();
	pDevice->SetRenderTarget(0, pSurface);
	pDevice->SetDepthStencilSurface(pZBuffer);
	SafeRelease(pSurface);
	SafeRelease(pZBuffer);
	_Surface.vXY.x	= static_cast<float>(vSize.x);
	_Surface.vXY.y	= static_cast<float>(vSize.y);
	_Surface.vUV.x	= _Surface.vXY.x / static_cast<float>(N);
	_Surface.vUV.y	= _Surface.vXY.y / static_cast<float>(N);
	//-----	エントリ
	try{
		cListSurface.push_back(&_Surface);
	}catch(...){
		QuitSurface(_Surface);
		return (false);
	}
	return (true);
}

//***********************************************************************************************************************************************************************
//		GRAPH9::QuitSurface(SURFACE9 &)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::QuitSurface(SURFACE9 &_Surface){
	//-----	エントリから消去
	for(LISTSURFACEITERATOR p = cListSurface.begin(); p != cListSurface.end(); p++){
		if((*p) == &(_Surface)){
			(*p)->pTexture	= NULL;
			(*p)->pSurface	= NULL;
			(*p)->pZBuffer	= NULL;
			cListSurface.erase(p);
			break;
		}
	}
	//-----	データ消去
	SafeRelease(_Surface.pZBuffer);
	SafeRelease(_Surface.pSurface);
	SafeRelease(_Surface.pTexture);
	return (true);
}

//***********************************************************************************************************************************************************************
//		GRAPH9::FillSurface(SURFACE9 &, const core::VECT3f &, const core::COLORf &)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::FillSurface(SURFACE9 &_Surface, const core::VECT3f &_Point, const core::COLORf &_Color){
	TDVERTEX2D	nVertex[]	= {
		{ D3DXVECTOR4(_Point.x + static_cast<float>(vDestport.X                  ), _Point.y + static_cast<float>(vDestport.Y + vDestport.Height), _Point.z, 1.0f), TrueColor32(_Color), D3DXVECTOR2(          0.0f, _Surface.vUV.y) },
		{ D3DXVECTOR4(_Point.x + static_cast<float>(vDestport.X                  ), _Point.y + static_cast<float>(vDestport.Y                   ), _Point.z, 1.0f), TrueColor32(_Color), D3DXVECTOR2(          0.0f,           0.0f) },
		{ D3DXVECTOR4(_Point.x + static_cast<float>(vDestport.X + vDestport.Width), _Point.y + static_cast<float>(vDestport.Y + vDestport.Height), _Point.z, 1.0f), TrueColor32(_Color), D3DXVECTOR2(_Surface.vUV.x, _Surface.vUV.y) },
		{ D3DXVECTOR4(_Point.x + static_cast<float>(vDestport.X + vDestport.Width), _Point.y + static_cast<float>(vDestport.Y                   ), _Point.z, 1.0f), TrueColor32(_Color), D3DXVECTOR2(_Surface.vUV.x,           0.0f) }
	};
	pDevice->SetFVF(FVF_TDVERTEX2D);
	pDevice->SetTexture(0, _Surface.pTexture);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, nVertex, sizeof(TDVERTEX2D));
	pDevice->SetTexture(0, NULL);
	return (true);
}

//***********************************************************************************************************************************************************************
//		GRAPH9::DrawSurface(SURFACE9 &, const core::VECT3f &, const core::COLORf &)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::DrawSurface(SURFACE9 &_Surface, const core::VECT3f &_Point, const core::COLORf &_Color){
	TDVERTEX2D	nVertex[]	= {
		{ D3DXVECTOR4(_Point.x                 , _Point.y + _Surface.vXY.y, _Point.z, 1.0f), TrueColor32(_Color), D3DXVECTOR2(          0.0f, _Surface.vUV.y) },
		{ D3DXVECTOR4(_Point.x                 , _Point.y                 , _Point.z, 1.0f), TrueColor32(_Color), D3DXVECTOR2(          0.0f,           0.0f) },
		{ D3DXVECTOR4(_Point.x + _Surface.vXY.x, _Point.y + _Surface.vXY.y, _Point.z, 1.0f), TrueColor32(_Color), D3DXVECTOR2(_Surface.vUV.x, _Surface.vUV.y) },
		{ D3DXVECTOR4(_Point.x + _Surface.vXY.x, _Point.y                 , _Point.z, 1.0f), TrueColor32(_Color), D3DXVECTOR2(_Surface.vUV.x,           0.0f) }
	};
	pDevice->SetFVF(FVF_TDVERTEX2D);
	pDevice->SetTexture(0, _Surface.pTexture);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, nVertex, sizeof(TDVERTEX2D));
	pDevice->SetTexture(0, NULL);
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::DrawSurface(SURFACE9 &, const core::VECT3f &, core::COLORf, float)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::DrawSurface(SURFACE9 &_Surface, const core::VECT3f &_Point, const core::COLORf &_Color, float _Rotate){
	float		fRz		= 0.5f * _Surface.vXY.Abs();
	float		fTh		= _Surface.vXY.Rot();
	TDVERTEX2D	nVertex[]	= {
		{ D3DXVECTOR4(_Point.x + fRz * cos(core::PI - fTh - _Rotate), _Point.y + fRz * sin(core::PI - fTh - _Rotate), _Point.z, 1.0f), TrueColor32(_Color), D3DXVECTOR2(          0.0f, _Surface.vUV.y) },
		{ D3DXVECTOR4(_Point.x + fRz * cos(core::PI + fTh - _Rotate), _Point.y + fRz * sin(core::PI + fTh - _Rotate), _Point.z, 1.0f), TrueColor32(_Color), D3DXVECTOR2(          0.0f,           0.0f) },
		{ D3DXVECTOR4(_Point.x + fRz * cos(         + fTh - _Rotate), _Point.y + fRz * sin(         + fTh - _Rotate), _Point.z, 1.0f), TrueColor32(_Color), D3DXVECTOR2(_Surface.vUV.x, _Surface.vUV.y) },
		{ D3DXVECTOR4(_Point.x + fRz * cos(         - fTh - _Rotate), _Point.y + fRz * sin(         - fTh - _Rotate), _Point.z, 1.0f), TrueColor32(_Color), D3DXVECTOR2(_Surface.vUV.x,           0.0f) }
	};
	pDevice->SetFVF(FVF_TDVERTEX2D);
	pDevice->SetTexture(0, _Surface.pTexture);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, nVertex, sizeof(TDVERTEX2D));
	pDevice->SetTexture(0, NULL);
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::DrawSurface(SURFACE9 &, const core::VECT3f &, core::COLORf, float)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::DrawSurface(SURFACE9 &_Surface, const core::VECT3f &_Point, const core::COLORf &_Color, float _Rotate, float _Extend){
	float		fRz		= 0.5f * _Extend * _Surface.vXY.Abs();
	float		fTh		= _Surface.vXY.Rot();
	TDVERTEX2D	nVertex[]	= {
		{ D3DXVECTOR4(_Point.x + fRz * cos(core::PI - fTh - _Rotate), _Point.y + fRz * sin(core::PI - fTh - _Rotate), _Point.z, 1.0f), TrueColor32(_Color), D3DXVECTOR2(          0.0f, _Surface.vUV.y) },
		{ D3DXVECTOR4(_Point.x + fRz * cos(core::PI + fTh - _Rotate), _Point.y + fRz * sin(core::PI + fTh - _Rotate), _Point.z, 1.0f), TrueColor32(_Color), D3DXVECTOR2(          0.0f,           0.0f) },
		{ D3DXVECTOR4(_Point.x + fRz * cos(         + fTh - _Rotate), _Point.y + fRz * sin(         + fTh - _Rotate), _Point.z, 1.0f), TrueColor32(_Color), D3DXVECTOR2(_Surface.vUV.x, _Surface.vUV.y) },
		{ D3DXVECTOR4(_Point.x + fRz * cos(         - fTh - _Rotate), _Point.y + fRz * sin(         - fTh - _Rotate), _Point.z, 1.0f), TrueColor32(_Color), D3DXVECTOR2(_Surface.vUV.x,           0.0f) }
	};
	pDevice->SetFVF(FVF_TDVERTEX2D);
	pDevice->SetTexture(0, _Surface.pTexture);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, nVertex, sizeof(TDVERTEX2D));
	pDevice->SetTexture(0, NULL);
	return (true);
}

//***********************************************************************************************************************************************************************
//		GRAPH9::SetSection(SURFACE9 &)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::SetSection(SURFACE9 &_Surface, const owl::core::COLORf _Color){
	UINT	N	= core::MinSquared(static_cast<int>(std::max<float>(_Surface.vXY.x, _Surface.vXY.y)));
	if(FAILED(pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pSurface))){
		return (false);
	}
	if(FAILED(pDevice->SetRenderTarget(0, _Surface.pSurface))){
		return (false);
	}
	if(FAILED(pDevice->GetDepthStencilSurface(&pZBuffer))){
		return (false);
	}
	if(FAILED(pDevice->SetDepthStencilSurface(_Surface.pZBuffer))){
		return (false);
	}
	if(FAILED(pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, TrueColor32(_Color), 1.0f, 0))){
		return (false);
	}
	SetMatrixOrth(1.0f, 10000.0f, owl::core::VECT2i(static_cast<int>(N), static_cast<int>(N)));
	return (true);
}

//***********************************************************************************************************************************************************************
//		GRAPH9::EndSection(SURFACE9 &)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::EndSection(SURFACE9 &_Surface){
	if(FAILED(pDevice->SetRenderTarget(0, pSurface))){
		return (false);
	}
	if(FAILED(pDevice->SetDepthStencilSurface(pZBuffer))){
		return (false);
	}
	SafeRelease(pSurface);
	SafeRelease(pZBuffer);
	return (true);
}

//***********************************************************************************************************************************************************************
//		GRAPH9::InitResource(SURFACE9 **)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::InitResource(SURFACE9 **_Surface){
	UINT	N	= core::MinSquared(std::max<unsigned int>(cD3Dpps.BackBufferWidth, cD3Dpps.BackBufferHeight));
	if(FAILED(pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pSurface))){
		return (false);
	}
	if(FAILED(pDevice->GetDepthStencilSurface(&pZBuffer))){
		return (false);
	}
	if(FAILED(pDevice->CreateTexture(N, N, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &(*_Surface)->pTexture, NULL))){
		return (false);
	}
	if(FAILED(pDevice->CreateDepthStencilSurface(N, N, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE, &(*_Surface)->pZBuffer, NULL))){
		SafeRelease((*_Surface)->pTexture);
		return (false);
	}
	if(FAILED(pDevice->SetRenderTarget(0, (*_Surface)->pSurface)) || FAILED(pDevice->SetDepthStencilSurface((*_Surface)->pZBuffer))){
		SafeRelease((*_Surface)->pZBuffer);
		SafeRelease((*_Surface)->pSurface);	
		SafeRelease((*_Surface)->pTexture);
		return (false);
	}
	pDevice->BeginScene();
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	pDevice->EndScene();
	pDevice->SetRenderTarget(0, pSurface);
	pDevice->SetDepthStencilSurface(pZBuffer);
	SafeRelease(pSurface);
	SafeRelease(pZBuffer);
	(*_Surface)->vXY.x	= static_cast<float>(cD3Dpps.BackBufferWidth );
	(*_Surface)->vXY.y	= static_cast<float>(cD3Dpps.BackBufferHeight);
	(*_Surface)->vUV.x	= (*_Surface)->vXY.x / static_cast<float>(N);
	(*_Surface)->vUV.y	= (*_Surface)->vXY.y / static_cast<float>(N);
	return true;
}

//***********************************************************************************************************************************************************************
//		GRAPH9::FreeResource(SURFACE9 **)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::FreeResource(SURFACE9 **_Surface){
	SafeRelease((*_Surface)->pZBuffer);
	SafeRelease((*_Surface)->pSurface);	
	SafeRelease((*_Surface)->pTexture);
	return true;
}
