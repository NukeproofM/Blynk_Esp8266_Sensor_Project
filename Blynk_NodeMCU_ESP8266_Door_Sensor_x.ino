// Author : maysoEngineering

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID           ". . ."
#define BLYNK_TEMPLATE_NAME         ". . ."
#define BLYNK_AUTH_TOKEN            ". . ."

#define DOOR_SENSOR_PIN  D7  // The ESP8266 pin D7 connected to door sensor's pin

// Comment this out to disable prints and save space 
// #define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Your WiFi credentials.
// Set password to "" for open networks.
const char ssid[] = ". . .";
const char pass[] = "";

BlynkTimer timer;

int door_state; // value read off of our digital input pin
bool open_or_closed; // value of the state of the sensor
bool prev_state; // value of previous state of the sensor

// This function is called every time the device is connected to the Blynk.Cloud
//BLYNK_CONNECTED(){ // leaving here so you or I can inject code later; perhaps to help with troubleshooting Blynk cloud connection
//}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent(){
  // You can send any value at any time.
  // Please don't send more that 10 values per second ! ! ! ! 
  Blynk.virtualWrite(V2, millis() / 1000);
}

// This function sends the state of the sensor to Virtual Pin 1 AND handles open/closed notifications
void sensorState(){
  door_state = digitalRead(DOOR_SENSOR_PIN); // read state
  if (door_state == HIGH) {
    // Serial.println("The door is open");
    open_or_closed = 1;
  } else {
    // Serial.println("The door is closed");
    open_or_closed = 0;
  }

  if(prev_state == 1 && open_or_closed == 0){ // this checks condition of an OPEN->CLOSED state update
    // recall if previous state is 1 (open) and the current state is 0 (closed) our condition is satisfied
    Blynk.logEvent("sensor_closed"); // if so, notify Blynk of this specific state change; Alert to user that sensor is closed ONCE 
  }
   if(prev_state == 0 && open_or_closed == 1){ // this checks condition of an CLOSED->OPEN state update
    // recall if previous state is 0 (closed) and the current state is 1 (open) our condition is satisfied
    Blynk.logEvent("sensor_open"); // if so, notify Blynk of this specific state change; Alert to user that sensor is opened ONCE
  }
  
  prev_state = open_or_closed; // set previous state with our current state so we may
  // compare once again after the current state is updated again

  // Please don't send more that 10 values per second - just something to remember!
  Blynk.virtualWrite(V1, open_or_closed); // open_or_closed is a bool; 1 is open, 0 is closed. Send current state to Blynk.
  // open_or_closed reflects door_state

}

void setup(){
  // Debug console
  Serial.begin(115200);
  pinMode(DOOR_SENSOR_PIN, INPUT_PULLUP); // Configure the ESP8266 pin to the input pull-up mode
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass); 
  // You can also specify server:
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", nn);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(nnn,xxx,n,xxx), zzzz);

  // Setup a function to be called every second - to send uptime to Blynk
  timer.setInterval(1000L, myTimerEvent);
  // Setup a function to be called every second - to send state of sensor to Blynk
  timer.setInterval(1000L, sensorState);

  prev_state = open_or_closed; // initialize the previous state as the current state of the system upon beginning
}

void loop(){
  Blynk.run();
  timer.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function! *
}