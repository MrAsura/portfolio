package jaakko.myledger;

import android.app.Activity;
import android.util.Pair;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;

import java.util.ArrayList;

/**
 * Created by Jaakko on 10.2.2016.
 */
public class ItemViewAdapter extends BaseAdapter {

    //Define internal variables for storing important stuff
    LedgerModel items_;
    Activity activity_;


    public ItemViewAdapter( Activity activity, LedgerModel items){
        super();
        items_ = items;
        activity_ = activity;
    }

    /**
     * How many items are in the data set represented by this Adapter.
     *
     * @return Count of items.
     */
    @Override
    public int getCount() {
        return items_.length();
    }

    /**
     * Get the data item associated with the specified position in the data set.
     *
     * @param position Position of the item whose data we want within the adapter's
     *                 data set.
     * @return The data at the specified position.
     */
    @Override
    public Pair<String,String> getItem(int position) {
        return items_.get(position);
    }

    /**
     * Get the row id associated with the specified position in the list.
     *
     * @param position The position of the item within the adapter's data set whose row id we want.
     * @return The id of the item at the specified position.
     */
    @Override
    public long getItemId(int position) {
        return 0;
    }

    /**
     * Get a View that displays the data at the specified position in the data set. You can either
     * create a View manually or inflate it from an XML layout file. When the View is inflated, the
     * parent View (GridView, ListView...) will apply default layout parameters unless you use
     * {@link LayoutInflater#inflate(int, ViewGroup, boolean)}
     * to specify a root view and to prevent attachment to the root.
     *
     * @param position    The position of the item within the adapter's data set of the item whose view
     *                    we want.
     * @param convertView The old view to reuse, if possible. Note: You should check that this view
     *                    is non-null and of an appropriate type before using. If it is not possible to convert
     *                    this view to display the correct data, this method can create a new view.
     *                    Heterogeneous lists can specify their number of view types, so that this View is
     *                    always of the right type (see {@link #getViewTypeCount()} and
     *                    {@link #getItemViewType(int)}).
     * @param parent      The parent that this view will eventually be attached to
     * @return A View corresponding to the data at the specified position.
     */
    @Override
    public View getView(int position, View convertView, ViewGroup parent) {

        LayoutInflater inflater = activity_.getLayoutInflater(); //Used to load views from xml
        ViewHolder holder = null;

        //Check if convertView can be reused
        if( convertView == null )
        {
            convertView = inflater.inflate(R.layout.item_row,null);
            holder = new ViewHolder();

            holder.name = (TextView) convertView.findViewById(R.id.text_name);
            holder.iban = (TextView) convertView.findViewById(R.id.text_iban);
            holder.del = (Button) convertView.findViewById(R.id.del);

            //Inner class needs final variables to reference
            final ViewGroup par = parent;
            final int pos = position;

            //Add event listeners to buttons
            holder.del.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    ((ListView) par).performItemClick(v,pos,0);
                }
            });

            convertView.setTag(holder);
        }
        else {
            holder = (ViewHolder) convertView.getTag();
        }

        Pair<String,String> data = getItem(position);
        holder.name.setText(data.first);
        holder.iban.setText(data.second);
        holder.del.setTag(position);

        return convertView;
    }

    //Implement a view holder class for storing and recycling views
    private static class ViewHolder{
        public TextView name;
        public TextView iban;
        public Button del;
    }
}
