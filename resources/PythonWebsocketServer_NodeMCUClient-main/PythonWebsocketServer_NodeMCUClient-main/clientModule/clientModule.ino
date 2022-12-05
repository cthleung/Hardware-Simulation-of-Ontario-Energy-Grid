//#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#include <WebSocketsClient_Generic.h>
#include <SocketIOclient_Generic.h>

bool RECEIVED_DATE = false;

const char* ssid = "4US2 WiFi"; //Enter SSID
const char* password = ""; //Enter Password

// Select the server IP address and port according to your local network
IPAddress serverIP(192, 168, 1, 102);
uint16_t  serverPort = 5001;

// Initalize SocketIO client
SocketIOclient socketIO;

// Function for recieveing different socketIO events
void socketIOEvent(const socketIOmessageType_t& type, uint8_t * payload, const size_t& length)
{
  switch (type) 
  {
    case sIOtype_DISCONNECT:
      Serial.println("[IOc] Disconnected");
      break;
    case sIOtype_CONNECT:
      Serial.print("[IOc] Connected to url: ");
      Serial.println((char*) payload);

      // join default namespace (no auto join in Socket.IO V3)
      socketIO.send(sIOtype_CONNECT, "/");
      break;

    case sIOtype_EVENT:
      Serial.print("[IOc] Get event: ");
      Serial.println((char*) payload);
      handleNewEvent((char*) payload);

      // if (payload == 'date') {
      //   // split date by
      // }
      break;
    case sIOtype_PING:
      Serial.println("[IOc] Get PING");

      break;

   case sIOtype_PONG:
      Serial.println("[IOc] Get PONG");

      break;

    default:
      break;
  }
}

void handleNewEvent(char* jsonEvent) {
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, jsonEvent);

  if (error) {
    Serial.print("Deserialization Failed: ");
    Serial.println(error.f_str());
    return;
  }

  if (strcmp(doc[0], "date") == 0) {
    Serial.print("Received Event: date");
    const char* date = doc[1];
    Serial.println(date);
    RECEIVED_DATE = true;
    // Need to send to python
    // Than recieve back value for that hour
    // Possibly return all 24 data points and store in an array
    // Start looping through and writing data points to operators
    return;
  }

  if (strcmp(doc[0], "percent_max_output") == 0) {
    Serial.print("Recieved Event: percent_max_output");
    const int percentage = doc[1];
    Serial.println(percentage);
    return;
  }

}

void setup()
{
    Serial.begin(115200);
    Serial.println("Connecting...\n");
    
    WiFi.mode(WIFI_STA);
    // WiFi.begin(ssid, password); // change it to your current wifi ssid and password
   WiFi.begin(ssid); // Use when connecting to the 4US2 Wifi
    
    // Connect to Wifi
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.print("WebSockets Client running @ IP address: ");
    Serial.println(WiFi.localIP());

    // Set connection details and connect to server
    socketIO.setReconnectInterval(10000);
    socketIO.begin(serverIP, serverPort);
    socketIO.onEvent(socketIOEvent); 
    Serial.print("WebSockets Client Connected to server");
}


unsigned long messageTimestamp = 0;
void loop() {

  socketIO.loop();


  if (RECEIVED_DATE) {
    
    // Package and send to server every 12.5 seconds
    // When you recieve data back send over PWM to effect circuit

    //   // creat JSON message for Socket.IO (event)
    //   DynamicJsonDocument doc(1024);
    //   JsonArray array = doc.to<JsonArray>();

    //   // add evnet name
    //   // Hint: socket.on('event_name', ....
    //   array.add("output_value");
    //   array.add("80");


    //   String output;
    //   serializeJson(doc, output);

    //   // Send event
    //   socketIO.sendEVENT(output);

    //   // Print JSON for debugging
    //   Serial.println(output);
    return;
  }

  // For value in 24 data values 

  // uint64_t now = millis();

  // if (now - messageTimestamp > 12000) 
  // {
  //   messageTimestamp = now;

  //   // creat JSON message for Socket.IO (event)
  //   DynamicJsonDocument doc(1024);
  //   JsonArray array = doc.to<JsonArray>();

  //   // add evnet name
  //   // Hint: socket.on('event_name', ....
  //   array.add("output_value");
  //   array.add("80");


  //   String output;
  //   serializeJson(doc, output);

  //   // Send event
  //   socketIO.sendEVENT(output);

  //   // Print JSON for debugging
  //   Serial.println(output);
  // }
}
