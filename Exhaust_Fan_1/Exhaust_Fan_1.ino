             
// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>






// Replace with your network credentials
const char* ssid     = "ZEJAK";
const char* password = "Zafeer1996";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

String manual_state = "OFF";
String fan_state = "OFF";
String fan_sensor = "MANUAL";

const int manual_switch = 4;
const int pir_switch = 5;
const int pir = 2;
const int relay = 16;
String mdnsname = "zejak-exh-01";





void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(relay, OUTPUT);
  
  pinMode(pir_switch, OUTPUT);
  pinMode(manual_switch, OUTPUT);
  pinMode(pir, INPUT);         

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


   if (MDNS.begin(mdnsname,WiFi.localIP())) {
    Serial.println("MDNS Successful");}
    
  server.begin();

}

void loop() {

  MDNS.update();

  // put your main code here, to run repeatedly:
  WiFiClient client = server.available();   // Listen for incoming clients

   if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();


             if (header.indexOf("GET /4/Off") >= 0)
             {
              Serial.println("Manual Off");
              manual_state = "OFF";
              digitalWrite(manual_switch,LOW);
             }

              if (header.indexOf("GET /4/On") >= 0)
             {
              Serial.println("Manual On");
              manual_state = "ON";
              digitalWrite(manual_switch,HIGH);
             }


             if (header.indexOf("GET /5/Off") >= 0)
             {
              Serial.println("Sensor Off");
              fan_sensor = "MANUAL";
              digitalWrite(pir_switch,LOW);
             }

             if (header.indexOf("GET /5/On") >= 0)
             {
              Serial.println("Sensor On");
              fan_sensor = "AUTOMATIC";
              digitalWrite(pir_switch,HIGH);
             }

            
            
             if((digitalRead(pir_switch)==HIGH && digitalRead(pir)==HIGH) || digitalRead(manual_switch)==HIGH)
                {
                  digitalWrite(relay,HIGH);
                  fan_state = "ON";
                }
             else 
                {
                  digitalWrite(relay,LOW);
                  fan_state = "OFF";
                }
             

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");


            

            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
          
            client.println(".button { border: none; color: black; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button1 {background-color: #F1FF14;}");
            client.println(".button2 {background-color: #00B90E;}");
            client.println(".button3 {background-color: #005FCB;}");
            client.println(".button4 {background-color: #f44336;}</style></head>");

            
            
            // Web Page Heading
            client.println("<body><h1>ZEJAK Smart Exhaust</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 4 
            client.println("<p>FAN STATE : " + fan_state + "</p>");
            client.println("<p>FAN SWITCH : " + manual_state + "</p>");          
            client.println("<p>FAN MODE : " + fan_sensor + "</p>");



            client.println("<p>MANUAL SWITCH : ");
            client.println("<a href=\"/4/On\"><button class=\"button button2\">ON</button></a>");
            client.println("<a href=\"/4/Off\"><button class=\"button button4\">OFF</button></a></p>");
                    

              client.println("<p>MODE : ");
            // If the fanOsc is off, it displays the ON button       
              client.println("<a href=\"/5/Off\"><button class=\"button button3\">MANUAL</button></a>");
              client.println("<a href=\"/5/On\"><button class=\"button button1\">AUTOMATIC</button></a></p>");





            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
   }



            

}
