#include<ESP8266WiFi.h>
#include<DNSServer.h>
#include<ESP8266WebServer.h>
#include<WiFiManager.h>
#include "FirebaseESP8266.h"
#define ap D2
#define FIREBASE_HOST "" // add firebase 
#define FIREBASE_AUTH ""  // add auth code here
//int sdata=0;
String mystring = "";
boolean Ready = false;
boolean statuschange = false;
String booking = "111111";
String mystring1 = "";
char rdata;
int realtimearray[6] = {1, 1, 1, 1, 1, 1};
FirebaseData firebaseData;
void setup() {
  Serial.begin(9600);
  pinMode(ap, INPUT);
  //   pinMode(A0,INPUT);
  WiFiManager wifimanager;
  Serial.print("connecting..");
  wifimanager.autoConnect("parking", "12345");
  Serial.println("*WM:wifi connected");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
  delay(100);
  if (Firebase.getBool(firebaseData, "/readystatus")) {
    Serial.println(firebaseData.dataType());

    if (firebaseData.dataType() == "boolean") {

      Serial.println(firebaseData.boolData());
      Ready = firebaseData.boolData();
    }

  } else {
    Serial.println(firebaseData.errorReason());
  }
  delay(100);
  if (Ready)
  {
    Serial.println("*WM:Web_connected");
  }

}


void loop() {

  if (Ready)
  {

    if (Serial.available() > 0)
    {
      rdata = Serial.read();
      mystring1 = mystring1 + rdata;
      //    Serial.println(rdata);
      if ( rdata == '\n')
      {
        Serial.println(mystring1);
        realtimestatus(mystring1);
        mystring1 = "";
      }
    }
    else
    {
      if (Firebase.readStream(firebaseData))
      {
        getbookspacestatus();
      }
    }
    if (statuschange)
    {
      Serial.print("*node:");
      Serial.println(booking);
      statuschange = false;
    }


  }

}
void getbookspacestatus()
{
  String path = "";
  //        Serial.println("hi");

  for (int i = 0; i <= 5; i++)
  {
    path = path + "/bookparks/space" + (i + 1);
    //  Serial.println(path);
    if (Firebase.getString(firebaseData, path)) {
      //    Serial.println(firebaseData.dataType());

      if (firebaseData.dataType() == "string") {
        //      Serial.println(firebaseData.stringData());
        mystring = mystring + firebaseData.stringData();
      }

    } else {
      Serial.println(firebaseData.errorReason());
      //    if(firebaseData.errorReason()=="read Timeout")
      //    {
      //      i=6;
      //      Serial.println(i);
      //
      //    }

    }
    path = "";
  }



  if ((mystring.length()) == 6)
  {
    for (int i = 0; i <= 5; i++)
    {
      if (mystring[i] != booking[i])
      {
        statuschange = true;
        booking[i] = mystring[i];
      }

    }
  }
  mystring = "";
}



void realtimestatus(String s)
{

  String m = "";
  String bucket = "";
  int j = 0;
  int k = s.length() - 1;
  for (int i = 0; i <= k; i++)
  {
    m = m + s[i];
    if (s[i] == ':')
    {
      j = i + 1;
      i = k;
    }

  }
  if (m == "*real:")
  {
    int data = 0;
    for (int l = 0; l <= 5; l++)
    {
      bucket = bucket + "realtimereading/space" + (l + 1);
      data = s[l + 6] - 48;



      Firebase.setInt(firebaseData, bucket, data);
      bucket = "";
    }

    m = "";
    s = "";
  }
  if (m == "*book:")
  {
    String data = "";
    for (int l = 0; l <= 5; l++)
    {
      bucket = bucket + "bookparks/space" + (l + 1);
      data = data + s[l + 6];



      if (Firebase.setString(firebaseData, bucket, data))
      {
        data = "";
      }
      bucket = "";
    }

    m = "";
    s = "";
  }
  if (m == "*config:")
  {
    wifIconfig();
    m = "";
    s = "";
  }
}




void wifIconfig()
{
  WiFiManager wifiManager;
  if (!wifiManager.startConfigPortal("bookparks")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    ESP.reset();
    delay(5000);
  }
  Serial.println("connected...:)");
}
