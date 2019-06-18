#include"owlWindows.hpp"

//***********************************************************************************************************************************************************************
//
//		共通初期化項
//
//***********************************************************************************************************************************************************************
//		owl::core::SockInit()
//***********************************************************************************************************************************************************************
bool owl::core::SockInit(){
	WSADATA	wsaData;
	if(0 != WSAStartup(MAKEWORD(2, 0), &wsaData)){
		return (false);
	}
	return (true);
}


//***********************************************************************************************************************************************************************
//		owl::core::SockQuit()
//***********************************************************************************************************************************************************************
bool owl::core::SockQuit(){
	WSACleanup();
	return (true);
}

//***********************************************************************************************************************************************************************
//
//		ソケットコントローラ
//
//***********************************************************************************************************************************************************************
//		owl::core::LINK::LINK()
//***********************************************************************************************************************************************************************
owl::core::LINK::LINK(){
	iMode	= (0);
	iLoop	= (0);
	hProc	= (NULL);
	InitializeCriticalSection(&cSect);
}

//***********************************************************************************************************************************************************************
//		owl::core::LINK::~LINK()
//***********************************************************************************************************************************************************************
owl::core::LINK::~LINK(){
	Quit();
	iMode	= (0);
	iLoop	= (0);
	DeleteCriticalSection(&cSect);
}

//***********************************************************************************************************************************************************************
//		owl::core::LINK::InitServer(unsigned short, int)
//***********************************************************************************************************************************************************************
bool owl::core::LINK::InitServer(unsigned short _Port, int _Connect){
	BOOL		bin		= 1;
	SOCKET		sock;
	SOCKADDR_IN	addr;
	if(mSockList.empty() == false){
		return (true);
	}
	sock						= socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_family				= AF_INET;
	addr.sin_port				= htons(_Port);
	addr.sin_addr.S_un.S_addr	= INADDR_ANY;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char *>(&bin), sizeof(bin));
	bind(sock, reinterpret_cast<SOCKADDR *>(&addr), sizeof(addr));
	listen(sock, _Connect);
	//-----	<プッシュ>
	mSockList.push_back(sock);
	mAddrList.push_back(addr);
	//-----	<スレッド開始：接続>
	iMode	= (1);
	iLoop	= (1);
	hProc	= reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, LinkProc, this, 0, NULL));
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::core::LINK::InitClient(unsigned short, const wchar_t *)
//***********************************************************************************************************************************************************************
bool owl::core::LINK::InitClient(unsigned short _Port, const wchar_t *_IP){
	char		IP[MAX_PATH];
	SOCKET		sock;
	SOCKADDR_IN	addr;
	if(mSockList.empty() == false){
		return (true);
	}
	wcstombs(IP, _IP, MAX_PATH - 1);
	sock						= socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_family				= AF_INET;
	addr.sin_port				= htons(_Port);
	addr.sin_addr.S_un.S_addr	= inet_addr(IP);
	if(addr.sin_addr.S_un.S_addr == 0xFFFFFFFF){
		HOSTENT	* pHost;
		pHost	= gethostbyname(IP);
		if(pHost == NULL){
			return (false);
		}
		addr.sin_addr.S_un.S_addr	= *reinterpret_cast<unsigned int *>(pHost->h_addr_list[0]);
	}
	if(SOCKET_ERROR == connect(sock, reinterpret_cast<SOCKADDR *>(&addr), sizeof(addr))){
		return (false);
	}
	//-----	<プッシュ>
	mSockList.push_back(sock);
	mAddrList.push_back(addr);
	//-----	<スレッド開始：転送待ち>
	iMode	= (2);
	iLoop	= (1);
	hProc	= reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, RecvProc, this, 0, NULL));
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::core::LINK::Quit()
//***********************************************************************************************************************************************************************
bool owl::core::LINK::Quit(){
	Stop();
	mSockList.clear();
	mAddrList.clear();
	iMode	= (0);
	return (false);
}

//***********************************************************************************************************************************************************************
//		owl::core::LINK::Size()
//***********************************************************************************************************************************************************************
int owl::core::LINK::Size(){
	int	iSize	= 0;
	EnterCriticalSection(&cSect);
	iSize	= static_cast<int>(mSockList.size());
	LeaveCriticalSection(&cSect);
	return (iSize - 1);
}

