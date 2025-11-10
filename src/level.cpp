#include "../include/INIParser.h"
#include "../include/EntityManager.h"
#include "../include/Player.h"
#include "../include/Enemy.h"
#include "../include/Explosion.h"
#include "../include/Bullet.h"
#include "../include/Shield.h"
#include "../include/PowerUp.h"
#include "../include/Hud.h"
#include "../include/PointLight.h"
#include "../include/DirectionalLight.h"
#include "../include/topdowncamera.h"
#include "../include/BackDrop.h"
#include "../include/SeekerEnemy.h"
#include "../include/RandomEnemy.h"
#include "../include/ShooterEnemy.h"
#include "../include/D3DRenderer.h"
#include "../include/game.h"
#include "../include/highscore.h"

// This Includes
#include "../include/level.h"

// Static Variables

// Static Function Prototypes

// Implementation
CLevel::CLevel()
	: m_pIniParser(0)
	  , m_fPlayAreaWidth(0.0f)
	  , m_fPlayAreaDepth(0.0f)
	  , m_fSafeZoneRadius(0.0f)
	  , m_ucNumOfPlayers(0)
	  , m_arrayVec3PlayerStartPos(0)
	  , m_fNumSeekerEnemies(0.0f)
	  , m_fNumShooterEnemies(0.0f)
	  , m_fNumRandomEnemies(0.0f)
	  , m_fShieldPowerUpTime(0.0f)
	  , m_fBulletPowerUpTime(0.0f)
	  , m_ucNumberOfLives(0)
	  , m_ucBulletNormalDamage(0)
	  , m_ucBulletPowerUpDamage(0)
	  , m_ucSeekerEnemyLife(0)
	  , m_ucRandomEnemyLife(0)
	  , m_ucShooterEnemyLife(0)
	  , m_usSeekerEnemyScoreValue(0)
	  , m_usRandomEnemyScoreValue(0)
	  , m_usShooterEnemyScoreValue(0)
	  , m_usRandomReloadTime(0)
	  , m_fEnemyReloadTime(0.0f)
	  , m_fEnemySpawnMultiplier(0.0f)
	  , m_ucPowerUpDropRate(0)
	  , m_ucBulletSpawn(0)
	  , m_ucShieldSpawn(0)
	  , m_ucSpeedSpawn(0)
	  , m_fPowerUpSpeed(0.0f)
	  , m_arraypPlayers(0)
	  , m_fCameraHeight(0.0f)
	  , m_pRenderer(0)
	  , m_fPlayerSpeed(0)
	  , m_pBackDrop(0)
	  , m_fPlayerFriction(0)
	  , m_fPlayerTurnSpeed(0)
	  , m_arraypPlayerBullets(0)
	  , m_arraypEnemyBullets(0)
	  , m_fPlayerReloadTime(0.0f)
	  , m_fSeekerSpeed(0.0f)
	  , m_fSeekerTurnSpeed(0.0f)
	  , m_fRandomSpeed(0.0f)
	  , m_fRandomTurnSpeed(0.0f)
	  , m_fShooterSpeed(0.0f)
	  , m_fShooterTurnSpeed(0.0f)
	  , m_fPrevNumSeekers(0.0f)
	  , m_fPrevNumShooters(0.0f)
	  , m_fPrevNumRandoms(0.0f)
	  , m_arraypPlayerExplosions(0)
	  , m_arraypPointLights(0)
	  , m_pDirectionLight(0)
	  , m_uiVectorIndex(0)
	  , m_pHud(0)
	  , m_pArrayLives(0)
	  , m_pArrayScores(0)
	  , m_arraypPowerUps(0)
	  , m_pHighScoreParser(0)
	  , m_bHighScoreAdded(false)
	  , m_arraypShields(0)
{
	ZeroMemory(&m_rectBoundary, sizeof(RECT));
}

CLevel::~CLevel()
{
	delete m_pIniParser;
	m_pIniParser = 0;

	delete[] m_arrayVec3PlayerStartPos;
	m_arrayVec3PlayerStartPos = 0;

	delete[] m_arraypPlayers;
	m_arraypPlayers = 0;

	delete m_pBackDrop;
	m_pBackDrop = 0;

	delete[] m_arraypPlayerBullets;
	m_arraypPlayerBullets = 0;

	delete[] m_arraypEnemyBullets;
	m_arraypEnemyBullets = 0;

	delete[] m_arraypPlayerExplosions;
	m_arraypPlayerExplosions = 0;

	delete[] m_arraypShields;
	m_arraypShields = 0;

	for (UInt32 i = 0; i < m_vecpEnemies.size(); ++i)
	{
		delete m_vecpEnemies[i];
		m_vecpEnemies[i] = 0;
	}
	m_vecpEnemies.clear();

	for (UInt32 i = 0; i < m_vecpEnemyExplosions.size(); ++i)
	{
		delete m_vecpEnemyExplosions[i];
		m_vecpEnemyExplosions[i] = 0;
	}
	m_vecpEnemyExplosions.clear();

	delete[] m_arraypPointLights;
	m_arraypPointLights = 0;

	delete m_pDirectionLight;
	m_pDirectionLight = 0;

	delete m_pHud;
	m_pHud = 0;

	delete[] m_pArrayLives;
	m_pArrayLives = 0;

	delete[] m_pArrayScores;
	m_pArrayScores = 0;

	delete[] m_arraypPowerUps;
	m_arraypPowerUps = 0;

	delete m_pHighScoreParser;
	m_pHighScoreParser = 0;
}

