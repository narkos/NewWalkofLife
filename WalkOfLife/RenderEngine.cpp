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
	this->theBinaryTree = new BinaryTree(30, 30);
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
	Collision tempC(theCharacter1);
	theCollision = &tempC;
	testStaticPlatforms = tempC;
	testDynamicPlatforms = tempC;
	//theCustomImporter.ImportFBX(gDevice, "Objects/121.bin");
	theCustomImporter.ImportFBX(gDevice, "Objects/testFile.bin");
	//theCharacters = &theCustomImporter.GetPlayers()[0];
	//theCharacter2 = theCustomImporter.GetPlayers[1];
	//theCharacter3 = theCustomImporter.GetPlayers[2];
	//theCustomImporter.GetPlayers()[0]
	for (int i = 0; i < theCustomImporter.GetStaticPlatforms().size(); i++)
	{
		theBinaryTree->AddPlatform(theCustomImporter.GetStaticPlatforms().at(i));
	}
	for (int i = 0; i < theCustomImporter.GetBackGroundObjects().size(); i++)
	{
		theBinaryTree->AddObject(theCustomImporter.GetBackGroundObjects().at(i));
	}
	for (int i = 0; i < theCustomImporter.GetDynamicPlatforms().size(); i++)
	{
		theBinaryTree->addMovingPlatforms(theCustomImporter.GetDynamicPlatforms().at(i));
	}
	for (int i = 0; i < theCustomImporter.GetDynamicCollectableObject().size(); i++)
	{
		theBinaryTree->addMovingCollectable(theCustomImporter.GetDynamicCollectableObject().at(i));
	}
	for (int i = 0; i < theCustomImporter.GetDynamicDeadlyObjects().size(); i++)
	{
		theBinaryTree->addDeadlyMovingPlatform(theCustomImporter.GetDynamicDeadlyObjects().at(i));
	}
	for (int i = 0; i < theCustomImporter.GetStaticCollectableObjects().size(); i++)
	{
		theBinaryTree->addCollectables(theCustomImporter.GetStaticCollectableObjects().at(i));
	}
	for (int i = 0; i < theCustomImporter.GetStaticDeadlyObjects().size(); i++)
	{
		theBinaryTree->addDeadlyPlatforms(theCustomImporter.GetStaticDeadlyObjects().at(i));
	}

	for (int i = 0; i < theCustomImporter.GetPlayers().size(); i++)
	{
		theCharacters.push_back(theCustomImporter.GetPlayers().at(i));
		
	}

	TextureFunc();
	mainMenu.CreateTextures(gDevice);

	//Font
	Fonts();
	mainMenu.menuInit(gDeviceContext);
	theCharacters.at(0).setJumpHeight(0.5f);
	theCharacters.at(0).setRunSpeed(0.1f);
	theCharacters.at(0).xPos = 4;
	theCharacters.at(0).yPos = 9;
	theCharacters.at(1).setRunSpeed(0.1f);
	theCharacters.at(1).setJumpHeight(0.6f);
	theCharacters.at(2).setRunSpeed(0.1f);
	theCharacters.at(2).setJumpHeight(0.6f);
	theCharacters.at(0).SetRayOrigins(-0.4f, -1.0f, 0.5f, 5, 0.5f, 0.3f);
	theCharacters.at(1).SetRayOrigins(-0.6f, -1.0f, 0.5f, 5, 0.5f, 0.3f);
	theCharacters.at(2).SetRayOrigins(-0.5f, -1.0f, 0.5f, 5, 0.5f, 0.3f);

	theCharacters.at(0).SetRayRanges(0.5f, 0.5f, 1.0f);
	theCharacters.at(1).SetRayRanges(0.5f, 0.5f, 1.0f);

	//highscore stuff
	//theHighScore.AddScore(5, 2, 13);

	//theHighScore.AddScore(2, 9, 3);
	//theHighScore.AddScore(1, 2, 44);
	////theHighScore.AddScore(1, 2, 1);
	//theHighScore.ReOrganizeLists();
	////Import
	
	//ImportObj("Objects/testPlayer1.obj", "Objects/testPlayer1.mtl", gDevice, 0, false);
	//ImportObj("Objects/testPlayer1.obj", "Objects/testPlayer1.mtl", gDevice, 4, false);
	//ImportObj("Objects/Char_man.obj", "Objects/Char_man.mtl", gDevice, 4, false);
	
	//ImportObj("Objects/mapPart1.obj", "Objects/mapPart1.mtl", gDevice, false);
	//ImportObj("Objects/mapPart2.obj", "Objects/mapPart2.mtl", gDevice, false);

	//ImportObj("Objects/mapPart3.obj", "Objects/mapPart3.mtl", gDevice, 1, true);
	//theBinaryTree->testPlatforms->at(0).at(0).Translate(0.0f, -2000.0f, 0.0f);
	//ImportObj("Objects/mapPart4.obj", "Objects/mapPart4.mtl", gDevice, 1, true);
	//ImportObj("Objects/mapPart5.obj", "Objects/mapPart5.mtl", gDevice, 1, false);
	//ImportObj("Objects/mapPart6.obj", "Objects/mapPart6.mtl", gDevice, 1, true);

//	ImportObj("Objects/mapPart7.obj", "Objects/mapPart7.mtl", gDevice, 1, true);
	//ImportObj("Objects/mapPart7.obj", "Objects/mapPart7.mtl", gDevice, 2);
	int test = 1;
