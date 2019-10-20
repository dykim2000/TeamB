#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define WIFI_SSID "Nextop1"
#define WIFI_PASSWORD "20183365"
#define FIREBASE_HOST ""   //Github올릴때 지우기
#define FIREBASE_AUTH "" //Github올릴때 지우기


int redLed = 13;
int greenLed = 12;
int buzzer = 14;
int smokeA0 = A0;

int FB_LED; //0 = Green 1 = Red
int FB_Gas;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  // Connect to WiFi network
  wifiConnect();
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(smokeA0, INPUT);

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
  if(Firebase.getInt("LED") == NULL) { //Key값이 없을때만 작동
    Firebase.setInt("LED", 0);
    Firebase.setInt("Gas", 0);
    return;
  }else {
    FB_LED = Firebase.getInt("LED");
    FB_Gas = Firebase.getInt("Gas");
  }
  
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();

  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  int analogSensor = analogRead(smokeA0);
  int LED = digitalRead(redLed);
  Serial.print("Pin A0: ");
  Serial.println(analogSensor);

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  //client.println( "Refresh: 20");        // refresh the page automatically every 20 sec
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  client.println("<p>Gas value is:<b> ");
  client.println(FB_Gas);
  client.println("</b></p>");
  if(FB_LED == 1) client.println("<p style=\"color:red\"> LED is now <b>Red</b></p>");
  else client.println("<p style=\"color:green\"> LED is now <b>Green</b></p>");

  client.println("<a href=\"/C\"><button>Check Gas Value</button></a>");
  client.println("</html>");

  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");

  if(request.indexOf("GET /C") >=0 ) 
  {
    if(analogSensor > 800) 
    {
      digitalWrite(redLed, HIGH);
      digitalWrite(greenLed, LOW);
      FB_LED = 1;
      FB_Gas = analogSensor;
      Firebase.setInt("LED", FB_LED);
      Firebase.setInt("Gas", FB_Gas);
      tone(buzzer, 1000, 20000);
  
    }else 
    {
      digitalWrite(redLed, LOW);
      digitalWrite(greenLed, HIGH);
      FB_LED = 0;
      FB_Gas = analogSensor;
      Firebase.setInt("LED", FB_LED);
      Firebase.setInt("Gas", FB_Gas);
      noTone(buzzer);
    }
  }
}

void wifiConnect()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID); Serial.println(" ...");

  while (WiFi.status() != WL_CONNECTED)
  {                                       // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print('.'); Serial.print(' ');
  }
    // Start the server
  server.begin();
  Serial.println("Server started");

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
}
