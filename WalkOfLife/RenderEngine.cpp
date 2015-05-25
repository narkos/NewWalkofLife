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

struct cbPerObject
{
	XMMATRIX  WVP;
	XMMATRIX World;
	XMVECTOR wCamPos;
	BOOL hasNormMap;

};
cbPerObject cbPerObj;

// CONSTRUCTOR

RenderEngine::RenderEngine(HINSTANCE hInstance, std::string name, UINT scrW, UINT scrH){
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	this->hInstance = hInstance;
	applicationName = name;
	screen_Width = (UINT)mainCamera.getWindowWidth();
	screen_Height = (UINT)mainCamera.getWindowHeight();
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
	//skapa billboards!!
	BillboardTextureEffect *temp = new BillboardTextureEffect(gDevice, 21, 2.0f, 2.0f, 0.05f, "ArrowUp", ".png");
	particleEffects.push_back(temp);
	BillboardTextureEffect *temp2 = new BillboardTextureEffect(gDevice, 23, 2.0f, 2.0f, 0.05f, "ArrowDown", ".png");
	particleEffects.push_back(temp2);
	BillboardTextureEffect *temp3 = new BillboardTextureEffect(gDevice, 8, 2.0f, 2.0f, 0.05f, "getCoinSprite_", ".png");
	particleEffects.push_back(temp3);

	//BillboardTextureEffect temp2(gDevice, 5, 4.0f, 4.0f, 0.1f, "SpriteExplosion", ".png");
	////temp2.SetPosMatrix(XMMatrixIdentity()); var den ska renderas!!!!
	//particleEffects.push_back(temp2);
	Collision tempC(theCharacter1);
	theCollision = &tempC;
	testStaticPlatforms = tempC;
	testDynamicPlatforms = tempC;
	//theHighScore.LoadHighScore();
	//theCustomImporter.ImportFBX(gDevice, "Objects/121.bin");
	theCustomImporter.ImportFBX(gDevice, "Objects/testFile.bin");
	intArrayTex = theCustomImporter.GetindexArray();
	statez = CurrChar.getCharSate();

	//D3D11_BUFFER_DESC bDesc;
	//ZeroMemory(&bDesc, sizeof(D3D11_BUFFER_DESC));
	//bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//bDesc.Usage = D3D11_USAGE_DEFAULT;
	//bDesc.ByteWidth = sizeof(VertexData)*(vertecies.size());

	//D3D11_SUBRESOURCE_DATA data;
	//data.pSysMem = vertecies.data();//<--------
	//HRESULT VertexBufferChecker = gDevice->CreateBuffer(&bDesc, &data, &Tempidlebuffer);

	//Tempidlebuffer = theCharacters.at(0).GetVertexBuffer();
	//--
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
	
	StartMenu.CreateTextures(gDevice);
	//Font
	Fonts();
	//StartMenu.menuInit(gDeviceContext);
	//mainMenu.menuInit(gDeviceContext);

	theCharacters.at(0).xPos = 4;
	theCharacters.at(0).yPos = 9;

	theCharacters.at(0).setRunSpeed(0.08f);
	theCharacters.at(0).setJumpHeight(0.66f);
	theCharacters.at(8).setRunSpeed(0.15f);
	theCharacters.at(8).setJumpHeight(0.68f);
	theCharacters.at(16).setRunSpeed(0.05f);
	theCharacters.at(16).setJumpHeight(0.75f);

	// Set hit ray info
	theCharacters.at(0).SetRayOrigins(-0.3f, -1.0f, 0.6f, 3, 0.3f, 0.2f);
	theCharacters.at(8).SetRayOrigins(-0.5f, -1.0f, 1.0f, 5, 0.3f, 0.3f);
	theCharacters.at(16).SetRayOrigins(-0.3f, -1.0f, 1.0f, 5, 0.5f, 0.3f);

	theCharacters.at(0).SetRayRanges(0.35f, 0.5f, 0.3f);
	theCharacters.at(8).SetRayRanges(0.5f, 0.5f, 0.5f);
	theCharacters.at(16).SetRayRanges(0.48f, 0.5f, 0.4f);


	//Baby World Start Pos
	resetXpos[0] = 4.0f;
	resetYpos[0] = 9.0f;

	resetXpos[1] = 200.0f;
	resetYpos[1] = 20.0f;

	resetXpos[2] = 557.0f;
	resetYpos[2] = 20.0f;
	
	

	//SHADOWS
	Shadows tempShadows((int)mainCamera.getWindowWidth(), (int)mainCamera.getWindowHeight(), gDevice, gDeviceContext);
	shadows = tempShadows;
	shadows.createShadowMap();

	D3D11_BUFFER_DESC shadowSettingsDESC;
	memset(&shadowSettingsDESC, 0, sizeof(shadowSettingsDESC));
	shadowSettingsDESC.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	shadowSettingsDESC.Usage = D3D11_USAGE_DEFAULT;
	shadowSettingsDESC.ByteWidth = sizeof(shadowSettings);

	HRESULT hr1212 = gDevice->CreateBuffer(&shadowSettingsDESC, NULL, &shadowBuffer);
	//__________________________________________________________________________________________________________________________________________________________________________________


	mainMenu.menuInit(gDeviceContext);

	//theCharacters.at(0).xPos = 4;
	//theCharacters.at(0).yPos = 9;
	//
	//theCharacters.at(0).setRunSpeed(0.08f);
	//theCharacters.at(0).setJumpHeight(0.66f);
	//theCharacters.at(0).setDash(2.0f);
	//theCharacters.at(8).setRunSpeed(0.13f);
	//theCharacters.at(8).setJumpHeight(0.68f);
	//theCharacters.at(0).setDash(2.7f);
	//theCharacters.at(16).setRunSpeed(0.05f);
	//theCharacters.at(16).setJumpHeight(0.75f);
	//



	//// Set hit ray info
	//theCharacters.at(0).SetRayOrigins(-0.3f, -1.0f, 0.6f, 3, 0.3f, 0.2f);
	//theCharacters.at(8).SetRayOrigins(-0.5f, -1.0f, 1.0f, 5, 0.3f, 0.3f);
	//theCharacters.at(16).SetRayOrigins(-0.3f, -1.0f, 1.0f, 5, 0.5f, 0.3f);

	//theCharacters.at(0).SetRayRanges(0.35f, 0.5f, 0.3f);
	//theCharacters.at(8).SetRayRanges(0.5f, 0.5f, 0.5f);
	//theCharacters.at(16).SetRayRanges(0.48f, 0.5f, 0.4f);


	//Baby World Start Pos
	/*resetXpos[0] = 4.0f;
	resetYpos[0] = 9.0f;

	resetXpos[1] = 200.0f;
	resetYpos[1] = 20.0f;

	resetXpos[2] = 364.0f;
	resetYpos[2] = 20.0f;*/


	

	//highscore stuff
	//theHighScore.AddScore(5, 2, 13);

	//theHighScore.AddScore(2, 9, 3);
	//theHighScore.AddScore(1, 2, 44);
	////theHighScore.AddScore(1, 2, 1);
	//theHighScore.ReOrganizeLists();
	////Import


	// Rotatation And transform Buffer
	D3D11_BUFFER_DESC transformbuffer;
	memset(&transformbuffer, 0, sizeof(transformbuffer));
	transformbuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	transformbuffer.Usage = D3D11_USAGE_DEFAULT;
	transformbuffer.ByteWidth = sizeof(World);

	HRESULT hr1112 = gDevice->CreateBuffer(&transformbuffer, NULL, &gWorld);

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


	////Normalmap buffer
	//D3D11_BUFFER_DESC cbbd;
	//ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	//cbbd.Usage = D3D11_USAGE_DEFAULT;
	//cbbd.ByteWidth = sizeof(cbPerObject);
	//cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//cbbd.CPUAccessFlags = 0;
	//cbbd.MiscFlags = 0;

	//gDevice->CreateBuffer(&cbbd, NULL, &cbPerObjectBuffer);



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
	vp.Width = (float)screen_Width;
	vp.Height = (float)screen_Height;
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
		fpsDisplay = (UINT)fps;
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
std::wstring string2wString(const std::string& s){
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}
// CREATE TEXTURES
#include <AtlBase.h>
#include <atlconv.h>
void RenderEngine::TextureFunc(){

	HRESULT texCheck;
	vector<string> texNames = theCustomImporter.GettexNameArray();
	RSWArray = new ID3D11ShaderResourceView*[texNames.size()];

	std::wstring fest = L"Textures/ObjectTextures/";
	for (int texIt = 0; texIt < texNames.size(); texIt++){

		std::wstring fest2 = fest + string2wString(texNames[texIt]);;
		const wchar_t* texNameS = fest2.c_str(); 
		texCheck = CreateDDSTextureFromFile(gDevice, texNameS, nullptr, &RSWArray[texIt]);
		if (texCheck == E_FAIL){
			CreateWICTextureFromFile(gDevice, texNameS, nullptr, &RSWArray[texIt]);
		}
	}



	//DirectX::CreateWICTextureFromFile(gDevice, L"Textures/normalmap.dds", nullptr, &normalMap);
	
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
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	gDevice->CreateSamplerState(&samplerDesc, &sampState1);
	//DEAFULT SHADERS

	//create vertex shader
	ID3DBlob* pVS = nullptr;
	ID3DBlob* pGS = nullptr;
	ID3DBlob* errorMessage = nullptr;
	ShaderTest = CompileShader(L"defaultVS.hlsl", "VS_main", "vs_5_0", &pVS);
	ShaderTest = gDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &gVertexShader);

	//create input layout (verified using vertex shader)
	//D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
	//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	//};
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
	

	HRESULT hrParVS = CompileShader(L"bbVertexShader.hlsl", "main", "vs_5_0", &pVS);
	gDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &gFakeBillboardVertexShader);

	HRESULT hrParGS = CompileShader(L"bbGeometryShader.hlsl", "main", "gs_5_0", &pGS);
	gDevice->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(), nullptr, &gFakeBillboardGeometryShader);

	HRESULT hParPS = CompileShader(L"bbPixelShader.hlsl", "main", "ps_5_0", &pPS);
	gDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &gFakeBillboardPixelShader);

	D3D11_INPUT_ELEMENT_DESC inputDescParticle[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	gDevice->CreateInputLayout(inputDescParticle, ARRAYSIZE(inputDescParticle), pVS->GetBufferPointer(), pVS->GetBufferSize(), &gFakeBillboardLayout);


	// Realese shaders
	pVS->Release();
	pPS->Release();
	pGS->Release();
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

		//NEEDED?

		//D3D11_DEPTH_STENCIL_DESC dsDesc;
		//ZeroMemory(&dsDesc, sizeof(dsDesc));
		////Depth test settings
		//dsDesc.DepthEnable = true;
		//dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		//dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
		////Stencil tests
		//dsDesc.StencilEnable = true;
		//dsDesc.StencilReadMask = 0xFF;
		//dsDesc.StencilWriteMask = 0xFF;
		////Stencil operations - Pixel Front Facing
		//dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		//dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		//dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		//dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		////Stencil operations - Pixel Back Facing
		//dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		//dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		//dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		//dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		//HRESULT hr3 = gDevice->CreateDepthStencilState(&dsDesc, &gDepthStencilState);
		//gDeviceContext->OMSetDepthStencilState(gDepthStencilState, 0);

		
		
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

			if (StartMenu.getstartmeny() == true)
			{
				
				StartMenuUpdate(0.0f);
				StartMenu.ActivestartMenu(gDeviceContext, mainCamera.getWindowWidth(), mainCamera.getWindowHeight(), gSwapChain, theHighScore.getHSbool());
				if (theHighScore.getHSbool() == true)
				{
					theHighScore.Highscorespritebatch(gDevice, gDeviceContext, mainCamera.getWindowWidth(), mainCamera.getWindowHeight(), gSwapChain);
				}
			//	gDeviceContext->OMSetDepthStencilState(gDepthStencilState, 0);
			}
			//mainMenu.Meterfunc(gDeviceContext, mainCamera.getWindowWidth(), gSwapChain, gCounter.theAge.years);

			else if (StartMenu.getstartmeny() == false)
			{
				theHighScore.setHSbool(false);
				//mainMenu.Meterfunc(gDeviceContext, mainCamera.getWindowWidth(), gSwapChain, gCounter.theAge.years);

				if (gCounter.theAge.years > 32 && Character2 == false)

				{
					Character2 = true;
					//CurrChar.switchCharState(theCharacter1->xPos);
					haschanged = true;
					CurrChar.setCharState(8);
					/*particleEffects[1].SetPosMatrix(theCharacter.pos);
					particleEffects[1].Play();*/
					//theCharacter2->TranslateExact(theCharacter1->xPos, theCharacter1->yPos, 0);

					theCharacters.at(8).xPos = theCharacters.at(0).xPos;
					theCharacters.at(8).yPos = theCharacters.at(0).yPos + 2;
					//theCharacters.at(8).SetDivision(theCharacters.at(0).getDivision());

					soundIGotThis.PlayMp3();
					soundIGotThis.daCapo();

				}

				if (gCounter.theAge.years > 65 && Character3 == false)
				{
					Character3 = true;
					//CurrChar.switchCharState(theCharacter1->xPos);
					haschanged = true;
					CurrChar.setCharState(16);
					//theCharacter2->TranslateExact(theCharacter1->xPos, theCharacter1->yPos, 0);

					theCharacters.at(16).xPos = theCharacters.at(8).xPos;
					theCharacters.at(16).yPos = theCharacters.at(8).yPos;
					//theCharacters.at(16).SetDivision(theCharacters.at(8).getDivision());

					tempInt = rand() % 2;
					if (tempInt == 0)
					{
						soundHelpMe.PlayMp3();
						soundHelpMe.daCapo();
					}
					else
					{
						soundWhereAmI.PlayMp3();
						soundWhereAmI.daCapo();
					}

				}


				if (mainMenu.getPause() == FALSE)
				{

					if ((gTimer.TotalTime() - time3) >= 0.012f)
					{
						fpscounter();

						
					//	switchAnimation(theCharacters.at(CurrChar.getCharSate()), CurrChar.getCharSate());
						


						if (CurrChar.getCharSate() == 0)
						{
							
							Update(0.0f, theCharacters.at(0));
							//switchAnimation(&theCharacters.at(0), 0);
							Render(&theCharacters.at(0));
						}
						else if (CurrChar.getCharSate() == 8)
						{
							//theCollision
							//theCharacters.at(1).setVertexBuffer(theCharacters.at(2).GetVertexBuffer());
							Update(0.0f, theCharacters.at(8));
							//switchAnimation(&theCharacters.at(1), 1);
							Render(&theCharacters.at(8));
						}
						else if (CurrChar.getCharSate() == 16)
						{
							Update(0.0f, theCharacters.at(16));
							//switchAnimation(&theCharacters.at(2), 2);
							Render(&theCharacters.at(16));
						}
						time3 = gTimer.TotalTime();
					}

				}
				if (mainMenu.getPause() == TRUE)
				{

					/*	scrolltime = gTimer.TotalTime();
					if (gTimer.TotalTime() >= scrolltime+1.0f)*/
				//	if (theHighScore.getHSbool() == FALSE)
					{
						if (CurrChar.getCharSate() == 0)
						{
							
							
							MenuUpdate(0.0f, theCharacters.at(0));

							mainMenu.ActiveMenu(gDeviceContext, mainCamera.getWindowWidth(), mainCamera.getWindowHeight(), gSwapChain, theHighScore.getHSbool(), mainMenu.getreplay());
						}
						else if (CurrChar.getCharSate() == 8)
						{
							//theCollision
							MenuUpdate(0.0f, theCharacters.at(8));
							mainMenu.ActiveMenu(gDeviceContext, mainCamera.getWindowWidth(), mainCamera.getWindowHeight(), gSwapChain, theHighScore.getHSbool(), mainMenu.getreplay());
						}
						else if (CurrChar.getCharSate() == 16)
						{
							MenuUpdate(0.0f, theCharacters.at(8));
							
							mainMenu.ActiveMenu(gDeviceContext, mainCamera.getWindowWidth(), mainCamera.getWindowHeight(), gSwapChain, theHighScore.getHSbool(), mainMenu.getreplay());
						}
			

					}

					if (theHighScore.getHSbool() == TRUE)
					{
						theHighScore.Highscorespritebatch(gDevice, gDeviceContext, mainCamera.getWindowWidth(), mainCamera.getWindowHeight(), gSwapChain);
					}


				}

			}
		}
	}

	this->Release();
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