/**
*
* This function initialises the level.
*
* @param _ucNumOfPlayers: the number of players and cameras in the game.
* @param _arraypCameras: pointer to an array of cameras.
* @param _pRenderer: point to a D3DRenderer.
* @return Returns true if successful.
*
*/
bool
CLevel::Initialise(UInt8 _ucNumOfPlayers,
                   CTopDownCamera **_arraypCameras,
                   CD3DRenderer *_pRenderer)
{
	bool bFailure = false;

	m_ucNumOfPlayers = _ucNumOfPlayers;
	m_pRenderer = _pRenderer;

	// Creating the player(s).
	if (!m_arraypPlayers)
	{
		m_arraypPlayers = new CPlayer[m_ucNumOfPlayers];
	}
	if (!m_arrayVec3PlayerStartPos)
	{
		m_arrayVec3PlayerStartPos = new D3DXVECTOR3[m_ucNumOfPlayers];
		ZeroMemory(m_arrayVec3PlayerStartPos, sizeof(D3DXVECTOR3)
		           * m_ucNumOfPlayers);
	}

	// INI Parser
	LoadVariables();

	// Creating the player(s).
	InitialisePlayers(_arraypCameras);

	// Creates the Back drop.
	if (!m_pBackDrop)
	{
		m_pBackDrop = new CBackDrop;
	}

	bFailure = !m_pBackDrop->Initialise(L"media\\plane.x",
	                                    L"media\\tile.png",
	                                    _pRenderer,
	                                    m_fPlayAreaWidth,
	                                    m_fPlayAreaDepth);

	// Creating the Players bullets.

	InitialiseBullets();

	InitialiseEnemies();

	InitialiseExplosions();

	InitialisePowerUps();

	// Initialise the directional light.
	if (!m_pDirectionLight)
	{
		m_pDirectionLight = new CDirectionalLight;
	}

	m_pDirectionLight->Initialise(_pRenderer);

	// Initialise point lights.
	if (!m_arraypPointLights)
	{
		m_arraypPointLights = new CPointLight[m_ucNumOfPlayers];
	}

	for (UInt8 i = 0; i < m_ucNumOfPlayers; ++i)
	{
		m_arraypPointLights[i].Initialise(_pRenderer,
		                                  m_arraypPlayers[i].GetPosition(),
		                                  m_fSafeZoneRadius,
		                                  i);
	}

	if (!m_pHud)
	{
		m_pHud = new CHud;
		if (!m_pArrayLives)
		{
			m_pArrayLives = new UInt8 *[m_ucNumOfPlayers];
		}
		if (!m_pArrayScores)
		{
			m_pArrayScores = new UInt32 *[m_ucNumOfPlayers];
		}
		for (UInt8 i = 0; i < m_ucNumOfPlayers; ++i)
		{
			m_pArrayLives[i] = m_arraypPlayers[i].GetLives();
			m_pArrayScores[i] = m_arraypPlayers[i].GetScore();
		}
		m_pHud->Initialise(CGame::GetInstance().GetWindowWidth(),
		                   CGame::GetInstance().GetWindowHeight(),
		                   m_ucNumOfPlayers,
		                   m_pArrayScores,
		                   m_pArrayLives,
		                   m_pRenderer->GetDevice());
	}

	InitialiseShields();

	return (!bFailure);
}

/**
*
* This function is resposible for doing any processing that the Level requires.
*
* @param _fDeltaTick is the amount of time elapsed between frames.
* @return Returns void.
*
*/
void
CLevel::Process(Float32 _fDeltaTick)
{
	ProcessPlayers(_fDeltaTick);

	m_pBackDrop->Process(_fDeltaTick);

	ProcessEnemies(_fDeltaTick);
	ProcessHUD(_fDeltaTick);
	ProcessPowerUps(_fDeltaTick);
	ProcessBullets(_fDeltaTick);
	ProcessExplosions(_fDeltaTick);
	ProcessLights(_fDeltaTick);
	ProcessShields(_fDeltaTick);
}

/**
*
* This function is resposible for drawing level objects.
*
* @param
* @return Returns void.
*
*/
void
CLevel::Draw()
{
	for (UInt8 i = 0; i < m_ucNumOfPlayers; ++i)
	{
		m_arraypPlayers[i].Draw();
		m_arraypPlayerExplosions[i].Draw();
	}

	m_pBackDrop->Draw();

	for (UInt8 i = 0; i < m_ucNumOfPlayerBullets; ++i)
	{
		if (m_arraypPlayerBullets[i].IsBulletActive())
		{
			m_arraypPlayerBullets[i].Draw();
		}
	}

	for (UInt8 i = 0; i < m_ucNumOfEnemyBullets; ++i)
	{
		if (m_arraypEnemyBullets[i].IsBulletActive())
		{
			m_arraypEnemyBullets[i].Draw();
		}
	}

	for (UInt8 i = 0; i < m_vecpEnemies.size(); ++i)
	{
		m_vecpEnemies[i]->Draw();
		m_arraypPowerUps[i].Draw();
	}

	for (UInt32 i = 0; i < m_vecpEnemyExplosions.size(); ++i)
	{
		m_vecpEnemyExplosions[i]->Draw();
	}

	m_pHud->Draw();

	for (UInt32 i = 0; i < m_ucNumOfPlayers; ++i)
	{
		m_arraypShields[i].Draw();
	}
}