//	ImportObj("Objects/sphrThingy_01.obj", "Objects/sphrThingy_01.mtl", gDevice, 2, true);


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


	// Rotatation And transform Buffer
	D3D11_BUFFER_DESC transformbuffer;
	memset(&transformbuffer, 0, sizeof(transformbuffer));
	transformbuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	transformbuffer.Usage = D3D11_USAGE_DEFAULT;
	transformbuffer.ByteWidth = sizeof(World);

	HRESULT hr1112 = gDevice->CreateBuffer(&transformbuffer, NULL, &gWorld);
	hr1112 = gDevice->CreateBuffer(&transformbuffer, NULL, &cWorld);


	// Light Buffer
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
	texCheck = CreateDDSTextureFromFile(gDevice, L"Textures/Lowpoly_man.dds", nullptr, &ddsTex2);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Meter_org.dds", nullptr, &Meter);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Meter.dds", nullptr, &Meter1);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Meter1.dds", nullptr, &Meter2);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Meter2.dds", nullptr, &Meter3);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Meter3.dds", nullptr, &Meter4);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Meter4.dds", nullptr, &Meter5);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Meter5.dds", nullptr, &Meter6);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Meter6.dds", nullptr, &Meter7);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Meter7.dds", nullptr, &Meter8);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Meter8.dds", nullptr, &Meter9);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Meter9.dds", nullptr, &Meter10);
	DirectX::CreateDDSTextureFromFile(gDevice, L"Textures/Meter10.dds", nullptr, &Meter11);


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
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	gDevice->CreateSamplerState(&samplerDesc, &sampState1);
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
	//D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
	//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	//};
	ShaderTest = gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &gVertexLayout);
	
	//create pixel shader
	ID3DBlob* pPS = nullptr;
	ShaderTest = CompileShader(L"defaultPS.hlsl", "PS_main", "ps_5_0", &pPS);
	ShaderTest = gDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &gPixelShader);

	ID3DBlob* pPS2 = nullptr;
	ShaderTest = CompileShader(L"simplePS.hlsl", "PS_main", "ps_5_0", &pPS2);
	ShaderTest = gDevice->CreatePixelShader(pPS2->GetBufferPointer(), pPS2->GetBufferSize(), nullptr, &gPixelShader2);


	////wireframe
	//D3D11_INPUT_ELEMENT_DESC inputDescPosOnly[] = {
	//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	//};

	//gDevice->CreateInputLayout(inputDescPosOnly, ARRAYSIZE(inputDescPosOnly), pVS->GetBufferPointer(), pVS->GetBufferSize(), &gWireFrameLayout);


	//HRESULT hrWireFrameVS = CompileShader(L"WireFrameVS.hlsl", "main", "vs_5_0", &pVS);
	//gDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &gWireFrameVertexShader);

	//HRESULT hrWireFramePS = CompileShader(L"WireFramePS.hlsl", "main", "ps_5_0", &pPS);
	//gDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &gWireFramePixelShader);


	// Realese shaders
	pVS->Release();
	pPS->Release();
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

			//mainMenu.Meterfunc(gDeviceContext, mainCamera.getWindowWidth(), gSwapChain, gCounter.theAge.years);

			if (gCounter.theAge.years == 1000 && !Character2)
			{
				Character2 = true;
					//CurrChar.switchCharState(theCharacter1->xPos);
				CurrChar.setCharState(1);
				//theCharacter2->TranslateExact(theCharacter1->xPos, theCharacter1->yPos, 0);
				theCharacters.at(1).xPos = theCharacters.at(0).xPos;
				theCharacters.at(1).yPos = theCharacters.at(0).yPos;
				
			}
			
			if (mainMenu.getPause() == FALSE)
			{
				
				if ((gTimer.TotalTime() - time3) >= 0.012f)
				{
					fpscounter();
					
					
					if (CurrChar.getCharSate() == 0)
					{
						Update(0.0f, theCharacters.at(0));
						Render(&theCharacters.at(0));
					}
					else if (CurrChar.getCharSate() == 1)
					{
						//theCollision
						Update(0.0f, theCharacters.at(1));
						Render(&theCharacters.at(1));
					}
					else if (CurrChar.getCharSate() == 2)
					{
						Update(0.0f, theCharacters.at(2));
						Render(&theCharacters.at(2));
					}
					time3 = gTimer.TotalTime();
				}
				
			}
			if (mainMenu.getPause() == TRUE)
			{

				/*	scrolltime = gTimer.TotalTime();
				if (gTimer.TotalTime() >= scrolltime+1.0f)*/
				//if (theHighScore.getHSbool() == FALSE)
				{
					MenuUpdate(0.0f);
					mainMenu.ActiveMenu(gDeviceContext, mainCamera.getWindowWidth(), mainCamera.getWindowHeight(), gSwapChain, theHighScore.getHSbool());
					
				}

				if(theHighScore.getHSbool()==TRUE)
				{
					theHighScore.Highscorespritebatch(gDevice, gDeviceContext, mainCamera.getWindowWidth(), mainCamera.getWindowHeight(), gSwapChain);
				}


			}
			
		}
	}
	return static_cast<int>(msg.wParam);
}


// UPDATE MATRICIES FOR PER OBJECT CONSTANT BUFFER
void RenderEngine::UpdateMatricies(XMMATRIX &worldM, XMMATRIX &viewM, XMMATRIX &projM)
{
	XMStoreFloat4x4(&perObjCBData.WorldSpace, XMMatrixTranspose(worldM));
	XMStoreFloat4x4(&perObjCBData.View, XMMatrixTranspose(viewM));
	XMStoreFloat4x4(&perObjCBData.Projection, XMMatrixTranspose(projM));

	XMStoreFloat4x4(&perObjCBData.InvWorld, XMMatrixTranspose(XMMatrixInverse(nullptr, worldM)));
	XMMATRIX wvp;
	wvp = worldM * viewM * projM;
	XMStoreFloat4x4(&perObjCBData.WVP, XMMatrixTranspose(wvp));

	gDeviceContext->UpdateSubresource(gWorld, 0, NULL, &perObjCBData, 0, 0);
}


// RENDER

