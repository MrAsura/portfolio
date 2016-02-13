package layout;


import android.content.Context;

import android.os.Bundle;
import android.support.v4.app.Fragment;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.WrapperListAdapter;


import jaakko.myledger.ItemViewAdapter;
import jaakko.myledger.LedgerModel;
import jaakko.myledger.R;

/**
 * A simple {@link Fragment} subclass.
 * Activities that contain this fragment must implement the
 * {@link Items.OnFragmentInteractionListener} interface
 * to handle interaction events.
 * Use the {@link Items#newInstance} factory method to
 * create an instance of this fragment.
 */
public class Items extends Fragment {

    //Define argument name for data model used to generate the items
    private static final String MODEL = "model_";

    //The data model used to generate the items
    private LedgerModel model_;

    private OnFragmentInteractionListener mListener; //Communicate with the main activity

    public Items() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param model data model that contains the items for the list view.
     * @return A new instance of fragment Items.
     */
    public static Items newInstance( LedgerModel model ) {
        Items fragment = new Items();
        Bundle args = new Bundle();
        args.putSerializable(MODEL, model);
        fragment.setArguments(args);
        return fragment;
    }


    //Try to recover old data if it exists
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        if (getArguments() != null) {
            model_ = (LedgerModel) getArguments().getSerializable(MODEL);
        }
    }


    //Prepare and return the items view
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        // Inflate the layout for this fragment
        View fragView = inflater.inflate(R.layout.fragment_items, container, false);

        //Set adapter for the item list view
        ListView listV = (ListView) fragView.findViewById(R.id.item_view);
        ItemViewAdapter adapter = new ItemViewAdapter(getActivity(),model_);
        listV.setAdapter(adapter);

        //Connect item click listener to onButtonPressed. Click is triggered in button click event set in the adapter.
        listV.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                onButtonPressed(view);
            }
        });

        return fragView;
    }

    //Make sure the list is up to date when resuming fragment
    @Override
    public void onResume(){
        super.onResume();
        //Update the state of the ListView to show possible changes to the data
        //Probably not strictly necessary and is called every time a dialog is shown etc.
        //How ever if the data is changed outside of the program, may be needed to update data
        ListView list = (ListView) getActivity().findViewById(R.id.item_view);
        ListAdapter adapter = list.getAdapter();

        //Check if lists adapter is a wrapper
        if (adapter instanceof WrapperListAdapter) {
            adapter = ((WrapperListAdapter) adapter).getWrappedAdapter();
        }
        if (adapter instanceof ItemViewAdapter) {
            ((ItemViewAdapter) adapter).notifyDataSetChanged();
        }
    }

    // Event for when a deletion button is pressed
    public void onButtonPressed(View view) {
        if (mListener != null) {
            //The position of the clicked button is contained in the tag of the view
            mListener.onItemDelEvent((Integer) view.getTag());
        }
    }

    //Connect to activity and check the interface has been implemented
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
     * This is the interface for handling button clicks
     */
    public interface OnFragmentInteractionListener {
        // Send the position of the item to be deleted
        void onItemDelEvent(int position);
    }
}
