package com.example.demo;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
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

public class Main2Activity extends AppCompatActivity  {
    Button hengio;
    TimePicker timePicker;
    TimePicker timePicker2;
    TextView textView;
    Calendar calendar;
    DatabaseReference mData;
    Spinner spinnertuoi;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main2);

        mData = FirebaseDatabase.getInstance().getReference();

        hengio = (Button)findViewById(R.id.hengio);
        textView = (TextView)findViewById(R.id.textView);
        timePicker = (TimePicker)findViewById(R.id.Timepicker);
        timePicker2 = (TimePicker) findViewById(R.id.Timepicker2);
        calendar = Calendar.getInstance();
        spinnertuoi = (Spinner)findViewById(R.id.spinnertuoi);

        ArrayList<String> arraydays = new ArrayList<>();
        arraydays.add("Monday");
        arraydays.add("Tuesday");
        arraydays.add("Wednesday");
        arraydays.add("Thursday");
        arraydays.add("Friday");
        arraydays.add("Saturday");
        arraydays.add("Sunday");

        ArrayAdapter arrayAdapter = new ArrayAdapter(this, android.R.layout.simple_spinner_dropdown_item, arraydays);
        spinnertuoi.setAdapter(arrayAdapter);

        hengio.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                calendar.set(Calendar.HOUR_OF_DAY,timePicker.getCurrentHour());
                calendar.set(Calendar.MINUTE,timePicker.getCurrentMinute());

                int gio = timePicker.getCurrentHour();
                int phut = timePicker.getCurrentMinute();
                int gio2 = timePicker2.getCurrentHour();
                int phut2 = timePicker2.getCurrentMinute();

                String string_gio = String.valueOf(gio);
                String string_phut = String.valueOf(phut);
                String string_gio2 = String.valueOf(gio2);
                String string_phut2 = String.valueOf(phut2);


                mData.child("gio").setValue(gio);
                mData.child("phut").setValue(phut);
                mData.child("gio2").setValue(gio2);
                mData.child("phut2").setValue(phut2);

                textView.setText("Gio ban dat la: "+ string_gio + " h " + string_phut
                        + " va gio ban dat tiep theo la: " + string_gio2 + " gio " + string_phut2);
            }
        });
        spinnertuoi.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                String lich = spinnertuoi.getSelectedItem().toString();
                mData.child("lich").setValue(lich);
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });

    }
}
