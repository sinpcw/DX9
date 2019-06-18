#include"owlEngine9.hpp"

//**********************************************************************************************************************************************************************
//		Static
//**********************************************************************************************************************************************************************
static	unsigned int __stdcall	CallbackWAV(void *);
static	unsigned int __stdcall	CallbackOGG(void *);

//**********************************************************************************************************************************************************************
//
//		Load WAV Component
//
//**********************************************************************************************************************************************************************
//		LoadStreamWAV(owl::engine::STREAM *) for CallbackWAV(void *)
//***********************************************************************************************************************************************************************
bool LoadStreamWAV(owl::engine::STREAM *_Data){
	FILE *		fp;
	void *		pVoid	= NULL;
	UINT		uSize	= 0;
	if(_Data->uFileRead + _Data->uInfoSize >= _Data->uWaveSize){
		UINT	sz;
		if(0 != _wfopen_s(&fp, _Data->cFilePath, L"rb")){
			return false;
		}
		fseek(fp, _Data->uHeadSize + _Data->uFileRead, SEEK_SET);
		uSize	= _Data->uWaveSize - _Data->uFileRead;
		sz		= _Data->uFileLoop + (_Data->uInfoSize - uSize);
		fread(&_Data->pWaveData[0]    , sizeof(BYTE),                    uSize, fp);
		fseek(fp, _Data->uHeadSize + _Data->uFileLoop, SEEK_SET);
		fread(&_Data->pWaveData[uSize], sizeof(BYTE), _Data->uInfoSize - uSize, fp);
		fclose(fp);
		HRESULT	hRet	= _Data->pBuffer->Lock(_Data->uNextSize, _Data->uInfoSize, &pVoid, reinterpret_cast<DWORD *>(&uSize), NULL, NULL, 0);
		if(hRet == DSERR_BUFFERLOST){
			if(FAILED(_Data->pBuffer->Restore())){
				return false;
			}
			if(FAILED(_Data->pBuffer->Lock(_Data->uNextSize, _Data->uInfoSize, &pVoid, reinterpret_cast<DWORD *>(&uSize), NULL, NULL, 0))){
				return false;
			}
		}else if(FAILED(hRet)){
			return false;
		}
		memcpy(pVoid, _Data->pWaveData, sizeof(BYTE) * uSize);
		if(FAILED(_Data->pBuffer->Unlock(pVoid, uSize, NULL, NULL))){
			return false;
		}
		_Data->uNextSize	= (_Data->uNextSize + _Data->uInfoSize) % (_Data->uInfoSize * 4);
		_Data->uFileRead	= sz;
	}else{
		if(0 != _wfopen_s(&fp, _Data->cFilePath, L"rb")){
			return false;
		}
		fseek(fp, _Data->uHeadSize + _Data->uFileRead, SEEK_SET);
		fread(_Data->pWaveData, sizeof(BYTE), _Data->uInfoSize, fp);
		fclose(fp);
		HRESULT	hRet	= _Data->pBuffer->Lock(_Data->uNextSize, _Data->uInfoSize, &pVoid, reinterpret_cast<DWORD *>(&uSize), NULL, NULL, 0);
		if(hRet	== DSERR_BUFFERLOST){
			if(FAILED(_Data->pBuffer->Restore())){
				return false;
			}
			if(FAILED(_Data->pBuffer->Lock(_Data->uNextSize, _Data->uInfoSize, &pVoid, reinterpret_cast<DWORD *>(&uSize), NULL, NULL, 0))){
				return false;
			}
		}else if(FAILED(hRet)){
			return false;
		}
		memcpy(pVoid, _Data->pWaveData, sizeof(BYTE) * uSize);
		if(FAILED(_Data->pBuffer->Unlock(pVoid, uSize, NULL, NULL))){
			return false;
		}
		_Data->uNextSize	+= uSize;
		_Data->uNextSize	%= _Data->uDataSize;
		_Data->uFileRead	+= uSize;
	}
	return true;
}

//***********************************************************************************************************************************************************************
//		CallbackWAV(void *)
//***********************************************************************************************************************************************************************
unsigned int __stdcall CallbackWAV(void *pVoid){
	owl::engine::STREAM *	pThis	= static_cast<owl::engine::STREAM *>(pVoid);
	while(1){
		switch(MsgWaitForMultipleObjects(1, &pThis->hEvents, FALSE, INFINITE, QS_ALLEVENTS)){
			case (WAIT_OBJECT_0 + 0):
				LoadStreamWAV(pThis);
				break;
			case (WAIT_OBJECT_0 + 1):
				return 0;
		}
	}
	return 0;
}

