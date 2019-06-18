#include"owlWindows.hpp"

//***********************************************************************************************************************************************************************
//		Static	タイマー値
//***********************************************************************************************************************************************************************
static	unsigned int	_Start	= 0;
static	unsigned int	_Count	= 0;

//***********************************************************************************************************************************************************************
//		owl::core::TimeInit()
//			[desc]
//				タイマー関連の初期化をする
//***********************************************************************************************************************************************************************
unsigned int owl::core::TimeInit(){
	timeBeginPeriod(1);
	return (_Start	= timeGetTime());
}

//***********************************************************************************************************************************************************************
//		owl::core::TimeQuit()
//			[desc]
//				タイマー関連の解除をする
//***********************************************************************************************************************************************************************
unsigned int owl::core::TimeQuit(){
	timeEndPeriod(1);
	return (timeGetTime() - _Start);
}

//***********************************************************************************************************************************************************************
//		owl::core::TimeGain()
//			[desc]
//				タイマーを初期化してからの時間を計測
//***********************************************************************************************************************************************************************
unsigned int owl::core::TimeGain(){
	return (timeGetTime() - _Start);
}

//***********************************************************************************************************************************************************************
//		owl::core::CountSet()
//			[desc]
//				カウンタを零化
//***********************************************************************************************************************************************************************
unsigned int owl::core::CountSet(){
	_Count	= timeGetTime();
	return (0);
}

//***********************************************************************************************************************************************************************
//		owl::core::CountRef()
//			[desc]
//				カウンタを取得
//***********************************************************************************************************************************************************************
unsigned int owl::core::CountRef(){
	return (timeGetTime() - _Count);
}

//***********************************************************************************************************************************************************************
//		owl::core::CountEnd()
//			[desc]
//				カウンタを終了
//***********************************************************************************************************************************************************************
unsigned int owl::core::CountEnd(){
	unsigned int	_Timer;
	_Timer	= timeGetTime() - _Count;
	_Count	= 0;
	return (_Timer);
}