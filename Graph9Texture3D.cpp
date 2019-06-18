#include"owlEngine9.hpp"

//***********************************************************************************************************************************************************************
//
//		Class Constructor / Destructor
//
//***********************************************************************************************************************************************************************
//		TEXTURE3D9::TEXTURE3D9()
//***********************************************************************************************************************************************************************
owl::engine::TEXTURE3D9::TEXTURE3D9() : TEXTURE9(){
	uSize	= 0;
	uPush	= 0;
	pInfo	= NULL;
	pThis	= NULL;
	pIB		= NULL;
	pVB		= NULL;
}

//***********************************************************************************************************************************************************************
//		TEXTURE3D9::~TEXTURE3D9()
//***********************************************************************************************************************************************************************
owl::engine::TEXTURE3D9::~TEXTURE3D9(){
	uSize	= 0;
	uPush	= 0;
	pThis	= NULL;
	SafeDeleteArray(pInfo);
	SafeRelease(pIB);
	SafeRelease(pVB);
}

//***********************************************************************************************************************************************************************
//
//		GRAPH9 Function
//
//***********************************************************************************************************************************************************************
//		GRAPH9::InitTexture(TEXTURE3D9 &, unsigned int, const core::COLORf &, const wchar_t *)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::InitTexture(TEXTURE3D9 &_Tex, unsigned int _Buffer, const core::COLORf &_Color, const wchar_t *_FileName){
	if(_Tex.pTexture != NULL){
		return (true);
	}
	_Buffer	= _Buffer - (_Buffer % 4);
	if(FAILED(pDevice->CreateVertexBuffer(_Buffer * sizeof(TDVERTEX3D), 0, FVF_TDVERTEX3D, D3DPOOL_MANAGED, &_Tex.pVB, NULL))){
		QuitTexture(_Tex);
		return (false);
	}
	if(FAILED(pDevice->CreateIndexBuffer((_Buffer >> 1) * 3 * sizeof(WORD), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &_Tex.pIB, NULL))){
		QuitTexture(_Tex);
		return (false);
	}
	try{
		_Tex.pInfo	= new TDVERTEX3D[_Buffer];
	}catch(std::bad_alloc){
		QuitTexture(_Tex);
		return (false);
	}
	_Tex.uPush	= 0;
	_Tex.uSize	= _Buffer;
	_Tex.pThis	= _Tex.pInfo;
	memset(_Tex.pInfo, 0, sizeof(TDVERTEX3D));

	WORD *	pIndex;
	if(FAILED(_Tex.pIB->Lock(0, 0, reinterpret_cast<void **>(&pIndex), 0))){
		QuitTexture(_Tex);
		return (false);
	}
	for(UINT i = 0; i < (_Buffer >> 2); i++){
		pIndex[6 * i + 0]	= 4 * i + 0;
		pIndex[6 * i + 1]	= 4 * i + 1;
		pIndex[6 * i + 2]	= 4 * i + 2;
		pIndex[6 * i + 3]	= 4 * i + 2;
		pIndex[6 * i + 4]	= 4 * i + 1;
		pIndex[6 * i + 5]	= 4 * i + 3;
	}
	if(FAILED(_Tex.pIB->Unlock())){
		QuitTexture(_Tex);
		return (false);
	}
	D3DXIMAGE_INFO		Info;
	D3DSURFACE_DESC		Desc;
	if(_Tex.pTexture != NULL){
		return (true);
	}
	if(FAILED(D3DXGetImageInfoFromFileW(_FileName, &Info))){
		QuitTexture(_Tex);
		return (false);
	}
	if(FAILED(D3DXCreateTextureFromFileEx(pDevice, _FileName, D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, TrueColor32(_Color), NULL, NULL, &_Tex.pTexture))){
		QuitTexture(_Tex);
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
//		GRAPH9::InitTexture(TEXTURE3D9 &, unsigned int, const core::COLORf &, const unsigned char *, unsigned int)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::InitTexture(TEXTURE3D9 &_Tex, unsigned int _Buffer, const core::COLORf &_Color, const unsigned char *_Stream, unsigned int _Count){
	if(_Tex.pTexture != NULL){
		return (true);
	}
	_Buffer	= _Buffer - (_Buffer % 4);
	if(FAILED(pDevice->CreateVertexBuffer(_Buffer * sizeof(TDVERTEX3D), 0, FVF_TDVERTEX3D, D3DPOOL_MANAGED, &_Tex.pVB, NULL))){
		QuitTexture(_Tex);
		return (false);
	}
	if(FAILED(pDevice->CreateIndexBuffer((_Buffer >> 1) * 3 * sizeof(WORD), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &_Tex.pIB, NULL))){
		QuitTexture(_Tex);
		return (false);
	}
	try{
		_Tex.pInfo	= new TDVERTEX3D[_Buffer];
	}catch(std::bad_alloc){
		QuitTexture(_Tex);
		return (false);
	}
	_Tex.uPush	= 0;
	_Tex.uSize	= _Buffer;
	_Tex.pThis	= _Tex.pInfo;
	memset(_Tex.pInfo, 0, sizeof(TDVERTEX3D));
	WORD *	pIndex;
	if(FAILED(_Tex.pIB->Lock(0, 0, reinterpret_cast<void **>(&pIndex), 0))){
		QuitTexture(_Tex);
		return (false);
	}
	for(UINT i = 0; i < (_Buffer >> 2); i++){
		pIndex[6 * i + 0]	= 4 * i + 0;
		pIndex[6 * i + 1]	= 4 * i + 1;
		pIndex[6 * i + 2]	= 4 * i + 2;
		pIndex[6 * i + 3]	= 4 * i + 2;
		pIndex[6 * i + 4]	= 4 * i + 1;
		pIndex[6 * i + 5]	= 4 * i + 3;
	}
	if(FAILED(_Tex.pIB->Unlock())){
		QuitTexture(_Tex);
		return (false);
	}
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
//		GRAPH9::InitTexture(TEXTURE3D9 &, unsigned int, const core::COLORf &, HMODULE, const wchar_t *)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::InitTexture(TEXTURE3D9 &_Tex, unsigned int _Buffer, const core::COLORf &_Color, HMODULE _Module, const wchar_t *_Resource){
	if(_Tex.pTexture != NULL){
		return (true);
	}
	_Buffer	= _Buffer - (_Buffer % 4);
	if(FAILED(pDevice->CreateVertexBuffer(_Buffer * sizeof(TDVERTEX3D), 0, FVF_TDVERTEX3D, D3DPOOL_MANAGED, &_Tex.pVB, NULL))){
		QuitTexture(_Tex);
		return (false);
	}
	if(FAILED(pDevice->CreateIndexBuffer((_Buffer >> 1) * 3 * sizeof(WORD), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &_Tex.pIB, NULL))){
		QuitTexture(_Tex);
		return (false);
	}
	try{
		_Tex.pInfo	= new TDVERTEX3D[_Buffer];
	}catch(std::bad_alloc){
		QuitTexture(_Tex);
		return (false);
	}
	_Tex.uPush	= 0;
	_Tex.uSize	= _Buffer;
	_Tex.pThis	= _Tex.pInfo;
	memset(_Tex.pInfo, 0, sizeof(TDVERTEX3D));
	WORD *	pIndex;
	if(FAILED(_Tex.pIB->Lock(0, 0, reinterpret_cast<void **>(&pIndex), 0))){
		QuitTexture(_Tex);
		return (false);
	}
	for(UINT i = 0; i < (_Buffer >> 2); i++){
		pIndex[6 * i + 0]	= 4 * i + 0;
		pIndex[6 * i + 1]	= 4 * i + 1;
		pIndex[6 * i + 2]	= 4 * i + 2;
		pIndex[6 * i + 3]	= 4 * i + 2;
		pIndex[6 * i + 4]	= 4 * i + 1;
		pIndex[6 * i + 5]	= 4 * i + 3;
	}
	if(FAILED(_Tex.pIB->Unlock())){
		QuitTexture(_Tex);
		return (false);
	}
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
//		GRAPH9::InitTexture(TEXTURE3D9 &, unsigned int, const core::COLORf &, const core::VECT2i &)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::InitTexture(TEXTURE3D9 &_Tex, unsigned int _Buffer, const core::COLORf &_Color, const core::VECT2i &_Size){
	if(_Tex.pTexture != NULL){
		return (true);
	}
	_Buffer	= _Buffer - (_Buffer % 4);
	if(FAILED(pDevice->CreateVertexBuffer(_Buffer * sizeof(TDVERTEX3D), 0, FVF_TDVERTEX3D, D3DPOOL_MANAGED, &_Tex.pVB, NULL))){
		QuitTexture(_Tex);
		return (false);
	}
	if(FAILED(pDevice->CreateIndexBuffer((_Buffer >> 1) * 3 * sizeof(WORD), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &_Tex.pIB, NULL))){
		QuitTexture(_Tex);
		return (false);
	}
	try{
		_Tex.pInfo	= new TDVERTEX3D[_Buffer];
	}catch(std::bad_alloc){
		QuitTexture(_Tex);
		return (false);
	}
	_Tex.uPush	= 0;
	_Tex.uSize	= _Buffer;
	_Tex.pThis	= _Tex.pInfo;
	memset(_Tex.pInfo, 0, sizeof(TDVERTEX3D));

	WORD *	pIndex;
	if(FAILED(_Tex.pIB->Lock(0, 0, reinterpret_cast<void **>(&pIndex), 0))){
		QuitTexture(_Tex);
		return (false);
	}
	for(UINT i = 0; i < (_Buffer >> 2); i++){
		pIndex[6 * i + 0]	= 4 * i + 0;
		pIndex[6 * i + 1]	= 4 * i + 1;
		pIndex[6 * i + 2]	= 4 * i + 2;
		pIndex[6 * i + 3]	= 4 * i + 2;
		pIndex[6 * i + 4]	= 4 * i + 1;
		pIndex[6 * i + 5]	= 4 * i + 3;
	}
	if(FAILED(_Tex.pIB->Unlock())){
		QuitTexture(_Tex);
		return (false);
	}
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
	_Tex.vXY.x	= static_cast<float>(_Size.x);
	_Tex.vXY.y	= static_cast<float>(_Size.y);
	_Tex.vUV.x	= static_cast<float>(_Size.x) / static_cast<float>(_S);
	_Tex.vUV.y	= static_cast<float>(_Size.y) / static_cast<float>(_S);
	return (true);
}

