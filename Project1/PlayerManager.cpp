#include "PlayerManager.h"



PlayerManager::PlayerManager()
{
}

PlayerManager::PlayerManager(unsigned int uiPlayerCount)
	:m_vOtherPlayers(uiPlayerCount - 1)
{
}

PlayerManager::~PlayerManager()
{
}
