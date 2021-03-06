#pragma once

#ifndef RENDERENGINE_H
#define RENDERENGINE_H
#endif

#include <windows.h>
#include <DirectXMath.h>
#include <DirectXMathMatrix.inl>
#include <DirectXMathVector.inl>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <DirectXCollision.h>
#include "HighScore.h"
#include "FBXImporter.h"
#include "BillboardTextureEffect.h"

//INLCUDE H FILES HERE

//#include "GameObject.h"
#include "BinaryTree.h"
#include "Camera.h"
#include "Physics.h"
#include "Input.h"
#include "Platform.h"
#include "PlayerObject.h"
#include "GameTimer.h"
#include "Counters.h"
#include "Obj.h"
#include "Light.h"
#include "Sound.h"
#include "Extra DirectXLibs\Inc\DDSTextureLoader.h"
#include "Extra DirectXLibs\Inc\SpriteFont.h"
#include "Extra DirectXLibs\Inc\SpriteBatch.h"
#include "Extra DirectXLibs\Inc\SimpleMath.h"
#include "Extra DirectXLibs\Inc\WICTextureLoader.h"
#include "Menuclass.h"
#include "HighScore.h"
#include "CharSwitch.h"
#include "Startmeny.h"
#include "Shadows.h"


using namespace DirectX;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

class RenderEngine{ //DENNA KLASSEN �R ABSTRAKT - g�r inte instantiera

public:
	HWND hWindow;
	RenderEngine(HINSTANCE hInstance, std::string name, UINT scrW, UINT scrH);
	virtual ~RenderEngine();

	int Run();
	float time3;
	float time4;
	int loop = 1;
	void SetViewport();
	virtual bool Init();
	void Update(float dt, PlayerObject& theCharacter);

	void MenuUpdate(float tt, PlayerObject& theCharacter);

	void StartMenuUpdate(float tt);
	void Render(PlayerObject* theCharacter);
	virtual void Release();
	void Fonts();
	void fpscounter();
	void reset(PlayerObject* theCharacter, bool fullreset);
	void LoadSounds();
	void drawScene(int viewPoint, PlayerObject* theCharacter);
	void UpdateMatricies(XMMATRIX &worldM, XMMATRIX &viewM, XMMATRIX &projM);
	bool winning = false;
	float tajmerz = 0;

	// Render states functions
	void RenderEngine::setDepthStencilOff();
	void RenderEngine::setDepthStencilOn();
	void RenderEngine::setAlphaBlendingOff();
	void RenderEngine::setAlphaBlendingOn();
	

	//animation
	void RenderEngine::switchAnimation(PlayerObject* theCharacter, int curr, int switcher);

	std::vector<PlayerObject> theCharacters;
	Sound soundOldJump1;
	Sound soundOldJump2;
	Sound soundOldJump3;
	Sound soundYoungJump1;
	Sound soundYoungJump2;
	Sound soundYoungJump3;
	Sound soundManJump1;
	Sound soundManJump2;
	Sound soundManJump3;
	Sound soundManDash;
	Sound soundHelpMe;
	Sound soundGoingThisWay;
	Sound soundIGotThis;
	Sound soundWhereAmI;
	Sound soundOverHere;
	Sound soundYoungDash;
	Sound soundYoungDie;
	Sound soundOldDash1;
	Sound soundOldDash2;
	Sound soundOldDie;
	Sound soundManDie;
	Sound soundLifeIsA;
	Sound soundGonnaDie;
	Sound soundBackground;
	Sound soundMenu;
	int tempInt;
	bool start;
	BinaryTree *theBinaryTree;
	//Quadtree *theQuadtree;
	Collision* theCollision;
	Collision testStaticPlatforms, testDynamicPlatforms;
	Collision tempD;
	//Collision* theCollision2;
	//Collision* theCollision3;
	OBJ theOBJ;
	Physics thePhysics;
	FBXImporter theCustomImporter;
	HighScore theHighScore;
	GameTimer gTimer;
	Counters gCounter;
	PlayerObject* theCharacter1;
	PlayerObject* theCharacter2;
	PlayerObject* theCharacter3;
	int texes = 0;
	Camera mainCamera;
	Shadows shadows;
	vector <BillboardTextureEffect*> particleEffects; //g�r det till en vektor
	Menu mainMenu;
	CharSwitch CurrChar;

	Startmenu StartMenu;

	//test 
	int statez;

	//Input theInput;
	bool Character1 = false;
	bool Character2 = false;
	bool Character3 = false;

	std::wstring mainwname;
	bool rightDirection;
	bool haschanged = false;

	//Light Shit
	
	ID3D11Buffer* lightConstBuff;
	XMFLOAT4 globalAmb;
	LightProperties lightProp01;
	Light currentMainLight[2];
	XMFLOAT4 camPos;
	//float camxPos;
	//float camyPos;


	float lightOffsetTest;

	//Material Shit
	ID3D11Buffer* matConstBuff;
	ID3D11Buffer* cbPerObjectBuffer = nullptr;
	ID3D11Buffer* Tempidlebuffer = nullptr;
	MaterialProperties matProperties;
	//Import Functions
	void ImportObj(char* geometryFileName, char* materialFileName, ID3D11Device* gDev, int type, bool isStatic);// , bool isStatic, XMMATRIX startPosMatrix);
	//Struct for objLoader
	//struct GameObjects{
	//	int gameObjectIndex;
	//	ID3D11Buffer* gVertexBuffer;
	//	//ID3D11Buffer* gIndexBuffer;
	//	UINT nrElements = 0;
	//	BoundingBox bbox;

