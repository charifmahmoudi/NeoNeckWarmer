#include <WiFi101.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiSSLClient.h>
#include <WiFiUdp.h>

#include <Adafruit_NeoPixel.h>

void colorAll(uint32_t c, uint8_t wait);
void colorWipe(uint32_t c, uint8_t wait);
void rainbow(uint8_t wait);
void rainbowCycle(uint8_t wait);
uint32_t Wheel(byte WheelPos);


// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_COUNT 30
#define PIXEL_PIN 6
#define PIXEL_TYPE NEO_GRBW + NEO_KHZ400

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

// WIFI configutarion

char ssid[] = "charif";      //  your network SSID (name)
char pass[] = "mahmoudi";   // your network password
int status = WL_IDLE_STATUS;
WiFiServer server(80);

bool bow = false;


void setup() {
  Serial.begin(9600);      // initialize serial communication
  Serial.print("Start Serial ");
  strip.begin();
  //colorWipe(strip.Color(0, 255, 0), 50); // green
  //delay(3000);

  if (WiFi.status() == WL_NO_SHIELD) {
        // Shows red color if connection issue 
        colorWipe(strip.Color(255, 0, 0), 50); // Red
        delay(3000);
        return; // don't continue
  }
      // attempt to connect to Wifi network:
    while ( status != WL_CONNECTED) {
        
        colorWipe(strip.Color(255, 0, 0), 50); // Red

        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(ssid, pass);
        if(status != WL_CONNECTED){
          // wait 10 seconds for connection:
        colorWipe(strip.Color(0, 255, 0), 50);
        delay(10000);
        }
        
    }
    server.begin();  
    printWifiStatus();
   colorWipe(strip.Color(0, 0, 0), 50);
  
}

void loop() {
  if(bow){
    rainbowCycle(1);
  }
    WiFiClient client = server.available();   // listen for incoming clients
   
    if (client) {                             // if you get a client,
      Serial.println("new client");
       //rainbow(50);
       //colorWipe(strip.Color(0, 0, 0), 50);
        String currentLine = "";                // make a String to hold incoming data from the client
        while (client.connected()) {            // loop while the client's connected
            if (client.available()) {             // if there's bytes to read from the client,
                char c = client.read();             // read a byte, then
                
                if (c == '\n') {                    // if the byte is a newline character
                                       
                    // if the current line is blank, you got two newline characters in a row.
                    // that's the end of the client HTTP request, so send a response:
                    if (currentLine.length() != 0) {
                        if(currentLine.indexOf("GET") != -1){
                          if (currentLine.indexOf("/neo") != -1) {
                            sendStatusNeo(currentLine,client);
                          }if (currentLine.indexOf("/rainbow") != -1) {
                            sendStatusRainbow(client);
                          }else {
                            sendPage(client);
                          }
                          break;
                        }
                        
                    }
                    else {      // if you got a newline, then clear currentLine:
                        currentLine = "";
                    }
                }
                else if (c != '\r') {    // if you got anything else but a carriage return character,
                    currentLine += c;      // add it to the end of the currentLine
                }
                
            }
            else{
              break;
            }
        }
        // close the connection:
        client.stop();
        Serial.println("client disonnected");

    }
}

void sendStatusRainbow( WiFiClient client){

   bow = true;
    // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:
  client.println("HTTP/1.1 201 OK");
  client.println("Content-type:application/json");
  client.println("Connection: close");
  client.println();
    // the content of the HTTP response follows the header:
  client.print("{\"NeoPixelsStatus\":\"updated\",\"mode\":\"rainbow\"}");
  client.println();
}

