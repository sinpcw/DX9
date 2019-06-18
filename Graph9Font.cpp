#include"owlEngine9.hpp"

#define	TEXTLEVEL0			(0)
#define	TEXTLEVEL1			(1)
#define	TEXTLEVEL2			(2)
#define	TEXTLEVEL3			(3)
#define	TEXTBUFFER			(1024)

//***********************************************************************************************************************************************************************
//
//		Class Constructor / Destructor
//
//***********************************************************************************************************************************************************************
//		FONT9::FONT9()
//***********************************************************************************************************************************************************************
owl::engine::FONT9::FONT9(){
	hContext	= NULL;
	hNewFont	= NULL;
	hOldFont	= NULL;
	iQuality	= 0;
	mTexture.clear();
}

//***********************************************************************************************************************************************************************
//		FONT9::~FONT9()
//***********************************************************************************************************************************************************************
owl::engine::FONT9::~FONT9(){
	if(hContext){
		SelectObject(hContext, hOldFont);
		DeleteObject(hNewFont);
		ReleaseDC(NULL, hContext);
		hContext	= NULL;
		hNewFont	= NULL;
		hOldFont	= NULL;
		iQuality	= 0;
	}
	mTexture.clear();
}

//***********************************************************************************************************************************************************************
//
//		GRAPH9 Function
//
//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::InitText()
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::InitText(FONT9 &_Font, const wchar_t *_FontName, int _Size, int _Quality){
	QuitText(_Font);
	if(NULL == (_Font.hNewFont = CreateFont(_Size, 0, 0, 0, FW_NORMAL, 0, 0, 0, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH | FF_MODERN, _FontName))){
		return (false);
	}
	_Font.iQuality	= max(TEXTLEVEL0, min(_Quality, TEXTLEVEL3));
	_Font.hContext	= GetDC(NULL);
	_Font.hOldFont	= SelectFont(_Font.hContext, _Font.hNewFont);
	SelectFont(_Font.hContext, _Font.hOldFont);
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::QuitText()
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::QuitText(FONT9 &_Font){
	if(_Font.hContext){
		SelectFont(_Font.hContext, _Font.hOldFont);
		DeleteFont(_Font.hNewFont);
		ReleaseDC(NULL, _Font.hContext);
		_Font.hContext	= NULL;
		_Font.hNewFont	= NULL;
		_Font.hOldFont	= NULL;
		_Font.iQuality	= 0;
	}
	_Font.mTexture.clear();
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::GetWidths()
//***********************************************************************************************************************************************************************
float owl::engine::GRAPH9::GetWidths(FONT9 &_Font, const wchar_t *_Text){
	owl::engine::CHAR9	Ch;
	float		Ln	= 0;
	for(UINT u = 0; u < wcslen(_Text); u++){
		if(FindChar(_Font, _Text[u], &Ch)){
			Ln += Ch.vXY.x;
		}
	}
	return Ln;
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::GetWidthf()
//***********************************************************************************************************************************************************************
float owl::engine::GRAPH9::GetWidthf(FONT9 &_Font, const wchar_t *_Text, ...){
	owl::engine::CHAR9	Ch;
	float		Ln	= 0;
	wchar_t		St[TEXTBUFFER];
	vswprintf_s(St, TEXTBUFFER - 1, _Text, reinterpret_cast<va_list>(&_Text + 1));
	St[TEXTBUFFER - 1]	= '\0';
	for(UINT u = 0; u < wcslen(St); u++){
		if(FindChar(_Font, St[u], &Ch)){
			Ln += Ch.vXY.x;
		}
	}
	return Ln;
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::DrawTexts()
//***********************************************************************************************************************************************************************
float owl::engine::GRAPH9::DrawTexts(FONT9 &_Font, const core::VECT3f &_Point, const core::COLORf &_ColorA, const wchar_t *_Text){
	owl::engine::CHAR9	Ch;
	float		Tx;
	float		Ty;
	float		Ln	= _Point.x;
	pDevice->SetFVF(FVF_TDVERTEX2D);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	for(UINT u = 0; u < wcslen(_Text); u++){
		if(FindChar(_Font, _Text[u], &Ch)){
			Tx	 = Ch.vXY.x;
			Ty	 = Ch.vXY.y;
			Ln	+= Tx;
			if(iswspace(_Text[u]) == 0){
				TDVERTEX2D	Vertex[] = {
					{ D3DXVECTOR4(Ln - Tx, _Point.y + Ty, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 1.0f) },
					{ D3DXVECTOR4(Ln - Tx, _Point.y     , _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Ln     , _Point.y + Ty, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 1.0f) },
					{ D3DXVECTOR4(Ln     , _Point.y     , _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 0.0f) }
				};
				pDevice->SetTexture(0, Ch.pTexture);
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, Vertex, sizeof(TDVERTEX2D));
			}
		}
	}
	pDevice->SetTexture(0, NULL);
	return (Ln - _Point.x);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::DrawTextf()
//***********************************************************************************************************************************************************************
float owl::engine::GRAPH9::DrawTextf(FONT9 &_Font, const core::VECT3f &_Point, const core::COLORf &_ColorA, const wchar_t *_Text, ...){
	wchar_t		St[TEXTBUFFER];
	owl::engine::CHAR9	Ch;
	float		Tx;
	float		Ty;
	float		Ln	= _Point.x;
	vswprintf_s(St, TEXTBUFFER, _Text, reinterpret_cast<va_list>(&_Text + 1));
	pDevice->SetFVF(FVF_TDVERTEX2D);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	for(UINT u = 0; u < wcslen(St); u++){
		if(FindChar(_Font, St[u], &Ch)){
			Tx	 = Ch.vXY.x;
			Ty	 = Ch.vXY.y;
			Ln	+= Tx;
			if(iswspace(St[u]) == 0){
				TDVERTEX2D	Vertex[] = {
					{ D3DXVECTOR4(Ln - Tx, _Point.y + Ty, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 1.0f) },
					{ D3DXVECTOR4(Ln - Tx, _Point.y     , _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Ln     , _Point.y + Ty, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 1.0f) },
					{ D3DXVECTOR4(Ln     , _Point.y     , _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 0.0f) }
				};
				pDevice->SetTexture(0, Ch.pTexture);
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, Vertex, sizeof(TDVERTEX2D));
			}
		}
	}
	pDevice->SetTexture(0, NULL);
	return (Ln - _Point.x);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::DrawTexts()
//***********************************************************************************************************************************************************************
float owl::engine::GRAPH9::DrawTexts(FONT9 &_Font, const core::VECT3f &_Point, const core::COLORf &_ColorA, const core::COLORf &_ColorB, const wchar_t *_Text){
	owl::engine::CHAR9	Ch;
	float				Tx;
	float				Ty;
	float				Ln	= _Point.x;
	
	pDevice->SetFVF(FVF_TDVERTEX2D);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	for(UINT u = 0; u < wcslen(_Text); u++){
		if(FindChar(_Font, _Text[u], &Ch)){
			Tx	 = Ch.vXY.x;
			Ty	 = Ch.vXY.y;
			Ln	+= Tx;
			if(iswspace(_Text[u]) == 0){
				TDVERTEX2D	Vertex[] = {
					//-----	1
					{ D3DXVECTOR4(Ln - Tx + 1, _Point.y + Ty + 1, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 1.0f) },
					{ D3DXVECTOR4(Ln - Tx + 1, _Point.y      + 1, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Ln      + 1, _Point.y + Ty + 1, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 1.0f) },
					//-----	2
					{ D3DXVECTOR4(Ln      + 1, _Point.y + Ty + 1, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 1.0f) },
					{ D3DXVECTOR4(Ln - Tx + 1, _Point.y      + 1, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Ln      + 1, _Point.y      + 1, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 0.0f) },
					//-----	3
					{ D3DXVECTOR4(Ln - Tx - 1, _Point.y + Ty - 1, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 1.0f) },
					{ D3DXVECTOR4(Ln - Tx - 1, _Point.y      - 1, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Ln      - 1, _Point.y + Ty - 1, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 1.0f) },
					//-----	4
					{ D3DXVECTOR4(Ln      - 1, _Point.y + Ty - 1, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 1.0f) },
					{ D3DXVECTOR4(Ln - Tx - 1, _Point.y      - 1, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Ln      - 1, _Point.y      - 1, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 0.0f) }
				};
				pDevice->SetTexture(0, Ch.pTexture);
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 4, Vertex, sizeof(TDVERTEX2D));
			}
		}
	}
	pDevice->SetTexture(0, NULL);
	return (Ln - _Point.x);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::DrawTextf()
//***********************************************************************************************************************************************************************
float owl::engine::GRAPH9::DrawTextf(FONT9 &_Font, const core::VECT3f &_Point, const core::COLORf &_ColorA, const core::COLORf &_ColorB, const wchar_t *_Text, ...){
	owl::engine::CHAR9	Ch;
	wchar_t				St[TEXTBUFFER];
	float				Tx;
	float				Ty;
	float				Ln	= _Point.x;
	
	vswprintf_s(St, TEXTBUFFER, _Text, reinterpret_cast<va_list>(&_Text + 1));
	pDevice->SetFVF(FVF_TDVERTEX2D);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	for(UINT u = 0; u < wcslen(St); u++){
		if(FindChar(_Font, St[u], &Ch)){
			Tx	 = Ch.vXY.x;
			Ty	 = Ch.vXY.y;
			Ln	+= Tx;
			if(iswspace(St[u]) == 0){
				TDVERTEX2D	Vertex[] = {
					//-----	1
					{ D3DXVECTOR4(Ln - Tx + 1, _Point.y + Ty + 1, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 1.0f) },
					{ D3DXVECTOR4(Ln - Tx + 1, _Point.y      + 1, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Ln      + 1, _Point.y + Ty + 1, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 1.0f) },
					//-----	2
					{ D3DXVECTOR4(Ln      + 1, _Point.y + Ty + 1, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 1.0f) },
					{ D3DXVECTOR4(Ln - Tx + 1, _Point.y      + 1, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Ln      + 1, _Point.y      + 1, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 0.0f) },
					//-----	3
					{ D3DXVECTOR4(Ln - Tx - 1, _Point.y + Ty - 1, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 1.0f) },
					{ D3DXVECTOR4(Ln - Tx - 1, _Point.y      - 1, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Ln      - 1, _Point.y + Ty - 1, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 1.0f) },
					//-----	4
					{ D3DXVECTOR4(Ln      - 1, _Point.y + Ty - 1, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 1.0f) },
					{ D3DXVECTOR4(Ln - Tx - 1, _Point.y      - 1, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Ln      - 1, _Point.y      - 1, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 0.0f) }
				};
				pDevice->SetTexture(0, Ch.pTexture);
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 4, Vertex, sizeof(TDVERTEX2D));
			}
		}
	}
	pDevice->SetTexture(0, NULL);
	return (Ln - _Point.x);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::DrawTexts()
//***********************************************************************************************************************************************************************
float owl::engine::GRAPH9::DrawTexts(FONT9 &_Font, const core::VECT3f &_Point, const core::COLORf &_ColorA, float _Shift, const wchar_t *_Text){
	owl::engine::CHAR9	Ch;
	float		Tx;
	float		Ty;
	float		Sv	= _Shift;
	float		Ln	= _Point.x;
	
	pDevice->SetFVF(FVF_TDVERTEX2D);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	for(UINT u = 0; u < wcslen(_Text); u++){
		if(FindChar(_Font, _Text[u], &Ch)){
			Tx	 = Ch.vXY.x;
			Ty	 = Ch.vXY.y;
			Ln	+= Tx;
			if(iswspace(_Text[u]) == 0){
				TDVERTEX2D	Vertex[] = {
					//-----	0
					{ D3DXVECTOR4(Ln - Tx, _Point.y + Ty, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 1.0f) },
					{ D3DXVECTOR4(Ln - Tx, _Point.y     , _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Ln     , _Point.y + Ty, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 1.0f) },
					//-----	1
					{ D3DXVECTOR4(Ln     , _Point.y + Ty, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 1.0f) },
					{ D3DXVECTOR4(Ln - Tx, _Point.y     , _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Ln     , _Point.y     , _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 0.0f) }
				};
				pDevice->SetTexture(0, Ch.pTexture);
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, Vertex, sizeof(TDVERTEX2D));
			}
		}
	}
	pDevice->SetTexture(0, NULL);
	return (Ln - _Point.x);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::DrawTextf()
//***********************************************************************************************************************************************************************
float owl::engine::GRAPH9::DrawTextf(FONT9 &_Font, const core::VECT3f &_Point, const core::COLORf &_ColorA, float _Shift, const wchar_t *_Text, ...){
	wchar_t		St[TEXTBUFFER];
	owl::engine::CHAR9	Ch;
	float		Tx;
	float		Ty;
	float		Sv	= _Shift;
	float		Ln	= _Point.x;
	
	vswprintf_s(St, TEXTBUFFER, _Text, reinterpret_cast<va_list>(&_Text + 1));
	pDevice->SetFVF(FVF_TDVERTEX2D);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	for(UINT u = 0; u < wcslen(St); u++){
		if(FindChar(_Font, St[u], &Ch)){
			Tx	 = Ch.vXY.x;
			Ty	 = Ch.vXY.y;
			Ln	+= Tx;
			if(iswspace(St[u]) == 0){
				TDVERTEX2D	Vertex[] = {
					//-----	0
					{ D3DXVECTOR4(Ln - Tx, _Point.y + Ty, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 1.0f) },
					{ D3DXVECTOR4(Ln - Tx, _Point.y     , _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Ln     , _Point.y + Ty, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 1.0f) },
					//-----	1
					{ D3DXVECTOR4(Ln     , _Point.y + Ty, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 1.0f) },
					{ D3DXVECTOR4(Ln - Tx, _Point.y     , _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Ln     , _Point.y     , _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 0.0f) }
				};
				pDevice->SetTexture(0, Ch.pTexture);
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 10, Vertex, sizeof(TDVERTEX2D));
			}
		}
	}
	pDevice->SetTexture(0, NULL);
	return (Ln - _Point.x);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::DrawTexts()
//***********************************************************************************************************************************************************************
float owl::engine::GRAPH9::DrawTexts(FONT9 &_Font, const core::VECT3f &_Point, const core::COLORf &_ColorA, const core::COLORf &_ColorB, float _Shift, const wchar_t *_Text){
	owl::engine::CHAR9	Ch;
	float		Tx;
	float		Ty;
	float		Sv	= _Shift;
	float		Ln	= _Point.x;
	
	pDevice->SetFVF(FVF_TDVERTEX2D);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	for(UINT u = 0; u < wcslen(_Text); u++){
		if(FindChar(_Font, _Text[u], &Ch)){
			Tx	 = Ch.vXY.x;
			Ty	 = Ch.vXY.y;
			Ln	+= Tx;
			if(iswspace(_Text[u]) == 0){
				TDVERTEX2D	Vertex[] = {
					//-----	(H-1)
					{ D3DXVECTOR4(Ln - Tx + Sv, _Point.y + Ty, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 1.0f) },
					{ D3DXVECTOR4(Ln - Tx + Sv, _Point.y     , _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Ln      + Sv, _Point.y + Ty, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 1.0f) },
					//-----	(H-2)
					{ D3DXVECTOR4(Ln      + Sv, _Point.y + Ty, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 1.0f) },
					{ D3DXVECTOR4(Ln - Tx + Sv, _Point.y     , _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Ln      + Sv, _Point.y     , _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 0.0f) },
					//-----	(H-3)
					{ D3DXVECTOR4(Ln - Tx - Sv, _Point.y + Ty, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 1.0f) },
					{ D3DXVECTOR4(Ln - Tx - Sv, _Point.y     , _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Ln      - Sv, _Point.y + Ty, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 1.0f) },
					//-----	(H-4)
					{ D3DXVECTOR4(Ln      - Sv, _Point.y + Ty, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 1.0f) },
					{ D3DXVECTOR4(Ln - Tx - Sv, _Point.y     , _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Ln      - Sv, _Point.y     , _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 0.0f) },
					//-----	(H-5)
					{ D3DXVECTOR4(Ln - Tx, _Point.y + Ty + Sv, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 1.0f) },
					{ D3DXVECTOR4(Ln - Tx, _Point.y      + Sv, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Ln     , _Point.y + Ty + Sv, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 1.0f) },
					//-----	(H-6)
					{ D3DXVECTOR4(Ln     , _Point.y + Ty + Sv, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 1.0f) },
					{ D3DXVECTOR4(Ln - Tx, _Point.y      + Sv, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Ln     , _Point.y      + Sv, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 0.0f) },
					//-----	(H-7)
					{ D3DXVECTOR4(Ln - Tx, _Point.y + Ty - Sv, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 1.0f) },
					{ D3DXVECTOR4(Ln - Tx, _Point.y      - Sv, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Ln     , _Point.y + Ty - Sv, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 1.0f) },
					//-----	(H-8)
					{ D3DXVECTOR4(Ln     , _Point.y + Ty - Sv, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 1.0f) },
					{ D3DXVECTOR4(Ln - Tx, _Point.y      - Sv, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Ln     , _Point.y      - Sv, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 0.0f) },
					//-----	0
					{ D3DXVECTOR4(Ln - Tx, _Point.y + Ty, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 1.0f) },
					{ D3DXVECTOR4(Ln - Tx, _Point.y     , _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Ln     , _Point.y + Ty, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 1.0f) },
					//-----	1
					{ D3DXVECTOR4(Ln     , _Point.y + Ty, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 1.0f) },
					{ D3DXVECTOR4(Ln - Tx, _Point.y     , _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Ln     , _Point.y     , _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 0.0f) }
				};
				pDevice->SetTexture(0, Ch.pTexture);
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 10, Vertex, sizeof(TDVERTEX2D));
			}
		}
	}
	pDevice->SetTexture(0, NULL);
	return (Ln - _Point.x);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::DrawTextf()
//***********************************************************************************************************************************************************************
float owl::engine::GRAPH9::DrawTextf(FONT9 &_Font, const core::VECT3f &_Point, const core::COLORf &_ColorA, const core::COLORf &_ColorB, float _Shift, const wchar_t *_Text, ...){
	wchar_t		St[TEXTBUFFER];
	owl::engine::CHAR9	Ch;
	float		Tx;
	float		Ty;
	float		Sv	= _Shift;
	float		Ln	= _Point.x;
	
	vswprintf_s(St, TEXTBUFFER, _Text, reinterpret_cast<va_list>(&_Text + 1));
	pDevice->SetFVF(FVF_TDVERTEX2D);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	for(UINT u = 0; u < wcslen(St); u++){
		if(FindChar(_Font, St[u], &Ch)){
			Tx	 = Ch.vXY.x;
			Ty	 = Ch.vXY.y;
			Ln	+= Tx;
			if(iswspace(St[u]) == 0){
				TDVERTEX2D	Vertex[] = {
					//-----	(H-1)
					{ D3DXVECTOR4(Ln - Tx + Sv, _Point.y + Ty, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 1.0f) },
					{ D3DXVECTOR4(Ln - Tx + Sv, _Point.y     , _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Ln      + Sv, _Point.y + Ty, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 1.0f) },
					//-----	(H-2)
					{ D3DXVECTOR4(Ln      + Sv, _Point.y + Ty, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 1.0f) },
					{ D3DXVECTOR4(Ln - Tx + Sv, _Point.y     , _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Ln      + Sv, _Point.y     , _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 0.0f) },
					//-----	(H-3)
					{ D3DXVECTOR4(Ln - Tx - Sv, _Point.y + Ty, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 1.0f) },
					{ D3DXVECTOR4(Ln - Tx - Sv, _Point.y     , _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Ln      - Sv, _Point.y + Ty, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 1.0f) },
					//-----	(H-4)
					{ D3DXVECTOR4(Ln      - Sv, _Point.y + Ty, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 1.0f) },
					{ D3DXVECTOR4(Ln - Tx - Sv, _Point.y     , _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Ln      - Sv, _Point.y     , _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 0.0f) },
					//-----	(H-5)
					{ D3DXVECTOR4(Ln - Tx, _Point.y + Ty + Sv, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 1.0f) },
					{ D3DXVECTOR4(Ln - Tx, _Point.y      + Sv, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Ln     , _Point.y + Ty + Sv, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 1.0f) },
					//-----	(H-6)
					{ D3DXVECTOR4(Ln     , _Point.y + Ty + Sv, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 1.0f) },
					{ D3DXVECTOR4(Ln - Tx, _Point.y      + Sv, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Ln     , _Point.y      + Sv, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 0.0f) },
					//-----	(H-7)
					{ D3DXVECTOR4(Ln - Tx, _Point.y + Ty - Sv, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 1.0f) },
					{ D3DXVECTOR4(Ln - Tx, _Point.y      - Sv, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Ln     , _Point.y + Ty - Sv, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 1.0f) },
					//-----	(H-8)
					{ D3DXVECTOR4(Ln     , _Point.y + Ty - Sv, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 1.0f) },
					{ D3DXVECTOR4(Ln - Tx, _Point.y      - Sv, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Ln     , _Point.y      - Sv, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 0.0f) },
					//-----	0
					{ D3DXVECTOR4(Ln - Tx, _Point.y + Ty, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 1.0f) },
					{ D3DXVECTOR4(Ln - Tx, _Point.y     , _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Ln     , _Point.y + Ty, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 1.0f) },
					//-----	1
					{ D3DXVECTOR4(Ln     , _Point.y + Ty, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 1.0f) },
					{ D3DXVECTOR4(Ln - Tx, _Point.y     , _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Ln     , _Point.y     , _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 0.0f) }
				};
				pDevice->SetTexture(0, Ch.pTexture);
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 10, Vertex, sizeof(TDVERTEX2D));
			}
		}
	}
	pDevice->SetTexture(0, NULL);
	return (Ln - _Point.x);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::DrawTexts()
//***********************************************************************************************************************************************************************
float owl::engine::GRAPH9::DrawTexts(FONT9 &_Font, const core::VECT3f &_Point, const core::COLORf &_ColorA, const core::VECT2f &_Scale, const wchar_t *_Text){
	owl::engine::CHAR9	Ch;
	float		Tx;
	float		Ty;
	float		Ln	= _Point.x;
	float		Px;
	float		Py;
	float		Sx;
	float		Sy;
	pDevice->SetFVF(FVF_TDVERTEX2D);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	for(UINT u = 0; u < wcslen(_Text); u++){
		if(FindChar(_Font, _Text[u], &Ch)){
			Tx	= Ch.vXY.x;
			Ty	= Ch.vXY.y;
			Px	= 0.5f * Tx + Ln;
			Py	= 0.5f * Ty + _Point.y;
			Sx	= 0.5f * _Scale.x * Tx;
			Sy	= 0.5f * _Scale.y * Ty;
			if(iswspace(_Text[u]) == 0){
				TDVERTEX2D	Vertex[] = {
					{ D3DXVECTOR4(Px - Sx, Py + Sy, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 1.0f) },
					{ D3DXVECTOR4(Px - Sx, Py - Sy, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Px + Sx, Py + Sy, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 1.0f) },
					{ D3DXVECTOR4(Px + Sx, Py - Sy, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 0.0f) }
				};
				pDevice->SetTexture(0, Ch.pTexture);
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, Vertex, sizeof(TDVERTEX2D));
			}
			Ln	+= Tx;
		}
	}
	pDevice->SetTexture(0, NULL);
	return (Ln - _Point.x);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::DrawTextf()
//***********************************************************************************************************************************************************************
float owl::engine::GRAPH9::DrawTextf(FONT9 &_Font, const core::VECT3f &_Point, const core::COLORf &_ColorA, const core::VECT2f &_Scale, const wchar_t *_Text, ...){
	wchar_t		St[TEXTBUFFER];
	owl::engine::CHAR9	Ch;
	float		Tx;
	float		Ty;
	float		Ln	= _Point.x;
	float		Px;
	float		Py;
	float		Sx;
	float		Sy;
	vswprintf_s(St, TEXTBUFFER, _Text, reinterpret_cast<va_list>(&_Text + 1));
	pDevice->SetFVF(FVF_TDVERTEX2D);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	for(UINT u = 0; u < wcslen(St); u++){
		if(FindChar(_Font, St[u], &Ch)){
			Tx	= Ch.vXY.x;
			Ty	= Ch.vXY.y;
			Px	= 0.5f * Tx + Ln;
			Py	= 0.5f * Ty + _Point.y;
			Sx	= 0.5f * _Scale.x * Tx;
			Sy	= 0.5f * _Scale.y * Ty;
			if(iswspace(St[u]) == 0){
				TDVERTEX2D	Vertex[] = {
					{ D3DXVECTOR4(Px - Sx, Py + Sy, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 1.0f) },
					{ D3DXVECTOR4(Px - Sx, Py - Sy, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Px + Sx, Py + Sy, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 1.0f) },
					{ D3DXVECTOR4(Px + Sx, Py - Sy, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 0.0f) }
				};
				pDevice->SetTexture(0, Ch.pTexture);
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, Vertex, sizeof(TDVERTEX2D));
			}
			Ln	+= Tx;
		}
	}
	pDevice->SetTexture(0, NULL);
	return (Ln - _Point.x);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::DrawTexts()
//***********************************************************************************************************************************************************************
float owl::engine::GRAPH9::DrawTexts(FONT9 &_Font, const core::VECT3f &_Point, const core::COLORf &_ColorA, const core::COLORf &_ColorB, const core::VECT2f &_Scale, const wchar_t *_Text){
	owl::engine::CHAR9	Ch;
	float				Tx;
	float				Ty;
	float				Ln	= _Point.x;
	float				Px;
	float				Py;
	float				Sx;
	float				Sy;
	pDevice->SetFVF(FVF_TDVERTEX2D);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	for(UINT u = 0; u < wcslen(_Text); u++){
		if(FindChar(_Font, _Text[u], &Ch)){
			Tx	= Ch.vXY.x;
			Ty	= Ch.vXY.y;
			Px	= 0.5f * Tx + Ln;
			Py	= 0.5f * Ty + _Point.y;
			Sx	= 0.5f * _Scale.x * Tx;
			Sy	= 0.5f * _Scale.y * Ty;
			if(iswspace(_Text[u]) == 0){
				TDVERTEX2D	Vertex[] = {
					//-----	1
					{ D3DXVECTOR4(Px - Sx + 1, Py + Sy + 1, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 1.0f) },
					{ D3DXVECTOR4(Px - Sx + 1, Py - Sy + 1, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Px + Sx + 1, Py + Sy + 1, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 1.0f) },
					//-----	2
					{ D3DXVECTOR4(Px + Sx + 1, Py + Sy + 1, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 1.0f) },
					{ D3DXVECTOR4(Px - Sx + 1, Py - Sy + 1, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Px + Sx + 1, Py - Sy + 1, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 0.0f) },
					//-----	3
					{ D3DXVECTOR4(Px - Sx - 1, Py + Sy - 1, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 1.0f) },
					{ D3DXVECTOR4(Px - Sx - 1, Py - Sy - 1, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Px + Sx - 1, Py + Sy - 1, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 1.0f) },
					//-----	4
					{ D3DXVECTOR4(Px + Sx - 1, Py + Sy - 1, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 1.0f) },
					{ D3DXVECTOR4(Px - Sx - 1, Py - Sy - 1, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Px + Sx - 1, Py - Sy - 1, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 0.0f) },
				};
				pDevice->SetTexture(0, Ch.pTexture);
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 4, Vertex, sizeof(TDVERTEX2D));
			}
			Ln	+= Tx;
		}
	}
	pDevice->SetTexture(0, NULL);
	return (Ln - _Point.x);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::DrawTextf()
//***********************************************************************************************************************************************************************
float owl::engine::GRAPH9::DrawTextf(FONT9 &_Font, const core::VECT3f &_Point, const core::COLORf &_ColorA, const core::COLORf &_ColorB, const core::VECT2f &_Scale, const wchar_t *_Text, ...){
	owl::engine::CHAR9	Ch;
	wchar_t				St[TEXTBUFFER];
	float				Tx;
	float				Ty;
	float				Ln	= _Point.x;
	float				Px;
	float				Py;
	float				Sx;
	float				Sy;
	vswprintf_s(St, TEXTBUFFER, _Text, reinterpret_cast<va_list>(&_Text + 1));
	pDevice->SetFVF(FVF_TDVERTEX2D);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	for(UINT u = 0; u < wcslen(St); u++){
		if(FindChar(_Font, St[u], &Ch)){
			Tx	= Ch.vXY.x;
			Ty	= Ch.vXY.y;
			Px	= 0.5f * Tx + Ln;
			Py	= 0.5f * Ty + _Point.y;
			Sx	= 0.5f * _Scale.x * Tx;
			Sy	= 0.5f * _Scale.y * Ty;
			if(iswspace(St[u]) == 0){
				TDVERTEX2D	Vertex[] = {
					//-----	1
					{ D3DXVECTOR4(Px - Sx + 1, Py + Sy + 1, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 1.0f) },
					{ D3DXVECTOR4(Px - Sx + 1, Py - Sy + 1, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Px + Sx + 1, Py + Sy + 1, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 1.0f) },
					//-----	2
					{ D3DXVECTOR4(Px + Sx + 1, Py + Sy + 1, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 1.0f) },
					{ D3DXVECTOR4(Px - Sx + 1, Py - Sy + 1, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Px + Sx + 1, Py - Sy + 1, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 0.0f) },
					//-----	3
					{ D3DXVECTOR4(Px - Sx - 1, Py + Sy - 1, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 1.0f) },
					{ D3DXVECTOR4(Px - Sx - 1, Py - Sy - 1, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Px + Sx - 1, Py + Sy - 1, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 1.0f) },
					//-----	4
					{ D3DXVECTOR4(Px + Sx - 1, Py + Sy - 1, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 1.0f) },
					{ D3DXVECTOR4(Px - Sx - 1, Py - Sy - 1, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Px + Sx - 1, Py - Sy - 1, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 0.0f) },
				};
				pDevice->SetTexture(0, Ch.pTexture);
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 4, Vertex, sizeof(TDVERTEX2D));
			}
			Ln	+= Tx;
		}
	}
	pDevice->SetTexture(0, NULL);
	return (Ln - _Point.x);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::DrawRectTexts()
//***********************************************************************************************************************************************************************
owl::core::VECT2f owl::engine::GRAPH9::DrawTexts(FONT9 &_Font, const core::VECT3f &_Point, const core::VECT2f &_Size, float _Pitch, const core::COLORf &_ColorA, const wchar_t *_Text){
	owl::engine::CHAR9	Ch;
	float		Tx;
	float		Ty;
	float		Ox	= _Point.x;
	float		Lx	= _Point.x;
	float		Ly	= _Point.y;
	pDevice->SetFVF(FVF_TDVERTEX2D);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	for(UINT u = 0; u < wcslen(_Text); u++){
		if(FindChar(_Font, _Text[u], &Ch)){
			Tx	 = Ch.vXY.x;
			Ty	 = Ch.vXY.y;
			Lx	+= Tx;
			if(Lx > Ox + _Size.x){
				Lx	 = Ox + Tx;
				Ly	+= _Pitch;
				if(Ly > _Point.y + _Size.y){
					break;
				}
			}
			if(iswspace(_Text[u]) == 0){
				TDVERTEX2D	Vertex[] = {
					{ D3DXVECTOR4(Lx - Tx, Ly + Ty, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 1.0f) },
					{ D3DXVECTOR4(Lx - Tx, Ly     , _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Lx     , Ly + Ty, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 1.0f) },
					{ D3DXVECTOR4(Lx     , Ly     , _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 0.0f) }
				};
				pDevice->SetTexture(0, Ch.pTexture);
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, Vertex, sizeof(TDVERTEX2D));
			}else if(_Text[u] == '\n'){
				Lx	 = Ox;
				Ly	+= _Pitch;
				if(Ly > _Point.y + _Size.y){
					break;
				}
			}
		}
	}
	pDevice->SetTexture(0, NULL);
	return owl::core::VECT2f(Lx - _Point.x, Ly - _Point.y);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::DrawTexts()
//***********************************************************************************************************************************************************************
owl::core::VECT2f owl::engine::GRAPH9::DrawTexts(FONT9 &_Font, const core::VECT3f &_Point, const core::VECT2f &_Size, float _Pitch, const core::COLORf &_ColorA, const core::COLORf &_ColorB, const wchar_t *_Text){
	owl::engine::CHAR9	Ch;
	float		Tx;
	float		Ty;
	float		Ox	= _Point.x;
	float		Lx	= _Point.x;
	float		Ly	= _Point.y;
	pDevice->SetFVF(FVF_TDVERTEX2D);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	for(UINT u = 0; u < wcslen(_Text); u++){
		if(FindChar(_Font, _Text[u], &Ch)){
			Tx	 = Ch.vXY.x;
			Ty	 = Ch.vXY.y;
			Lx	+= Tx;
			if(Lx > Ox + _Size.x){
				Lx	 = Ox + Tx;
				Ly	+= _Pitch;
				if(Ly > _Point.y + _Size.y){
					break;
				}
			}
			if(iswspace(_Text[u]) == 0){
				TDVERTEX2D	Vertex[] = {
					//-----	1
					{ D3DXVECTOR4(Lx - Tx + 1, Ly + Ty + 1, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 1.0f) },
					{ D3DXVECTOR4(Lx - Tx + 1, Ly      + 1, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Lx      + 1, Ly + Ty + 1, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 1.0f) },
					//-----	2
					{ D3DXVECTOR4(Lx      + 1, Ly + Ty + 1, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 1.0f) },
					{ D3DXVECTOR4(Lx - Tx + 1, Ly      + 1, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Lx      + 1, Ly      + 1, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 0.0f) },
					//-----	3
					{ D3DXVECTOR4(Lx - Tx - 1, Ly + Ty - 1, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 1.0f) },
					{ D3DXVECTOR4(Lx - Tx - 1, Ly      - 1, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Lx      - 1, Ly + Ty - 1, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 1.0f) },
					//-----	4
					{ D3DXVECTOR4(Lx      - 1, Ly + Ty - 1, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 1.0f) },
					{ D3DXVECTOR4(Lx - Tx - 1, Ly      - 1, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Lx      - 1, Ly      - 1, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 0.0f) }
				};
				pDevice->SetTexture(0, Ch.pTexture);
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 4, Vertex, sizeof(TDVERTEX2D));
			}else if(_Text[u] == '\n'){
				Lx	 = Ox;
				Ly	+= _Pitch;
				if(Ly > _Point.y + _Size.y){
					break;
				}
			}
		}
	}
	pDevice->SetTexture(0, NULL);
	return owl::core::VECT2f(Lx - _Point.x, Ly - _Point.y);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::DrawRectTextf()
//***********************************************************************************************************************************************************************
owl::core::VECT2f owl::engine::GRAPH9::DrawTextf(FONT9 &_Font, const core::VECT3f &_Point, const core::VECT2f &_Size, float _Pitch, const core::COLORf &_ColorA, const wchar_t *_Text, ...){
	wchar_t		St[TEXTBUFFER];
	owl::engine::CHAR9	Ch;
	float		Tx;
	float		Ty;
	float		Ox	= _Point.x;
	float		Lx	= _Point.x;
	float		Ly	= _Point.y;
	vswprintf_s(St, TEXTBUFFER, _Text, reinterpret_cast<va_list>(&_Text + 1));
	pDevice->SetFVF(FVF_TDVERTEX2D);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	for(UINT u = 0; u < wcslen(St); u++){
		if(FindChar(_Font, St[u], &Ch)){
			Tx	 = Ch.vXY.x;
			Ty	 = Ch.vXY.y;
			Lx	+= Tx;
			if(Lx > Ox + _Size.x){
				Lx	 = Ox + Tx;
				Ly	+= _Pitch;
				if(Ly > _Point.y + _Size.y){
					break;
				}
			}
			if(iswspace(St[u]) == 0){
				TDVERTEX2D	Vertex[] = {
					{ D3DXVECTOR4(Lx - Tx, Ly + Ty, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 1.0f) },
					{ D3DXVECTOR4(Lx - Tx, Ly     , _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Lx     , Ly + Ty, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 1.0f) },
					{ D3DXVECTOR4(Lx     , Ly     , _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 0.0f) }
				};
				pDevice->SetTexture(0, Ch.pTexture);
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, Vertex, sizeof(TDVERTEX2D));
			}else if(_Text[u] == '\n'){
				Lx	 = Ox;
				Ly	+= _Pitch;
				if(Ly > _Point.y + _Size.y){
					break;
				}
			}
		}
	}
	pDevice->SetTexture(0, NULL);
	return owl::core::VECT2f(Lx - _Point.x, Ly - _Point.y);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::DrawTextf()
//***********************************************************************************************************************************************************************
owl::core::VECT2f owl::engine::GRAPH9::DrawTextf(FONT9 &_Font, const core::VECT3f &_Point, const core::VECT2f &_Size, float _Pitch, const core::COLORf &_ColorA, const core::COLORf &_ColorB, const wchar_t *_Text, ...){
	wchar_t		St[TEXTBUFFER];
	owl::engine::CHAR9	Ch;
	float		Tx;
	float		Ty;
	float		Ox	= _Point.x;
	float		Lx	= _Point.x;
	float		Ly	= _Point.y;
	vswprintf_s(St, TEXTBUFFER, _Text, reinterpret_cast<va_list>(&_Text + 1));
	pDevice->SetFVF(FVF_TDVERTEX2D);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	for(UINT u = 0; u < wcslen(St); u++){
		if(FindChar(_Font, St[u], &Ch)){
			Tx	 = Ch.vXY.x;
			Ty	 = Ch.vXY.y;
			Lx	+= Tx;
			if(Lx > Ox + _Size.x){
				Lx	 = Ox + Tx;
				Ly	+= _Pitch;
				if(Ly > _Point.y + _Size.y){
					break;
				}
			}
			if(iswspace(St[u]) == 0){
				TDVERTEX2D	Vertex[] = {
					//-----	1
					{ D3DXVECTOR4(Lx - Tx + 1, Ly + Ty + 1, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 1.0f) },
					{ D3DXVECTOR4(Lx - Tx + 1, Ly      + 1, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Lx      + 1, Ly + Ty + 1, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 1.0f) },
					//-----	2
					{ D3DXVECTOR4(Lx      + 1, Ly + Ty + 1, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 1.0f) },
					{ D3DXVECTOR4(Lx - Tx + 1, Ly      + 1, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Lx      + 1, Ly      + 1, _Point.z, 1.0f), TrueColor32(_ColorB), D3DXVECTOR2(1.0f, 0.0f) },
					//-----	3
					{ D3DXVECTOR4(Lx - Tx - 1, Ly + Ty - 1, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 1.0f) },
					{ D3DXVECTOR4(Lx - Tx - 1, Ly      - 1, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Lx      - 1, Ly + Ty - 1, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 1.0f) },
					//-----	4
					{ D3DXVECTOR4(Lx      - 1, Ly + Ty - 1, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 1.0f) },
					{ D3DXVECTOR4(Lx - Tx - 1, Ly      - 1, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(0.0f, 0.0f) },
					{ D3DXVECTOR4(Lx      - 1, Ly      - 1, _Point.z, 1.0f), TrueColor32(_ColorA), D3DXVECTOR2(1.0f, 0.0f) }
				};
				pDevice->SetTexture(0, Ch.pTexture);
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 4, Vertex, sizeof(TDVERTEX2D));
			}else if(_Text[u] == '\n'){
				Lx	 = Ox;
				Ly	+= _Pitch;
				if(Ly > _Point.y + _Size.y){
					break;
				}
			}
		}
	}
	pDevice->SetTexture(0, NULL);
	return owl::core::VECT2f(Lx - _Point.x, Ly - _Point.y);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::FindChar()
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::FindChar(FONT9 &_Font, wchar_t _Char, owl::engine::CHAR9 *_Tex){
	owl::engine::CHAR9	Ch;
	TEXTMETRIC			TM;
	GLYPHMETRICS		GM;
	D3DLOCKED_RECT		nLock;
	DWORD				nSize;
	BYTE *				pBit	= NULL;
	const MAT2			mMat	= { { 0, 1 }, { 0, 0 }, { 0, 0 }, { 0, 1 } };
	MAPCHARITERATOR		pIte	= _Font.mTexture.find(_Char);

	if(pIte != _Font.mTexture.end()){
		*_Tex	= pIte->second;
		return true;
	}
	SelectFont(_Font.hContext, _Font.hNewFont);
	if(GetTextMetricsW(_Font.hContext, &TM) == 0){
		SelectFont(_Font.hContext, _Font.hOldFont);
		return false;
	}
	switch(_Font.iQuality){
		case (TEXTLEVEL3):	nSize = GetGlyphOutline(_Font.hContext, _Char, GGO_GRAY8_BITMAP, &GM, 0, NULL, &mMat);	break;
		case (TEXTLEVEL2):	nSize = GetGlyphOutline(_Font.hContext, _Char, GGO_GRAY4_BITMAP, &GM, 0, NULL, &mMat);	break;
		case (TEXTLEVEL1):	nSize = GetGlyphOutline(_Font.hContext, _Char, GGO_GRAY2_BITMAP, &GM, 0, NULL, &mMat);	break;
		default:			nSize = GetGlyphOutline(_Font.hContext, _Char, GGO_BITMAP      , &GM, 0, NULL, &mMat);	break;
	}
	if(nSize == GDI_ERROR){
		return false;
	}
	int				iOfsX	= GM.gmptGlyphOrigin.x;
	int				iOfsY	= TM.tmHeight - (TM.tmDescent + GM.gmptGlyphOrigin.y);
	int				iBmpW	= GM.gmBlackBoxX + (4 - (GM.gmBlackBoxX % 4)) % 4;
	int				iBmpH	= GM.gmBlackBoxY;
	int				iX;
	int				iY;
	unsigned int	nAlpha;
	unsigned int	nColor;
	if(static_cast<DWORD>((iOfsX + iBmpW) * (iOfsY + iBmpH)) > nSize){
		nSize = static_cast<DWORD>((iOfsX + iBmpW) * (iOfsY + iBmpH));
	}
	try{
		pBit = new BYTE[nSize];
	}catch(std::bad_alloc){
		SelectFont(_Font.hContext, _Font.hOldFont);
		return false;
	}
	switch(_Font.iQuality){
		case (TEXTLEVEL3):	GetGlyphOutline(_Font.hContext, _Char, GGO_GRAY8_BITMAP, &GM, nSize, pBit, &mMat);	break;
		case (TEXTLEVEL2):	GetGlyphOutline(_Font.hContext, _Char, GGO_GRAY4_BITMAP, &GM, nSize, pBit, &mMat);	break;
		case (TEXTLEVEL1):	GetGlyphOutline(_Font.hContext, _Char, GGO_GRAY2_BITMAP, &GM, nSize, pBit, &mMat);	break;
		default:			GetGlyphOutline(_Font.hContext, _Char, GGO_BITMAP      , &GM, nSize, pBit, &mMat);	break;
	}
	if(FAILED(pDevice->CreateTexture(GM.gmCellIncX + 2, TM.tmHeight + 2, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &Ch.pTexture, NULL))){
		SelectFont(_Font.hContext, _Font.hOldFont);
		delete [] pBit;
		return false;
	}
	switch(_Font.iQuality){
		case (TEXTLEVEL3):
			if(FAILED(Ch.pTexture->LockRect(0, &nLock, NULL, D3DLOCK_DISCARD))){
				SafeRelease(Ch.pTexture);
				SelectFont(_Font.hContext, _Font.hOldFont);
				delete [] pBit;
				return false;
			}
			FillMemory(nLock.pBits, 4 * (GM.gmCellIncX + 2) * (TM.tmHeight + 2), 0x00);
			for(iY = iOfsY; iY < iOfsY + iBmpH; iY++){
				for(iX = iOfsX; iX < iOfsX + iBmpW; iX++){
					nAlpha	= static_cast<unsigned int>((256 * pBit[iX - iOfsX + iBmpW * (iY - iOfsY)]) >> 6);
					nAlpha	= (0x000000FF < nAlpha) ? (0x000000FF) : (nAlpha);
					nColor	= 0x00FFFFFF | (nAlpha << 24);
					memcpy(reinterpret_cast<BYTE *>(nLock.pBits) + nLock.Pitch * iY + 4 * iX, &nColor, sizeof(DWORD));
				}
			}
			Ch.pTexture->UnlockRect(0);
			break;
		case (TEXTLEVEL2):
			if(FAILED(Ch.pTexture->LockRect(0, &nLock, NULL, D3DLOCK_DISCARD))){
				SafeRelease(Ch.pTexture);
				SelectFont(_Font.hContext, _Font.hOldFont);
				delete [] pBit;
				return false;
			}
			FillMemory(nLock.pBits, 4 * (GM.gmCellIncX + 2) * (TM.tmHeight + 2), 0x00);
			for(iY = iOfsY; iY < iOfsY + iBmpH; iY++){
				for(iX = iOfsX; iX < iOfsX + iBmpW; iX++){
					nAlpha	= static_cast<unsigned int>((256 * pBit[iX - iOfsX + iBmpW * (iY - iOfsY)]) >> 4);
					nAlpha	= (0x000000FF < nAlpha) ? (0x000000FF) : (nAlpha);
					nColor	= 0x00FFFFFF | (nAlpha << 24);
					memcpy(reinterpret_cast<BYTE *>(nLock.pBits) + nLock.Pitch * iY + 4 * iX, &nColor, sizeof(DWORD));
				}
			}
			Ch.pTexture->UnlockRect(0);
			break;
		case (TEXTLEVEL1):
			if(FAILED(Ch.pTexture->LockRect(0, &nLock, NULL, D3DLOCK_DISCARD))){
				SafeRelease(Ch.pTexture);
				SelectFont(_Font.hContext, _Font.hOldFont);
				delete [] pBit;
				return false;
			}
			FillMemory(nLock.pBits, 4 * (GM.gmCellIncX + 2) * (TM.tmHeight + 2), 0x00);
			for(iY = iOfsY; iY < iOfsY + iBmpH; iY++){
				for(iX = iOfsX; iX < iOfsX + iBmpW; iX++){
					nAlpha	= static_cast<unsigned int>((256 * pBit[iX - iOfsX + iBmpW * (iY - iOfsY)]) >> 2);
					nAlpha	= (0x000000FF < nAlpha) ? (0x000000FF) : (nAlpha);
					nColor	= 0x00FFFFFF | (nAlpha << 24);
					memcpy(reinterpret_cast<BYTE *>(nLock.pBits) + nLock.Pitch * iY + 4 * iX, &nColor, sizeof(DWORD));
				}
			}
			Ch.pTexture->UnlockRect(0);
			break;
		default:
			unsigned int	nPos;
			BYTE			cBit, cSet, cMask;
			if(FAILED(Ch.pTexture->LockRect(0, &nLock, NULL, D3DLOCK_DISCARD))){
				SafeRelease(Ch.pTexture);
				SelectFont(_Font.hContext, _Font.hOldFont);
				delete [] pBit;
				return false;
			}
			FillMemory(nLock.pBits, 4 * (GM.gmCellIncX + 2) * (TM.tmHeight + 2), 0x00);
			for(iY = iOfsY; iY < iOfsY + iBmpH; iY++){
				for(iX = iOfsX; iX < iOfsX + iBmpW; iX++){
					nPos	 = (iX - iOfsX) >> 3;
					cBit	 = static_cast<BYTE>((iX - iOfsX) % 8);
					cMask	 = static_cast<BYTE>(0x01) << (7 - cBit);
					cSet	 = *(static_cast<BYTE *>(pBit) + ((1 + (GM.gmBlackBoxX >> 5)) << 2) * (iY - iOfsY) + nPos);
					cSet	&= cMask;
					nAlpha	 = static_cast<unsigned int>((cSet >> (7 - cBit)) ? (255) : (0)); 
					nColor	 = 0x00FFFFFF | (nAlpha << 24);
					memcpy(reinterpret_cast<BYTE *>(nLock.pBits) + nLock.Pitch * iY + 4 * iX, &nColor, sizeof(DWORD));
				}
			}
			Ch.pTexture->UnlockRect(0);
			break;
	}
	delete	[] pBit;
	Ch.vXY.x		= static_cast<float>(GM.gmCellIncX + 2);
	Ch.vXY.y		= static_cast<float>(TM.tmHeight   + 2);
	_Font.mTexture.insert(MAPCHARPAIR(_Char, Ch));
	*_Tex		= Ch;
	SelectFont(_Font.hContext, _Font.hOldFont);
	return true;
}
