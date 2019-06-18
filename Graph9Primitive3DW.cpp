#include"owlEngine9.hpp"

//***********************************************************************************************************************************************************************
//
//		Class Constructor / Destructor
//
//***********************************************************************************************************************************************************************
//		owl::engine::PRIMITIVE3D9W::PRIMITIVE3D9W()
//***********************************************************************************************************************************************************************
owl::engine::PRIMITIVE3D9W::PRIMITIVE3D9W(){
	uSize	= 0;
	uPush	= 0;
	pDraw	= NULL;
	pInfo	= NULL;
	pThis	= NULL;
	pIB		= NULL;
	pVB		= NULL;
}

//***********************************************************************************************************************************************************************
//		PRIMITIVE3D9W::~PRIMITIVE3D9W()
//***********************************************************************************************************************************************************************
owl::engine::PRIMITIVE3D9W::~PRIMITIVE3D9W(){
	uSize	= 0;
	uPush	= 0;
	pDraw	= NULL;
	pThis	= NULL;
	SafeDeleteArray(pInfo);
	SafeRelease(pIB);
	SafeRelease(pVB);
}

//***********************************************************************************************************************************************************************
//
//		Primitive Type
//
//***********************************************************************************************************************************************************************
//		owl::engine::render::SPOT3D(IDirect3DDevice9 *, PRIMITIVE3D &)
//***********************************************************************************************************************************************************************
void owl::engine::render::SPOT3D(IDirect3DDevice9 *pDevice, owl::engine::PRIMITIVE3D9W &Primitive){
	pDevice->DrawPrimitiveUP(D3DPT_POINTLIST, (Primitive.Push()), Primitive.Base(), sizeof(owl::engine::TDNVERTEX3D));
}

//***********************************************************************************************************************************************************************
//		owl::engine::render::LINE3D(IDirect3DDevice9 *, PRIMITIVE3D &)
//***********************************************************************************************************************************************************************
void owl::engine::render::LINE3D(IDirect3DDevice9 *pDevice, owl::engine::PRIMITIVE3D9W &Primitive){
	pDevice->DrawPrimitiveUP(D3DPT_LINELIST, (Primitive.Push() >> 1), Primitive.Base(), sizeof(owl::engine::TDNVERTEX3D));
}

//***********************************************************************************************************************************************************************
//		owl::engine::render::WIRE3D(IDirect3DDevice9 *, PRIMITIVE3D &)
//***********************************************************************************************************************************************************************
void owl::engine::render::WIRE3D(IDirect3DDevice9 *pDevice, owl::engine::PRIMITIVE3D9W &Primitive){
	pDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, Primitive.Push() - 1, Primitive.Base(), sizeof(owl::engine::TDNVERTEX3D));
}

//***********************************************************************************************************************************************************************
//		owl::engine::render::POLY3D(IDirect3DDevice9 *, PRIMITIVE3D &)
//***********************************************************************************************************************************************************************
void owl::engine::render::POLY3D(IDirect3DDevice9 *pDevice, owl::engine::PRIMITIVE3D9W &Primitive){
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, (Primitive.Push() / 3), Primitive.Base(), sizeof(owl::engine::TDNVERTEX3D));
}

//***********************************************************************************************************************************************************************
//		owl::engine::render::QUAD3D(IDirect3DDevice9 *, PRIMITIVE3D &)
//***********************************************************************************************************************************************************************
void owl::engine::render::QUAD3D(IDirect3DDevice9 *pDevice, owl::engine::PRIMITIVE3D9W &Primitive){
	TDNVERTEX3D *	pVtx;
	assert(Primitive.VtxBuf());
	if(SUCCEEDED(Primitive.VtxBuf()->Lock(0, 0, reinterpret_cast<void **>(&pVtx), 0))){
		for(unsigned int i = 0; i < (Primitive.Push() >> 2); i++){
			pVtx[4 * i + 0]	= Primitive.Base()[4 * i + 0];
			pVtx[4 * i + 1]	= Primitive.Base()[4 * i + 1];
			pVtx[4 * i + 2]	= Primitive.Base()[4 * i + 2];
			pVtx[4 * i + 3]	= Primitive.Base()[4 * i + 3];
		}
		if(SUCCEEDED(Primitive.VtxBuf()->Unlock())){
			pDevice->SetIndices(Primitive.IdxBuf());
			pDevice->SetStreamSource(0, Primitive.VtxBuf(), 0, sizeof(TDNVERTEX3D));
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, Primitive.Push(), 0, (Primitive.Push() >> 1));
		}
	}
}

