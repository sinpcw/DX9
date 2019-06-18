#include"owlWindows.hpp"

//***********************************************************************************************************************************************************************
//		owl::core::XML::XML()
//***********************************************************************************************************************************************************************
owl::core::XML::XML(){
	CoInitialize(NULL);
	pDOM	= NULL;
}

//***********************************************************************************************************************************************************************
//		owl::core::XML::~XML()
//***********************************************************************************************************************************************************************
owl::core::XML::~XML(){
	Quit();
	CoUninitialize();
}

//***********************************************************************************************************************************************************************
//		owl::core::XML::Init(const wchar_t *)
//***********************************************************************************************************************************************************************
bool owl::core::XML::Init(const wchar_t *_in){
	VARIANT_BOOL	bRet;

	if(pDOM != NULL){
		return (true);
	}
	if(FAILED(pDOM.CreateInstance(CLSID_DOMDocument))){
		CoUninitialize();
		return (false);
	}
	if(FAILED(pDOM->put_async(VARIANT_FALSE))){
		CoUninitialize();
		return (false);
	}
	if(FAILED(pDOM->load(_variant_t(_in), &bRet))){
		CoUninitialize();
		return (false);
	}
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::core::XML::Quit()
//***********************************************************************************************************************************************************************
bool owl::core::XML::Quit(){
	pDOM	= NULL;
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::core::XML::Count(const wchar_t *)
//***********************************************************************************************************************************************************************
int owl::core::XML::Count(const wchar_t *_in){
	long				lSize				= 0;
	BSTR				nText				= NULL;
	IXMLDOMNode	*		pNode				= NULL;
	IXMLDOMNodeList	*	pInterface			= NULL;
	if(FAILED(pDOM->selectNodes(const_cast<wchar_t *>(_in), &pInterface))){
		goto ERROR_HANDLER;
	}
	if(FAILED(pInterface->get_length(&lSize))){
		goto ERROR_HANDLER;
	}
	SafeRelease(pInterface);
	SafeRelease(pNode);
	return static_cast<int>(lSize);

ERROR_HANDLER:
	if(nText){
		SysFreeString(nText);
	}
	SafeRelease(pInterface);
	SafeRelease(pNode);
	return (-1);
}

//***********************************************************************************************************************************************************************
//		owl::core::XML::Query(const wchar_t *, int, wchar_t *)
//***********************************************************************************************************************************************************************
int owl::core::XML::Query(const wchar_t *_in, int _order, wchar_t *_out){
	long				lSize				= 0;
	BSTR				nText				= NULL;
	IXMLDOMNode	*		pNode				= NULL;
	IXMLDOMNodeList	*	pInterface			= NULL;
	if(FAILED(pDOM->selectNodes(const_cast<wchar_t *>(_in), &pInterface))){
		goto ERROR_HANDLER;
	}
	if(FAILED(pInterface->get_length(&lSize))){
		goto ERROR_HANDLER;
	}
	if(_order < 0 || static_cast<int>(lSize) <= _order){
		goto ERROR_HANDLER;
	}
	if(FAILED(pInterface->get_item(_order, &pNode))){
		goto ERROR_HANDLER;
	}
	if(FAILED(pNode->get_text(&nText))){
		goto ERROR_HANDLER;
	}
	if(nText == NULL){
		goto ERROR_HANDLER;
	}else{
		wcscpy_s(_out, MAX_PATH - 1, nText);
		SysFreeString(nText);
	}
	SafeRelease(pInterface);
	SafeRelease(pNode);
	return (_order);

ERROR_HANDLER:
	if(nText){
		SysFreeString(nText);
	}
	SafeRelease(pInterface);
	SafeRelease(pNode);
	return (-1);
}