//**********************************************************************************************************************************************************************
//
//		Load Ogg/Vorbis Component
//
//***********************************************************************************************************************************************************************
//		LoadStreamOGG(owl::engine::STREAM *) for CallbackOGG(void *)
//***********************************************************************************************************************************************************************
bool LoadStreamOGG(owl::engine::STREAM *_Data){
	FILE *			fp;
	INT				iBit	= 0;
	LONG			lByte	= 0;
	LONG			lTell	= 0;
	BYTE			cBuf[4096];
	HRESULT			hRet;
	OggVorbis_File	cOggVorbis;
	vorbis_info *	pVorbisInfo;
	void *			pVoid	= NULL;
	UINT			uSize	= 0;
	if(_Data->uFileRead + _Data->uInfoSize >= _Data->uWaveSize){
		_wfopen_s(&fp, _Data->cFilePath, L"rb");
		if(NULL == fp){
			return false;
		}
		if(ov_open(fp, &cOggVorbis, NULL, 0) < 0){
			fclose(fp);
			return false;
		}
		if(NULL == (pVorbisInfo = ov_info(&cOggVorbis, -1))){
			ov_clear(&cOggVorbis);
			fclose(fp);
			return false;
		}
		uSize	= _Data->uWaveSize - _Data->uFileRead;
		ov_pcm_seek(&cOggVorbis, _Data->uFileRead / (pVorbisInfo->channels * 2));
		while(lByte < static_cast<LONG>(uSize) && 0 < (lTell = ov_read(&cOggVorbis, reinterpret_cast<char *>(cBuf), (_Data->uInfoSize - lByte >= 4096) ? (4096) : (_Data->uInfoSize - lByte), 0, 2, 1, &iBit))){
			memcpy(&_Data->pWaveData[lByte], cBuf, sizeof(BYTE) * lTell);
			lByte	+= lTell;
		}
		ov_pcm_seek(&cOggVorbis, _Data->uFileLoop / (pVorbisInfo->channels * 2));
		while(lByte < static_cast<LONG>(_Data->uInfoSize) && 0 < (lTell = ov_read(&cOggVorbis, reinterpret_cast<char *>(cBuf), (_Data->uInfoSize - lByte >= 4096) ? (4096) : (_Data->uInfoSize - lByte), 0, 2, 1, &iBit))){
			memcpy(&_Data->pWaveData[lByte], cBuf, sizeof(BYTE) * lTell);
			lByte	+= lTell;
		}
		HRESULT	hRet	= _Data->pBuffer->Lock(_Data->uNextSize, _Data->uInfoSize, &pVoid, reinterpret_cast<DWORD *>(&uSize), NULL, NULL, 0);
		if(hRet == DSERR_BUFFERLOST){
			if(FAILED(_Data->pBuffer->Restore())){
				return false;
			}
			if(FAILED(_Data->pBuffer->Lock(_Data->uNextSize, _Data->uInfoSize, &pVoid, reinterpret_cast<DWORD *>(&uSize), NULL, NULL, 0))){
				return false;
			}
		}else if(FAILED(hRet)){
			return false;
		}
		memcpy(pVoid, _Data->pWaveData, sizeof(BYTE) * uSize);
		if(FAILED(_Data->pBuffer->Unlock(pVoid, uSize, NULL, NULL))){
			return false;
		}
		_Data->uNextSize	= (_Data->uNextSize + _Data->uInfoSize) % (_Data->uInfoSize * 4);
		_Data->uFileRead	= _Data->uFileLoop + ( _Data->uInfoSize - uSize);
		return true;
	}else{
		_wfopen_s(&fp, _Data->cFilePath, L"rb");
		if(NULL == fp){
			return false;
		}
		if(ov_open(fp, &cOggVorbis, NULL, 0) < 0){
			fclose(fp);
			return false;
		}
		if(NULL == (pVorbisInfo = ov_info(&cOggVorbis, -1))){
			ov_clear(&cOggVorbis);
			fclose(fp);
			return false;
		}
		ov_pcm_seek(&cOggVorbis, _Data->uFileRead / (pVorbisInfo->channels * 2));
		while(lByte < static_cast<LONG>(_Data->uInfoSize) && 0 < (lTell = ov_read(&cOggVorbis, reinterpret_cast<char *>(cBuf), (_Data->uInfoSize - lByte >= 4096) ? (4096) : (_Data->uInfoSize - lByte), 0, 2, 1, &iBit))){
			memcpy(&_Data->pWaveData[lByte], cBuf, sizeof(BYTE) * lTell);
			lByte	+= lTell;
		}
		ov_clear(&cOggVorbis);
		fclose(fp);
		hRet	= _Data->pBuffer->Lock(_Data->uNextSize, _Data->uInfoSize, &pVoid, reinterpret_cast<DWORD *>(&uSize), NULL, NULL, 0);
		if(hRet	== DSERR_BUFFERLOST){
			if(FAILED(_Data->pBuffer->Restore())){
				return false;
			}
			if(FAILED(_Data->pBuffer->Lock(_Data->uNextSize, _Data->uInfoSize, &pVoid, reinterpret_cast<DWORD *>(&uSize), NULL, NULL, 0))){
				return false;
			}
		}else if(FAILED(hRet)){
			return false;
		}
		memcpy(pVoid, _Data->pWaveData, sizeof(BYTE) * uSize);
		if(FAILED(_Data->pBuffer->Unlock(pVoid, uSize, NULL, NULL))){
			return false;
		}
		_Data->uNextSize	+= uSize;
		_Data->uNextSize	%= _Data->uDataSize;
		_Data->uFileRead	+= uSize;
	}
	return true;
}

//***********************************************************************************************************************************************************************
//		CallbackOGG(void *)
//***********************************************************************************************************************************************************************
unsigned int __stdcall CallbackOGG(void *pVoid){
	owl::engine::STREAM *	pThis	= static_cast<owl::engine::STREAM *>(pVoid);
	while(1){
		switch(MsgWaitForMultipleObjects(1, &pThis->hEvents, FALSE, INFINITE, QS_ALLEVENTS)){
			case (WAIT_OBJECT_0 + 0):
				LoadStreamOGG(pThis);
				break;
			case (WAIT_OBJECT_0 + 1):
				return 0;
		}
	}
	return 0;
}

