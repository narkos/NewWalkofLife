#pragma once

#ifndef HIGHSCORE_H
#define HIGHSCORE_H
#endif



#include <iostream>
#include <fstream>
#include <d3d11.h>
#include <vector>
#include <string>
#include "Extra DirectXLibs\Inc\DDSTextureLoader.h"
#include "Extra DirectXLibs\Inc\SpriteFont.h"
#include "Extra DirectXLibs\Inc\SpriteBatch.h"
#include "Extra DirectXLibs\Inc\SimpleMath.h"
//using namespace DirectX;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

using namespace std;
class HighScore{
private:
	ifstream loadFile;
	ofstream saveFile;
	char* fileNameLoad;
	char* fileNameSave;

	struct Score{
		int coins;
		int time;
		int points;
	};
	vector<Score> highScoreList;
	vector<Score> coinList;
	vector<Score> timeList;
	bool HSbool = FALSE;

	int maxListSize = 10;
	//anton
	

public:

	bool LoadHighScore();
	bool getHSbool();
	void setHSbool(bool set);
	bool SaveHighScore();
	std::unique_ptr<DirectX::SpriteFont> spritefont_HS;
	std::unique_ptr<DirectX::SpriteBatch> spriteBatch_HS;

	HighScore(){
		fileNameLoad = "HighScoreLoad.txt";
		fileNameSave = "HighScoreLoad.txt";
		bool isFine;
		isFine = LoadHighScore();
	}
	~HighScore(){
		highScoreList.clear();
		coinList.clear();
		timeList.clear();
	}


	

	void AddScore(int coins, int time, int points){
		Score scoreTemp;
		scoreTemp.coins = coins;
		scoreTemp.time = time;
		scoreTemp.points = points;
		highScoreList.push_back(scoreTemp);
	}

	void ReOrganizeLists();
	void SwapInt(int &a, int &b){
		int temp;
		temp = a;
		a = b;
		b = temp;
	}
	void SwapScore(Score &a, Score &b){
		Score temp;
		temp = a;
		a = b;
		b = temp;
	}

	vector<Score> GetHighScoreList(){
		return highScoreList;
	}
	vector<Score> GetCoinList(){
		return coinList;
	}
	vector<Score> GetTimeList(){
		return timeList;
	}
	void Highscorespritebatch(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext, float width, float height, IDXGISwapChain* gSwapChain);
};