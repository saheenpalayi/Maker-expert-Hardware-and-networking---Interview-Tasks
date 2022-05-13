# Maker expert ( Hardware and networking) - Interview Tasks

1)  Which is the best GSM module can be used in a arduino Project and which one is the best Arduino GSM Library ?

 - ### Best GSM Module
 
     **SIM900 is the best module** I ever used and also, this is most recommended  from the hardware maker communities 
        SIM800L is the cheapest one but not that easy to use like SIM900 and required special power supply ,also have trouble with range and connection issue sometimes.

- ### Best GSM Arduino Library

    From My opinion GSM modules are not That hard to Interface. It's easy to talk through the AT commands of the particular module and using the Arduino software serial Library I always create a function to Identify the commands back from the GSM module to make the code more optimize and to use less memory (less memmory use will help to use smaller Microcontroller instead of big to cut coast)

    These are the few libraries found on Internet for interfacing GSM modules 
    - GSM.H
    - TinyGSM.h
     - GSM_Generic.h

    The **GSM.H** is supported by Arduino itself and available in Arduino IDE newer version Its also a nice library but did not updated after 2019

    The **tinyGSM.H** is an awesome and up to dated Arduino library and popular in many arduino project. I also remember I helped a student in our community regarding a GSM based project (IoT based GAS cylinder monitor) wit this tinyGSM library

    And the **GSM_Generic** is newly committed Arduino Library which  supports various and new  arduino hardwares .But the popularity is slowly gaining I've to give it a try to test the library.The documentation looks great and supports lots of hardware platforms

    ### tinyGSM.H is the Best I found for Arduino Projects

- References
    - [GSM.H](https://docs.arduino.cc/retired/archived-libraries/GSM)
    - [TinyGSM.h](https://www.arduinolibraries.info/libraries/tiny-gsm)
    - [GSM_Generic.h](https://www.arduinolibraries.info/libraries/gsm_generic)

 - Sources

    - [GSM.H](https://github.com/arduino-libraries/GSM)
    - [TinyGSM.h](https://github.com/vshymanskyy/TinyGSM)
    - [GSM_Generic.h](https://github.com/khoih-prog/GSM_Generic)




2) Show a large arduino program You created (300+ lines of code)
 I've attached a Project folder named 'IoT based Parking System' .It's a n IoT project That I built for helping some school student and it has two hardware parts an arduino and a NodeMCU(ESP8266). I've also added the codes of both hardware and block diagram as well in the folder. [click Here](./IoT%20based%20Parking%20System/)

3) Create a Class Object for an arduino program of a Robotic arm which has a child function for the angles of the robotic arm for driving the arm 

     I've Many doubts about the question so I've created a new arduino sketch with the available information I got and Declared a class and some function for the RoboticArm. [check here](./Arduino-class-example/Arduino-class-example.ino)