//***********************************************************************************************************************************************************************
//		GRAPH9::CopyTexture()
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::CopyTexture(TEXTURE3D9 &_Tex, unsigned int _Buffer, owl::engine::TEXTURE9 &_Src){
	if(_Tex.pTexture != NULL){
		return (true);
	}
	_Buffer	= _Buffer - (_Buffer % 4);
	if(FAILED(pDevice->CreateVertexBuffer(_Buffer * sizeof(TDVERTEX3D), 0, FVF_TDVERTEX3D, D3DPOOL_MANAGED, &_Tex.pVB, NULL))){
		QuitTexture(_Tex);
		return (false);
	}
	if(FAILED(pDevice->CreateIndexBuffer((_Buffer >> 1) * 3 * sizeof(WORD), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &_Tex.pIB, NULL))){
		QuitTexture(_Tex);
		return (false);
	}
	try{
		_Tex.pInfo	= new TDVERTEX3D[_Buffer];
	}catch(std::bad_alloc){
		QuitTexture(_Tex);
		return (false);
	}
	_Tex.uPush	= 0;
	_Tex.uSize	= _Buffer;
	_Tex.pThis	= _Tex.pInfo;
	memset(_Tex.pInfo, 0, sizeof(TDVERTEX3D));

	WORD *	pIndex;
	if(FAILED(_Tex.pIB->Lock(0, 0, reinterpret_cast<void **>(&pIndex), 0))){
		QuitTexture(_Tex);
		return (false);
	}
	for(UINT i = 0; i < (_Buffer >> 2); i++){
		pIndex[6 * i + 0]	= 4 * i + 0;
		pIndex[6 * i + 1]	= 4 * i + 1;
		pIndex[6 * i + 2]	= 4 * i + 2;
		pIndex[6 * i + 3]	= 4 * i + 2;
		pIndex[6 * i + 4]	= 4 * i + 1;
		pIndex[6 * i + 5]	= 4 * i + 3;
	}
	if(FAILED(_Tex.pIB->Unlock())){
		QuitTexture(_Tex);
		return (false);
	}
	if(_Tex.pTexture != NULL){
		return (true);
	}
	if(_Src.pTexture == NULL){
		return (false);
	}
	_Tex.pTexture	= _Src.pTexture;
	_Tex.vXY		= _Src.vXY;
	_Tex.vUV		= _Src.vUV;
	_Tex.pTexture->AddRef();
	return (true);
}

