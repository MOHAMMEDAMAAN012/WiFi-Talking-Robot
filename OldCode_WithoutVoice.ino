// Motor driver pin definitions
#define ENA   14         // Enable pin for Motor A
#define ENB   12         // Enable pin for Motor B
#define IN_1  15         // Control pin 1 for Motor A
#define IN_2  13         // Control pin 2 for Motor A
#define IN_3  2          // Control pin 1 for Motor B
#define IN_4  0          // Control pin 2 for Motor B

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

// Global variables
String command;              // Stores received command
int speedCar = 800;          // Default speed for the motors
int speed_Coeff = 3;         // Speed coefficient for smooth turns

const char* ssid = " Project wifi car";  // WiFi SSID
ESP8266WebServer server(80);             // Create web server on port 80

void setup() {
  // Set motor control pins as output
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);  
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT); 

  Serial.begin(115200);  // Start serial communication for debugging

  // Set up ESP in Access Point (AP) mode
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);  // Start the AP with defined SSID

  // Display IP address of the AP
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
 
  // Define web server routes
  server.on("/", HTTP_handleRoot);          // Handle root path
  server.onNotFound(HTTP_handleRoot);       // Handle 404 (not found)
  server.begin();                           // Start web server
}

// Movement functions

// Move forward
void goAhead(){ 
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, speedCar);
}

// Move backward
void goBack(){ 
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar);
}

// Turn right
void goRight(){ 
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, speedCar);
}

// Turn left
void goLeft(){
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar);
}

// Move forward-right
void goAheadRight(){
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, speedCar / speed_Coeff);  // Slow left motor

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, speedCar);                // Normal speed on right
}

// Move forward-left
void goAheadLeft(){
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, speedCar / speed_Coeff);  // Slow right motor
}

// Move backward-right
void goBackRight(){ 
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar / speed_Coeff);  // Slow left motor

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar);                // Normal speed on right
}

// Move backward-left
void goBackLeft(){ 
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar / speed_Coeff);  // Slow right motor
}

// Stop the robot
void stopRobot(){  
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar);
}

// Main loop
void loop() {
  server.handleClient();  // Handle incoming client requests

  // Read and handle command from URL query
  command = server.arg("State");

  // Movement commands
  if (command == "F") goAhead();
  else if (command == "B") goBack();
  else if (command == "L") goLeft();
  else if (command == "R") goRight();
  else if (command == "I") goAheadRight();
  else if (command == "G") goAheadLeft();
  else if (command == "J") goBackRight();
  else if (command == "H") goBackLeft();

  // Speed control (0–9)
  else if (command == "0") speedCar = 400;
  else if (command == "1") speedCar = 470;
  else if (command == "2") speedCar = 540;
  else if (command == "3") speedCar = 610;
  else if (command == "4") speedCar = 680;
  else if (command == "5") speedCar = 750;
  else if (command == "6") speedCar = 820;
  else if (command == "7") speedCar = 890;
  else if (command == "8") speedCar = 960;
  else if (command == "9") speedCar = 1023;

  // Stop
  else if (command == "S") stopRobot();
}

// Handles incoming HTTP requests
void HTTP_handleRoot(void) {
  // Print command to Serial if present
  if(server.hasArg("State")) {
    Serial.println(server.arg("State"));
  }

  // Respond with empty page (can be updated for GUI)
  server.send(200, "text/html", "");
  delay(1);
}
