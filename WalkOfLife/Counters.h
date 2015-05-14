#include <windows.h>
#include <DirectXMath.h>

using namespace DirectX;

class Counters{

public:
	Counters();
	void addCoin();
	void addCigg();
	void addCream();
	//------ get functions
	int getCoin();
	int getCigg();
	int getCream();
	void setYear(int year);
	void setMonth(int month);
	void takeCigg();
	void takeCream();
	void takeCoin();
	void addCollectable(XMFLOAT2);

	struct age
	{
		int years;
		int months;
	};
	age theAge;

private:
	int coins;
	int ciggs;
	int creams;
	

};