void RenderEngine::Render(PlayerObject* theCharacter){
	
	static float rot = 0.00f;
	UINT32 vertexSize = sizeof(float) * 8;
	UINT32 offset = 0;
	rot += 0.01;
	float clearColor[] = { 0.15f,0.6f,1.0f, 0.2f };
	gDeviceContext->OMSetBlendState(0, 0, 0xffffffff);
	gDeviceContext->OMSetRenderTargets(1, &gBackRufferRenderTargetView, gDepthStencilView);
	gDeviceContext->ClearRenderTargetView(gBackRufferRenderTargetView, clearColor);
	gDeviceContext->ClearDepthStencilView(gDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	if (mainMenu.getPause() == TRUE)
	{
		mainMenu.ActiveMenu(gDeviceContext, mainCamera.getWindowWidth(), mainCamera.getWindowHeight(), gSwapChain, theHighScore.getHSbool());
	}
	// Draw Text
	spriteBatch->Begin();

	std::wstring yearCount = std::to_wstring(gCounter.theAge.years);
	std::wstring monthCount = std::to_wstring(gCounter.theAge.months);
	std::wstring xPos = std::to_wstring(theCharacter->xPos);
	std::wstring yPos = std::to_wstring(theCharacter->yPos);
	//std::wstring coins = std::to_wstring(gCounter.getCoin());
	std::wstring coins = std::to_wstring(theCharacter->getDivision());
	std::wstring dass = std::to_wstring(fpsDisplay);
	std::wstring name(L"FPS: ");
	std::wstring year(L"\nYear: ");
	std::wstring month(L"\nMonth: ");
	std::wstring XXX(L"\nxPos: ");
	std::wstring YYY(L"\nyPos: ");
	std::wstring coin(L"\nCoins: ");
	std::wstring nrOfDeaths(L"\nDeaths: ");
	std::wstring superutedass = name + dass + year + yearCount + month + monthCount + XXX + xPos + YYY + yPos + coin + coins;

	std::wstring Gameover(L"\nGAME OVER MOTHERFUCKER!! ");


	const wchar_t* AMAZING_SUPER_UTE_DASS = superutedass.c_str();

	if (gCounter.theAge.years == 100){

		AMAZING_SUPER_UTE_DASS = Gameover.c_str();

	}

	spritefont->DrawString(spriteBatch.get(), AMAZING_SUPER_UTE_DASS, DirectX::SimpleMath::Vector2(0, 10));

	spriteBatch->End();
	mainMenu.Meterfunc(gDeviceContext, mainCamera.getWindowWidth(), gCounter.theAge.years);
	///////////////////////////////////////////

	gDeviceContext->IASetInputLayout(gVertexLayout);
	gDeviceContext->OMSetDepthStencilState(gDepthStencilState, 0);


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


	XMMATRIX WVP;
	


	gDeviceContext->IASetInputLayout(gVertexLayout);
	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gDeviceContext->VSSetShader(gVertexShader, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->PSSetShader(gPixelShader, nullptr, 0);
	gDeviceContext->PSSetSamplers(0, 1, &sampState1);

	matProperties.Material = MatPresets::Lambert;
	//TEST CUSTOM FORMAT
	for each (Platform var in theBinaryTree->testPlatforms->at(theCharacter->getDivision()))
	{
		gDeviceContext->IASetVertexBuffers(0, 1, &var.vertexBuffer, &vertexSize, &offset);
		gDeviceContext->PSSetShaderResources(0, 1, &ddsTex1);
		
		var.CalculateWorld();
		//var.material = MatPresets::Emerald;
		//matProperties.Material = var.material;
		//matProperties.Material.UseTexture = 0;
		gDeviceContext->UpdateSubresource(matConstBuff, 0, nullptr, &matProperties, 0, 0);
		UpdateMatricies(var.world, CamView, CamProjection);
		gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);

		gDeviceContext->Draw(var.nrElements * 3, 0);
	}

	for each (CollectableObject var in theBinaryTree->collectables->at(theCharacter->getDivision()))
	{
		if (var.GetActive())
		{
			
			gDeviceContext->IASetVertexBuffers(0, 1, &var.vertexBuffer, &vertexSize, &offset);
			gDeviceContext->PSSetShaderResources(0, 1, &ddsTex1);

			var.CalculateWorld();
			//var.material = MatPresets::Emerald;
			//matProperties.Material = var.material;
			//matProperties.Material.UseTexture = 0;
			gDeviceContext->UpdateSubresource(matConstBuff, 0, nullptr, &matProperties, 0, 0);
			UpdateMatricies(var.world, CamView, CamProjection);
			gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);

			gDeviceContext->Draw(var.nrElements * 3, 0);
		}
		
	}


	//Render Moving Platforms
	for each (Platform var in theBinaryTree->platformsMoving->at(theCharacter->getDivision()))
	{
		if (var.GetActive())
		{
			//gDeviceContext->PSSetShaderResources(0, 1, &ddsTex1);

			gDeviceContext->IASetVertexBuffers(0, 1, &var.vertexBuffer, &vertexSize, &offset);
			gDeviceContext->PSSetShaderResources(0, 1, &ddsTex1);

			var.CalculateWorld();
			//var.material = MatPresets::Emerald;
			//matProperties.Material = var.material;
			//matProperties.Material.UseTexture = 0;
			gDeviceContext->UpdateSubresource(matConstBuff, 0, nullptr, &matProperties, 0, 0);
			UpdateMatricies(var.world, CamView, CamProjection);
			gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);

			gDeviceContext->Draw(var.nrElements * 3, 0);
		}

	}

	//Render Deadly Moving Platforms
	for each (Platform var in theBinaryTree->deadlyMoving->at(theCharacter->getDivision()))
	{
		if (var.GetActive())
		{
			//gDeviceContext->PSSetShaderResources(0, 1, &ddsTex1);

			gDeviceContext->IASetVertexBuffers(0, 1, &var.vertexBuffer, &vertexSize, &offset);
			gDeviceContext->PSSetShaderResources(0, 1, &ddsTex1);

			var.CalculateWorld();
			//var.material = MatPresets::Emerald;
			//matProperties.Material = var.material;
			//matProperties.Material.UseTexture = 0;
			gDeviceContext->UpdateSubresource(matConstBuff, 0, nullptr, &matProperties, 0, 0);
			UpdateMatricies(var.world, CamView, CamProjection);
			gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);

			gDeviceContext->Draw(var.nrElements * 3, 0);
		}

	}

	//Render Collectables
	for each (CollectableObject var in theBinaryTree->collectables->at(theCharacter->getDivision()+1))
	{
		if (var.GetActive())
		{
			//gDeviceContext->PSSetShaderResources(0, 1, &ddsTex1);

			gDeviceContext->IASetVertexBuffers(0, 1, &var.vertexBuffer, &vertexSize, &offset);
			gDeviceContext->PSSetShaderResources(0, 1, &ddsTex1);

			var.CalculateWorld();
			//var.material = MatPresets::Emerald;
			//matProperties.Material = var.material;
			//matProperties.Material.UseTexture = 0;
			gDeviceContext->UpdateSubresource(matConstBuff, 0, nullptr, &matProperties, 0, 0);
			UpdateMatricies(var.world, CamView, CamProjection);
			gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);

			gDeviceContext->Draw(var.nrElements * 3, 0);
		}

	}

	for each (Platform var in theBinaryTree->deadly->at(theCharacter->getDivision()))
	{
		
		
		gDeviceContext->IASetVertexBuffers(0, 1, &var.vertexBuffer, &vertexSize, &offset);
		gDeviceContext->PSSetShaderResources(0, 1, &ddsTex1);

		var.CalculateWorld();
		//var.material = MatPresets::Emerald;
		//matProperties.Material = var.material;
		//matProperties.Material.UseTexture = 0;
		gDeviceContext->UpdateSubresource(matConstBuff, 0, nullptr, &matProperties, 0, 0);
		UpdateMatricies(var.world, CamView, CamProjection);
		gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);

		gDeviceContext->Draw(var.nrElements * 3, 0);
		

	}

	for each (Platform var in theBinaryTree->deadly->at(theCharacter->getDivision()+1))
	{
		
		
		gDeviceContext->IASetVertexBuffers(0, 1, &var.vertexBuffer, &vertexSize, &offset);
		gDeviceContext->PSSetShaderResources(0, 1, &ddsTex1);

		var.CalculateWorld();
		//var.material = MatPresets::Emerald;
		//matProperties.Material = var.material;
		//matProperties.Material.UseTexture = 0;
		gDeviceContext->UpdateSubresource(matConstBuff, 0, nullptr, &matProperties, 0, 0);
		UpdateMatricies(var.world, CamView, CamProjection);
		gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);

		gDeviceContext->Draw(var.nrElements * 3, 0);
		

	}

	if (theCharacter->getDivision() != 0)
	{
		for each (Platform var in theBinaryTree->deadly->at(theCharacter->getDivision() - 1))
		{	
			gDeviceContext->IASetVertexBuffers(0, 1, &var.vertexBuffer, &vertexSize, &offset);
			gDeviceContext->PSSetShaderResources(0, 1, &ddsTex1);

			var.CalculateWorld();
			//var.material = MatPresets::Emerald;
			//matProperties.Material = var.material;
			//matProperties.Material.UseTexture = 0;
			gDeviceContext->UpdateSubresource(matConstBuff, 0, nullptr, &matProperties, 0, 0);
			UpdateMatricies(var.world, CamView, CamProjection);
			gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);

			gDeviceContext->Draw(var.nrElements * 3, 0);
			
		}
	}
	

	if (theCharacter->getDivision() != 0)
	{
		for each (CollectableObject var in theBinaryTree->collectables->at(theCharacter->getDivision() - 1))
		{
			if (var.GetActive())
			{
				gDeviceContext->IASetVertexBuffers(0, 1, &var.vertexBuffer, &vertexSize, &offset);
				gDeviceContext->PSSetShaderResources(0, 1, &ddsTex1);

				var.CalculateWorld();
				//var.material = MatPresets::Emerald;
				//matProperties.Material = var.material;
				//matProperties.Material.UseTexture = 0;
				gDeviceContext->UpdateSubresource(matConstBuff, 0, nullptr, &matProperties, 0, 0);
				UpdateMatricies(var.world, CamView, CamProjection);
				gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);

				gDeviceContext->Draw(var.nrElements * 3, 0);
			}

		}
	}
	

	for each (Platform var in theBinaryTree->testPlatforms->at(theCharacter->getDivision()+1))
	{
		//gDeviceContext->PSSetShaderResources(0, 1, &ddsTex1);

		gDeviceContext->IASetVertexBuffers(0, 1, &var.vertexBuffer, &vertexSize, &offset);
		gDeviceContext->PSSetShaderResources(0, 1, &ddsTex1);

		var.CalculateWorld();
		//var.material = MatPresets::Emerald;
		//matProperties.Material = var.material;
		//matProperties.Material.UseTexture = 0;
		gDeviceContext->UpdateSubresource(matConstBuff, 0, nullptr, &matProperties, 0, 0);
		UpdateMatricies(var.world, CamView, CamProjection);
		gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);

		gDeviceContext->Draw(var.nrElements * 3, 0);
	}
	if (theCharacter->getDivision() != 0)
	{
		for each (Platform var in theBinaryTree->testPlatforms->at(theCharacter->getDivision() - 1))
		{
			//gDeviceContext->PSSetShaderResources(0, 1, &ddsTex1);

			gDeviceContext->IASetVertexBuffers(0, 1, &var.vertexBuffer, &vertexSize, &offset);
			gDeviceContext->PSSetShaderResources(0, 1, &ddsTex1);

			var.CalculateWorld();
			//var.material = MatPresets::Emerald;
			//matProperties.Material = var.material;
			//matProperties.Material.UseTexture = 0;
			gDeviceContext->UpdateSubresource(matConstBuff, 0, nullptr, &matProperties, 0, 0);
			UpdateMatricies(var.world, CamView, CamProjection);
			gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);

			gDeviceContext->Draw(var.nrElements * 3, 0);
		}
	}
	
	
