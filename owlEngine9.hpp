#ifndef	__OWL_ENGINE_HPP_
#define	__OWL_ENGINE_HPP_
#define	INIT_GUID
#define	DIRECTINPUT_VERSION		(0x0800)
#include"owlCore.hpp"
#include<windows.h>
#include<windowsx.h>
#include<mmsystem.h>
#include<dwmapi.h>
#include<d3d9.h>
#include<d3dx9.h>
#include<dsound.h>
#include<dinput.h>
#pragma	warning(disable : 4244) 
#include<ogg/ogg.h>
#include<vorbis/codec.h>
#include<vorbis/vorbisenc.h>
#include<vorbis/vorbisfile.h>
#pragma	warning(default : 4244)
#pragma	comment(lib, "winmm.lib")
#pragma	comment(lib, "dxguid.lib")
#pragma	comment(lib, "d3d9.lib")
#pragma	comment(lib, "d3dx9.lib")
#pragma	comment(lib, "dinput8.lib")
#pragma	comment(lib, "dsound.lib")
#ifdef	_DEBUG
	#pragma	comment(lib, "ogg_static_d.lib")
	#pragma	comment(lib, "vorbis_static_d.lib")
	#pragma	comment(lib, "vorbisenc_static_d.lib")
	#pragma	comment(lib, "vorbisfile_static_d.lib")
#else
	#pragma	comment(lib, "ogg_static.lib")
	#pragma	comment(lib, "vorbis_static.lib")
	#pragma	comment(lib, "vorbisenc_static.lib")
	#pragma	comment(lib, "vorbisfile_static.lib")
#endif

#ifndef	SafeRelease
	#define	SafeRelease(p)				{ if(p){ (p)->Release(); (p) = NULL; } }
#endif
#ifndef	SafeInputRelease
	#define	SafeInputRelease(p)			{ if(p){ (p)->Unacquire(); (p)->Release(); (p) = NULL; } }
#endif
#ifndef	SafeForceRelease
	#define	SafeForceRelease(p)			{ if(p){ (p)->Unload(); (p)->Release(); (p) = NULL; } }
#endif
#ifndef	SafeAudioRelease
	#define	SafeAudioRelease(p)			{ if(p){ (p)->Stop(); (p)->Release(); (p) = NULL; } }
#endif

#define	OVK_ESCAPE			DIK_ESCAPE
#define	OVK_1				DIK_1
#define	OVK_2				DIK_2
#define	OVK_3				DIK_3
#define	OVK_4				DIK_4
#define	OVK_5				DIK_5
#define	OVK_6				DIK_6
#define	OVK_7				DIK_7
#define	OVK_8				DIK_8
#define	OVK_9				DIK_9
#define	OVK_0				DIK_0
#define	OVK_MINUS			DIK_MINUS
#define	OVK_EQUALS			DIK_EQUALS
#define	OVK_BACK			DIK_BACK
#define	OVK_TAB				DIK_TAB
#define	OVK_Q				DIK_Q
#define	OVK_W				DIK_W
#define	OVK_E				DIK_E
#define	OVK_R				DIK_R
#define	OVK_T				DIK_T
#define	OVK_Y				DIK_Y
#define	OVK_U				DIK_U
#define	OVK_I				DIK_I
#define	OVK_O				DIK_O
#define	OVK_P				DIK_P
#define	OVK_LBRACKET		DIK_LBRACKET
#define	OVK_RBRACKET		DIK_RBRACKET
#define	OVK_RETURN			DIK_RETURN
#define	OVK_LCONTROL		DIK_LCONTROL
#define	OVK_A				DIK_A
#define	OVK_S				DIK_S
#define	OVK_D				DIK_D
#define	OVK_F				DIK_F
#define	OVK_G				DIK_G
#define	OVK_H				DIK_H
#define	OVK_J				DIK_J
#define	OVK_K				DIK_K
#define	OVK_L				DIK_L
#define	OVK_SEMICOLON		DIK_SEMICOLON
#define	OVK_APOSTROPHE		DIK_APOSTROPHE
#define	OVK_GRAVE			DIK_GRAVE
#define	OVK_LSHIFT			DIK_LSHIFT
#define	OVK_BACKSLASH		DIK_BACKSLASH
#define	OVK_Z				DIK_Z
#define	OVK_X				DIK_X
#define	OVK_C				DIK_C
#define	OVK_V				DIK_V
#define	OVK_B				DIK_B
#define	OVK_N				DIK_N
#define	OVK_M				DIK_M
#define	OVK_COMMA			DIK_COMMA
#define	OVK_PERIOD			DIK_PERIOD
#define	OVK_SLASH			DIK_SLASH
#define	OVK_RSHIFT			DIK_RSHIFT
#define	OVK_MULTIPLY		DIK_MULTIPLY
#define	OVK_LMENU			DIK_LMENU
#define	OVK_SPACE			DIK_SPACE
#define	OVK_CAPITAL			DIK_CAPITAL
#define	OVK_F1				DIK_F1
#define	OVK_F2				DIK_F2
#define	OVK_F3				DIK_F3
#define	OVK_F4				DIK_F4
#define	OVK_F5				DIK_F5
#define	OVK_F6				DIK_F6
#define	OVK_F7				DIK_F7
#define	OVK_F8				DIK_F8
#define	OVK_F9				DIK_F9
#define	OVK_F10				DIK_F10
#define	OVK_NUMLOCK			DIK_NUMLOCK
#define	OVK_SCROLL			DIK_SCROLL
#define	OVK_NUMPAD7			DIK_NUMPAD7
#define	OVK_NUMPAD8			DIK_NUMPAD8
#define	OVK_NUMPAD9			DIK_NUMPAD9
#define	OVK_SUBTRACT		DIK_SUBTRACT
#define	OVK_NUMPAD4			DIK_NUMPAD4
#define	OVK_NUMPAD5			DIK_NUMPAD5
#define	OVK_NUMPAD6			DIK_NUMPAD6
#define	OVK_ADD				DIK_ADD
#define	OVK_NUMPAD1			DIK_NUMPAD1
#define	OVK_NUMPAD2			DIK_NUMPAD2
#define	OVK_NUMPAD3			DIK_NUMPAD3
#define	OVK_NUMPAD0			DIK_NUMPAD0
#define	OVK_DECIMAL			DIK_DECIMAL
#define	OVK_OEM_102			DIK_OEM_102
#define	OVK_F11				DIK_F11
#define	OVK_F12				DIK_F12
#define	OVK_F13				DIK_F13
#define	OVK_F14				DIK_F14
#define	OVK_F15				DIK_F15
#define	OVK_KANA			DIK_KANA
#define	OVK_ABNT_C1			DIK_ABNT_C1
#define	OVK_CONVERT			DIK_CONVERT
#define	OVK_NOCONVERT		DIK_NOCONVERT
#define	OVK_YEN				DIK_YEN
#define	OVK_ABNT_C2			DIK_ABNT_C2
#define	OVK_NUMPADEQUALS	DIK_NUMPADEQUALS
#define	OVK_PREVTRACK		DIK_PREVTRACK
#define	OVK_AT				DIK_AT
#define	OVK_COLON			DIK_COLON
#define	OVK_UNDERLINE		DIK_UNDERLINE
#define	OVK_KANJI			DIK_KANJI
#define	OVK_STOP			DIK_STOP
#define	OVK_AX				DIK_AX
#define	OVK_UNLABELED		DIK_UNLABELED
#define	OVK_NEXTTRACK		DIK_NEXTTRACK
#define	OVK_NUMPADENTER		DIK_NUMPADENTER
#define	OVK_RCONTROL		DIK_RCONTROL
#define	OVK_MUTE			DIK_MUTE
#define	OVK_CALCULATOR		DIK_CALCULATOR
#define	OVK_PLAYPAUSE		DIK_PLAYPAUSE
#define	OVK_MEDIASTOP		DIK_MEDIASTOP
#define	OVK_VOLUMEDOWN		DIK_VOLUMEDOWN
#define	OVK_VOLUMEUP		DIK_VOLUMEUP
#define	OVK_WEBHOME			DIK_WEBHOME
#define	OVK_NUMPADCOMMA		DIK_NUMPADCOMMA
#define	OVK_DIVIDE			DIK_DIVIDE
#define	OVK_SYSRQ			DIK_SYSRQ
#define	OVK_RMENU			DIK_RMENU
#define	OVK_PAUSE			DIK_PAUSE
#define	OVK_HOME			DIK_HOME
#define	OVK_UP				DIK_UP
#define	OVK_PRIOR			DIK_PRIOR
#define	OVK_LEFT			DIK_LEFT
#define	OVK_RIGHT			DIK_RIGHT
#define	OVK_END				DIK_END
#define	OVK_DOWN			DIK_DOWN
#define	OVK_NEXT			DIK_NEXT
#define	OVK_INSERT			DIK_INSERT
#define	OVK_DELETE			DIK_DELETE
#define	OVK_LWIN			DIK_LWIN
#define	OVK_RWIN			DIK_RWIN
#define	OVK_APPS			DIK_APPS
#define	OVK_POWER			DIK_POWER
#define	OVK_SLEEP			DIK_SLEEP
#define	OVK_WAKE			DIK_WAKE
#define	OVK_WEBSEARCH		DIK_WEBSEARCH
#define	OVK_WEBFAVORITES	DIK_WEBFAVORITES
#define	OVK_WEBREFRESH		DIK_WEBREFRESH
#define	OVK_WEBSTOP			DIK_WEBSTOP
#define	OVK_WEBFORWARD		DIK_WEBFORWARD
#define	OVK_WEBBACK			DIK_WEBBACK
#define	OVK_MYCOMPUTER		DIK_MYCOMPUTER
#define	OVK_MAIL			DIK_MAIL
#define	OVK_MEDIASELECT		DIK_MEDIASELECT
#define	OVK_BACKSPACE		DIK_BACKSPACE
#define	OVK_NUMPADSTAR		DIK_NUMPADSTAR
#define	OVK_LALT			DIK_LALT
#define	OVK_CAPSLOCK		DIK_CAPSLOCK
#define	OVK_NUMPADMINUS		DIK_NUMPADMINUS
#define	OVK_NUMPADPLUS		DIK_NUMPADPLUS
#define	OVK_NUMPADPERIOD	DIK_NUMPADPERIOD
#define	OVK_NUMPADSLASH		DIK_NUMPADSLASH
#define	OVK_RALT			DIK_RALT
#define	OVK_UPARROW			DIK_UPARROW
#define	OVK_PGUP			DIK_PGUP
#define	OVK_LEFTARROW		DIK_LEFTARROW
#define	OVK_RIGHTARROW		DIK_RIGHTARROW
#define	OVK_DOWNARROW		DIK_DOWNARROW
#define	OVK_PGDN			DIK_PGDN
#define OVK_CIRCUMFLEX		DIK_CIRCUMFLEX

