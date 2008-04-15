// File: jacorb/demo/example1/server.java
//
// A simple example server 
//

public class AccountImpl implements Account{

    void Account(){
	_current_balance=0;
    }
    private int _current_balance;

    public void deposit(int amount){
	_current_balance+=amount;
    }

    public void withdraw(int amount){
	_current_balance-=amount;
    }

    public int balance(){
	return _current_balance;
    }

}
