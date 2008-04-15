package examples;

import java.rmi.RemoteException; 
import javax.ejb.SessionBean;
import javax.ejb.SessionContext;

public class ConverterBean implements SessionBean {
 
    int _euro_rate = 11;

   public int dollar_to_euro (int dollars) {

      int result = dollars * _euro_rate;
      return result;
   }

   public int euro_to_dollar (int euros) {

      int result = euros / _euro_rate;
      return result;
   }

   public ConverterBean() {}
   public void ejbCreate() {}
   public void ejbRemove() {}
   public void ejbActivate() {}
   public void ejbPassivate() {}
   public void setSessionContext(SessionContext sc) {}

} // ConverterBean
