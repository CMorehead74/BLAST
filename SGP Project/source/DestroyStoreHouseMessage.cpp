#include "DestroyStoreHouseMessage.h"


CDestroyStoreHouseMessage::CDestroyStoreHouseMessage(CStoreHouse* pStoreHouse )
	: CMessage( MSG_DESTROY_STOREHOUSE)
{
	m_pStoreHouse = pStoreHouse;
}


CDestroyStoreHouseMessage::~CDestroyStoreHouseMessage(void)
{
}
