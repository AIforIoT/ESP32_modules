#include <WiFi.h>

//WIFI DEFINITIONS
int status = WL_IDLE_STATUS;
const char* ssid     = "Aquaris X5 Plus";
const char* password = "3cdb401cb5d6";




// Initialize the Wifi server library
WiFiServer server(80);

void setup(){
    Serial.begin(115200);

    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid);
        status = WiFi.begin(ssid, password);
        delay(2000);
    }
    Serial.println("Connected");
}

void loop(){

    // send it out the serial port.This is for debugging purposes only:
    //Setup
    Serial.begin(115200);
    pinMode(5, OUTPUT);      // set the RELAY pin mode
    server.begin();
    Serial.println("Server started");
    Serial.print("Server is at ");
    Serial.println(WiFi.localIP());
    //Loop
    while(true){
        // listen for incoming clients
        WiFiClient client = server.available();
        if (client) {
            Serial.println("new client");
            String currentLine = "";                // make a String to hold incoming data from the client
            while (client.connected()) {            // loop while the client's connected
                if (client.available()) {             // if there's bytes to read from the client,
                char c = client.read();             // read a byte, then
                Serial.write(c);                    // print it out the serial monitor
                    if (c == '\n') {                    // if the byte is a newline character

                        // if the current line is blank, you got two newline characters in a row.
                        // that's the end of the client HTTP request, so send a response:
                        if (currentLine.length() == 0) {
                            //Nothing
                        } else {    // if you got a newline, then clear currentLine:
                            currentLine = "";
                        }
                    } else if (c != '\r') {  // if you got anything else but a carriage return character,
                        currentLine += c;      // add it to the end of the currentLine
                    }
                    // Check to see if the client request was "GET /H" or "GET /L":
                    if (currentLine.endsWith("GET /H")) {
                        digitalWrite(5, HIGH);               // GET /H turns the REALY on
                    }
                    if (currentLine.endsWith("GET /L")) {
                        digitalWrite(5, LOW);                // GET /L turns the RELAY off
                    }
                }
            }
            // close the connection:
            client.stop();
            Serial.println("Client Disconnected.");
        }
    }
}//END
