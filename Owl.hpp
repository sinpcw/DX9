#ifndef	__OWL_HPP_
#define	__OWL_HPP_

#include"owlCore.hpp"
#include"owlExterns.hpp"
#include"owlWindows.hpp"
#include"owlEngine9.hpp"

#ifdef	UNICODE
	#define	OwlMain()	__stdcall wWinMain(HINSTANCE, HINSTANCE, LPWSTR, INT)
#else
	#define	OwlMain()	__stdcall  WinMain(HINSTANCE, HINSTANCE, LPSTR , INT)
#endif

namespace owl {

	class UI : public engine::GRAPH9, public engine::INPUT8, public engine::AUDIO8 {
	public:
				 UI();
		virtual	~UI();
	public:
		virtual	bool	Init(const core::VECT2i &_Size,const wchar_t *_Title, const wchar_t *_Icon = NULL, const wchar_t *_Menu = NULL);
		virtual bool	Init(const core::VECT2i &_Size, int _Windowed, int _DWM, const wchar_t *_Title, const wchar_t *_Icon = NULL);
		virtual	bool	Quit();
		virtual bool	Ctrl();
	};

};

#endif