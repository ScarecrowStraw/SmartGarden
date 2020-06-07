package com.example.demo;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.content.Intent;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import java.sql.DatabaseMetaData;

import static android.widget.Toast.LENGTH_SHORT;

public class MainActivity extends AppCompatActivity {

    TextView txtNoidung;

//    TextView txtDemo;

    Button btnClick;
    Button btnClick2;
    Button buttonTrangthai;
    Switch swtWifi;
    WifiManager wifiManager;

    DatabaseReference mData;
    boolean a = true;

    @Override

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mData = FirebaseDatabase.getInstance().getReference();

        // ánh xạ

        final Button btnClick2= findViewById(R.id.button);

        final Button buttonTrangthai = findViewById(R.id.buttonTrangthai);

        txtNoidung = (TextView) findViewById(R.id.textViewNoiDung);


        swtWifi = (Switch) findViewById(R.id.switchWifi);

        // viết code

        wifiManager = (WifiManager) getApplicationContext().getSystemService(Context.WIFI_SERVICE);
        if(wifiManager.isWifiEnabled()){
            swtWifi.setChecked(true);
        }
        else{
            swtWifi.setChecked(false);
        }

        swtWifi.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked) {
                    wifiManager.setWifiEnabled(true);
                    Toast.makeText(MainActivity.this, "bạn đã mở Wifi", LENGTH_SHORT).show();
                }
                else {
                    wifiManager.setWifiEnabled(false);
                    Toast.makeText(MainActivity.this, "bạn đã tắt Wifi", LENGTH_SHORT).show();
                }
            }
        });
        btnClick2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this, Main2Activity.class);
                startActivity(intent);
            }
        });
    }

}
