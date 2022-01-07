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

String armPassword = "Hwuqi1o331agywua";

int intervalUdpSend =  5000; 
unsigned long previousMillis = 0; 

//udp server
WiFiUDP Udp;


class Packet
{
  public: 
    char* bufferChar;
    int readPos = 0; 
    Packet(char* _packet)
    {
      bufferChar = _packet;
    }

    Packet()
    {
       
    }

    void SetBuffer(char* _packet, bool resetPos = true)
    {
      bufferChar = _packet;  
      if(resetPos)
      {
        readPos = 0; 
      }
      Serial.println("packetSet"); 
    }

    int ReadInt()
    {
      int i = (bufferChar[readPos+3] << 24) | (bufferChar[readPos+2] << 16) | (bufferChar[readPos+1] << 8) | (bufferChar[readPos]);
      readPos += 4; 
      return i;  
    }

    float ReadFloat() 
    {
      long x = (long)bufferChar[readPos+3]<<24|(long)bufferChar[readPos+2]<<16|bufferChar[readPos+1]<<8|bufferChar[readPos];
      readPos += 4; 
      union
      {
        long y;
        float z;
      }data; 
      data.y = x; 
      return data.z; 
    }
};

Packet PacketReader;
void PacketWrite(String s)
{
      
  PacketWrite(s.length());
  Udp.write(s.c_str());
}

void PacketWrite(int n)
{
      
  char bytes[sizeof n];
  std::copy(static_cast<const char*>(static_cast<const void*>(&n)),
          static_cast<const char*>(static_cast<const void*>(&n)) + sizeof n,
          bytes);
  Udp.write(bytes[0]); //255
  Udp.write(bytes[1]); //255
  Udp.write(bytes[2]); //255
  Udp.write(bytes[3]);
}
String nodePass = "Nwifugu31393g2HSDUg18173d_fb3yja";

char incomingPacket[255];

unsigned int localport = 4000;
int targetPort; 

String serverIp; 

char reply[] = "sup";

bool registeredToFleet = false; 

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
    obj[String("armPass")] = armPassword;
    
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
  
  
  int packetSize = Udp.parsePacket(); 
  if(packetSize)
  {
    char bufferPacket[255]; 
    int len = Udp.read(bufferPacket, 255);
    
    PacketReader.SetBuffer(bufferPacket); 
    int testInt = PacketReader.ReadInt(); 
    
    float testFloat = PacketReader.ReadFloat(); 
    Serial.println("Recieved Test Int:");
    Serial.println(testInt);  
    Serial.println("Recieved Test Float:");
    Serial.println(testFloat);  
    
  }
  unsigned long currentMillis = millis(); 
  if((unsigned long)(currentMillis - previousMillis) >= intervalUdpSend)
  {
    
    PacketSend();
    previousMillis = currentMillis;
  } 
  
}

void PacketSend()
{
  
  int socketSuccess = Udp.beginPacket(serverIp.c_str(), targetPort);
  PacketWrite(-1);
  PacketWrite(nodePass); 
  if(registeredToFleet)
  {
    PacketWrite(1);
    PacketWrite(id);
    PacketWrite("test");
  }
  else
  {
    PacketWrite(0);
    PacketWrite(id);
    PacketWrite(armPassword);
  }
  int sendSuccess = Udp.endPacket();

  //listen for a response or smth lol 
  Serial.print("connect Success: ");
  Serial.println(socketSuccess);
  Serial.print("Send sucess: ");
  Serial.println(socketSuccess);
  
}
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
