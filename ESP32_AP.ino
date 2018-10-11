
#include <WiFi.h>

WiFiServer server(80);
void setup(){
    WiFi.mode(WIFI_AP);
    Serial.begin(115200);
    Serial.println("ESP32 mode: WIFI_AP");

    const char* ssid     = "";
    const char* password = "";
    const String HTMLPage=""
    "<!DOCTYPE html>\n"
    "<html>\n"
    "<head>\n"
    "<style>\n"
    "    h1 {text-align:center;}\n"
    "    p {text-align:center;}\n"
    "    fieldset {\n"
    "        text-align:center;\n"
    "        display: block;\n"
    "        margin-left: 100px;\n"
    "        margin-right: 100px;\n"
    "        padding-top: 0.35em;\n"
    "        padding-bottom: 0.625em;\n"
    "        padding-left: 0.75em;\n"
    "        padding-right: 0.75em;\n"
    "        border: 2px groove (internal value);\n"
    "    }\n"
    "    form {text-align:center;}\n"
    "    input[type=text] {\n"
    "        margin: 8px 0;\n"
    "        box-sizing: border-box;\n"
    "        border: 3px solid #ccc;\n"
    "        -webkit-transition: 0.5s;\n"
    "        transition: 0.5s;\n"
    "        outline: none;\n"
    "    }\n"
    "    input[type=text]:focus {\n"
    "        border: 3px solid #555;\n"
    "    }\n"
    "    input[type=password] {\n"
    "        margin: 8px 0;\n"
    "        box-sizing: border-box;\n"
    "        border: 3px solid #ccc;\n"
    "        -webkit-transition: 0.5s;\n"
    "        transition: 0.5s;\n"
    "        outline: none;\n"
    "    }\n"
    "    input[type=password]:focus {\n"
    "        border: 3px solid #555;\n"
    "    }\n"
    "    select{\n"
    "        border: 3px solid #ccc;\n"
    "    }\n"
    "    select:focus{\n"
    "        border: 3px solid #555;\n"
    "    }\n"
    "    body.sansserif {\n"
    "        font-family: Arial, Helvetica, sans-serif;\n"
    "    }\n"
    "</style>\n"
    "</head>\n"
    "<body class=\"sansserif\">\n"
    "\n"
    "    <h1>Project \"IOUTI\"</h1>\n"
    "\n"
    "    <p>Login WebPage for your ESP32</p>\n"
    "    <fieldset>\n"
    "        <form method=\"post\">\n"
    "                SSID:<br>\n"
    "            <input type=\"text\" name=\"SSID\"><br>\n"
    "                PASSWORD:<br>\n"
    "            <input type=\"password\" name=\"PASS\"><br>\n"
    "                ESP32 type:<br><br>\n"
    "            <select name=\"type\">\n"
    "                <option value=\"none\" selected>None</option>\n"
    "                <option value=\"light\">Light</option>\n"
    "                <option value=\"Example1\">Lorem Ipsum</option>\n"
    "                <option value=\"Example2\">Ipsum Lorem</option>\n"
    "              </select>\n"
    "              <br><br>\n"
    "            <input type=\"submit\" value=\"Submit\">\n"
    "        </form>\n"
    "    </fieldset>\n"
    "</body>\n"
    "</html>\n";



    WiFi.softAP("ESP32_IOT");
    Serial.println("ESP32 acces point started");
    Serial.print("Server is at: ");
    Serial.println(WiFi.softAPIP());
    server.begin();

    while(true){
      // listen for incoming clients
      WiFiClient client = server.available();
      if (client) {
          Serial.println("new client");
          String currentLine = "";                // make a String to hold incoming data from the client
      }
      // listen for incoming clients
      while (client.connected()) {            // loop while the client's connected
        if (client.available()) {             // if there's bytes to read from the client,
          char c = client.read();             // read a byte, then
          Serial.write(c);                    // print it out the serial monitor
          if (c == '\n') {                    // if the byte is a newline character
            // if the current line is blank, you got two newline characters in a row.
            // that's the end of the client HTTP request, so send a response:
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            String httpResponse = "";
            httpResponse += "HTTP/1.1 200 OK\r\n";
            httpResponse += "Content-type:text/html\r\n\r\n";

            // then the HTML page
            httpResponse += HTMLPage;

            // The HTTP response ends with a blank line:
            httpResponse += "\r\n";
            client.println(httpResponse);
            client.stop();
            Serial.println("Client Disconnected.");
          }
        }
      }
  }
}


void loop(){
    delay(1000);
}
