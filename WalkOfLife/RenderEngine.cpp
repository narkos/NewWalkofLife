//############################################################################################################################################################################################################################################################################################################
//###																																																																									   ###
//###																													WALK OF LIFE MAIN FILE																																							   ###	
//###																																																																									   ###	
//############################################################################################################################################################################################################################################################################################################



#include "RenderEngine.h"
#include <string.h>



namespace{
	RenderEngine* pRenderEngine; //pointer to the application
}

// MSG HANDLER FOR WINMAIN  ( Gives feedback íf program crashes)

LRESULT CALLBACK MainWindowProc(HWND hwindow, UINT msg, WPARAM wParam, LPARAM lParam){ //denna används endast för att komma åt RenderEnginens MsgProc() och skicka vidare meddelanden till den
	return pRenderEngine->MsgProc(hwindow, msg, wParam, lParam);
}

// CONSTRUCTOR

RenderEngine::RenderEngine(HINSTANCE hInstance, std::string name, UINT scrW, UINT scrH){
	this->hInstance = hInstance;
	applicationName = name;
	screen_Width = mainCamera.getWindowWidth();
	screen_Height = mainCamera.getWindowHeight();
	//screen_Width = scrW; //OLD
	//screen_Height = scrH; //OLD
	pRenderEngine = this;
	windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX;
	//this->theQuadtree = new Quadtree(0, 0, 100, 100, 1, 6);
	this->theBinaryTree = new BinaryTree(100, 100);
	this->LoadSounds();
}

// DESTRUCTOR

RenderEngine::~RenderEngine(){ //destruktor

}


//######################################################################################################################################################
//###																																				 ###
//###										   INTITIALIZE FUNCTIONS IN : init()																	 ###	
//###																																				 ###	
//######################################################################################################################################################

// INITIALIZE CORE FUNCTIONS
bool RenderEngine::Init(){

	if (!InitWindow()){
		return false; //gick inte att skapa window
	}

	if (!InitDirect3D(hWindow)){
		return false; //gick inte att skapa Direct3D
	}
	// Sets and creates viewport
	SetViewport();

	//Initialize Shaders and triangle data
	Shaders();
	CreatePlaneData();
	TextureFunc();

	//Font
	Fonts();

	//highscore stuff
	//theHighScore.AddScore(5, 2, 13);

	//theHighScore.AddScore(2, 9, 3);
	//theHighScore.AddScore(1, 2, 44);
	//theHighScore.AddScore(1, 2, 1);
	//theHighScore.ReOrganizeLists();
	//Import
	
	ImportObj("Objects/testPlayer1.obj", "Objects/testPlayer1.mtl", gDevice, 0, false);
	
	//ImportObj("Objects/mapPart1.obj", "Objects/mapPart1.mtl", gDevice, false);
	//ImportObj("Objects/mapPart2.obj", "Objects/mapPart2.mtl", gDevice, false);
	ImportObj("Objects/mapPart3.obj", "Objects/mapPart3.mtl", gDevice, 1, true);
	//theBinaryTree->testPlatforms->at(0).at(0).Translate(0.0f, -2000.0f, 0.0f);
	//ImportObj("Objects/mapPart4.obj", "Objects/mapPart4.mtl", gDevice, 1, true);
	ImportObj("Objects/mapPart5.obj", "Objects/mapPart5.mtl", gDevice, 1, false);
	//ImportObj("Objects/mapPart6.obj", "Objects/mapPart6.mtl", gDevice, 1, true);

	ImportObj("Objects/mapPart7.obj", "Objects/mapPart7.mtl", gDevice, 1, true);
	//ImportObj("Objects/mapPart7.obj", "Objects/mapPart7.mtl", gDevice, 2);
	int test = 1;
	ImportObj("Objects/sphrThingy_01.obj", "Objects/sphrThingy_01.mtl", gDevice, 2, true);

	////LIGHT TEST ZONE BITCHES

	//testLight[0] = LightClass(l_Directional, XMFLOAT3(0.0f, -1.0f, 0.0f), true, true);
	//testLight[0].lightObject.Color = XMFLOAT4(Colors::White);
	////testLight[0].ToggleActive();

	//testLight[1] = LightClass(l_Point, XMFLOAT3(-3.3f, 8.0f, -4.0f), true, true);

	//testLight[1].lightObject.Color = XMFLOAT4(Colors::LightCyan);
	//testLight[1].lightObject.AttConst = 1.0f;
	//testLight[1].lightObject.AttLinear = 0.8f;
	//testLight[1].lightObject.AttQuadratic = 0.0001f;
	//testLight[1].lightObject.Range = 10.0f;
	////testLight[1].ToggleActive();

	//lightProp01.lights[0] = testLight[0].lightObject;
	//lightProp01.lights[1] = testLight[1].lightObject;

 //	lightProp01.GlobalAmbient = XMFLOAT4(Colors::Yellow);



	lightProp01.lights[2].Position = XMFLOAT4(5.0f, -3.0f, 0.0f, 1.0f);
	lightOffsetTest = 0.0f;

	D3D11_BUFFER_DESC lbuffDesc;
	ZeroMemory(&lbuffDesc, sizeof(lbuffDesc));
	lbuffDesc.Usage = D3D11_USAGE_DEFAULT;
	lbuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lbuffDesc.ByteWidth = sizeof(LightProperties);
	lbuffDesc.CPUAccessFlags = 0;
	lbuffDesc.MiscFlags = 0;
	

	HRESULT hr = gDevice->CreateBuffer(&lbuffDesc, NULL, &lightConstBuff);
	

	// Material Buffers Init
	D3D11_BUFFER_DESC mbuffDesc;
	ZeroMemory(&lbuffDesc, sizeof(lbuffDesc));
	mbuffDesc.Usage = D3D11_USAGE_DEFAULT;
	mbuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	mbuffDesc.ByteWidth = sizeof(MaterialProperties);
	mbuffDesc.CPUAccessFlags = 0;
	mbuffDesc.MiscFlags = 0;
	hr = gDevice->CreateBuffer(&mbuffDesc, NULL, &matConstBuff);



	return true; //om båda funkade så returnera true (y)
}