for (int i = 0; i < theBinaryTree->renderObjects->at(theCharacter->getDivision()).size(); i++)
	{
		//gDeviceContext->PSSetShaderResources(0, 1, &ddsTex1);

		gDeviceContext->IASetVertexBuffers(0, 1, &theBinaryTree->renderObjects->at(theCharacter->getDivision())[i].vertexBuffer, &vertexSize, &offset);


		theBinaryTree->renderObjects->at(theCharacter->getDivision())[i].CalculateWorld();
		theBinaryTree->renderObjects->at(theCharacter->getDivision())[i].material = MatPresets::Emerald;
		theBinaryTree->renderObjects->at(theCharacter->getDivision())[i].material.SpecPow = 38.0f;

		matProperties.Material = theBinaryTree->renderObjects->at(theCharacter->getDivision())[i].material;

		gDeviceContext->UpdateSubresource(matConstBuff, 0, nullptr, &matProperties, 0, 0);

		UpdateMatricies(theBinaryTree->renderObjects->at(theCharacter->getDivision())[i].world, CamView, CamProjection);


		gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);

		gDeviceContext->Draw(theBinaryTree->renderObjects->at(theCharacter->getDivision())[i].nrElements * 3, 0);
	}

for (int i = 0; i < theBinaryTree->renderObjects->at(theCharacter->getDivision()+1).size(); i++)
{

	gDeviceContext->IASetVertexBuffers(0, 1, &theBinaryTree->renderObjects->at(theCharacter->getDivision() + 1)[i].vertexBuffer, &vertexSize, &offset);


	theBinaryTree->renderObjects->at(theCharacter->getDivision() + 1)[i].CalculateWorld();
	theBinaryTree->renderObjects->at(theCharacter->getDivision() + 1)[i].material = MatPresets::Emerald;
	theBinaryTree->renderObjects->at(theCharacter->getDivision() + 1)[i].material.SpecPow = 38.0f;

	matProperties.Material = theBinaryTree->renderObjects->at(theCharacter->getDivision() + 1)[i].material;

	gDeviceContext->UpdateSubresource(matConstBuff, 0, nullptr, &matProperties, 0, 0);

	UpdateMatricies(theBinaryTree->renderObjects->at(theCharacter->getDivision() + 1)[i].world, CamView, CamProjection);


	gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);

	gDeviceContext->Draw(theBinaryTree->renderObjects->at(theCharacter->getDivision() + 1)[i].nrElements * 3, 0);


	//gDeviceContext->PSSetShaderResources(0, 1, &ddsTex1);

}

