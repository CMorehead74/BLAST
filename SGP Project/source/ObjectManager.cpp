///*
//	File:				ObjectManager.cpp
//	Course:				SGP
//	Author:				Vortex Studios
//	Last Modified By:	Michael Yatckoske
//	Last Edited:		9/14/2012
//	Purpose:			Contain and create all of the game objects
//						to render and update them all easily.
//						Will also clean up the memory easily.
//*/
//
//#include "ObjectManager.h"
//#include <cassert>
//
////template <typename ClassID, typename BaseClass>
////CObjectManager<ClassID, BaseClass>::CObjectManager(void)
////{
////}
////
////template <typename ClassID, typename BaseClass>
////CObjectManager<ClassID, BaseClass>::~CObjectManager(void)
////{
////}
////
////template <typename ClassID, typename BaseClass>
////CObjectManager<ClassID, BaseClass>* CObjectManager<ClassID, BaseClass>::GetInstance(void)
////{
////	static CObjectManager<ClassID, BaseClass> s_Instance;
////	return &s_Instance;
////}
//
////template<typename ClassID, typename BaseClass>
////template<typename DerivedClass>
////bool CObjectManager<ClassID, BaseClass>::RegisterClassType(ClassID id)
////{
////	std::pair<ClassID, ObjectCreator> objTypeDef;
////
////	objTypeDef.first	=	id;
////	objTypeDef.second	=	ConstructObject<DerivedClass>;
////
////	m_ObjectCreators.insert(objTypeDef);
////
////	return true;
////}
//
//template<typename ClassID, typename BaseClass>
//bool CObjectManager<ClassID, BaseClass>::UnregisterClassType(ClassID id)
//{
//	map<ClassID, ObjectCreator>::iterator iter = m_ObjectCreators.find(id);
//
//	if(iter == m_ObjectCreators.end())
//		return false;
//
//	m_ObjectCreators.erase(iter);
//
//	return true;
//}
//
//template<typename ClassID, typename BaseClass>
//BaseClass* CObjectManager<ClassID, BaseClass>::CreateObject(ClassID id)
//{
//	map<ClassID, ObjectCreator>::iterator iter = m_ObjectCreators.find(id);
//
//	if (iter == m_ObjectCreators.end())
//			return NULL;
//
//	BaseClass* ptr = (*iter).second();
//
//	m_lObjects.push_back(ptr);
//	ptr->AddRef();
//
//	return ptr;
//}
//
//template<typename ClassID, typename BaseClass>
//void CObjectManager<ClassID, BaseClass>::RemoveObject(IEntity* ptr)
//{
//	assert(ptr != nullptr && "CObjectManager::RemoveObject -- Pointer to the object to remove was a nullptr.");
//
//	OListIterator iter = m_lObjects.begin();
//
//	while(iter != m_lObjects.end())
//	{
//		if((*iter) == ptr)
//		{
//			iter = m_lObjects.erase(iter);
//			ptr->Release();
//			break;
//		}
//		else
//			++iter;
//	}
//}
//
//template<typename ClassID, typename BaseClass>
//void CObjectManager<ClassID, BaseClass>::UpdateObjects(float fElapsedTime)
//{
//	for(OListIterator iter = m_lObjects.begin(); iter != m_lObjects.end(); ++iter)
//		(*iter)->Update(fElapsedTime);
//}
//
//template<typename ClassID, typename BaseClass>
//void CObjectManager<ClassID, BaseClass>::RenderObjects(void)
//{
//	for(OListIterator iter = m_lObjects.begin(); iter != m_lObjects.end(); ++iter)
//		(*iter)->Render();
//}
//
//template<typename ClassID, typename BaseClass>
//void CObjectManager<ClassID, BaseClass>::CheckCollisions(void)
//{
//
//}
//
//template<typename ClassID, typename BaseClass>
//void CObjectManager<ClassID, BaseClass>::Shutdown(void)
//{
//	m_ObjectCreators.clear();
//
//	// Call the Release method of each IEntity in our list
//	for( OListIterator iter = m_lObjects.begin(); iter != m_lObjects.end(); ++iter )
//		(*iter)->Release();
//
//	// Collapse the vector!
//	m_lObjects.clear();
//}