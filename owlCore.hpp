#ifndef	__OWL_CORE_HPP_
#define	__OWL_CORE_HPP_
#define	_CRT_SECURE_NO_WARNINGS		(1)
#include<new>
#include<map>
#include<list>
#include<vector>
#include<string>
#include<sstream>
#include<algorithm>
#include<cmath>
#include<ctime>
#include<cstdio>
#include<cassert>
#include<locale.h>
#include<process.h>

#ifndef	SafeDelete
	#define	SafeDelete(p)			{ if(p){ delete    (p);  (p) = NULL; } }
#endif
#ifndef	SafeDeleteArray
	#define	SafeDeleteArray(p)		{ if(p){ delete [] (p);  (p) = NULL; } }
#endif
#ifndef	DOMAIN_NAME
	#define	DOMAIN_NAME				(L"OWL")
#endif

namespace owl {

	//###################################################################################################################################################################
	//
	//		Core
	//
	namespace core {
		
		//---------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		Const
		//
		const	float		PI			= 3.1415926535897932384626433832795f;

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		Inline
		//
		inline	float		DtR(float _f){ return (_f *  0.017453292519943295769236907685f); }
		inline	float		RtD(float _f){ return (_f * 57.295779513082320876798154814105f); }

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		STORAGE
		//
		class STORAGE {
		protected:
			unsigned int	nSize;
			unsigned char *	pData;
		public:
					 STORAGE();
			virtual	~STORAGE();
		public:
			bool		Init(unsigned int _in);
			bool		Quit();
			bool		Load(const wchar_t *_in);
			bool		Save(const wchar_t *_in);
			bool		Bind(const STORAGE &_in);
		public:
			inline	unsigned int	Size(){
				return nSize;
			}
			inline	unsigned char	operator [](unsigned int _in) const {
				return pData[_in];
			}
			inline	unsigned char &	operator [](unsigned int _in){
				return pData[_in];
			}
		};

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		VECTOR2
		//
		template <typename type> class VECTOR2 {
		public:
			type	x, y;
		public:
			inline	VECTOR2<type>(){}
			inline	VECTOR2<type>(type _t){
				x	= _t;
				y	= _t;
			}
			inline	VECTOR2<type>(type _x, type _y){
				x	= _x;
				y	= _y;
			}
			inline	VECTOR2<type>(const VECTOR2<type> &_v){
				x	= _v.x;
				y	= _v.y;
			}
			inline	VECTOR2<type>	operator + (const VECTOR2<type> &_v) const {
				return VECTOR2<type>(x + _v.x, y + _v.y);
			}
			inline	VECTOR2<type>	operator - (const VECTOR2<type> &_v) const {
				return VECTOR2<type>(x - _v.x, y - _v.y);
			}
			inline	VECTOR2<type>	operator * (type _t) const {
				return VECTOR2<type>(x * _t, y * _t);
			}
			inline	VECTOR2<type>	operator / (type _t) const {
				return VECTOR2<type>(x / _t, y / _t);
			}
			inline	VECTOR2<type> &	operator += (const VECTOR2<type> &_v){
				x	+= _v.x;
				y	+= _v.y;
				return (*this);
			}
			inline	VECTOR2<type> &	operator -= (const VECTOR2<type> &_v){
				x	-= _v.x;
				y	-= _v.y;
				return (*this);
			}
			inline	VECTOR2<type> &	operator *= (type _t){
				x	*= _t;
				y	*= _t;
				return (*this);
			}
			inline	VECTOR2<type> &	operator /= (type _t){
				x	/= _t;
				y	/= _t;
				return (*this);
			}
		public:
			inline	type	Abs(){
				return static_cast<type>(sqrt(x * x + y * y));
			}
			inline	type	Rot(){
				return static_cast<type>(atan2(y, x));
			}
		};

