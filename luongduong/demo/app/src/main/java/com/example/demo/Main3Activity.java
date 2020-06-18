package com.example.demo;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import java.sql.DatabaseMetaData;


public class Main3Activity extends AppCompatActivity {
    TextView txtNhietdo;
    TextView txtDoamdat;
    TextView txtDoamkk;
    TextView txtAnhsang;
    DatabaseReference mData;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main3);

        mData = FirebaseDatabase.getInstance().getReference();
        txtNhietdo = (TextView) findViewById(R.id.textViewnhietdo);
        txtDoamkk = (TextView) findViewById(R.id.textViewdoamkk);
        txtDoamdat = (TextView) findViewById(R.id.textViewdoamdat);
        txtAnhsang = (TextView) findViewById(R.id.textViewAnhsang);
        final float doamdat = 0;
        mData.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {

                txtNhietdo.setText(dataSnapshot.child("Temperature_STATUS").getValue().toString());
                txtDoamkk.setText(dataSnapshot.child("Humidity_STATUS").getValue().toString());
                txtDoamdat.setText(dataSnapshot.child("DoAmDat_STATUS").getValue().toString());
                txtAnhsang.setText(dataSnapshot.child("LightMeter: ").getValue().toString());
                Log.d("recv",dataSnapshot.child("LightMeter: ").getValue().toString() );
//                doamdat.(dataSnapshot.child("DoAmDat_STATUS").getValue().toString());

            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {

            }
        });


    }
}