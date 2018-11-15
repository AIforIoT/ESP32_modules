/*Based on Simple example of multi loop
* By Evandro Luis Copercini
* Based on pcbreflux video
* Public domain license 2017
*Based on Simple Wifi Server
*created for arduino 25 Nov 2012
*by Tom Igoe
*ported for sparkfun esp32
*31.01.2017 by Jan Hendrik Berlin
*/


#include <WiFi.h>

//WIFI DEFINITIONS
int status = WL_IDLE_STATUS;
const char* ssid     = "Aquaris X5 Plus";
const char* password = "3cdb401cb5d6";
const char* ip = "192.168.43.81"

char google[] = "www.google.com";
unsigned long lastConnectionTime = 0;               // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 5L * 1000L;   // delay between updates, in milliseconds

// Initialize the Wifi client library
WiFiClient client;
// Initialize the Wifi server library
WiFiServer server(80);

//THREADS DEFINITIONS
TaskHandle_t Task1;
TaskHandle_t Task2;

void codeForTask1( void * parameter){

    Serial.begin(115200);
    while(true){
        Serial.println("Print from core 0, TASK1");
        delay(2000);
    }
}

void serverTask2( void * parameter){

    //Setup
    Serial.begin(115200);
    pinMode(5, OUTPUT);      // set the LED pin mode
    server.begin();
    Serial.print("server is at ");
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
                            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                            // and a content-type so the client knows what's coming, then a blank line:
                            client.println("HTTP/1.1 200 OK");
                            client.println("Content-type:text/html");
                            client.println();
                            // the content of the HTTP response follows the header:
                            client.print("Click <a href=\"/H\">here</a> to turn the LED on pin 5 on.<br>");
                            client.print("Click <a href=\"/L\">here</a> to turn the LED on pin 5 off.<br>");
                            // The HTTP response ends with another blank line:
                            client.println();
                            // break out of the while loop:
                            break;
                        } else {    // if you got a newline, then clear currentLine:
                            currentLine = "";
                        }
                    } else if (c != '\r') {  // if you got anything else but a carriage return character,
                        currentLine += c;      // add it to the end of the currentLine
                    }
                    // Check to see if the client request was "GET /H" or "GET /L":
                    if (currentLine.endsWith("GET /H")) {
                        digitalWrite(5, HIGH);               // GET /H turns the LED on
                    }
                    if (currentLine.endsWith("GET /L")) {
                        digitalWrite(5, LOW);                // GET /L turns the LED off
                    }
                }
            }
            // close the connection:
            client.stop();
            Serial.println("Client Disconnected.");
        }
    }
}

///SETUP

void setup(){
    Serial.begin(115200);

    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid);
        status = WiFi.begin(ssid, password);
        delay(2000);
    }
    Serial.println("Connected");

    xTaskCreatePinnedToCore(
        codeForTask1,             //Task function
        "Task_1",                 //name of task
        1000,                     //Stack size of the task
        NULL,                     //parameter of the task
        1,                        //priority of the task
        &Task1,                   //Task handle to keep track of created task
        0);                       //core

    xTaskCreatePinnedToCore(
        serverTask2,             //Task function
        "Task_2",                 //name of task
        5000,                     //Stack size of the task
        NULL,                     //parameter of the task
        1,                        //priority of the task
        &Task2,                   //Task handle to keep track of created task
        1);                       //core
}

void loop(){

    // send it out the serial port.This is for debugging purposes only:
    while (client.available()) {
        char c = client.read();
        Serial.write(c);
    }
    //if ten seconds have passed since your last connection,
    // then connect again and send data:
    if (millis() - lastConnectionTime > postingInterval) {
        httpRequest();
    }
}//END


// this method makes a HTTP connection to the server:
void httpRequest() {
    // close any connection before send a new request.
    // This will free the socket on the WiFi shield
    client.stop();

    //if there's a successful connection:
    if (client.connect(google, 80)) {
        Serial.println("connecting...");
        // send the HTTP PUT request:
        client.println("GET /index.html HTTP/1.0");
        client.println("content-type: tex/plain");
        client.println("");
        client.println("");
        //client.println("DATA VALUE GOES HERE");
        //note the time that the connection was made:
        lastConnectionTime = millis();
    }else{
        // if you couldn't make a connection:
        Serial.println("connection failed");
    }
}
