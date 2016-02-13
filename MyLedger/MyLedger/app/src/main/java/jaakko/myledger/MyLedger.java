package jaakko.myledger;

import android.app.DialogFragment;
import android.content.Context;
import android.support.design.widget.TabLayout;
import android.support.v4.app.FragmentStatePagerAdapter;
import android.support.v4.view.PagerAdapter;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v4.view.ViewPager;
import android.os.Bundle;

import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.WrapperListAdapter;

import java.io.IOException;

import layout.AddItemFragment;
import layout.Items;
import layout.MyDialogFragment;


public class MyLedger extends AppCompatActivity implements AddItemFragment.OnFragmentInteractionListener,
        Items.OnFragmentInteractionListener, MyDialogFragment.DialogListener {

    /**
     * The {@link PagerAdapter} that will provide
     * fragments for each of the sections. We use a
     * {@link FragmentPagerAdapter} derivative, which will keep every
     * loaded fragment in memory. If this becomes too memory intensive, it
     * may be best to switch to a
     * {@link FragmentStatePagerAdapter}.
     */
    private SectionsPagerAdapter mSectionsPagerAdapter;

    /**
     * The {@link ViewPager} that will host the section contents.
     */
    private ViewPager mViewPager;


    //Internal data structure for containing item data
    private LedgerModel ledger_;

    //Dialog fragment tag
    private final String DEL_DIALOG_TAG = "deletion";

    private final String FILENAME = "data"; //default name for datafile

    private int primedPosition; //used to hold the position of an item waiting to be deleted

    //Initialize data and views
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_my_ledger);

        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        //Initialize data model
        //Saved data is loaded later
        ledger_ = new LedgerModel();

        // Create the adapter that will return a fragment for each of the two
        // primary sections of the activity.
        mSectionsPagerAdapter = new SectionsPagerAdapter(getSupportFragmentManager());

        // Set up the ViewPager with the sections adapter.
        mViewPager = (ViewPager) findViewById(R.id.container);
        mViewPager.setAdapter(mSectionsPagerAdapter);

        TabLayout tabLayout = (TabLayout) findViewById(R.id.tabs);
        tabLayout.setupWithViewPager(mViewPager);

    }

    @Override
    public void onStart() {
        super.onStart();

        //Load item list from file
        //All changes should be saved in the onStop(), if the activity is restarted
        //The ListView is refreshed in its onResume()-function so the new data should be loaded before that
        try {
            ledger_.load(openFileInput(FILENAME));
        } catch (IOException e) {
            e.printStackTrace();
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void onStop() {
        super.onStop();

        //Save item list to file
        //Saving is done here (in sted of in onPause()) to make sure transitions (like popup dialogs) remain efficient
        //Down side is that changes may be lost if the app is suddenly destroyed and onStop is not called
        //At least on a virtual machine terminating from the IDE causes the changes not to be saved
        try {
            ledger_.save(openFileOutput(FILENAME, Context.MODE_PRIVATE));
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    //This function is called when user has confirmed deletion in the dialog
    private void deletePrimedItem(){
        ledger_.delItem(primedPosition);
        primedPosition = -1; //Reset primed to prevent accidental deletions

        ListView list = (ListView) findViewById(R.id.item_view);
        ListAdapter adapter = list.getAdapter();

        //Check if lists adapter is a wrapper
        if (adapter instanceof WrapperListAdapter) {
            adapter = ((WrapperListAdapter) adapter).getWrappedAdapter();
        }
        if (adapter instanceof ItemViewAdapter) {
            ((ItemViewAdapter) adapter).notifyDataSetChanged(); //Data removed so notify adapter
        }
    }

    //Implement the listener function defined in Items.OnFragmentInteractionListener
    @Override
    public void onItemDelEvent(int position){
        //Delete the item at the given position and notify the list adapter of the data change

        //We need to wait for an accept event from the dialog
        // so prime the item for deletion and the final delete is called from the button click event
        primedPosition = position;

        //Show confirmation dialog
        DialogFragment delDialog = MyDialogFragment.newInstance("Delete item","Are you sure you want to delete the info for \"" + ledger_.nameAt(position) + "\".","Delete","Cancel");
        delDialog.show(getFragmentManager(),DEL_DIALOG_TAG);
    }


    //Handle adding new item
    //Implementation for the listener function defined in AddItemFragment.OnFragmentInteractionListener
    @Override
    public Boolean onItemAddEvent(String name, String iban){
        //Perform checks to validate data and create the needed dialogs if addition fails
        String msg = "";

        if( !LedgerModel.checkName(name) )
        {
            //Invalid name
            msg += "The name you have entered is invalid. Please check that you have entered a name and that it doesn't contain special characters.\n";
        }
        if( !LedgerModel.checkIBAN(iban) )
        {
            //Invalid iban
            msg += "The iban is invalid. Please check that you have entered it correctly and have used capital letters.";
        }

        if( !msg.equals("") )
        {
            //Name or iban is invalid. Open an error dialog
            MyDialogFragment errorDialog = MyDialogFragment.newInstance("Error",msg,"Ok","");
            errorDialog.show(getFragmentManager(),"error");

            return false;
        }


        if( ledger_.addItem(name, iban) )
        {
            //Notify items of changed model
            ListView list = (ListView) findViewById(R.id.item_view);
            ListAdapter adapter = list.getAdapter();

            //Check if lists adapter is a wrapper
            if (adapter instanceof WrapperListAdapter) {
                adapter = ((WrapperListAdapter) adapter).getWrappedAdapter();
            }
            if (adapter instanceof ItemViewAdapter) {
                ((ItemViewAdapter) adapter).notifyDataSetChanged(); //New data added so notify adapter
            }

            //Open a dialog to confirm that a new item was added
            MyDialogFragment okDialog = MyDialogFragment.newInstance("Success","New item was added succesfully.","Ok","");
            okDialog.show(getFragmentManager(),"ok");

            return true;
        }

        //TODO Something went wrong throw error?

        return false;
    }

    //Function handle for positive clicks in a dialog
    //Only the deletion dialog has an action that needs to be performed if user conforms to the message
    //Implements function defined in MyDialogFragment.DialogListener
    @Override
    public void onDialogPositiveClick(DialogFragment dialog) {
        //Check dialogs tag to determine what dialog is activated
        if( dialog.getTag().equals(DEL_DIALOG_TAG) )
        {
            deletePrimedItem();
        }
    }


    /**
     * A {@link FragmentPagerAdapter} that returns a fragment corresponding to
     * one of the sections/tabs/pages.
     */
    public class SectionsPagerAdapter extends FragmentPagerAdapter {

        public SectionsPagerAdapter(FragmentManager fm) {
            super(fm);
        }


        @Override
        public Fragment getItem(int position) {
            switch (position) {
                case 1:
                    //Create item adding view
                    return AddItemFragment.newInstance();
                case 0:
                    //Create the ItemView
                    return Items.newInstance(ledger_);
            }
            return null;
        }

        @Override
        public int getCount() {
            return 2;
        }

        @Override
        public CharSequence getPageTitle(int position) {
            switch (position) {
                case 0:
                    return "Ledger";
                case 1:
                    return "Add Item";
            }
            return null;
        }
    }
}