//***********************************************************************************************************************************************************************
//		GRAPH9::QuitTexture(TEXTURE3D9 &)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::QuitTexture(TEXTURE3D9 &_Tex){
	SafeDeleteArray(_Tex.pInfo);
	SafeRelease(_Tex.pIB);
	SafeRelease(_Tex.pVB);
	SafeRelease(_Tex.pTexture);
	_Tex.uSize	= 0;
	_Tex.vXY	= core::VECT2f(0);
	_Tex.vUV	= core::VECT2f(0);
	_Tex.pThis	= NULL;
	return (true);
}

//***********************************************************************************************************************************************************************
//		GRAPH9::PostTexture(TEXTURE3D9 &, const core::VECT3f &, const core::VECT2f &, const core::VECT4f &, const core::COLORf &)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::PostTexture(TEXTURE3D9 &_Tex, const core::VECT3f &_Point, const core::VECT2f &_Size, const core::VECT4f &_Coord, const core::COLORf &_Color){
	if(_Tex.uPush + 4 > _Tex.uSize){
		return (false);
	}
	core::VECT4f	_TexCoord(_Coord.x * _Tex.vUV.x, _Coord.y * _Tex.vUV.y, _Coord.z * _Tex.vUV.x, _Coord.w * _Tex.vUV.y);
	TDVERTEX3D		nVertex[]	= {
		{ D3DXVECTOR3(_Point.x          , _Point.y          , _Point.z), TrueColor32(_Color), D3DXVECTOR2(_TexCoord.x, _TexCoord.w) },
		{ D3DXVECTOR3(_Point.x          , _Point.y + _Size.y, _Point.z), TrueColor32(_Color), D3DXVECTOR2(_TexCoord.x, _TexCoord.y) },
		{ D3DXVECTOR3(_Point.x + _Size.x, _Point.y          , _Point.z), TrueColor32(_Color), D3DXVECTOR2(_TexCoord.z, _TexCoord.w) },
		{ D3DXVECTOR3(_Point.x + _Size.x, _Point.y + _Size.y, _Point.z), TrueColor32(_Color), D3DXVECTOR2(_TexCoord.z, _TexCoord.y) }
	};
	_Tex.pThis[0]	 = nVertex[0];
	_Tex.pThis[1]	 = nVertex[1];
	_Tex.pThis[2]	 = nVertex[2];
	_Tex.pThis[3]	 = nVertex[3];
	_Tex.pThis		+= 4;
	_Tex.uPush		+= 4;
	return (true);
}