//**********************************************************************************************************************************************************************
//
//		Loader Component (Static)
//
//***********************************************************************************************************************************************************************-
//		owl::engine::loader::StaticWAV(IDirectSound8 *, STATIC *, const wchar_t *)
//***********************************************************************************************************************************************************************-
bool owl::engine::loader::StaticWAV(IDirectSound8 *_Info, STATIC *_Data, const wchar_t *_File){
	FILE *			fp;
	UINT			uCounts		= 0;
	UINT			uLength		= 0;
	BYTE *			pMemory		= NULL;
	BYTE *			pLocked		= NULL;
	BYTE			cBuffer[8]	= { 0 };
	WAVEFORMATEX	cWaveFormat	= { 0 };
	DSBUFFERDESC	cDescBuffer	= { 0 };
	
	_wfopen_s(&fp, _File, L"rb");
	if(NULL == fp){
		return (false);
	}
	//-----	<RIFF / WAVE を確認>
	fread(cBuffer, sizeof(BYTE), 8, fp);
	if(cBuffer[0] != 'R' || cBuffer[1] != 'I' || cBuffer[2] != 'F' || cBuffer[3] != 'F'){
		fclose(fp);
		return (false);
	}
	fread(cBuffer, sizeof(BYTE), 4, fp);
	if(cBuffer[0] != 'W' || cBuffer[1] != 'A' || cBuffer[2] != 'V' || cBuffer[3] != 'E'){
		fclose(fp);
		return (false);
	}
	//-----	<タグ取得>
	for(int i = 0; i < 4; i++){
		if(8 != fread(cBuffer, sizeof(BYTE), 8, fp)){
			break;
		}
		uCounts	= (static_cast<UINT>(cBuffer[4]) << 0) | (static_cast<UINT>(cBuffer[5]) << 8) | (static_cast<UINT>(cBuffer[6]) << 16) | (static_cast<UINT>(cBuffer[7]) << 24);
		try{
			pMemory	= new BYTE[uCounts];
		}catch(std::bad_alloc){
			fclose(fp);
			return (false);
		}
		if(cBuffer[0] == 'L' && cBuffer[1] == 'I' && cBuffer[2] == 'S' && cBuffer[3] == 'T'){
			//-----	コメント・著作権情報
			fread(pMemory, sizeof(BYTE), uCounts, fp);
		}
		if(cBuffer[0] == 'f' && cBuffer[1] == 'a' && cBuffer[2] == 'c' && cBuffer[3] == 't'){
			//-----	全サンプル数
			fread(pMemory, sizeof(BYTE), uCounts, fp);	
		}
		if(cBuffer[0] == 'f' && cBuffer[1] == 'm' && cBuffer[2] == 't' && cBuffer[3] == ' '){
			fread(pMemory, sizeof(BYTE), uCounts, fp);
			cWaveFormat.wFormatTag		= (static_cast<UINT>(pMemory[ 1]) <<  8) | (static_cast<UINT>(pMemory[ 0]) <<  0);
			cWaveFormat.nChannels		= (static_cast<UINT>(pMemory[ 3]) <<  8) | (static_cast<UINT>(pMemory[ 2]) <<  0);
			cWaveFormat.nSamplesPerSec	= (static_cast<UINT>(pMemory[ 7]) << 24) | (static_cast<UINT>(pMemory[ 6]) << 16) | (static_cast<UINT>(pMemory[ 5]) <<  8) | (static_cast<UINT>(pMemory[ 4]) <<  0);
			cWaveFormat.wBitsPerSample	= (static_cast<UINT>(pMemory[15]) <<  8) | (static_cast<UINT>(pMemory[14]) <<  0);
			cWaveFormat.nBlockAlign		= cWaveFormat.wBitsPerSample * cWaveFormat.nChannels / 8;
			cWaveFormat.nAvgBytesPerSec	= cWaveFormat.nSamplesPerSec * cWaveFormat.nBlockAlign;
			cWaveFormat.cbSize			= 0;
			if(cWaveFormat.wFormatTag != WAVE_FORMAT_PCM){
				SafeDeleteArray(pMemory);
				fclose(fp);
				return (false);
			}
		}
		if(cBuffer[0] == 'd' && cBuffer[1] == 'a' && cBuffer[2] == 't' || cBuffer[3] == 'a'){
			fread(pMemory, sizeof(BYTE), uCounts, fp);
			cDescBuffer.dwSize				= sizeof(DSBUFFERDESC);
			cDescBuffer.dwFlags				= DSBCAPS_STATIC | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_GLOBALFOCUS;
			cDescBuffer.dwBufferBytes		= static_cast<DWORD>(uCounts);
			cDescBuffer.guid3DAlgorithm		= DS3DALG_DEFAULT;
			cDescBuffer.lpwfxFormat			= &cWaveFormat;
			if(FAILED(_Info->CreateSoundBuffer(&cDescBuffer, reinterpret_cast<IDirectSoundBuffer **>(&_Data->pBuffer), NULL))){
				SafeDeleteArray(pMemory);
				fclose(fp);
				return (false);
			}
			if(FAILED(_Data->pBuffer->Lock(0, uCounts, reinterpret_cast<void **>(&pLocked), reinterpret_cast<DWORD *>(&uLength), NULL, NULL, 0))){
				SafeDeleteArray(pMemory);
				SafeRelease(_Data->pBuffer);
				fclose(fp);
				return (false);
			}
			memcpy(pLocked, pMemory, sizeof(BYTE) * uLength);
			if(FAILED(_Data->pBuffer->Unlock(reinterpret_cast<void *>(pLocked), static_cast<DWORD>(uLength), NULL, NULL))){
				SafeDeleteArray(pMemory);
				SafeRelease(_Data->pBuffer);
				fclose(fp);
				return (false);
			}
		}	
		SafeDeleteArray(pMemory);
	}
	fclose(fp);
	return (true);
}

