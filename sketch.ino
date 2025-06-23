#include <WiFi.h>
#include <PubSubClient.h>

// Replace these values with your Adafruit IO
#define IO_USERNAME ""
#define IO_KEY ""

const char* SSID = "Wokwi-GUEST";
const char* PASSWORD = "";

const char* MQTT_SERVER = "io.adafruit.com";
const int MQTT_PORT = 1883;

const char* topicFlow = "";
const char* topicWaterLevel = "";
const char* topicAlert = "";

const char* leakAlertMessage = "Alert. There was a leak detected";
const char* lowWaterLevelAlertMessage = "Alert. The water level is low";

unsigned long previousMillis = 0;
const long interval = 30000;

// Ultrasonic pins
#define trigPin 5
#define echoPin 18
#define lowThreshold 350 // Threshold for low water level
float previousDistance = 0;

// Flow pins (simulated with slide pots)
#define flowInPin 33
#define flowOutPin 34
#define flowThreshold 300  // Threshold for leak detection
int previousFlowDifference = 0;

WiFiClient wifi;
PubSubClient client(wifi);

void setup() 
{
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(flowInPin, INPUT);
  pinMode(flowOutPin, INPUT);

  WiFi.begin(SSID, PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  client.setServer(MQTT_SERVER, MQTT_PORT);

  Serial.print("Connecting to Adafruit IO...");
  while (!client.connected()) 
  {
    if (client.connect("ESP32Client", IO_USERNAME, IO_KEY)) 
    {
      Serial.println("connected");
    } 
    else 
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void loop() 
{ 
  if (!client.connected()) {
    client.connect("ESP32Client", IO_USERNAME, IO_KEY);
  }
  client.loop();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) 
  {
    previousMillis = currentMillis;
    // Read ultrasonic water level
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH, 30000);
    float distance = duration * 0.034 / 2;
    
    if (!DistanceOutOfRange(distance))
    {
      if (previousDistance != distance)
      {
        previousDistance = distance;
        Serial.print("Water Level (cm): ");
        Serial.println(distance);

        char waterStr[10];
        dtostrf(distance, 1, 2, waterStr); // dtostrf to convert float to string for publish
        client.publish(topicWaterLevel, waterStr);
      }
      
      // Check for low water level
      if (DistanceAboveThreshold(distance)) 
      {
        client.publish(topicAlert, lowWaterLevelAlertMessage);
      }
    }

    int flowIn = analogRead(flowInPin);
    int flowOut = analogRead(flowOutPin);
    int flowDiff = abs(flowIn - flowOut);

    if (!FlowOutOfRange(flowIn) && !FlowOutOfRange(flowOut))
    {
      if (previousFlowDifference != flowDiff)
      {
        previousFlowDifference = flowDiff;
        Serial.print("Flow In: "); Serial.print(flowIn);
        Serial.print(" | Flow Out: "); Serial.print(flowOut);
        Serial.print(" | Difference: "); Serial.println(flowDiff);

        char flowStr[10];
        itoa(flowDiff, flowStr, 10); // itoa to convert int to string for publish
        client.publish(topicFlow, flowStr); // Publish flow difference
      }

      // Check for leak
      if (FlowDifferenceAboveThreshold(flowDiff)) 
      {
        client.publish(topicAlert, leakAlertMessage);
      }
    }
  }
}

bool DistanceOutOfRange(float distance)
{
  if (distance < 0 || distance > 400)
  {
    Serial.println("Distance was out of range");
    return true;
  }
  return false;
}

bool FlowOutOfRange(int flow)
{
  if (flow < 0 || flow > 4095)
  {
    Serial.println("Flow was out of range");
    return true;
  }
  return false;
}

bool DistanceAboveThreshold(float distance)
{
  if (distance > lowThreshold)
  {
    return true;
  }
  return false;
}

bool FlowDifferenceAboveThreshold(int flowDiff)
{
  if (flowDiff > flowThreshold) 
  {
    return true;
  }
  return false;
}
