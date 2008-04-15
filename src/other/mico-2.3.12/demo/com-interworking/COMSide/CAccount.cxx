#include <windows.h>
#include <stdio.h>
#include "account.h"

// Mico
#include <CORBA-SMALL.h>
#include "..\account2\account.h"

void DecrementLockCount();
void IncrementLockCount();
void ObjectCreated();
void ObjectDestroyed();

extern "C" const CLSID CLSID_Account;

long    g_fClassRegistered  = FALSE;
DWORD   g_dwRegister        = 0;

//The CAccount class implements the server object.
class CAccount : public IAccount
{
private:
    unsigned long    m_cRef;

    // Mico
    CORBA::ORB_var orb;
    CORBA::BOA_var boa;
    CORBA::Object_var obj;
    Account_var client;     

    //destructor
    ~CAccount()
    {
        DecrementLockCount();
        ObjectDestroyed();
    }
    
public:
    //constructor
    CAccount()
    {
        m_cRef = 1;
        ObjectCreated();
        IncrementLockCount();

        // Mico
        // ORB initialization
        int argc = 2;
        char *argv[2];
        argv[0] = "client.exe";
        argv[1] = "inet:localhost:12456";

        orb = CORBA::ORB_init( argc, argv, "mico-local-orb" );
        boa = orb->BOA_init (argc, argv, "mico-local-boa");

        CORBA::ORB::ObjectTag_var tag = CORBA::ORB::string_to_tag ("foobar");
        /*
         * bind to an instance of type "IDL:Account:1.0" whose ReferenceData
         * equals "foobar".
         */
        obj = orb->bind ("IDL:Account:1.0", tag, argv[1]);
        assert (!CORBA::is_nil (obj));
        client = Account::_narrow( obj );
       
    }
    
	HRESULT STDMETHODCALLTYPE QueryInterface(
		REFIID iid,
	    void **ppv);
	
	ULONG STDMETHODCALLTYPE AddRef();
	
	ULONG STDMETHODCALLTYPE Release();
	
	//HRESULT STDMETHODCALLTYPE HelloProc(unsigned char *pszString);
	HRESULT STDMETHODCALLTYPE balance(long *pRetVal);
	HRESULT STDMETHODCALLTYPE deposit(unsigned long amount);
	HRESULT STDMETHODCALLTYPE withdraw(unsigned long amount);
};

//+-------------------------------------------------------------------------
//
//  Method:     CAccount::AddRef, public
//
//  Synopsis:   Increment reference count
//
//  See Also:   IUnknown::AddRef
//
//--------------------------------------------------------------------------
ULONG STDMETHODCALLTYPE 
CAccount::AddRef()
{
    InterlockedIncrement((long *) &m_cRef);
    return m_cRef;
}

//+---------------------------------------------------------------------------
//
//  Method:     CAccount::balance, deposit, withdraw
//
//  Synopsis:   Call the CORBA Object
//
//
//
//----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE
CAccount::balance(long *pRetVal)
{
    *pRetVal = client->balance();
    return S_OK;
}

HRESULT STDMETHODCALLTYPE
CAccount::deposit(unsigned long amount)
{
    client->deposit(amount);
    return S_OK;
}

HRESULT STDMETHODCALLTYPE
CAccount::withdraw(unsigned long amount)
{
    client->withdraw(amount);
    return S_OK;
}

//+-------------------------------------------------------------------------
//
//  Method:     CAccount::QueryInterface, public
//
//  Synopsis:   Query for an interface on the class factory.
//
//  See Also:   IUnknown:QueryInterface
//
//--------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE 
CAccount::QueryInterface (
    REFIID iid,
    void **ppv )
{
    HRESULT hr;

    if ( IsEqualGUID( iid, IID_IUnknown ) ||
         IsEqualGUID( iid, IID_IAccount ) )
    {
        *ppv = (IAccount *) this;
        AddRef();
	    hr = S_OK;
    }
    else
    {
        *ppv = 0;
        hr = E_NOINTERFACE;
    }

    return hr;
}

//+-------------------------------------------------------------------------
//
//  Method:     CAccount::Release, public
//
//  Synopsis:   Decrement DLL reference count
//
//  Notes:      After the m_cRef is decremented, the object may be 
//              deleted by another thread.  In order to make this code safe
//              for multiple threads, we have to access the object state 
//              before decrementing m_cRef.
//
//  See Also:   IUnknown::Release.
//
//--------------------------------------------------------------------------
ULONG STDMETHODCALLTYPE
CAccount::Release()
{
    unsigned long count;
    
    count = m_cRef - 1;

    if(InterlockedDecrement((long *) &m_cRef) == 0)
    {
        count = 0;
        delete this;
    }

    return count;
}



// this is the class factory for the CHello server object.
// it manufactures CHello server objects in response to a CreateInstance
// call

class CAccountClassFactory : public IClassFactory
{
private:
    unsigned long    m_cRef;

    //destructor
    ~CAccountClassFactory()
    {
        ObjectDestroyed();
    }

public:
    //constructor
    CAccountClassFactory()
    {
        m_cRef = 1;
        ObjectCreated();
    }