//***********************************************************************************************************************************************************************
//
//		GRAPH9 Function
//
//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::InitPrimitive()
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::InitPrimitive(PRIMITIVE3D9W &_Prm, unsigned int _Buffer, unsigned int _Primitive){
	if(_Prm.pInfo != NULL){
		return (true);
	}
	switch(_Primitive){
		case (owl::engine::PRIMITIVE_POINT):
			try{
				_Prm.pInfo	= new TDNVERTEX3D[_Buffer];
			}catch(std::bad_alloc){
				QuitPrimitive(_Prm);
				return (false);
			}
			_Prm.uPush	= 0;
			_Prm.uSize	= _Buffer;
			_Prm.pDraw	= owl::engine::render::SPOT3D;
			_Prm.pThis	= _Prm.pInfo;
			memset(_Prm.pInfo, 0, sizeof(DNVERTEX3D));
			break;
		case (owl::engine::PRIMITIVE_LINE):
			try{
				_Prm.pInfo	= new TDNVERTEX3D[_Buffer];
			}catch(std::bad_alloc){
				QuitPrimitive(_Prm);
				return (false);
			}
			_Prm.uPush	= 0;
			_Prm.uSize	= _Buffer;
			_Prm.pDraw	= owl::engine::render::LINE3D;
			_Prm.pThis	= _Prm.pInfo;
			memset(_Prm.pInfo, 0, sizeof(DNVERTEX3D));
			break;
		case (owl::engine::PRIMITIVE_WIRE):
			try{
				_Prm.pInfo	= new TDNVERTEX3D[_Buffer];
			}catch(std::bad_alloc){
				QuitPrimitive(_Prm);
				return (false);
			}
			_Prm.uPush	= 0;
			_Prm.uSize	= _Buffer;
			_Prm.pDraw	= owl::engine::render::WIRE3D;
			_Prm.pThis	= _Prm.pInfo;
			memset(_Prm.pInfo, 0, sizeof(DNVERTEX3D));
			break;
		case (owl::engine::PRIMITIVE_POLY):
			try{
				_Prm.pInfo	= new TDNVERTEX3D[_Buffer];
			}catch(std::bad_alloc){
				QuitPrimitive(_Prm);
				return (false);
			}
			_Prm.uPush	= 0;
			_Prm.uSize	= _Buffer;
			_Prm.pDraw	= owl::engine::render::POLY3D;
			_Prm.pThis	= _Prm.pInfo;
			memset(_Prm.pInfo, 0, sizeof(DNVERTEX3D));
			break;
		case (owl::engine::PRIMITIVE_QUAD):
			_Buffer	= _Buffer - (_Buffer % 4);
			if(FAILED(pDevice->CreateVertexBuffer(_Buffer * sizeof(DNVERTEX3D), 0, FVF_DNVERTEX3D, D3DPOOL_MANAGED, &_Prm.pVB, NULL))){
				QuitPrimitive(_Prm);
				return (false);
			}
			if(FAILED(pDevice->CreateIndexBuffer((_Buffer >> 1) * 3 * sizeof(WORD), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &_Prm.pIB, NULL))){
				QuitPrimitive(_Prm);
				return (false);
			}
			try{
				_Prm.pInfo	= new TDNVERTEX3D[_Buffer];
			}catch(std::bad_alloc){
				QuitPrimitive(_Prm);
				return (false);
			}
			_Prm.uPush	= 0;
			_Prm.uSize	= _Buffer;
			_Prm.pDraw	= owl::engine::render::QUAD3D;
			_Prm.pThis	= _Prm.pInfo;
			memset(_Prm.pInfo, 0, sizeof(DNVERTEX3D));
			WORD *	pIndex;
			if(FAILED(_Prm.pIB->Lock(0, 0, reinterpret_cast<void **>(&pIndex), 0))){
				QuitPrimitive(_Prm);
				return (false);
			}
			for(unsigned int i = 0; i < (_Buffer >> 2); i++){
				pIndex[6 * i + 0]	= 4 * i + 0;
				pIndex[6 * i + 1]	= 4 * i + 1;
				pIndex[6 * i + 2]	= 4 * i + 2;
				pIndex[6 * i + 3]	= 4 * i + 2;
				pIndex[6 * i + 4]	= 4 * i + 1;
				pIndex[6 * i + 5]	= 4 * i + 3;
			}
			if(FAILED(_Prm.pIB->Unlock())){
				QuitPrimitive(_Prm);
				return (false);
			}
			break;
		default:
			return (false);
	}
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::QuitPrimitive()
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::QuitPrimitive(PRIMITIVE3D9W &_Prm){
	SafeDeleteArray(_Prm.pInfo);
	SafeRelease(_Prm.pIB);
	SafeRelease(_Prm.pVB);
	_Prm.uSize	= 0;
	_Prm.uPush	= 0;
	_Prm.pThis	= NULL;
	_Prm.pDraw	= NULL;
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::PostPrimitive()
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::PostPrimitive(PRIMITIVE3D9W &_Prm, const core::VECT3f &_Point, const core::VECT3f &_Normal, const core::COLORf &_Color, const core::VECT2f &_Coord){
	if(_Prm.uPush + 1 >= _Prm.uSize){
		return (false);
	}
	_Prm.pThis->Point.x		= _Point.x;
	_Prm.pThis->Point.y		= _Point.y;
	_Prm.pThis->Point.z		= _Point.z;
	_Prm.pThis->Normal.x	= _Normal.x;
	_Prm.pThis->Normal.y	= _Normal.y;
	_Prm.pThis->Normal.z	= _Normal.z;
	_Prm.pThis->Color		= TrueColor32(_Color);
	_Prm.pThis->Coord.x		= _Coord.x;
	_Prm.pThis->Coord.y		= _Coord.y;
	_Prm.pThis++;
	_Prm.uPush++;
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::DrawPrimitive()
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::DrawPrimitive(PRIMITIVE3D9W &_Prm, TEXTURE9 &_Tex){
	pDevice->SetFVF(FVF_TDNVERTEX3D);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetTexture(0, _Tex.pTexture);
	_Prm.pDraw(pDevice, _Prm);
	_Prm.uPush	= 0;
	_Prm.pThis	= _Prm.pInfo;
	pDevice->SetTexture(0, NULL);
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::BindPrimitive()
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::BindPrimitive(PRIMITIVE3D9W &_Prm, unsigned int _Primitive){
	if(_Prm.pDraw == owl::engine::render::QUAD3D){
		return (false);
	}else{
		switch(_Primitive){
			case (owl::engine::PRIMITIVE_POINT):
				_Prm.pDraw	= owl::engine::render::SPOT3D;
				break;
			case (owl::engine::PRIMITIVE_LINE):
				_Prm.pDraw	= owl::engine::render::LINE3D;
				break;
			case (owl::engine::PRIMITIVE_WIRE):
				_Prm.pDraw	= owl::engine::render::WIRE3D;
				break;
			case (owl::engine::PRIMITIVE_POLY):
				_Prm.pDraw	= owl::engine::render::POLY3D;
				break;
			default:
				return (false);
		}
	}
	return (true);
}