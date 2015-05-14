#include <iostream>
#include <fstream>
#include <d3d11.h>
#include <vector>
#include <string>

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
	vector<int> coinList;
	vector<int> timeList;

public:
	HighScore(char* fileNameL, char* fileNameS){
		this->fileNameLoad = fileNameL;
		this->fileNameSave = fileNameS;
		bool isFine;
		isFine = LoadHighScore();
	}

	bool LoadHighScore();

	bool SaveHighScore();

	void AddScore(int coins, int time, int points){
		Score scoreTemp;
		scoreTemp.coins = coins;
		scoreTemp.time = time;
		scoreTemp.points = points;
	}

	void ReOrganizeLists();
	void SwapInt(int &a, int &b){
		int temp;
		temp = a;
		a = b;
		b = temp;
	}
};