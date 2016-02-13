package layout;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.DialogInterface;
import android.os.Bundle;

/**
 * Created by Jaakko on 11.2.2016.
 * Dialog Fragment used for displaying dialogs to the user. This class is used to show delete confirmations
 * and item adding errors.
 */
public class MyDialogFragment extends DialogFragment {

    //Interface for notifying activity of the users decision
    public interface DialogListener{
        void onDialogPositiveClick(DialogFragment dialog);
    }

    //Use to send events to activity
    DialogListener mListener;

    //Arg names for the bundle
    private static final String TITLE_ARG = "title";
    private static final String MSG_ARG = "message";
    private static final String POS_ARG = "positive";
    private static final String NEG_ARG = "negative";

    //The content of the dialog
    private String mTitle;
    private String mMsg;
    private String mPos;
    private String mNeg;

    //Provide a newInstance of the dialog. Takes as an argument the title, message and possible button text.
    //If a button text is empty, the respective button is not created
    public static MyDialogFragment newInstance(String title, String msg, String positiveBtn, String negativeBtn)
    {
        MyDialogFragment f = new MyDialogFragment();

        //Add the parameters in to a bundle
        Bundle args = new Bundle();
        args.putString(TITLE_ARG,title);
        args.putString(MSG_ARG,msg);
        args.putString(POS_ARG,positiveBtn);
        args.putString(NEG_ARG, negativeBtn);
        f.setArguments(args);

        return f;
    }

    //Overide onCreateDialog to read arguments
    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState )
    {
        if( getArguments() != null )
        {
            mTitle = getArguments().getString(TITLE_ARG);
            mMsg = getArguments().getString(MSG_ARG);
            mPos = getArguments().getString(POS_ARG);
            mNeg = getArguments().getString(NEG_ARG);
        }

        //Build dialog
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        builder.setTitle(mTitle).setMessage(mMsg);

        //Select buttons to use
        if( !mPos.equals(""))
        {
            builder.setPositiveButton(mPos, new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int which) {
                    mListener.onDialogPositiveClick(MyDialogFragment.this);
                }
            });
        }
        if( !mNeg.equals("") )
        {
            builder.setNegativeButton(mNeg, new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int which) {
                    //Negative has no special effect
                }
            });
        }

        return builder.create();
    }

    //Override onAttach to instantiate the listener
    @Override
    public void onAttach(Activity activity)
    {
        super.onAttach(activity);

        //Verify that the listener interface is implemented
        try{
            //Connect to host
            mListener = (DialogListener) activity;
        }
        catch (ClassCastException e)
        {
            //Not implemnted. Throw error
            throw new ClassCastException(activity.toString()+" must implement DialogListener");
        }
    }


}