//***********************************************************************************************************************************************************************
//		owl::core::LINK::Stop()
//***********************************************************************************************************************************************************************
bool owl::core::LINK::Stop(){
	iLoop	= (0);
	//-----	<スレッド終了>
	if(hProc){
		WaitForSingleObject(hProc, INFINITE);
		CloseHandle(hProc);
		hProc	= NULL;
	}
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::core::LINK::Wait()
//***********************************************************************************************************************************************************************
bool owl::core::LINK::Wait(){
	if(hProc != NULL){
		Stop();
	}
	//-----	<スレッド開始：転送待ち>
	iLoop	= (1);
	hProc	= reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, RecvProc, this, 0, NULL));
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::core::LINK::LinkProc()
//***********************************************************************************************************************************************************************
unsigned int __stdcall owl::core::LINK::LinkProc(void *pVoid){
	int			iSize	= sizeof(SOCKADDR_IN);
	LINK *		pLink	= static_cast<LINK *>(pVoid);
	SOCKET		nSock;
	SOCKADDR_IN	nAddr;
	TIMEVAL		vtim;
	fd_set		fdsw;
	fd_set		fdsr;

	FD_ZERO(&fdsr);
	FD_SET(pLink->mSockList[0], &fdsr);
	//-----	<1秒毎にタイムアウト>
	vtim.tv_sec		= 1;
	vtim.tv_usec	= 0;
	//-----	<制御ループ>
	while(pLink->iLoop > 0){
		memcpy(&fdsw, &fdsr, sizeof(fd_set));
		select(pLink->mSockList[0], &fdsw, NULL, NULL, &vtim);
		if(FD_ISSET(pLink->mSockList[0], &fdsw)){
			if(INVALID_SOCKET != (nSock = accept(pLink->mSockList[0], reinterpret_cast<SOCKADDR *>(&nAddr), &iSize))){
				EnterCriticalSection(&pLink->cSect);
				pLink->mSockList.push_back(nSock);
				pLink->mAddrList.push_back(nAddr);
				LeaveCriticalSection(&pLink->cSect);
			}
		}
	}
	return (0);
}

//***********************************************************************************************************************************************************************
//		owl::core::LINK::RecvProc()
//***********************************************************************************************************************************************************************
unsigned int __stdcall owl::core::LINK::RecvProc(void *pVoid){
	LINK *		pLink	= static_cast<LINK *>(pVoid);
	STREAMDATA	strm;
	TIMEVAL		vtim;
	fd_set		fdsw;
	fd_set		fdsr;
	char		cBuf[MAX_BUFFER]	= { 0 };
	int			size, ret;
	FD_ZERO(&fdsr);
	//-----	<全てに応答が来るようにする>
	size	= static_cast<int>(pLink->mSockList.size());
	for(int i = 0; i < size; i++){
		FD_SET(pLink->mSockList[i], &fdsr);
	}
	//-----	<1秒毎にタイムアウト>
	vtim.tv_sec		= 0;
	vtim.tv_usec	= 100000;
	//-----	<制御ループ>
	while(pLink->iLoop > 0){
		memcpy(&fdsw, &fdsr, sizeof(fd_set));
		if(select(0, &fdsw, NULL, NULL, &vtim) == 0){
			continue;
		}
		for(int i = 0; i < size; i++){
			if(FD_ISSET(pLink->mSockList[i], &fdsw)){
				if(0 == (ret = recv(pLink->mSockList[i], cBuf, MAX_BUFFER, 0))){
					shutdown(pLink->mSockList[i], SD_BOTH);
					closesocket(pLink->mSockList[i]);
				}
				if(ret < 0){
					continue;
				}else{
					memcpy(strm.cData, cBuf, sizeof(unsigned char) * ret);
					strm.cFrom	= pLink->mAddrList[i];
					strm.iSize	= ret;
				}
				//-----	<共有データ書込>
				EnterCriticalSection(&pLink->cSect);
				pLink->mStrmList.push_back(strm);
				LeaveCriticalSection(&pLink->cSect);
			}
		}
	}
	//-----	<ソケットを閉じる>
	for(int i = 0; i < size; i++){
		shutdown(pLink->mSockList[i], SD_BOTH);
		closesocket(pLink->mSockList[i]);
		pLink->mSockList[i]	= INVALID_SOCKET;
	}
	return (0);
}

//***********************************************************************************************************************************************************************
//		owl::core::LINK::Send(const unsigned char *, int, int)
//***********************************************************************************************************************************************************************
bool owl::core::LINK::Send(const unsigned char *_Data, int _Size, int _Dest){
	if(iMode == (2)){
		if(SOCKET_ERROR == send(mSockList[0], reinterpret_cast<const char *>(_Data), _Size, 0)){
			return (false);
		}
	}else{
		if(_Dest < 0){
			for(int i = 0, t = static_cast<int>(mSockList.size()); i < t; i++){
				send(mSockList[i], reinterpret_cast<const char *>(_Data), _Size, 0);
			}
		}else if(_Dest < static_cast<int>(mSockList.size())){
			send(mSockList[_Dest], reinterpret_cast<const char *>(_Data), _Size, 0);
		}else{
			return (false);
		}
	}
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::core::LINK::Recv(unsigned char *, int, int *)
//***********************************************************************************************************************************************************************
bool owl::core::LINK::Recv(unsigned char *_Data, int _Size, int *_Recv){
	memset(_Data, 0, sizeof(unsigned char) * _Size);
	EnterCriticalSection(&cSect);
	if(mStrmList.empty()){
		*_Recv	= 0;
	}else{
		*_Recv	= mStrmList.front().iSize;
		memcpy(_Data, mStrmList.front().cData, std::min<int>(_Size, mStrmList.front().iSize) * sizeof(unsigned char));
		mStrmList.pop_front();
	}
	LeaveCriticalSection(&cSect);
	return (true);
}