#include"owlEngine9.hpp"

//***********************************************************************************************************************************************************************
//
//		Class Constructor / Destructor
//
//***********************************************************************************************************************************************************************
//		TEXTURE9::TEXTURE9()
//***********************************************************************************************************************************************************************
owl::engine::TEXTURE9::TEXTURE9(){
	pTexture	= NULL;
	vXY			= core::VECT2f(0);
	vUV			= core::VECT2f(0);
}

//***********************************************************************************************************************************************************************
//		TEXTURE9::~TEXTURE9()
//***********************************************************************************************************************************************************************
owl::engine::TEXTURE9::~TEXTURE9(){
	SafeRelease(pTexture);
	vXY			= core::VECT2f(0);
	vUV			= core::VECT2f(0);
}

//***********************************************************************************************************************************************************************
//
//		GUI Function
//
//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::InitTexture(TEXTURE9 &, const core::COLORf &, const wchar_t *)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::InitTexture(TEXTURE9 &_Tex, const core::COLORf &_Color, const wchar_t *_FileName){
	D3DXIMAGE_INFO		Info;
	D3DSURFACE_DESC		Desc;
	if(_Tex.pTexture != NULL){
		return (true);
	}
	if(FAILED(D3DXGetImageInfoFromFileW(_FileName, &Info))){
		return (false);
	}
	if(FAILED(D3DXCreateTextureFromFileEx(pDevice, _FileName, D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, TrueColor32(_Color), NULL, NULL, &_Tex.pTexture))){
		return (false);
	}
	_Tex.pTexture->GetLevelDesc(0, &Desc);
	_Tex.vXY.x		= static_cast<float>(Info.Width );
	_Tex.vXY.y		= static_cast<float>(Info.Height);
	_Tex.vUV.x		= static_cast<float>(Info.Width ) / static_cast<float>(Desc.Width );
	_Tex.vUV.y		= static_cast<float>(Info.Height) / static_cast<float>(Desc.Height);
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::InitTexture(TEXTURE9 &, core::COLORf, const unsigned char *, unsigned int)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::InitTexture(TEXTURE9 &_Tex, const core::COLORf &_Color, const unsigned char *_Stream, unsigned int _Count){
	D3DXIMAGE_INFO		Info;
	D3DSURFACE_DESC		Desc;
	if(_Tex.pTexture != NULL){
		return (true);
	}
	if(FAILED(D3DXGetImageInfoFromFileInMemory(_Stream, _Count, &Info))){
		return (false);
	}
	if(FAILED(D3DXCreateTextureFromFileInMemoryEx(pDevice, _Stream, _Count, D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, TrueColor32(_Color), NULL, NULL, &_Tex.pTexture))){
		return (false);
	}
	_Tex.pTexture->GetLevelDesc(0, &Desc);
	_Tex.vXY.x		= static_cast<float>(Info.Width );
	_Tex.vXY.y		= static_cast<float>(Info.Height);
	_Tex.vUV.x		= static_cast<float>(Info.Width ) / static_cast<float>(Desc.Width );
	_Tex.vUV.y		= static_cast<float>(Info.Height) / static_cast<float>(Desc.Height);
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::InitTexture(TEXTURE9 &, core::COLORf, HMODULE, const wchar_t *)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::InitTexture(TEXTURE9 &_Tex, const core::COLORf &_Color, HMODULE _Module, const wchar_t *_Resource){
	D3DXIMAGE_INFO		Info;
	D3DSURFACE_DESC		Desc;
	if(_Tex.pTexture != NULL){
		return (true);
	}
	if(FAILED(D3DXGetImageInfoFromResourceW(_Module, _Resource, &Info))){
		return (false);
	}
	if(FAILED(D3DXCreateTextureFromResourceExW(pDevice, _Module, _Resource, D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, TrueColor32(_Color), NULL, NULL, &_Tex.pTexture))){
		return (false);
	}
	_Tex.pTexture->GetLevelDesc(0, &Desc);
	_Tex.vXY.x		= static_cast<float>(Info.Width );
	_Tex.vXY.y		= static_cast<float>(Info.Height);
	_Tex.vUV.x		= static_cast<float>(Info.Width ) / static_cast<float>(Desc.Width );
	_Tex.vUV.y		= static_cast<float>(Info.Height) / static_cast<float>(Desc.Height);
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::InitTexture()
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::InitTexture(TEXTURE9 &_Tex, const core::COLORf &_Color, const core::VECT2i &_Size){
	unsigned int	_S	= core::MinSquared(std::max<unsigned int>(_Size.x, _Size.y));
	if(_Tex.pTexture != NULL){
		return (true);
	}
	if(FAILED(D3DXCreateTexture(pDevice, _S, _S, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &_Tex.pTexture))){
		return (false);
	}
	RECT			_R	= { 0, 0, _S, _S };
	D3DLOCKED_RECT	_L;
	if(FAILED(_Tex.pTexture->LockRect(0, &_L, &_R, D3DLOCK_DISCARD))){
		QuitTexture(_Tex);
		return (false);
	}
	for(UINT uY = 0; uY < _S; uY++){
		for(UINT uX = 0; uX < _S; uX++){
			static_cast<unsigned int *>(_L.pBits)[uX + uY * (_L.Pitch >> 2)]	= TrueColor32(_Color);
		}
	}
	if(FAILED(_Tex.pTexture->UnlockRect(0))){
		QuitTexture(_Tex);
		return (false);
	}
	_Tex.vXY.x		= static_cast<float>(_Size.x);
	_Tex.vXY.y		= static_cast<float>(_Size.y);
	_Tex.vUV.x		= static_cast<float>(_Size.x) / static_cast<float>(_S);
	_Tex.vUV.y		= static_cast<float>(_Size.y) / static_cast<float>(_S);
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::CopyTexture()
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::CopyTexture(TEXTURE9 &_Tex, TEXTURE9 &_Src){
	if(_Tex.pTexture != NULL){
		return (true);
	}
	if(_Src.pTexture == NULL){
		return (false);
	}
	_Tex		= _Src;
	_Tex.pTexture->AddRef();
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::QuitTexture()
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::QuitTexture(TEXTURE9 &_Tex){
	SafeRelease(_Tex.pTexture);
	_Tex.vXY	= core::VECT2f(0);
	_Tex.vUV	= core::VECT2f(0);
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::FillTexture()
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::FillTexture(TEXTURE9 &_Tex, const core::VECT3f &_Point, const core::COLORf &_Color){
	pDevice->SetFVF(FVF_TDVERTEX2D);
	pDevice->SetTexture(0, _Tex.pTexture);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	TDVERTEX2D	nVertex[]	= {
		{ D3DXVECTOR4(static_cast<float>(vDestport.X)                   + _Point.x, static_cast<float>(vDestport.Y + vDestport.Height) + _Point.y, _Point.z, 1.0f), TrueColor32(_Color), D3DXVECTOR2(      0.0f, _Tex.vUV.y) },
		{ D3DXVECTOR4(static_cast<float>(vDestport.X)                   + _Point.x, static_cast<float>(vDestport.Y                   ) + _Point.y, _Point.z, 1.0f), TrueColor32(_Color), D3DXVECTOR2(      0.0f,       0.0f) },
		{ D3DXVECTOR4(static_cast<float>(vDestport.X + vDestport.Width) + _Point.x, static_cast<float>(vDestport.Y + vDestport.Height) + _Point.y, _Point.z, 1.0f), TrueColor32(_Color), D3DXVECTOR2(_Tex.vUV.x, _Tex.vUV.y) },
		{ D3DXVECTOR4(static_cast<float>(vDestport.X + vDestport.Width) + _Point.x, static_cast<float>(vDestport.Y                   ) + _Point.y, _Point.z, 1.0f), TrueColor32(_Color), D3DXVECTOR2(_Tex.vUV.x,       0.0f) }
	};
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, nVertex, sizeof(TDVERTEX2D));
	pDevice->SetTexture(0, NULL);
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::DrawTexture()
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::DrawTexture(TEXTURE9 &_Tex, const core::VECT3f &_Point, const core::COLORf &_Color){
	pDevice->SetFVF(FVF_TDVERTEX2D);
	pDevice->SetTexture(0, _Tex.pTexture);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	TDVERTEX2D	nVertex[]	= {
		{ D3DXVECTOR4(_Point.x             , _Point.y + _Tex.vXY.y, _Point.z, 1.0f), TrueColor32(_Color), D3DXVECTOR2(      0.0f, _Tex.vUV.y) },
		{ D3DXVECTOR4(_Point.x             , _Point.y             , _Point.z, 1.0f), TrueColor32(_Color), D3DXVECTOR2(      0.0f,       0.0f) },
		{ D3DXVECTOR4(_Point.x + _Tex.vXY.x, _Point.y + _Tex.vXY.y, _Point.z, 1.0f), TrueColor32(_Color), D3DXVECTOR2(_Tex.vUV.x, _Tex.vUV.y) },
		{ D3DXVECTOR4(_Point.x + _Tex.vXY.x, _Point.y             , _Point.z, 1.0f), TrueColor32(_Color), D3DXVECTOR2(_Tex.vUV.x,       0.0f) }
	};
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, nVertex, sizeof(TDVERTEX2D));
	pDevice->SetTexture(0, NULL);
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::DrawTexture()
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::DrawTexture(TEXTURE9 &_Tex, const core::VECT3f &_Point, const core::COLORf &_Color, float _Rotate){
	pDevice->SetFVF(FVF_TDVERTEX2D);
	pDevice->SetTexture(0, _Tex.pTexture);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	float		fRz		= 0.5f * _Tex.vXY.Abs();
	float		fTh		= _Tex.vXY.Rot();
	TDVERTEX2D	nVertex[]	= {
		{ D3DXVECTOR4(_Point.x + fRz * cos(core::PI - fTh - _Rotate), _Point.y + fRz * sin(core::PI - fTh - _Rotate), _Point.z, 1.0f), TrueColor32(_Color), D3DXVECTOR2(      0.0f, _Tex.vUV.y) },
		{ D3DXVECTOR4(_Point.x + fRz * cos(core::PI + fTh - _Rotate), _Point.y + fRz * sin(core::PI + fTh - _Rotate), _Point.z, 1.0f), TrueColor32(_Color), D3DXVECTOR2(      0.0f,       0.0f) },
		{ D3DXVECTOR4(_Point.x + fRz * cos(         + fTh - _Rotate), _Point.y + fRz * sin(         + fTh - _Rotate), _Point.z, 1.0f), TrueColor32(_Color), D3DXVECTOR2(_Tex.vUV.x, _Tex.vUV.y) },
		{ D3DXVECTOR4(_Point.x + fRz * cos(         - fTh - _Rotate), _Point.y + fRz * sin(         - fTh - _Rotate), _Point.z, 1.0f), TrueColor32(_Color), D3DXVECTOR2(_Tex.vUV.x,       0.0f) }
	};
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, nVertex, sizeof(TDVERTEX2D));
	pDevice->SetTexture(0, NULL);
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::DrawTexture()
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::DrawTexture(TEXTURE9 &_Tex, const core::VECT3f &_Point, const core::COLORf &_Color, float _Rotate, float _Extend){
	pDevice->SetFVF(FVF_TDVERTEX2D);
	pDevice->SetTexture(0, _Tex.pTexture);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	float		fRz		= 0.5f * _Extend * _Tex.vXY.Abs();
	float		fTh		= _Tex.vXY.Rot();
	TDVERTEX2D	nVertex[]	= {
		{ D3DXVECTOR4(_Point.x + fRz * cos(core::PI - fTh - _Rotate), _Point.y + fRz * sin(core::PI - fTh - _Rotate), _Point.z, 1.0f), TrueColor32(_Color), D3DXVECTOR2(      0.0f, _Tex.vUV.y) },
		{ D3DXVECTOR4(_Point.x + fRz * cos(core::PI + fTh - _Rotate), _Point.y + fRz * sin(core::PI + fTh - _Rotate), _Point.z, 1.0f), TrueColor32(_Color), D3DXVECTOR2(      0.0f,       0.0f) },
		{ D3DXVECTOR4(_Point.x + fRz * cos(         + fTh - _Rotate), _Point.y + fRz * sin(         + fTh - _Rotate), _Point.z, 1.0f), TrueColor32(_Color), D3DXVECTOR2(_Tex.vUV.x, _Tex.vUV.y) },
		{ D3DXVECTOR4(_Point.x + fRz * cos(         - fTh - _Rotate), _Point.y + fRz * sin(         - fTh - _Rotate), _Point.z, 1.0f), TrueColor32(_Color), D3DXVECTOR2(_Tex.vUV.x,       0.0f) }
	};
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, nVertex, sizeof(TDVERTEX2D));
	pDevice->SetTexture(0, NULL);
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::SaveTexture(TEXTURE9 &, const wchar_t *, unsigned int)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::SaveTexture(TEXTURE9 &_Tex, const wchar_t *_FileName, unsigned int _Format){
	RECT				cRect		= { 0, 0, static_cast<int>(_Tex.vXY.x), static_cast<int>(_Tex.vXY.y) };
	IDirect3DSurface9 *	pSurface	= NULL;
	if(FAILED(_Tex.pTexture->GetSurfaceLevel(0, &pSurface))){
		return (false);
	}
	if(FAILED(D3DXSaveSurfaceToFileW(_FileName, static_cast<D3DXIMAGE_FILEFORMAT>(_Format), pSurface, NULL, &cRect))){
		SafeRelease(pSurface);
		return (false);
	}
	SafeRelease(pSurface);
	return (true);
}