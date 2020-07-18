// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include <Servo.h>


Servo fan_speed;  // create servo object to control a servo
Servo fan_osc;


// Replace with your network credentials
const char* ssid     = "ZEJAK";
const char* password = "Zafeer1996";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

String fanSpeed = "Off";
String fanOsc = "Off";






void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  fan_speed.attach(4);
  fan_osc.attach(5);

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

void loop() {
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
              Serial.println("Fan Off");
              fanSpeed = "Off";
              fan_speed.write(0);
             }

             if (header.indexOf("GET /4/Slow") >= 0)
             {
              Serial.println("Fan Slow");
              fanSpeed = "Slow";
              fan_speed.write(45);
             }

             if (header.indexOf("GET /4/Medium") >= 0)
             {
              Serial.println("Fan Medium");
              fanSpeed = "Medium";
              fan_speed.write(90);
             }

             if (header.indexOf("GET /4/Fast") >= 0)
             {
              Serial.println("Fan Fast");
              fanSpeed = "Fast";
              fan_speed.write(135);
             }




             




            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");


            
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
          
            client.println(".button { background-color: #138603; border: none; color: black; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #DCD616;}");
            client.println(".button3 {background-color: #f44336;}");
            client.println(".button4 {background-color: #008CBA;}</style></head>");

            
            
            // Web Page Heading
            client.println("<body><h1>ZEJAK Smart Fan</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 4 
            client.println("<p>CURRENT MODE : " + fanSpeed + "</p>");
                
            if (fanSpeed=="Off") {
              client.println("<p><a href=\"/4/Slow\"><button class=\"button\">SLOW</button></a></p>");
            } else
            if (fanSpeed=="Slow") {
              client.println("<p><a href=\"/4/Medium\"><button class=\"button button2\">MEDIUM</button></a></p>");
            } else
             if (fanSpeed=="Medium") {
            client.println("<p><a href=\"/4/Fast\"><button class=\"button button3\">FAST</button></a></p>");
             }  else
            {
              client.println("<p><a href=\"/4/Off\"><button class=\"button button4\">OFF</button></a></p>");
            } 




               
//            // Display current state, and ON/OFF buttons for GPIO 4  
//            client.println("<p>Manual : " + output4State + "</p>");
//            // If the output4State is off, it displays the ON button       
//            if (output4State=="Off") {
//              client.println("<p><a href=\"/4/On\"><button class=\"button\">ON</button></a></p>");
//            } else {
//              client.println("<p><a href=\"/4/Off\"><button class=\"button button2\">OFF</button></a></p>");
//            }




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