		template <typename type> type Dot(const VECTOR2<type> &_a, const VECTOR2<type> &_b){
			return static_cast<type>(_a.x * _b.x + _a.y * _b.y);
		}
		template <typename type> type Det(const VECTOR2<type> &_a, const VECTOR2<type> &_b){
			return static_cast<type>(_a.x * _b.y - _a.y * _b.x);
		}
		template <typename type> type	Distance(const VECTOR2<type> &_a, const VECTOR2<type> &_b){
			return static_cast<type>(sqrt((_a.x - _b.x) * (_a.x - _b.x) + (_a.y - _b.y) * (_a.y - _b.y)));
		}
		template <typename type> type	DistanceSquared(const VECTOR2<type> &_a, const VECTOR2<type> &_b){
			return static_cast<type>((_a.x - _b.x) * (_a.x - _b.x) + (_a.y - _b.y) * (_a.y - _b.y));
		}
		
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		VECTOR3
		//
		template <typename type> class VECTOR3 {
		public:
			type	x, y, z;
		public:
			inline	VECTOR3<type>(){}
			inline	VECTOR3<type>(type _t){
				x	= _t;
				y	= _t;
				z	= _t;
			}
			inline	VECTOR3<type>(type _x, type _y, type _z){
				x	= _x;
				y	= _y;
				z	= _z;
			}
			inline	VECTOR3<type>(const VECTOR3<type> &_v){
				x	= _v.x;
				y	= _v.y;
				z	= _v.z;
			}
			inline	VECTOR3<type>	operator + (const VECTOR3<type> &_v) const {
				return VECTOR3<type>(x + _v.x, y + _v.y, z + _v.z);
			}
			inline	VECTOR3<type>	operator - (const VECTOR3<type> &_v) const {
				return VECTOR3<type>(x - _v.x, y - _v.y, z - _v.z);
			}
			inline	VECTOR3<type>	operator * (type _t) const {
				return VECTOR3<type>(x * _t, y * _t, z * _t);
			}
			inline	VECTOR3<type>	operator / (type _t) const {
				return VECTOR3<type>(x / _t, y / _t, z / _t);
			}
			inline	VECTOR3<type> &	operator += (const VECTOR3<type> &_v){
				x	+= _v.x;
				y	+= _v.y;
				z	+= _v.z;
				return (*this);
			}
			inline	VECTOR3<type> &	operator -= (const VECTOR3<type> &_v){
				x	-= _v.x;
				y	-= _v.y;
				z	-= _v.z;
				return (*this);
			}
			inline	VECTOR3<type> &	operator *= (type _t){
				x	*= _t;
				y	*= _t;
				z	*= _t;
				return (*this);
			}
			inline	VECTOR3<type> &	operator /= (type _t){
				x	/= _t;
				y	/= _t;
				z	/= _t;
				return (*this);
			}
		public:
			inline	type	Abs(){
				return static_cast<type>(sqrt(x * x + y * y + z * z));
			}
			inline	type	Rot(){
				return static_cast<type>(atan2(y, x));
			}
			inline	type	RotAxisX(){
				return static_cast<type>(atan2(y, z));
			}
			inline	type	RotAxisY(){
				return static_cast<type>(atan2(z, x));
			}
			inline	type	RotAxisZ(){
				return static_cast<type>(atan2(y, x));
			}
		};

