/*
	File:				HighScoreState.cpp
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/24/2012
	Purpose:			Displays the high scores list to the player
*/

#include "HighScoreState.h"
#include "GamePlayState.h"
#include "Game.h"
#include "../tinyxml/tinyxml.h"
#include "GameStateManager.h"

// Singleton
CHighScoreState* CHighScoreState::GetInstance(void)
{
	static CHighScoreState s_Instance;
	return &s_Instance;
}

CHighScoreState::CHighScoreState(void)
{
	// Assign data members to clean values
	m_pD3D	= nullptr;
	m_pTM	= nullptr;
	m_pDI	= nullptr;
	m_pXA	= nullptr;
	m_pFont	= nullptr;

	// Image IDs:
	m_nBackgroundImgID	= -1;
	
	// Audio IDs:
}

void CHighScoreState::Enter(void)
{
	// Access the singletons of the SGD Wrappers
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pTM  = CSGD_TextureManager::GetInstance();
	m_pDI  = CSGD_DirectInput::GetInstance();
	m_pXA  = CSGD_XAudio2::GetInstance();

	m_pFont	= CBitmapFont::GetInstance();

	// Load Images
	m_nBackgroundImgID	= m_pTM->LoadTexture(_T("resource/graphics/instructions.png"), D3DCOLOR_XRGB(0,0,0));
	m_nMenuImgID		= m_pTM->LoadTexture(_T("resource/graphics/Blast_Menu_Text_Bg.png"));

	// Load Bitmap Image
	m_nBitmapFontImgID		= m_pTM->LoadTexture(_T("resource/graphics/SansationFont_0.png"), D3DCOLOR_XRGB(0,0,0));
	m_pFont->SetImageID(m_nBitmapFontImgID);

	// Load Audio

	// Load Scores
	if(LoadScores(m_vScores, "highscores.xml") == false)
		GenerateScores(m_vScores);
}

void CHighScoreState::Exit(void)
{
	// Unload Images
	if(m_nBackgroundImgID != -1)
	{
		m_pTM->UnloadTexture(m_nBackgroundImgID);
		m_nBackgroundImgID = -1;
	}
	if(m_nMenuImgID != -1)
	{
		m_pTM->UnloadTexture(m_nMenuImgID);
		m_nMenuImgID = -1;
	}

	// Unload Audio

	// Save Scores
	SaveScores(m_vScores, "highscores.xml");
}

bool CHighScoreState::Input(void)
{
	if(m_pDI->KeyPressed(DIK_ESCAPE) || m_pDI->JoystickButtonPressed(6) )
	{
		CGameStateManager::GetInstance()->PreviousState();
		//CGameStateManager::GetInstance()->ChangeState(CMainMenuState::GetInstance());
		return true;
	}
	return true;
}

void CHighScoreState::Update(float fElapsedTime)
{

}

void CHighScoreState::Render(void)
{
	CGame* pGame = CGame::GetInstance();

	RECT rSource = {0,0,800,600};
	// Render the background
	m_pTM->Draw(m_nBackgroundImgID,0,0,1.0f,1.0f,&rSource,0.0f,0.0f,0.0f);

	// Render the Text Background
	SetRect(&rSource, 0, 0, 256, 450);
	m_pTM->Draw(m_nMenuImgID, pGame->GetWidth()/2 - 130, 30, 1.25f, 1.25f, &rSource);

	ostringstream output;

	m_pFont->PrintString("High Scores", pGame->GetWidth()/2 - 50, 50, 1.0f, 255, 255, 255, 255);

	for(int i = 0; i < 10; ++i)
	{
		output << i + 1;// << m_vScores[i].szName << "      " << m_vScores[i].nScore;
		m_pFont->PrintString(output.str().c_str(), pGame->GetWidth()/2 - 100, 100 + i*30, 1.0f, 255, 255, 255, 255);
		output.str("");

		output << m_vScores[i].szName;
		m_pFont->PrintString(output.str().c_str(), pGame->GetWidth()/2 - 25, 100 + i*30, 1.0f, 255, 255, 0, 0);
		output.str("");

		output << m_vScores[i].nScore;
		m_pFont->PrintString(output.str().c_str(), pGame->GetWidth()/2 + 80, 100 + i*30, 1.0f, 255, 0, 0, 255);
		output.str("");
	}
}