//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//SHADOW TESTING 4 DUMMIES
//0 = Render the scene as usual WITHOUT shadows
//1 = Render the scene as usual WITH shadows
//2 = Render the depth value
//3 = Render shadowed and non shadowed parts in two contrasts (Shadows = Black, Lit parts = White)
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//

int tex = 0;

void RenderEngine::Render(PlayerObject* theCharacter){
	//SHADOW MAPPING-----------////-----------////-----------////-----------////

	shadows.renderSceneToShadowMap(XMMatrixIdentity(), lightProp01.lights[0].Position, mainCamera.getCameraXPos(), lightProp01.lights[0].Direction);	//It's put here in case the light would be moving. Otherwise it could be in the constant buffer for optimization
	WVP = shadows.getLightWVP();	//Transpose the matrices (WVP To lights POV) to prepare them for the shader, For Shadow mapping in this case
	//XMStoreFloat4x4(&perObjCBData.WorldSpace, XMMatrixTranspose(shadows.getShadowWorld()));
	XMStoreFloat4x4(&perObjCBData.WVP, XMMatrixTranspose(WVP));
	XMStoreFloat4x4(&perObjCBData.lightView, XMMatrixTranspose(shadows.getLightView()));
	XMStoreFloat4x4(&perObjCBData.lightProjection, XMMatrixTranspose(shadows.getLightProjection()));

	gDeviceContext->UpdateSubresource(gWorld, 0, NULL, &perObjCBData, 0, 0);
	gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);
	//gDeviceContext->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	//gDeviceContext->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);

	gDeviceContext->IASetInputLayout(gVertexLayout);	//Set the vertex input layout.
	gDeviceContext->VSSetShader(gVertexShader, NULL, 0);	//Set the vertex and pixel shaders that will be used to render

	//RENDER THE SHADOW MAP
	drawScene(1, theCharacter);	//Draws Entire Scene to Shadow map	// 1 = From lights POV. 2 = From mainCameras POV.

	shadows.setShaderResource();	//Set (SHADOW MAP) shader texture resource in the pixel shader.
	//SHADOW MAPPING-----------////-----------////-----------////-----------////

	//static float rot = 0.00f;
	UINT32 vertexSize = sizeof(float)* 8;
	UINT32 offset = 0;
	//rot += 0.01;

	float clearColor[] = { 0.15f, 0.6f, 1.0f, 0.2f };
	gDeviceContext->OMSetBlendState(0, 0, 0xffffffff);
	gDeviceContext->OMSetRenderTargets(1, &gBackRufferRenderTargetView, gDepthStencilView);
	gDeviceContext->ClearRenderTargetView(gBackRufferRenderTargetView, clearColor);
	gDeviceContext->ClearDepthStencilView(gDepthStencilView, D3D11_CLEAR_DEPTH/* | D3D11_CLEAR_STENCIL*/, 1.0f, 0);

	
	// Draw Text
	
	spriteBatch->Begin();

	std::wstring yearCount = std::to_wstring(gCounter.theAge.years);
	std::wstring monthCount = std::to_wstring(gCounter.theAge.months);
	std::wstring xPos = std::to_wstring(theCharacter->xPos);
	std::wstring yPos = std::to_wstring(theCharacter->yPos);
	//std::wstring coins = std::to_wstring(gCounter.getCoin());
	std::wstring coins = std::to_wstring(gCounter.getCoin());
	std::wstring dass = std::to_wstring(fpsDisplay);
	std::wstring name(L"FPS: ");
	std::wstring year(L"\nYear: ");
	std::wstring month(L"\nMonth: ");
	std::wstring XXX(L"\nxPos: ");
	std::wstring YYY(L"\nyPos: ");
	std::wstring coin(L"\nCoins: ");
	std::wstring nrOfDeaths(L"\nDeaths: ");
	std::wstring superutedass = name + dass + year + yearCount + month + monthCount + coin + coins;

	std::wstring Gameover(L"\nGAME OVER MOTHERFUCKER!! ");

	
	const wchar_t* AMAZING_SUPER_UTE_DASS = superutedass.c_str();

	if (gCounter.theAge.years == 100){

		AMAZING_SUPER_UTE_DASS = Gameover.c_str();

	}

	spritefont->DrawString(spriteBatch.get(), AMAZING_SUPER_UTE_DASS, DirectX::SimpleMath::Vector2(0, 10));

	spriteBatch->End();
	//gSwapChain->Present(0, 0); //växla back/front buffer
	mainMenu.Meterfunc(gDeviceContext, mainCamera.getWindowWidth(), gCounter.theAge.years);
	
	///////////////////////////////////////////

	gDeviceContext->IASetInputLayout(gVertexLayout);
	gDeviceContext->OMSetDepthStencilState(gDepthStencilState, 0);


	mainCamera.setPlayerXPos(theCharacter->xPos);
	mainCamera.setPlayerYPos(theCharacter->yPos);

	//WORLD
	//XMMATRIX YRotation = XMMatrixRotationY(rot);

	//The Camera Matrices are now defined in the camera class (mainCamera)

	CamView = mainCamera.getCamView();					//Defined in .h file
	CamProjection = mainCamera.getCamProjection();		//Defined in .h file
	identityM = XMMatrixIdentity();						//Defined in .h file
	WorldInv = XMMatrixInverse(nullptr, XMMatrixIdentity());		//Defined in .h file

	// LIGHT BUFFER UPDATE
	lightProp01.CamPosition = XMFLOAT4(mainCamera.getCameraXPos(), mainCamera.getCameraYPos(), mainCamera.getCameraZPos(), 0.0f);
	gDeviceContext->UpdateSubresource(lightConstBuff, 0, NULL, &lightProp01, 0, 0);
	gDeviceContext->PSSetShader(gPixelShader, nullptr, 0);
	gDeviceContext->PSSetConstantBuffers(2, 1, &lightConstBuff);
	gDeviceContext->PSSetConstantBuffers(1, 1, &matConstBuff);
	// END LIGHT BUFFER UPDATE
	// normalmap buffer
	//gDeviceContext->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	//gDeviceContext->PSSetConstantBuffers(4, 1, &cbPerObjectBuffer);
	//XMMATRIX WVP;		//Defined in .h file

	gDeviceContext->IASetInputLayout(gVertexLayout);
	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gDeviceContext->VSSetShader(gVertexShader, nullptr, 0);
	gDeviceContext->GSSetShader(nullptr, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->PSSetShader(gPixelShader, nullptr, 0);
	gDeviceContext->PSSetSamplers(0, 1, &sampState1);

	drawScene(2, theCharacter);	// 1 = From lights POV. 2 = From mainCameras POV.
	if (mainMenu.getPause() == TRUE)
	{
		mainMenu.ActiveMenu(gDeviceContext, mainCamera.getWindowWidth(), mainCamera.getWindowHeight(), gSwapChain, theHighScore.getHSbool(), mainMenu.getreplay());
	}
	gSwapChain->Present(0, 0); //växla back/front buffer
}