if (theCharacter->getDivision() != 0)
{
	for (int i = 0; i < theBinaryTree->renderObjects->at(theCharacter->getDivision() - 1).size(); i++)
	{
		//gDeviceContext->PSSetShaderResources(0, 1, &ddsTex1);

		gDeviceContext->IASetVertexBuffers(0, 1, &theBinaryTree->renderObjects->at(theCharacter->getDivision() - 1)[i].vertexBuffer, &vertexSize, &offset);


		theBinaryTree->renderObjects->at(theCharacter->getDivision() - 1)[i].CalculateWorld();
		theBinaryTree->renderObjects->at(theCharacter->getDivision() - 1)[i].material = MatPresets::Emerald;
		theBinaryTree->renderObjects->at(theCharacter->getDivision() - 1)[i].material.SpecPow = 38.0f;

		matProperties.Material = theBinaryTree->renderObjects->at(theCharacter->getDivision() - 1)[i].material;

		gDeviceContext->UpdateSubresource(matConstBuff, 0, nullptr, &matProperties, 0, 0);

		UpdateMatricies(theBinaryTree->renderObjects->at(theCharacter->getDivision() - 1)[i].world, CamView, CamProjection);


		gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);

		gDeviceContext->Draw(theBinaryTree->renderObjects->at(theCharacter->getDivision() - 1)[i].nrElements * 3, 0);
	}
}

for (int i = 0; i < theBinaryTree->renderObjects->at(theCharacter->getDivision() + 2).size(); i++)
{
	//gDeviceContext->PSSetShaderResources(0, 1, &ddsTex1);

	gDeviceContext->IASetVertexBuffers(0, 1, &theBinaryTree->renderObjects->at(theCharacter->getDivision() + 2)[i].vertexBuffer, &vertexSize, &offset);


	theBinaryTree->renderObjects->at(theCharacter->getDivision() + 2)[i].CalculateWorld();
	theBinaryTree->renderObjects->at(theCharacter->getDivision() + 2)[i].material = MatPresets::Emerald;
	theBinaryTree->renderObjects->at(theCharacter->getDivision() + 2)[i].material.SpecPow = 38.0f;

	matProperties.Material = theBinaryTree->renderObjects->at(theCharacter->getDivision() + 2)[i].material;

	gDeviceContext->UpdateSubresource(matConstBuff, 0, nullptr, &matProperties, 0, 0);

	UpdateMatricies(theBinaryTree->renderObjects->at(theCharacter->getDivision() + 2)[i].world, CamView, CamProjection);


	gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);

	gDeviceContext->Draw(theBinaryTree->renderObjects->at(theCharacter->getDivision() + 2)[i].nrElements * 3, 0);
}

for (int i = 0; i < theBinaryTree->renderObjects->at(theCharacter->getDivision() + 3).size(); i++)
{
	//gDeviceContext->PSSetShaderResources(0, 1, &ddsTex1);

	gDeviceContext->IASetVertexBuffers(0, 1, &theBinaryTree->renderObjects->at(theCharacter->getDivision() + 3)[i].vertexBuffer, &vertexSize, &offset);


	theBinaryTree->renderObjects->at(theCharacter->getDivision() + 3)[i].CalculateWorld();
	theBinaryTree->renderObjects->at(theCharacter->getDivision() + 3)[i].material = MatPresets::Emerald;
	theBinaryTree->renderObjects->at(theCharacter->getDivision() + 3)[i].material.SpecPow = 38.0f;

	matProperties.Material = theBinaryTree->renderObjects->at(theCharacter->getDivision() + 3)[i].material;

	gDeviceContext->UpdateSubresource(matConstBuff, 0, nullptr, &matProperties, 0, 0);

	UpdateMatricies(theBinaryTree->renderObjects->at(theCharacter->getDivision() + 3)[i].world, CamView, CamProjection);


	gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);

	gDeviceContext->Draw(theBinaryTree->renderObjects->at(theCharacter->getDivision() + 3)[i].nrElements * 3, 0);
}

if (theCharacter->getDivision() >1)
{
	for (int i = 0; i < theBinaryTree->renderObjects->at(theCharacter->getDivision() - 2).size(); i++)
	{
		//gDeviceContext->PSSetShaderResources(0, 1, &ddsTex1);

		gDeviceContext->IASetVertexBuffers(0, 1, &theBinaryTree->renderObjects->at(theCharacter->getDivision() - 2)[i].vertexBuffer, &vertexSize, &offset);


		theBinaryTree->renderObjects->at(theCharacter->getDivision() - 2)[i].CalculateWorld();
		theBinaryTree->renderObjects->at(theCharacter->getDivision() - 2)[i].material = MatPresets::Emerald;
		theBinaryTree->renderObjects->at(theCharacter->getDivision() - 2)[i].material.SpecPow = 38.0f;

		matProperties.Material = theBinaryTree->renderObjects->at(theCharacter->getDivision() - 2)[i].material;

		gDeviceContext->UpdateSubresource(matConstBuff, 0, nullptr, &matProperties, 0, 0);

		UpdateMatricies(theBinaryTree->renderObjects->at(theCharacter->getDivision() - 2)[i].world, CamView, CamProjection);


		gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);

		gDeviceContext->Draw(theBinaryTree->renderObjects->at(theCharacter->getDivision() - 2)[i].nrElements * 3, 0);
	}
}

if (theCharacter->getDivision() > 2)
{
	for (int i = 0; i < theBinaryTree->renderObjects->at(theCharacter->getDivision() - 3).size(); i++)
	{
		//gDeviceContext->PSSetShaderResources(0, 1, &ddsTex1);

		gDeviceContext->IASetVertexBuffers(0, 1, &theBinaryTree->renderObjects->at(theCharacter->getDivision() - 3)[i].vertexBuffer, &vertexSize, &offset);


		theBinaryTree->renderObjects->at(theCharacter->getDivision() - 3)[i].CalculateWorld();
		theBinaryTree->renderObjects->at(theCharacter->getDivision() - 3)[i].material = MatPresets::Emerald;
		theBinaryTree->renderObjects->at(theCharacter->getDivision() - 3)[i].material.SpecPow = 38.0f;

		matProperties.Material = theBinaryTree->renderObjects->at(theCharacter->getDivision() - 3)[i].material;

		gDeviceContext->UpdateSubresource(matConstBuff, 0, nullptr, &matProperties, 0, 0);

		UpdateMatricies(theBinaryTree->renderObjects->at(theCharacter->getDivision() - 3)[i].world, CamView, CamProjection);


		gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);

		gDeviceContext->Draw(theBinaryTree->renderObjects->at(theCharacter->getDivision() - 3)[i].nrElements * 3, 0);
	}
}

