//NodeMCU script
#include<SoftwareSerial.h>
#include<ESP8266WiFi.h>
#include<WiFiClient.h>
#include<ESP8266HTTPClient.h>
#include<WiFiUdp.h>
#include<vector>
#include<string.h>
#include<ArduinoJson.h>

//test id
int id = 32142102;

const char* ssid = "NETGEAR78";
const char* password = "coolmoon622";
//api/arm/register
//http server
String server = "http://192.168.1.4:3000/api/arm/register";


//udp server
WiFiUDP Udp;

class Packet
{
  public:
    std::vector<char> writeableBuffer; 
    char* bufferChar;
    int readPos = 0; 
    Packet(char* _packet)
    {
      bufferChar = _packet;
      std::vector<char> data(_packet,_packet + (unsigned)strlen(_packet));
      writeableBuffer.insert(writeableBuffer.end(), data.begin(), data.end());
      
    }
};

String nodePass = "Nwifugu31393g2HSDUg18173d_fb3yja";

char incomingPacket[255];

unsigned int localport = 4000;
int targetPort; 

String serverIp; 

char reply[] = "sup";
 

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  //WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("connecting to internet");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  int i = Udp.begin(localport);
  Serial.println(i);
  delay(2000);
  while(true)
  {
    
    WiFiClient client; 
    HTTPClient http;
    http.begin(client, server.c_str()); 

    http.addHeader("Content-Type", "application/json");
    DynamicJsonDocument doc(1024);

  // You can use a String as your JSON input.
  // WARNING: the string in the input  will be duplicated in the JsonDocument.
    String input =
        "{}";
    deserializeJson(doc, input);
    JsonObject obj = doc.as<JsonObject>();
    obj[String("pass")] = nodePass;
    obj[String("id")] = id; 
    obj[String("port")] = localport; 
    obj[String("ip")] = WiFi.localIP();
    
    String output;
    
    Serial.println("sending packet");
    //output 
    serializeJson(doc, output) ; 
    Serial.println(output);
    int httpResponseCode = http.POST(output);
    String payload = http.getString();
    Serial.println(payload);
    Serial.println(httpResponseCode);
    deserializeJson(doc,payload);
    obj = doc.as<JsonObject>();
    if((!obj["error"] || obj["existing"]) && httpResponseCode > -1)
    {
      serverIp = obj["message"].as<String>();
      targetPort = obj["port"];
      Serial.println("Connecting to udp");
      
      Serial.println(payload);
      Serial.println(serverIp);
      Serial.println(targetPort);
      break; 
    }
    
    delay(5000);
  }
  Serial.println("starting udp process");
  delay(1000);
}

void loop() {
  int socketSuccess = Udp.beginPacket(serverIp.c_str(), targetPort);
  Udp.write("hello");
  int sendSuccess = Udp.endPacket();
  Serial.print("connect Success: ");
  Serial.println(socketSuccess);
  Serial.print("Send sucess: ");
  Serial.println(socketSuccess);
  delay(5000);


  //Serial1.println()
}
//bool udpSendMessage(IPAddress ipAddr, String udpMsg, int udpPort) {
//  /** WiFiUDP class for creating UDP communication */
//  
//  WiFiUDP udpClientServer;
//
//  // Start UDP client for sending packets
//  int connOK = udpClientServer.begin(udpPort);
//  
//  if (connOK == 0) {
//    Serial.println("UDP could not get socket");
//    return false;
//  }
//  int beginOK = udpClientServer.beginPacket(ipAddr, udpPort);
//
//  if (beginOK == 0) { // Problem occured!
//    udpClientServer.stop();
//    Serial.println("UDP connection failed");
//    return false;
//  }
//  int bytesSent = udpClientServer.print(udpMsg);
//  if (bytesSent == udpMsg.length()) {
//    Serial.println("Sent " + String(bytesSent) + " bytes from " + udpMsg + " which had a length of " + String(udpMsg.length()) + " bytes");
//    udpClientServer.endPacket();
//    udpClientServer.stop();
//    return true;
//  } else {
//    
//    Serial.println("Failed to send " + udpMsg + ", sent " + String(bytesSent) + " of " + String(udpMsg.length()) + " bytes");
//    udpClientServer.endPacket();
//    udpClientServer.stop();
//    return false;
//  }
//}


//  int packetSize = Udp.parsePacket();
//  if(packetSize)
//  {
//    Serial.print("Received packet of size ");
//    Serial.println(packetSize);
//    Serial.print("From IP : ");
//
//    IPAddress remote = Udp.remoteIP();
//    //print out the remote connection's IP address
//    Serial.print(remote);
//
//    Serial.print(" on port : ");
//    //print out the remote connection's port
//    Serial.println(Udp.remotePort());
//  }

//HTTP talking

//  if(WiFi.status() == WL_CONNECTED)
//  {
//    WiFiClient client;
//    HTTPClient http;
//
//     http.begin(client, server.c_str());
//     int httpResponseCode = http.GET();
//     if(httpResponseCode > 0)
//     {
//      Serial.print("HTTP Reponse code: " );
//      Serial.println(httpResponseCode);
//      String payload = http.getString();
//      Serial.println(payload);
//     }
//     else
//     {
//      Serial.print("Error code: ");
//      Serial.print(httpResponseCode);
//
//     }
//     http.end();
//    }
//    else
//    {
//      Serial.println("no wifi");
//    }
//
//
//  delay(10000);
