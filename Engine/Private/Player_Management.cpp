
#include "..\Public\Player_Management.h"
#include "../../Reference/Headers/Player_Typedef.h"
IMPLEMENT_SINGLETON(CPlayer_Management)

CPlayer_Management::CPlayer_Management()
	:m_iPlayerKind(CPlayer_Typedef::PLAYER_KIND::PLAYER_SPONGEBOB)
{
}


void CPlayer_Management::ManageMent()
{
}

void CPlayer_Management::Free()
{

}
