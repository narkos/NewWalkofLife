#include "RenderEngine.h"
#include <string.h>


class Applikation : public RenderEngine{
public:
	Applikation(HINSTANCE hInstance, std::string name, UINT scrW, UINT scrH);
	~Applikation();

	bool Init() override;
	void Update(float dt);
	void Render();


};

Applikation::Applikation(HINSTANCE hInstance, std::string name, UINT scrW, UINT scrH) :RenderEngine(hInstance, name, scrW, scrH){

}

Applikation::~Applikation(){

}

bool Applikation::Init(){
	if (!RenderEngine::Init()){
		return false;
	}

	return true;
}




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	Applikation* pApplication = new Applikation(hInstance, "App name", 800, 600);

	if (!pApplication->Init()){
		MessageBox(NULL, L"Kunde inte k?ra Init()", NULL, NULL);
		return 1;
	}

	pApplication->Run(); //om allt funkar s? ska den b?rja k?ra applikationen - Run() k?r ig?ng bla Update() och Render()
	return 0;
}