// INITIALIZE WINDOW

bool RenderEngine::InitWindow(){
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.hInstance = hInstance;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW; // hur den ska måla ut allt, fast spelar nog ingen roll i vårt fall
	wcex.lpfnWndProc = MainWindowProc;
	//wcex.hCursor = LoadCursor(NULL, IDC_NO); //skoj
	//wcex.hIcon = LoadIcon(NULL, IDI_ERROR); //skoj * 2
	//wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"RENDERENGINECLASS";
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex)){
		MessageBox(NULL, L"Kunde inte registrera window classen", NULL, NULL);
		return false;
	}

	RECT r = { 0, 0, screen_Width, screen_Height };
	AdjustWindowRect(&r, windowStyle, FALSE); //kommer inte kunna resiza skiten
	int width = r.right - r.left;
	int height = r.bottom - r.top;
	mainwname = L"Build: Walk_Of_Life.0008";
	hWindow = CreateWindow(L"RENDERENGINECLASS",
		L"Direct3D Projekt", //INTE SÄKER PÅ DETTA, namnet på applikationen blir typ i kinaspråk så venne, kan vara detta
		WS_OVERLAPPEDWINDOW,//Window handlers
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width, height, nullptr, nullptr, hInstance, nullptr);

	if (!hWindow){
		MessageBox(NULL, L"Failed to create window", NULL, NULL);
		return false;
	}


	return true;
}

// CREATE VIEWPORT

void RenderEngine::SetViewport()
{
	D3D11_VIEWPORT vp;
	vp.Width = screen_Width;
	vp.Height = screen_Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	gDeviceContext->RSSetViewports(1, &vp);
}

// FPS COUNTER

void RenderEngine::fpscounter()
{
	// Code computes the avarage frame time and frames per second

	static int framecount = 0;
	static float time = 0.0f;
	static float time2 = 0.0f;

	//Function is beeing initialiased every time a frame is made
	framecount++;

	// Compute averages over one second period
	if ((gTimer.TotalTime() - time) >= 0.25f)
	{
		float fps = (float)framecount * 4; // fps = framecount / 1
		fpsDisplay = fps;
		float mspf = 1000.0f / fps;
		float timer = gTimer.TotalTime();
		// Makes a String for the window handler
		std::wostringstream outs;
		
		outs.precision(6);
		outs << mainwname << L" "   
			<< L"        Frame Time: " << mspf << L" (ms)"
			<< L"        Time: " << timer << L" sec";
		
		//Prints the text in the window handler
		SetWindowText(hWindow, outs.str().c_str());

		// Reset for next fps.
		framecount = 0;
		time += 0.25f;

	
		
	}

	if ((gTimer.TotalTime() - time2) >= 0.20f && gCounter.theAge.years < 100){
		gCounter.setMonth(1);
		time2 += 0.20f;

	}
		

	
}

// CREATE TEXTURES

void RenderEngine::TextureFunc(){

	HRESULT texCheck;
	texCheck = CreateDDSTextureFromFile(gDevice, L"Textures/temp.dds", nullptr, &ddsTex1);

}

 // CREATE FONTS

void RenderEngine::Fonts(){

	spritefont.reset(new DirectX::SpriteFont(gDevice, L"Fonts/Arial.spritefont"));
	spriteBatch.reset(new DirectX::SpriteBatch(gDeviceContext));
}

// COMPILE SHADERS FROM .HLSL FILES

HRESULT CompileShader(_In_ LPCWSTR srcFile, _In_ LPCSTR entryPoint, _In_ LPCSTR profile, _Outptr_ ID3DBlob** blob)
{
	if (!srcFile || !entryPoint || !profile || !blob)
		return E_INVALIDARG;

	*blob = nullptr;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif

	const D3D_SHADER_MACRO defines[] =
	{
		"EXAMPLE_DEFINE", "1",
		NULL, NULL
	};

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompileFromFile(srcFile, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint, profile,
		flags, 0, &shaderBlob, &errorBlob);
	if (FAILED(hr))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}

		if (shaderBlob)
			shaderBlob->Release();

		return hr;
	}

	*blob = shaderBlob;

	return hr;
}

// CREATE AND COMPILE SHADERS