void sendStatusNeo(String currentLine, WiFiClient client){
  bow = false;
  int r = currentLine.indexOf("/");
      r = currentLine.indexOf("/", r + 1);
  int g = currentLine.indexOf("/", r + 1);
  int b = currentLine.indexOf("/", g + 1);
  int e = currentLine.indexOf(" ", b + 1);
  r = currentLine.substring(r + 1, g).toInt();
  g = currentLine.substring(g + 1, b).toInt();
  b = currentLine.substring(b + 1, e).toInt();
  
  colorWipe(strip.Color(r,g,b), 50);

    // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:
  client.println("HTTP/1.1 201 OK");
  client.println("Content-type:application/json");
  client.println("Connection: close");
  client.println();
    // the content of the HTTP response follows the header:
  client.print("{NeoPixelsStatus:updated,r:"+String(r)+",g:"+String(g)+",b:"+String(b)+"}");
  client.println();
}
void sendPage(WiFiClient client){

  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();
  
  // the content of the HTTP response follows the header:
 client.print("<!DOCTYPE html> \n");
client.print("<html> \n");
client.print("<head> \n");
client.print(" <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> \n");
client.print(" <link href=\"http://code.jquery.com/mobile/1.3.2/jquery.mobile-1.3.2.min.css\" rel=\"stylesheet\" type=\"text/css\" /> \n");
client.print(" <script src=\"http://code.jquery.com/jquery-1.9.1.min.js\"></script> \n");
client.print(" <script src=\"http://code.jquery.com/mobile/1.3.2/jquery.mobile-1.3.2.min.js\"></script> \n");
client.print(" <script src=\"http://jscolor.com/release/2.0/jscolor-2.0.4/jscolor.js\"></script> \n");
client.print(" <meta charset=utf-8 /> \n");
client.print(" <title>Neo Neck Warmer</title> \n");
client.print(" <script> \n");
client.print(" \n");
client.print(" $(document).bind('pageinit', function() { \n");
client.print(" $(\"#rainbow\").click(function (e) { \n");
client.print(" $.get(\"rainbow\", function(data, status){ \n");
client.print(" alert(\"Data: \" + data + \" Status: \" + status); \n");
client.print(" }); \n");
client.print(" $(\"#footerH3 span\").text( \"rainbow\" ); \n");
client.print(" }); \n");
client.print(" $(\"#neocolor\").on(\"change\", function() { \n");
client.print(" var hex = $.trim($(\"#neocolor\").val()); \n");
client.print(" var rgburl = \"neo/\"+parseInt(hex.substring(0, 2), 16)+\"/\"+parseInt(hex.substring(2, 4), 16)+\"/\"+parseInt(hex.substring(4, 6), 16); \n");
client.print(" $.get(rgburl, function(data, status){ \n");
client.print(" alert(\"Data: \" + data + \" Status: \" + status); \n");
client.print(" }); \n");
client.print(" $(\"#footerH3 span\").text( \"color \" + rgburl ); \n");
client.print(" }); \n");
client.print(" }); \n");
client.print(" </script> \n");
client.print("</head> \n");
client.print("<body> \n");
client.print("<div data-role=\"page\"> \n");
client.print(" <div data-role=\"header\"><h1>Neo Neck Warmer</h1></div> \n");
client.print(" <div data-role=\"content\"> \n");
client.print(" <p>Choose a color</p> \n");
client.print(" <input class=\"jscolor\" id=\"neocolor\" value=\"99CC00\" /> \n");
client.print(" <p>Rainbow mode </p> \n");
client.print(" <input type=\"button\" value=\"Rainbow\" id=\"rainbow\"> \n");
client.print(" </div> \n");
client.print(" <div data-role=\"footer\"> \n");
client.print(" <h3 id=\"footerH3\" >Mode <span></span></h3> \n");
client.print(" </div> \n");
client.print("</div> \n");
client.print("</body> \n");
client.print("</html> \n");

 
  
  
  
  // The HTTP response ends with another blank line:
  client.println();
  // break out of the while loop:
}


// Set all pixels in the strip to a solid color, then wait (ms)
void colorAll(uint32_t c, uint8_t wait) {
  uint16_t i;

  for(i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
  delay(wait);
}

// Fill the dots one after the other with a color, wait (ms) after each one
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout, then wait (ms)
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) { // 1 cycle of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void printWifiStatus() {
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
    
    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
    
    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
    // print where to go in a browser:
    Serial.print("To see this page in action, open a browser to http://");
    Serial.println(ip);
}

