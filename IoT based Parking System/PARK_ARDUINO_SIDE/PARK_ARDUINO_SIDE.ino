#include<SoftwareSerial.h>
#include<Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display
SoftwareSerial nodemcu(2, 3);
boolean entryflag = false;
boolean exitflag = false;
boolean statuschange = true;
boolean listening = true;
boolean recvflag = false;
boolean blynkflag = false;
boolean booking = false;
boolean Ready = false;
String mystring = "";
String s = "";
char rdata;
int a = 0;
int parked = 0;
int unparked = 0;
int entrycount = 0;
int x = 0;
Servo gate;
unsigned long crnttime = 0;
unsigned long prvtime = 0;
unsigned long prvtime1 = 0;
int availspace = 0;
int statusarray[6] = {1, 1, 1, 1, 1, 1};
int changestatusarray[6] = {1, 1, 1, 1, 1, 1};
int realtimearray[6] = {1, 1, 1, 1, 1, 1};
int bookingarray[6] = {1, 1, 1, 1, 1, 1};
boolean gates = false;
void setup() {
  Serial.begin(9600);
  nodemcu.begin(9600);
  lcd.init();
  lcd.init();
  lcd.backlight();
  pinMode(4, OUTPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  gate.attach(10);
  pinMode(11, INPUT);
  pinMode(12, INPUT);
  pinMode(13, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A6, INPUT);
  ledstatus(0, true);
  gate.write(0);
  readstatus();
  changestatus();
  arrayresults();
  entrycount = 6 - availspace;
  parked = entrycount;
  gateservo();

}

void loop() {
  if (Ready)
  {
    gateservo();

    crnttime = millis();

    if (nodemcu.available() > 0 && listening)
    {
      rdata = nodemcu.read();
      mystring = mystring + rdata;
      //    Serial.println(rdata);
      if ( rdata == '\n')
      {
        //     Serial.println(mystring);
        s = mystring;
        mystring = "";
        //     listening = false;
      }
    }
    else
    {
      //          Serial.println("hi");
      String data = "";
      if (statuschange || booking)
      {
        for (int i = 0; i < 6; i++)
        {
          data = data + realtimearray[i];
        }
        //                  Serial.println(data.length());

        if (data.length() == 6)
        {
          Serial.print("*real:");
          nodemcu.print("*real:");
          nodemcu.println(data);
          Serial.println(data);
          data = "";
          statuschange = false;
          booking = false;
        }
      }
    }
    unparked = entrycount - parked;
    if (entrycount != 0)
    {
      readstatus();
    }

    noderead();


    if (digitalRead(11) == LOW && !gates && !exitflag && availspace != 0 && entrycount < 7)
    {
      delay(500);
      if (digitalRead(11) == LOW)
      {
        gate.write(90);
        delay(15);
        x = 90;
        entryflag = true;
        gates = true;
      }
    }

    if (digitalRead(12) == HIGH && !gates && !entryflag && entrycount > 0 && parked < entrycount )
    {
      delay(600);
      if (digitalRead(12) == HIGH)
      {
        gate.write(90);
        delay(15);
        x = 90;
        exitflag = true;
        gates = true;
      }

    }
    if (digitalRead(12) == HIGH && entryflag && gates)
    {
      gate.write(0);
      delay(15);
      x = 0;
      entryflag = false;
      gates = false;
      entrycount++;

    }

    if (digitalRead(11) == LOW && exitflag && gates)
    {
      gate.write(0);
      delay(15);
      x = 0;
      exitflag = false;

      gates = false;
      entrycount--;
    }
    arrayresults();
    if (crnttime - prvtime >= 1000)
    {
      a++;
      prvtime = crnttime;
    }
    displays();
    changestatus();
  }
  else
  {
    if (nodemcu.available() > 0)
    {
      rdata = nodemcu.read();
      mystring = mystring + rdata;
      //    Serial.println(rdata);
      if ( rdata == '\n')
      {
        //     Serial.println(mystring);
        s = mystring;
        String m = "";
        String message = "";
        int j = 0;
        int k = s.length() - 3;
        for (int i = 0; i <= k; i++)
        {
          m = m + s[i];
          if (s[i] == ':')
          {
            j = i + 1;
            i = k;
          }

        }
        if (m == "*WM:")
        {
          //    Serial.println(m);
          for (int i = j; i <= k; i++)
          {
            message = message + s[i];
          }

          if (message.length() < 16 && message.length() > 3)
          {
            lcd.setCursor(0, 0);
            lcd.print(message);
            delay(500);
          }
          if ( message == "Web_connected")
          {
            Ready = true;
          }
          Serial.println(message);

          m = "";
          s = "";

        }
        mystring = "";
      }
    }
  }
}


void readstatus()
{
  if (analogRead(A6) <= 650)
  {
    statusarray[0] = 0;
  }
  else
  {
    statusarray[0] = 1;
  }

  if (digitalRead(5) == LOW)
  {
    statusarray[1] = 0;
  }
  else
  {
    statusarray[1] = 1;
  }
  if (digitalRead(6) == LOW)
  {
    statusarray[2] = 0;
  }
  else
  {
    statusarray[2] = 1;
  }
  if (digitalRead(7) == LOW)
  {
    statusarray[3] = 0;
  }
  else
  {
    statusarray[3] = 1;
  }
  if (digitalRead(8) == LOW)
  {
    statusarray[4] = 0;
  }
  else
  {

    statusarray[4] = 1;
  }
  if (digitalRead(9) == LOW)
  {
    statusarray[5] = 0;
  }
  else
  {
    statusarray[5] = 1;
  }
}
void arrayresults()
{

  if (!gates && recvflag )
  {
    for (int i = 0; i <= 5; ++i)
    {
      //      if(realtimearray[i]!=changestatusarray[i])
      //      {
      if (realtimearray[i] == 1 && statusarray[i] == 1)
      {
        bookingarray[i] = changestatusarray[i];

        realtimearray[i] = changestatusarray[i];
        booking = true;
      }
      else
      {
        changestatusarray[i] = 1;
        bookingarray[i] = 1;
      }
      //      }
      //      else
      //      {
      //        changestatusarray[i]=1;
      //      }

    }
  }
  else
  {
    if (recvflag)
    {
      String data = "";

      for (int i = 0; i < 6; i++)
      {
        data = data + bookingarray[i];
      }

      if (data.length() == 6)
      {
        Serial.print("*book:");
        nodemcu.print("*book:");
        nodemcu.println(data);
        Serial.println(data);
        data = "";
      }
    }
  }
  recvflag = false;
  int space = 0;
  for (int i = 0; i <= 5; ++i)
  {
    if (realtimearray[i] == 1)
    {
      ledstatus(i, true);
      space++;
    }
    else
    {
      if (statusarray[i] == 0 && changestatusarray[i] == 0)
      {
        blynk(i);
      }
      else
      {

        ledstatus(i, false);
      }
    }


  }
  availspace = space;
}
void ledstatus(int ls, boolean state) {
  switch (ls)
  {
    case 0:
      digitalWrite(13, state);
      break;
    case 1:
      digitalWrite(A0, state);
      break;
    case 2:
      digitalWrite(A1, state);
      break;
    case 3:
      digitalWrite(A2, state);
      break;
    case 4:
      digitalWrite(A3, state);
      break;
    case 5:
      digitalWrite(4, state);
      break;
    default:
      digitalWrite(13, state);
      digitalWrite(A0, state);
      digitalWrite(A1, state);
      digitalWrite(A2, state);
      digitalWrite(A3, state);
      digitalWrite(4, state);
      break;
  }



}

void displays()
{

  switch (a)
  {

      break;
    case 0:
      lcd.setCursor(0, 0);
      lcd.print("    WELCOME      ");
      break;
    case 1:
      lcd.setCursor(0, 0);
      lcd.print("      TO       ");
      break;
    case 2:
      lcd.setCursor(0, 0);
      lcd.print("   BookParks   ");
      break;
    case 3:
      if (availspace != 0)
      {
        lcd.setCursor(0, 0);
        lcd.print("Available Spaces");
        lcd.setCursor(0, 1);
        lcd.print("   ");
        for (int i = 0; i <= 5; i++)
        {
          if (realtimearray[i] == 1)
          {
            lcd.print(i + 1);
          }
          else {
            lcd.print("-");
          }
          lcd.print(" ");
        }
        lcd.print("     ");
      }
      else
      {
        lcd.setCursor(0, 1);
        lcd.print("             ");
        delay(200);
        a = 5;
      }
      break;
    case 5:
      a = 0;
      if (availspace != 0)
      {
        lcd.setCursor(0, 1);
        lcd.print(availspace);
        lcd.print(" lot available ");
      }
      else
      {
        lcd.setCursor(0, 1);
        lcd.print("    ALL FULL      ");
        delay(200);
        lcd.setCursor(0, 1);
        lcd.print("             ");
        delay(200);
        lcd.setCursor(0, 1);
        lcd.print("    ALL FULL  ");
      }
      break;

  }

}
void changestatus()
{


  for (int i = 0; i <= 5; ++i)
  {
    if (statusarray[i] != realtimearray[i])
    {
      if (statusarray[i] == 0)
      {
        parked++;
        statuschange = true;

        //        Serial.println("parking...");
      }
      if (statusarray[i] == 1 && parked != 0)
      {
        parked--;
        statuschange = true;

        //        Serial.println("unparking...");
      }
      if (changestatusarray[i] == 1)
      {
        realtimearray[i] = statusarray[i];
        statuschange = true;

      }

    }
  }
}


void noderead()
{
  String m = "";
  String message = "";
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

  //  Serial.println(k);
  if (m == "*vM:")
  {
    //    Serial.println(m);
    for (int i = j; i <= k; i++)
    {
      message = message + s[i];
    }
    Serial.println(message);
    m = "";
    s = "";

  }
  if (m == "*node:")
  {

    for (int l = 0; l <= 5; l++)
    {
      changestatusarray[l] = (s[l + 6] - 48);
      Serial.println(changestatusarray[l]);
    }
    m = "";
    s = "";
    recvflag = true;
  }

}
void blynk(int d)
{

  if (crnttime - prvtime1 >= 100)
  {
    blynkflag = !blynkflag;
    prvtime1 = crnttime;
  }
  ledstatus(d, blynkflag);

}

void gateservo()
{
  gate.write(x);
  delay(15);
}
