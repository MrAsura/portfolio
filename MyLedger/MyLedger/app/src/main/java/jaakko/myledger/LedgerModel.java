package jaakko.myledger;

import android.support.annotation.NonNull;
import android.util.Pair;

import java.io.FileInputStream;
import java.io.*;
import java.util.ArrayList;


/**
 * Created by Jaakko on 10.2.2016.
 */
public class LedgerModel implements Serializable {

    //Internal data structure containing the dat
    //Define separate ArrayLists for iban and name, because a pair is not serializable
    private ArrayList<String> ledger_names_ = new ArrayList<String>();
    private ArrayList<String> ledger_ibans_ = new ArrayList<String>();


    LedgerModel() {

        //Generate test data to test the ListView
        //Need to desable loading or these get overwritten
        /*for (int i = 0; i < 20; ++i){
            ledger_names_.add("Test" + Integer.toString(i));
            ledger_ibans_.add("IBAN");
        }*/

    }


    //Previous data is overwritten.
    public void load(FileInputStream file) throws IOException, ClassNotFoundException {
        //Ready object stream
        ObjectInputStream ois = new ObjectInputStream(file);

        ledger_names_ = (ArrayList<String>) ois.readObject();
        ledger_ibans_ = (ArrayList<String>) ois.readObject();

        ois.close();
    }

    public void save(FileOutputStream file) throws IOException {
        //ready object stream
        ObjectOutputStream oos = new ObjectOutputStream(file);

        oos.writeObject(ledger_names_);
        oos.writeObject(ledger_ibans_);

        oos.close();
    }


    public Boolean addItem(String name, String IBAN)
    {
        //Check validity of name and IBAN
        if( !checkName(name) || !checkIBAN(IBAN) )
        {
            return false;
        }

        ledger_names_.add(name);
        ledger_ibans_.add(IBAN);

        //save(); //save changes

        return true;
    }

    public void delItem(int ind)
    {
        ledger_names_.remove(ind);
        ledger_ibans_.remove(ind);
        //save(); //Save changes
    }

    public int length()
    {
        return ledger_names_.size();
    }

    public String nameAt(int ind)
    {
        return ledger_names_.get(ind);
    }

    public String ibanAt(int ind)
    {
        return ledger_ibans_.get(ind);
    }

    public Pair<String,String> get(int ind)
    {
        return new Pair<String,String>(ledger_names_.get(ind),ledger_ibans_.get(ind));
    }

    @NonNull
    static public Boolean checkName(String name)
    {
        //check name validity
        //Since it is near impossible to make a comprehensive rule for valid names that accepts
        //any actual name in the world, only limit the use of some special characters
        return !name.matches(".*[!\"#\\$%&£€\\(\\)\\*\\+,/:;<=>\\?@\\[\\]\\^_\\{\\|\\}~\\\\¤½]+.*") && !name.isEmpty();
    }

    @NonNull
    static public Boolean checkIBAN(String IBAN)
    {
        //Remove white space
        IBAN = IBAN.replaceAll("\\s","");

        //Check that IBAN begins with a country and does not contain special chars
        if(!IBAN.matches("^[A-Z]{2}\\w{2,}$"))
        {
            return false;
        }

        //Move Country code and checksum to end
        String cc = IBAN.substring(0, 4);
        IBAN = (IBAN.substring(4) + cc).toUpperCase();

        //Convert Characters to numbers
        //A->10,B->11...Z->35
        //Convert IBAN to base 36
        Character c;
        Integer num;
        for( c = 'A', num = 10 ; c <= 'Z' ; ++c, ++num)
        {
            IBAN = IBAN.replace(c.toString(),num.toString());
        }

        //Calculate modulo piece by piece
        int modulo = Integer.valueOf(IBAN.substring(0, 1));
        for( int ind = 1; ind < IBAN.length() ; ++ind )
        {
            modulo = (modulo*10 + Integer.valueOf(IBAN.substring(ind,ind+1)))%97;
        }

        //Check validity iban mod 97 == 1
        return (modulo == 1);
    }

}
