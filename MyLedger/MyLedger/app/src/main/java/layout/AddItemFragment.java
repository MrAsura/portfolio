package layout;

import android.app.Activity;
import android.content.Context;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;

import jaakko.myledger.R;

/**
 * A simple {@link Fragment} subclass.
 * Activities that contain this fragment must implement the
 * {@link AddItemFragment.OnFragmentInteractionListener} interface
 * to handle interaction events.
 * Use the {@link AddItemFragment#newInstance} factory method to
 * create an instance of this fragment.
 */
public class AddItemFragment extends Fragment {

    //Used for communication with the main activity
    private OnFragmentInteractionListener mListener;

    public AddItemFragment() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @return A new instance of fragment AddItemFragment.
     */
    // No arguments needed
    public static AddItemFragment newInstance() {
        AddItemFragment fragment = new AddItemFragment();
        Bundle args = new Bundle();
        fragment.setArguments(args);
        return fragment;
    }

    //Prepare and return the view for adding new items
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View view = inflater.inflate(R.layout.fragment_add_item, container, false);
        // Get the button and add a click listener to it
        Button addButton = (Button) view.findViewById(R.id.add_button);
        addButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                onButtonPressed(v);
            }
        });

        return view;
    }

    // This function is called when add item button is clicked
    public void onButtonPressed(View view) {
        if (mListener != null) {
            // Notify activity of event
            Activity activity = getActivity();
            EditText name = ((EditText) activity.findViewById(R.id.name_edit));
            EditText iban = ((EditText) activity.findViewById(R.id.iban_edit));

            if( mListener.onItemAddEvent(name.getText().toString(),iban.getText().toString()) )
            {
                //The item was added successfully so reset the text field
                name.setText("");
                iban.setText("");
            }
        }
    }

    //Make connect to activity and make sure the interface is defined
    @Override
    public void onAttach(Context context) {
        super.onAttach(context);
        if (context instanceof OnFragmentInteractionListener) {
            mListener = (OnFragmentInteractionListener) context;
        } else {
            throw new RuntimeException(context.toString()
                    + " must implement OnFragmentInteractionListener");
        }
    }

    @Override
    public void onDetach() {
        super.onDetach();
        mListener = null;
    }

    /**
     * Use this interface to notify the activity when the user clicks the add item button
     */
    public interface OnFragmentInteractionListener {
        //Send the name and iban value entered by the user.
        //Should return information on whether the addition was successful or not
        Boolean onItemAddEvent(String name, String iban);
    }
}