void RenderEngine::drawScene(int viewPoint, PlayerObject* theCharacter)
{
	XMMATRIX currProjection;
	XMMATRIX currView;


	if (viewPoint == 1)
	{
		currProjection = shadows.getLightProjection();
		currView = shadows.getLightView();
	}
	if (viewPoint == 2)
	{
		currProjection = CamProjection;
		currView = CamView;
	}

	shadows.setShaderResource();	//Set (SHADOW MAP) shader texture resource in the pixel shader.

	matProperties.Material = MatPresets::BlinnBase;
	matProperties.Material.UseTexture = 1;
	//TEST CUSTOM FORMAT

	//######################################################################################################################################################
	//###													SHADOW CASTING OBJECTS GOES HERE BELOW							  							 ###	
	//######################################################################################################################################################

	float tempDiv = -1.0f;
	if (theCharacter->getDivision() == 0)
	{
		tempDiv = 0.0f;
	}
	for (int i = theCharacter->getDivision() + (int)tempDiv; i <= theCharacter->getDivision() + 1; i++)
	{
		for (int j = 0; j < theBinaryTree->testPlatforms->at(i).size(); j++)
		{
			if (theBinaryTree->testPlatforms->at(i)[j].GetActive())
			{
				tex = intArrayTex[theBinaryTree->testPlatforms->at(i)[j].indexT];
				gDeviceContext->PSSetShaderResources(0, 1, &RSWArray[tex]);
				gDeviceContext->IASetVertexBuffers(0, 1, &theBinaryTree->testPlatforms->at(i)[j].vertexBuffer, &vertexSize, &offset);

				gDeviceContext->UpdateSubresource(matConstBuff, 0, nullptr, &matProperties, 0, 0);
				UpdateMatricies(theBinaryTree->testPlatforms->at(i)[j].world, currView, currProjection);
				gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);

				gDeviceContext->Draw(theBinaryTree->testPlatforms->at(i)[j].nrElements * 3, 0);
			}
		}
	}

	// Render Collectables
	tempDiv = -1.0f;
	if (theCharacter->getDivision() == 0)
	{
		tempDiv = 0.0f;
	}

	for (int i = theCharacter->getDivision() + (int)tempDiv; i <= theCharacter->getDivision() + 1; i++)

	{
		for (int j = 0; j < theBinaryTree->collectables->at(i).size(); j++)
		{
			if (theBinaryTree->collectables->at(i)[j].GetActive())
			{
				tex = intArrayTex[theBinaryTree->collectables->at(i)[j].indexT];
				gDeviceContext->PSSetShaderResources(0, 1, &RSWArray[tex]);
gDeviceContext->IASetVertexBuffers(0, 1, &theBinaryTree->collectables->at(i)[j].vertexBuffer, &vertexSize, &offset);


/*theBinaryTree->renderObjects->at(i)[j].material = MatPresets::Emerald;
theBinaryTree->renderObjects->at(i)[j].material.SpecPow = 38.0f;*/

//theBinaryTree->collectables->at(i)[j].material;
//theBinaryTree->collectables->at(i)[j].material.UseTexture = 1;
//matProperties.Material = MatPresets::BlinnBase;
//matProperties.Material.Emissive = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
gDeviceContext->UpdateSubresource(matConstBuff, 0, nullptr, &matProperties, 0, 0);
UpdateMatricies(theBinaryTree->collectables->at(i)[j].world, currView, currProjection);
gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);

gDeviceContext->Draw(theBinaryTree->collectables->at(i)[j].nrElements * 3, 0);
			}
		}
	}


	//Render Moving Platforms
	tempDiv = -1.0f;
	if (theCharacter->getDivision() == 0)
	{
		tempDiv = 0.0f;
	}
	for (int i = theCharacter->getDivision() + (int)tempDiv; i <= theCharacter->getDivision() + 1; i++)
	{
		for (int j = 0; j < theBinaryTree->platformsMoving->at(i).size(); j++)
		{
			if (theBinaryTree->platformsMoving->at(i)[j].GetActive())
			{
				tex = intArrayTex[theBinaryTree->platformsMoving->at(i)[j].indexT];
				gDeviceContext->PSSetShaderResources(0, 1, &RSWArray[tex]);
				gDeviceContext->IASetVertexBuffers(0, 1, &theBinaryTree->platformsMoving->at(i)[j].vertexBuffer, &vertexSize, &offset);

				gDeviceContext->UpdateSubresource(matConstBuff, 0, nullptr, &matProperties, 0, 0);
				UpdateMatricies(theBinaryTree->platformsMoving->at(i)[j].world, currView, currProjection);
				gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);

				gDeviceContext->Draw(theBinaryTree->platformsMoving->at(i)[j].nrElements * 3, 0);
			}
		}
	}

	//Render Deadly Moving Platforms

	tempDiv = -1.0f;
	if (theCharacter->getDivision() == 0)
	{
		tempDiv = 0.0f;
	}

	for (int i = theCharacter->getDivision() + (int)tempDiv; i <= theCharacter->getDivision() + 1; i++)
	{
		for (int j = 0; j < theBinaryTree->deadlyMoving->at(i).size(); j++)
		{
			if (theBinaryTree->deadlyMoving->at(i)[j].GetActive())
			{
				tex = intArrayTex[theBinaryTree->deadlyMoving->at(i)[j].indexT];
				gDeviceContext->PSSetShaderResources(0, 1, &RSWArray[tex]);
				gDeviceContext->IASetVertexBuffers(0, 1, &theBinaryTree->deadlyMoving->at(i)[j].vertexBuffer, &vertexSize, &offset);

				gDeviceContext->UpdateSubresource(matConstBuff, 0, nullptr, &matProperties, 0, 0);
				UpdateMatricies(theBinaryTree->deadlyMoving->at(i)[j].world, currView, currProjection);
				gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);

				gDeviceContext->Draw(theBinaryTree->deadlyMoving->at(i)[j].nrElements * 3, 0);
			}
		}
	}

	tempDiv = -1.0f;
	if (theCharacter->getDivision() == 0)
	{
		tempDiv = 0.0f;
	}
	for (int i = theCharacter->getDivision() + (int)tempDiv; i <= theCharacter->getDivision() + 1; i++)
	{
		for (int j = 0; j < theBinaryTree->deadly->at(i).size(); j++)
		{
			if (theBinaryTree->deadly->at(i)[j].GetActive())
			{
				tex = intArrayTex[theBinaryTree->deadly->at(i)[j].indexT];
				gDeviceContext->PSSetShaderResources(0, 1, &RSWArray[tex]);
				gDeviceContext->IASetVertexBuffers(0, 1, &theBinaryTree->deadly->at(i)[j].vertexBuffer, &vertexSize, &offset);

				gDeviceContext->UpdateSubresource(matConstBuff, 0, nullptr, &matProperties, 0, 0);
				UpdateMatricies(theBinaryTree->deadly->at(i)[j].world, currView, currProjection);
				gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);

				gDeviceContext->Draw(theBinaryTree->deadly->at(i)[j].nrElements * 3, 0);
			}
		}
	}

	//######################################################################################################################################################
	//###						*NON* SHADOW CASTING OBJECTS GOES IN IF-STATEMENT HERE BELOW	(if (viewPoint == 2))					  				 ###	
	//######################################################################################################################################################
	if (viewPoint == 2)
		{
			gDeviceContext->PSSetShader(gPixelShader2, nullptr, 0);
			for (int i = 0; i < theBinaryTree->renderObjects->size(); i++)
			{
				for (int j = 0; j < theBinaryTree->renderObjects->at(i).size(); j++)
				{
					tex = intArrayTex[theBinaryTree->renderObjects->at(i)[j].indexT];
					gDeviceContext->PSSetShaderResources(0, 1, &RSWArray[tex]);
					gDeviceContext->IASetVertexBuffers(0, 1, &theBinaryTree->renderObjects->at(i)[j].vertexBuffer, &vertexSize, &offset);


					theBinaryTree->renderObjects->at(i)[j].CalculateWorld();
					/*theBinaryTree->renderObjects->at(i)[j].material = MatPresets::Emerald;
					theBinaryTree->renderObjects->at(i)[j].material.SpecPow = 38.0f;*/
					//theBinaryTree->renderObjects->at(i)[j].material.Emissive = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
					//theBinaryTree->renderObjects->at(i)[j].material
					//theBinaryTree->renderObjects->at(i)[j].material.UseTexture = 1;

					matProperties.Material = MatPresets::BlinnBase;
					matProperties.Material.UseTexture = 1;

					gDeviceContext->UpdateSubresource(matConstBuff, 0, nullptr, &matProperties, 0, 0);

					UpdateMatricies(theBinaryTree->renderObjects->at(i)[j].world, CamView, CamProjection);



					gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);

					gDeviceContext->Draw(theBinaryTree->renderObjects->at(i)[j].nrElements * 3, 0);
				}
			}
		}
	

	gDeviceContext->PSSetShader(gPixelShader, nullptr, 0);

	// Shouldn't need to set shaders again if we don't change them in the above loops.
	// Leaving it meanwhile

	gDeviceContext->IASetVertexBuffers(0, 1, &theCharacter->vertexBuffer, &vertexSize, &offset);
	gDeviceContext->PSSetSamplers(0, 1, &sampState1);
	theCharacter->CalculateWorld();
	
	
	UpdateMatricies(theCharacter->world, currView, currProjection);

	gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);

	
	tex = intArrayTex[theCharacter->indexT];
	gDeviceContext->PSSetShaderResources(0, 1, &RSWArray[tex]);
	matProperties.Material = MatPresets::BlinnBase;
	matProperties.Material.UseTexture = 1;
	gDeviceContext->UpdateSubresource(matConstBuff, 0, nullptr, &matProperties, 0, 0);

	gDeviceContext->Draw(theCharacter->nrElements * 3, 0);


	//######################################################################################################################################################
	//###						SIMONS									<3										PARTIKLAR				  				 ###	
	//######################################################################################################################################################

	if (viewPoint == 2)		//If object only should be rendered from cameras POV, and therefore not to the shadow map to cast shadows.
	{
		//PARTIKLEMOJSSSSSSSS!!
		gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		gDeviceContext->IASetInputLayout(gFakeBillboardLayout);
		gDeviceContext->VSSetShader(gFakeBillboardVertexShader, nullptr, 0);
		gDeviceContext->GSSetShader(gFakeBillboardGeometryShader, nullptr, 0);
		gDeviceContext->PSSetShader(gFakeBillboardPixelShader, nullptr, 0);


		for (int i = 0; i < particleEffects.size(); i++){
			if (particleEffects[i]->playing == true){
				particleEffects[i]->PlayBillboard(gTimer.TotalTime());

				gDeviceContext->PSSetShaderResources(0, 1, particleEffects[i]->GetCurrRSV());
				gDeviceContext->IASetVertexBuffers(0, 1, particleEffects[i]->GetVertexBuffer(), &vertexSize, &offset);
				//particleEffects[i].SetPosMatrix(theCharacter->pos);
				gDeviceContext->UpdateSubresource(matConstBuff, 0, nullptr, &matProperties, 0, 0);
				UpdateMatricies(particleEffects[i]->GetPosMatrix(), currView, currProjection);
				gDeviceContext->GSSetConstantBuffers(0, 1, &gWorld);

				gDeviceContext->Draw(4, 0);
			}
		}
		gDeviceContext->VSSetShader(nullptr, nullptr, 0);
		gDeviceContext->GSSetShader(nullptr, nullptr, 0);
		gDeviceContext->PSSetShader(nullptr, nullptr, 0);
	}
}