/**
*
* This function is resposible for load variables for data driving.
*
* @param
* @return Returns void.
*
*/
void
CLevel::LoadVariables()
{
	if (!m_arrayVec3PlayerStartPos)
	{
		m_arrayVec3PlayerStartPos = new D3DXVECTOR3[m_ucNumOfPlayers];
		ZeroMemory(m_arrayVec3PlayerStartPos, sizeof(D3DXVECTOR3) * m_ucNumOfPlayers);
	}

	// Create INI Parser and Load variables from file...
	if (!m_pIniParser)
	{
		m_pIniParser = new CINIParser;
	}
	m_pIniParser->LoadIniFile("config\\level.ini");

	m_pIniParser->GetFloatValue("level", "playareawidth", m_fPlayAreaWidth);
	m_pIniParser->GetFloatValue("level", "playareadepth", m_fPlayAreaDepth);
	m_rectBoundary.left -= static_cast<LONG>(m_fPlayAreaWidth / 2);
	m_rectBoundary.right += static_cast<LONG>(m_fPlayAreaWidth / 2);
	m_rectBoundary.top += static_cast<LONG>(m_fPlayAreaDepth / 2);
	m_rectBoundary.bottom -= static_cast<LONG>(m_fPlayAreaDepth / 2);

	m_pIniParser->GetFloatValue("level", "safezoneradius", m_fSafeZoneRadius);
	m_pIniParser->GetFloatValue("level", "cameraheight", m_fCameraHeight);

	m_pIniParser->GetFloatValue("player", "shieldpoweruptime", m_fShieldPowerUpTime);
	m_pIniParser->GetIntValue("player", "playerstartinglives", m_ucNumberOfLives);
	m_pIniParser->GetFloatValue("player", "bulletpoweruptime", m_fBulletPowerUpTime);
	m_pIniParser->GetFloatValue("player", "playerspeed", m_fPlayerSpeed);

	m_pIniParser->GetFloatValue("player", "player1startpositionx",
	                            m_arrayVec3PlayerStartPos[0].x);
	m_pIniParser->GetFloatValue("player", "player1startpositiony",
	                            m_arrayVec3PlayerStartPos[0].y);
	m_pIniParser->GetFloatValue("player", "player1startpositionz",
	                            m_arrayVec3PlayerStartPos[0].z);
	m_pIniParser->GetFloatValue("player", "playerfriction", m_fPlayerFriction);
	m_pIniParser->GetFloatValue("player", "playerturnspeed", m_fPlayerTurnSpeed);
	m_pIniParser->GetFloatValue("player", "playerreloadtime", m_fPlayerReloadTime);

	if (m_ucNumOfPlayers > 1)
	{
		m_pIniParser->GetFloatValue("player", "player2startpositionx",
		                            m_arrayVec3PlayerStartPos[1].x);
		m_pIniParser->GetFloatValue("player", "player2startpositiony",
		                            m_arrayVec3PlayerStartPos[1].y);
		m_pIniParser->GetFloatValue("player", "player2startpositionz",
		                            m_arrayVec3PlayerStartPos[1].z);
	}

	m_pIniParser->GetIntValue("bullet", "normaldamage", m_ucBulletNormalDamage);
	m_pIniParser->GetIntValue("bullet", "powerupdamage",
	                          m_ucBulletPowerUpDamage);
	m_pIniParser->GetIntValue("bullet", "playerbullets",
	                          m_ucNumOfPlayerBullets);
	m_pIniParser->GetIntValue("bullet", "enemybullets", m_ucNumOfEnemyBullets);
	m_pIniParser->GetFloatValue("bullet", "bulletspeed", m_fBulletSpeed);

	// Create the number of bullets based on the numnber of players.
	m_ucNumOfPlayerBullets *= m_ucNumOfPlayers;

	m_pIniParser->GetFloatValue("enemies", "numseekerenemies",
	                            m_fNumSeekerEnemies);
	m_pIniParser->GetFloatValue("enemies", "numrandomenemies",
	                            m_fNumRandomEnemies);
	m_pIniParser->GetFloatValue("enemies", "numshooterenemies",
	                            m_fNumShooterEnemies);
	m_pIniParser->GetIntValue("enemies", "seekerenemylife",
	                          m_ucSeekerEnemyLife);
	m_pIniParser->GetIntValue("enemies", "randomenemylife",
	                          m_ucRandomEnemyLife);
	m_pIniParser->GetIntValue("enemies", "shooterenemylife",
	                          m_ucShooterEnemyLife);
	m_pIniParser->GetIntValue("enemies", "seekerscorevalue",
	                          m_usSeekerEnemyScoreValue);
	m_pIniParser->GetIntValue("enemies", "randomscorevalue",
	                          m_usRandomEnemyScoreValue);
	m_pIniParser->GetIntValue("enemies", "shooterscorevalue",
	                          m_usShooterEnemyScoreValue);
	m_pIniParser->GetIntValue("enemies", "randomreloadtime",
	                          m_usRandomReloadTime);
	m_pIniParser->GetFloatValue("enemies", "enemyreloadtime",
	                            m_fEnemyReloadTime);
	m_pIniParser->GetFloatValue("enemies", "enemymultiplier",
	                            m_fEnemySpawnMultiplier);
	m_pIniParser->GetFloatValue("enemies", "seekerspeed",
	                            m_fSeekerSpeed);
	m_pIniParser->GetFloatValue("enemies", "seekerturnspeed",
	                            m_fSeekerTurnSpeed);
	m_pIniParser->GetFloatValue("enemies", "randomspeed",
	                            m_fRandomSpeed);
	m_pIniParser->GetFloatValue("enemies", "randomturnspeed",
	                            m_fRandomTurnSpeed);
	m_pIniParser->GetFloatValue("enemies", "shooterspeed", m_fShooterSpeed);
	m_pIniParser->GetFloatValue("enemies", "shooterturnspeed",
	                            m_fShooterTurnSpeed);

	m_pIniParser->GetIntValue("powerup", "powerupdroprate", m_ucPowerUpDropRate);
	m_pIniParser->GetFloatValue("powerup", "powerupspeed", m_fPowerUpSpeed);
	m_pIniParser->GetIntValue("powerup", "bulletspawn", m_ucBulletSpawn);
	m_pIniParser->GetIntValue("powerup", "shieldspawn", m_ucShieldSpawn);
	m_pIniParser->GetIntValue("powerup", "smartbombspawn", m_ucSpeedSpawn);

	// Create High Score Parser and load high score from file...
	if (!m_pHighScoreParser)
	{
		m_pHighScoreParser = new CHighScore;
	}
	m_pHighScoreParser->LoadHighScores("highscore.tbl");
}

