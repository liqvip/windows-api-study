#pragma once

#define PERSONDATA 1;
#define SCOREDATA 2;

typedef struct PERSONSTRUCT {
	TCHAR m_szName[256];
	int m_nAge;
	double m_dDeposit;
}PERSONSTRUCT, *PPERSONSTRUCT;


typedef struct SCORESTRUCT {
	float m_fChiniess;
	float m_fMath;
	float m_fEnglish;
}SCORESTRUCT, *PSCORESTRUCT;

