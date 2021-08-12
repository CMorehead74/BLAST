#pragma once
#include "IEntity.h"

class CEntity : public IEntity
{
public:
	enum ObjectType { OBJ_BASE, OBJ_PLAYER, OBJ_ENEMY, OBJ_ALLY, OBJ_BULLET, OBJ_HELICOPTER, OBJ_BUILDING, OBJ_PICKUP, OBJ_BUILDBASE, OBJ_BUNKER, OBJ_STOREHOUSE, OBJ_MINE, OBJ_SHOP };

	CEntity(void);
	virtual ~CEntity(void);

	// Mutators
	void SetPosX( float fX )		{ m_fPosX = fX; }
	void SetPosY( float fY	)		{ m_fPosY = fY; }
	void SetWidth( int nW )			{ m_nWidth = nW; }
	void SetHeight( int nH )		{ m_nHeight = nH; }
	void SetImageID( int nImgID)	{ m_nImageID = nImgID; }
	void SetHealth(int nHP);
	

	//Accessors
	float GetPosX()		const	{ return m_fPosX; }
	float GetPosY()		const	{ return m_fPosY; }
	int GetWidth()		const	{ return m_nWidth; }
	int GetHeight()		const	{ return m_nHeight; }
	int GetImageID()	const	{ return m_nImageID; }
	int GetHealth()		const	{ return m_nHealth; }

	virtual void SetAnimationID(int nAnimationID)				{m_nAnimationID = nAnimationID; }
	virtual int  GetAnimationID()								{ return m_nAnimationID; }

	virtual RECT GetRect() const;
	virtual int  GetType() const { return m_eType; }

	void AddRef() { m_unReferenceCount++; }
	void Release();

	// IEntity Interface Functions
	virtual void Update( float fElapsedTime );
	virtual void Render();
	virtual bool CheckCollision( IEntity* pOther );

	virtual void MiniMapRender();

protected:
	ObjectType m_eType;

private:
	// Data Members
	float				m_fRotation;
	int		m_nHealth;
	
	//entity position
	float				m_fPosX;
	float				m_fPosY;

	//image
	int					m_nImageID;
	int					m_nHeight;
	int					m_nWidth;

			
	int					m_nAnimationID;
	// Reference Count
	unsigned int		m_unReferenceCount;
};