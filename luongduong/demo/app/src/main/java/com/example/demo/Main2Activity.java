package com.example.demo;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.TimePicker;
import android.widget.Toast;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import java.sql.DatabaseMetaData;
import java.util.ArrayList;
import java.util.Calendar;

public class Main2Activity extends AppCompatActivity {
    Button hengio;
    TimePicker timePicker;
    TextView textView;
    Calendar calendar;
    DatabaseReference mData;
    Spinner spinnerdanhsach;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main2);

        mData = FirebaseDatabase.getInstance().getReference();

        hengio = (Button)findViewById(R.id.hengio);
        textView = (TextView)findViewById(R.id.textView);
        timePicker = (TimePicker)findViewById(R.id.Timepicker);
        calendar = Calendar.getInstance();
        spinnerdanhsach = (Spinner)findViewById(R.id.Spinner);

        final ArrayList<String> arrayVonglap = new ArrayList<String>();
        arrayVonglap.add("Once time ");
        arrayVonglap.add("Twice time");
        arrayVonglap.add("ba ngay moimet lan");
        arrayVonglap.add("mot tuan mot lan");
        arrayVonglap.add("mot thang mot lan");

        final ArrayAdapter arrayAdapter = new ArrayAdapter(this, android.R.layout.simple_spinner_item,arrayVonglap);
        arrayAdapter.setDropDownViewResource(android.R.layout.simple_spinner_item);

        spinnerdanhsach.setAdapter(arrayAdapter);
        spinnerdanhsach.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                Toast.makeText(Main2Activity.this, arrayVonglap.get(position), Toast.LENGTH_SHORT);
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {
            }
        });


        hengio.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                calendar.set(Calendar.HOUR_OF_DAY,timePicker.getCurrentHour());
                calendar.set(Calendar.MINUTE,timePicker.getCurrentMinute());

                int gio = timePicker.getCurrentHour();
                int phut = timePicker.getCurrentMinute();

                String string_gio = String.valueOf(gio);
                String string_phut = String.valueOf(phut);

                mData.child("time").child("gio").setValue(gio);
                mData.child("time").child("phut").setValue(phut);

                textView.setText("Gio ban dat la: "+ string_gio + " h " + string_phut);

            }
        });

    }
}
