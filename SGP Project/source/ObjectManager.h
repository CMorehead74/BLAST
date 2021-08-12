/*
	File:				ObjectManager.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Naveen Anand Gunalan
	Last Edited:		9/14/2012
	Purpose:			Contain and create all of the game objects
						to render and update them all easily.
						Will also clean up the memory easily.
*/

#pragma once

#include "IEntity.h"
#include <cassert>
#include <list>
#include <map>
using std::map;

template <typename ClassID, typename BaseClass>
class CObjectManager
{
private:
	// Function Pointer Declaration
	typedef BaseClass* (*ObjectCreator)(void);

	// Database of function pointers to create the object
	map<ClassID, ObjectCreator> m_ObjectCreators;

	// Secure the Singleton!
	CObjectManager(void) { m_bShutdown = false; }
	~CObjectManager(void) { }
	CObjectManager(const CObjectManager&) { }
	CObjectManager& operator=(const CObjectManager&) { }

	bool	m_bShutdown;

	// Private function to create an object
	template<typename DerivedClass>
	static BaseClass* ConstructObject(void)
	{
		return new DerivedClass;
	}

	// Typedefs to make accessing the list easier
	typedef std::list<IEntity*>		ObjectList;
	typedef ObjectList::iterator	ObjListIter;

	ObjectList	m_lObjects;

public:
	// Singleton
	static CObjectManager<ClassID, BaseClass>* GetInstance(void)
	{
		static CObjectManager<ClassID, BaseClass> s_Instance;
		return &s_Instance;
	}

	//Accessor
	std::list<IEntity*> GetObjectList(void){ return m_lObjects; }

	// Build the map data of function pointers
	template<typename DerivedClass>
	bool RegisterClassType(ClassID id)
	{
		std::pair<ClassID, ObjectCreator> objTypeDef;

		objTypeDef.first	=	id;
		objTypeDef.second	=	ConstructObject<DerivedClass>;

		m_ObjectCreators.insert(objTypeDef);

		return true;
	}

	// Remove the information
	bool UnregisterClassType(ClassID id)
	{
		map<ClassID, ObjectCreator>::iterator iter = m_ObjectCreators.find(id);

		if(iter == m_ObjectCreators.end())
			return false;

		m_ObjectCreators.erase(iter);

		return true;
	}

	// Create an Object and add it to the list
	BaseClass* CreateObject(ClassID id)
	{
		map<ClassID, ObjectCreator>::iterator iter = m_ObjectCreators.find(id);

		if (iter == m_ObjectCreators.end())
			return NULL;

		BaseClass* ptr = (*iter).second();

		m_lObjects.push_back(ptr);
		ptr->AddRef();

		return ptr;
	}

	// Shutdown the Object Manager
	void Shutdown(void)
	{
		m_ObjectCreators.clear();

		// Call the Release method of each IEntity in our list
		for( ObjListIter iter = m_lObjects.begin(); iter != m_lObjects.end(); ++iter )
			(*iter)->Release();

		// Collapse the vector!
		m_lObjects.clear();

		m_bShutdown = true;
	}

	bool IsShutdown(void)
	{
		return m_bShutdown;
	}

	void SetShutdown(bool bShutdown)
	{
		m_bShutdown = bShutdown;
	}

	// Remove an object from the list
	void RemoveObject(IEntity* ptr)
	{
		assert(ptr != nullptr && "CObjectManager::RemoveObject -- Pointer to the object to remove was a nullptr.");

		ObjListIter iter = m_lObjects.begin();

		while(iter != m_lObjects.end())
		{
			if((*iter) == ptr)
			{
				iter = m_lObjects.erase(iter);
				ptr->Release();
				break;
			}
			else
				++iter;
	}
	}

	void UpdateObjects(float fElapsedTime)
	{
		for(ObjListIter iter = m_lObjects.begin(); iter != m_lObjects.end(); ++iter)
			(*iter)->Update(fElapsedTime);
	}

	void CheckCollisions(void)
	{
		for(ObjListIter iterI = m_lObjects.begin(); iterI != m_lObjects.end(); ++iterI)
		{
			for(ObjListIter iterJ = m_lObjects.begin(); iterJ != m_lObjects.end(); ++iterJ)
			{
				//if((*iterI)->GetType() != (*iterJ)->GetType())
				//{
					/*if(*/(*iterI)->CheckCollision(*iterJ);/*)*/
						//break;
				//}
			}
		}
	}

	void RenderObjects(void)
	{
		for(ObjListIter iter = m_lObjects.begin(); iter != m_lObjects.end(); ++iter)
			(*iter)->Render();
	}
};