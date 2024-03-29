#include"owlWindows.hpp"

//***********************************************************************************************************************************************************************
//		Static
//***********************************************************************************************************************************************************************
static	wchar_t		_User[MAX_PATH]	= { '\0' };
static	wchar_t		_Path[MAX_PATH]	= { '\0' };

//***********************************************************************************************************************************************************************
//		PathUser()
//***********************************************************************************************************************************************************************
const wchar_t * owl::core::PathUser(){
	if(wcslen(_User) == 0){
		SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, _User);
	}
	return (_User);
}

//***********************************************************************************************************************************************************************
//		PathBind()
//***********************************************************************************************************************************************************************
const wchar_t * owl::core::PathBind(const wchar_t *_in){
	if(_in == NULL){
		memset(_Path, 0, sizeof(wchar_t) * MAX_PATH);
	}else{
		swprintf(_Path, MAX_PATH - 1, L"%s%s", _User, _in);
	}
	return (_Path);
}

//***********************************************************************************************************************************************************************
//		PathOpen()
//***********************************************************************************************************************************************************************
const wchar_t * owl::core::PathOpen(const wchar_t *_path, ...){
	vswprintf_s(_Path, MAX_PATH - 1, _path, reinterpret_cast<va_list>(&_path + 1));
	return (_Path);
}

//***********************************************************************************************************************************************************************
//		PathMake()
//***********************************************************************************************************************************************************************
bool owl::core::PathMake(const wchar_t *_path, ...){
	vswprintf_s(_Path, MAX_PATH - 1, _path, reinterpret_cast<va_list>(&_path + 1));
	if(CreateDirectory(_Path, NULL) == 0){
		if(ERROR_ALREADY_EXISTS != GetLastError()){
			return (false);
		}
	}
	return (true);
}

//***********************************************************************************************************************************************************************
//		PathThis()
//***********************************************************************************************************************************************************************
const wchar_t * owl::core::PathThis(const wchar_t *_in){
	int	iSize;
	GetModuleFileName(NULL, _Path, MAX_PATH - 1);
	for(iSize = static_cast<int>(wcslen(_Path)); 0 <= iSize; iSize--){
		if(_Path[iSize] == '\\'){
			_Path[iSize + 1]	= '\0';
			break;
		}
	}
	wcscat_s(_Path, MAX_PATH - 1, _in);
	return (_Path);
}