/**
*
* This function calls process on all the player objects.
*
* @param _fDeltaTick is the amount of time elapsed between frames.
* @return Returns void.
*
*/
void
CLevel::ProcessPlayers(Float32 _fDeltaTick)
{
	bool bAllPlayersDead = true;

	for (UInt8 i = 0; i < m_ucNumOfPlayers; ++i)
	{
		m_arraypPlayers[i].Process(_fDeltaTick);

		if (*m_arraypPlayers[i].GetLives() > 0)
		{
			bAllPlayersDead = false;
		}

		if (m_arraypPlayers[i].IsActive())
		{
			if (m_arraypPlayers[i].IsFiring())
			{
				for (UInt8 j = 0; j < m_ucNumOfPlayerBullets; ++j)
				{
					if (m_arraypPlayerBullets[j].IsBulletActive() == false)
					{
						m_arraypPlayerBullets[j].SetBulletActivity(true);
						m_arraypPlayerBullets[j].SetXYZDirection
						(m_arraypPlayers[i].GetPosition(),
						 m_arraypPlayers[i].GetFiringDirection());

						break;
					}
				}
			}
			for (UInt32 j = 0; j < m_vecpEnemies.size(); ++j)
			{
				if (m_vecpEnemies[j]->IsActive())
				{
					if (!m_arraypShields[i].IsAlive())
					{
						if (CDynamicEntity::IsCollision(&m_arraypPlayers[i], m_vecpEnemies[j]))
						{
							DestroyPlayer(i);
							DestroyEnemy(j);
						}
					}
				}
			}
		} else
		{
			//Player is dead.
			if (*m_arraypPlayers[i].GetLives() > 0)
			{
				//If the player has a life.
				if (!m_arraypPlayerExplosions[i].IsAlive())
				{
					//If the player has a life, respawn.
					m_arraypPlayers[i].SetXYZ(m_arrayVec3PlayerStartPos[i].x,
					                          m_arrayVec3PlayerStartPos[i].y,
					                          m_arrayVec3PlayerStartPos[i].z);

					m_arraypPlayers[i].SetActive(true);
					m_arraypPlayers[i].SetLives(*m_arraypPlayers[i].GetLives() - 1);
					D3DCOLOR tempColour = D3DCOLOR_XRGB(0, 166, 255);
					D3DXVECTOR3 *tempPosition = m_arraypPlayers[i].GetPosition();
					m_arraypPlayerExplosions[i].Initialise(tempPosition,
					                                       tempColour,
					                                       5000,
					                                       m_pRenderer->GetDevice(),
					                                       5.0f,
					                                       25.0f);
					m_arraypPlayerExplosions[i].SetAlive(false);

					m_arraypShields[i].SetLifeTime(5.0f);

					// Turn Rumble Off.
					CMessageProcessor::GetInstance()->SendMessage(MESSAGE_XINPUT_CONTROLLER_RUMBLE_OFF);
				}
			}
		}

		if (*m_arraypPlayers[i].GetLives() <= 0)
		{
			// Send a message to turn off the controller rumble.
			CMessageProcessor::GetInstance()->SendMessage(MESSAGE_XINPUT_CONTROLLER_RUMBLE_OFF);
		}

		// If all the players are dead then see if there is a highscore.
		if (bAllPlayersDead)
		{
			if (!m_bHighScoreAdded)
			{
				m_bHighScoreAdded = true;
				UInt32 *uiScore = m_arraypPlayers[i].GetScore();

				if (m_pHighScoreParser->IsHighScore(*uiScore))
				{
					// Set the highscore.
					m_pHighScoreParser->AddHighScore(*uiScore);
					m_pHighScoreParser->SaveHighScores("highscore.tbl");
					m_pHud->SetHighScore(*uiScore);
				}
			}
		}
	}
}

