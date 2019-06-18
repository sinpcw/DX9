#include"owlCore.hpp"

//***********************************************************************************************************************************************************************
//		owl::core::STORAGE::STORAGE()
//***********************************************************************************************************************************************************************
owl::core::STORAGE::STORAGE(){
	nSize	= 0;
	pData	= NULL;
}

//***********************************************************************************************************************************************************************
//		owl::core::STORAGE::~STORAGE()
//***********************************************************************************************************************************************************************
owl::core::STORAGE::~STORAGE(){
	nSize	= 0;
	SafeDeleteArray(pData);
}

//***********************************************************************************************************************************************************************
//		owl::core::STORAGE::Init(unsigned int)
//***********************************************************************************************************************************************************************
bool owl::core::STORAGE::Init(unsigned int _in){
	if(pData != NULL){
		return false;
	}
	try{
		pData	= new unsigned char[_in];
	}catch(...){
		return false;
	}
	memset(pData, 0, sizeof(unsigned char) * _in);
	nSize	= _in;
	return true;
}

//***********************************************************************************************************************************************************************
//		owl::core::STORAGE::Quit()
//***********************************************************************************************************************************************************************
bool owl::core::STORAGE::Quit(){
	SafeDeleteArray(pData);
	nSize	= 0;
	return true;
}

//***********************************************************************************************************************************************************************
//		owl::core::STORAGE::Load(const wchar_t *)
//***********************************************************************************************************************************************************************
bool owl::core::STORAGE::Load(const wchar_t *_in){
	FILE *	fp;
	size_t	sz;
	size_t	sv;
	if(pData != NULL){
		return false;
	}
	if(NULL == (fp = _wfopen(_in, L"rb"))){
		return false;
	}
	fseek(fp, 0, SEEK_END);
	sv	= ftell(fp);
	fseek(fp, 0, SEEK_SET);
	try{
		pData	= new unsigned char[sv];
	}catch(...){
		fclose(fp);
		return false;
	}
	sz	= fread(pData, sizeof(unsigned char), sv, fp);
	fclose(fp);
	if(sz != sv){
		SafeDeleteArray(pData);
		return false;
	}
	nSize	= static_cast<unsigned int>(sv);
	return true;
}

//***********************************************************************************************************************************************************************
//		owl::core::STORAGE::Save(const wchar_t *)
//***********************************************************************************************************************************************************************
bool owl::core::STORAGE::Save(const wchar_t *_in){
	FILE *	fp;
	size_t	sz;

	if(pData == NULL){
		return false;
	}
	if(NULL == (fp = _wfopen(_in, L"wb"))){
		return false;
	}
	sz	= fwrite(pData, sizeof(unsigned char), nSize, fp);
	fclose(fp);
	if(sz != nSize){
		return false;
	}
	return true;
}

//***********************************************************************************************************************************************************************
//		owl::core::STORAGE::Bind(const STORAGE &)
//***********************************************************************************************************************************************************************
bool owl::core::STORAGE::Bind(const STORAGE &_in){
	unsigned char *	pTemp	= NULL;

	if(_in.pData == NULL){
		return false;
	}
	try{
		pTemp	= new unsigned char[nSize + _in.nSize];
	}catch(...){
		return false;
	}
	if(pData){
		memcpy(pTemp, pData, sizeof(unsigned char) * nSize);
	}
	memcpy(&pTemp[nSize], _in.pData, sizeof(unsigned char) * _in.nSize);
	SafeDeleteArray(pData);
	pData	 = pTemp;
	nSize	+= _in.nSize;
	return true;
}
