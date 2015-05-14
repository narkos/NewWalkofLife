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
	vector<Score> coinList;
	vector<Score> timeList;

public:

	HighScore(){
		fileNameLoad = "C:\\Users\\Simon\\Desktop\\HighScoreLoad.txt";
		fileNameSave = "C:\\Users\\Simon\\Desktop\\HighScoreLoad.txt";
		bool isFine;
		isFine = LoadHighScore();
	}
	~HighScore(){}

	bool LoadHighScore();

	bool SaveHighScore();

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
};