//***********************************************************************************************************************************************************************-
//		owl::engine::loader::StaticOGG(IDirectSound8 *, STATIC *, const wchar_t *)
//***********************************************************************************************************************************************************************-
bool owl::engine::loader::StaticOGG(IDirectSound8 *_Info, STATIC *_Data, const wchar_t *_File){
	FILE *			fp;
	INT				iBit;
	LONG			lTell		= 0;
	LONG			lByte		= 0;
	UINT			uLength		= 0;
	BYTE			cBuf[4096];
	BYTE *			pLocked		= NULL;
	WAVEFORMATEX	cWaveFormat	= { 0 };
	DSBUFFERDESC	cDescBuffer	= { 0 };

	OggVorbis_File	cOggVorbis;
	vorbis_info *	pVorbisInfo;

	_wfopen_s(&fp, _File, L"rb");
	if(NULL == fp){
		return (false);
	}
	if(ov_open(fp, &cOggVorbis, NULL, 0) < 0){
		fclose(fp);
		return (false);
	}
	if(NULL == (pVorbisInfo = ov_info(&cOggVorbis, -1))){
		ov_clear(&cOggVorbis);
		fclose(fp);
		return (false);
	}
	cWaveFormat.wFormatTag			= WAVE_FORMAT_PCM;
	cWaveFormat.nChannels			= pVorbisInfo->channels;
	cWaveFormat.nSamplesPerSec		= 44100;
	cWaveFormat.wBitsPerSample		= 16;
	cWaveFormat.nBlockAlign			= pVorbisInfo->channels * 16 / 8;
	cWaveFormat.nAvgBytesPerSec		= cWaveFormat.nSamplesPerSec * cWaveFormat.nBlockAlign;
	cWaveFormat.cbSize				= 0;
	cDescBuffer.dwSize				= sizeof(DSBUFFERDESC);
	cDescBuffer.dwFlags				= DSBCAPS_STATIC | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_GLOBALFOCUS;
	cDescBuffer.dwBufferBytes		= static_cast<DWORD>(pVorbisInfo->channels * 2 * ov_pcm_total(&cOggVorbis, -1));
	cDescBuffer.guid3DAlgorithm		= DS3DALG_DEFAULT;
	cDescBuffer.lpwfxFormat			= &cWaveFormat;
	if(FAILED(_Info->CreateSoundBuffer(&cDescBuffer, reinterpret_cast<IDirectSoundBuffer **>(&_Data->pBuffer), NULL))){
		ov_clear(&cOggVorbis);
		fclose(fp);
		return (false);
	}
	while(0 < (lTell = ov_read(&cOggVorbis, reinterpret_cast<char *>(cBuf), 4096, 0, 2, 1, &iBit))){
		if(FAILED(_Data->pBuffer->Lock(lByte, 4096, reinterpret_cast<void **>(&pLocked), reinterpret_cast<DWORD *>(&uLength), NULL, NULL, 0))){
			SafeAudioRelease(_Data->pBuffer);
			ov_clear(&cOggVorbis);
			fclose(fp);
			return (false);
		}
		memcpy(pLocked, cBuf, sizeof(BYTE) * uLength);
		if(FAILED(_Data->pBuffer->Unlock(reinterpret_cast<void *>(pLocked), static_cast<DWORD>(uLength), NULL, NULL))){
			SafeAudioRelease(_Data->pBuffer);
			ov_clear(&cOggVorbis);
			fclose(fp);
			return (false);
		}
		lByte	+= lTell;
	}
	ov_clear(&cOggVorbis);
	fclose(fp);
	return (true);
}