//***********************************************************************************************************************************************************************
//		GRAPH9::PostTexture(TEXTURE3D9 &, const core::VECT3f &, const core::VECT2f &, const core::VECT4f &, const core::COLORf &, float)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::PostTexture(TEXTURE3D9 &_Tex, const core::VECT3f &_Point, const core::VECT2f &_Size, const core::VECT4f &_Coord, const core::COLORf &_Color, float _Rotate){
	if(_Tex.uPush + 4 > _Tex.uSize){
		return (false);
	}
	FLOAT			fRz		= 0.5f * sqrt(_Size.x * _Size.x + _Size.y * _Size.y);
	FLOAT			fTh		= atan2(_Size.y, _Size.x);
	core::VECT4f	_TexCoord(_Coord.x * _Tex.vUV.x, _Coord.y * _Tex.vUV.y, _Coord.z * _Tex.vUV.x, _Coord.w * _Tex.vUV.y);
	TDVERTEX3D		nVertex[]	= {
		{ D3DXVECTOR3(_Point.x + fRz * cos(core::PI - fTh + _Rotate), _Point.y + fRz * sin(core::PI - fTh + _Rotate), _Point.z), TrueColor32(_Color), D3DXVECTOR2(_TexCoord.x, _TexCoord.w) },
		{ D3DXVECTOR3(_Point.x + fRz * cos(core::PI + fTh + _Rotate), _Point.y + fRz * sin(core::PI + fTh + _Rotate), _Point.z), TrueColor32(_Color), D3DXVECTOR2(_TexCoord.x, _TexCoord.y) },
		{ D3DXVECTOR3(_Point.x + fRz * cos(         + fTh + _Rotate), _Point.y + fRz * sin(         + fTh + _Rotate), _Point.z), TrueColor32(_Color), D3DXVECTOR2(_TexCoord.z, _TexCoord.w) },
		{ D3DXVECTOR3(_Point.x + fRz * cos(         - fTh + _Rotate), _Point.y + fRz * sin(         - fTh + _Rotate), _Point.z), TrueColor32(_Color), D3DXVECTOR2(_TexCoord.z, _TexCoord.y) }
	};
	_Tex.pThis[0]	 = nVertex[0];
	_Tex.pThis[1]	 = nVertex[1];
	_Tex.pThis[2]	 = nVertex[2];
	_Tex.pThis[3]	 = nVertex[3];
	_Tex.pThis		+= 4;
	_Tex.uPush		+= 4;
	return (true);
}

