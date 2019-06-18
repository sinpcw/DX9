#ifndef	__OWL_EXTERNAL_HPP_
#define	__OWL_EXTERNAL_HPP_
#include<dsfmt.h>
#ifdef	_DEBUG
	#pragma	comment(lib, "dsfmt_d.lib")
#else
	#pragma	comment(lib, "dsfmt.lib")
#endif

namespace owl {

	//###################################################################################################################################################################
	//
	//		rand
	//
	namespace rand {

		extern	void			Init(unsigned int _in, unsigned int _call = 0);
		extern	float			Gain();
		extern	unsigned int	Call();
		extern	float			GetFlt();
		extern	int				GetInt();
		extern	int				GetInt(unsigned int _in);
		extern	int				GetIdx();
		extern	void			SetIdx(int _in);

		//---------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//		RAND
		//
		class RAND {
		protected:
			dsfmt_t			tRand;
			unsigned int	uSeed;
			unsigned int	uCall;
		public:
			RAND(){
				uSeed	= 0;
				uCall	= 0;
			}
			inline bool		Init(unsigned int _seed, unsigned int _call = 0){
				uSeed	= _seed;
				uCall	= _call;
				dsfmt_init_gen_rand(&tRand, _seed);
				for(unsigned int i = 0; i < _call; i++){
					dsfmt_genrand_close_open(&tRand);
				}
				return (true);
			}
			inline int		Int32()		{ uCall++; return static_cast<int  >(dsfmt_genrand_uint32(&tRand) & 0x7FFFFFFF); }
			inline float	Float()		{ uCall++; return static_cast<float>(dsfmt_genrand_close_open(&tRand));			 }
			inline unsigned int	Seed()	{ return uSeed; }
			inline unsigned int	Call()	{ return uCall; }
		};
	};
};

#endif