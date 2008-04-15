package examples;

import javax.ejb.EJBHome;
import java.rmi.RemoteException;
import javax.ejb.CreateException;

public interface CorbaFriendlySessionHome
   extends EJBHome
{
   public CorbaFriendlySession create() 
      throws CreateException, RemoteException;
}

