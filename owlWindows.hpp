#ifndef	__OWL_WINDOWS_HPP_
#define	__OWL_WINDOWS_HPP_
#include"owlCore.hpp"
#include<process.h>
#include<winsock2.h>
#include<windowsx.h>
#include<mmsystem.h>
#include<msxml6.h>
#include<comdef.h>
#include<shlobj.h>
#pragma	comment(lib, "winmm.lib")
#pragma	comment(lib, "ws2_32.lib")
#pragma	comment(lib, "msxml6.lib")
#pragma	comment(lib, "shfolder.lib")

#ifndef	SafeRelease
	#define	SafeRelease(p)			{ if(p){ (p)->Release(); (p) = NULL; } }
#endif
#ifndef	SafeCloseHandle
	#define	SafeCloseHandle(p)		{ if(p){ CloseHandle(p); (p) = NULL; } }
#endif
#if	(IPv6)
	#ifndef	LOOPBACK_ADDR
		#define	LOOPBACK_ADDR		(L"0:0:0:0:0:1")
	#endif
	#ifndef	NONRESERVED_PORT
		#define	NONRESERVED_PORT	(49152)
	#endif
	#ifndef	MAX_BUFFER
		#define	MAX_BUFFER			(4096)
	#endif
	#ifndef	DEFAULT_MAXCONNECT
		#define	DEFAULT_MAXCONNECT		(32)
	#endif
#else
	#ifndef	LOOPBACK_ADDR
		#define	LOOPBACK_ADDR		(L"127.0.0.1")
	#endif
	#ifndef	NONRESERVED_PORT
		#define	NONRESERVED_PORT	(49152)
	#endif
	#ifndef	MAX_BUFFER
		#define	MAX_BUFFER			(4096)
	#endif
	#ifndef	DEFAULT_MAXCONNECT
		#define	DEFAULT_MAXCONNECT	(32)
	#endif
#endif

namespace owl {

	//###################################################################################################################################################################
	//
	//		Core
	//
	namespace core {

		//---------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		XML
		//
		class XML {
		protected:
			IXMLDOMDocumentPtr	pDOM;
		public:
					 XML();
			virtual	~XML();
		public:
			bool		Init(const wchar_t *_in);
			bool		Quit();
			int			Count(const wchar_t *_in);
			int			Query(const wchar_t *_in, int _order, wchar_t *_out);
		};

		//---------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		Core Util
		//
		extern	void			OnceInit();
		extern	void			OnceQuit();

		extern	unsigned int	TimeInit();
		extern	unsigned int	TimeQuit();
		extern	unsigned int	TimeGain();

		extern	unsigned int	CountSet();
		extern	unsigned int	CountRef();
		extern	unsigned int	CountEnd();

		extern	const wchar_t *	PathUser();
		extern	const wchar_t *	PathBind(const wchar_t *_in);
		extern	const wchar_t *	PathOpen(const wchar_t *_in, ...);
		extern	bool			PathMake(const wchar_t *_in, ...);

		extern	const wchar_t *	PathThis(const wchar_t *_in);

		//---------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		Network
		//
		extern	bool			SockInit();
		extern	bool			SockQuit();

		//---------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		Typedef
		//
		typedef	std::vector<SOCKET>			SOCKLIST;
		typedef	std::vector<SOCKADDR_IN>	ADDRLIST;

		class STREAMDATA {
		public:
			SOCKADDR_IN		cFrom;
			unsigned char	cData[MAX_BUFFER];
			unsigned int	iSize;
		public:
			STREAMDATA(){
				iSize	= 0;
				memset(&cFrom, 0, sizeof(SOCKADDR_IN));
				memset( cData, 0, sizeof(unsigned char) * MAX_BUFFER);
			}
		};

		typedef	std::list<STREAMDATA>		STRMLIST;

		//---------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		LINK
		//
		class LINK {
		protected:
			short				iMode;
			short				iLoop;
			HANDLE				hProc;
			CRITICAL_SECTION	cSect;
			SOCKLIST			mSockList;
			ADDRLIST			mAddrList;
			STRMLIST			mStrmList;
		public:
					 LINK();
			virtual	~LINK();
		public:
			bool	InitServer(unsigned short _Port = NONRESERVED_PORT, int _Connect = DEFAULT_MAXCONNECT);
			bool	InitClient(unsigned short _Port = NONRESERVED_PORT, const wchar_t *_IP = LOOPBACK_ADDR);
			bool	Quit();
		public:
			int		Size();
			bool	Stop();
			bool	Wait();
		public:
			bool	Send(const unsigned char *_Data, int _Size, int  _Dest = (-1));
			bool	Recv(      unsigned char *_Data, int _Size, int *_Recv);
		private:
			static	unsigned int __stdcall	LinkProc(void *);
			static	unsigned int __stdcall	RecvProc(void *);
		};
	};
};

#endif