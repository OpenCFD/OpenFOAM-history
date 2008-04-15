package examples;

import java.io.Serializable;
import java.rmi.RemoteException;
import javax.ejb.CreateException;
import javax.ejb.EJBHome;

public interface CartHome extends EJBHome {
 
    Cart create(String person) throws RemoteException, CreateException;
    Cart create(String person, String id) throws RemoteException, 
                                                 CreateException; 
}
