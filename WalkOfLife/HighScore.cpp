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
		ReOrganizeLists();
		return true;
	}
	else return false;

	
}

bool HighScore::SaveHighScore(){
	saveFile.open(fileNameSave);
	if (saveFile.is_open() == true){
		saveFile.clear();
		int i = 0;
		for each(Score s in highScoreList){
			saveFile << "c" << s.coins;
			saveFile << "\n";
			saveFile << "t" << s.time;
			saveFile << "\n";
			saveFile << "p" << s.points;
			saveFile << "\n";
			if (i > maxListSize)
				break;
			i++;
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
	for (int i = 0; i < highScoreList.size(); i++){
		for (int y = 0; y < highScoreList.size(); y++){
			if (highScoreList[y].points > highScoreList[i].points){
				SwapScore(highScoreList[i], highScoreList[y]);
			}
		}
	}
	SaveHighScore();
}

void HighScore::Highscorespritebatch(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext, float width, float height, IDXGISwapChain* gSwapChain)
{
	spritefont_HS.reset(new DirectX::SpriteFont(gDevice, L"Fonts/Arial.spritefont"));
	spriteBatch_HS.reset(new DirectX::SpriteBatch(gDeviceContext));
	

	spriteBatch_HS->Begin();
	///-------Coinlist
	std::wstring BestC(L"Best coin run ");
	std::wstring C_coin1 = std::to_wstring(coinList[coinList.size()-1].coins);
	std::wstring C_time1 = std::to_wstring(coinList[coinList.size()-1].time);
	std::wstring C_point1 = std::to_wstring(coinList[coinList.size()-1].points);
	///-------Timelist
	std::wstring BestT(L"Fastest run ");
	std::wstring T_coin1 = std::to_wstring(timeList[0].coins);
	std::wstring T_time1 = std::to_wstring(timeList[0].time);
	std::wstring T_point1 = std::to_wstring(timeList[0].points);
	///-------Pointlist
	std::wstring BestH(L"Best total point run ");
	std::wstring H_coin1 = std::to_wstring(highScoreList[highScoreList.size()-1].coins);
	std::wstring H_time1 = std::to_wstring(highScoreList[highScoreList.size() - 1].time);
	std::wstring H_point1 = std::to_wstring(highScoreList[highScoreList.size() - 1].points);
	/*std::wstring C_coin1 = std::to_wstring(1);
	std::wstring C_time1 = std::to_wstring(2);
	std::wstring C_point1 = std::to_wstring(3);*/
	std::wstring coinz(L"\nCoins: ");
	std::wstring timez(L"\nTime: ");
	std::wstring pointz(L"\nPoints: ");
	std::wstring C_list1 = BestC + coinz + C_coin1 + timez + C_time1 + pointz + C_point1;
	std::wstring T_list1 = BestT + coinz + T_coin1 + timez + T_time1 + pointz + T_point1;
	std::wstring H_list1 = BestH + coinz + H_coin1 + timez + H_time1 + pointz + H_point1;

	std::wstring Gameover(L"\nGAME OVER MOTHERFUCKER!! ");


	const wchar_t* C_LIST_1 = C_list1.c_str();
	const wchar_t* T_LIST_1 = T_list1.c_str();
	const wchar_t* H_LIST_1 = H_list1.c_str();
	

	spritefont_HS->DrawString(spriteBatch_HS.get(), C_LIST_1, DirectX::SimpleMath::Vector2((width / 1.4)+10, 500));
	spritefont_HS->DrawString(spriteBatch_HS.get(), T_LIST_1, DirectX::SimpleMath::Vector2((width / 1.4)+10, 350));
	spritefont_HS->DrawString(spriteBatch_HS.get(), H_LIST_1, DirectX::SimpleMath::Vector2((width / 1.4)+10, 200));


	spriteBatch_HS->End();
}

bool HighScore::getHSbool()
{
	return HSbool;
}
void HighScore::setHSbool(bool set)
{
	HSbool = set;
	
}