// UPDATES

void RenderEngine::Update(float dt, PlayerObject& theCharacter)
{
	if (!start)
	{
		soundGonnaDie.PlayMp3();
		soundGonnaDie.daCapo();
		start = true;
		soundBackground.PlayMp3();
		soundBackground.soundTime = gTimer.TotalTime();
	}
	
	if (gTimer.TotalTime() - soundBackground.soundTime > soundBackground.getDuration()/10000000 - 1)
	{
		soundBackground.daCapo();
		soundBackground.StopMp3();
		soundBackground.PlayMp3();
		soundBackground.soundTime = gTimer.TotalTime();
	}

	//soundBackground.daCapo();
	Input theInput;		//Defined in .h file
	theInput.initInput(this->hInstance, hWindow);
	int input = 0;
	bool jump = false;
	bool dash = false;

	input = theInput.detectInput(hWindow);
	jump = theInput.detectJump(hWindow);
	dash = theInput.detectDash(hWindow);


	shadowBufferData.shadowTesting = theInput.detectRenderState(hWindow);

	gDeviceContext->PSSetConstantBuffers(3, 1, &shadowBuffer);
	gDeviceContext->UpdateSubresource(shadowBuffer, 0, NULL, &shadowBufferData, 0, 0);

	//Lean Camera
	mainCamera.leanCamera(theInput.detectCameraLean(hWindow));

	//Update Moving Platforms
	float tempDiv = -1;
	if (theCharacter.getDivision() == 0)
	{
		tempDiv = 0.0f;
	}
	for (int i = theCharacter.getDivision() + (int)tempDiv; i <= theCharacter.getDivision() + 1; i++)
	{
		
		for (int j = 0; j < theBinaryTree->platformsMoving->at(i).size(); j++)
			{
				theBinaryTree->platformsMoving->at(i)[j].PatrolInterval(gTimer.TotalTime());
				theBinaryTree->platformsMoving->at(i)[j].UpdateBBOX();
				theBinaryTree->platformsMoving->at(i)[j].CalculateWorld();
			}
		
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
		theCharacter.momentum = 1.0f;
		if (theCharacter.jumpMomentumX < 0)
		{
			theCharacter.momentum = 1.0f;
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
			theCharacter.jumpMomentumX = -0.1f;
		}

		if (theCharacter.jumpMomentumX > 0.0f)

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
		reset(&theCharacter, true);
		mainMenu.setPause(true);
		mainMenu.setreplay(true);
		mainMenu.setgameover(true);
	}

	if (theCollision->TestCollisionDeadly(theBinaryTree->deadlyMoving->at(theCharacter.getDivision()), &theCharacter) == true)
	{
		reset(&theCharacter, true);
		mainMenu.setPause(true);
		mainMenu.setreplay(true);
		mainMenu.setgameover(true);
	}

	if (gCounter.theAge.years > 99)
	{
		reset(&theCharacter, true);
		mainMenu.setPause(true);
		mainMenu.setreplay(true);
		mainMenu.setgameover(true);
	}

	if (theCharacter.xPos > 557 && theCharacter.xPos < 565 && theCharacter.yPos > 5 && theCharacter.yPos < 11)
	{
		if (!winning)
		{
			tajmerz = gTimer.TotalTime();
			winning = true;
		}
		
		if (tajmerz + 1.0f < gTimer.TotalTime())
		{
		mainMenu.setwin(true);
		mainMenu.setPause(true);
		mainMenu.setreplay(true);
		mainMenu.setgameover(true);
		}
		
	}
	////theCollision.TestCollision(theCustomImporter.GetStaticPlatforms()); //vi ska använda dem från customformatet men samtidigt får joel mecka så att culling fungerar med dem!


	//theCollision.TestCollision(theCustomImporter.GetStaticPlatforms()); //vi ska använda dem från customformatet men samtidigt får joel mecka så att culling fungerar med dem!



	XMFLOAT2 tempPickUpValue;
	tempPickUpValue.x = 0.0f;
	tempPickUpValue.y = 0.0f;
	tempPickUpValue = theCollision->TestCollision(theBinaryTree->collectables->at(theCharacter.getDivision()), &theCharacter);
	gCounter.addCollectable(tempPickUpValue);

	if (tempPickUpValue.y <= -0.1f){ //negativ tid
		particleEffects[0]->SetPosMatrix(theCharacter.pos);
		particleEffects[0]->Play();
	}
	else if (tempPickUpValue.y > 0.1f){ //positiv tid
		particleEffects[1]->SetPosMatrix(theCharacter.pos);
		particleEffects[1]->Play();
	}

	if (tempPickUpValue.x > 0.1f){ //coins
		particleEffects[2]->SetPosMatrix(theCharacter.pos);
		particleEffects[2]->Play();
	}

	if (input == 1 && theCollision->leftValid() == true)
	{
		if (rightDirection && CurrChar.getCharSate() == 8)
		{
			if (rand()%20 == 0)
			{
				soundGoingThisWay.PlayMp3();
				soundGoingThisWay.daCapo();
			}
			else if (rand()%20 == 10)
			{
				soundOverHere.PlayMp3();
				soundOverHere.daCapo();
			}
		}

		rightDirection = false;
		if (theCharacter.jumpMomentumState)
		{
			if (theCharacter.jumpMomentumX > theCharacter.getSpeed() * -1.0f)
			{
				theCharacter.jumpMomentumX -= 0.005f;
			}

			else
			{
				//theCharacter->jumpMomentumX = theCharacter->getSpeed() * -1;
			}

		}
		else
		{
			
			//statez = CurrChar.getCharSate();
			this->switchAnimation(&theCharacter, CurrChar.getCharSate(), 0);
			theCharacter.Move(false); //right
			
		}
		if (jump) //om grounded och man har klickat in jump

		{
			if (jump&&theCharacter.dashDisabling == true)
				this->switchAnimation(&theCharacter, CurrChar.getCharSate(), 2);
		}
		

	}

	else if (input == 2 && theCollision->rightValid() == true)
	{
		if (!rightDirection && CurrChar.getCharSate() == 8)
		{
			if (rand() % 20 == 0)
			{
				soundGoingThisWay.PlayMp3();
				soundGoingThisWay.daCapo();
			}
			else if (rand() % 20 == 10)
			{
				soundOverHere.PlayMp3();
				soundOverHere.daCapo();
			}
		}

		rightDirection = true;
		if (theCharacter.jumpMomentumState)
		{
			if (theCharacter.jumpMomentumX < theCharacter.getSpeed())
			{
				theCharacter.jumpMomentumX += 0.005f;

			}

			else
			{
				//theCharacter->jumpMomentumX = theCharacter->getSpeed();
			}

		}
		else
		{
			//statez = CurrChar.getCharSate();
			this->switchAnimation(&theCharacter, CurrChar.getCharSate(), 0);
			theCharacter.Move(true); //right
			
			
		}
		if (theCharacter.jumpMomentumState) //om grounded och man har klickat in jump

		{
			if (jump&&theCharacter.dashDisabling == true)
				this->switchAnimation(&theCharacter, CurrChar.getCharSate(), 2);
		}


	}

	else
	{
		theCharacter.momentum = 0;
		if (!theCharacter.jumpMomentumState && !jump)
		{
			
			//theCharacter.setVertexBuffer(theCharacters.at(CurrChar.getCharSate()).GetVertexBuffer());
			
			this->switchAnimation(&theCharacter, CurrChar.getCharSate(), 3);
		}
	}
		

	if (theCharacter.jumpMomentumState)
	{
		theCharacter.momentum = theCharacter.jumpMomentumX;
	}

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
		reset(&theCharacter, true);
	}
	if (input == 5)
	{
		theCharacters.at(0).xPos = theCharacter.xPos;
		theCharacters.at(0).yPos = theCharacter.yPos;
		theCharacters.at(0).SetDivision(theCharacter.getDivision());

		CurrChar.setCharState(0);
		statez = CurrChar.getCharSate();
		return;

	}
	if (input == 6)
	{
		theCharacters.at(8).xPos = theCharacter.xPos;
		theCharacters.at(8).yPos = theCharacter.yPos;
		theCharacters.at(8).SetDivision( theCharacter.getDivision());
		CurrChar.setCharState(8);
		statez = CurrChar.getCharSate();
		return;

	}
	if (input == 7)
	{
		theCharacters.at(16).xPos = theCharacter.xPos;
		theCharacters.at(16).yPos = theCharacter.yPos;
		theCharacters.at(16).SetDivision ( theCharacter.getDivision());
		CurrChar.setCharState(16);
		statez = CurrChar.getCharSate();
		return;

	}
	if (input == 8)
	{
		
		
		mainMenu.setPause(true);
		mainMenu.setreplay(true);
		mainMenu.setgameover(true);
	}
	if (input == 9)
	{

		mainMenu.setwin(true);
		mainMenu.setPause(true);
		mainMenu.setreplay(true);
		mainMenu.setgameover(true);
	}
	//if (input == 11){
	//	particleEffects[0].Play();
	//}

	if (dash && theCharacter.dashAvailable)
	{
		this->switchAnimation(&theCharacter, CurrChar.getCharSate(), 1);
		theCharacter.Dash();
		theCharacter.dashTimer = gTimer.TotalTime();

		if (CurrChar.getCharSate() == 0)
		{
			soundYoungDash.PlayMp3();
			soundYoungDash.daCapo();
		}
		else if (CurrChar.getCharSate() == 8)
		{
			soundManDash.PlayMp3();
			soundManDash.daCapo();
		}
		else if(CurrChar.getCharSate() == 16)
		{
			if (rand()%2 == 0)
			{
				soundOldDash1.PlayMp3();
				soundOldDash1.daCapo();
			}
			else
			{
				soundOldDash2.PlayMp3();
				soundOldDash2.daCapo();
			}
		}

	}
	
	if (theCollision->upValid() == false){
		thePhysics.DisableUpForce();
	}

	if (jump && theCollision->isGrounded() == true && theCharacter.jumpMomentumState == false && gTimer.TotalTime() - theCharacter.jumpTimer > 0.3f) //om grounded och man har klickat in jump

	{
		thePhysics.someBool = true;
		thePhysics.Jump(theCollision, &theCharacter);
		thePhysics.onPlatform = false;
		theCharacter.jumpTimer = gTimer.TotalTime();

		tempInt = rand() % 3;
		if (CurrChar.getCharSate() == 0)
		{
			if (tempInt == 0 || tempInt == 3)
			{
				soundYoungJump1.PlayMp3();
				soundYoungJump1.daCapo();
			}
			else if (tempInt == 1 || tempInt == 4)
			{
				soundYoungJump2.PlayMp3();
				soundYoungJump2.daCapo();
			}
			else
			{
				soundYoungJump3.PlayMp3();
				soundYoungJump3.daCapo();
			}
		}
		if (CurrChar.getCharSate() == 8)
		{
			if (tempInt == 0)
			{
				soundManJump1.PlayMp3();
				soundManJump1.daCapo();
			}
			else if (tempInt == 1)
			{
				soundManJump2.PlayMp3();
				soundManJump2.daCapo();
			}
			else
			{
				soundManJump3.PlayMp3();
				soundManJump3.daCapo();
			}
		}
		if (CurrChar.getCharSate() == 16)
		{
			if (tempInt == 0)
			{
				soundOldJump1.PlayMp3();
				soundOldJump1.daCapo();
			}
			else if (tempInt == 1)
			{
				soundOldJump2.PlayMp3();
				soundOldJump2.daCapo();
			}
			else
			{
				soundOldJump3.PlayMp3();
				soundOldJump3.daCapo();
			}
		}
	//	if (!dash&&!theCharacter.momentum > 1.1f)
	//	this->switchAnimation(&theCharacter, CurrChar.getCharSate(), 2);
		
	}

	if (jump&&theCharacter.dashDisabling == true)
	{
		this->switchAnimation(&theCharacter, CurrChar.getCharSate(), 2);
	}
	if (theCharacter.jumpMomentumState == true)
	{
		if (dash)
			this->switchAnimation(&theCharacter, CurrChar.getCharSate(), 1);

	}


	




	//TEMPORARY RESET FUNCTIONS
	if (input == 12)
	{
		resetValues[0] = resetXpos[0];
		resetValues[1] = resetYpos[0];
		reset(&theCharacter, false);
	}
	if (input == 13)
	{
		resetValues[0] = resetXpos[1];
		resetValues[1] = resetYpos[1];
		reset(&theCharacter, false);
	}
	if (input == 14)
	{
		resetValues[0] = resetXpos[2];
		resetValues[1] = resetYpos[2];
		reset(&theCharacter, false);
	}

	//MOVING PLATFORM POSITION UPDATE

	//for (unsigned i = 0; i < theBinaryTree->platformsMoving->at(theCharacter.getDivision()).size(); i++)


	// DEADLY MOVING PLATFORM ( SLAMMER ) UPDATE
	thePhysics.Gravitation(theCollision, &theCharacter);
	theCharacter.UpdatePosition(theCollision->rightValid(), theCollision->leftValid());
	theCharacter.CalculateWorld();

	tempDiv = -1.0f;
	if (theCharacter.getDivision() == 0)
	{
		tempDiv = 0.0f;
	}

	for (int o = 0; o < theBinaryTree->deadlyMoving->size(); o++)
	{			
			for (int i = 0; i < theBinaryTree->deadlyMoving->at(o).size(); i++)
			{
				int poop = 0;
				if (o == theCharacter.getDivision() || o == theCharacter.getDivision() +1 || o == theCharacter.getDivision() -1 && theCharacter.getDivision() > 0)
				{
					theBinaryTree->deadlyMoving->at(o)[i].SlamaJamma(gTimer.TotalTime(), 1);
					theBinaryTree->deadlyMoving->at(o)[i].UpdateBBOX();
					theBinaryTree->deadlyMoving->at(o)[i].CalculateWorld();
				
				}
				
				else
				{
					theBinaryTree->deadlyMoving->at(o)[i].SlamaJamma(gTimer.TotalTime(), 0);
					theBinaryTree->deadlyMoving->at(o)[i].UpdateBBOX();
					theBinaryTree->deadlyMoving->at(o)[i].CalculateWorld();
				}
		

			}
	}





	if (rightDirection)
	{
		theCharacter.Rotate(XMVECTOR(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)), 0.0f);
	}

	else
	{
		theCharacter.Rotate(XMVECTOR(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)), 3.14f);
	}

	for (int i = 0; i < theBinaryTree->collectables->at(theCharacter.getDivision()).size(); i++)
	{
		theBinaryTree->collectables->at(theCharacter.getDivision()).at(i).rotate();
	}

	for (int i = 0; i < theBinaryTree->collectables->at(theCharacter.getDivision() + 1).size(); i++)
	{
		theBinaryTree->collectables->at(theCharacter.getDivision() + 1).at(i).rotate();
	}

	if (theCharacter.getDivision() != 0)
	{
		for (int i = 0; i < theBinaryTree->collectables->at(theCharacter.getDivision() - 1).size(); i++)
		{
			theBinaryTree->collectables->at(theCharacter.getDivision() - 1).at(i).rotate();
		}
	}



	lightProp01.lights[0].Position = XMFLOAT4(mainCamera.getCameraXPos(), 60.0f, 0.0f, 1.0f);
	lightProp01.lights[0].Type = l_Directional;
	lightProp01.lights[0].Direction = XMFLOAT4(0.0f, -1.0f, 0.0f, 1.0f);
	lightProp01.lights[0].Color = XMFLOAT4(Colors::Beige);


	lightProp01.lights[1].Type = l_Point;
	lightProp01.lights[1].Position = XMFLOAT4(theCharacter.xPos, theCharacter.yPos, 0.0f, 1.0f);
	lightProp01.lights[1].Color = XMFLOAT4(Colors::WhiteSmoke);
	lightProp01.lights[1].AttConst = 0.2f;
	lightProp01.lights[1].AttLinear = 0.3f;
	lightProp01.lights[1].AttQuadratic = 0.5f;
	lightProp01.lights[1].Range = 10.0f;

	float moveL = 0.0f;

	if (lightOffsetTest < 1.0f)
	{
		lightOffsetTest += moveL;
		lightProp01.lights[2].Position = XMFLOAT4(3.0f + lightOffsetTest, -3.0f, 0.0f, 1.0f);
	}



		lightProp01.lights[2].Type = l_Directional;
		lightProp01.lights[2].Color = XMFLOAT4(0.6f, 0.6f, 0.2f, 1.0f);
		lightProp01.lights[2].Direction = XMFLOAT4(0.0f, -1.0f, -1.0f, 1.0f);
		lightProp01.lights[2].AttConst = 0.3f;
		lightProp01.lights[2].AttLinear = 0.2f;
		lightProp01.lights[2].AttQuadratic = 0.5f;
		lightProp01.lights[2].Range = 15.0f;

	lightProp01.lights[3].Type = l_Point;
	lightProp01.lights[3].Position = XMFLOAT4(20.0f, 10.0f, 0.0f, 1.0f);
	lightProp01.lights[3].Color = XMFLOAT4(Colors::Red);
	lightProp01.lights[3].AttConst = 0.7f;
	lightProp01.lights[3].AttLinear = 0.2f;
	lightProp01.lights[3].AttQuadratic = 0.0f;
	lightProp01.lights[3].Range = 15.0f;


	lightProp01.lights[4].Type = l_Point;
	lightProp01.lights[4].Position = XMFLOAT4(20.0f, 15.0f, 0.0f, 1.0f);
	lightProp01.lights[4].Color = XMFLOAT4(Colors::Blue);
	lightProp01.lights[4].AttConst = 0.7f;
	lightProp01.lights[4].AttLinear = 0.2f;
	lightProp01.lights[4].AttQuadratic = 0.0f;
	lightProp01.lights[4].Range = 15.0f;


	lightProp01.lights[5].Type = l_Point;
	lightProp01.lights[5].Position = XMFLOAT4(25.0f, 15.0f, 0.0f, 1.0f);
	lightProp01.lights[5].Color = XMFLOAT4(Colors::Blue);
	lightProp01.lights[5].AttConst = 0.7f;
	lightProp01.lights[5].AttLinear = 0.2f;
	lightProp01.lights[5].AttQuadratic = 0.0f;
	lightProp01.lights[5].Range = 15.0f;

	lightProp01.lights[6].Type = l_Point;
	lightProp01.lights[6].Position = XMFLOAT4(25.0f, 10.0f, 0.0f, 1.0f);
	lightProp01.lights[6].Color = XMFLOAT4(Colors::Red);
	lightProp01.lights[6].AttConst = 0.7f;
	lightProp01.lights[6].AttLinear = 0.2f;
	lightProp01.lights[6].AttQuadratic = 0.0f;
	lightProp01.lights[6].Range = 15.0f;

	lightProp01.lights[7].Type = l_Point;
	lightProp01.lights[7].Position = XMFLOAT4(15.0f, 5.0f, 0.0f, 1.0f);
	lightProp01.lights[7].Color = XMFLOAT4(Colors::Red);
	lightProp01.lights[7].AttConst = 0.7f;
	lightProp01.lights[7].AttLinear = 0.2f;
	lightProp01.lights[7].AttQuadratic = 0.0f;
	lightProp01.lights[7].Range = 15.0f;

	lightProp01.lights[0].Active = 1;
	lightProp01.lights[1].Active = 1;
	lightProp01.lights[2].Active = 0;
	lightProp01.lights[3].Active = 0;
	lightProp01.lights[4].Active = 0;
	lightProp01.lights[5].Active = 0;
	lightProp01.lights[6].Active = 0;
	lightProp01.lights[7].Active = 0;
	lightProp01.GlobalAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);

	if (theCharacter.yPos < -20)
	{
		if (CurrChar.getCharSate() == 0) 
		{
			soundYoungDie.PlayMp3();
		}

		else if (CurrChar.getCharSate() == 8)
		{
			soundManDie.PlayMp3();
		}
		else if (CurrChar.getCharSate() == 16)
		{
			soundOldDie.PlayMp3();
		}

		mainMenu.setPause(true);
		mainMenu.setreplay(true);
		mainMenu.setgameover(true);
	}
}