	HRESULT STDMETHODCALLTYPE QueryInterface(
		REFIID iid,
	    void **ppv);
	
	ULONG STDMETHODCALLTYPE AddRef();
	
	ULONG STDMETHODCALLTYPE Release();
	
	HRESULT STDMETHODCALLTYPE CreateInstance(
	    IUnknown *punkOuter,
	    REFIID riid,
	    void **ppv);

   HRESULT STDMETHODCALLTYPE LockServer(
        BOOL fLock );

};

//+-------------------------------------------------------------------------
//
//  Method:     CAccountClassFactory::AddRef, public
//
//  Synopsis:   Increment DLL reference counts
//
//  See Also:   IUnknown::AddRef
//
//--------------------------------------------------------------------------
ULONG STDMETHODCALLTYPE 
CAccountClassFactory::AddRef()
{
    InterlockedIncrement((long *) &m_cRef);
    return m_cRef;
}

//+-------------------------------------------------------------------------
//
//  Method:     CAccountClassFactory::CreateInstance, public
//
//  Synopsis:   Create an instance of CAccount.
//
//  See Also:   IClassFactory::CreateInstance
//
//--------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE 
CAccountClassFactory::CreateInstance
(
    IUnknown *punkOuter,
    REFIID riid,
    void **ppv
)
{
    HRESULT hr;
    CAccount *pAccount;

    if(punkOuter != 0)
    {
        //The CAccount class doesn't support aggregation.
        return CLASS_E_NOAGGREGATION;
    }

    pAccount = new CAccount();
    if(pAccount != 0)
    {
        hr = pAccount->QueryInterface(riid, ppv);
        pAccount->Release();
    }
    else
    {
        hr = E_OUTOFMEMORY;
        *ppv = 0;
    }

    return hr;
}

//+-------------------------------------------------------------------------
//
//  Method:     CAccountClassFactory::LockServer, public
//
//  Synopsis:   Lock the server in memory (by adding an extra reference)
//
//  Notes:      The class factory will be revoked when the lock count
//              is decremented to zero.  LockServer(TRUE) will increment the
//              lock count and ensure that the class factory will
//              not be revoked.
//
//  See Also:   IClassFactory::LockServer
//
//--------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE 
CAccountClassFactory::LockServer(
    BOOL fLock )
{
    if (fLock == TRUE)
        IncrementLockCount();
    else
        DecrementLockCount();

    return S_OK;
}


//+-------------------------------------------------------------------------
//
//  Method:     CAccountClassFactory::QueryInterface, public
//
//  Synopsis:   Query for an interface on the class factory.
//
//  See Also:   IUnknown::QueryInterface
//
//--------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE 
CAccountClassFactory::QueryInterface (
    REFIID iid,
    void **ppv )
{
    HRESULT hr;

    if ( IsEqualGUID( iid, IID_IUnknown) ||
         IsEqualGUID( iid, IID_IClassFactory ) )
    {
        *ppv = this;
        AddRef();
        hr = S_OK;
    }
    else
    {
        *ppv = 0;
        hr = E_NOINTERFACE;
    }

    return hr;
}
//+-------------------------------------------------------------------------
//
//  Method:     CAccountClassFactory::Release, public
//
//  Synopsis:   Decrement DLL reference count
//
//  See Also:   IUnknown::Release
//
//--------------------------------------------------------------------------
ULONG STDMETHODCALLTYPE
CAccountClassFactory::Release()
{
    unsigned long count;
    
    count = m_cRef - 1;

    if(InterlockedDecrement((long *) &m_cRef) == 0)
    {
        count = 0;

        ObjectDestroyed();
    }

    return count;
}

//+-------------------------------------------------------------------------
//
//  Function:   RegisterClassFactory.
//
//  Synopsis:   Register the class factory if it is not currently registered.
//
//--------------------------------------------------------------------------
HRESULT RegisterClassFactory()
{
    HRESULT hr;
    CAccountClassFactory *pClassFactory;

    if(InterlockedExchange(&g_fClassRegistered, TRUE) == FALSE)
    {
        pClassFactory = new CAccountClassFactory;
        
        if(pClassFactory != 0)
        {
            hr = CoRegisterClassObject(CLSID_Account,
	   	        		               (IUnknown *) pClassFactory,
	    	        	               CLSCTX_LOCAL_SERVER,
		    	                       REGCLS_MULTIPLEUSE,
			                           &g_dwRegister);
          	pClassFactory->Release();
        }
        else
        {
           hr = E_OUTOFMEMORY;
        }
    }
    else
    {
        hr = S_OK;
    }
    return hr;  
}

//+-------------------------------------------------------------------------
//
//  Function:   RevokeClassFactory.
//
//  Synopsis:   Revoke the registered class factories if they have not
//              already been revoked.
//
//--------------------------------------------------------------------------
HRESULT RevokeClassFactory()
{
    HRESULT hr;

    if(InterlockedExchange(&g_fClassRegistered, FALSE) == TRUE)
    {
        hr = CoRevokeClassObject(g_dwRegister);
    }
    else
    {
        hr = S_OK;
    }
    return hr;
}