//**********************************************************************************************************************************************************************
//
//		Loader Component (Stream)
//
//***********************************************************************************************************************************************************************-
//		owl::engine::loader::StreamWAV(IDirectSound8 *, STREAM *, const wchar_t *)
//***********************************************************************************************************************************************************************-
bool owl::engine::loader::StreamWAV(IDirectSound8 *_Info, STREAM *_Data, const wchar_t *_File){
	FILE *					fp;
	UINT					uNotify		= 0;
	BYTE *					pMemory		= NULL;
	BYTE *					pLocked		= NULL;
	BYTE					cBuffer[8]	= { 0x00 };
	UINT					uCounts		= 0;
	WAVEFORMATEX			cWaveFormat	= { 0 };
	DSBUFFERDESC			cDescBuffer	= { 0 };
	IDirectSoundNotify *	pNotify		= NULL;
	DSBPOSITIONNOTIFY		vPosition[4];

	_wfopen_s(&fp, _File, L"rb");
	if(NULL == fp){
		return (false);
	}
	//-----	<RIFF / WAVE を確認>
	fread(cBuffer, sizeof(BYTE), 8, fp);
	if(cBuffer[0] != 'R' || cBuffer[1] != 'I' || cBuffer[2] != 'F' || cBuffer[3] != 'F'){
		fclose(fp);
		return (false);
	}
	fread(cBuffer, sizeof(BYTE), 4, fp);
	if(cBuffer[0] != 'W' || cBuffer[1] != 'A' || cBuffer[2] != 'V' || cBuffer[3] != 'E'){
		fclose(fp);
		return (false);
	}
	//-----	<タグ取得>
	for(int i = 0; i < 4; i++){
		if(8 != fread(cBuffer, sizeof(BYTE), 8, fp)){
			break;
		}
		uCounts	= (static_cast<UINT>(cBuffer[4]) << 0) | (static_cast<UINT>(cBuffer[5]) << 8) | (static_cast<UINT>(cBuffer[6]) << 16) | (static_cast<UINT>(cBuffer[7]) << 24);
		if(cBuffer[0] == 'L' && cBuffer[1] == 'I' && cBuffer[2] == 'S' && cBuffer[3] == 'T'){
			//-----	コメント・著作権情報
			try{
				pMemory	= new BYTE[uCounts];
			}catch(std::bad_alloc){
				SafeAudioRelease(_Data->pBuffer);
				CloseHandle(_Data->hEvents);
				CloseHandle(_Data->hThread);
				_Data->hEvents		= NULL;
				_Data->hThread		= NULL;
				_Data->uHeadSize	= 0;
				_Data->uWaveSize	= 0;
				_Data->uDataSize	= 0;
				_Data->uInfoSize	= 0;
				_Data->uNextSize	= 0;
				_Data->uFileLoop	= 0;
				_Data->uFileRead	= 0;
				memset(_Data->cFilePath, 0, sizeof(wchar_t) * MAX_PATH);
				fclose(fp);
				return (false);
			}
			fread(pMemory, sizeof(BYTE), uCounts, fp);
			SafeDeleteArray(pMemory);
		}
		if(cBuffer[0] == 'f' && cBuffer[1] == 'a' && cBuffer[2] == 'c' && cBuffer[3] == 't'){
			//-----	全サンプル数
			try{
				pMemory	= new BYTE[uCounts];
			}catch(std::bad_alloc){
				SafeAudioRelease(_Data->pBuffer);
				CloseHandle(_Data->hEvents);
				CloseHandle(_Data->hThread);
				_Data->hEvents		= NULL;
				_Data->hThread		= NULL;
				_Data->uHeadSize	= 0;
				_Data->uWaveSize	= 0;
				_Data->uDataSize	= 0;
				_Data->uInfoSize	= 0;
				_Data->uNextSize	= 0;
				_Data->uFileLoop	= 0;
				_Data->uFileRead	= 0;
				memset(_Data->cFilePath, 0, sizeof(wchar_t) * MAX_PATH);
				fclose(fp);
				return (false);
			}
			fread(pMemory, sizeof(BYTE), uCounts, fp);
			SafeDeleteArray(pMemory);
		}
		if(cBuffer[0] == 'f' && cBuffer[1] == 'm' && cBuffer[2] == 't' && cBuffer[3] == ' '){
			//-----	フォーマット
			try{
				pMemory	= new BYTE[uCounts];
			}catch(std::bad_alloc){
				SafeAudioRelease(_Data->pBuffer);
				CloseHandle(_Data->hEvents);
				CloseHandle(_Data->hThread);
				_Data->hEvents		= NULL;
				_Data->hThread		= NULL;
				_Data->uHeadSize	= 0;
				_Data->uWaveSize	= 0;
				_Data->uDataSize	= 0;
				_Data->uInfoSize	= 0;
				_Data->uNextSize	= 0;
				_Data->uFileLoop	= 0;
				_Data->uFileRead	= 0;
				memset(_Data->cFilePath, 0, sizeof(wchar_t) * MAX_PATH);
				fclose(fp);
				return (false);
			}
			fread(pMemory, sizeof(BYTE), uCounts, fp);
			cWaveFormat.wFormatTag		= (static_cast<UINT>(pMemory[ 1]) <<  8) | (static_cast<UINT>(pMemory[ 0]) <<  0);
			cWaveFormat.nChannels		= (static_cast<UINT>(pMemory[ 3]) <<  8) | (static_cast<UINT>(pMemory[ 2]) <<  0);
			cWaveFormat.nSamplesPerSec	= (static_cast<UINT>(pMemory[ 7]) << 24) | (static_cast<UINT>(pMemory[ 6]) << 16) | (static_cast<UINT>(pMemory[ 5]) <<  8) | (static_cast<UINT>(pMemory[ 4]) <<  0);
			cWaveFormat.wBitsPerSample	= (static_cast<UINT>(pMemory[15]) <<  8) | (static_cast<UINT>(pMemory[14]) <<  0);
			cWaveFormat.nBlockAlign		= cWaveFormat.wBitsPerSample * cWaveFormat.nChannels / 8;
			cWaveFormat.nAvgBytesPerSec	= cWaveFormat.nSamplesPerSec * cWaveFormat.nBlockAlign;
			cWaveFormat.cbSize			= 0;
			SafeDeleteArray(pMemory);
			if(cWaveFormat.wFormatTag != WAVE_FORMAT_PCM){
				SafeAudioRelease(_Data->pBuffer);
				CloseHandle(_Data->hEvents);
				CloseHandle(_Data->hThread);
				_Data->hEvents		= NULL;
				_Data->hThread		= NULL;
				_Data->uHeadSize	= 0;
				_Data->uWaveSize	= 0;
				_Data->uDataSize	= 0;
				_Data->uInfoSize	= 0;
				_Data->uNextSize	= 0;
				_Data->uFileLoop	= 0;
				_Data->uFileRead	= 0;
				memset(_Data->cFilePath, 0, sizeof(wchar_t) * MAX_PATH);
				fclose(fp);
				return (false);
			}
		}
		if(cBuffer[0] == 'd' && cBuffer[1] == 'a' && cBuffer[2] == 't' || cBuffer[3] == 'a'){
			//-----	データ
			uNotify							 = cWaveFormat.nSamplesPerSec * cWaveFormat.nBlockAlign / 2;
			uNotify							-= uNotify % cWaveFormat.nBlockAlign;
			cDescBuffer.dwSize				 = sizeof(DSBUFFERDESC);
			cDescBuffer.dwFlags				 = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_GLOBALFOCUS;
			cDescBuffer.dwBufferBytes		 = uNotify * 4;
			cDescBuffer.guid3DAlgorithm		 = DS3DALG_DEFAULT;
			cDescBuffer.lpwfxFormat			 = &cWaveFormat;
			_Data->uInfoSize				 = uNotify;
			_Data->hEvents					 = CreateEvent(NULL, FALSE, FALSE, NULL);
			_Data->hThread					 = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, CallbackWAV, _Data, 0, 0));
			if(FAILED(_Info->CreateSoundBuffer(&cDescBuffer, reinterpret_cast<IDirectSoundBuffer **>(&_Data->pBuffer), NULL))){
				SafeAudioRelease(_Data->pBuffer);
				CloseHandle(_Data->hEvents);
				CloseHandle(_Data->hThread);
				_Data->hEvents		= NULL;
				_Data->hThread		= NULL;
				_Data->uHeadSize	= 0;
				_Data->uWaveSize	= 0;
				_Data->uDataSize	= 0;
				_Data->uInfoSize	= 0;
				_Data->uNextSize	= 0;
				_Data->uFileLoop	= 0;
				_Data->uFileRead	= 0;
				memset(_Data->cFilePath, 0, sizeof(wchar_t) * MAX_PATH);
				fclose(fp);
				return (false);
			}
			_Data->uHeadSize	= ftell(fp);
			_Data->uWaveSize	= uCounts;
			try{
				pMemory	= new BYTE[cDescBuffer.dwBufferBytes];
			}catch(std::bad_alloc){
				SafeAudioRelease(_Data->pBuffer);
				CloseHandle(_Data->hEvents);
				CloseHandle(_Data->hThread);
				_Data->hEvents		= NULL;
				_Data->hThread		= NULL;
				_Data->uHeadSize	= 0;
				_Data->uWaveSize	= 0;
				_Data->uDataSize	= 0;
				_Data->uInfoSize	= 0;
				_Data->uNextSize	= 0;
				_Data->uFileLoop	= 0;
				_Data->uFileRead	= 0;
				memset(_Data->cFilePath, 0, sizeof(wchar_t) * MAX_PATH);
				fclose(fp);
				return (false);
			}
			fread(pMemory, sizeof(BYTE), cDescBuffer.dwBufferBytes, fp);
			if(FAILED(_Data->pBuffer->Lock(0, cDescBuffer.dwBufferBytes, reinterpret_cast<void **>(&pLocked), reinterpret_cast<DWORD *>(&cDescBuffer.dwBufferBytes), NULL, NULL, 0))){
				SafeDeleteArray(pMemory);
				SafeAudioRelease(_Data->pBuffer);
				CloseHandle(_Data->hEvents);
				CloseHandle(_Data->hThread);
				_Data->hEvents		= NULL;
				_Data->hThread		= NULL;
				_Data->uHeadSize	= 0;
				_Data->uWaveSize	= 0;
				_Data->uDataSize	= 0;
				_Data->uInfoSize	= 0;
				_Data->uNextSize	= 0;
				_Data->uFileLoop	= 0;
				_Data->uFileRead	= 0;
				memset(_Data->cFilePath, 0, sizeof(wchar_t) * MAX_PATH);
				fclose(fp);
				return (false);
			}
			_Data->uDataSize	= cDescBuffer.dwBufferBytes;
			memcpy(pLocked, pMemory, sizeof(BYTE) * cDescBuffer.dwBufferBytes);
			if(FAILED(_Data->pBuffer->Unlock(reinterpret_cast<void *>(pLocked), static_cast<DWORD>(cDescBuffer.dwBufferBytes), NULL, NULL))){
				SafeDeleteArray(pMemory);
				SafeAudioRelease(_Data->pBuffer);
				CloseHandle(_Data->hEvents);
				CloseHandle(_Data->hThread);
				_Data->hEvents		= NULL;
				_Data->hThread		= NULL;
				_Data->uHeadSize	= 0;
				_Data->uWaveSize	= 0;
				_Data->uDataSize	= 0;
				_Data->uInfoSize	= 0;
				_Data->uNextSize	= 0;
				_Data->uFileLoop	= 0;
				_Data->uFileRead	= 0;
				memset(_Data->cFilePath, 0, sizeof(wchar_t) * MAX_PATH);
				fclose(fp);
				return (false);
			}
			SafeDeleteArray(pMemory);
			fclose(fp);
			_Data->uFileRead	= cDescBuffer.dwBufferBytes;
			if(FAILED(_Data->pBuffer->QueryInterface(IID_IDirectSoundNotify, reinterpret_cast<void **>(&pNotify)))){
				SafeAudioRelease(_Data->pBuffer);
				CloseHandle(_Data->hEvents);
				CloseHandle(_Data->hThread);
				_Data->hEvents		= NULL;
				_Data->hThread		= NULL;
				_Data->uHeadSize	= 0;
				_Data->uWaveSize	= 0;
				_Data->uDataSize	= 0;
				_Data->uInfoSize	= 0;
				_Data->uNextSize	= 0;
				_Data->uFileLoop	= 0;
				_Data->uFileRead	= 0;
				memset(_Data->cFilePath, 0, sizeof(wchar_t) * MAX_PATH);
				return (false);
			}
			for(int i = 0; i < 4; i++){
				vPosition[i].dwOffset		= (_Data->uInfoSize * i) + (_Data->uInfoSize - 1);
				vPosition[i].hEventNotify	= (_Data->hEvents);
			}
			if(FAILED(pNotify->SetNotificationPositions(4, vPosition))){
				SafeAudioRelease(_Data->pBuffer);
				CloseHandle(_Data->hEvents);
				CloseHandle(_Data->hThread);
				_Data->hEvents		= NULL;
				_Data->hThread		= NULL;
				_Data->uHeadSize	= 0;
				_Data->uWaveSize	= 0;
				_Data->uDataSize	= 0;
				_Data->uInfoSize	= 0;
				_Data->uNextSize	= 0;
				_Data->uFileLoop	= 0;
				_Data->uFileRead	= 0;
				memset(_Data->cFilePath, 0, sizeof(wchar_t) * MAX_PATH);
				return (false);
			}
			try{
				_Data->pWaveData	= new BYTE[_Data->uInfoSize];
			}catch(std::bad_alloc){
				SafeAudioRelease(_Data->pBuffer);
				CloseHandle(_Data->hEvents);
				CloseHandle(_Data->hThread);
				_Data->hEvents		= NULL;
				_Data->hThread		= NULL;
				_Data->uHeadSize	= 0;
				_Data->uWaveSize	= 0;
				_Data->uDataSize	= 0;
				_Data->uInfoSize	= 0;
				_Data->uNextSize	= 0;
				_Data->uFileLoop	= 0;
				_Data->uFileRead	= 0;
				memset(_Data->cFilePath, 0, sizeof(wchar_t) * MAX_PATH);
				return (false);
			}
		}
	}
	wcscpy_s(_Data->cFilePath, MAX_PATH - 1, _File);
	_Data->cFilePath[MAX_PATH - 1]	= '\0';
	_Data->iVolume		= owl::engine::MAX_VOLUME;
	_Data->iPosition	= owl::engine::MAX_VOLUME;
	_Data->iVelocity	= 0;
	return (true);
}