void RenderEngine::Shaders(){
	// SHADER TESTER
	HRESULT ShaderTest;

	//DEAFULT SHADERS

	//create vertex shader
	ID3DBlob* pVS = nullptr;
	ID3DBlob* errorMessage = nullptr;
	ShaderTest = CompileShader(L"defaultVS.hlsl", "VS_main", "vs_5_0", &pVS);
	ShaderTest = gDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &gVertexShader);

	//create input layout (verified using vertex shader)
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	ShaderTest = gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &gVertexLayout);
	
	//create pixel shader
	ID3DBlob* pPS = nullptr;
	ShaderTest = CompileShader(L"defaultPS.hlsl", "PS_main", "ps_5_0", &pPS);
	ShaderTest = gDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &gPixelShader);

	//wireframe
	D3D11_INPUT_ELEMENT_DESC inputDescPosOnly[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	gDevice->CreateInputLayout(inputDescPosOnly, ARRAYSIZE(inputDescPosOnly), pVS->GetBufferPointer(), pVS->GetBufferSize(), &gWireFrameLayout);


	HRESULT hrWireFrameVS = CompileShader(L"WireFrameVS.hlsl", "main", "vs_5_0", &pVS);
	gDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &gWireFrameVertexShader);

	HRESULT hrWireFramePS = CompileShader(L"WireFramePS.hlsl", "main", "ps_5_0", &pPS);
	gDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &gWireFramePixelShader);


	// Realese shaders
	pVS->Release();
	pPS->Release();
}

// CREATE BUFFERS AND TEMP PLANE

void RenderEngine::CreatePlaneData(){

	struct PlaneVertex
	{
		float x, y, z;
		float ux, uy;
		float nx, ny, nz;
	}
	PlaneVertices[4] =
	{
		-1.8f, -1.8f, -0.9f, //v0 pos
		0.0f, 1.0f,
		0.0f, 0.0f, -1.0f, //n0

		-1.8f, 1.8f, -0.9f, //v1
		0.0f, 0.0f,
		0.0f, 0.0f, -1.0f,  //n1

		1.8f, -1.8f, -0.9f, //v2
		1.0f, 1.0f,
		0.0f, 0.0f, -1.0f,  //n2

		1.8f, 1.8f, -0.9f, //v3
		1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,

	};

	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(PlaneVertices);

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = PlaneVertices;
	gDevice->CreateBuffer(&bufferDesc, &data, &gVertexBuffer);


	// Rotatation And transform Buffer
	D3D11_BUFFER_DESC transformbuffer;
	memset(&transformbuffer, 0, sizeof(transformbuffer));
	transformbuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	transformbuffer.Usage = D3D11_USAGE_DEFAULT;
	transformbuffer.ByteWidth = sizeof(World);

	HRESULT hr1112 = gDevice->CreateBuffer(&transformbuffer, NULL, &gWorld);
	hr1112 = gDevice->CreateBuffer(&transformbuffer, NULL, &cWorld);
}

// INITIALIZE DIRECTX OBJECT


bool RenderEngine::InitDirect3D(HWND hWindow){

	DXGI_SWAP_CHAIN_DESC scd;

	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hWindow;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.SampleDesc.Count = 1; //anti-aliasing
	
	//scd.SampleDesc.Quality - kan vi mecka senare men är lite saker som ska göras då
	scd.Windowed = TRUE;
	//scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	//scd.Flags = 0;

	// create a device, device context and swap chain using the information in the scd struct
	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&gSwapChain,
		&gDevice,
		NULL,
		&gDeviceContext);

	if (SUCCEEDED(hr))
	{
		gSwapChain->ResizeBuffers(1, screen_Width, screen_Height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
		// get the address of the back buffer
		ID3D11Texture2D* pBackBuffer = nullptr;
		gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		// use the back buffer address to create the render target
		gDevice->CreateRenderTargetView(pBackBuffer, NULL, &gBackRufferRenderTargetView);
		pBackBuffer->Release();


		////DepthBuffer
		//D3D11_TEXTURE2D_DESC depthStencilDesc;
		//ZeroMemory(&depthStencilDesc, sizeof(D3D11_TEXTURE2D_DESC));
		//depthStencilDesc.Width = screen_Width;
		//depthStencilDesc.Height = screen_Height;
		//depthStencilDesc.MipLevels = 1;
		//depthStencilDesc.ArraySize = 1;
		//depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		//depthStencilDesc.SampleDesc.Count = 1;
		//depthStencilDesc.SampleDesc.Quality = 0;
		//depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		//depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		//depthStencilDesc.CPUAccessFlags = 0;
		//depthStencilDesc.MiscFlags = 0;

		//HRESULT hr1 = gDevice->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
		//HRESULT hr2 = gDevice->CreateDepthStencilView(depthStencilBuffer, NULL, &gDepthStencilView);



		//DepthBuffer
		D3D11_TEXTURE2D_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(D3D11_TEXTURE2D_DESC));
		depthStencilDesc.Width = screen_Width;
		depthStencilDesc.Height = screen_Height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		HRESULT hr1 = gDevice->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;

		HRESULT hr2 = gDevice->CreateDepthStencilView(depthStencilBuffer, &descDSV, &gDepthStencilView);

		D3D11_DEPTH_STENCIL_DESC dsDesc;
		ZeroMemory(&dsDesc, sizeof(dsDesc));
		//Depth test settings
		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
		//Stencil tests
		dsDesc.StencilEnable = true;
		dsDesc.StencilReadMask = 0xFF;
		dsDesc.StencilWriteMask = 0xFF;
		//Stencil operations - Pixel Front Facing
		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		//Stencil operations - Pixel Back Facing
		dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		HRESULT hr3 = gDevice->CreateDepthStencilState(&dsDesc, &gDepthStencilState);
		gDeviceContext->OMSetDepthStencilState(gDepthStencilState, 0);

		
		
		// set the render target as the back buffer
		gDeviceContext->OMSetRenderTargets(1, &gBackRufferRenderTargetView, gDepthStencilView);

		return true; //returnerar att den HAR klarat av att skapa device och swapchain
	}

	return false; //det gick inte att skapa device och swapchain, snyft :'(
}