void RenderEngine::MenuUpdate(float tt, PlayerObject& theCharacter){
	Menu Menuinput;
	Menuinput.menuInit(this->hInstance, hWindow);
	int input2 = 0;

	int currentTabTemp = mainMenu.getCurrentTab();
	int scrolltime = (int)gTimer.TotalTime();
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
			if (gTimer.TotalTime() >= menuTime + 0.55f) // Om det gått 0.14 sec sen ditt senaste knapptryck
			{
				if (mainMenu.getgameover() == true)
				{
					mainMenu.setgameover(false);
					menuTime = gTimer.TotalTime();
				}
				else if (mainMenu.getgameover() == false)
				{
					//run wichever tab i currently selected --------------------------------------- button functions
					if (mainMenu.getCurrentTab() == 1)
					{
						//gTimer.setPausedTime(pauseTime);
						//gTimer.setCurrTime(pauseTime);
						if (mainMenu.getreplay() == false)
						{


							gTimer.Start(pauseTime);
							mainMenu.setPause(FALSE);
							theHighScore.setHSbool(FALSE);
							menuTime = gTimer.TotalTime();
						}
						else if (mainMenu.getreplay() == true)
						{

							reset(&theCharacter, true);
							mainMenu.setreplay(false);
							mainMenu.setPause(false);
							menuTime = gTimer.TotalTime();

						}
					}
					else if (mainMenu.getCurrentTab() == 2)
					{
						// open highscores
						if (theHighScore.getHSbool() == FALSE)
						{
							theHighScore.setHSbool(TRUE);
						}
						else if (theHighScore.getHSbool() == TRUE)
						{
							theHighScore.setHSbool(FALSE);

						}
						menuTime = gTimer.TotalTime();
					}
					else if (mainMenu.getCurrentTab() == 3)
					{
						PostMessage(hWindow, WM_QUIT, 0, 0);
					}
				}
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
		if (input2 == 5)
		{
			
			//mainMenu.setreplay(true);

		}
	}



}

