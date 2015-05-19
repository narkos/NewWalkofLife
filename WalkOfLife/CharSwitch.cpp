#include "CharSwitch.h"

void CharSwitch::switchCharState(int charstate)
{
	if (charstate == 0)
	{
		
	}
	else if (charstate == 1)
	{
		//delete current charater model.
		//move new character to current position.
		//theCharacter->setVertexbuffer(vBuffer2);
		//theCustomImporter.GetPlayers()[0]->setVertexbuffer(vBuffer2);

	}
	else if (charstate == 2)
	{
		//theCharacter->setVertexbuffer(vBuffer3);
	}
}

int CharSwitch::getCharSate()
{
	return charstate;
}
void CharSwitch::setCharState(int set)
{
	charstate = set;
}