	//	char* ObjName;
	//	std::vector<XMFLOAT3> verteciesPos;
	//	std::vector<float> verteciesPosX;
	//	std::vector<float> verteciesPosY;
	//	std::vector<float> verteciesPosZ;
	//	std::vector<int> verteciesIndex;

	//};

	//Tempor?r geometry
	void Shaders();
	void TextureFunc();
	//Debugging variables
	float resetXpos[3];
	float resetYpos[3];
	float resetValues[2];


	// Message handler
	LRESULT MsgProc(HWND hwindow, UINT msg, WPARAM wParam, LPARAM lParam);


	//FONTS AND BATCHES
	std::unique_ptr<DirectX::SpriteFont> spritefont;
	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;

	struct World
	{
		XMFLOAT4X4 View;
		XMFLOAT4X4 Projection;
		XMFLOAT4X4 WorldSpace;
		XMFLOAT4X4 InvWorld;
		XMFLOAT4X4 WVP;
		//BOOL hasNormMap;

		//Shadow Stuff
		XMFLOAT4X4 lightView;
		XMFLOAT4X4 lightProjection;
	};

	World perObjCBData;
	
	struct shadowSettings
	{
		int shadowTesting;
		int pad1;
		int pad2;
		int pad3;
	};
	shadowSettings shadowBufferData;

protected:
	 //handle till f?nstret
	HINSTANCE hInstance;

	std::string applicationName;
	UINT screen_Width;
	UINT screen_Height;
	UINT fpsDisplay=0;
	DWORD windowStyle; //se konstruktorn

	bool InitWindow();
	bool InitDirect3D(HWND hWindow);

	ID3D11Device* gDevice = nullptr;
	ID3D11DeviceContext* gDeviceContext = nullptr;
	ID3D11RenderTargetView* gBackRufferRenderTargetView = nullptr;
	IDXGISwapChain* gSwapChain = nullptr;
	ID3D11DepthStencilView* gDepthStencilView = nullptr;

	ID3D11Texture2D* depthStencilBuffer = nullptr;
	ID3D11DepthStencilState* gDepthStencilState = nullptr;
	ID3D11DepthStencilState* gDepthStencilStateDisable = nullptr;
	ID3D11BlendState* gBlendStateTransparency = nullptr;
	ID3D11BlendState* gBlendStateDefault = nullptr;

	ID3D11Buffer* gVertexBuffer = nullptr;
	ID3D11Buffer* gVertexBuffer2 = nullptr;
	ID3D11Buffer* gWorld;
	ID3D11Buffer* shadowBuffer = nullptr;

	ID3D11SamplerState* sampState1 = nullptr;

	ID3D11ShaderResourceView* gTextureView = nullptr;
	
	ID3D11ShaderResourceView** RSWArray = nullptr;
	ID3D11ShaderResourceView* normalMap = nullptr;


	ID3D11InputLayout* gVertexLayout = nullptr;
	ID3D11VertexShader* gVertexShader = nullptr;
	ID3D11PixelShader* gPixelShader = nullptr;
	ID3D11PixelShader* gPixelShader2 = nullptr;
	ID3D11GeometryShader* gGeometryShader = nullptr;


	ID3D11VertexShader* gWireFrameVertexShader = nullptr;
	ID3D11PixelShader* gWireFramePixelShader = nullptr;
	ID3D11InputLayout* gWireFrameLayout = nullptr;

	ID3D11VertexShader* gFakeBillboardVertexShader = nullptr;
	ID3D11GeometryShader* gFakeBillboardGeometryShader = nullptr;
	ID3D11PixelShader* gFakeBillboardPixelShader = nullptr;
	ID3D11InputLayout* gFakeBillboardLayout = nullptr;
	vector<int> intArrayTex;
	////
	//std::unique_ptr<DirectX::SpriteBatch> MeterSpriteBatch;
	ID3D11ShaderResourceView* Meter = nullptr;
	ID3D11ShaderResourceView* Meter1 = nullptr;
	ID3D11ShaderResourceView* Meter2 = nullptr;
	ID3D11ShaderResourceView* Meter3 = nullptr;
	ID3D11ShaderResourceView* Meter4 = nullptr;
	ID3D11ShaderResourceView* Meter5 = nullptr;
	ID3D11ShaderResourceView* Meter6 = nullptr;
	ID3D11ShaderResourceView* Meter7 = nullptr;
	ID3D11ShaderResourceView* Meter8 = nullptr;
	ID3D11ShaderResourceView* Meter9 = nullptr;
	ID3D11ShaderResourceView* Meter10 = nullptr;
	ID3D11ShaderResourceView* Meter11 = nullptr;
	void Meterfunc();
	float meterwidth = mainCamera.getWindowWidth();
	//////////////////////Anton
	float pauseTime = 0;
	float menuTime = 0;
	int pausYearCount;
	int pausMonthCount;
	int Tabofdoom;
	float scrolltime;
	int iz = 1;
	int izz = 1;
	int shadowTemp = 0;

	UINT32 vertexSize = sizeof(float)* 8;
	UINT32 offset = 0;

	//World perObjCBData;
	XMMATRIX WVP;

	//Shadow implement
	//The Camera Matrices are now defined in the camera class (mainCamera)
	XMMATRIX CamView;
	XMMATRIX CamProjection;
	XMMATRIX identityM;
	XMMATRIX WorldInv;
};