//***********************************************************************************************************************************************************************-
//		LOADSTREAMOGG(IDirectSound8 *, STREAM *, const wchar_t *)
//***********************************************************************************************************************************************************************-
bool owl::engine::loader::StreamOGG(IDirectSound8 *_Info, STREAM *_Data, const wchar_t *_File){
	FILE *					fp;
	INT						iBit;
	LONG					lTell		= 0;
	LONG					lByte		= 0;
	UINT					uLength		= 0;
	BYTE *					pLocked		= NULL;
	BYTE					cBuf[4096]	= { 0 };
	WAVEFORMATEX			cWaveFormat	= { 0 };
	DSBUFFERDESC			cDescBuffer	= { 0 };
	IDirectSoundNotify *	pNotify		= NULL;
	DSBPOSITIONNOTIFY		vPosition[4];

	OggVorbis_File			cOggVorbis;
	vorbis_info *			pVorbisInfo;

	_wfopen_s(&fp, _File, L"rb");
	if(NULL == fp){
		return (false);
	}
	if(ov_open(fp, &cOggVorbis, NULL, 0) < 0){
		fclose(fp);
		return (false);
	}
	if(NULL == (pVorbisInfo = ov_info(&cOggVorbis, -1))){
		ov_clear(&cOggVorbis);
		fclose(fp);
		return (false);
	}
	cWaveFormat.wFormatTag			 = WAVE_FORMAT_PCM;
	cWaveFormat.nChannels			 = pVorbisInfo->channels;
	cWaveFormat.nSamplesPerSec		 = 44100;
	cWaveFormat.wBitsPerSample		 = 16;
	cWaveFormat.nBlockAlign			 = pVorbisInfo->channels * 16 / 8;
	cWaveFormat.nAvgBytesPerSec		 = cWaveFormat.nSamplesPerSec * cWaveFormat.nBlockAlign;
	cWaveFormat.cbSize				 = 0;
	_Data->uInfoSize				 = cWaveFormat.nSamplesPerSec * cWaveFormat.nBlockAlign;
	_Data->uInfoSize				-= _Data->uInfoSize % cWaveFormat.nBlockAlign;
	cDescBuffer.dwSize				 = sizeof(DSBUFFERDESC);
	cDescBuffer.dwFlags				 = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_GLOBALFOCUS;
	cDescBuffer.dwBufferBytes		 = _Data->uInfoSize * 4;
	cDescBuffer.guid3DAlgorithm		 = DS3DALG_DEFAULT;
	cDescBuffer.lpwfxFormat			 = &cWaveFormat;
	_Data->hEvents					 = CreateEvent(NULL, FALSE, FALSE, NULL);
	_Data->hThread					 = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, CallbackOGG, _Data, 0, 0));
	if(FAILED(_Info->CreateSoundBuffer(&cDescBuffer, reinterpret_cast<IDirectSoundBuffer **>(&_Data->pBuffer), NULL))){
		SafeAudioRelease(_Data->pBuffer);
		CloseHandle(_Data->hEvents);
		CloseHandle(_Data->hThread);
		_Data->hEvents		= NULL;
		_Data->hThread		= NULL;
		_Data->uHeadSize	= 0;
		_Data->uWaveSize	= 0;
		_Data->uDataSize	= 0;
		_Data->uInfoSize	= 0;
		_Data->uNextSize	= 0;
		_Data->uFileLoop	= 0;
		_Data->uFileRead	= 0;
		memset(_Data->cFilePath, 0, sizeof(wchar_t) * MAX_PATH);
		ov_clear(&cOggVorbis);
		fclose(fp);
		return (false);
	}
	_Data->uWaveSize	= static_cast<UINT>(pVorbisInfo->channels * 2 * ov_pcm_total(&cOggVorbis, -1));
	while(lByte < static_cast<LONG>(cDescBuffer.dwBufferBytes) && 0 < (lTell = ov_read(&cOggVorbis, reinterpret_cast<char *>(cBuf), (cDescBuffer.dwBufferBytes - lByte >= 4096) ? (4096) : (cDescBuffer.dwBufferBytes - lByte), 0, 2, 1, &iBit))){
		if(FAILED(_Data->pBuffer->Lock(lByte, (cDescBuffer.dwBufferBytes - lByte >= 4096) ? (4096) : (cDescBuffer.dwBufferBytes - lByte), reinterpret_cast<void **>(&pLocked), reinterpret_cast<DWORD *>(&uLength), NULL, NULL, 0))){
			SafeAudioRelease(_Data->pBuffer);
			CloseHandle(_Data->hEvents);
			CloseHandle(_Data->hThread);
			_Data->hEvents		= NULL;
			_Data->hThread		= NULL;
			_Data->uHeadSize	= 0;
			_Data->uWaveSize	= 0;
			_Data->uDataSize	= 0;
			_Data->uInfoSize	= 0;
			_Data->uNextSize	= 0;
			_Data->uFileLoop	= 0;
			_Data->uFileRead	= 0;
			memset(_Data->cFilePath, 0, sizeof(wchar_t) * MAX_PATH);
			ov_clear(&cOggVorbis);
			fclose(fp);
			return (false);
		}
		memcpy(pLocked, cBuf, sizeof(BYTE) * uLength);
		if(FAILED(_Data->pBuffer->Unlock(reinterpret_cast<void *>(pLocked), static_cast<DWORD>(uLength), NULL, NULL))){
			SafeAudioRelease(_Data->pBuffer);
			CloseHandle(_Data->hEvents);
			CloseHandle(_Data->hThread);
			_Data->hEvents		= NULL;
			_Data->hThread		= NULL;
			_Data->uHeadSize	= 0;
			_Data->uWaveSize	= 0;
			_Data->uDataSize	= 0;
			_Data->uInfoSize	= 0;
			_Data->uNextSize	= 0;
			_Data->uFileLoop	= 0;
			_Data->uFileRead	= 0;
			memset(_Data->cFilePath, 0, sizeof(wchar_t) * MAX_PATH);
			ov_clear(&cOggVorbis);
			fclose(fp);
			return (false);
		}
		lByte	+= lTell;
	}
	ov_clear(&cOggVorbis);
	fclose(fp);
	_Data->uFileRead	= lByte;
	_Data->uDataSize	= lByte;
	if(FAILED(_Data->pBuffer->QueryInterface(IID_IDirectSoundNotify, reinterpret_cast<void **>(&pNotify)))){
		SafeAudioRelease(_Data->pBuffer);
		CloseHandle(_Data->hEvents);
		CloseHandle(_Data->hThread);
		_Data->hEvents		= NULL;
		_Data->hThread		= NULL;
		_Data->uHeadSize	= 0;
		_Data->uWaveSize	= 0;
		_Data->uDataSize	= 0;
		_Data->uInfoSize	= 0;
		_Data->uNextSize	= 0;
		_Data->uFileLoop	= 0;
		_Data->uFileRead	= 0;
		memset(_Data->cFilePath, 0, sizeof(wchar_t) * MAX_PATH);
		return (false);
	}
	for(int i = 0; i < 4; i++){
		vPosition[i].dwOffset		= (_Data->uInfoSize * i) + (_Data->uInfoSize - 1);
		vPosition[i].hEventNotify	= (_Data->hEvents);
	}
	if(FAILED(pNotify->SetNotificationPositions(4, vPosition))){
		SafeAudioRelease(_Data->pBuffer);
		CloseHandle(_Data->hEvents);
		CloseHandle(_Data->hThread);
		_Data->hEvents		= NULL;
		_Data->hThread		= NULL;
		_Data->uHeadSize	= 0;
		_Data->uWaveSize	= 0;
		_Data->uDataSize	= 0;
		_Data->uInfoSize	= 0;
		_Data->uNextSize	= 0;
		_Data->uFileLoop	= 0;
		_Data->uFileRead	= 0;
		memset(_Data->cFilePath, 0, sizeof(wchar_t) * MAX_PATH);
		return (false);
	}
	try{
		_Data->pWaveData	= new BYTE[_Data->uInfoSize];
	}catch(std::bad_alloc){
		SafeAudioRelease(_Data->pBuffer);
		CloseHandle(_Data->hEvents);
		CloseHandle(_Data->hThread);
		_Data->hEvents		= NULL;
		_Data->hThread		= NULL;
		_Data->uHeadSize	= 0;
		_Data->uWaveSize	= 0;
		_Data->uDataSize	= 0;
		_Data->uInfoSize	= 0;
		_Data->uNextSize	= 0;
		_Data->uFileLoop	= 0;
		_Data->uFileRead	= 0;
		memset(_Data->cFilePath, 0, sizeof(wchar_t) * MAX_PATH);
		return (false);
	}
	wcscpy_s(_Data->cFilePath, MAX_PATH - 1, _File);
	_Data->cFilePath[MAX_PATH - 1]	= '\0';
	_Data->iVolume		= owl::engine::MAX_VOLUME;
	_Data->iPosition	= owl::engine::MAX_VOLUME;
	_Data->iVelocity	= 0;
	return (true);
}
