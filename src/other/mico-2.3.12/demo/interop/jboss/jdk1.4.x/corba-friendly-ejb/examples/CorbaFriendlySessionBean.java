package examples;

import java.rmi.RemoteException; 
import javax.ejb.SessionBean;
import javax.ejb.SessionContext;

import examples.corba.DataObject;

public class CorbaFriendlySessionBean implements SessionBean {
 
   public DataObject echo(DataObject data) {
      data.i++;
      data.s += "!";
      return data;
   }
   
   public DataObject[] echoSequence(DataObject[] dataSeq) {
      for (int j = 0; j < dataSeq.length; j++) {
	 dataSeq[j].i++;
	 dataSeq[j].s += "!";
      }
      return dataSeq;
   }
   
   public CorbaFriendlySessionBean() {}
   public void ejbCreate() {}
   public void ejbRemove() {}
   public void ejbActivate() {}
   public void ejbPassivate() {}
   public void setSessionContext(SessionContext sc) {}
   
}