//***********************************************************************************************************************************************************************
//		GRAPH9::PostTexture(TEXTURE3D9 &, const core::VECT3f &, const core::VECT2f &, const core::VECT4f &, const core::COLORf &, float, float)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::PostTexture(TEXTURE3D9 &_Tex, const core::VECT3f &_Point, const core::VECT2f &_Size, const core::VECT4f &_Coord, const core::COLORf &_Color, float _Rotate, float _Extend){
	if(_Tex.uPush + 4 > _Tex.uSize){
		return (false);
	}
	FLOAT			fRz		= 0.5f * _Extend * sqrt(_Size.x * _Size.x + _Size.y * _Size.y);
	FLOAT			fTh		= atan2(_Size.y, _Size.x);
	core::VECT4f	_TexCoord(_Coord.x * _Tex.vUV.x, _Coord.y * _Tex.vUV.y, _Coord.z * _Tex.vUV.x, _Coord.w * _Tex.vUV.y);
	TDVERTEX3D		nVertex[]	= {
		{ D3DXVECTOR3(_Point.x + fRz * cos(core::PI - fTh + _Rotate), _Point.y + fRz * sin(core::PI - fTh + _Rotate), _Point.z), TrueColor32(_Color), D3DXVECTOR2(_TexCoord.x, _TexCoord.w) },
		{ D3DXVECTOR3(_Point.x + fRz * cos(core::PI + fTh + _Rotate), _Point.y + fRz * sin(core::PI + fTh + _Rotate), _Point.z), TrueColor32(_Color), D3DXVECTOR2(_TexCoord.x, _TexCoord.y) },
		{ D3DXVECTOR3(_Point.x + fRz * cos(         + fTh + _Rotate), _Point.y + fRz * sin(         + fTh + _Rotate), _Point.z), TrueColor32(_Color), D3DXVECTOR2(_TexCoord.z, _TexCoord.w) },
		{ D3DXVECTOR3(_Point.x + fRz * cos(         - fTh + _Rotate), _Point.y + fRz * sin(         - fTh + _Rotate), _Point.z), TrueColor32(_Color), D3DXVECTOR2(_TexCoord.z, _TexCoord.y) }
	};
	_Tex.pThis[0]	 = nVertex[0];
	_Tex.pThis[1]	 = nVertex[1];
	_Tex.pThis[2]	 = nVertex[2];
	_Tex.pThis[3]	 = nVertex[3];
	_Tex.pThis		+= 4;
	_Tex.uPush		+= 4;
	return (true);
}

//***********************************************************************************************************************************************************************
//		GRAPH9::DrawTexture(TEXTURE3D9 &)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::DrawTexture(TEXTURE3D9 &_Tex){
	TDVERTEX3D *	pVtx;
	assert(_Tex.pVB);
	if(FAILED(_Tex.pVB->Lock(0, 0, reinterpret_cast<void **>(&pVtx), 0))){
		return (false);
	}
	for(UINT i = 0; i < (_Tex.uSize >> 2); i++){
		pVtx[4 * i + 0]	= _Tex.pInfo[4 * i + 0];
		pVtx[4 * i + 1]	= _Tex.pInfo[4 * i + 1];
		pVtx[4 * i + 2]	= _Tex.pInfo[4 * i + 2];
		pVtx[4 * i + 3]	= _Tex.pInfo[4 * i + 3];
	}
	if(FAILED(_Tex.pVB->Unlock())){
		return (false);
	}
	pDevice->SetIndices(_Tex.pIB);
	pDevice->SetFVF(FVF_TDVERTEX3D);
	pDevice->SetTexture(0, _Tex.pTexture);
	pDevice->SetStreamSource(0, _Tex.pVB, 0, sizeof(TDVERTEX3D));
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, _Tex.uSize, 0, (_Tex.uPush >> 1));
	pDevice->SetTexture(0, NULL);
	_Tex.uPush	= 0;
	_Tex.pThis	= _Tex.pInfo;
	return (true);
}