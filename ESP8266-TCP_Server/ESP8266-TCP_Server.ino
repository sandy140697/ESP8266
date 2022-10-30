#include <ESP8266WiFi.h>
#include <string.h>

/*
  -Send data from client ending with <CR><LF>
  -Send ledon from TCP client to make LED ON
  -Send ledoff from TCP client to make LED OFF
  -If correct request send from client, server will respond with OK message, otherwise reponse will be invalid request.
  -Code can be tested with any readily available app for TCP/IP.    
*/

#define LED_D4 D4

WiFiClient client;
WiFiServer server(8080);         // creating server object with port number 8080
String SSID = "sandy";           // router SSID
String password = "1234098765";  // router password

char c;  // character to store incoming data from client
String request;
bool datareceived = false;
bool CRLF_Flag = false;

// IPAddress  ip(192,168,0,72);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);          // Initializing UART with baud rate 9600
  WiFi.mode(WIFI_STA);         // Setting Wifi module in station mode
  WiFi.begin(SSID, password);  // Initialize wifi
  pinMode(LED_D4, OUTPUT);     // Making D4 as output


  // Uncomment below commented code to configure your esp for static IP
  /*
  IPAddress ip(192, 168, 43, 120);
  IPAddress gateway(192, 168, 43, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(ip, gateway, subnet);  // configure wi-fi for static IP address, gateway and subnet
*/
  while (WiFi.status() != WL_CONNECTED) {  // Checking if wifi module is connected to router
    Serial.print("..");
    delay(200);
  }

  Serial.println();
  Serial.print("WiFi is connected to   SSID: ");
  Serial.print(SSID);
  Serial.print("       Password: ");
  Serial.println(password);
  Serial.println(WiFi.localIP());  // peinting local IP assigned to ESP32
  server.begin();                  // Initializing server
  Serial.println("Server Started...");
}

void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient client = server.available();  // checking if any client is available on server

  if (!client) {
    return;
  }

  Serial.println("New client!!");
  while (client.connected()) {  // loop untill client is connected to the server
    if (client.available()) {   // checking if any data requested from client
      c = client.read();        // reading the data sent by client
      // Serial.print(c);

      if (c == '\r') {  // checking the <cr><lf> at end of data received
        CRLF_Flag = true;
      } else if ((c == '\n') && (CRLF_Flag == true)) {
        datareceived = true;
        CRLF_Flag == false;
      } else {
        request += c;  // <CR><LF> characters will not be added in the string
      }
    }

    if (datareceived) {
      if (request == "ledon") {
        Serial.println("LED ON");
        digitalWrite(LED_D4, 0);
        client.print("OK");  // response to client with "OK" message
        request = "";
        datareceived = 0;
      } else if (request == "ledoff") {
        Serial.println("LED OFF");
        digitalWrite(LED_D4, 1);
        client.print("OK");  // response to client with "OK" message
        request = "";
        datareceived = 0;
      } else {
        request = "";
        datareceived = 0;
        client.print("INVALID REQUEST");  // response to client with "INVALID REQUEST" message, if correct command not received from client
      }
    }
  }

  c = '\0';       // Nullify the character
  client.stop();  // Close client connection
  Serial.println();
  Serial.println("Client Disconnected");
}