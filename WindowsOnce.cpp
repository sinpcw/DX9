#include"owlWindows.hpp"

//***********************************************************************************************************************************************************************
//		Static	�n���h���l
//***********************************************************************************************************************************************************************
static	HANDLE	_Mutex	= NULL;

//***********************************************************************************************************************************************************************
//		owl::core::OnceInit()
//			[desc]
//				���d�N���h�~��ݒ肷��
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
		MessageBoxW(NULL, L"�~���[�e�b�N�X�̍쐬�Ɏ��s���܂���", L"ERROR", MB_OK | MB_ICONSTOP);
		exit(0);
	}else if(GetLastError() == ERROR_ALREADY_EXISTS){
		SafeCloseHandle(_Mutex);
		MessageBoxW(NULL, L"���ɋN�����Ă��܂�", L"ERROR", MB_OK | MB_ICONSTOP);
		exit(0);
	}
}

//***********************************************************************************************************************************************************************
//		owl::core::OnceQuit()
//			[desc]
//				���d�N���h�~����������
//***********************************************************************************************************************************************************************
void owl::core::OnceQuit(){
	SafeCloseHandle(_Mutex);
}