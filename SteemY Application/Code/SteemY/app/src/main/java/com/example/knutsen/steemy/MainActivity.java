package com.example.knutsen.steemy;

import android.content.Intent;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.content.Context;
import android.graphics.Typeface;
import android.util.AttributeSet;
import android.widget.TextView;



public class MainActivity extends AppCompatActivity {
    private static int SPLASH_TIME_OUT = 4000;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Typeface myTypeface = Typeface.createFromAsset(getAssets(), "handwriting.tff");
        TextView steemyText = findViewById(R.id.steemy);
        steemyText.setTypeface(myTypeface);


        new Handler().postDelayed(new Runnable() {
            @Override
            public void run()
            {
                Intent homeIntent = new Intent(MainActivity. this, HomeActivity.class);
                startActivity(homeIntent);
                finish();
            }
        },SPLASH_TIME_OUT);




    }


}