void RenderEngine::StartMenuUpdate(float tt){
	Startmenu Menuinput;
	Menuinput.menuInit(this->hInstance, hWindow);
	int input3 = 0;

	int currentTabTemp = StartMenu.getCurrentTab();
	int scrolltime = (int)gTimer.TotalTime();
	//if (gTimer.TotalTime() >= scrolltime + 1.0f)

	if (gTimer.TotalTime() >= menuTime + 0.14f) // Om det gått 0.14 sec sen ditt senaste knapptryck
	{
		input3 = Menuinput.menuInput(hWindow);

		if (input3 == 2)
		{
			if (izz < 3)
				izz++;
			else
				izz = 1;
			menuTime = gTimer.TotalTime();

		}
		if (input3 == 1)
		{
			if (izz > 1)
				izz--;
			else
				izz = 3;

			menuTime = gTimer.TotalTime();
		}

		if (input3 == 3)
		{
			if (gTimer.TotalTime() >= menuTime + 0.55f) // Om det gått 0.14 sec sen ditt senaste knapptryck
			{
				//run wichever tab i currently selected --------------------------------------- button functions
				if (StartMenu.getCurrentTab() == 1)
				{
					//gTimer.setPausedTime(pauseTime);
					//gTimer.setCurrTime(pauseTime);
					gTimer.Reset();
					gTimer.Start(pauseTime);
					//mainMenu.setPause(FALSE);
					//theHighScore.setHSbool(FALSE);
					StartMenu.setstartmeny(false);
					menuTime = 0;
					menuTime = gTimer.TotalTime();
				}
				else if (StartMenu.getCurrentTab() == 2)
				{
					// open highscores
					if (theHighScore.getHSbool() == FALSE)
					{
						theHighScore.setHSbool(TRUE);
						menuTime = gTimer.TotalTime();
					}
					else if (theHighScore.getHSbool() == TRUE)
					{
						theHighScore.setHSbool(FALSE);
						menuTime = gTimer.TotalTime();

					}
				}
				else if (StartMenu.getCurrentTab() == 3)
				{
					PostMessage(hWindow, WM_QUIT, 0, 0);
				}


			}
		}

		StartMenu.setCurrentTab(izz);


		if (input3 == 4)
		{
			gTimer.Start(pauseTime);

			//gTimer.setCurrTime(pauseTime);
			//__int64 Paous = gTimer.TotalTime() - pauseTime;
			//gTimer.setPausedTime(Paous);
			
			theHighScore.setHSbool(FALSE);
			menuTime = 0;

			//float gTimer.TotalTime() = pauseTime;

		}
	}



}

