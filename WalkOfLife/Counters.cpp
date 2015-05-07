#include "Counters.h"

Counters::Counters()
{
	this->ciggs = 0;
	this->coins = 0;
	this->creams = 0;
}

void Counters::addCoin()
{
	this->coins = this->coins + 1;

}
void Counters::addCigg()
{
	this->ciggs = this->ciggs + 1;
}
void Counters::addCream()
{
	this->creams = this->creams + 1;
}

int Counters::getCoin()
{
	return this->coins;
}
int Counters::getCigg()
{
	return this->ciggs;
}
int Counters::getCream()
{
	return this->creams;
}

void Counters::setYear(int year)
{
	theAge.years = theAge.years + year;
}
void Counters::setMonth(int month)
{
	theAge.months = theAge.months + month;
	if (theAge.months == 12)
	{
		setYear(1);
		setMonth(-12);
	}
}