namespace owl {
	
	//###################################################################################################################################################################
	//
	//		Engine
	//
	namespace engine {

		//***********************************************************************************************************************************************************
		//
		//		Graphics
		//
		//***********************************************************************************************************************************************************

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		Typedef
		//
		typedef	D3DXMATRIX	MATRIX;

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		Const
		//
		const	float				COLOR_PITCH		= 0.003921568627451f;

		const	unsigned int		FVF_XDVERTEX2D	= (D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
		const	unsigned int		FVF_TDVERTEX2D	= (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
		const	unsigned int		FVF_XDVERTEX3D	= (D3DFVF_XYZ    | D3DFVF_DIFFUSE);
		const	unsigned int		FVF_TDVERTEX3D	= (D3DFVF_XYZ    | D3DFVF_DIFFUSE | D3DFVF_TEX1);
		const	unsigned int		FVF_DNVERTEX3D	= (D3DFVF_XYZ    | D3DFVF_NORMAL  | D3DFVF_DIFFUSE);
		const	unsigned int		FVF_TDNVERTEX3D	= (D3DFVF_XYZ    | D3DFVF_NORMAL  | D3DFVF_DIFFUSE | D3DFVF_TEX1);

		const	unsigned int		PRIMITIVE_POINT	= (1);
		const	unsigned int		PRIMITIVE_LINE	= (2);
		const	unsigned int		PRIMITIVE_WIRE	= (3);
		const	unsigned int		PRIMITIVE_POLY	= (4);
		const	unsigned int		PRIMITIVE_QUAD	= (5);

		const	unsigned int		MESH_BOX		= (0);
		const	unsigned int		MESH_TEXT		= (1);
		const	unsigned int		MESH_TORUS		= (2);
		const	unsigned int		MESH_TEAPOT		= (3);
		const	unsigned int		MESH_SPHERE		= (4);
		const	unsigned int		MESH_CYLINDER	= (5);

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		Inline
		//
		inline	D3DXVECTOR2		Vect(const core::VECT2f &_v){ return D3DXVECTOR2 (_v.x, _v.y);				}
		inline	D3DXVECTOR3		Vect(const core::VECT3f &_v){ return D3DXVECTOR3 (_v.x, _v.y, _v.z);		}
		inline	D3DXVECTOR4		Vect(const core::VECT4f &_v){ return D3DXVECTOR4 (_v.x, _v.y, _v.z, _v.w);	}
		inline	core::VECT2f	Vect(const D3DXVECTOR2  &_v){ return core::VECT2f(_v.x, _v.y);				}
		inline	core::VECT3f	Vect(const D3DXVECTOR3  &_v){ return core::VECT3f(_v.x, _v.y, _v.z);		}
		inline	core::VECT4f	Vect(const D3DXVECTOR4  &_v){ return core::VECT4f(_v.x, _v.y, _v.z, _v.w);	}

		inline	core::COLORf	Color(unsigned int _c){ return core::COLORf(COLOR_PITCH * static_cast<float>((_c & 0x00FF0000) >> 16), COLOR_PITCH * static_cast<float>((_c & 0x0000FF00) >> 8), COLOR_PITCH * static_cast<float>((_c & 0x000000FF) >> 0),COLOR_PITCH * static_cast<float>((_c & 0xFF000000) >> 24)); }

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		Declaration
		//
		class	GRAPH9;
		class	PRIMITIVE2D9X;
		class	PRIMITIVE3D9X;
		class	PRIMITIVE3D9N;
		class	PRIMITIVE3D9W;

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		Typedef
		//
		typedef	struct _XDVERTEX2D {
			D3DXVECTOR4		Point;
			DWORD			Color;
		} XDVERTEX2D;

		typedef	struct _TDVERTEX2D {
			D3DXVECTOR4		Point;
			DWORD			Color;
			D3DXVECTOR2		Coord;
		} TDVERTEX2D;

		typedef	struct _XDVERTEX3D {
			D3DXVECTOR3		Point;
			DWORD			Color;
		} XDVERTEX3D;

		typedef	struct _DNVERTEX3D {
			D3DXVECTOR3		Point;
			D3DXVECTOR3		Normal;
			DWORD			Color;
		} DNVERTEX3D;

		typedef	struct _TDVERTEX3D {
			D3DXVECTOR3		Point;
			DWORD			Color;
			D3DXVECTOR2		Coord;
		} TDVERTEX3D;

		typedef	struct _TDNVERTEX3D {
			D3DXVECTOR3		Point;
			D3DXVECTOR3		Normal;
			DWORD			Color;
			D3DXVECTOR2		Coord;
		} TDNVERTEX3D;

		class MOUSEINFO {
		public:
			int		iX;
			int		iY;
			int		iZ;
			int		iButton;
		public:
			MOUSEINFO(){
				iX		= 0;
				iY		= 0;
				iZ		= 0;
				iButton	= 0;
			}
		};
			
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		TEXTURE9
		//
		class TEXTURE9 {
			friend	GRAPH9;
		protected:
			IDirect3DTexture9 *		pTexture;
			core::VECT2f			vXY;
			core::VECT2f			vUV;
		public:
					 TEXTURE9();
			virtual	~TEXTURE9();
		public:
			virtual	core::VECT2f XY(){ return vXY; }
			virtual	core::VECT2f UV(){ return vUV; }
		public:
			virtual	IDirect3DTexture9 * GetTexture(){ return pTexture; }
		};
			
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		Primitive Render
		//
		namespace render {

			typedef	void	(*TYPE2DX)(IDirect3DDevice9 *, PRIMITIVE2D9X &);
			typedef	void	(*TYPE3DX)(IDirect3DDevice9 *, PRIMITIVE3D9X &);
			typedef	void	(*TYPE3DN)(IDirect3DDevice9 *, PRIMITIVE3D9N &);
			typedef	void	(*TYPE3DW)(IDirect3DDevice9 *, PRIMITIVE3D9W &);

			extern	void	SPOT2D(IDirect3DDevice9 *, PRIMITIVE2D9X &);
			extern	void	LINE2D(IDirect3DDevice9 *, PRIMITIVE2D9X &);
			extern	void	WIRE2D(IDirect3DDevice9 *, PRIMITIVE2D9X &);
			extern	void	POLY2D(IDirect3DDevice9 *, PRIMITIVE2D9X &);
			extern	void	QUAD2D(IDirect3DDevice9 *, PRIMITIVE2D9X &);

			extern	void	SPOT3D(IDirect3DDevice9 *, PRIMITIVE3D9X &);
			extern	void	LINE3D(IDirect3DDevice9 *, PRIMITIVE3D9X &);
			extern	void	WIRE3D(IDirect3DDevice9 *, PRIMITIVE3D9X &);
			extern	void	POLY3D(IDirect3DDevice9 *, PRIMITIVE3D9X &);
			extern	void	QUAD3D(IDirect3DDevice9 *, PRIMITIVE3D9X &);

			extern	void	SPOT3D(IDirect3DDevice9 *, PRIMITIVE3D9N &);
			extern	void	LINE3D(IDirect3DDevice9 *, PRIMITIVE3D9N &);
			extern	void	WIRE3D(IDirect3DDevice9 *, PRIMITIVE3D9N &);
			extern	void	POLY3D(IDirect3DDevice9 *, PRIMITIVE3D9N &);
			extern	void	QUAD3D(IDirect3DDevice9 *, PRIMITIVE3D9N &);

			extern	void	SPOT3D(IDirect3DDevice9 *, PRIMITIVE3D9W &);
			extern	void	LINE3D(IDirect3DDevice9 *, PRIMITIVE3D9W &);
			extern	void	WIRE3D(IDirect3DDevice9 *, PRIMITIVE3D9W &);
			extern	void	POLY3D(IDirect3DDevice9 *, PRIMITIVE3D9W &);
			extern	void	QUAD3D(IDirect3DDevice9 *, PRIMITIVE3D9W &);

		};

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		PRIMITIVE2D9X
		//
		class PRIMITIVE2D9X {
			friend	GRAPH9;
		protected:
			unsigned int				uSize;
			unsigned int				uPush;
			render::TYPE2DX				pDraw;
			XDVERTEX2D *				pInfo;
			XDVERTEX2D *				pThis;
			IDirect3DIndexBuffer9 *		pIB;
			IDirect3DVertexBuffer9 *	pVB;
		public:
					 PRIMITIVE2D9X();
			virtual	~PRIMITIVE2D9X();
		public:
			virtual	unsigned int				Push(){ return uPush; }
			virtual	XDVERTEX2D *				Base(){ return pInfo; }
			virtual	IDirect3DIndexBuffer9 *		IdxBuf(){ return pIB; }
			virtual	IDirect3DVertexBuffer9 *	VtxBuf(){ return pVB; }
		};

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		PRIMITIVE3D9X
		//
		class PRIMITIVE3D9X {
			friend	GRAPH9;
		protected:
			unsigned int				uSize;
			unsigned int				uPush;
			render::TYPE3DX				pDraw;
			XDVERTEX3D *				pInfo;
			XDVERTEX3D *				pThis;
			IDirect3DIndexBuffer9 *		pIB;
			IDirect3DVertexBuffer9 *	pVB;
		public:
					 PRIMITIVE3D9X();
			virtual	~PRIMITIVE3D9X();
		public:
			virtual	unsigned int				Push(){ return uPush; }
			virtual	XDVERTEX3D *				Base(){ return pInfo; }
			virtual	IDirect3DIndexBuffer9 *		IdxBuf(){ return pIB; }
			virtual	IDirect3DVertexBuffer9 *	VtxBuf(){ return pVB; }
		};

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		PRIMITIVE3D9N
		//
		class PRIMITIVE3D9N {
			friend	GRAPH9;
		protected:
			unsigned int				uSize;
			unsigned int				uPush;
			render::TYPE3DN				pDraw;
			DNVERTEX3D *				pInfo;
			DNVERTEX3D *				pThis;
			IDirect3DIndexBuffer9 *		pIB;
			IDirect3DVertexBuffer9 *	pVB;
		public:
					 PRIMITIVE3D9N();
			virtual	~PRIMITIVE3D9N();
		public:
			virtual	unsigned int				Push(){ return uPush; }
			virtual	DNVERTEX3D *				Base(){ return pInfo; }
			virtual	IDirect3DIndexBuffer9 *		IdxBuf(){ return pIB; }
			virtual	IDirect3DVertexBuffer9 *	VtxBuf(){ return pVB; }
		public:
			unsigned int	Recycle(unsigned int _in){
				return (uPush = std::min<unsigned int>(uSize, _in));
			}
		};

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		PRIMITIVE3D9W
		//
		class PRIMITIVE3D9W {
			friend	GRAPH9;
		protected:
			unsigned int				uSize;
			unsigned int				uPush;
			render::TYPE3DW				pDraw;
			TDNVERTEX3D *				pInfo;
			TDNVERTEX3D *				pThis;
			IDirect3DIndexBuffer9 *		pIB;
			IDirect3DVertexBuffer9 *	pVB;
		public:
					 PRIMITIVE3D9W();
			virtual	~PRIMITIVE3D9W();
		public:
			virtual	unsigned int				Push(){ return uPush; }
			virtual	TDNVERTEX3D *				Base(){ return pInfo; }
			virtual	IDirect3DIndexBuffer9 *		IdxBuf(){ return pIB; }
			virtual	IDirect3DVertexBuffer9 *	VtxBuf(){ return pVB; }
		public:
			unsigned int	Recycle(unsigned int _in){
				return (uPush = std::min<unsigned int>(uSize, _in));
			}
		};

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		TEXTURE2D9
		//
		class TEXTURE2D9 : public TEXTURE9 {
			friend	GRAPH9;
		protected:
			unsigned int				uSize;
			unsigned int				uPush;
			TDVERTEX2D *				pInfo;
			TDVERTEX2D *				pThis;
			IDirect3DIndexBuffer9 *		pIB;
			IDirect3DVertexBuffer9 *	pVB;
		public:
					 TEXTURE2D9();
			virtual	~TEXTURE2D9();
		public:
			virtual	IDirect3DTexture9 *	GetTexture(){ return pTexture; }
		};

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		TEXTURE3D9
		//
		class TEXTURE3D9 : public TEXTURE9 {
			friend	GRAPH9;
		protected:
			unsigned int				uSize;
			unsigned int				uPush;
			TDVERTEX3D *				pInfo;
			TDVERTEX3D *				pThis;
			IDirect3DIndexBuffer9 *		pIB;
			IDirect3DVertexBuffer9 *	pVB;
		public:
					 TEXTURE3D9();
			virtual	~TEXTURE3D9();
		public:
			virtual	IDirect3DTexture9 *	GetTexture(){ return pTexture; }
		};

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		SURFACE9
		//
		class SURFACE9 {
			friend	GRAPH9;
		protected:
			IDirect3DTexture9 *		pTexture;
			IDirect3DSurface9 *		pSurface;
			IDirect3DSurface9 *		pZBuffer;
			core::VECT2f			vXY;
			core::VECT2f			vUV;
		public:
					 SURFACE9();
			virtual	~SURFACE9();
		public:
			virtual	IDirect3DSurface9 * GetSurface(){ return pSurface; }
			virtual	IDirect3DTexture9 *	GetTexture(){ return pTexture; }
		};

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		SHADER9
		//
		class SHADER9 {
			friend	GRAPH9;
		protected:
			ID3DXEffect *	pShader;
		public:
					 SHADER9();
			virtual	~SHADER9();
		public:
			virtual	ID3DXEffect *	GetShaderPtr(){ return pShader; }
		};

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		Typedef
		//
		typedef	std::list<SURFACE9 *>				LISTSURFACE;
		typedef	std::list<SURFACE9 *>::iterator		LISTSURFACEITERATOR;
		typedef	std::list<SHADER9 *>				LISTSHADER;
		typedef	std::list<SHADER9 *>::iterator		LISTSHADERITERATOR;

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		MESH9
		//
		class MESH9 {
			friend	GRAPH9;
		protected:
			unsigned int	uSize;
			ID3DXMesh *		pMesh;
			D3DMATERIAL9 *	pMaterial;
		public:
					 MESH9();
			virtual	~MESH9();
		public:
			virtual	ID3DXMesh *		GetMesh()									{ return pMesh; }
			virtual	unsigned int	GetCount()									{ return uSize;	}
			virtual	D3DMATERIAL9	GetMaterial(UINT _in)						{ return pMaterial[_in]; }
			virtual	void			SetMaterial(UINT _in, D3DMATERIAL9 &_mat)	{ pMaterial[_in] = _mat; }
		};

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		MESH9EX
		//
		class MESH9EX {
			friend	GRAPH9;
		protected:
			unsigned int			uSize;
			ID3DXMesh *				pMesh;
			D3DMATERIAL9 *			pMaterial;
			IDirect3DTexture9 **	ppTexture;
		public:
					 MESH9EX();
			virtual	~MESH9EX();
		public:
			virtual	ID3DXMesh *			GetMesh()									{ return pMesh; }
			virtual	unsigned int		GetCount()									{ return uSize;	}
			virtual	IDirect3DTexture9 *	GetTexture(UINT _in)						{ return ppTexture[_in]; }
			virtual	D3DMATERIAL9		GetMaterial(UINT _in)						{ return pMaterial[_in]; }
			virtual	void				SetMaterial(UINT _in, D3DMATERIAL9 &_mat)	{ pMaterial[_in] = _mat; }
		};

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		CHAR9
		//
		class CHAR9 {
			friend	GRAPH9;
		protected:
			IDirect3DTexture9 *		pTexture;
			core::VECT2f			vXY;
			core::VECT2f			vUV;
		public:
			CHAR9(){
				pTexture	= NULL;
				vXY			= core::VECT2f(0);
				vUV			= core::VECT2f(0);
			}
		};
			
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		Typedef
		//
		typedef	std::map<wchar_t, CHAR9>				MAPCHAR;
		typedef	std::map<wchar_t, CHAR9>::iterator		MAPCHARITERATOR;
	
		typedef	std::pair<wchar_t, CHAR9>				MAPCHARPAIR;
		
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		FONT9
		//
		class FONT9 {
			friend	GRAPH9;
		protected:
			HDC			hContext;
			HFONT		hNewFont;
			HFONT		hOldFont;
			MAPCHAR		mTexture;
			int			iQuality;
		public:
					 FONT9();
			virtual	~FONT9();
		};

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		MESH9CREATEPARAM9
		//
		class MESH9CREATEPARAM {
		public:
			unsigned int	nInfo;
			float			fX;
			float			fY;
			float			fZ;
			int				iU;
			int				iV;
		public:
			MESH9CREATEPARAM(){
				nInfo	= MESH_TEAPOT;
				fX		= 0;
				fY		= 0;
				fZ		= 0;
				iU		= 0;
				iV		= 0;
			}
			MESH9CREATEPARAM(float _Radius, int _Slice, int _Stack){
				nInfo	= MESH_SPHERE;
				fX		= _Radius;
				fY		= 0;
				fZ		= 0;
				iU		= _Slice;
				iV		= _Stack;
			}
			MESH9CREATEPARAM(float _Width, float _Height, float _Depth){
				nInfo	= MESH_BOX;
				fX		= _Width;
				fY		= _Height;
				fZ		= _Depth;
				iU		= 0;
				iV		= 0;
			}
			MESH9CREATEPARAM(float _RadiusI, float _RadiusO, int _Slice, int _Stack){
				nInfo	= MESH_TORUS;
				fX		= _RadiusI;
				fY		= _RadiusO;
				fZ		= 0;
				iU		= _Slice;
				iV		= _Stack;
			}
			MESH9CREATEPARAM(float _Radius1, float _Radius2, float _Length, int _Slice, int _Stack){
				nInfo	= MESH_CYLINDER;
				fX		= _Radius1;
				fY		= _Radius2;
				fZ		= _Length;
				iU		= _Slice;
				iV		= _Stack;
			}
		};

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		Custom GUI
		//
		typedef	int (*WNDFUNC)(HWND, UINT, WPARAM, LPARAM);

		inline int Null(HWND, UINT, WPARAM, LPARAM){
			return (-1);
		}

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		GRAPH9
		//
		class GRAPH9 {
		protected:
			HWND					hWindow;
			MOUSEINFO				cMouseInfo;
		protected:
			FLOAT					fAspect;
			FLOAT					fFrameRate;
		protected:
			IDirect3D9 *			pGraph;
			IDirect3DDevice9 *		pDevice;
			IDirect3DSurface9 *		pSurface;
			IDirect3DSurface9 *		pZBuffer;
			D3DPRESENT_PARAMETERS	cD3Dpps;
			D3DXVECTOR3				vViewer;
			D3DXVECTOR3				vLookat;
			D3DXVECTOR3				vHeadup;
			D3DVIEWPORT9			vViewport;
			D3DVIEWPORT9			vDestport;
			LISTSURFACE				cListSurface;
			LISTSHADER				cListShader;
		public:
			WNDFUNC					pCommand;
		public:
			 GRAPH9();
			~GRAPH9();
		protected:
			void		Defaults();
			bool		FindChar(FONT9 &, wchar_t, CHAR9 *);
			bool		InitResource(SURFACE9 **);
			bool		FreeResource(SURFACE9 **);
			bool		InitResource(SHADER9 **);
			bool		FreeResource(SHADER9 **);
		private:
			static	LRESULT CALLBACK	Proc(HWND, UINT, WPARAM, LPARAM);
		public:
			inline	HWND					GetWindowPtr(){	return hWindow; }
			inline	IDirect3DDevice9 *		GetDevicePtr(){ return pDevice; }
			inline	float					GetFrameRate(){ return fFrameRate; }
			inline	core::VECT3f			GetViewerPos(){ return Vect(vViewer); }
			inline	core::VECT3f			GetLookatPos(){ return Vect(vLookat); }
			inline	core::VECT3f			GetHeadupPos(){ return Vect(vHeadup); }
			inline	void					SetViewerPos(const core::VECT3f &_in){ vViewer = Vect(_in); }
			inline	void					SetLookatPos(const core::VECT3f &_in){ vLookat = Vect(_in); }
			inline	void					SetHeadupPos(const core::VECT3f &_in){ vHeadup = Vect(_in); }
			inline	D3DPRESENT_PARAMETERS	GetPresentParam(){ return (cD3Dpps); }
			inline	void					PostQuit(){ SendNotifyMessage(hWindow, WM_CLOSE, 0, 0); }
			inline	void					SetCommand(WNDFUNC _in){ if(_in == NULL){ _in = Null; } pCommand = _in; }
		public:
			bool			Init(const core::VECT2i &_Size, const wchar_t *_Title, const wchar_t *_Icon = NULL, const wchar_t *_Menu = NULL);
			bool			Init(const core::VECT2i &_Size, int _Windowed, int _DWM, const wchar_t *_Title, const wchar_t *_Icon = NULL);
			bool			Quit();
			bool			Ctrl();
			bool			Swap(int _Windowed, int _DWM = (TRUE));
			bool			ClearDepth();
			bool			GraphSet(const core::COLORf &_color = core::COLORf(0.5f, 0.5f, 0.5f, 1.0f));
			bool			GraphEnd();
			void			SetMatrixOrth(float _Zn, float _Zf);
			void			SetMatrixOrth(float _Zn, float _Zf, core::VECT2i &_Size);
			void			SetMatrixPers(float _Zn, float _Zf, float _Va);
			void			SetMatrixPers(float _Zn, float _Zf, float _Va, float _Aspect);
			void			SetViewport(const core::VECT2i &_From, const core::VECT2i &_Size, const core::COLORf &_color = core::COLORf(0.5f, 0.5f, 0.5f, 1.0f));
			void			EndViewport();
			void			SetLightVec(const core::VECT3f &_in, const core::COLORf &_Ambient = core::COLORf(0.00f, 0.00f, 0.00f, 0.00f), const core::COLORf &_Diffuse = core::COLORf(0.85f, 0.85f, 0.85f, 1.00f), const core::COLORf &_Specular = core::COLORf(0.15f, 0.15f, 0.15f, 1.00f));
		public:
			MOUSEINFO		GetMouseInfo();
		public:
			void			SetBlendDefault();
			void			SetBlendAdd();
			void			SetBlendSub();
			void			SetBlendMod();
			void			SetBlendInv();
			void			SetSamplerStateNone();
			void			SetSamplerStatePoint();
			void			SetSamplerStateLinear();
			void			SetSamplerStateAsiotropic();
		public:
			bool			DrawLine(const core::VECT3f &_Point, const core::VECT2f &_Size, const core::COLORf &_Color);
			bool			DrawRect(const core::VECT3f &_Point, const core::VECT2f &_Size, const core::COLORf &_Color);
		public:
			bool			InitPrimitive(PRIMITIVE2D9X &_Prm, unsigned int _Buffer, unsigned int _Primitive);
			bool			QuitPrimitive(PRIMITIVE2D9X &_Prm);
			bool			PostPrimitive(PRIMITIVE2D9X &_Prm, const core::VECT3f &_Point, const core::COLORf &_Color);
			bool			DrawPrimitive(PRIMITIVE2D9X &_Prm);
			bool			BindPrimitive(PRIMITIVE2D9X &_Prm, unsigned int _NewPrimitive);
		public:
			bool			InitPrimitive(PRIMITIVE3D9X &_Prm, unsigned int _Buffer, unsigned int _Primitive);
			bool			QuitPrimitive(PRIMITIVE3D9X &_Prm);
			bool			PostPrimitive(PRIMITIVE3D9X &_Prm, const core::VECT3f &_Point, const core::COLORf &_Color);
			bool			DrawPrimitive(PRIMITIVE3D9X &_Prm);
			bool			BindPrimitive(PRIMITIVE3D9X &_Prm, unsigned int _NewPrimitive);
		public:
			bool			InitPrimitive(PRIMITIVE3D9N &_Prm, unsigned int _Buffer, unsigned int _Primitive);
			bool			QuitPrimitive(PRIMITIVE3D9N &_Prm);
			bool			PostPrimitive(PRIMITIVE3D9N &_Prm, const core::VECT3f &_Point, const core::VECT3f &_Normal, const core::COLORf &_Color);
			bool			DrawPrimitive(PRIMITIVE3D9N &_Prm);
			bool			BindPrimitive(PRIMITIVE3D9N &_Prm, unsigned int _NewPrimitive);
		public:
			bool			InitPrimitive(PRIMITIVE3D9W &_Prm, unsigned int _Buffer, unsigned int _Primitive);
			bool			QuitPrimitive(PRIMITIVE3D9W &_Prm);
			bool			PostPrimitive(PRIMITIVE3D9W &_Prm, const core::VECT3f &_Point, const core::VECT3f &_Normal, const core::COLORf &_Color, const core::VECT2f &_Coord);
			bool			DrawPrimitive(PRIMITIVE3D9W &_Prm, TEXTURE9 &_Tex);
			bool			BindPrimitive(PRIMITIVE3D9W &_Prm, unsigned int _NewPrimitive);
		public:
			bool			InitTexture(TEXTURE9 &_Tex, const core::COLORf &_Color, const wchar_t *_FileName);
			bool			InitTexture(TEXTURE9 &_Tex, const core::COLORf &_Color, const unsigned char *_Stream, unsigned int _Count);
			bool			InitTexture(TEXTURE9 &_Tex, const core::COLORf &_Color, HMODULE _Module, const wchar_t *_Resource);
			bool			InitTexture(TEXTURE9 &_Tex, const core::COLORf &_Color, const core::VECT2i &_Size);
			bool			CopyTexture(TEXTURE9 &_Tex, TEXTURE9 &_Src);
			bool			QuitTexture(TEXTURE9 &_Tex);
			bool			FillTexture(TEXTURE9 &_Tex, const core::VECT3f &_Point, const core::COLORf &_Color);
			bool			DrawTexture(TEXTURE9 &_Tex, const core::VECT3f &_Point, const core::COLORf &_Color);
			bool			DrawTexture(TEXTURE9 &_Tex, const core::VECT3f &_Point, const core::COLORf &_Color, float _Rotate);
			bool			DrawTexture(TEXTURE9 &_Tex, const core::VECT3f &_Point, const core::COLORf &_Color, float _Rotate, float _Extend);
			bool			SaveTexture(TEXTURE9 &_Tex, const wchar_t *_FileName, unsigned int _Format = D3DXIFF_BMP);
		public:
			bool			InitTexture(TEXTURE2D9 &_Tex, unsigned int _Buffer, const core::COLORf &_Color, const wchar_t *_FileName);
			bool			InitTexture(TEXTURE2D9 &_Tex, unsigned int _Buffer, const core::COLORf &_Color, const unsigned char *_Stream, unsigned int _Count);
			bool			InitTexture(TEXTURE2D9 &_Tex, unsigned int _Buffer, const core::COLORf &_Color, HMODULE _Module, const wchar_t *_Resource);
			bool			InitTexture(TEXTURE2D9 &_Tex, unsigned int _Buffer, const core::COLORf &_Color, const core::VECT2i &_Size);
			bool			CopyTexture(TEXTURE2D9 &_Tex, unsigned int _Buffer, TEXTURE9 &_Src);
			bool			QuitTexture(TEXTURE2D9 &_Tex);
			bool			PostTexture(TEXTURE2D9 &_Tex, const core::VECT3f &_Point, const core::VECT2f &_Size, const core::VECT4f &_Coord, const core::COLORf &_Color);
			bool			PostTexture(TEXTURE2D9 &_Tex, const core::VECT3f &_Point, const core::VECT2f &_Size, const core::VECT4f &_Coord, const core::COLORf &_Color, float _Rotate);
			bool			PostTexture(TEXTURE2D9 &_Tex, const core::VECT3f &_Point, const core::VECT2f &_Size, const core::VECT4f &_Coord, const core::COLORf &_Color, float _Rotate, float _Extend);
			bool			DrawTexture(TEXTURE2D9 &_Tex);
		public:
			bool			InitTexture(TEXTURE3D9 &_Tex, unsigned int _Buffer, const core::COLORf &_Color, const wchar_t *_FileName);
			bool			InitTexture(TEXTURE3D9 &_Tex, unsigned int _Buffer, const core::COLORf &_Color, const unsigned char *_Stream, unsigned int _Count);
			bool			InitTexture(TEXTURE3D9 &_Tex, unsigned int _Buffer, const core::COLORf &_Color, HMODULE _Module, const wchar_t *_Resource);
			bool			InitTexture(TEXTURE3D9 &_Tex, unsigned int _Buffer, const core::COLORf &_Color, const core::VECT2i &_Size);
			bool			CopyTexture(TEXTURE3D9 &_Tex, unsigned int _Buffer, TEXTURE9 &_Src);
			bool			QuitTexture(TEXTURE3D9 &_Tex);
			bool			PostTexture(TEXTURE3D9 &_Tex, const core::VECT3f &_Point, const core::VECT2f &_Size, const core::VECT4f &_Coord, const core::COLORf &_Color);
			bool			PostTexture(TEXTURE3D9 &_Tex, const core::VECT3f &_Point, const core::VECT2f &_Size, const core::VECT4f &_Coord, const core::COLORf &_Color, float _Rotate);
			bool			PostTexture(TEXTURE3D9 &_Tex, const core::VECT3f &_Point, const core::VECT2f &_Size, const core::VECT4f &_Coord, const core::COLORf &_Color, float _Rotate, float _Extend);
			bool			DrawTexture(TEXTURE3D9 &_Tex);
		public:
			bool			InitMesh(MESH9 &_Mesh, const wchar_t *_FileName);
			bool			InitMesh(MESH9 &_Mesh, const unsigned char *_Stream, unsigned int _Count);
			bool			InitMesh(MESH9 &_Mesh, const MESH9CREATEPARAM &_in, const core::VECT4f &_Diffuse = core::VECT4f(0.80f, 0.80f, 0.80f, 1.00f), const core::VECT4f &_Specular = core::VECT4f(0.20f, 0.20f, 0.20f, 0.20f), const core::VECT4f &_Emissive = core::VECT4f(0.0f), float _Power = 5.0f);
			bool			QuitMesh(MESH9 &_Mesh);
			bool			DrawMesh(MESH9 &_Mesh, MATRIX *_Mat, float _Alpha);
			bool			DrawMesh(MESH9 &_Mesh, MATRIX *_Mat, float _Alpha, const core::VECT4f &_Diffuse, const core::VECT4f &_Specular = core::VECT4f(0.20f, 0.20f, 0.20f, 0.20f), const core::VECT4f &_Emissive = core::VECT4f(0.0f), float _Power = 5.0f);
			bool			DrawMeshTexture(MESH9 &_Mesh, MATRIX *_Mat, float _Alpha);
		public:
			bool			InitMesh(MESH9EX &_Mesh, const wchar_t *_FileName);
			bool			InitMesh(MESH9EX &_Mesh, const unsigned char *_Stream, unsigned int _Count);
			bool			InitMesh(MESH9EX &_Mesh, const MESH9CREATEPARAM &_in, const core::VECT4f &_Diffuse = core::VECT4f(0.80f, 0.80f, 0.80f, 1.00f), const core::VECT4f &_Specular = core::VECT4f(0.20f, 0.20f, 0.20f, 0.20f), const core::VECT4f &_Emissive = core::VECT4f(0.0f), float _Power = 5.0f);
			bool			QuitMesh(MESH9EX &_Mesh);
			bool			DrawMesh(MESH9EX &_Mesh, MATRIX *_Mat, float _Alpha);
			bool			DrawMesh(MESH9EX &_Mesh, MATRIX *_Mat, float _Alpha, const core::VECT4f &_Diffuse, const core::VECT4f &_Specular = core::VECT4f(0.20f, 0.20f, 0.20f, 0.20f), const core::VECT4f &_Emissive = core::VECT4f(0.0f), float _Power = 5.0f);
			bool			BindMesh(MESH9EX &_Mesh, TEXTURE9 &_Texture, unsigned int _Count);
		public:
			bool			InitText(FONT9 &_Font, const wchar_t *_FontName, int _Size, int _Quality);
			bool			QuitText(FONT9 &_Font);
			float			DrawTexts(FONT9 &_Font, const core::VECT3f &_Point, const core::COLORf &_ColorA, const wchar_t *_Text);
			float			DrawTextf(FONT9 &_Font, const core::VECT3f &_Point, const core::COLORf &_ColorA, const wchar_t *_Text, ...);
			float			DrawTexts(FONT9 &_Font, const core::VECT3f &_Point, const core::COLORf &_ColorA, float _Shift, const wchar_t *_Text);
			float			DrawTextf(FONT9 &_Font, const core::VECT3f &_Point, const core::COLORf &_ColorA, float _Shift, const wchar_t *_Text, ...);
			float			DrawTexts(FONT9 &_Font, const core::VECT3f &_Point, const core::COLORf &_ColorA, const core::VECT2f &_Scale, const wchar_t *_Text);
			float			DrawTextf(FONT9 &_Font, const core::VECT3f &_Point, const core::COLORf &_ColorA, const core::VECT2f &_Scale, const wchar_t *_Text, ...);
			float			DrawTexts(FONT9 &_Font, const core::VECT3f &_Point, const core::COLORf &_ColorA, const core::COLORf &_ColorB, const wchar_t *_Text);
			float			DrawTextf(FONT9 &_Font, const core::VECT3f &_Point, const core::COLORf &_ColorA, const core::COLORf &_ColorB, const wchar_t *_Text, ...);
			float			DrawTexts(FONT9 &_Font, const core::VECT3f &_Point, const core::COLORf &_ColorA, const core::COLORf &_ColorB, float _Shift, const wchar_t *_Text);
			float			DrawTextf(FONT9 &_Font, const core::VECT3f &_Point, const core::COLORf &_ColorA, const core::COLORf &_ColorB, float _Shift, const wchar_t *_Text, ...);
			float			DrawTexts(FONT9 &_Font, const core::VECT3f &_Point, const core::COLORf &_ColorA, const core::COLORf &_ColorB, const core::VECT2f &_Scale, const wchar_t *_Text);
			float			DrawTextf(FONT9 &_Font, const core::VECT3f &_Point, const core::COLORf &_ColorA, const core::COLORf &_ColorB, const core::VECT2f &_Scale, const wchar_t *_Text, ...);
			core::VECT2f	DrawTexts(FONT9 &_Font, const core::VECT3f &_Point, const core::VECT2f &_Size, float _Pitch, const core::COLORf &_ColorA, const wchar_t *_Text);
			core::VECT2f	DrawTextf(FONT9 &_Font, const core::VECT3f &_Point, const core::VECT2f &_Size, float _Pitch, const core::COLORf &_ColorA, const wchar_t *_Text, ...);
			core::VECT2f	DrawTexts(FONT9 &_Font, const core::VECT3f &_Point, const core::VECT2f &_Size, float _Pitch, const core::COLORf &_ColorA, const core::COLORf &_ColorB, const wchar_t *_Text);
			core::VECT2f	DrawTextf(FONT9 &_Font, const core::VECT3f &_Point, const core::VECT2f &_Size, float _Pitch, const core::COLORf &_ColorA, const core::COLORf &_ColorB, const wchar_t *_Text, ...);
			float			GetWidths(FONT9 &_Font, const wchar_t *_Text);
			float			GetWidthf(FONT9 &_Font, const wchar_t *_Text, ...);
		public:
			bool			InitSurface(SURFACE9 &_Surface, const owl::core::VECT2i &_Size = owl::core::VECT2i(0, 0));
			bool			QuitSurface(SURFACE9 &_Surface);
			bool			FillSurface(SURFACE9 &_Surface, const core::VECT3f &_Point, const core::COLORf &_Color);
			bool			DrawSurface(SURFACE9 &_Surface, const core::VECT3f &_Point, const core::COLORf &_Color);
			bool			DrawSurface(SURFACE9 &_Surface, const core::VECT3f &_Point, const core::COLORf &_Color, float _Rotate);
			bool			DrawSurface(SURFACE9 &_Surface, const core::VECT3f &_Point, const core::COLORf &_Color, float _Rotate, float _Extend);
			bool			SetSection(SURFACE9 &_Surface, const owl::core::COLORf _Color = owl::core::COLORf(0.0f));
			bool			EndSection(SURFACE9 &_Surface);
		public:
			bool			InitShader(SHADER9 &_Shader, const wchar_t *_FileName);
			bool			InitShader(SHADER9 &_Shader, const unsigned char *_Stream, unsigned int _Count);
			bool			QuitShader(SHADER9 &_Shader);
			bool			SetSection(SHADER9 &_Shader, unsigned int _Pass = 0);
			bool			EndSection(SHADER9 &_Shader);
			bool			SetTechnique(SHADER9 &_Shader, const wchar_t *_Tech);
		};


		//***********************************************************************************************************************************************************
		//
		//		Input
		//
		//***********************************************************************************************************************************************************

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		Extern
		//
		extern	const wchar_t *	CvtStr(unsigned char _in);

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		Const
		//
		const	int		MAX_INPUTRANGE	= (+1000);
		const	int		MIN_INPUTRANGE	= (-1000);
		const	int		KEY_BUTTONS		= 256;
		const	int		JOY_BUTTONS		= 128;

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		DIKEYSTATE2
		//
		typedef struct _DIKEYSTATE2 {
			unsigned char	rgbButtons[KEY_BUTTONS];
		} DIKEYSTATE2, *LPDIKEYSTATE2;

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		DIJOYPARAM2
		//
		typedef struct _DIJOYPARAM2 {
			bool			bEnable;
			int				iDetect;
			int				iAnalog;
		} DIJOYPARAM2, *LPDIJOYPARAM2;

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		KEYDEVICE
		//
		class KEYDEVICE {
		public:
			HWND					hWindow;
			IDirectInputDevice8W *	pDevice;
			DIKEYSTATE2				cStatus;
		public:
			KEYDEVICE(){
				hWindow		= NULL;
				pDevice		= NULL;
				memset(&cStatus, 0, sizeof(DIKEYSTATE2));
			}
		};
		
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		JOYDEVICE
		//
		class JOYDEVICE {
		public:
			IDirectInputDevice8W *	pDevice;
			IDirectInputEffect *	pEffect;
			DIJOYSTATE2				cStatus;
			DIJOYPARAM2				cParams;
		public:
			JOYDEVICE(){
				pDevice		= NULL;
				pEffect		= NULL;
				memset(&cStatus, 0, sizeof(DIJOYSTATE2));
				memset(&cParams, 0, sizeof(DIJOYPARAM2));
			}
		};
		
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		Typedef
		//
		typedef	std::vector<JOYDEVICE>				VECTORJOYPAD;
		typedef	std::vector<JOYDEVICE>::iterator	VECTORJOYPADITERATOR;

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		INPUT8
		//
		class INPUT8 {
		protected:
			IDirectInput8W *	pInput;
			KEYDEVICE			vKeypad;
			VECTORJOYPAD		vJoypad;
		public:
					 INPUT8();
			virtual	~INPUT8();
		public:
			virtual	bool	Init(HWND _hWnd);
			virtual	bool	Quit();
			virtual	bool	Ctrl();
		public:
			virtual	bool	GetKeyButtons(unsigned int _Input, unsigned int _Device = 0);
			virtual	bool	GetJoyButtons(unsigned int _Input, unsigned int _Device = 0);
			//-----	<Digital>
			virtual	bool	GetJoyDirectL(unsigned int _Device = 0);
			virtual	bool	GetJoyDirectR(unsigned int _Device = 0);
			virtual	bool	GetJoyDirectU(unsigned int _Device = 0);
			virtual	bool	GetJoyDirectD(unsigned int _Device = 0);
			//-----	<Digital>
			virtual	int		GetJoyDirectDx(unsigned int _Device = 0);
			virtual	int		GetJoyDirectDy(unsigned int _Device = 0);
			//-----	<Analog>
			virtual	int		GetJoyDirectLx(unsigned int _Device = 0);
			virtual	int		GetJoyDirectLy(unsigned int _Device = 0);
			virtual	int		GetJoyDirectRx(unsigned int _Device = 0);
			virtual	int		GetJoyDirectRy(unsigned int _Device = 0);
			//-----	<Count>
			virtual	unsigned int	GetJoyCount();
			//-----	<Config>
			virtual	bool			SetJoyEffectEnable(bool _Enable, unsigned int _Device = 0); 
			virtual	bool			SetJoyDirectDetect( int _Detect, unsigned int _Device = 0); 
			virtual	bool			SetJoyAnalogDetect( int _Detect, unsigned int _Device = 0); 
		private:
			static	BOOL CALLBACK CallbackJoypad(const DIDEVICEINSTANCE *      , void *);
			static	BOOL CALLBACK CallbackObject(const DIDEVICEOBJECTINSTANCE *, void *);
			static	BOOL CALLBACK CallbackEffect(const DIEFFECTINFO *          , void *);
		};

		//***********************************************************************************************************************************************************
		//
		//		Audio
		//
		//***********************************************************************************************************************************************************

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		Define
		//
		const	int			FADEs			= (+2);
		const	int			FADEi			= (+1);
		const	int			FADEe			= ( 0);
		const	int			FADEo			= (-1);
		const	int			MAX_VOLUME		= (10000);
		const	int			MIN_VOLUME		= (    0);
		const	float		AUDIO_GAIN		= (1.0f / 16.0f);

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		Declaration
		//
		class	STATIC;
		class	STREAM;
		
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		Typedef
		//
		typedef	bool	(*STATICLOADER)(IDirectSound8 *, STATIC *, const wchar_t *);
		typedef	bool	(*STREAMLOADER)(IDirectSound8 *, STREAM *, const wchar_t *);

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		Loader
		//
		namespace loader {

			extern	bool	StaticWAV(IDirectSound8 *, STATIC *, const wchar_t *);
			extern	bool	StaticOGG(IDirectSound8 *, STATIC *, const wchar_t *);

			extern	bool	StreamWAV(IDirectSound8 *, STREAM *, const wchar_t *);
			extern	bool	StreamOGG(IDirectSound8 *, STREAM *, const wchar_t *);

		};

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		STATIC
		//
		class STATIC {
		public:
			IDirectSoundBuffer8	*	pBuffer;
		public:
			STATIC(){
				pBuffer	= NULL;
			}
			virtual	~STATIC(){
				SafeAudioRelease(pBuffer);
			}
		};

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		STREAM
		//
		class STREAM : public STATIC {
		public:
			IDirectSoundBuffer8	*	pBuffer;
		public:
			HANDLE			hEvents;
			HANDLE			hThread;
			int				iVolume;
			int				iPosition;
			int				iVelocity;
			unsigned int	uHeadSize;
			unsigned int	uWaveSize;
			unsigned int	uDataSize;
			unsigned int	uInfoSize;
			unsigned int	uNextSize;
			unsigned int	uFileLoop;
			unsigned int	uFileRead;
			unsigned char *	pWaveData;
			wchar_t			cFilePath[MAX_PATH];
		public:
			STREAM(){
				pBuffer		= NULL;
				hEvents		= NULL;
				hThread		= NULL;
				iVolume		= 0;
				iPosition	= 0;
				iVelocity	= 0;
				uHeadSize	= 0;
				uWaveSize	= 0;
				uDataSize	= 0;
				uInfoSize	= 0;
				uNextSize	= 0;
				uFileLoop	= 0;
				uFileRead	= 0;
				pWaveData	= NULL;
				memset(cFilePath, 0, sizeof(wchar_t) * MAX_PATH);
			}
			virtual	~STREAM(){
				if(pBuffer){
					pBuffer->Stop();
				}
				if(hEvents){
					CloseHandle(hEvents);
				}
				if(hThread){
					CloseHandle(hThread);
				}
				SafeRelease(pBuffer);
				SafeDeleteArray(pWaveData);
			}
		};

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		Typedef
		//
		typedef	std::list<STREAM *>				VOLUMEMIXER;
		typedef	std::list<STREAM *>::iterator	VOLUMEMIXERITERATOR;

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		AUDIO8
		//
		class AUDIO8 {
		protected:
			IDirectSound8 *			pAudio;
			IDirectSoundBuffer8 *	pDummy;
			VOLUMEMIXER				cMixer;
		public:
					 AUDIO8();
			virtual ~AUDIO8();
		protected:
			virtual	bool		Init(HWND _hWnd);
			virtual	bool		Quit();
			virtual	bool		Ctrl();
		public:	//-----	<STATIC>
			virtual	bool		CreateStatic(STATIC &_Static, STATICLOADER _Loader, const wchar_t *_File);
			virtual	bool		DeleteStatic(STATIC &_Static);
			virtual	bool		SharedStatic(STATIC &_Static, STATIC &_Source);
			virtual	bool		Play(STATIC &_Static);
			virtual	bool		Stop(STATIC &_Static);
			virtual	bool		SetVolume(STATIC &_Static, int _Volume);
		public:	//-----	<STREAM>
			virtual	bool		CreateStream(STREAM &_Stream, STREAMLOADER _Loader, const wchar_t *_File);
			virtual	bool		DeleteStream(STREAM &_Stream);
			virtual	bool		Play(STREAM &_Stream, int _Fade = FADEi);
			virtual	bool		Stop(STREAM &_Stream, int _Fade = FADEo);
			virtual	bool		SetVolume(STREAM &_Stream, int _Volume);
			virtual	bool		SetLooped(STREAM &_Stream, unsigned int _Looped);
		};

	};
};

#endif