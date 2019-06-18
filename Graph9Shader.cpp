#include"owlEngine9.hpp"

//***********************************************************************************************************************************************************************
//
//		Class Constructor / Destructor
//
//***********************************************************************************************************************************************************************
//		SHADER9::SHADER9()
//***********************************************************************************************************************************************************************
owl::engine::SHADER9::SHADER9(){
	pShader		= NULL;
}

//***********************************************************************************************************************************************************************
//		SHADER9::~SHADER9()
//***********************************************************************************************************************************************************************
owl::engine::SHADER9::~SHADER9(){
	//-----	no Action
}

//***********************************************************************************************************************************************************************
//
//		GRAPH9 Function
//
//***********************************************************************************************************************************************************************
//		GRAPH9::InitShader(SHADER9 &)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::InitShader(SHADER9 &_Shader, const wchar_t *_FileName){
	HRESULT			hRet;
	ID3DXBuffer	*	pError	= NULL;
	if(FAILED(hRet = D3DXCreateEffectFromFile(pDevice, _FileName, NULL, NULL, 0, NULL, &_Shader.pShader, &pError))){
		#ifdef	_DEBUG
			FILE *	f;
			char *	p	= reinterpret_cast<char *>(pError->GetBufferPointer());
			fopen_s(&f, "ShaderCompileError.txt", "w");
			if(NULL != f){
				fprintf(f, "%s > %s\n", _FileName, p);
				fclose(f);
			}
		#endif
		SafeRelease(pError);
		return (false);
	}
	SafeRelease(pError);
	//-----	エントリ
	try{
		cListShader.push_back(&_Shader);
	}catch(...){
		SafeRelease(_Shader.pShader);
		return (false);
	}
	return (true);
}

//***********************************************************************************************************************************************************************
//		GRAPH9::InitShader(SHADER9 &)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::InitShader(SHADER9 &_Shader, const unsigned char *_Stream, unsigned int _Count){
	HRESULT			hRet;
	ID3DXBuffer	*	pError	= NULL;
	if(FAILED(hRet = D3DXCreateEffect(pDevice, _Stream, _Count, NULL, NULL, 0, NULL, &_Shader.pShader, &pError))){
		#ifdef	_DEBUG
			FILE *	f;
			char *	p	= reinterpret_cast<char *>(pError->GetBufferPointer());
			fopen_s(&f, "ShaderCompileError.txt", "w");
			if(NULL != f){
				fprintf(f, "%s > %s\n", "inline file", p);
				fclose(f);
			}
		#endif
		SafeRelease(pError);
		return (false);
	}
	//-----	エントリ
	try{
		cListShader.push_back(&_Shader);
	}catch(...){
		SafeRelease(_Shader.pShader);
		return (false);
	}
	return (true);
}

//***********************************************************************************************************************************************************************
//		GRAPH9::QuitShader(SHADER9 &)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::QuitShader(SHADER9 &_Shader){
	//-----	エントリから消去
	for(LISTSHADERITERATOR p = cListShader.begin(); p != cListShader.end(); p++){
		if((*p) == &(_Shader)){
			cListShader.erase(p);
			break;
		}
	}
	//-----	データ消去
	SafeRelease(_Shader.pShader);
	return (true);
}

//***********************************************************************************************************************************************************************
//		GRAPH9::SetSection(SHADER9 &)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::SetSection(SHADER9 &_Shader, unsigned int _Pass){
	UINT	nPass;
	if(_Shader.pShader == NULL){
		return (false);
	}
	_Shader.pShader->Begin(&nPass, 0);
	_Shader.pShader->BeginPass(_Pass);
	return (true);
}

//***********************************************************************************************************************************************************************
//		GRAPH9::EndSection(SHADER9 &)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::EndSection(SHADER9 &_Shader){
	if(_Shader.pShader == NULL){
		return (false);
	}
	_Shader.pShader->EndPass();
	_Shader.pShader->End();
	return (true);
}

//***********************************************************************************************************************************************************************
//		GRAPH9::InitResource(SHADER9 **)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::InitResource(SHADER9 **_Shader){
	return (FAILED((*_Shader)->pShader->OnResetDevice())) ? (false) : (true);	
}

//***********************************************************************************************************************************************************************
//		GRAPH9::FreeResource(SHADER9 **)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::FreeResource(SHADER9 **_Shader){
	return (FAILED((*_Shader)->pShader->OnLostDevice())) ? (false) : (true);	
}

//***********************************************************************************************************************************************************************
//		GRAPH9::SetTechnique(SHADER9 &, const wchar_t *)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::SetTechnique(SHADER9 &_Shader, const wchar_t *_Tech){
	size_t	szt;
	char	mbs[MAX_PATH]	= { 0 };
	if(_Shader.pShader == NULL){
		return (false);
	}
	wcstombs_s(&szt, mbs, MAX_PATH - 1, _Tech, wcslen(_Tech));
	return (FAILED(_Shader.pShader->SetTechnique(mbs))) ? (false) : (true);
}