// MESSAGE HANDLER

LRESULT RenderEngine::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch (msg){
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_CHAR: //en tangentbordsknapp har tryckts i
		switch (wParam){
		case VK_ESCAPE:
			PostQuitMessage(0);
			return 0;
		}
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}


//######################################################################################################################################################
//###																																				 ###
//###								Call functions that shall be updated frame by frame in :  Run() ,  Update()	        							 ###	
//###																																				 ###	
//######################################################################################################################################################

// RUN PROGRAM

int RenderEngine::Run(){

	//om allt funkat:
	ShowWindow(hWindow, SW_SHOW);
	MSG msg = { 0 }; //töm alla platser i msg
	gTimer.Reset();
	while (msg.message != WM_QUIT){
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else{ //applikationen är fortfarande igång
			gTimer.Tick();
			fpscounter();
			if ((gTimer.TotalTime() - time3) >= 0.01f)
			{

				Update(0.0f);
				Render();
				time3 = gTimer.TotalTime();
			}
			
		}
	}
	return static_cast<int>(msg.wParam);
}

// RENDER

void RenderEngine::Render(){
	
	static float rot = 0.00f;
	UINT32 vertexSize = sizeof(float) * 8;
	UINT32 offset = 0;
	rot += 0.01;
	float clearColor[] = { 0.15f,0.6f,1.0f, 0.2f };
	gDeviceContext->OMSetBlendState(0, 0, 0xffffffff);
	gDeviceContext->OMSetRenderTargets(1, &gBackRufferRenderTargetView, gDepthStencilView);
	gDeviceContext->ClearRenderTargetView(gBackRufferRenderTargetView, clearColor);
	gDeviceContext->ClearDepthStencilView(gDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	// Draw Text
	spriteBatch->Begin();

	std::wstring yearCount = std::to_wstring(gCounter.theAge.years);
	std::wstring monthCount = std::to_wstring(gCounter.theAge.months);
	std::wstring xPos = std::to_wstring(theCharacter->xPos);
	std::wstring yPos = std::to_wstring(theCharacter->yPos);
	std::wstring dass = std::to_wstring(fpsDisplay);
	std::wstring name(L"FPS: ");
	std::wstring year(L"\nYear: ");
	std::wstring month(L"\nMonth: ");
	std::wstring XXX(L"\nxPos: ");
	std::wstring YYY(L"\nyPos: ");
	std::wstring superutedass = name + dass + year + yearCount + month + monthCount + XXX + xPos + YYY + yPos;

	std::wstring Gameover(L"\nGAME OVER MOTHERFUCKER!! ");


	const wchar_t* AMAZING_SUPER_UTE_DASS = superutedass.c_str();

	if (gCounter.theAge.years == 100){

		AMAZING_SUPER_UTE_DASS = Gameover.c_str();

	}

	spritefont->DrawString(spriteBatch.get(), AMAZING_SUPER_UTE_DASS, DirectX::SimpleMath::Vector2(0, 10));

	spriteBatch->End();
	///////////////////////////////////////////
	gDeviceContext->IASetInputLayout(gVertexLayout);
	gDeviceContext->OMSetDepthStencilState(gDepthStencilState, 0);
	int bajs = 1;
	mainCamera.setPlayerXPos(theCharacter->xPos);
	mainCamera.setPlayerYPos(theCharacter->yPos);

	mainCamera.updateCamera();
	//WORLD
	XMMATRIX YRotation = XMMatrixRotationY(rot);

	//The Camera Matrices are now defined in the camera class (mainCamera)

	XMMATRIX CamView = mainCamera.getCamView();
	XMMATRIX CamProjection = mainCamera.getCamProjection();
	XMMATRIX identityM = XMMatrixIdentity();
	XMMATRIX WorldInv = XMMatrixInverse(nullptr, XMMatrixIdentity());

	// LIGHT BUFFER UPDATE
	lightProp01.CamPosition = XMFLOAT4(mainCamera.getCameraXPos(), mainCamera.getCameraYPos(), mainCamera.getCameraZPos(), 0.0f);
	gDeviceContext->UpdateSubresource(lightConstBuff, 0, NULL, &lightProp01, 0, 0);
	gDeviceContext->PSSetShader(gPixelShader, nullptr, 0);
	gDeviceContext->PSSetConstantBuffers(2, 1, &lightConstBuff);
	gDeviceContext->PSSetConstantBuffers(1, 1, &matConstBuff);
	// END LIGHT BUFFER UPDATE


	World perObjCBData;

	XMMATRIX WVP;
	WVP = identityM* CamView*CamProjection;


	XMStoreFloat4x4(&perObjCBData.WVP, XMMatrixTranspose(WVP));
	XMStoreFloat4x4(&perObjCBData.View, XMMatrixTranspose(CamView));
	XMStoreFloat4x4(&perObjCBData.Projection, XMMatrixTranspose(CamProjection));
	XMStoreFloat4x4(&perObjCBData.WorldSpace, XMMatrixTranspose(XMMatrixIdentity()));
	XMStoreFloat4x4(&perObjCBData.InvWorld, XMMatrixTranspose(WorldInv));

	
	gDeviceContext->UpdateSubresource(gWorld, 0, NULL, &perObjCBData, 0, 0);
	gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);
	gDeviceContext->IASetVertexBuffers(0, 1, &gVertexBuffer, &vertexSize, &offset);
	
	
	//RENDER OBJ FILES

	for each (GameObject var in theBinaryTree->testPlatforms->at(theCharacter->getDivision()))
	{
			gDeviceContext->PSSetShaderResources(0, 1, &ddsTex1);
	
		gDeviceContext->IASetInputLayout(gVertexLayout);
		gDeviceContext->IASetVertexBuffers(0, 1, &var.vertexBuffer, &vertexSize, &offset);
		gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		gDeviceContext->VSSetShader(gVertexShader, nullptr, 0);
		gDeviceContext->HSSetShader(nullptr, nullptr, 0);
		gDeviceContext->DSSetShader(nullptr, nullptr, 0);
		gDeviceContext->PSSetShader(gPixelShader, nullptr, 0);
		
		var.CalculateWorld();


		var.material = MatPresets::Emerald;
		matProperties.Material = var.material;
		gDeviceContext->UpdateSubresource(matConstBuff, 0, nullptr, &matProperties, 0, 0);
		
		XMStoreFloat4x4(&perObjCBData.InvWorld, XMMatrixTranspose(XMMatrixInverse(nullptr, var.world)));
		XMStoreFloat4x4(&perObjCBData.WorldSpace, XMMatrixTranspose(var.world));
		WVP = XMMatrixIdentity();
		WVP = var.world * CamView *CamProjection;

		XMStoreFloat4x4(&perObjCBData.WVP, XMMatrixTranspose(WVP));


		gDeviceContext->UpdateSubresource(gWorld, 0, NULL, &perObjCBData, 0, 0);
		gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);

		gDeviceContext->Draw(var.nrElements * 3, 0);
	}

	for (int i = 0; i < theBinaryTree->renderObjects->at(theCharacter->getDivision()).size(); i++)
	{
		gDeviceContext->PSSetShaderResources(0, 1, &ddsTex1);

		gDeviceContext->IASetInputLayout(gVertexLayout);
		gDeviceContext->IASetVertexBuffers(0, 1, &theBinaryTree->testPlatforms->at(theCharacter->getDivision())[i].vertexBuffer, &vertexSize, &offset);
		gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		gDeviceContext->VSSetShader(gVertexShader, nullptr, 0);
		gDeviceContext->HSSetShader(nullptr, nullptr, 0);
		gDeviceContext->DSSetShader(nullptr, nullptr, 0);
		gDeviceContext->PSSetShader(gPixelShader, nullptr, 0);

		theBinaryTree->testPlatforms->at(theCharacter->getDivision())[i].CalculateWorld();
		theBinaryTree->testPlatforms->at(theCharacter->getDivision())[i].material = MatPresets::Emerald;
		theBinaryTree->testPlatforms->at(theCharacter->getDivision())[i].material.SpecPow = 38.0f;

		matProperties.Material = theBinaryTree->testPlatforms->at(theCharacter->getDivision())[i].material;

		gDeviceContext->UpdateSubresource(matConstBuff, 0, nullptr, &matProperties, 0, 0);


		XMStoreFloat4x4(&perObjCBData.InvWorld, XMMatrixTranspose(XMMatrixInverse(nullptr, theBinaryTree->testPlatforms->at(theCharacter->getDivision())[i].world)));
		XMStoreFloat4x4(&perObjCBData.WorldSpace, XMMatrixTranspose(theBinaryTree->testPlatforms->at(theCharacter->getDivision())[i].world));
		WVP = XMMatrixIdentity();
		WVP = theBinaryTree->testPlatforms->at(theCharacter->getDivision())[i].world * CamView *CamProjection;

		XMStoreFloat4x4(&perObjCBData.WVP, XMMatrixTranspose(WVP));


		gDeviceContext->UpdateSubresource(gWorld, 0, NULL, &perObjCBData, 0, 0);
		gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);

		gDeviceContext->Draw(theBinaryTree->testPlatforms->at(theCharacter->getDivision())[i].nrElements * 3, 0);
	}

	//for each (GameObject var in theBinaryTree->renderObjects->at(theCharacter->getDivision()))
	//{
	//	gDeviceContext->PSSetShaderResources(0, 1, &ddsTex1);

	//	gDeviceContext->IASetInputLayout(gVertexLayout);
	//	gDeviceContext->IASetVertexBuffers(0, 1, &var.vertexBuffer, &vertexSize, &offset);
	//	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//	gDeviceContext->VSSetShader(gVertexShader, nullptr, 0);
	//	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	//	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	//	gDeviceContext->PSSetShader(gPixelShader, nullptr, 0);

	//	var.CalculateWorld();
	//	var.material = MatPresets::Emerald;
	//	var.material.SpecPow = 38.0f;

	//	matProperties.Material = var.material;

	//	gDeviceContext->UpdateSubresource(matConstBuff, 0, nullptr, &matProperties, 0, 0);



	//	XMStoreFloat4x4(&perObjCBData.InvWorld, XMMatrixTranspose(XMMatrixInverse(nullptr, var.world)));
	//	XMStoreFloat4x4(&perObjCBData.WorldSpace, XMMatrixTranspose(var.world));
	//	WVP = XMMatrixIdentity();
	//	WVP = var.world * CamView *CamProjection;

	//	XMStoreFloat4x4(&perObjCBData.WVP, XMMatrixTranspose(WVP));


	//	gDeviceContext->UpdateSubresource(gWorld, 0, NULL, &perObjCBData, 0, 0);
	//	gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);

	//	gDeviceContext->Draw(var.nrElements * 3, 0);
	//}
	for (int i = 0; i < theBinaryTree->renderObjects->at(theCharacter->getDivision()).size(); i++)
	{
		gDeviceContext->PSSetShaderResources(0, 1, &ddsTex1);

		gDeviceContext->IASetInputLayout(gVertexLayout);
		gDeviceContext->IASetVertexBuffers(0, 1, &theBinaryTree->renderObjects->at(theCharacter->getDivision())[i].vertexBuffer, &vertexSize, &offset);
		gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		gDeviceContext->VSSetShader(gVertexShader, nullptr, 0);
		gDeviceContext->HSSetShader(nullptr, nullptr, 0);
		gDeviceContext->DSSetShader(nullptr, nullptr, 0);
		gDeviceContext->PSSetShader(gPixelShader, nullptr, 0);

		theBinaryTree->renderObjects->at(theCharacter->getDivision())[i].CalculateWorld();
		theBinaryTree->renderObjects->at(theCharacter->getDivision())[i].material = MatPresets::Emerald;
		theBinaryTree->renderObjects->at(theCharacter->getDivision())[i].material.SpecPow = 38.0f;

		matProperties.Material = theBinaryTree->renderObjects->at(theCharacter->getDivision())[i].material;

		gDeviceContext->UpdateSubresource(matConstBuff, 0, nullptr, &matProperties, 0, 0);



		XMStoreFloat4x4(&perObjCBData.InvWorld, XMMatrixTranspose(XMMatrixInverse(nullptr, theBinaryTree->renderObjects->at(theCharacter->getDivision())[i].world)));
		XMStoreFloat4x4(&perObjCBData.WorldSpace, XMMatrixTranspose(theBinaryTree->renderObjects->at(theCharacter->getDivision())[i].world));
		WVP = XMMatrixIdentity();
		WVP = theBinaryTree->renderObjects->at(theCharacter->getDivision())[i].world * CamView *CamProjection;

		XMStoreFloat4x4(&perObjCBData.WVP, XMMatrixTranspose(WVP));


		gDeviceContext->UpdateSubresource(gWorld, 0, NULL, &perObjCBData, 0, 0);
		gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);

		gDeviceContext->Draw(theBinaryTree->renderObjects->at(theCharacter->getDivision())[i].nrElements * 3, 0);
	}


	//wireframe bbox
	UINT32 bufferElementSize = sizeof(XMFLOAT3);
	UINT32 offset1 = 0;

	gDeviceContext->IASetInputLayout(gWireFrameLayout);
	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	gDeviceContext->VSSetShader(gWireFrameVertexShader, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->GSSetShader(nullptr, nullptr, 0);
	gDeviceContext->PSSetShader(gWireFramePixelShader, nullptr, 0);

	for (int i = 0; i < theBinaryTree->testPlatforms->at(theCharacter->getDivision()).size(); i++){
		gDeviceContext->IASetVertexBuffers(0, 1, &theBinaryTree->testPlatforms->at(theCharacter->getDivision())[i].boundingBoxVertexBuffer, &bufferElementSize, &offset1);
		theBinaryTree->testPlatforms->at(theCharacter->getDivision())[i].CalculateWorld();
		XMStoreFloat4x4(&perObjCBData.InvWorld, XMMatrixTranspose(XMMatrixInverse(nullptr, theBinaryTree->testPlatforms->at(theCharacter->getDivision())[i].world)));
		XMStoreFloat4x4(&perObjCBData.WorldSpace, XMMatrixTranspose(theBinaryTree->testPlatforms->at(theCharacter->getDivision())[i].world));
		//var.world = XMMatrixIdentity();
		gDeviceContext->UpdateSubresource(gWorld, 0, NULL, &perObjCBData, 0, 0);
		gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);
		gDeviceContext->Draw(16, 0);
	}



	gDeviceContext->PSSetShaderResources(0, 1, &ddsTex1);

	gDeviceContext->IASetInputLayout(gVertexLayout);
	gDeviceContext->IASetVertexBuffers(0, 1, &theCharacter->vertexBuffer, &vertexSize, &offset);
	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gDeviceContext->VSSetShader(gVertexShader, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->PSSetShader(gPixelShader, nullptr, 0);

	theCharacter->CalculateWorld();
	
	
	WVP = XMMatrixIdentity();
	WVP = theCharacter->world * CamView *CamProjection;
	theCharacter->world = XMMatrixTranspose(theCharacter->world);

	XMStoreFloat4x4(&perObjCBData.WVP, XMMatrixTranspose(WVP));
	XMStoreFloat4x4(&perObjCBData.WorldSpace, XMMatrixTranspose(theCharacter->world));
	XMStoreFloat4x4(&perObjCBData.InvWorld, XMMatrixTranspose(XMMatrixInverse(nullptr, theCharacter->world)));

	gDeviceContext->UpdateSubresource(gWorld, 0, NULL, &perObjCBData, 0, 0);
	gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);



	gDeviceContext->Draw(theCharacter->nrElements * 3, 0);

	//////////////////////////////////////////////////////////////
	
	gSwapChain->Present(0, 0); //växla back/front buffer
}