/**
*
* This function calls process on all the enemy objects.
*
* @param _fDeltaTick is the amount of time elapsed between frames.
* @return Returns void.
*
*/
void
CLevel::ProcessEnemies(Float32 _fDeltaTick)
{
	for (UInt8 i = 0; i < m_vecpEnemies.size(); ++i)
	{
		m_vecpEnemies[i]->Process(_fDeltaTick);
		m_arraypPowerUps[i].Process(_fDeltaTick);
		if (!m_vecpEnemies[i]->GetLife())
		{
			//Enemy has died.
			DestroyEnemy(i);
			SpawnPowerUp(i);
		}
		if (m_vecpEnemies[i]->IsReadyToShoot() == true)
		{
			for (UInt8 j = 0; j < m_ucNumOfEnemyBullets; ++j)
			{
				if (m_arraypEnemyBullets[j].IsBulletActive() == false)
				{
					m_arraypEnemyBullets[j].SetBulletActivity(true);
					m_arraypEnemyBullets[j].SetXYZDirection(m_vecpEnemies[i]->
					                                        GetPosition(),
					                                        m_vecpEnemies[i]->
					                                        GetHeadingVector());

					break;
				}
			}
		}
	}

	bool bLevelComplete = true;
	for (UInt8 i = 0; i < m_vecpEnemies.size(); ++i)
	{
		if (m_vecpEnemies[i]->IsActive() || m_vecpEnemyExplosions[i]->IsAlive())
		{
			bLevelComplete = false;
			break;
		}
	}
	if (bLevelComplete)
	{
		RemakeLevel();
	}
}

/**
*
* This function calls process on all the Heads-up display.
*
* @param _fDeltaTick is the amount of time elapsed between frames.
* @return Returns void.
*
*/
void
CLevel::ProcessHUD(Float32 _fDeltaTick)
{
}

/**
*
* This function calls process on all the power-ups.
*
* @param _fDeltaTick is the amount of time elapsed between frames.
* @return Returns void.
*
*/
void
CLevel::ProcessPowerUps(Float32 _fDeltaTick)
{
	for (UInt32 i = 0; i < m_vecpEnemies.size(); ++i)
	{
		if (m_arraypPowerUps[i].IsActive())
		{
			for (UInt32 j = 0; j < m_ucNumOfPlayers; ++j)
			{
				if (CDynamicEntity::IsCollision(&m_arraypPlayers[j],
				                                &m_arraypPowerUps[i]))
				{
					m_arraypPowerUps[i].SetActive(false);
					switch (m_arraypPowerUps[i].GetPowerUpType())
					{
						case POWERUP_SHIELD:
						{
							m_arraypShields[j].SetLifeTime(5.0f);
						}
						break;
						case POWERUP_SPEED:
						{
							m_arraypPlayers[j].SetSpeedPowerUpTime(5.0f);
						}
						break;
						case POWERUP_SHOT:
						{
							m_arraypPlayers[j].SetBulletPowerUpTime(5.0f);
						}
						break;
					}
				}
			}
		}
	}
}

/**
*
* This function calls process on all the bullets.
*
* @param _fDeltaTick is the amount of time elapsed between frames.
* @return Returns void.
*
*/
void
CLevel::ProcessBullets(Float32 _fDeltaTick)
{
	// Process the Players bullets
	for (UInt8 i = 0; i < m_ucNumOfPlayerBullets; ++i)
	{
		m_arraypPlayerBullets[i].Process(_fDeltaTick);
		if (m_arraypPlayerBullets[i].IsBulletActive())
		{
			for (UInt32 j = 0; j < m_vecpEnemies.size(); ++j)
			{
				if (m_vecpEnemies[j]->IsActive())
				{
					if (CDynamicEntity::IsCollision(m_vecpEnemies[j], &m_arraypPlayerBullets[i]))
					{
						m_vecpEnemies[j]->DepleteLife(m_arraypPlayerBullets[i].GetBulletDamage());
						m_arraypPlayerBullets[i].SetBulletActivity(false);

						break;
					}
				}
			}
		}
	}

	// Process the Enemies bullets
	for (UInt8 i = 0; i < m_ucNumOfEnemyBullets; ++i)
	{
		m_arraypEnemyBullets[i].Process(_fDeltaTick);
		if (m_arraypEnemyBullets[i].IsBulletActive())
		{
			for (UInt32 j = 0; j < m_ucNumOfPlayers; ++j)
			{
				if (m_arraypPlayers[j].IsActive())
				{
					if (CDynamicEntity::IsCollision(&m_arraypPlayers[j], &m_arraypEnemyBullets[i]))
					{
						m_arraypEnemyBullets[i].SetBulletActivity(false);
						DestroyPlayer(j);
					}
				}
			}
		}
	}
}

/**
*
* This function calls process on all the explosions.
*
* @param _fDeltaTick is the amount of time elapsed between frames.
* @return Returns void.
*
*/
void
CLevel::ProcessExplosions(Float32 _fDeltaTick)
{
	for (UInt32 i = 0; i < m_vecpEnemyExplosions.size(); ++i)
	{
		m_vecpEnemyExplosions[i]->Process(_fDeltaTick);
	}
	for (UInt32 i = 0; i < m_ucNumOfPlayers; ++i)
	{
		m_arraypPlayerExplosions[i].Process(_fDeltaTick);
	}
}

