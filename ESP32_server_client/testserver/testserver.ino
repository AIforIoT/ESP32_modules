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
                }
                //This ESP32 server only expects HTTP Request:
                // GET /H HTTP/1.x
                // GET /L HTTP/1.x
                if (c != '\r') {  // if you got anything else but a carriage return character,
                    currentLine += c;      // add it to the end of the currentLine
                    // Check to see if the client request was "GET /H" or "GET /L":
                    if (currentLine.endsWith("GET /H")) {
                        digitalWrite(5, HIGH);               // GET /H turns the REALY on
                        client.println("HTTP/1.1 200 OK");
                        client.println();
                        Serial.println("H request detected");

                    }else if (currentLine.endsWith("GET /L")) {
                        digitalWrite(5, LOW);                // GET /L turns the RELAY off
                        client.println("HTTP/1.1 200 OK");
                        client.println();
                        Serial.println("L request detected");

                    }else if (currentLine.endsWith("GET /data/on")) {
                        // DATA REQUEST FROM RASPI
                        client.println("HTTP/1.1 200 OK");
                        client.println();
                        Serial.println("DATA request detected");
                        //DATA CODE GOES HERE...


                    }else if (currentLine.endsWith("GET /data/off")) {
                        // DATA REQUEST FROM RASPI
                        client.println("HTTP/1.1 200 OK");
                        client.println();
                        Serial.println("NODATA request detected");
                        //NODATA CODE GOES HERE...

                    }else if (currentLine.endsWith("GET /volume")) {
                        // DATA REQUEST FROM RASPI
                        client.println("HTTP/1.1 200 OK");
                        client.println();
                        Serial.println("VOLUME request detected");
                        //VOLUME CODE GOES HERE...

                    }
                }else{  //End of first request line , no accepted get requested
                    //Error 405 Method Not Allowed
                    client.println("HTTP/1.1 405 Method Not Allowed");
                    client.println();
                }
            }
            // close the connection:
            client.stop();
            Serial.println("Client Disconnected.");
        }
    }//END WHILE
}//END LOOP
