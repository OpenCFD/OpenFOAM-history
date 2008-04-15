package examples;

import javax.ejb.EJBObject;
import java.rmi.RemoteException;

import examples.corba.DataObject;


public interface CorbaFriendlySession
   extends EJBObject
{
   public DataObject echo(DataObject data)
      throws RemoteException;
   
   public DataObject[] echoSequence(DataObject[] dataSeq)  
      throws RemoteException;
   
}