/**
*
* This function calls process on all the lights.
*
* @param _fDeltaTick is the amount of time elapsed between frames.
* @return Returns void.
*
*/
void
CLevel::ProcessLights(Float32 _fDeltaTick)
{
	for (UInt8 i = 0; i < m_ucNumOfPlayers; ++i)
	{
		m_arraypPointLights[i].Process(_fDeltaTick);
	}
}

void
CLevel::InitialiseEnemies()
{
	//Work out how many of each enemy to create.

	float numSeekersToCreate = m_fNumSeekerEnemies - m_fPrevNumSeekers;
	float numShootersToCreate = m_fNumShooterEnemies - m_fPrevNumShooters;
	float numRandomsToCreate = m_fNumRandomEnemies - m_fPrevNumRandoms;

	D3DXVECTOR3 zeroVector;
	ZeroMemory(zeroVector, sizeof(D3DXVECTOR3));
	UInt32 currentNumberOfEnemies = 0;

	// Creating the Seeker Enemies
	for (UInt32 i = 0; i < static_cast<UInt32>(numSeekersToCreate); ++i)
	{
		CSeekerEnemy *tempEnemy = new CSeekerEnemy;
		m_vecpEnemies.push_back(reinterpret_cast<CEnemy *>(tempEnemy));
		m_vecpEnemies[m_uiVectorIndex + currentNumberOfEnemies]->Initialise(0,
		                                                                    zeroVector,
		                                                                    L"media\\enemy1.x",
		                                                                    L"media\\enemy1.png",
		                                                                    m_pRenderer,
		                                                                    m_fSeekerSpeed,
		                                                                    m_fSeekerTurnSpeed,
		                                                                    m_ucSeekerEnemyLife,
		                                                                    m_fEnemyReloadTime,
		                                                                    m_usSeekerEnemyScoreValue);
		m_vecpEnemies[m_uiVectorIndex + currentNumberOfEnemies]->SetBoundary(m_rectBoundary);
		++currentNumberOfEnemies;
	}

	// Creating the Shooter Enemies
	for (UInt32 i = 0; i < static_cast<UInt32>(numShootersToCreate); ++i)
	{
		//Create shooter enemies.

		CShooterEnemy *tempEnemy = new CShooterEnemy;
		m_vecpEnemies.push_back(reinterpret_cast<CEnemy *>(tempEnemy));
		m_vecpEnemies[m_uiVectorIndex + currentNumberOfEnemies]->Initialise(0,
		                                                                    zeroVector,
		                                                                    L"media\\enemy3.x",
		                                                                    L"media\\enemy3.png",
		                                                                    m_pRenderer,
		                                                                    m_fShooterSpeed,
		                                                                    m_fShooterTurnSpeed,
		                                                                    m_ucShooterEnemyLife,
		                                                                    m_fEnemyReloadTime,
		                                                                    m_usShooterEnemyScoreValue);
		m_vecpEnemies[m_uiVectorIndex + currentNumberOfEnemies]->SetBoundary(m_rectBoundary);
		++currentNumberOfEnemies;
	}

	// Creating the Random Enemies
	for (UInt32 i = 0; i < static_cast<UInt32>(numRandomsToCreate); ++i)
	{
		CRandomEnemy *tempEnemy = new CRandomEnemy;
		m_vecpEnemies.push_back(reinterpret_cast<CEnemy *>(tempEnemy));
		m_vecpEnemies[m_uiVectorIndex + currentNumberOfEnemies]->Initialise(0,
		                                                                    zeroVector,
		                                                                    L"media\\enemy2.x",
		                                                                    L"media\\enemy2.png",
		                                                                    m_pRenderer,
		                                                                    m_fRandomSpeed,
		                                                                    m_fRandomTurnSpeed,
		                                                                    m_ucRandomEnemyLife,
		                                                                    m_fEnemyReloadTime,
		                                                                    m_usRandomEnemyScoreValue);
		m_vecpEnemies[m_uiVectorIndex + currentNumberOfEnemies]->SetBoundary(m_rectBoundary);
		++currentNumberOfEnemies;
	}

	D3DXMATRIX *pPointToFollow = 0;
	for (UInt32 i = 0; i < m_vecpEnemies.size(); ++i)
	{
		pPointToFollow = m_arraypPlayers[i % m_ucNumOfPlayers].GetMatrix();
		D3DXVECTOR3 goodPosition = GetGoodEnemyPosition();
		m_vecpEnemies[i]->Initialise(pPointToFollow, goodPosition);
	}

	m_fPrevNumSeekers = floor(m_fNumSeekerEnemies);
	m_fPrevNumShooters = floor(m_fNumShooterEnemies);
	m_fPrevNumRandoms = floor(m_fNumRandomEnemies);

	m_uiVectorIndex = static_cast<UInt32>(m_vecpEnemies.size());

	m_fNumSeekerEnemies *= m_fEnemySpawnMultiplier;
	m_fNumShooterEnemies *= m_fEnemySpawnMultiplier;
	m_fNumRandomEnemies *= m_fEnemySpawnMultiplier;
}