		template <typename type> type Dot(const VECTOR3<type> &_a, const VECTOR3<type> &_b){
			return static_cast<type>(_a.x * _b.x + _a.y * _b.y + _a.z * _b.z);
		}
		template <typename type> VECTOR3<type> Det(const VECTOR3<type> &_a, const VECTOR3<type> &_b){
			return VECTOR3<type>(_a.y * _b.z - _a.z * _b.y, _a.z * _b.x - _a.x * _b.z, _a.x * _b.y - _a.y * _b.x);
		}
		template <typename type> type	Distance(const VECTOR3<type> &_a, const VECTOR3<type> &_b){
			return static_cast<type>(sqrt((_a.x - _b.x) * (_a.x - _b.x) + (_a.y - _b.y) * (_a.y - _b.y) + (_a.z - _b.z) * (_a.z - _b.z)));
		}
		template <typename type> type	DistanceSquared(const VECTOR3<type> &_a, const VECTOR3<type> &_b){
			return static_cast<type>((_a.x - _b.x) * (_a.x - _b.x) + (_a.y - _b.y) * (_a.y - _b.y) + (_a.z - _b.z) * (_a.z - _b.z));
		}

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		VECTOR4
		//
		template <typename type> class VECTOR4 {
		public:
			type	x, y, z, w;
		public:
			inline	VECTOR4<type>(){}
			inline	VECTOR4<type>(type _t){
				x	= _t;
				y	= _t;
				z	= _t;
				w	= _t;
			}
			inline	VECTOR4<type>(type _x, type _y, type _z, type _w){
				x	= _x;
				y	= _y;
				z	= _z;
				w	= _w;
			}
			inline	VECTOR4<type>(const VECTOR4<type> &_v){
				x	= _v.x;
				y	= _v.y;
				z	= _v.z;
				w	= _v.w;
			}
			inline	VECTOR4<type>	operator + (const VECTOR4<type> &_v) const {
				return VECTOR4<type>(x + _v.x, y + _v.y, z + _v.z, w + _v.w);
			}
			inline	VECTOR4<type>	operator - (const VECTOR4<type> &_v) const {
				return VECTOR4<type>(x - _v.x, y - _v.y, z - _v.z, w - _v.w);
			}
			inline	VECTOR4<type>	operator * (type _t) const {
				return VECTOR4<type>(x * _t, y * _t, z * _t, w * _t);
			}
			inline	VECTOR4<type>	operator / (type _t) const {
				return VECTOR4<type>(x / _t, y / _t, z / _t, w / _t);
			}
			inline	VECTOR4<type> &	operator += (const VECTOR4<type> &_v){
				x	+= _v.x;
				y	+= _v.y;
				z	+= _v.z;
				w	+= _v.w;
				return (*this);
			}
			inline	VECTOR4<type> &	operator -= (const VECTOR4<type> &_v){
				x	-= _v.x;
				y	-= _v.y;
				z	-= _v.z;
				w	-= _v.w;
				return (*this);
			}
			inline	VECTOR4<type> &	operator *= (type _t){
				x	*= _t;
				y	*= _t;
				z	*= _t;
				w	*= _t;
				return (*this);
			}
			inline	VECTOR4<type> &	operator /= (type _t){
				x	/= _t;
				y	/= _t;
				z	/= _t;
				w	/= _t;
				return (*this);
			}
		};

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		COLOR
		//
		template <typename type> class COLOR {
		public:
			type	r, g, b, a;
		public:
			inline	COLOR<type>(){}
			inline	COLOR<type>(type _t){
				r	= _t;
				g	= _t;
				b	= _t;
				a	= _t;
			}
			inline	COLOR<type>(type _r, type _g, type _b, type _a){
				r	= _r;
				g	= _g;
				b	= _b;
				a	= _a;
			}
			inline	COLOR<type>(const COLOR<type> &_v){
				r	= _v.r;
				g	= _v.g;
				b	= _v.b;
				a	= _v.a;
			}
			inline	COLOR<type>	operator + (const COLOR<type> &_v) const {
				return COLOR<tgpe>(r + _v.r, g + _v.g, b + _v.b, a + _v.a);
			}
			inline	COLOR<type>	operator - (const COLOR<type> &_v) const {
				return COLOR<tgpe>(r - _v.r, g - _v.g, b - _v.b, a - _v.a);
			}
			inline	COLOR<type>	operator * (type _t) const {
				return COLOR<tgpe>(r * _t, g * _t, b * _t, a * _t);
			}
			inline	COLOR<type>	operator / (type _t) const {
				return COLOR<tgpe>(r / _t, g / _t, b / _t, a / _t);
			}
			inline	COLOR<type>	operator += (const COLOR<type> &_v){
				r	+= _v.r;
				g	+= _v.g;
				b	+= _v.b;
				a	+= _v.a;
				return (*this);
			}
			inline	COLOR<type>	operator -= (const COLOR<type> &_v){
				r	-= _v.r;
				g	-= _v.g;
				b	-= _v.b;
				a	-= _v.a;
				return (*this);
			}
			inline	COLOR<type>	operator *= (type _t){
				r	*= _t;
				g	*= _t;
				b	*= _t;
				a	*= _t;
				return (*this);
			}
			inline	COLOR<type>	operator /= (type _t){
				r	/= _t;
				g	/= _t;
				b	/= _t;
				a	/= _t;
				return (*this);
			}
		};

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		VECTORs
		//
		typedef	VECTOR2<int>			VECT2i;
		typedef	VECTOR2<float>			VECT2f;
		typedef	VECTOR3<int>			VECT3i;
		typedef	VECTOR3<float>			VECT3f;
		typedef	VECTOR4<int>			VECT4i;
		typedef	VECTOR4<float>			VECT4f;

		typedef	COLOR<unsigned char>	COLOR8;
		typedef	COLOR<int>				COLORi;
		typedef	COLOR<float>			COLORf;

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		Core Dump
		//
		extern	void			DumpInit(const wchar_t *_in);
		extern	void			DumpQuit();
		extern	void			DumpText(const wchar_t *_in, ...);

		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		Core Util
		//
		extern	COLORf			PseudoColor(float _in);
		extern	unsigned int	TrueColor32(const COLOR8 &_in);
		extern	unsigned int	TrueColor32(const COLORi &_in);
		extern	unsigned int	TrueColor32(const COLORf &_in);

		extern	unsigned int	MinSquared(unsigned int _in);

		extern	const  char   *	CvtStrA(const wchar_t *_in);
		extern	const wchar_t *	CvtStrW(const  char   *_in);

		extern	int				GCD(int _in1, int _in2);
		extern	int				LCM(int _in1, int _in2);

		extern	bool			IsExistFile(const wchar_t *);

	};
};

#endif