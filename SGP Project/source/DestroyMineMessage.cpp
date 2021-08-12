#include "DestroyMineMessage.h"


CDestroyMineMessage::CDestroyMineMessage(CMine* pMine)
	: CMessage( MSG_DESTROY_MINE )
{
	m_pMine = pMine;
}


CDestroyMineMessage::~CDestroyMineMessage(void)
{
}
