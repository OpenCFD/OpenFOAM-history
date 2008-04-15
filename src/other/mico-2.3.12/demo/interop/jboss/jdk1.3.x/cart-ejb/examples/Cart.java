package examples;

import java.util.*;
import javax.ejb.EJBObject;
import java.rmi.RemoteException;

public interface Cart extends EJBObject {
 
   public void addBook(String title) throws RemoteException;
   public void removeBook(String title) throws BookException, RemoteException;
   public String[] getContents() throws RemoteException;
}