// REALESE AND CLEANUP

void RenderEngine::Release(){

	for (int i = 0; i < texes-11; i++)
	{
		RSWArray[i]->Release();
	}
		
	
	Meter->Release();
	Meter2->Release();
	Meter3->Release();
	Meter4->Release();
	Meter5->Release();
	Meter6->Release();
	Meter7->Release();
	Meter8->Release();
	Meter9->Release();
	Meter10->Release();
	Meter11->Release();

	delete[] RSWArray;
	matConstBuff->Release();
	/*delete theCharacter1;
	delete theCharacter2;
	delete theCharacter3;*/
	delete theBinaryTree;
	theCharacters.clear();

	for (int i = 0; i < particleEffects.size(); i++) {
		delete particleEffects[i];
	}
	particleEffects.clear();

	gDevice->Release();
	depthStencilBuffer->Release();

	//gVertexBuffer->Release();
	gVertexLayout->Release();
	gVertexShader->Release();
	gPixelShader->Release();
	gDeviceContext->Release();
//	cbPerObjectBuffer->Release();

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

		//theCharacter1->CreateBBOXVertexBuffer(gDevice);
		theCharacter1->nrElements = objectTest.GetNrElements();
		//gameObjects.push_back(*theCharacter);
	}

	else if (type == 1)
	{
		if (isStatic == false){
			Platform testPlatform(false, *objectTest.GetVertexBuffer(), XMFLOAT3(0, 0, 0), true, false, *objectTest.theBoundingBox, 3, 4.0f, 1.0f, 1.0f);
			//testPlatform.CreateBBOXVertexBuffer(gDevice);
			testPlatform.nrElements = objectTest.GetNrElements();
			
			theBinaryTree->AddPlatform(testPlatform);
		}
		else{
			Platform testPlatform(false, *objectTest.GetVertexBuffer(), XMFLOAT3(0, 0, 0), true, true, *objectTest.theBoundingBox, 0, 0, 0, 0);
			//testPlatform.CreateBBOXVertexBuffer(gDevice);
			testPlatform.nrElements = objectTest.GetNrElements();
			theBinaryTree->AddPlatform(testPlatform);
		}
	}
	if (type == 4)
	{
		theCharacter2 = new PlayerObject(*objectTest.GetVertexBuffer(), XMFLOAT3(4, 9, 0), true, false, BoundingBox(XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1)), 0, 0, 0.1f, 0.6f);
		theCharacter2->Scale(0.5f, 0.5f, 0.5f);
		//theCharacter2->CreateBBOXVertexBuffer(gDevice);
		theCharacter2->nrElements = objectTest.GetNrElements();
		
		/*Collision tempD(theCharacter2);
		theCollision = &tempD;*/
		//gameObjects.push_back(*theCharacter);
	}
	if (type == 5)
	{
		theCharacter3 = new PlayerObject(*objectTest.GetVertexBuffer(), XMFLOAT3(4, 9, 0), true, false, BoundingBox(XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1)), 0, 0, 0.1f, 0.6f);

		//theCharacter3->CreateBBOXVertexBuffer(gDevice);
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

