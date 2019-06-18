#include"owlWindows.hpp"

//***********************************************************************************************************************************************************************
//		Static	ハンドル値
//***********************************************************************************************************************************************************************
static	HANDLE	_Mutex	= NULL;

//***********************************************************************************************************************************************************************
//		owl::core::OnceInit()
//			[desc]
//				多重起動防止を設定する
//***********************************************************************************************************************************************************************
void owl::core::OnceInit(){
	SECURITY_DESCRIPTOR	sDescriptor;
	SECURITY_ATTRIBUTES	sAttributes;

	InitializeSecurityDescriptor(&sDescriptor, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&sDescriptor, TRUE, 0, FALSE);

	sAttributes.nLength					= sizeof(sAttributes);
	sAttributes.lpSecurityDescriptor	= &sDescriptor;
	sAttributes.bInheritHandle			= TRUE; 

	_Mutex = CreateMutex(&sAttributes, FALSE, DOMAIN_NAME);
	if(_Mutex == NULL){
		MessageBoxW(NULL, L"ミューテックスの作成に失敗しました", L"ERROR", MB_OK | MB_ICONSTOP);
		exit(0);
	}else if(GetLastError() == ERROR_ALREADY_EXISTS){
		SafeCloseHandle(_Mutex);
		MessageBoxW(NULL, L"既に起動しています", L"ERROR", MB_OK | MB_ICONSTOP);
		exit(0);
	}
}

//***********************************************************************************************************************************************************************
//		owl::core::OnceQuit()
//			[desc]
//				多重起動防止を解除する
//***********************************************************************************************************************************************************************
void owl::core::OnceQuit(){
	SafeCloseHandle(_Mutex);
}