#include"owlEngine9.hpp"

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::GRAPH9()
//***********************************************************************************************************************************************************************
owl::engine::GRAPH9::GRAPH9() {
	_wsetlocale(LC_ALL, L"japanese");
	//-----	<Window>
	hWindow			= NULL;
	fAspect			= 1.0f;
	fFrameRate		= 0.0f;
	cMouseInfo		= MOUSEINFO();
	//-----	<DirectX9>
	pGraph			= NULL;
	pDevice			= NULL;
	pSurface		= NULL;
	pZBuffer		= NULL;
	vViewer			= D3DXVECTOR3(0, 0, -1);
	vLookat			= D3DXVECTOR3(0, 0,  0);
	vHeadup			= D3DXVECTOR3(0, 1,  0);
	memset(&vViewport, 0, sizeof(D3DVIEWPORT9));
	memset(&vDestport, 0, sizeof(D3DVIEWPORT9));
	memset(&cD3Dpps, 0, sizeof(D3DPRESENT_PARAMETERS));
	//-----	<Command>
	pCommand		= owl::engine::Null;
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::~GRAPH9()
//***********************************************************************************************************************************************************************
owl::engine::GRAPH9::~GRAPH9(){
	Quit();
	_wsetlocale(LC_ALL, L"C");
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::Init(const core::VECT2i &, int, int, const wchar_t *, const wchar_t *)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::Init(const core::VECT2i &_Size, int _Windowed, int _DWM, const wchar_t *_Title, const wchar_t *_Icon){
	//-----	<Desktop Window Manager>
	typedef	HRESULT (WINAPI *pfDWMce_t)(BOOL *);
	typedef	HRESULT (WINAPI *pfDWMec_t)(UINT  );
	BOOL	bUse	= (_DWM) ? (TRUE) : (FALSE);
	HMODULE	hDll	= LoadLibraryW(L"dwmapi.dll");
	if(hDll != NULL){
		pfDWMce_t	pDWMce	= reinterpret_cast<pfDWMce_t>(GetProcAddress(hDll, "DwmIsCompositionEnabled"));
		pfDWMec_t	pDWMec	= reinterpret_cast<pfDWMec_t>(GetProcAddress(hDll, "DwmEnableComposition"   ));
		if(pDWMce != NULL && pDWMec != NULL){
			if((*pDWMce)(&bUse) == S_OK && bUse == TRUE){
				(*pDWMec)((_DWM) ? (DWM_EC_ENABLECOMPOSITION) : (DWM_EC_DISABLECOMPOSITION));
			}
		}
		FreeLibrary(hDll);
	}
	//-----	<Window‰Šú‰»>
	RECT			rect	= { 0 };
	WNDCLASSEX		wcex	= { 0 };
	if(GetSystemMetrics(SM_CXSCREEN) < static_cast<int>(_Size.x) || GetSystemMetrics(SM_CYSCREEN) < static_cast<int>(_Size.y)){
		MessageBoxW(NULL, L"‰ð‘œ“x‚ª•s‘«‚µ‚Ä‚¢‚Ü‚·", L"ERROR", MB_OK | MB_ICONSTOP);
		return (false);
	}else if(GetSystemMetrics(SM_CXSCREEN) == static_cast<int>(_Size.x) && GetSystemMetrics(SM_CYSCREEN) == static_cast<int>(_Size.y)){
		_Windowed	= false;
	}
	wcex.cbSize				= sizeof(WNDCLASSEX);
	wcex.lpfnWndProc		= Proc;
	wcex.hInstance			= GetModuleHandle(NULL);
	wcex.hIcon				= (NULL == _Icon) ? (NULL) : (LoadIconW(wcex.hInstance, MAKEINTRESOURCEW(_Icon)));
	wcex.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground		= static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wcex.lpszMenuName		= NULL;
	wcex.lpszClassName		= _Title;
	if(!RegisterClassEx(&wcex)){
		return (false);
	}
	//-----	<‘‹ƒNƒ‰ƒX‚Ì“o˜^>
	if(_Windowed){
		if(NULL == (hWindow = CreateWindow(wcex.lpszClassName, wcex.lpszClassName, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, wcex.hInstance, this))){
			return (false);
		}
		RECT		rcc;
		RECT		rcw;
		GetWindowRect(hWindow, &rcw);
		GetClientRect(hWindow, &rcc);
		rect.left	= (GetSystemMetrics(SM_CXFULLSCREEN) >> 1) - (_Size.x >> 1) + (rcw.left   - rcc.left  );
		rect.right	= (GetSystemMetrics(SM_CXFULLSCREEN) >> 1) + (_Size.x >> 1) + (rcw.right  - rcc.right );
		rect.top	= (GetSystemMetrics(SM_CYFULLSCREEN) >> 1) - (_Size.y >> 1) + (rcw.top    - rcc.top   );
		rect.bottom	= (GetSystemMetrics(SM_CYFULLSCREEN) >> 1) + (_Size.y >> 1) + (rcw.bottom - rcc.bottom);
	}else{
		if(NULL == (hWindow = CreateWindow(wcex.lpszClassName, wcex.lpszClassName, WS_POPUP | WS_VISIBLE, 0, 0, _Size.x, _Size.y, NULL, NULL, wcex.hInstance, this))){
			return (false);
		}
		rect.left	=  0;
		rect.right	= _Size.x;
		rect.top	=  0;
		rect.bottom	= _Size.y;
	}
	SetWindowPos(hWindow, HWND_TOP, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0);
	ShowWindow(hWindow, SW_NORMAL);

	//-----	<DirectX9‰Šú‰»>
	D3DDISPLAYMODE					disp;
	std::vector<D3DDISPLAYMODE>		mode;
	if(NULL == (pGraph = Direct3DCreate9(D3D_SDK_VERSION))){
		SendNotifyMessage(hWindow, WM_CLOSE, 0, 0);
		return (false);
	}
	for(int i = 0, j = pGraph->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8); i < j; i++){
		pGraph->EnumAdapterModes(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8, i, &disp);
		mode.push_back(disp);
	}
	disp.Format			= D3DFMT_X8R8G8B8;
	disp.RefreshRate	= 0;
	disp.Width			= 0;
	disp.Height			= 0;
	for(int i = 0, j = mode.size(); i < j; i++){
		if(mode[i].Width == _Size.x && mode[i].Height == _Size.y){
			if(mode[i].RefreshRate == 60){
				disp	= mode[i];
				break;
			}else if(disp.RefreshRate < mode[i].RefreshRate){
				disp	= mode[i];
			}
		}
	}
	fAspect		= static_cast<float>(_Size.x) / static_cast<float>(_Size.y);
	cD3Dpps.BackBufferFormat			= disp.Format;
	cD3Dpps.BackBufferWidth				= static_cast<unsigned int>(_Size.x);
	cD3Dpps.BackBufferHeight			= static_cast<unsigned int>(_Size.y);
	cD3Dpps.SwapEffect					= D3DSWAPEFFECT_DISCARD;
	cD3Dpps.EnableAutoDepthStencil		= TRUE;
	cD3Dpps.AutoDepthStencilFormat		= D3DFMT_D24S8;
	cD3Dpps.MultiSampleType				= D3DMULTISAMPLE_NONE;
	cD3Dpps.PresentationInterval		= (disp.RefreshRate != 60) ? (D3DPRESENT_INTERVAL_IMMEDIATE) : (D3DPRESENT_INTERVAL_DEFAULT);
	cD3Dpps.Flags						= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	cD3Dpps.Windowed					= (_Windowed) ? (TRUE) : (FALSE);
	cD3Dpps.hDeviceWindow				= hWindow;
	cD3Dpps.BackBufferCount				= 2;
	if(FAILED(pGraph->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &cD3Dpps, &pDevice))){
		if(FAILED(pGraph->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &cD3Dpps, &pDevice))){
			cD3Dpps.BackBufferCount		= 1;
			if(FAILED(pGraph->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &cD3Dpps, &pDevice))){
				if(FAILED(pGraph->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &cD3Dpps, &pDevice))){
					if(FAILED(pGraph->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &cD3Dpps, &pDevice))){
						if(FAILED(pGraph->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &cD3Dpps, &pDevice))){
							return (false);
						}
					}
				}
			}
		}
	}
	if(FAILED(pDevice->GetViewport(&vViewport))){
		return (false);
	}else{
		vDestport	= vViewport;
	}	
	Defaults();
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::Init(const core::VECT2i &, const wchar_t *, const wchar_t *, const wchar_t *)
//			Extention for Application
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::Init(const core::VECT2i &_Size, const wchar_t *_Title, const wchar_t *_Icon, const wchar_t *_Menu){
	//-----	<Window‰Šú‰»>
	WNDCLASSEX		wcex	= { 0 };
	wcex.cbSize				= sizeof(WNDCLASSEX);
	wcex.lpfnWndProc		= Proc;
	wcex.hInstance			= GetModuleHandle(NULL);
	wcex.hIcon				= (NULL == _Icon) ? (NULL) : (LoadIconW(wcex.hInstance, MAKEINTRESOURCEW(_Icon)));
	wcex.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground		= static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wcex.lpszMenuName		= _Menu;
	wcex.lpszClassName		= _Title;
	if(!RegisterClassEx(&wcex)){
		return (false);
	}
	//-----	<‘‹ƒNƒ‰ƒX‚Ì“o˜^>
	if(NULL == (hWindow = CreateWindow(wcex.lpszClassName, wcex.lpszClassName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, wcex.hInstance, this))){
		return (false);
	}
	ShowWindow(hWindow, SW_NORMAL);

	//-----	<DirectX9‰Šú‰»>
	D3DDISPLAYMODE					disp;
	std::vector<D3DDISPLAYMODE>		mode;
	if(NULL == (pGraph = Direct3DCreate9(D3D_SDK_VERSION))){
		SendNotifyMessage(hWindow, WM_CLOSE, 0, 0);
		return (false);
	}
	for(int i = 0, j = pGraph->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8); i < j; i++){
		pGraph->EnumAdapterModes(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8, i, &disp);
		mode.push_back(disp);
	}
	disp.Format			= D3DFMT_X8R8G8B8;
	disp.RefreshRate	= 0;
	disp.Width			= 0;
	disp.Height			= 0;
	for(int i = 0, j = mode.size(); i < j; i++){
		if(mode[i].Width == _Size.x && mode[i].Height == _Size.y){
			if(mode[i].RefreshRate == 60){
				disp	= mode[i];
				break;
			}else if(disp.RefreshRate < mode[i].RefreshRate){
				disp	= mode[i];
			}
		}
	}
	fAspect		= static_cast<float>(_Size.x) / static_cast<float>(_Size.y);
	cD3Dpps.BackBufferFormat			= disp.Format;
	cD3Dpps.BackBufferWidth				= disp.Width ;
	cD3Dpps.BackBufferHeight			= disp.Height;
	cD3Dpps.SwapEffect					= D3DSWAPEFFECT_DISCARD;
	cD3Dpps.EnableAutoDepthStencil		= TRUE;
	cD3Dpps.AutoDepthStencilFormat		= D3DFMT_D24S8;
	cD3Dpps.MultiSampleType				= D3DMULTISAMPLE_NONE;
	cD3Dpps.PresentationInterval		= (disp.RefreshRate != 60) ? (D3DPRESENT_INTERVAL_IMMEDIATE) : (D3DPRESENT_INTERVAL_DEFAULT);
	cD3Dpps.Flags						= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	cD3Dpps.Windowed					= TRUE;
	cD3Dpps.hDeviceWindow				= hWindow;
	cD3Dpps.BackBufferCount				= 2;
	if(FAILED(pGraph->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &cD3Dpps, &pDevice))){
		if(FAILED(pGraph->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &cD3Dpps, &pDevice))){
			cD3Dpps.BackBufferCount		= 1;
			if(FAILED(pGraph->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &cD3Dpps, &pDevice))){
				if(FAILED(pGraph->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &cD3Dpps, &pDevice))){
					if(FAILED(pGraph->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &cD3Dpps, &pDevice))){
						if(FAILED(pGraph->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &cD3Dpps, &pDevice))){
							return (false);
						}
					}
				}
			}
		}
	}
	if(FAILED(pDevice->GetViewport(&vViewport))){
		return (false);
	}else{
		vDestport	= vViewport;
	}	
	Defaults();
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::Quit()
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::Quit(){
	//-----	<DirectX9‰ð•ú>
	SafeRelease(pSurface);
	SafeRelease(pZBuffer);
	SafeRelease(pDevice);
	SafeRelease(pGraph);
	memset(&cD3Dpps, 0, sizeof(D3DPRESENT_PARAMETERS));
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::Ctrl()
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::Ctrl(){
	static	DWORD	nPushTime	= timeGetTime();
	static	DWORD	nPrevTime	= timeGetTime();
	static	DWORD	nFPS		= 0;
			DWORD	nThisTime	= timeGetTime();
			MSG		cMsg;

	while(PeekMessage(&cMsg, NULL, 0, 0, PM_NOREMOVE)){
		if(GetMessage(&cMsg, NULL, 0, 0) <= 0){
			return (false);
		}
		TranslateMessage(&cMsg);
		 DispatchMessage(&cMsg);
	}
	if(cD3Dpps.PresentationInterval == D3DPRESENT_INTERVAL_IMMEDIATE){
		while((nThisTime = timeGetTime()) - nPrevTime < 16){
			Sleep(1);
		}
	}
	nPrevTime	= nThisTime;
	if(nThisTime - nPushTime >= 1000){
		fFrameRate	= static_cast<float>(nFPS) * 1000.0f / static_cast<float>(nThisTime - nPushTime);
		nPushTime	= nThisTime;
		nFPS		= 0;
	}
	nFPS++;
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::Swap(bool, bool)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::Swap(int _Windowed, int _DWM){
	//-----	<Desktop Window Manager>
	typedef	HRESULT (WINAPI *pfDWMce_t)(BOOL *);
	typedef	HRESULT (WINAPI *pfDWMec_t)(UINT  );
	BOOL	bUse	= (_DWM) ? (TRUE) : (FALSE);
	HMODULE	hDll	= LoadLibraryW(L"dwmapi.dll");
	if(hDll != NULL){
		pfDWMce_t	pDWMce	= reinterpret_cast<pfDWMce_t>(GetProcAddress(hDll, "DwmIsCompositionEnabled"));
		pfDWMec_t	pDWMec	= reinterpret_cast<pfDWMec_t>(GetProcAddress(hDll, "DwmEnableComposition"   ));
		if(pDWMce != NULL && pDWMec != NULL){
			if((*pDWMce)(&bUse) == S_OK && bUse == TRUE){
				(*pDWMec)((_DWM) ? (DWM_EC_ENABLECOMPOSITION) : (DWM_EC_DISABLECOMPOSITION));
			}
		}
		FreeLibrary(hDll);
	}
	//-----	<Window Style>
	BOOL	Windowed	= (_Windowed) ? (TRUE) : (FALSE);
	if(cD3Dpps.Windowed == Windowed){
		return (true);
	}else{
		cD3Dpps.Windowed	= !cD3Dpps.Windowed;
	}
	if(pDevice->TestCooperativeLevel() != D3D_OK){
		return (false);
	}
	SafeRelease(pSurface);
	SafeRelease(pZBuffer);
	for(LISTSURFACEITERATOR p = cListSurface.begin(); p != cListSurface.end(); p++){
		FreeResource(&(*p));
	}
	for(LISTSHADERITERATOR p = cListShader.begin(); p != cListShader.end(); p++){
		FreeResource(&(*p));
	}
	if(pDevice->Reset(&cD3Dpps) != D3D_OK){
		return (false);
	}
	for(LISTSHADERITERATOR p = cListShader.begin(); p != cListShader.end(); p++){
		InitResource(&(*p));
	}
	for(LISTSURFACEITERATOR p = cListSurface.begin(); p != cListSurface.end(); p++){
		InitResource(&(*p));
	}
	Defaults();

	RECT	rcc;
	RECT	rcw;
	RECT	rcx;
	if(cD3Dpps.Windowed){
		SetWindowLong(hWindow, GWL_STYLE, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE);
		SetWindowPos(hWindow, HWND_TOP, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
		GetWindowRect(hWindow, &rcw);
		GetClientRect(hWindow, &rcc);
		rcx.left	= (GetSystemMetrics(SM_CXFULLSCREEN) >> 1) - (cD3Dpps.BackBufferWidth  >> 1) + (rcw.left   - rcc.left  );
		rcx.right	= (GetSystemMetrics(SM_CXFULLSCREEN) >> 1) + (cD3Dpps.BackBufferWidth  >> 1) + (rcw.right  - rcc.right );
		rcx.top		= (GetSystemMetrics(SM_CYFULLSCREEN) >> 1) - (cD3Dpps.BackBufferHeight >> 1) + (rcw.top    - rcc.top   );
		rcx.bottom	= (GetSystemMetrics(SM_CYFULLSCREEN) >> 1) + (cD3Dpps.BackBufferHeight >> 1) + (rcw.bottom - rcc.bottom);
	}else{
		SetWindowLong(hWindow, GWL_STYLE, WS_POPUP | WS_VISIBLE);
		SetWindowPos(hWindow, HWND_TOP, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
		rcx.left	= 0;
		rcx.right	= cD3Dpps.BackBufferWidth;
		rcx.top		= 0;
		rcx.bottom	= cD3Dpps.BackBufferHeight;
	}
	SetWindowPos(hWindow, HWND_TOP, rcx.left, rcx.top, rcx.right - rcx.left, rcx.bottom - rcx.top, 0);
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::GraphSet()
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::GraphSet(const core::COLORf &_Color){
	if(FAILED(pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, TrueColor32(_Color), 1.0f, 0))){
		return (false);
	}
	if(FAILED(pDevice->BeginScene())){
		return (false);
	}
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::GraphEnd()
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::GraphEnd(){
	pDevice->EndScene();
	if(pDevice->Present(NULL, NULL, NULL, NULL) == D3DERR_DEVICELOST){
		if(pDevice->TestCooperativeLevel() != D3DERR_DEVICENOTRESET){
			return (false);
		}
		SafeRelease(pSurface);
		SafeRelease(pZBuffer);
		for(LISTSURFACEITERATOR p = cListSurface.begin(); p != cListSurface.end(); p++){
			FreeResource(&(*p));
		}
		for(LISTSHADERITERATOR p = cListShader.begin(); p != cListShader.end(); p++){
			FreeResource(&(*p));
		}
		if(pDevice->Reset(&cD3Dpps) != D3D_OK){
			return (false);
		}
		for(LISTSHADERITERATOR p = cListShader.begin(); p != cListShader.end(); p++){
			InitResource(&(*p));
		}
		for(LISTSURFACEITERATOR p = cListSurface.begin(); p != cListSurface.end(); p++){
			InitResource(&(*p));
		}
		Defaults();
	}
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::ClearDepth()
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::ClearDepth(){
	if(FAILED(pDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0))){
		return (false);
	}
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::Proc(HWND, UINT, WPARAM, LPARAM)
//***********************************************************************************************************************************************************************
LRESULT CALLBACK owl::engine::GRAPH9::Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	static	GRAPH9	*	pThis	= NULL;
	switch(uMsg){
		case WM_CREATE:
			pThis	= static_cast<GRAPH9 *>(reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_SYSKEYUP:
		case WM_SYSKEYDOWN:
			switch(LOWORD(wParam)){
				case VK_F4:
					PostMessage(hWnd, WM_CLOSE, 0, 0);
					break;
				default:
					break;
			}
			break;
		case WM_COMMAND:
			if(pThis->pCommand(hWnd, uMsg, wParam, lParam) != 0){
				return DefWindowProc(hWnd, uMsg, wParam, lParam);
			}
			break;
		case WM_MOUSEMOVE:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
			pThis->cMouseInfo.iX		= LOWORD(lParam);
			pThis->cMouseInfo.iY		= HIWORD(lParam);
			pThis->cMouseInfo.iZ		= 0;
			pThis->cMouseInfo.iButton	= LOWORD(wParam);
			break;
		case WM_MOUSEWHEEL:
			pThis->cMouseInfo.iX		= LOWORD(lParam);
			pThis->cMouseInfo.iY		= HIWORD(lParam);
			pThis->cMouseInfo.iZ		= static_cast<short>(HIWORD(wParam)) / WHEEL_DELTA;
			pThis->cMouseInfo.iButton	= LOWORD(wParam);
			break;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return (0);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::Defaults()
//***********************************************************************************************************************************************************************
void owl::engine::GRAPH9::Defaults(){
	pDevice->SetRenderState(D3DRS_ZENABLE         , D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE    , TRUE);
	pDevice->SetRenderState(D3DRS_LIGHTING        , TRUE);
	pDevice->SetRenderState(D3DRS_SPECULARENABLE  , TRUE);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_BLENDOP         , D3DBLENDOP_ADD      );
	pDevice->SetRenderState(D3DRS_SRCBLEND        , D3DBLEND_SRCALPHA   );
	pDevice->SetRenderState(D3DRS_DESTBLEND       , D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState(D3DRS_AMBIENT         , 0xFF404040);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC      , D3DCMP_GREATEREQUAL);
	pDevice->SetRenderState(D3DRS_ALPHAREF       , 0x08);

	pDevice->SetTextureStageState(0, D3DTSS_COLOROP  , D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE  );
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE  );
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP  , D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE  );
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE  );

	if(D3D_OK != pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC)){
		if(D3D_OK != pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR)){
			pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
		}
	}
	if(D3D_OK != pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC)){
		if(D3D_OK != pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR)){
			pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
		}
	}

	SetMatrixOrth(1.0f, 10000.0f);
	
	D3DCOLORVALUE	vColor1	= { 0.85f, 0.85f, 0.85f, 1.00f };
	D3DCOLORVALUE	vColor2	= { 0.15f, 0.15f, 0.15f, 1.00f };
	D3DCOLORVALUE	vColor3	= { 0.00f, 0.00f, 0.00f, 0.00f };
	D3DXVECTOR3		vDirect	= D3DXVECTOR3(-0.50f, -0.50f, 0.75f);
	D3DLIGHT9		nLight	= { D3DLIGHT_DIRECTIONAL, vColor1, vColor2, vColor3, D3DXVECTOR3(0, 0, 0), vDirect / D3DXVec3Length(&vDirect), 0, 0, 0, 0, 0, 0, 0 };
	pDevice->SetLight(0, &nLight);
	pDevice->LightEnable(0, TRUE);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::SetMatrixOrth(float, float)
//***********************************************************************************************************************************************************************
void owl::engine::GRAPH9::SetMatrixOrth(float _Zn, float _Zf){
	D3DXMATRIX	mView;
	D3DXMATRIX	mProj;
	D3DXMatrixLookAtLH(&mView, &vViewer, &vLookat, &vHeadup);
	pDevice->SetTransform(D3DTS_VIEW, &mView);
	D3DXMatrixOrthoLH(&mProj, static_cast<FLOAT>(cD3Dpps.BackBufferWidth), static_cast<FLOAT>(cD3Dpps.BackBufferHeight), _Zn, _Zf);
	pDevice->SetTransform(D3DTS_PROJECTION, &mProj);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::SetMatrixOrth((float, float, core::VECT2i &)
//***********************************************************************************************************************************************************************
void owl::engine::GRAPH9::SetMatrixOrth(float _Zn, float _Zf, core::VECT2i &_Size){
	D3DXMATRIX	mView;
	D3DXMATRIX	mProj;
	D3DXMatrixLookAtLH(&mView, &vViewer, &vLookat, &vHeadup);
	pDevice->SetTransform(D3DTS_VIEW, &mView);
	D3DXMatrixOrthoLH(&mProj, static_cast<FLOAT>(_Size.x), static_cast<FLOAT>(_Size.y), _Zn, _Zf);
	pDevice->SetTransform(D3DTS_PROJECTION, &mProj);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::SetMatrixPers(float, float, float)
//***********************************************************************************************************************************************************************
void owl::engine::GRAPH9::SetMatrixPers(float _Zn, float _Zf, float _Va){
	D3DXMATRIX	mView;
	D3DXMATRIX	mProj;
	D3DXMatrixLookAtLH(&mView, &vViewer, &vLookat, &vHeadup);
	pDevice->SetTransform(D3DTS_VIEW, &mView);
	D3DXMatrixPerspectiveFovLH(&mProj, _Va, fAspect, _Zn, _Zf);
	pDevice->SetTransform(D3DTS_PROJECTION, &mProj);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::SetMatrixPers(float, float, float, float)
//***********************************************************************************************************************************************************************
void owl::engine::GRAPH9::SetMatrixPers(float _Zn, float _Zf, float _Va, float _Aspect){
	D3DXMATRIX	mView;
	D3DXMATRIX	mProj;
	D3DXMatrixLookAtLH(&mView, &vViewer, &vLookat, &vHeadup);
	pDevice->SetTransform(D3DTS_VIEW, &mView);
	D3DXMatrixPerspectiveFovLH(&mProj, _Va, _Aspect, _Zn, _Zf);
	pDevice->SetTransform(D3DTS_PROJECTION, &mProj);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::SetViewport()
//***********************************************************************************************************************************************************************
void owl::engine::GRAPH9::SetViewport(const core::VECT2i &_From, const core::VECT2i &_Size, const core::COLORf &_Color){
	D3DVIEWPORT9	vView;
	vView.X			= _From.x;
	vView.Y			= _From.y;
	vView.Width		= _Size.x;
	vView.Height	= _Size.y;
	vView.MinZ		= 0.0f;
	vView.MaxZ		= 1.0f;
	vDestport		= vView;
	if(SUCCEEDED(pDevice->SetViewport(&vView))){
		pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, TrueColor32(_Color), 1.0f, 0);
	}
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::EndViewport()
//***********************************************************************************************************************************************************************
void owl::engine::GRAPH9::EndViewport(){
	pDevice->SetViewport(&vViewport);
	vDestport	= vViewport;
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::SetLightVec(const core::VECT3f &, const core::COLORf &, const core::COLORf &, const core::COLORf &)
//***********************************************************************************************************************************************************************
void owl::engine::GRAPH9::SetLightVec(const core::VECT3f &_in, const core::COLORf &_Ambient, const core::COLORf &_Diffuse, const core::COLORf &_Specular){
	D3DXVECTOR3		vDirect		= Vect(_in);
	D3DCOLORVALUE	vDiffuse	= {  _Diffuse.r,  _Diffuse.g,  _Diffuse.b,  _Diffuse.a };
	D3DCOLORVALUE	vSpecluar	= { _Specular.r, _Specular.g, _Specular.b, _Specular.a };
	D3DCOLORVALUE	vAmbient	= {  _Ambient.r,  _Ambient.g,  _Ambient.b,  _Ambient.a };
	D3DLIGHT9		nLight		= { D3DLIGHT_DIRECTIONAL, vDiffuse, vSpecluar, vAmbient, D3DXVECTOR3(0, 0, 0), vDirect / D3DXVec3Length(&vDirect), 0, 0, 0, 0, 0, 0, 0 };
	pDevice->SetLight(0, &nLight);
	pDevice->LightEnable(0, TRUE);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::GetMouseInfo()
//***********************************************************************************************************************************************************************
owl::engine::MOUSEINFO owl::engine::GRAPH9::GetMouseInfo(){
	return cMouseInfo;
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::DrawLine()
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::DrawLine(const core::VECT3f &_Point, const core::VECT2f &_Size, const core::COLORf &_Color){
	pDevice->SetFVF(FVF_XDVERTEX2D);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	XDVERTEX2D	nVertex[]	= {
		{ D3DXVECTOR4(_Point.x          , _Point.y          , _Point.z, 1.0f), TrueColor32(_Color) },
		{ D3DXVECTOR4(_Point.x + _Size.x, _Point.y + _Size.y, _Point.z, 1.0f), TrueColor32(_Color) }
	};
	pDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 1, nVertex, sizeof(XDVERTEX2D));
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::DrawRect()
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::DrawRect(const core::VECT3f &_Point, const core::VECT2f &_Size, const core::COLORf &_Color){
	pDevice->SetFVF(FVF_XDVERTEX2D);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	XDVERTEX2D	nVertex[]	= {
		{ D3DXVECTOR4(_Point.x          , _Point.y + _Size.y, _Point.z, 1.0f), TrueColor32(_Color) },
		{ D3DXVECTOR4(_Point.x          , _Point.y          , _Point.z, 1.0f), TrueColor32(_Color) },
		{ D3DXVECTOR4(_Point.x + _Size.x, _Point.y + _Size.y, _Point.z, 1.0f), TrueColor32(_Color) },
		{ D3DXVECTOR4(_Point.x + _Size.x, _Point.y          , _Point.z, 1.0f), TrueColor32(_Color) }
	};
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, nVertex, sizeof(XDVERTEX2D));
	return (true);
}
