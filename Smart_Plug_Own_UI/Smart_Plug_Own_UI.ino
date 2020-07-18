/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
#include <ESP8266WiFi.h>

// Replace with your network credentials
const char* ssid     = "ZEJAK";
const char* password = "Zafeer1996";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output5State = "Manual";
String output4State = "Off";

// Assign output variables to GPIO pins
const int output5 = 5;
const int output4 = 2;
int wifi_led = 4;
int PIR = 13;
int relay = 14;
//const int output5 = 2;
//const int output4 = 0;

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(output5, OUTPUT);
  pinMode(output4, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output5, LOW);
  digitalWrite(output4, LOW);

    pinMode(PIR, INPUT);
    pinMode(wifi_led, OUTPUT);
    pinMode(relay,OUTPUT);
    digitalWrite(wifi_led,LOW);
    digitalWrite(relay,LOW);

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
  server.begin();
}

void loop(){
  if (WiFi.status() == WL_CONNECTED) 
        digitalWrite(wifi_led,HIGH);
      else digitalWrite(wifi_led,LOW);

      
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
            
//            // turns the GPIOs on and off
//            if (header.indexOf("GET /5/on") >= 0) {
//              Serial.println("GPIO 5 on");
//              output5State = "Automatic";
//              digitalWrite(output5, HIGH);
//            } else if (header.indexOf("GET /5/off") >= 0) {
//              Serial.println("GPIO 5 off");
//              output5State = "Manual";
//              digitalWrite(output5, LOW);
//            } else if (header.indexOf("GET /4/on") >= 0) {
//              Serial.println("GPIO 4 on");
//              output4State = "On";
//              digitalWrite(output4, HIGH);
//            } else if (header.indexOf("GET /4/off") >= 0) {
//              Serial.println("GPIO 4 off");
//              output4State = "Off";
//              digitalWrite(output4, LOW);
//            }

            // turns the GPIOs on and off
            if (header.indexOf("GET /5/Automatic") >= 0) {
              Serial.println("GPIO 5 on");
              output5State = "Automatic";
              digitalWrite(output5, HIGH);
            } else if (header.indexOf("GET /5/Manual") >= 0) {
              Serial.println("GPIO 5 off");
              output5State = "Manual";
              digitalWrite(output5, LOW);
            } else if (header.indexOf("GET /4/On") >= 0) {
              Serial.println("GPIO 4 on");
              output4State = "On";
              digitalWrite(output4, HIGH);
            } else if (header.indexOf("GET /4/Off") >= 0) {
              Serial.println("GPIO 4 off");
              output4State = "Off";
              digitalWrite(output4, LOW);
            }

            


            // put your main code here, to run repeatedly:
      

 


      if (digitalRead(PIR) == HIGH && digitalRead(output5)==HIGH)
      {
          digitalWrite(relay,HIGH);
          Serial.println("Case 1 : PIR HIGH, Automatic");
      }
      if(digitalRead(PIR)==LOW && digitalRead(output5)==HIGH)
      {
          digitalWrite(relay,LOW);
          Serial.println("Case 2 : PIR LOW, Automatic");
      }
      if(digitalRead(output4)==HIGH && digitalRead(output5)==LOW)
      {
          digitalWrite(relay,HIGH);
          Serial.println("Case 3 : On/Off HIGH, Manual");
      }
      if(digitalRead(output4)==LOW && digitalRead(output5)==LOW)
      {
          digitalWrite(relay,LOW);          
          Serial.println("Case 4 : On/Off LOW, Manual");
      }


      
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            //client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println(".button { background-color: #195B6A; border: none; color: black; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ZEJAK Smart Plug</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 5  
            client.println("<p>Mode : " + output5State + "</p>");
            // If the output5State is off, it displays the ON button       
            if (output5State=="Manual") {
              //client.println("<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>");
              client.println("<p><a href=\"/5/Automatic\"><button class=\"button\">AUTO</button></a></p>");
            } else {
              //client.println("<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>");
              client.println("<p><a href=\"/5/Manual\"><button class=\"button button2\">MANUAL</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for GPIO 4  
            client.println("<p>Manual : " + output4State + "</p>");
            // If the output4State is off, it displays the ON button       
            if (output4State=="Off") {
              client.println("<p><a href=\"/4/On\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/Off\"><button class=\"button button2\">OFF</button></a></p>");
            }
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
