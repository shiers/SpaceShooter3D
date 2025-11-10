#include <cstdio>
#include <string>
#include <cwchar>
#include <cstring> // added for strcpy_s

// Local Includes

// This Includes
#include "../include/Hud.h"
#include "../include/billboard.h"
#include "../include/highscore.h"

// Static Variables

// Static Function Prototypes

// Implementation
CHud::CHud()
 : m_puiScore(0)
   , m_pusLives(0)
   , m_fScreenWidth(0)
   , m_fScreenHeight(0)
   , m_ucNumberOfPlayers(0)
   , m_pFont(0)
   , m_arrayRects(0)
   , m_uiHighScore(0)
   , m_pHighScoreParser(0)
{
}

CHud::~CHud()
{
 if (m_pFont)
 {
  m_pFont->Release();
 }
 delete[] m_arrayRects;
 m_arrayRects = 0;

 delete m_pHighScoreParser;
 m_pHighScoreParser = 0;
}

bool
CHud::Initialise(UInt32 _fScreenWidth,
                 UInt32 _fScreenHeight,
                 UInt8 _ucNumPlayers,
                 UInt32 **_pArrayOfScores,
                 UInt8 **_pArrayOfLives,
                 IDirect3DDevice9 *_pDevice)
{
 m_ucNumberOfPlayers = _ucNumPlayers;

 m_puiScore = _pArrayOfScores;
 m_pusLives = _pArrayOfLives;

 m_pDevice = _pDevice;

 m_fScreenWidth = _fScreenWidth;
 m_fScreenHeight = _fScreenHeight;

 // Create High Score Parser and load high score from file...
 if (!m_pHighScoreParser)
 {
  m_pHighScoreParser = new CHighScore;
 }
 m_pHighScoreParser->LoadHighScores("highscore.tbl");
 std::string junk;

 m_pHighScoreParser->GetScore(0, m_uiHighScore, junk);

 if (!m_arrayRects)
 {
  m_arrayRects = new RECT[m_ucNumberOfPlayers];
 }
 for (UInt32 i = 0; i < m_ucNumberOfPlayers; ++i)
 {
  m_arrayRects[i].top = static_cast<LONG>((m_fScreenHeight / m_ucNumberOfPlayers) * i);
  m_arrayRects[i].bottom = static_cast<LONG>((m_fScreenHeight / m_ucNumberOfPlayers) + (m_fScreenHeight * 0.5f * i));
  m_arrayRects[i].left = static_cast<LONG>(0);
  m_arrayRects[i].right = static_cast<LONG>(m_fScreenWidth);
 }

 m_FontDescription.Height = static_cast<int>((_fScreenHeight) * 0.05f);
 m_FontDescription.Width = static_cast<int>(m_FontDescription.Height * 0.7f);
 m_FontDescription.Weight = 500;
 m_FontDescription.MipLevels = D3DX_DEFAULT;

 strcpy_s(m_FontDescription.FaceName, 32, "Arial");

 D3DXCreateFontIndirect(m_pDevice, &m_FontDescription, &m_pFont);

 return (true);
}

void
CHud::Draw()
{
 for (UInt32 i = 0; i < m_ucNumberOfPlayers; ++i)
 {
  const size_t maxSize = 256;
  wchar_t livesBuffer[256];
  swprintf_s(livesBuffer, maxSize, L"Lives: %u", static_cast<unsigned int>(*m_pusLives[i]));
  wchar_t scoreBuffer[256];
  swprintf_s(scoreBuffer, maxSize, L"Score: %u", static_cast<unsigned int>(*m_puiScore[i]));
  wchar_t highscoreBuffer[256];
  swprintf_s(highscoreBuffer, maxSize, L"High Score: %d", static_cast<int>(m_uiHighScore));

  m_pFont->DrawTextW(0,
                     livesBuffer,
                     -1,
                     &m_arrayRects[i],
                     DT_LEFT | DT_TOP,
                     D3DCOLOR_XRGB(255, 0, 0));
  m_pFont->DrawTextW(0,
                     scoreBuffer,
                     -1,
                     &m_arrayRects[i],
                     DT_TOP | DT_CENTER,
                     D3DCOLOR_XRGB(255, 0, 0));

  m_pFont->DrawTextW(0,
                     highscoreBuffer,
                     -1,
                     &m_arrayRects[i],
                     DT_BOTTOM | DT_LEFT,
                     D3DCOLOR_XRGB(255, 0, 0));
 }
}

void
CHud::SetHighScore(UInt32 _uiHighScore)
{
 m_uiHighScore = _uiHighScore;
}
