#include "HighScore.h"

bool HighScore::LoadHighScore(){
	loadFile.open(fileNameLoad);
	string line;

	if (saveFile.is_open()){
		while (getline(loadFile, line)){
			Score scoreTemp;
			sscanf_s(&line[0], "%d", &scoreTemp.coins);
			sscanf_s(&line[0], "%d", &scoreTemp.time);
			sscanf_s(&line[0], "%d", &scoreTemp.points);
			highScoreList.push_back(scoreTemp);
		}
		loadFile.close();
		return true;
	}
	else return false;

	
}

bool HighScore::SaveHighScore(){
	saveFile.open(fileNameSave);
	if (saveFile.is_open() == true){
		saveFile.clear();
		for each(Score s in highScoreList){
			saveFile << s.coins;
			saveFile << s.time;
			saveFile << s.points;
			saveFile << "\n";
		}
		saveFile.close();
		return true;
	}
	else return false;
	
}

void HighScore::ReOrganizeLists(){
	
	for (int i = 0; i < highScoreList.size(); i++){
		coinList.push_back(highScoreList[i].coins);
		timeList.push_back(highScoreList[i].time);
	}

	for (int y = 0; y < coinList.size(); y++){
		for (int i = 0; i < coinList.size(); i++){
			if (coinList[y] < coinList[i]){
				SwapInt(coinList[y], coinList[i]);
			}

			if (timeList[y] < timeList[i]){
				SwapInt(timeList[y], timeList[i]);
			}
		}
	}
}