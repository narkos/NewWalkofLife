#include "HighScore.h"

bool HighScore::LoadHighScore(){
	loadFile.open(fileNameLoad);
	string line;

	if (loadFile.is_open()){
		Score scoreTemp;
		while (getline(loadFile, line)){
			if (line[0] == 'c')
				sscanf_s(&line[0], "c%d", &scoreTemp.coins);
			else if (line[0] == 't')
				sscanf_s(&line[0], "t%d", &scoreTemp.time);
			else{
				sscanf_s(&line[0], "p%d", &scoreTemp.points);
				highScoreList.push_back(scoreTemp);
			}
			
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
			saveFile << "c" << s.coins;
			saveFile << "\n";
			saveFile << "t" << s.time;
			saveFile << "\n";
			saveFile << "p" << s.points;
			saveFile << "\n";
		}
		saveFile.close();
		return true;
	}
	else return false;
	
}

void HighScore::ReOrganizeLists(){
	
	for (int i = 0; i < highScoreList.size(); i++){
		coinList.push_back(highScoreList[i]);
		timeList.push_back(highScoreList[i]);
	}

	for (int y = 0; y < coinList.size(); y++){
		for (int i = 0; i < coinList.size(); i++){
			if (coinList[y].coins < coinList[i].coins){
				SwapScore(coinList[y], coinList[i]);
			}

			if (timeList[y].time < timeList[i].time){
				SwapScore(timeList[y], timeList[i]);
			}
		}
	}
	SaveHighScore();
}