// UPDATES

void RenderEngine::Update(float dt){
	
	soundBackground.PlayMp3();
		Input theInput;
		theInput.initInput(this->hInstance, hWindow);
		int input = 0;
		bool jump = false;
		bool dash = false;

		input = theInput.detectInput(hWindow);
		jump = theInput.detectJump(hWindow);
		dash = theInput.detectDash(hWindow);

		if (gTimer.TotalTime() - theCharacter->dashTimer > 0.30f && !theCharacter->dashDisabling)
		{
			theCharacter->momentum = 1;
			if (theCharacter->jumpMomentumX < 0)
			{
				theCharacter->jumpMomentumX = -0.1;
			}
			
			if (theCharacter->jumpMomentumX > 0)
			{
				theCharacter->jumpMomentumX = 0.1;
			}
	
			theCharacter->dashDisabling = true;
		}

		if (theCharacter->momentum > 1)
		{
			theCharacter->momentum -= 0.05;
		}

		if ((gTimer.TotalTime() - time4) >= 5.00f)
		{
			theCharacter->UpdateDivision(theBinaryTree->pixelsPerdivision);
			time4 = gTimer.TotalTime();
		}
		theCollision.TestCollision(theBinaryTree->testPlatforms->at(theCharacter->getDivision()));
		XMFLOAT2 tempPickUpValue;
		//tempPickUpValue = theCollision.TestCollision(theBinaryTree->collectables->at(theCharacter->getDivision()));

		//theCollision.TestCollision(theBinaryTree->testPlatforms->at(theCharacter->getDivision()+1));
		//theCollision.TestCollision(theBinaryTree->testPlatforms->at(theCharacter->getDivision()-1));

		if (input == 1 && theCollision.leftValid() == true)
		{
			if (theCharacter->jumpMomentumState)
			{
				if (theCharacter->jumpMomentumX > theCharacter->getSpeed() * -1)
				{
					theCharacter->jumpMomentumX -= 0.005;
				}

				else
				{
					//theCharacter->jumpMomentumX = theCharacter->getSpeed() * -1;
				}
				
			}
			else
			{
				this->theCharacter->Move(false); //left
				rightDirection = false;
			}
			
		}

		else if (input == 2 && theCollision.rightValid() == true)
		{
			if (theCharacter->jumpMomentumState)
			{
				if (theCharacter->jumpMomentumX < theCharacter->getSpeed())
				{
					theCharacter->jumpMomentumX += 0.005;
				}

				else
				{
					//theCharacter->jumpMomentumX = theCharacter->getSpeed();
				}
				
			}
			else
			{
			
				this->theCharacter->Move(true); //right
				rightDirection = true;
			}
			

		}

		else
			this->theCharacter->momentum = 0;

		if (input == 3)
		{
			reset();
		}

		if (dash && gTimer.TotalTime() - theCharacter->dashTimer > 0.30f)
		{
			theCharacter->Dash();
			theCharacter->dashTimer = gTimer.TotalTime();
		}

		

		if (jump && theCollision.isGrounded() == true && theCharacter->jumpMomentumState == false) //om grounded och man har klickat in jump
		{
			this->thePhysics.Jump(theCollision, theCharacter);
			thePhysics.onPlatform = false;
			theCharacter->setJumpMomentum(rightDirection);
			soundJump.PlayMp3();
			soundJump.daCapo();
		}


		if (theCollision.upValid() == false){
			thePhysics.DisableUpForce();
		}

		thePhysics.Gravitation(theCollision, theCharacter);
		theCharacter->UpdatePosition(theCollision.rightValid(), theCollision.leftValid());
		theCharacter->CalculateWorld();

		//förflyttar alla nonstatic objekt längs deras intervalbana (sin)

		for (int i = 0; i < theBinaryTree->testPlatforms->at(theCharacter->getDivision()).size(); i++){
			if (theBinaryTree->testPlatforms->at(theCharacter->getDivision())[i].GetStatic() == false)
				theBinaryTree->testPlatforms->at(theCharacter->getDivision())[i].PatrolInterval(gTimer.TotalTime());
		}

		lightProp01.lights[1].Type = l_Directional;
		lightProp01.lights[1].Direction = XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
		lightProp01.lights[1].Color = XMFLOAT4(0.1f, 0.1f,0.1f, 1.0f);


		lightProp01.lights[0].Type = l_Point;
		lightProp01.lights[0].Position = XMFLOAT4(0.0f, 0.2f, -5.0f, 1.0f);
		lightProp01.lights[0].Color = XMFLOAT4(Colors::WhiteSmoke);
		lightProp01.lights[0].AttConst = 1.2f;
		lightProp01.lights[0].AttLinear = 0.3f;
		lightProp01.lights[0].AttQuadratic = 0.0f;
		lightProp01.lights[0].Range = 10.0f;

		float moveL = 0.01f;

		if (lightOffsetTest <= 1.0f);
		{
			lightOffsetTest += moveL;
			lightProp01.lights[2].Position = XMFLOAT4(3.0f + lightOffsetTest, -3.0f, 0.0f, 1.0f);
		}
		


		lightProp01.lights[2].Type = l_Point;
		
		lightProp01.lights[2].Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		lightProp01.lights[2].AttConst = 0.3f;
		lightProp01.lights[2].AttLinear = 0.2f;
		lightProp01.lights[2].AttQuadratic = 0.5f;
		lightProp01.lights[2].Range = 15.0f;

		lightProp01.lights[3].Type = l_Point;
		lightProp01.lights[3].Position = XMFLOAT4(20.0f, 10.0f, 0.0f, 1.0f);
		lightProp01.lights[3].Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		lightProp01.lights[3].AttConst = 0.7f;
		lightProp01.lights[3].AttLinear = 0.2f;
		lightProp01.lights[3].AttQuadratic = 0.1f;
		lightProp01.lights[3].Range = 15.0f;


		lightProp01.lights[0].Active = 1;
		lightProp01.lights[1].Active = 0;
		lightProp01.lights[2].Active = 1;
		lightProp01.GlobalAmbient = XMFLOAT4(Colors::Black);

		
	
}