void RenderEngine::reset(PlayerObject* theCharacter, bool fullreset)
{
	soundBackground.daCapo();
	soundBackground.StopMp3();
	soundYoungDie.daCapo();
	soundYoungDie.StopMp3();
	soundManDie.daCapo();
	soundManDie.StopMp3();
	soundOldDie.daCapo();
	soundOldDie.StopMp3();
	start = false;
	Character2 = false;
	Character3 = false;
	CurrChar.setCharState(CurrChar.getCharSate());
	if (fullreset)
	{
		theCharacter->xPos = 4;
		theCharacter->yPos = 6;
	}
	
	else
	{
		theCharacter->xPos = resetValues[0];
		theCharacter->yPos = resetValues[1];
	}
	theCharacter->Translate(0, 0, 0);
	if (fullreset)
	{
		theCharacter->setDivision(0);
	}
	
	theCharacter->momentum = 0;
	theCharacter->jumpMomentumX = 0;
	mainCamera.setCameraXPos(theCharacter->xPos);
	mainCamera.setCameraYPos(theCharacter->yPos);
	gCounter.theAge.years = 0;
	gCounter.theAge.months = 0;
	gCounter.setCoins(0);

	for (int i = 0; i < 20; i++)
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
	resetValues[0] = resetXpos[0];
	resetValues[1] = resetYpos[0];
}

void RenderEngine::LoadSounds()
{
	soundBackground.InitMp3();
	soundBackground.LoadMp3("song.mp3");
	soundBackground.setVolume(-2000);
	soundGoingThisWay.InitMp3();
	soundGoingThisWay.LoadMp3("GoingThisWay.wav");
	soundGonnaDie.InitMp3();
	soundGonnaDie.LoadMp3("GonnaDie.wav");
	soundHelpMe.InitMp3();
	soundHelpMe.LoadMp3("HelpMe.wav");
	soundIGotThis.InitMp3();
	soundIGotThis.LoadMp3("IGotThis.wav");
	soundLifeIsA.InitMp3();
	soundLifeIsA.LoadMp3("LifeIsA.wav");
	soundManDie.InitMp3();
	soundManDie.LoadMp3("manDie.wav");
	soundManJump1.InitMp3();
	soundManJump1.LoadMp3("manJump1.wav");
	soundManJump2.InitMp3();
	soundManJump2.LoadMp3("manJump2.wav");
	soundManJump3.InitMp3();
	soundManJump3.LoadMp3("manJump3.wav");
	soundOldDash1.InitMp3();
	soundOldDash1.LoadMp3("oldDash.wav");
	soundOldDash2.InitMp3();
	soundOldDash2.LoadMp3("oldDash2.wav");
	soundOldDie.InitMp3();
	soundOldDie.LoadMp3("oldDie.wav");
	soundOldJump1.InitMp3();
	soundOldJump1.LoadMp3("oldJump1.wav");
	soundOldJump2.InitMp3();
	soundOldJump2.LoadMp3("oldJump2.wav");
	soundOldJump3.InitMp3();
	soundOldJump3.LoadMp3("oldJump3.wav");
	soundOverHere.InitMp3();
	soundOverHere.LoadMp3("What'sOverHere.wav");
	soundWhereAmI.InitMp3();
	soundWhereAmI.LoadMp3("WhereAmI.wav");
	soundYoungDash.InitMp3();
	soundYoungDash.LoadMp3("youngDash.wav");
	soundYoungDie.InitMp3();
	soundYoungDie.LoadMp3("youngDie.wav");
	//soundYoungDie.setVolume(1000);
	soundYoungJump1.InitMp3();
	soundYoungJump1.LoadMp3("youngJump1.wav");
	soundYoungJump2.InitMp3();
	soundYoungJump2.LoadMp3("youngJump2.wav");
	soundYoungJump3.InitMp3();
	soundYoungJump3.LoadMp3("youngJump3.wav");
	soundManDash.InitMp3();
	soundManDash.LoadMp3("manDash.wav");
	
}

void RenderEngine::switchAnimation(PlayerObject* theCharacter, int curr, int switcher)
{
	static float time = 0.0f;
	//float tajm = gTimer.TotalTime();
	//if (loop == 1 && tajm + 0.2f > gTimer.TotalTime())
	//{
	//	theCharacter->setVertexBuffer(theCharacters.at(curr + 1).GetVertexBuffer());

	//	tajm = gTimer.TotalTime();
	//	loop= 2;
	//	OutputDebugStringA("Enter loop 1");
	//}
	//if (loop == 2 && tajm + 0.2f > gTimer.TotalTime())
	//{
	//	theCharacter->setVertexBuffer(theCharacters.at(curr + 2).GetVertexBuffer());
	//	tajm = gTimer.TotalTime();
	//	loop = 1;
	//	OutputDebugStringA("Enter loop 2");
	//}
	if (switcher == 0)
	{
		if ((gTimer.TotalTime() - time) >= 0.12f)
		{
			theCharacter->setVertexBuffer(theCharacters.at(statez + 1).GetVertexBuffer());
			if (statez < CurrChar.getCharSate()+4)
				statez = statez + 1;
			if (statez == CurrChar.getCharSate() + 4)
				statez = curr;

			time += 0.12f;
		}
	}

	if (switcher == 1)
	{
		if ((gTimer.TotalTime() - time) >= 0.10f)
		{
			theCharacter->setVertexBuffer(theCharacters.at(curr + 5).GetVertexBuffer());
			

			time += 0.15f;
		}
		//theCharacter->setVertexBuffer(theCharacters.at(curr).GetVertexBuffer());
	}
	if (switcher == 2)
	{
		if ((gTimer.TotalTime() - time) >= 0.10f)
		{
			theCharacter->setVertexBuffer(theCharacters.at(curr + 6).GetVertexBuffer());


			time += 0.15f;
			//theCharacter->setVertexBuffer(theCharacters.at(curr).GetVertexBuffer());
		}
		
	}
	if (switcher == 3)
	{
		if ((gTimer.TotalTime() - time) >= 0.10f)
		{
			theCharacter->setVertexBuffer(theCharacters.at(curr+7).GetVertexBuffer());


			time += 0.15f;
		}
		//theCharacter->setVertexBuffer(theCharacters.at(curr).GetVertexBuffer());
	}
	
	//if (loop == 3 && tajm + 0.4f > gTimer.TotalTime())
	//{
	//	theCharacters.at(curr).setVertexBuffer(theCharacters.at(curr + 3).GetVertexBuffer());
	//	tajm = gTimer.TotalTime();
	//	loop = loop + 1;
	//}
	//if (loop == 4 && gTimer.TotalTime() >= tajm + 0.5f)
	//{
	//	theCharacters.at(curr).setVertexBuffer(theCharacters.at(curr + 4).GetVertexBuffer());
	//	tajm = gTimer.TotalTime();
	//	loop = loop + 1;
	//}
	//
	
}
