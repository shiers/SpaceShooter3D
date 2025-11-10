#pragma once

#ifndef SPACESHOOTER3D_HIGHSCORE_H
#define SPACESHOOTER3D_HIGHSCORE_H

// Library Includes
#include <vector>
#include <string>

// Local Includes
#include "defines.h"

// Types

// Constants

// Prototypes


class CHighScore
{
   //Member Functions
public:
	CHighScore();
	~CHighScore();

	void LoadHighScores(const Int8* _pFileName);
	void SaveHighScores(const Int8* _pFileName);

	void AddHighScore(Int32 _iScore,  const Int8* _cName = "SpaceShooter 3D");
	bool IsHighScore(Int32 _iScore);

	void GetScore(Int32 _iPlace, Int32& _iHighScore, std::string& _strHighName);
	Int32 GetNumScores();

protected:

private:

   //Member Variables
public:
	
protected:

private:
	std::vector<std::string> m_pvecNames;
	std::vector<Int32> m_pvecScores;

};

#endif // SPACESHOOTER3D_HIGHSCORE_H



