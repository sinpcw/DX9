#include"owlEngine9.hpp"

//***********************************************************************************************************************************************************************
//		owl::engine::engine::INPUT8::INPUT8()
//***********************************************************************************************************************************************************************
owl::engine::INPUT8::INPUT8(){
	pInput	= NULL;
}

//***********************************************************************************************************************************************************************
//		owl::INPUT8::~INPUT8()
//***********************************************************************************************************************************************************************
owl::engine::INPUT8::~INPUT8(){
	Quit();
}

//***********************************************************************************************************************************************************************
//		owl::INPUT8::Init(HWND)
//***********************************************************************************************************************************************************************
bool owl::engine::INPUT8::Init(HWND _hWnd){
	if(pInput != NULL){
		return (true);
	}
	//-----	<DirectInput8 作成>
	if(FAILED(DirectInput8Create(reinterpret_cast<HINSTANCE>(GetWindowLong(_hWnd, GWL_HINSTANCE)), DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void **>(&pInput), NULL))){
		return (false);
	}
	//-----	<キーボードデバイス>
	if(FAILED(pInput->CreateDevice(GUID_SysKeyboard, &vKeypad.pDevice, NULL))){
		SafeRelease(vKeypad.pDevice);
		goto CREATE_JOYPAD;
	}
	if(FAILED(vKeypad.pDevice->SetDataFormat(&c_dfDIKeyboard))){
		SafeRelease(vKeypad.pDevice);
		goto CREATE_JOYPAD;
	}
	if(FAILED(vKeypad.pDevice->SetCooperativeLevel(_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE))){
		SafeRelease(vKeypad.pDevice);
		goto CREATE_JOYPAD;
	}
	if(FAILED(vKeypad.pDevice->Acquire())){
		SafeRelease(vKeypad.pDevice);
		goto CREATE_JOYPAD;
	}
	vKeypad.hWindow	= _hWnd;

CREATE_JOYPAD:
	//-----	<ジョイパッドデバイス>
	pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, static_cast<LPDIENUMDEVICESCALLBACK>(CallbackJoypad), this, DIEDFL_ATTACHEDONLY | DIEDFL_FORCEFEEDBACK);
	for(VECTORJOYPADITERATOR p = vJoypad.begin(); p != vJoypad.end(); p++){
		GUID			gGuid;
		DWORD			nAxes[2]			= { DIJOFS_X, DIJOFS_Y };
		LONG			lDirection[2]		= { 0 };	
		DIDEVCAPS		diDevCaps			= { sizeof(DIDEVCAPS), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		DIEFFECT		diEffect			= { 0 };
		DIPERIODIC		diPeriodic			= { 0 };
		DIENVELOPE		diEnvelope			= { 0 };
		DIPROPDWORD		diPropAutoCenter	= { 0 };
		if(FAILED(p->pDevice->SetDataFormat(&c_dfDIJoystick2))){
			SafeRelease(p->pDevice);
			continue;
		}
		if(FAILED(p->pDevice->SetCooperativeLevel(_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE))){
			SafeRelease(p->pDevice);
			continue;
		}
		if(FAILED(p->pDevice->GetCapabilities(&diDevCaps))){
			SafeRelease(p->pDevice);
			continue;
		}
		if(FAILED(p->pDevice->EnumObjects(CallbackObject, p->pDevice, DIDFT_AXIS))){
			SafeRelease(p->pDevice);
			continue;
		}
		diPropAutoCenter.diph.dwSize		= sizeof(diPropAutoCenter);
		diPropAutoCenter.diph.dwHeaderSize	= sizeof(DIPROPHEADER);
		diPropAutoCenter.diph.dwObj			= 0;
		diPropAutoCenter.diph.dwHow			= DIPH_DEVICE;
		diPropAutoCenter.dwData				= DIPROPAUTOCENTER_OFF;
		if(FAILED(p->pDevice->SetProperty(DIPROP_AUTOCENTER, &diPropAutoCenter.diph))){
			SafeRelease(p->pDevice);
			continue;
		}
		if(FAILED(p->pDevice->EnumEffects(static_cast<LPDIENUMEFFECTSCALLBACK>(CallbackEffect), &gGuid, DIEFT_PERIODIC))){
			SafeRelease(p->pDevice);
			continue;
		}
		diPeriodic.dwMagnitude				= DI_FFNOMINALMAX;
		diPeriodic.lOffset					= 0;
		diPeriodic.dwPhase					= 0;
		diPeriodic.dwPeriod					= static_cast<DWORD>(0.01f * DI_SECONDS);
		diEnvelope.dwSize					= sizeof(DIENVELOPE);
		diEnvelope.dwAttackLevel			= 0;
		diEnvelope.dwAttackTime				= 0;
		diEnvelope.dwFadeLevel				= 0;
		diEnvelope.dwFadeTime				= 0;
		diEffect.dwSize						= sizeof(DIEFFECT); 
		diEffect.dwFlags					= DIEFF_POLAR | DIEFF_OBJECTOFFSETS; 
		diEffect.dwDuration					= INFINITE;
		diEffect.dwSamplePeriod				= 0;
		diEffect.dwGain						= DI_FFNOMINALMAX;
		diEffect.dwTriggerButton			= DIEB_NOTRIGGER;
		diEffect.dwTriggerRepeatInterval	= 0;
		diEffect.cAxes						= 2;
		diEffect.rgdwAxes					= nAxes;
		diEffect.rglDirection				= &lDirection[0];
		diEffect.lpEnvelope					= &diEnvelope;
		diEffect.cbTypeSpecificParams		= sizeof(diPeriodic);
		diEffect.lpvTypeSpecificParams		= &diPeriodic;
		if(FAILED(p->pDevice->CreateEffect(gGuid, &diEffect, &p->pEffect, NULL))){
			SafeRelease(p->pDevice);
			continue;
		}
		p->cParams.bEnable		= true;
		p->cParams.iDetect		= 0;
		p->cParams.iAnalog		= 0;
	}
	//-----	<ジョイパッドデバイス>
	pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, static_cast<LPDIENUMDEVICESCALLBACK>(CallbackJoypad), this, DIEDFL_ATTACHEDONLY);
	for(VECTORJOYPADITERATOR p = vJoypad.begin(); p != vJoypad.end(); p++){
		DIDEVCAPS	diDevCaps	= { sizeof(DIDEVCAPS), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		if(FAILED(p->pDevice->SetDataFormat(&c_dfDIJoystick2))){
			SafeRelease(p->pDevice);
			continue;
		}
		if(FAILED(p->pDevice->SetCooperativeLevel(_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE))){
			SafeRelease(p->pDevice);
			continue;
		}
		if(FAILED(p->pDevice->GetCapabilities(&diDevCaps))){
			SafeRelease(p->pDevice);
			continue;
		}
		if(FAILED(p->pDevice->EnumObjects(CallbackObject, p->pDevice, DIDFT_AXIS))){
			SafeRelease(p->pDevice);
			continue;
		}
		p->cParams.bEnable		= false;
		p->cParams.iDetect		= 0;
		p->cParams.iAnalog		= 0;
	}
	for(VECTORJOYPADITERATOR p = vJoypad.begin(); p != vJoypad.end(); p++){
		if(p->pDevice == NULL){
			SafeForceRelease(p->pEffect);
			SafeInputRelease(p->pDevice);
			vJoypad.erase(p);
			p = vJoypad.begin();
			break;
		}else{
			p->pDevice->Acquire();
		}
	}
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::INPUT8::Quit()
//***********************************************************************************************************************************************************************
bool owl::engine::INPUT8::Quit(){
	//-----	<キーボード解放>
	SafeInputRelease(vKeypad.pDevice);
	//-----	<ジョイパッド解放>
	for(VECTORJOYPADITERATOR p = vJoypad.begin(); p != vJoypad.end(); p++){
		SafeForceRelease(p->pEffect);
		SafeInputRelease(p->pDevice);
	}
	//-----	<DirectInput8 解放>
	SafeRelease(pInput);
	vJoypad.clear();
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::INPUT8::Ctrl()
//***********************************************************************************************************************************************************************
bool owl::engine::INPUT8::Ctrl(){
	bool	bError	= true;
	//-----	<キーボード>
	if(vKeypad.pDevice == NULL){
		bError	= false;
		memset(vKeypad.cStatus.rgbButtons, 0, sizeof(unsigned char) * owl::engine::KEY_BUTTONS);
	}else if(GetForegroundWindow() != vKeypad.hWindow){
		//-----	not Error
		memset(vKeypad.cStatus.rgbButtons, 0, sizeof(unsigned char) * owl::engine::KEY_BUTTONS);
	}else if(FAILED(vKeypad.pDevice->Acquire())){
		bError	= false;
		memset(vKeypad.cStatus.rgbButtons, 0, sizeof(unsigned char) * owl::engine::KEY_BUTTONS);
	}else if(FAILED(vKeypad.pDevice->GetDeviceState(sizeof(unsigned char) * owl::engine::KEY_BUTTONS, vKeypad.cStatus.rgbButtons))){
		bError	= false;
		memset(vKeypad.cStatus.rgbButtons, 0, sizeof(unsigned char) * owl::engine::KEY_BUTTONS);
	}
	//-----	<ジョイパッド>
	for(VECTORJOYPADITERATOR p = vJoypad.begin(); p != vJoypad.end(); p++){
		if(FAILED(p->pDevice->Acquire())){
			bError	= false;
			memset(&p->cStatus, 0, sizeof(DIJOYSTATE2));
			continue;
		}
		if(FAILED(p->pDevice->Poll())){
			bError	= false;
			memset(&p->cStatus, 0, sizeof(DIJOYSTATE2));
			continue;
		}
		if(FAILED(p->pDevice->GetDeviceState(sizeof(DIJOYSTATE2), &p->cStatus))){
			bError	= false;
			memset(&p->cStatus, 0, sizeof(DIJOYSTATE2));
			continue;
		}
	}
	return (bError);
}

//***********************************************************************************************************************************************************************
//		owl::engine::INPUT8::CallbackJoypad(const DIDEVICEINSTANCE *, void *)
//***********************************************************************************************************************************************************************
BOOL CALLBACK owl::engine::INPUT8::CallbackJoypad(const DIDEVICEINSTANCE *pDevInstance, void *pVoid){
	owl::engine::INPUT8 *	pThis	= static_cast<owl::engine::INPUT8 *>(pVoid);
	IDirectInputDevice8W *	pEnum	= NULL;
	if(SUCCEEDED(pThis->pInput->CreateDevice(pDevInstance->guidInstance, &pEnum, NULL))){
		pThis->vJoypad.push_back(owl::engine::JOYDEVICE());
		pThis->vJoypad.back().pDevice	= pEnum;
	}
	return DIENUM_CONTINUE;
}

//***********************************************************************************************************************************************************************
//		owl::engine::INPUT8::CallbackObject(const DIDEVICEOBJECTINSTANCE *, void *)
//***********************************************************************************************************************************************************************
BOOL CALLBACK owl::engine::INPUT8::CallbackObject(const DIDEVICEOBJECTINSTANCE *pDevObjectInstance, void *pVoid){
	IDirectInputDevice8W *	pDevice	= static_cast<IDirectInputDevice8W *>(pVoid);
	DIPROPRANGE				cDIPropRange;
	
	cDIPropRange.diph.dwSize		= sizeof(DIPROPRANGE);
	cDIPropRange.diph.dwHeaderSize	= sizeof(DIPROPHEADER);
	cDIPropRange.diph.dwHow			= DIPH_BYID;
	cDIPropRange.diph.dwObj			= pDevObjectInstance->dwType;
	cDIPropRange.lMin				= MIN_INPUTRANGE;
	cDIPropRange.lMax				= MAX_INPUTRANGE;
	pDevice->SetProperty(DIPROP_RANGE, &cDIPropRange.diph);

	return DIENUM_CONTINUE;
}

//***********************************************************************************************************************************************************************
//		owl::engine::INPUT8::CallbackEffect(const DIEFFECTINFO *, void *)
//***********************************************************************************************************************************************************************
BOOL CALLBACK owl::engine::INPUT8::CallbackEffect(const DIEFFECTINFO *pEffectInfo, void *pVoid){
	*(reinterpret_cast<GUID *>(pVoid)) = pEffectInfo->guid;
	return DIENUM_STOP;
}

//***********************************************************************************************************************************************************************
//		owl::engine::INPUT8::GetKeyButtons(unsigned int, unsigned int)
//***********************************************************************************************************************************************************************
bool owl::engine::INPUT8::GetKeyButtons(unsigned int _Input, unsigned int _Device){
	assert(_Input < owl::engine::KEY_BUTTONS);
	return (vKeypad.cStatus.rgbButtons[_Input] & 0x80) ? (true) : (false);
}

//***********************************************************************************************************************************************************************
//		owl::engine::INPUT8::GetJoyButtons(unsigned int, unsigned int)
//***********************************************************************************************************************************************************************
bool owl::engine::INPUT8::GetJoyButtons(unsigned int _Input, unsigned int _Device){
	if(vJoypad.size() <= _Device){
		return false;
	}
	assert(_Input < owl::engine::JOY_BUTTONS);
	return (vJoypad[_Device].cStatus.rgbButtons[_Input] & 0x80) ? (true) : (false);
}

//***********************************************************************************************************************************************************************
//		owl::engine::INPUT8::GetJoyDirectL(unsigned int)
//***********************************************************************************************************************************************************************
bool owl::engine::INPUT8::GetJoyDirectL(unsigned int _Device){
	if(vJoypad.size() <= _Device){
		return false;
	}
	return (vJoypad[_Device].cStatus.lX < -static_cast<long>(vJoypad[_Device].cParams.iDetect) || LOWORD(vJoypad[_Device].cStatus.rgdwPOV) == 27000) ? (true) : (false);
}

//***********************************************************************************************************************************************************************
//		owl::engine::INPUT8::GetJoyDirectR(unsigned int)
//***********************************************************************************************************************************************************************
bool owl::engine::INPUT8::GetJoyDirectR(unsigned int _Device){
	if(vJoypad.size() <= _Device){
		return false;
	}
	return (vJoypad[_Device].cStatus.lX > +static_cast<long>(vJoypad[_Device].cParams.iDetect) || LOWORD(vJoypad[_Device].cStatus.rgdwPOV) ==  9000) ? (true) : (false);
}

//***********************************************************************************************************************************************************************
//		owl::engine::INPUT8::GetJoyDirectU(unsigned int)
//***********************************************************************************************************************************************************************
bool owl::engine::INPUT8::GetJoyDirectU(unsigned int _Device){
	if(vJoypad.size() <= _Device){
		return false;
	}
	return (vJoypad[_Device].cStatus.lY < -static_cast<long>(vJoypad[_Device].cParams.iDetect) || LOWORD(vJoypad[_Device].cStatus.rgdwPOV) ==     0) ? (true) : (false);
}

//***********************************************************************************************************************************************************************
//		owl::engine::INPUT8::GetJoyDirectD(unsigned int)
//***********************************************************************************************************************************************************************
bool owl::engine::INPUT8::GetJoyDirectD(unsigned int _Device){
	if(vJoypad.size() <= _Device){
		return false;
	}
	return (vJoypad[_Device].cStatus.lY > +static_cast<long>(vJoypad[_Device].cParams.iDetect) || LOWORD(vJoypad[_Device].cStatus.rgdwPOV) == 18000) ? (true) : (false);
}

//***********************************************************************************************************************************************************************
//		owl::engine::INPUT8::GetJoyDirectDx(unsigned int)
//***********************************************************************************************************************************************************************
int owl::engine::INPUT8::GetJoyDirectDx(unsigned int _Device){
	if(vJoypad.size() <= _Device){
		return ( 0);
	}
	if(LOWORD(vJoypad[_Device].cStatus.rgdwPOV) == 27000){
		return (-1);
	}
	if(LOWORD(vJoypad[_Device].cStatus.rgdwPOV) ==  9000){
		return (+1);
	}
	return (0);
}

//***********************************************************************************************************************************************************************
//		owl::engine::INPUT8::GetJoyDirectDy(unsigned int)
//***********************************************************************************************************************************************************************
int owl::engine::INPUT8::GetJoyDirectDy(unsigned int _Device){
	if(vJoypad.size() <= _Device){
		return ( 0);
	}
	if(LOWORD(vJoypad[_Device].cStatus.rgdwPOV) ==     0){
		return (-1);
	}
	if(LOWORD(vJoypad[_Device].cStatus.rgdwPOV) == 18000){
		return (+1);
	}
	return (0);
}

//***********************************************************************************************************************************************************************
//		owl::engine::INPUT8::GetJoyDirectLx(unsigned int)
//***********************************************************************************************************************************************************************
int owl::engine::INPUT8::GetJoyDirectLx(unsigned int _Device){
	if(vJoypad.size() <= _Device){
		return (0);
	}
	return (abs(vJoypad[_Device].cStatus.lX ) < static_cast<long>(abs(vJoypad[_Device].cParams.iAnalog))) ? (0) : (vJoypad[_Device].cStatus.lX );
}

//***********************************************************************************************************************************************************************
//		owl::engine::INPUT8::GetJoyDirectLy(unsigned int)
//***********************************************************************************************************************************************************************
int owl::engine::INPUT8::GetJoyDirectLy(unsigned int _Device){
	if(vJoypad.size() <= _Device){
		return (0);
	}
	return (abs(vJoypad[_Device].cStatus.lY ) < static_cast<long>(abs(vJoypad[_Device].cParams.iAnalog))) ? (0) : (vJoypad[_Device].cStatus.lY );
}

#if	(1)	//-----	入力ドライバによって変化する可能性あり
//***********************************************************************************************************************************************************************
//		owl::engine::INPUT8::GetJoyDirectRx(unsigned int)
//***********************************************************************************************************************************************************************
int owl::engine::INPUT8::GetJoyDirectRx(unsigned int _Device){
	if(vJoypad.size() <= _Device){
		return (0);
	}
	return (abs(vJoypad[_Device].cStatus.lZ ) < static_cast<long>(abs(vJoypad[_Device].cParams.iAnalog))) ? (0) : (vJoypad[_Device].cStatus.lZ );
}

//***********************************************************************************************************************************************************************
//		owl::engine::INPUT8::GetJoyDirectRy(unsigned int)
//***********************************************************************************************************************************************************************
int owl::engine::INPUT8::GetJoyDirectRy(unsigned int _Device){
	if(vJoypad.size() <= _Device){
		return (0);
	}
	return (abs(vJoypad[_Device].cStatus.lRz) < static_cast<long>(abs(vJoypad[_Device].cParams.iAnalog))) ? (0) : (vJoypad[_Device].cStatus.lRz);
}
#else
//***********************************************************************************************************************************************************************
//		owl::engine::INPUT8::GetJoyDirectRx(unsigned int)
//***********************************************************************************************************************************************************************
int owl::engine::INPUT8::GetJoyDirectRx(unsigned int _Device){
	if(vJoypad.size() <= _Device){
		return (0);
	}
	return (abs(vJoypad[_Device].cStatus.lRz) < static_cast<long>(abs(vJoypad[_Device].cParams.iAnalog))) ? (0) : (vJoypad[_Device].cStatus.lRz);
}

//***********************************************************************************************************************************************************************
//		owl::engine::INPUT8::GetJoyDirectRy(unsigned int)
//***********************************************************************************************************************************************************************
int owl::engine::INPUT8::GetJoyDirectRy(unsigned int _Device){
	if(vJoypad.size() <= _Device){
		return (0);
	}
	return (abs(vJoypad[_Device].cStatus.lZ ) < static_cast<long>(abs(vJoypad[_Device].cParams.iAnalog))) ? (0) : (vJoypad[_Device].cStatus.lZ );
}
#endif

//***********************************************************************************************************************************************************************
//		owl::engine::INPUT8::GetJoyCount()
//***********************************************************************************************************************************************************************
unsigned int owl::engine::INPUT8::GetJoyCount(){
	return static_cast<unsigned int>(vJoypad.size());
}

//***********************************************************************************************************************************************************************
//		owl::engine::INPUT8::SetJoyEffectEnable(bool, unsigned int)
//***********************************************************************************************************************************************************************
bool owl::engine::INPUT8::SetJoyEffectEnable(bool _Enable, unsigned int _Device){
	if(_Device < vJoypad.size()){
		if(vJoypad[_Device].pEffect != NULL){
			vJoypad[_Device].cParams.bEnable	= _Enable;
			return (true);
		}
		return (false);
	}
	return (false);
}

//***********************************************************************************************************************************************************************
//		owl::engine::INPUT8::SetJoyDirectDetect(int, unsigned int)
//***********************************************************************************************************************************************************************
bool owl::engine::INPUT8::SetJoyDirectDetect(int _Detect, unsigned int _Device){
	if(_Device < vJoypad.size()){
		vJoypad[_Device].cParams.iDetect	= std::max<int>(1, std::min<int>(_Detect, MAX_INPUTRANGE - 1));
		return (true);
	}
	return (false);
}

//***********************************************************************************************************************************************************************
//		owl::engine::INPUT8::SetJoyAnalogDetect(int, unsigned int)
//***********************************************************************************************************************************************************************
bool owl::engine::INPUT8::SetJoyAnalogDetect(int _Detect, unsigned int _Device){
	if(_Device < vJoypad.size()){
		vJoypad[_Device].cParams.iAnalog	= std::max<int>(1, std::min<int>(_Detect, MAX_INPUTRANGE - 1));
		return (true);
	}
	return (false);
}