for (int i = 0; i < theBinaryTree->renderObjects->at(theCharacter->getDivision() + 4).size(); i++)
{
	//gDeviceContext->PSSetShaderResources(0, 1, &ddsTex1);

	gDeviceContext->IASetVertexBuffers(0, 1, &theBinaryTree->renderObjects->at(theCharacter->getDivision() + 4)[i].vertexBuffer, &vertexSize, &offset);


	theBinaryTree->renderObjects->at(theCharacter->getDivision() + 4)[i].CalculateWorld();
	theBinaryTree->renderObjects->at(theCharacter->getDivision() + 4)[i].material = MatPresets::Emerald;
	theBinaryTree->renderObjects->at(theCharacter->getDivision() + 4)[i].material.SpecPow = 38.0f;

	matProperties.Material = theBinaryTree->renderObjects->at(theCharacter->getDivision() + 4)[i].material;

	gDeviceContext->UpdateSubresource(matConstBuff, 0, nullptr, &matProperties, 0, 0);

	UpdateMatricies(theBinaryTree->renderObjects->at(theCharacter->getDivision() + 4)[i].world, CamView, CamProjection);


	gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);

	gDeviceContext->Draw(theBinaryTree->renderObjects->at(theCharacter->getDivision() + 4)[i].nrElements * 3, 0);
}

	// Shouldn't need to set shaders again if we don't change them in the above loops.
	// Leaving it meanwhile
	gDeviceContext->PSSetShaderResources(1, 1, &ddsTex2);
	gDeviceContext->IASetVertexBuffers(0, 1, &theCharacter->vertexBuffer, &vertexSize, &offset);
	gDeviceContext->PSSetSamplers(0, 1, &sampState1);
	theCharacter->CalculateWorld();
	
	
	UpdateMatricies(theCharacter->world, CamView, CamProjection);

	gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);

	

	matProperties.Material = MatPresets::Lambert;
	matProperties.Material.UseTexture = 1;
	gDeviceContext->UpdateSubresource(matConstBuff, 0, nullptr, &matProperties, 0, 0);

	gDeviceContext->Draw(theCharacter->nrElements * 3, 0);

	//////////////////////////////////////////////////////////////
	
	gSwapChain->Present(0, 0); //växla back/front buffer
}

// UPDATES

