package com.example.shciot

import android.content.Intent
import android.net.Uri
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.Button

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val button1: Button=findViewById(R.id.button1)

        button1.setOnClickListener {
            val openUrl= Intent(android.content.Intent.ACTION_VIEW)
            openUrl.data= Uri.parse("http://iotsmarthealthcare.com/User_1.php?")

            startActivity(openUrl)
        }

        val button2: Button=findViewById(R.id.button2)

        button2.setOnClickListener {
            val openUrl= Intent(android.content.Intent.ACTION_VIEW)
            openUrl.data= Uri.parse("http://iotsmarthealthcare.com/User_2.php?")

            startActivity(openUrl)
        }

        val button3: Button=findViewById(R.id.button3)

        button3.setOnClickListener {
            val openUrl= Intent(android.content.Intent.ACTION_VIEW)
            openUrl.data= Uri.parse("http://iotsmarthealthcare.com/User_3.php?")

            startActivity(openUrl)
        }

        val button4: Button=findViewById(R.id.button4)

        button4.setOnClickListener {
            val openUrl= Intent(android.content.Intent.ACTION_VIEW)
            openUrl.data= Uri.parse("http://iotsmarthealthcare.com/User_4.php?")

            startActivity(openUrl)
        }
    }
}