#include <include.h>
#include "criticalsection.h"

CriticalSection::CriticalSection()
{
	InitializeCriticalSection( &m_critical );
}

CriticalSection::~CriticalSection()
{
	DeleteCriticalSection( &m_critical );
}

bool CriticalSection::TryEnter()
{
	//return ( TryEnterCriticalSection( &_critical ) ? true : false );
	return false;
}

void CriticalSection::Enter()
{
	EnterCriticalSection( &m_critical );
}

void CriticalSection::Leave()
{
	LeaveCriticalSection( &m_critical );
}
