#include "Entity.h"

#include "Camera.h"

CEntity::CEntity(void)
{
	m_eType = OBJ_BASE;

	m_fRotation			= 0.0f;
	m_nHealth			= 100;
	m_fPosX				= 0.0f;
	m_fPosY				= 0.0f;
	m_nImageID			= -1;
	m_nHeight			= 64;
	m_nWidth			= 64;
	m_unReferenceCount	= 1;
}

CEntity::~CEntity(void)
{
}

void CEntity::SetHealth(int nHP)
{
	if(nHP <= 0)
		m_nHealth = 0;
	else
		m_nHealth = nHP;
}

// IEntity Interface Functions
void CEntity::Update( float fElapsedTime )
{
}

void CEntity::Render()
{
}

void CEntity::MiniMapRender()
{
}

RECT CEntity::GetRect() const
{
	RECT rTemp;

	rTemp.left	= (LONG)(GetPosX() - GetWidth()  * 0.5f) - (LONG)CCamera::GetInstance()->GetX();
	rTemp.top	= (LONG)(GetPosY() - GetHeight() * 0.5f) - (LONG)CCamera::GetInstance()->GetY();
	rTemp.right	= (LONG)(GetPosX() + GetWidth()  * 0.5f) - (LONG)CCamera::GetInstance()->GetX();
	rTemp.bottom= (LONG)(GetPosY() + GetHeight() * 0.5f) - (LONG)CCamera::GetInstance()->GetY();

	/*if(this->GetType() == OBJ_HELICOPTER)
	{
		rTemp.left	= (LONG)GetPosX();
		rTemp.top	= (LONG)(GetPosY()	+ 79);
		rTemp.right	= (LONG)(rTemp.left	+ 224);
		rTemp.bottom= (LONG)(rTemp.top	+ 35);
	}*/

	return rTemp;
}

bool CEntity::CheckCollision( IEntity* pOther )
{
	RECT rSelf = GetRect(), rOther = pOther->GetRect(), rOverlap;
	if(IntersectRect(&rOverlap, &rSelf, &rOther))
		return true;
	else
		return false;
	//return bColliding;
}

void CEntity::Release( void )
{
	// decrement the reference count 
	--m_unReferenceCount;

	// delete yourself if no one has a reference of you
	if( m_unReferenceCount == 0 )
		delete this;
}