// REALESE AND CLEANUP

void RenderEngine::Release(){

	gDevice->Release();
	depthStencilBuffer->Release();

	gVertexBuffer->Release();
	gVertexLayout->Release();
	gVertexShader->Release();
	gPixelShader->Release();
	gDeviceContext->Release();

	//Kill Lights
	//delete testLight;
}

void RenderEngine::ImportObj(char* geometryFileName, char* materialFileName, ID3D11Device* gDev, int type, bool isStatic){// , bool isStatic, XMMATRIX startPosMatrix){
	static int gameObjectIndex = 0;
	OBJ objectTest(gDev);
	//Load obj
	objectTest.LoadObject(geometryFileName, materialFileName);

	//Test if filename is correct
	OutputDebugStringA(geometryFileName);
	OutputDebugStringA("\n");
	OutputDebugStringA(materialFileName);
	OutputDebugStringA("\n");
	if (type == 0)
	{

		theCharacter = new PlayerObject(*objectTest.GetVertexBuffer(), XMFLOAT3(4, 9,0), true, false, BoundingBox(XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1)), 0, 0, 0.1f, 0.6f);


		theCharacter->CreateBBOXVertexBuffer(gDevice);
		theCharacter->nrElements = objectTest.GetNrElements();
		Collision tempC(theCharacter);
		theCollision = tempC;
		//gameObjects.push_back(*theCharacter);
	}

	else if (type == 1)
	{
		if (isStatic == false){
			Platform testPlatform(false, *objectTest.GetVertexBuffer(), XMFLOAT3(0, 0, 0), true, false, *objectTest.theBoundingBox, 1, 1, 1, 1);
			testPlatform.CreateBBOXVertexBuffer(gDevice);
			testPlatform.nrElements = objectTest.GetNrElements();
			
			theBinaryTree->AddPlatform(testPlatform);
		}
		else{
			Platform testPlatform(false, *objectTest.GetVertexBuffer(), XMFLOAT3(0, 0, 0), true, true, *objectTest.theBoundingBox, 0, 0, 0, 0);
			testPlatform.CreateBBOXVertexBuffer(gDevice);
			testPlatform.nrElements = objectTest.GetNrElements();
			theBinaryTree->AddPlatform(testPlatform);
		}
	}

	else
	{
		GameObject object(*objectTest.GetVertexBuffer(), XMFLOAT3(0, 0, 0), true, true, 0, 0, 0, 0);
		object.nrElements = objectTest.GetNrElements();
		theBinaryTree->AddObject(object);
	}

	//testObject.ObjName = objectTest.GetName();
	//Fill buffers
	//testObject.gameObjectIndex = gameObjectIndex; //används förtillfället vid frustum contains checken
	//gameObjectIndex++;
	//testObject.gVertexBuffer = *objectTest.GetVertexBuffer();


	//testObject.verteciesPos = objectTest.GetVertexPositions();
	//testObject.verteciesIndex = objectTest.GetIndecies();
	//testObject.verteciesPosX = objectTest.GetVerticiesX();
	//testObject.verteciesPosY = objectTest.GetVerticiesY();
	//testObject.verteciesPosZ = objectTest.GetVerticiesZ();




}

void RenderEngine::reset()
{
	theCharacter->xPos = 4;
	theCharacter->yPos = 9;
	theCharacter->Translate(0, 0, 0);
	theCharacter->setDivision(0);
	theCharacter->momentum = 0;
	theCharacter->jumpMomentumX = 0;
	mainCamera.setCameraXPos(theCharacter->xPos);
	mainCamera.setCameraYPos(theCharacter->yPos);
	gCounter.theAge.years = 0;
	gCounter.theAge.months = 0;
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < theBinaryTree->renderObjects->at(i).size(); j++)
		{
			theBinaryTree->renderObjects->at(i).at(j).SetActive(true);
		}

		for (int j = 0; j < theBinaryTree->testPlatforms->at(i).size(); j++)
		{
			theBinaryTree->testPlatforms->at(i).at(j).SetActive(true);
		}

	}
}

void RenderEngine::LoadSounds()
{
	soundBackground.InitMp3();
	soundBackground.LoadMp3("WalkOfLife.mp3");
	soundJump.InitMp3();
	soundJump.LoadMp3("boing.wav");
	
	
}