D3DXVECTOR3
CLevel::GetGoodEnemyPosition()
{
	D3DXVECTOR3 vec3Position;
	D3DXVECTOR3 *vec3PlayerPosition;
	D3DXVECTOR3 vec3DistanceVector;
	bool bNotPositionedCorrectly = true;
	while (bNotPositionedCorrectly == true)
	{
		vec3Position.x = static_cast<float>(rand() % static_cast<int>
		                                    (m_fPlayAreaWidth) - m_fPlayAreaWidth / 2);
		vec3Position.y = 0;
		vec3Position.z = static_cast<float>(rand() % static_cast<int>
		                                    (m_fPlayAreaDepth) - m_fPlayAreaDepth / 2);
		bool bBadPosition = false;
		for (UInt32 i = 0; i < m_ucNumOfPlayers; ++i)
		{
			vec3PlayerPosition = m_arraypPlayers[i].GetPosition();
			vec3DistanceVector = *vec3PlayerPosition - vec3Position;
			float bDistance = sqrt((vec3Position.x * vec3Position.x) +
			                       (vec3Position.y * vec3Position.y) +
			                       (vec3Position.z * vec3Position.z));
			if (bDistance < m_fSafeZoneRadius)
			{
				bBadPosition = true;
			}
		}
		bNotPositionedCorrectly = bBadPosition;
	}
	return (vec3Position);
}

bool
CLevel::InitialisePlayers(CTopDownCamera **_arraypCameras)
{
	if (!m_arraypPlayers)
	{
		m_arraypPlayers = new CPlayer[m_ucNumOfPlayers];
	}

	for (UInt8 i = 0; i < m_ucNumOfPlayers; ++i)
	{
		if (i == 0)
		{
			m_arraypPlayers[i].Initialise(m_arrayVec3PlayerStartPos[i],
			                              m_ucNumberOfLives,
			                              L"media\\playership.x",
			                              L"media\\playership.png",
			                              m_pRenderer,
			                              m_fPlayerSpeed,
			                              m_fPlayerTurnSpeed,
			                              m_fPlayerFriction,
			                              i,
			                              m_fPlayerReloadTime);
			m_arraypPlayers[i].SetBoundary(m_rectBoundary);
		} else
		{
			m_arraypPlayers[i].Initialise(m_arrayVec3PlayerStartPos[i],
			                              m_ucNumberOfLives,
			                              L"media\\playership.x",
			                              L"media\\playership.png",
			                              m_pRenderer,
			                              m_fPlayerSpeed,
			                              m_fPlayerTurnSpeed,
			                              m_fPlayerFriction,
			                              i,
			                              m_fPlayerReloadTime,
			                              MESSAGE_PRESSED_KEY_A,
			                              MESSAGE_PRESSED_KEY_D,
			                              MESSAGE_PRESSED_KEY_W,
			                              MESSAGE_PRESSED_KEY_S);
			m_arraypPlayers[i].SetBoundary(m_rectBoundary);
		}

		_arraypCameras[i]->SetFollow(m_arraypPlayers[i].GetMatrix());
		_arraypCameras[i]->SetHeight(m_fCameraHeight / m_ucNumOfPlayers);
	}
	return (true);
}

bool
CLevel::InitialiseBullets()
{
	if (!m_arraypPlayerBullets)
	{
		m_arraypPlayerBullets = new CBullet[m_ucNumOfPlayerBullets];
		for (UInt8 i = 0; i < m_ucNumOfPlayerBullets; ++i)
		{
			m_arraypPlayerBullets[i].Initialise(L"media\\bullet.x",
			                                    L"media\\bullet.png",
			                                    m_pRenderer,
			                                    m_ucBulletNormalDamage,
			                                    m_fBulletSpeed,
			                                    false);
			m_arraypPlayerBullets[i].SetBoundary(m_rectBoundary);
		}
	} else
	{
		for (UInt8 i = 0; i < m_ucNumOfPlayerBullets; ++i)
		{
			m_arraypPlayerBullets[i].SetBulletActivity(false);
		}
	}

	// Creating the Enemies bullets.
	if (!m_arraypEnemyBullets)
	{
		m_arraypEnemyBullets = new CBullet[m_ucNumOfEnemyBullets];
		for (UInt8 i = 0; i < m_ucNumOfEnemyBullets; ++i)
		{
			m_arraypEnemyBullets[i].Initialise(L"media\\bullet.x",
			                                   L"media\\bullet2.png",
			                                   m_pRenderer,
			                                   m_ucBulletNormalDamage,
			                                   m_fBulletSpeed,
			                                   false);
			m_arraypEnemyBullets[i].SetBoundary(m_rectBoundary);
		}
	} else
	{
		for (UInt8 i = 0; i < m_ucNumOfEnemyBullets; ++i)
		{
			m_arraypEnemyBullets[i].SetBulletActivity(false);
		}
	}
	return (true);
}

