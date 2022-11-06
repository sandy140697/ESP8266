#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <string.h>

WiFiClient client;                      
#define SSID "sandy"
#define PASSWORD "1234098765"

String response;
String server_url = "www.httpbin.org";
uint16_t server_port = 80;
int rand_num1;
int rand_num2;

String buff;

void HTTP_Post_Req(void) {
  buff = buff + "{\r\n\" key1 \": \" " + rand_num1 + " \",\r\n" + "\" key2 \": \" " + rand_num2 + " \"\r\n}";

  while (!client.connect(server_url, server_port))
    ;

  Serial.println("Client Connected....");
  delay(100);

  client.println("POST /post HTTP/1.1");
  client.println("Host: httpbin.org");
  client.println("Content-Type: application/json");
  // client.println("Content-Type: text/plain");
  client.println("Connection: close");
  // client.println("Connection: keep-alive");
  client.println("Content-Length: 41");
  // client.println("Content-Length: 10");
  client.println("");
  // client.println("helloooooo");
  // client.println("{");
  // client.println("\" key1 \": \" value1 \",");
  // client.println("\" key2 \": \" value2 \"");
  // client.println("}");
  client.println(buff);

  while (client.connected()) {
    response = client.readStringUntil('\n');
    Serial.println(response);
    if (response == "\r") {
      break;
    }
  }

  while (client.available()) {
    response = client.readStringUntil('\n');
    Serial.println(response);
  }

  if (!client.connected()) {
    Serial.println("Client Disconnected");
    buff = "";
  }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("Connecting");
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }
  Serial.println();
  Serial.println("Connected to Wifi");
  Serial.println("IP adddress: ");
  Serial.println(WiFi.localIP());

  randomSeed(analogRead(0));
}

void loop() {
  // put your main code here, to run repeatedly:
  rand_num1 = random(50);
  rand_num2 = random(100);
  Serial.println(rand_num1);
  Serial.println(rand_num2);

  HTTP_Post_Req();
  delay(5000);
}