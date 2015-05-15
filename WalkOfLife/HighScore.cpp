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

void HighScore::Highscorespritebatch()
{
	spriteBatch_HS->Begin();

	std::wstring C_coin1 = std::to_wstring(coinList[0].coins);
	std::wstring C_time1 = std::to_wstring(coinList[0].time);
	std::wstring C_point1 = std::to_wstring(coinList[0].points);
	std::wstring coinz(L"Coins: ");
	std::wstring timez(L"\nYear: ");
	std::wstring pointz(L"\nMonth: ");
	std::wstring C_list1 = coinz + C_coin1 + timez + C_time1 + pointz + C_point1;

	std::wstring Gameover(L"\nGAME OVER MOTHERFUCKER!! ");


	const wchar_t* C_LIST_1 = C_list1.c_str();

	

	spritefont_HS->DrawString(spriteBatch_HS.get(), C_LIST_1, DirectX::SimpleMath::Vector2(20, 100));

	spriteBatch_HS->End();
}