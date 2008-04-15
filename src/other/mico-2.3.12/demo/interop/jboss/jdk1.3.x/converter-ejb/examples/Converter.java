package examples;

import javax.ejb.EJBObject;
import java.rmi.RemoteException;

public interface Converter extends EJBObject {
 
   public int dollar_to_euro(int dollars) throws RemoteException;
   public int euro_to_dollar(int euro) throws RemoteException;
}
