#if !defined( __CRITICALSECTION_H__ )

	#define __CRITICALSECTION_H__
	
	#include <windows.h>

/**
 * The CriticalSection class is used to protect a section of code so that at any
 * given time only a single thread can perform the protected operation.
 * This implementation uses CRITICAL_SECTION.
 */
class MICODCM_API CriticalSection 
{

protected:

		CRITICAL_SECTION m_critical;

public:
	
							CriticalSection();

	virtual					~CriticalSection();

							/**
							 * Entering a CriticalSection locks the mutex for the current thread.
							 */
			void			Enter(void);


							/**
							 * Tries to lock the mutex for the current thread. Behaves like
							 * #Enter , except that it doesn't block the calling thread
							 * if the mutex is already locked by another thread.
							 * This implementation work only on NT/2000/XP.
							 *
							 * @return true if locking the mutex was succesful otherwise false
							 *
							 * @see Enter
							 * @see Leave
							 */
			bool			TryEnter(void);

							/**
							 * Leaving a mutex frees that mutex for use by another thread.
							 * @see #Enter
							 */
			void			Leave(void);

};
#endif
