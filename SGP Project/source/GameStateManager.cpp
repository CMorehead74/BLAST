#include "GameStateManager.h"


/*static*/ CGameStateManager* CGameStateManager::GetInstance( void )
{
	// Local static variable:
	//	- stored in global memory
	//	- there can be only one!
	static CGameStateManager s_Instance;

	return &s_Instance;
}

CGameStateManager::CGameStateManager(void)
{
}

CGameStateManager::~CGameStateManager(void)
{
	m_vGameStateStack.clear();
}

void CGameStateManager::NextState(IGameState* pState)
{
		m_vGameStateStack.push_back(pState);
		m_pCurrState = pState;
		pState->Enter();
}

void CGameStateManager::PreviousState(void)
{
	m_pCurrState->Exit();
	m_vGameStateStack.pop_back();
	if(m_vGameStateStack.size())
		m_pCurrState = m_vGameStateStack[m_vGameStateStack.size()-1];
}

// Game state machine access:
void CGameStateManager::ChangeState( IGameState* pNextState )
{
	// Clean up the existing state (if any)
	if(pNextState != nullptr)
	{
		if( m_pCurrState != nullptr )
		{
			m_pCurrState->Exit();
			m_vGameStateStack.pop_back();
		}

		// Store the new state	
		NextState(pNextState);
	}
	else
	{
		m_pCurrState->Exit();
		m_vGameStateStack.pop_back();
	}
}

bool CGameStateManager::Input(void)
{
	return m_pCurrState->Input();
}


void CGameStateManager::Update(float fElapsedTime)
{	
	m_pCurrState->Update(fElapsedTime);
}

void CGameStateManager::Render(void)
{
	for(unsigned int count=0; count<m_vGameStateStack.size(); count++)
		m_vGameStateStack[count]->Render();
}