bool
CLevel::InitialiseExplosions()
{
	for (UInt32 i = 0; i < m_vecpEnemyExplosions.size(); ++i)
	{
		delete m_vecpEnemyExplosions[i];
		m_vecpEnemyExplosions[i] = 0;
	}
	m_vecpEnemyExplosions.clear();
	for (UInt32 i = 0; i < m_vecpEnemies.size(); ++i)
	{
		D3DCOLOR tempColour = D3DCOLOR_XRGB(255, 207, 64);
		CExplosion *tempExplosion = new CExplosion;
		D3DXVECTOR3 *tempPosition = m_vecpEnemies[i]->GetPosition();
		tempExplosion->Initialise(tempPosition,
		                          tempColour,
		                          1000,
		                          m_pRenderer->GetDevice(),
		                          5.0f,
		                          20.0f);
		tempExplosion->SetAlive(false);
		m_vecpEnemyExplosions.push_back(tempExplosion);
	}

	if (!m_arraypPlayerExplosions)
	{
		m_arraypPlayerExplosions = new CExplosion[m_ucNumOfPlayers];
	}

	for (UInt32 i = 0; i < m_ucNumOfPlayers; ++i)
	{
		D3DCOLOR tempColour = D3DCOLOR_XRGB(0, 166, 255);
		D3DXVECTOR3 *tempPosition = m_arraypPlayers[i].GetPosition();
		m_arraypPlayerExplosions[i].Initialise(tempPosition,
		                                       tempColour,
		                                       5000,
		                                       m_pRenderer->GetDevice(),
		                                       5.0f,
		                                       25.0f);
		m_arraypPlayerExplosions[i].SetAlive(false);
	}

	return (true);
}

void
CLevel::RemakeLevel()
{
	InitialiseEnemies();
	InitialiseBullets();
	InitialiseExplosions();
	InitialisePowerUps();
	CGame::GetInstance().ResetClock();
}

void
CLevel::InitialisePowerUps()
{
	if (m_arraypPowerUps)
	{
		delete[] m_arraypPowerUps;
		m_arraypPowerUps = 0;
	}
	m_arraypPowerUps = new CPowerUp[m_vecpEnemies.size()];
	for (UInt32 i = 0; i < m_vecpEnemies.size(); ++i)
	{
		m_arraypPowerUps[i].SetActive(false);
	}
}

void
CLevel::SpawnPowerUp(UInt32 _uiIndex)
{
	EPowerUpType randomPowerUpType = INVALID_POWERUP_TYPE;

	UInt32 RandomNumber = rand() % m_ucPowerUpDropRate;

	if (RandomNumber <= m_ucBulletSpawn)
	{
		randomPowerUpType = POWERUP_SHOT;
	} else if (RandomNumber <= m_ucShieldSpawn)
	{
		randomPowerUpType = POWERUP_SHIELD;
	} else if (RandomNumber <= m_ucSpeedSpawn)
	{
		randomPowerUpType = POWERUP_SPEED;
	}

	if (randomPowerUpType != INVALID_POWERUP_TYPE)
	{
		m_arraypPowerUps[_uiIndex].Initialise(randomPowerUpType,
		                                      m_fPowerUpSpeed,
		                                      m_fShooterTurnSpeed,
		                                      m_pRenderer);
		m_arraypPowerUps[_uiIndex].SetXYZ(m_vecpEnemies[_uiIndex]->GetX(),
		                                  m_vecpEnemies[_uiIndex]->GetY(),
		                                  m_vecpEnemies[_uiIndex]->GetZ());
	}
}

void
CLevel::InitialiseShields()
{
	delete[] m_arraypShields;
	m_arraypShields = 0;

	m_arraypShields = new CShield[m_ucNumOfPlayers];

	for (UInt32 i = 0; i < m_ucNumOfPlayers; ++i)
	{
		m_arraypShields[i].Initialise(m_pRenderer, m_arraypPlayers[i].GetPosition());
	}
}

void
CLevel::ProcessShields(Float32 _fDeltaTick)
{
	for (UInt32 i = 0; i < m_ucNumOfPlayers; ++i)
	{
		m_arraypShields[i].Process(_fDeltaTick);
		if (m_arraypShields[i].IsAlive())
		{
			for (UInt32 j = 0; j < m_vecpEnemies.size(); ++j)
			{
				if (m_vecpEnemies[j]->IsActive())
				{
					if (CDynamicEntity::IsCollision(m_vecpEnemies[j], &m_arraypShields[i]))
					{
						DestroyEnemy(j);
					}
				}
			}
			for (UInt8 j = 0; j < m_ucNumOfEnemyBullets; ++j)
			{
				if (m_arraypEnemyBullets[j].IsBulletActive())
				{
					if (CDynamicEntity::IsCollision(&m_arraypEnemyBullets[j], &m_arraypShields[i]))
					{
						m_arraypEnemyBullets[j].SetBulletActivity(false);
					}
				}
			}
		}
	}
}

void
CLevel::DestroyEnemy(UInt32 _iIndex)
{
	m_vecpEnemies[_iIndex]->SetLife(-1);
	m_vecpEnemies[_iIndex]->SetActive(false);
	m_vecpEnemyExplosions[_iIndex]->SetAlive(true);
	for (UInt8 i = 0; i < m_ucNumOfPlayers; ++i)
	{
		m_arraypPlayers[i].AddToScore(m_vecpEnemies[i]->GetEnemyPointsValue());
	}
}

void
CLevel::DestroyPlayer(UInt8 _iIndex)
{
	m_arraypPlayers[_iIndex].SetActive(false);
	m_arraypPlayerExplosions[_iIndex].SetAlive(true);

	// Turn Rumble On.
	CMessageProcessor::GetInstance()->SendMessage(MESSAGE_XINPUT_CONTROLLER_RUMBLE_ON);
}



