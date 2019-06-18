#include"owlEngine9.hpp"

//***********************************************************************************************************************************************************************
//		owl::engine::AUDIO8::AUDIO8()
//***********************************************************************************************************************************************************************
owl::engine::AUDIO8::AUDIO8(){
	pAudio	= NULL;
	pDummy	= NULL;
}

//***********************************************************************************************************************************************************************
//		owl::engine::AUDIO8::~AUDIO8()
//***********************************************************************************************************************************************************************
owl::engine::AUDIO8::~AUDIO8(){
	Quit();
}

//***********************************************************************************************************************************************************************
//		owl::engine::AUDIO8::Init(HWND)
//***********************************************************************************************************************************************************************
bool owl::engine::AUDIO8::Init(HWND _hWnd){
	if(pAudio != NULL){
		return true;
	}
	if(FAILED(DirectSoundCreate8(NULL, &pAudio, NULL))){
		return (false);
	}
	if(FAILED(pAudio->SetCooperativeLevel(_hWnd, DSSCL_EXCLUSIVE | DSSCL_PRIORITY))){
		SafeRelease(pAudio);
		return (false);
	}
	//-----	<ダミーバッファ作成>
	DWORD			uSize;
	BYTE *			pWave;
	DSBUFFERDESC	cDescWave		= { 0 };
	WAVEFORMATEX	cWaveFmtx		= { 0 };
	cWaveFmtx.cbSize				= sizeof(WAVEFORMATEX);
	cWaveFmtx.wFormatTag			= WAVE_FORMAT_PCM;
	cWaveFmtx.nChannels				= 2;
	cWaveFmtx.nSamplesPerSec		= 44100;
	cWaveFmtx.wBitsPerSample		= 16;
	cWaveFmtx.nBlockAlign			= cWaveFmtx.wBitsPerSample * cWaveFmtx.nChannels / 8;
	cWaveFmtx.nAvgBytesPerSec		= cWaveFmtx.nSamplesPerSec * cWaveFmtx.nBlockAlign;
	cDescWave.dwSize				= sizeof(DSBUFFERDESC);
	cDescWave.dwFlags				= DSBCAPS_STATIC | DSBCAPS_GLOBALFOCUS;
	cDescWave.dwBufferBytes			= cWaveFmtx.nAvgBytesPerSec * 2;
	cDescWave.lpwfxFormat			= &cWaveFmtx;
	if(FAILED(pAudio->CreateSoundBuffer(&cDescWave, reinterpret_cast<IDirectSoundBuffer **>(&pDummy), NULL))){
		SafeRelease(pAudio);
		return (false);
	}
	if(FAILED(pDummy->Lock(0, cWaveFmtx.nAvgBytesPerSec * 2, reinterpret_cast<void **>(&pWave), &uSize, NULL, 0, 0))){
		SafeAudioRelease(pDummy);
		SafeRelease(pAudio);
		return (false);
	}
	memset(pWave, 0, uSize * sizeof(BYTE));
	if(FAILED(pDummy->Unlock(reinterpret_cast<void *>(pWave), uSize, NULL, 0))){
		SafeAudioRelease(pDummy);
		SafeRelease(pAudio);
		return (false);
	}
	if(FAILED(pDummy->Play(0, 0, DSBPLAY_LOOPING))){
		SafeAudioRelease(pDummy);
		SafeRelease(pAudio);
		return (false);
	}
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::AUDIO8::Quit()
//***********************************************************************************************************************************************************************
bool owl::engine::AUDIO8::Quit(){
	cMixer.clear();
	SafeAudioRelease(pDummy);
	SafeRelease(pAudio);
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::AUDIO8::Ctrl()
//***********************************************************************************************************************************************************************
bool owl::engine::AUDIO8::Ctrl(){
	for(VOLUMEMIXERITERATOR p = cMixer.begin(); p != cMixer.end(); /* no Action */){
		int		iState	= (*p)->iPosition + (*p)->iVelocity;
		if(iState <= MIN_VOLUME){
			(*p)->pBuffer->SetVolume(DSBVOLUME_MIN);
			(*p)	= NULL;
			p		= cMixer.erase(p);
			continue;
		}else if(iState >= (*p)->iVolume){
			(*p)->pBuffer->SetVolume(static_cast<long>((*p)->iVolume) + DSBVOLUME_MIN);
			(*p)	= NULL;
			p		= cMixer.erase(p);		
			continue;
		}
		(*p)->iPosition	+= (*p)->iVelocity;
		(*p)->pBuffer->SetVolume(static_cast<long>((*p)->iPosition) + DSBVOLUME_MIN);
		( p)++;
	}
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::AUDIO8::CreateStatic(STATIC &, STATICLOADER, const wchar_t *)
//***********************************************************************************************************************************************************************
bool owl::engine::AUDIO8::CreateStatic(STATIC &_Static, STATICLOADER _Loader, const wchar_t *_File){
	assert(_File != NULL);
	if(_Static.pBuffer != NULL){
		return (true);
	}
	if(_Loader == NULL){
		return (false);
	}
	return (_Loader(pAudio, &_Static, _File));
}

//***********************************************************************************************************************************************************************
//		owl::engine::AUDIO8::DeleteStatic(STATIC &)
//***********************************************************************************************************************************************************************
bool owl::engine::AUDIO8::DeleteStatic(STATIC &_Static){
	SafeAudioRelease(_Static.pBuffer);
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::AUDIO8::SharedStatic(STATIC &)
//***********************************************************************************************************************************************************************
bool owl::engine::AUDIO8::SharedStatic(STATIC &_Static, STATIC &_Source){
	if(_Source.pBuffer == NULL){
		return (false);
	}
	if(_Static.pBuffer != NULL){
		return (true);
	}
	if(FAILED(pAudio->DuplicateSoundBuffer(_Source.pBuffer, reinterpret_cast<IDirectSoundBuffer **>(&_Static.pBuffer)))){
		return (false);
	}
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::AUDIO8::Play(STATIC &)
//***********************************************************************************************************************************************************************
bool owl::engine::AUDIO8::Play(STATIC &_Static){
	if(_Static.pBuffer == NULL){
		return (false);
	}
	DWORD	nState	= 0;
	if(FAILED(_Static.pBuffer->GetStatus(&nState))){
		return (false);
	}
	if(nState & DSBSTATUS_PLAYING){
		return (false);
	}
	if(nState & DSBSTATUS_BUFFERLOST){
		if(FAILED(_Static.pBuffer->Restore())){
			DeleteStatic(_Static);
			return (false);
		}
	}
	if(FAILED(_Static.pBuffer->Play(0, 0, 0))){
		return (false);
	}
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::AUDIO8::Stop(STATIC &)
//***********************************************************************************************************************************************************************
bool owl::engine::AUDIO8::Stop(STATIC &_Static){
	if(_Static.pBuffer == NULL){
		return (false);
	}
	if(FAILED(_Static.pBuffer->Stop())){
		return (false);
	}
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::AUDIO8::SetVolume(STATIC &, int)
//***********************************************************************************************************************************************************************
bool owl::engine::AUDIO8::SetVolume(STATIC &_Static, int _Volume){
	if(_Static.pBuffer == NULL){
		return (false);
	}
	int	iVolume		= std::max<int>(owl::engine::MIN_VOLUME, std::min<int>(_Volume, owl::engine::MAX_VOLUME));
	_Static.pBuffer->SetVolume(static_cast<long>(std::pow(static_cast<float>(iVolume), owl::engine::AUDIO_GAIN) * 10000) + DSBVOLUME_MIN);
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::AUDIO8::CreateStream(STREAM &, STREAMLOADER, const wchar_t *)
//***********************************************************************************************************************************************************************
bool owl::engine::AUDIO8::CreateStream(STREAM &_Static, STREAMLOADER _Loader, const wchar_t *_File){
	assert(_File != NULL);
	if(_Static.pBuffer != NULL){
		return (true);
	}
	if(_Loader == NULL){
		return (false);
	}
	return (_Loader(pAudio, &_Static, _File));
}

//***********************************************************************************************************************************************************************
//		owl::engine::AUDIO8::DeleteStream(STREAM &)
//***********************************************************************************************************************************************************************
bool owl::engine::AUDIO8::DeleteStream(STREAM &_Static){
	VOLUMEMIXERITERATOR	p, e;
	for(p = cMixer.begin(); p != cMixer.end(); p++){
		if((*p) == &(_Static)){
			cMixer.erase(p);
			break;
		}
	}
	if(_Static.pBuffer != NULL){
		_Static.pBuffer->Stop();
	}
	CloseHandle(_Static.hEvents);
	CloseHandle(_Static.hThread);	
	SafeAudioRelease(_Static.pBuffer);
	_Static.hEvents		= NULL;
	_Static.hThread		= NULL;
	_Static.iVolume		= 0;
	_Static.iPosition	= 0;
	_Static.iVelocity	= 0;
	_Static.uHeadSize	= 0;
	_Static.uWaveSize	= 0;
	_Static.uDataSize	= 0;
	_Static.uInfoSize	= 0;
	_Static.uNextSize	= 0;
	_Static.uFileLoop	= 0;
	_Static.uFileRead	= 0;
	SafeDeleteArray(_Static.pWaveData);
	memset(_Static.cFilePath, 0, sizeof(wchar_t) * MAX_PATH);
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::AUDIO8::Play(STREAM &)
//***********************************************************************************************************************************************************************
bool owl::engine::AUDIO8::Play(STREAM &_Stream, int _Fade){
	if(_Stream.pBuffer == NULL){
		return (false);
	}
	DWORD	nState	= 0;
	if(FAILED(_Stream.pBuffer->GetStatus(&nState))){
		return (false);
	}
	if(nState & DSBSTATUS_PLAYING){
		return (true);
	}
	if(nState & DSBSTATUS_BUFFERLOST){
		if(FAILED(_Stream.pBuffer->Restore())){
			DeleteStream(_Stream);
			return (false);
		}
	}
	if(_Fade == owl::engine::FADEi){
		//-----	<Filehead : SET>
		_Stream.uFileRead	= 0;
		for(int i = 0; i < 4; i++){
			SetEvent(_Stream.hEvents);
		}
		DWORD	nPos	= 0;
		_Stream.pBuffer->GetCurrentPosition(&nPos, NULL);
		for(int i = 0; i < 4; i++){
			if(i * _Stream.uInfoSize < nPos && nPos < (i+1) * _Stream.uInfoSize){
				nPos	= ((i + 2) * _Stream.uInfoSize) % (_Stream.uInfoSize * 4);
				break;
			}
		}
		_Stream.pBuffer->SetCurrentPosition(nPos);
		//-----	<Filehead : END>
		_Stream.iVelocity	= static_cast<int>(_Stream.iVolume / 60);
		_Stream.iPosition	= _Stream.iVelocity;
		cMixer.push_back(&_Stream);
	}
	if(_Fade == owl::engine::FADEs){
		//-----	<Filehead : SET>
		_Stream.uFileRead	= 0;
		for(int i = 0; i < 4; i++){
			SetEvent(_Stream.hEvents);
		}
		DWORD	nPos	= 0;
		_Stream.pBuffer->GetCurrentPosition(&nPos, NULL);
		for(int i = 0; i < 4; i++){
			if(i * _Stream.uInfoSize < nPos && nPos < (i+1) * _Stream.uInfoSize){
				nPos	= ((i + 2) * _Stream.uInfoSize) % (_Stream.uInfoSize * 4);
				break;
			}
		}
		_Stream.pBuffer->SetCurrentPosition(nPos);
		//-----	<Filehead : END>
	}
	if(FAILED(_Stream.pBuffer->SetVolume(_Stream.iPosition + DSBVOLUME_MIN))){
		return (false);
	}
	if(FAILED(_Stream.pBuffer->Play(0, 0, DSBPLAY_LOOPING))){
		return (false);
	}
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::AUDIO8::Stop(STREAM &)
//***********************************************************************************************************************************************************************
bool owl::engine::AUDIO8::Stop(STREAM &_Stream, int _Fade){
	if(_Stream.pBuffer == NULL){
		return (false);
	}
	if(_Fade == owl::engine::FADEo){
		_Stream.iVelocity	= static_cast<int>(-_Stream.iVolume / 60);
		_Stream.iPosition	= _Stream.iPosition;
		cMixer.push_back(&_Stream);
	}else{
		DWORD	nState;
		if(FAILED(_Stream.pBuffer->Stop())){
			return (false);
		}
		do{
			_Stream.pBuffer->GetStatus(&nState);
		}while((nState & DSBSTATUS_PLAYING) != 0);
	}
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::AUDIO8::SetVolume(STREAM &, int)
//***********************************************************************************************************************************************************************
bool owl::engine::AUDIO8::SetVolume(STREAM &_Stream, int _Volume){
	if(_Stream.pBuffer == NULL){
		return (false);
	}
	_Stream.iVolume		= std::max<int>(owl::engine::MIN_VOLUME, std::min<int>(_Volume, owl::engine::MAX_VOLUME));
	_Stream.iPosition	= _Stream.iVolume;
	_Stream.iVelocity	= 0;
	_Stream.pBuffer->SetVolume(_Stream.iPosition + DSBVOLUME_MIN);
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::AUDIO8::SetLooped(STREAM &, unsigned int)
//***********************************************************************************************************************************************************************
bool owl::engine::AUDIO8::SetLooped(STREAM &_Stream, unsigned int _Looped){
	if(_Stream.pBuffer == NULL){
		return (false);
	}
	_Stream.uFileLoop	= std::max<unsigned int>(0, std::min<unsigned int>(_Looped, _Stream.uWaveSize));
	return (true);
}