void RenderEngine::Update(float dt, PlayerObject& theCharacter){
	soundBackground.PlayMp3();
		Input theInput;
		theInput.initInput(this->hInstance, hWindow);
		int input = 0;
		bool jump = false;
		bool dash = false;

		input = theInput.detectInput(hWindow);
		jump = theInput.detectJump(hWindow);
		dash = theInput.detectDash(hWindow);

		for (vector<int>::size_type i = 0; i != theBinaryTree->platformsMoving->at(theCharacter.getDivision()).size(); i++)
		{
			if (theBinaryTree->platformsMoving->at(theCharacter.getDivision())[i].GetStatic() == false)
			{
				theBinaryTree->platformsMoving->at(theCharacter.getDivision())[i].PatrolInterval(gTimer.TotalTime());
				theBinaryTree->platformsMoving->at(theCharacter.getDivision())[i].UpdateBBOX();
			}
			/*if (theCharacter.xPos >= theBinaryTree->testPlatforms->at(theCharacter.getDivision())[i].xPos - (theBinaryTree->testPlatforms->at(theCharacter.getDivision())[i].GetXInterval() - 3.0f)
			&& theCharacter.xPos <= theBinaryTree->testPlatforms->at(theCharacter.getDivision())[i].xPos + (theBinaryTree->testPlatforms->at(theCharacter.getDivision())[i].GetXInterval() + 3.0f))
			*/
			//theBinaryTree->testPlatforms->at(theCharacter.getDivision())[i].SlamaJamma(gTimer.TotalTime());

			//MOVING PLATFORM CALL ** DO NOT REMOVE ** ONLY COMMENTED FOR SLAM TESTING PURPOSES

		}


		if (theCharacter.getDivision() != 0)
		{
			testStaticPlatforms.TestCollision(theBinaryTree->testPlatforms->at(theCharacter.getDivision()), theBinaryTree->testPlatforms->at(theCharacter.getDivision() + 1), theBinaryTree->testPlatforms->at(theCharacter.getDivision() - 1), theCharacter);
			testDynamicPlatforms.TestCollision(theBinaryTree->platformsMoving->at(theCharacter.getDivision()), theBinaryTree->platformsMoving->at(theCharacter.getDivision() + 1), theBinaryTree->platformsMoving->at(theCharacter.getDivision() - 1), theCharacter);
		}

		else
		{
			testStaticPlatforms.TestCollision(theBinaryTree->testPlatforms->at(theCharacter.getDivision()), theBinaryTree->testPlatforms->at(theCharacter.getDivision() + 1), theBinaryTree->testPlatforms->at(theCharacter.getDivision()), theCharacter);
			testDynamicPlatforms.TestCollision(theBinaryTree->platformsMoving->at(theCharacter.getDivision()), theBinaryTree->platformsMoving->at(theCharacter.getDivision() + 1), theBinaryTree->platformsMoving->at(theCharacter.getDivision()), theCharacter);

		}

		if (testDynamicPlatforms.isGrounded() == true || testStaticPlatforms.isGrounded() == true)
			theCollision->SetGrounded(true);
		else
			theCollision->SetGrounded(false);

		if (testDynamicPlatforms.rightValid() == false || testStaticPlatforms.rightValid() == false)
			theCollision->SetRightValid(false);
		else 
			theCollision->SetRightValid(true);

		if (testDynamicPlatforms.leftValid() == false || testStaticPlatforms.leftValid() == false)
			theCollision->SetLeftValid(false);
		else
			theCollision->SetLeftValid(true);

		if (testDynamicPlatforms.upValid() == false || testStaticPlatforms.upValid() == false)
			theCollision->SetUpValid(false);
		else
			theCollision->SetUpValid(true);
		
		//theCollision->TestCollision(theBinaryTree->testPlatforms->at(theCharacter.getDivision()), theBinaryTree->testPlatforms->at(theCharacter.getDivision()+1), theBinaryTree->testPlatforms->at(theCharacter.getDivision()), theCharacter);


		if (!theCollision->rightValid() && theCharacter.jumpMomentumX > 0)
		{
			theCharacter.jumpMomentumX = 0;
		}

		if (!theCollision->leftValid() && theCharacter.jumpMomentumX < 0)
		{
			theCharacter.jumpMomentumX = 0;
		}

		if (!theCollision->isGrounded() && !theCharacter.jumpMomentumState)
		{
			theCharacter.setJumpMomentum(rightDirection);
		}

		if (gTimer.TotalTime() - theCharacter.dashTimer > 0.30f && !theCharacter.dashDisabling)
		{
			theCharacter.momentum = 1;
			if (theCharacter.jumpMomentumX < 0)
			{
				theCharacter.jumpMomentumX = -0.1f;
			}
			
			if (theCharacter.jumpMomentumX > 0)
			{
				theCharacter.jumpMomentumX = 0.1f;
			}
	
			theCharacter.dashDisabling = true;
		}

		if (gTimer.TotalTime() - theCharacter.dashTimer > 2.00f && !theCharacter.dashAvailable)
		{
			theCharacter.dashAvailable = true;
		}

	//	if ((gTimer.TotalTime() - time4) >= 1.00f)
		//{
		theCharacter.UpdateDivision(theBinaryTree->pixelsPerdivision);
			//time4 = gTimer.TotalTime();
		//}

		
		if (theCollision->TestCollisionDeadly(theBinaryTree->deadly->at(theCharacter.getDivision()), &theCharacter))
		{
			reset(&theCharacter);
		}
		////theCollision.TestCollision(theCustomImporter.GetStaticPlatforms()); //vi ska använda dem från customformatet men samtidigt får joel mecka så att culling fungerar med dem!

		
		//theCollision.TestCollision(theCustomImporter.GetStaticPlatforms()); //vi ska använda dem från customformatet men samtidigt får joel mecka så att culling fungerar med dem!

	
		
		XMFLOAT2 tempPickUpValue;
		tempPickUpValue = theCollision->TestCollision(theBinaryTree->collectables->at(theCharacter.getDivision()), &theCharacter);
			gCounter.addCollectable(tempPickUpValue); 

		if (input == 1 && theCollision->leftValid() == true)
		{
			rightDirection = false;
			if (theCharacter.jumpMomentumState)
			{
				if (theCharacter.jumpMomentumX > theCharacter.getSpeed() * -1)
				{
					theCharacter.jumpMomentumX -= 0.005;
				}

				else
				{
					//theCharacter->jumpMomentumX = theCharacter->getSpeed() * -1;
				}
				
			}
			else
			{
				theCharacter.Move(false); //left
				
			}
			
		}

		else if (input == 2 && theCollision->rightValid() == true)
		{
			rightDirection = true;
			if (theCharacter.jumpMomentumState)
			{
				if (theCharacter.jumpMomentumX < theCharacter.getSpeed())
				{
					theCharacter.jumpMomentumX += 0.005;
				}

				else
				{
					//theCharacter->jumpMomentumX = theCharacter->getSpeed();
				}
				
			}
			else
			{
			
				theCharacter.Move(true); //right
				
			}
			

		}

		else
			theCharacter.momentum = 0;

		if (input == 3)
		{
			//reset();
			pausYearCount = gCounter.theAge.years;
			pausMonthCount = gCounter.theAge.months;
			pauseTime = gTimer.TotalTime();
			menuTime = gTimer.TotalTime();
			gTimer.Stop();

			//pauseTime = 100;

			mainMenu.setPause(TRUE);
		}
		if (input == 4)
		{
			reset(&theCharacter);
		}
		if (input == 5)
		{
			theCharacters.at(0).xPos = theCharacter.xPos;
			theCharacters.at(0).yPos = theCharacter.yPos;
			CurrChar.setCharState(0);
			
		}
		if (input == 6)
		{
			theCharacters.at(1).xPos = theCharacter.xPos;
			theCharacters.at(1).yPos = theCharacter.yPos;
			CurrChar.setCharState(1);
			
		}
		if (input == 7)
		{
			theCharacters.at(2).xPos = theCharacter.xPos;
			theCharacters.at(2).yPos = theCharacter.yPos;
			CurrChar.setCharState(2);
			
		}

		if (dash && theCharacter.dashAvailable)
		{
			theCharacter.Dash();
			theCharacter.dashTimer = gTimer.TotalTime();
		}

		
		if (jump && theCollision->isGrounded() == true && theCharacter.jumpMomentumState == false && gTimer.TotalTime() - theCharacter.jumpTimer > 0.3) //om grounded och man har klickat in jump

		{
			thePhysics.Jump(theCollision, &theCharacter);
			thePhysics.onPlatform = false;
			soundJump.PlayMp3();
			soundJump.daCapo();
			theCharacter.jumpTimer = gTimer.TotalTime();
		}


		if (theCollision->upValid() == false){
			thePhysics.DisableUpForce();
		}


		//MOVING PLATFORM POSITION UPDATE

		//for (unsigned i = 0; i < theBinaryTree->platformsMoving->at(theCharacter.getDivision()).size(); i++)
	

		// DEADLY MOVING PLATFORM ( SLAMMER ) UPDATE

		for (vector<int>::size_type i = 0; i != theBinaryTree->deadlyMoving->at(theCharacter.getDivision()).size(); i++)
		{
			if (theBinaryTree->deadlyMoving->at(theCharacter.getDivision())[i].GetStatic() == false)
			{
				theBinaryTree->deadlyMoving->at(theCharacter.getDivision())[i].SlamaJamma(gTimer.TotalTime());
			}
			/*if (theCharacter.xPos >= theBinaryTree->testPlatforms->at(theCharacter.getDivision())[i].xPos - (theBinaryTree->testPlatforms->at(theCharacter.getDivision())[i].GetXInterval() - 3.0f)
			&& theCharacter.xPos <= theBinaryTree->testPlatforms->at(theCharacter.getDivision())[i].xPos + (theBinaryTree->testPlatforms->at(theCharacter.getDivision())[i].GetXInterval() + 3.0f))
			*/
			//theBinaryTree->testPlatforms->at(theCharacter.getDivision())[i].SlamaJamma(gTimer.TotalTime());

			//MOVING PLATFORM CALL ** DO NOT REMOVE ** ONLY COMMENTED FOR SLAM TESTING PURPOSES

		}
		thePhysics.Gravitation(theCollision, &theCharacter);
		theCharacter.UpdatePosition(theCollision->rightValid(), theCollision->leftValid());
		theCharacter.CalculateWorld();

		if (rightDirection)
		{
			theCharacter.Rotate(XMVECTOR(XMVectorSet(0, 1, 0, 0)), 0);
		}

		else
		{
			theCharacter.Rotate(XMVECTOR(XMVectorSet(0, 1, 0, 0)), 3.14);
		}

		lightProp01.lights[0].Type = l_Directional;
		lightProp01.lights[0].Direction = XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
		lightProp01.lights[0].Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);


		lightProp01.lights[1].Type = l_Point;
		lightProp01.lights[1].Position = XMFLOAT4(theCharacter.xPos, theCharacter.yPos, 0.0f, 1.0f);
		lightProp01.lights[1].Color = XMFLOAT4(Colors::WhiteSmoke);
		lightProp01.lights[1].AttConst = 0.2f;
		lightProp01.lights[1].AttLinear = 0.3f;
		lightProp01.lights[1].AttQuadratic = 0.5f;
		lightProp01.lights[1].Range = 10.0f;

		float moveL = 0.0f;

		if (lightOffsetTest < 1.0f);
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
		lightProp01.lights[3].Position = XMFLOAT4(20.0f, -1.0f, 0.0f, 1.0f);
		lightProp01.lights[3].Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		lightProp01.lights[3].AttConst = 0.7f;
		lightProp01.lights[3].AttLinear = 0.2f;
		lightProp01.lights[3].AttQuadratic = 0.0f;
		lightProp01.lights[3].Range = 15.0f;


		lightProp01.lights[0].Active = 1;
		lightProp01.lights[1].Active = 1;
		lightProp01.lights[2].Active = 1;
		lightProp01.lights[3].Active = 1;
		lightProp01.GlobalAmbient = XMFLOAT4(Colors::Black);

		
	
}
void RenderEngine::MenuUpdate(float tt){
	Menu Menuinput;
	Menuinput.menuInit(this->hInstance, hWindow);
	int input2 = 0;

	int currentTabTemp = mainMenu.getCurrentTab();
	int scrolltime = gTimer.TotalTime();
	//if (gTimer.TotalTime() >= scrolltime + 1.0f)

	if (gTimer.TotalTime() >= menuTime + 0.14f) // Om det gått 0.14 sec sen ditt senaste knapptryck
	{
		input2 = Menuinput.menuInput(hWindow);

		if (input2 == 2)
		{
			if (iz < 3)
				iz++;
			else
				iz = 1;
			menuTime = gTimer.TotalTime();

		}
		if (input2 == 1)
		{
			if (iz > 1)
				iz--;
			else
				iz = 3;

			menuTime = gTimer.TotalTime();
		}

		if (input2 == 3)
		{
			//run wichever tab i currently selected --------------------------------------- button functions
			if (mainMenu.getCurrentTab() == 1)
			{
				//gTimer.setPausedTime(pauseTime);
				//gTimer.setCurrTime(pauseTime);
				gTimer.Start(pauseTime);
				mainMenu.setPause(FALSE);
				theHighScore.setHSbool(FALSE);
				menuTime = 0;
			}
			else if (mainMenu.getCurrentTab() == 2)
			{
				// open highscores
				if (theHighScore.getHSbool() == FALSE)
				{
					theHighScore.setHSbool(TRUE);
				}
				else if(theHighScore.getHSbool() == TRUE)
				{
					theHighScore.setHSbool(FALSE);

				}
			}
			else if (mainMenu.getCurrentTab() == 3)
			{
				PostMessage(hWindow, WM_QUIT, 0, 0);
			}


		}

		mainMenu.setCurrentTab(iz);


		if (input2 == 4)
		{
			gTimer.Start(pauseTime);

			//gTimer.setCurrTime(pauseTime);
			//__int64 Paous = gTimer.TotalTime() - pauseTime;
			//gTimer.setPausedTime(Paous);
			mainMenu.setPause(FALSE);
			theHighScore.setHSbool(FALSE);
			menuTime = 0;

			//float gTimer.TotalTime() = pauseTime;

		}
	}



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

		theCharacter1 = new PlayerObject(*objectTest.GetVertexBuffer(), XMFLOAT3(10, 9,0), true, false, BoundingBox(XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1)), 0, 0, 0.1f, 0.6f);

		theCharacter1->CreateBBOXVertexBuffer(gDevice);
		theCharacter1->nrElements = objectTest.GetNrElements();
		//gameObjects.push_back(*theCharacter);
	}

	else if (type == 1)
	{
		if (isStatic == false){
			Platform testPlatform(false, *objectTest.GetVertexBuffer(), XMFLOAT3(0, 0, 0), true, false, *objectTest.theBoundingBox, 3, 4.0f, 1.0f, 1.0f);
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
	if (type == 4)
	{
		theCharacter2 = new PlayerObject(*objectTest.GetVertexBuffer(), XMFLOAT3(4, 9, 0), true, false, BoundingBox(XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1)), 0, 0, 0.1f, 0.6f);
		theCharacter2->Scale(0.5f, 0.5f, 0.5f);
		theCharacter2->CreateBBOXVertexBuffer(gDevice);
		theCharacter2->nrElements = objectTest.GetNrElements();
		
		/*Collision tempD(theCharacter2);
		theCollision = &tempD;*/
		//gameObjects.push_back(*theCharacter);
	}
	if (type == 5)
	{
		theCharacter3 = new PlayerObject(*objectTest.GetVertexBuffer(), XMFLOAT3(4, 9, 0), true, false, BoundingBox(XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1)), 0, 0, 0.1f, 0.6f);

		theCharacter3->CreateBBOXVertexBuffer(gDevice);
		theCharacter3->nrElements = objectTest.GetNrElements();
	/*	Collision tempDC(theCharacter3);
		theCollision = &tempDC;*/
		//gameObjects.push_back(*theCharacter);
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

void RenderEngine::reset(PlayerObject* theCharacter)
{
	Character2 = false;
	Character3 = false;
	CurrChar.setCharState(0);
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
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < theBinaryTree->collectables->at(i).size(); j++)
		{
			theBinaryTree->collectables->at(i).at(j).SetActive(true);
		}

	//	/*for (int j = 0; j < theBinaryTree->collectableMoving->at(i).size(); j++)
	//	{
	//		theBinaryTree->testPlatforms->at(i).at(j).SetActive(true);
	//	}*/

	}
}

void RenderEngine::LoadSounds()
{
	soundBackground.InitMp3();
	soundBackground.LoadMp3("WalkOfLife.mp3");
	soundJump.InitMp3();
	soundJump.LoadMp3("boing.wav");
	
	
}