CHighScoreState::~CHighScoreState(void)
{
}

void CHighScoreState::GenerateScores(vector<tScores>& vScores)
{
	// Generate High Scores
	tScores p1	= { 1000,	"SGP" };
	tScores p2	= { 900,	"SGD" };
	tScores p3	= { 800,	"ROG" };
	tScores p4	= { 700,	"DRX" };
	tScores p5	= { 600,	"SWA" };
	tScores p6	= { 500,	"LNA" };
	tScores p7	= { 400,	"PG3" };
	tScores p8	= { 300,	"PG2" };
	tScores p9	= { 200,	"PG1" };
	tScores p10	= { 100,	"DSF" };

	// Clear out Existing Data
	vScores.clear();

	// Push the new players onto the end
	vScores.push_back(p1);
	vScores.push_back(p2);
	vScores.push_back(p3);
	vScores.push_back(p4);
	vScores.push_back(p5);
	vScores.push_back(p6);
	vScores.push_back(p7);
	vScores.push_back(p8);
	vScores.push_back(p9);
	vScores.push_back(p10);
}

bool CHighScoreState::LoadScores(vector<tScores>& vScores, const char* szFileName)
{
	// Create a TinyXML Document
	TiXmlDocument doc;

	// Attempt to load the file
	if(doc.LoadFile(szFileName) == false)
		return false;

	// Access the root node ("scores_list")
	TiXmlElement* pRoot = doc.RootElement();
	if(pRoot == nullptr)
		return false;

	// Clear the existing scores (if any)
	vScores.clear();

	// Iterate through the nodes to load scores
	TiXmlElement* pScore = pRoot->FirstChildElement("score_info");

	while(pScore != nullptr)
	{
		// Score Struct
		tScores info = { };

		// Read the Name
		const char* pText = pScore->GetText();
		if(pText != nullptr)
			strcpy_s(info.szName, 32, pText);
		else
			strcpy_s(info.szName, 32, "EMPTY");

		// Read the Score
		pScore->Attribute("score", &info.nScore);

		// Save the info into the vector
		vScores.push_back(info);

		// Move to the next "score_info" node
		pScore = pScore->NextSiblingElement("score_info");
	}

	return true;
}

void CHighScoreState::SaveScores(const vector<tScores>& vScores, const char* szFileName)
{
	// Create a TinyXML Document
	TiXmlDocument doc;

	// Create the TinyXML declaration (version info)
	TiXmlDeclaration* pDec = new TiXmlDeclaration("1.0", "utf-8", "");

	// Write the declaration to the end of the document
	doc.LinkEndChild(pDec);

	// Create a root node for the "players_list"
	TiXmlElement* pRoot = new TiXmlElement("scores_list");

	// Add the node to the document
	doc.LinkEndChild(pRoot);

	// Iterate through the vector,
	// saving each score's info
	vector<tScores>::const_iterator iter;
	for(iter = vScores.begin(); iter != vScores.end(); ++iter)
	{
		// Create a node for this score
		TiXmlElement* pScore = new TiXmlElement("score_info");

		// Fill the node with score data
		pScore->SetAttribute("score", (*iter).nScore);

		// Create a Text node for the name
		TiXmlText* pText = new TiXmlText((*iter).szName);
		pScore->LinkEndChild(pText);

		// Write the score node to the root "scores_list"
		pRoot->LinkEndChild(pScore);
	}

	// Save the document to the file
	doc.SaveFile(szFileName);
}