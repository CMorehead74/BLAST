/*
	File:						BitmapFont.h
	Course:					SGP
	Author:					Vortex Studios
	Last Modified By:	Schadrac Francois
	Last Edited:			9/14/2012
	Purpose:				display text using a bitmap font
*/

#include <vector>

using namespace std;

#pragma once
class CBitmapFont
{
private:
	struct Characters
	{
		int				m_sCharID;
		int				m_nLeft;			// X in the xml file
		int				m_nTop;			// Y in the xml file
		int				m_nWidth;			// Width in the xml file
		int				m_nHeight;		// Height in the xml file

		//offset for each letter
		int				m_nXOffset;
		int				m_nYOffset;

		//how much to advance and write again
		int				m_nXAdvance;
	};

	struct KerningInfo
	{
		int				m_nFirst;
		int				m_nSecond;
		int				m_nAmount;
	};

	//container for all the characters in the string
	vector <Characters > m_vCharacterContainer;

	//holds kering info
	vector <KerningInfo* > m_vKerningInfoContainer;

	//font specific information
	const char*			m_szFontName;
	int					m_nFontSize;
	int					m_nImageID;

	//img ID
	const char*			m_szImageName;

	CBitmapFont(void);
	~CBitmapFont(void);

public:
	static CBitmapFont* GetInstance();

	void PrintString( const char* szString, int nX, int nY, float fScale = 1.0f, short nAlpha = 255, short nRed = 255, short nGreen = 255, short nBlue = 255 );

	bool LoadFont( const char* szFileName );

	int	GetImageID(void) const { return m_nImageID; }
	void SetImageID(int id) { m_nImageID = id; }
};

