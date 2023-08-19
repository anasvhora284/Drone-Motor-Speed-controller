#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *ssid = "Drone";
const char *password = "nevergiveup";

const int udpPort = 12345;
WiFiUDP udp;

void calibrateESCs()
{
  // Send a MIN signal to all ESCs to calibrate them
  myservo1.writeMicroseconds(1000);
  myservo2.writeMicroseconds(1000);
  myservo3.writeMicroseconds(1000);
  myservo4.writeMicroseconds(1000);

  // Wait for 1 seconds for ESCs to be calibrated
  delay(1000);
}

void setup()
{

  Serial.begin(57600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  calibrateESCs();
 
  // Initialize UDP
  udp.begin(udpPort);

  // Initialize motor control pins
  myservo1.attach(15);
  myservo2.attach(12);
  myservo3.attach(13);
  myservo4.attach(14);
}

void loop()
{
  int packetSize = udp.parsePacket();
  if (packetSize) {
    char packetData[packetSize];
    udp.read(packetData, packetSize);

    // Parse and process the received command
    processCommand(packetData);

    // Check for "ping" and respond with "pong"
    if (strcmp(packetData, "ping") == 0) {
      udp.beginPacket(udp.remoteIP(), udp.remotePort());
      udp.write("pong");
      udp.endPacket();
    }
  }
}

void processCommand(const char* command) {
  // Implement your motor control logic here
  // You'll need to map command strings to motor actions
  // For example, you can use "UP" to increase motor speed and "DOWN" to decrease it.
  
  // Sample code to control motor speed (adjust as needed)
  if (strcmp(command, "UP") == 0) {
    analogWrite(motor1Pin, 255);
    analogWrite(motor2Pin, 255);
    analogWrite(motor3Pin, 255);
    analogWrite(motor4Pin, 255);
  } else if (strcmp(command, "DOWN") == 0) {
    analogWrite(motor1Pin, 0);
    analogWrite(motor2Pin, 0);
    analogWrite(motor3Pin, 0);
    analogWrite(motor4Pin, 0);
  } else if (strcmp(command, "RIGHT") == 0) {
    analogWrite(motor1Pin, 0);
    analogWrite(motor2Pin, 0);
    analogWrite(motor3Pin, 255);
    analogWrite(motor4Pin, 255);
  } else if (strcmp(command, "LEFT") == 0) {
    analogWrite(motor1Pin, 255);
    analogWrite(motor2Pin, 255);
    analogWrite(motor3Pin, 0);
    analogWrite(motor4Pin, 0);
  } else if (strcmp(command, "FORWARD") == 0) {
    analogWrite(motor1Pin, 0);
    analogWrite(motor2Pin, 255);
    analogWrite(motor3Pin, 255);
    analogWrite(motor4Pin, 0);
  } else if (strcmp(command, "BACKWARD") == 0) {
    analogWrite(motor1Pin, 255);
    analogWrite(motor2Pin, 0);
    analogWrite(motor3Pin, 0);
    analogWrite(motor4Pin, 255);
  }
}

