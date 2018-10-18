
#include <WiFi.h>

WiFiServer server(80);
//enum Request {NONE,GET,POST};
const int NONE=0;
const int GET=1;
const int POST=2;


void setup(){
    WiFi.mode(WIFI_AP_STA);
    Serial.begin(115200);
    Serial.println("ESP32 mode: WIFI_AP");

    String ssid     = "";
    String password = "";
    String type     = "";
    String xPos     = "";
    String yPos     = "";


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
    "            ESP32 X axis:<br>\n"
    "            <input type=\"text\" name=\"XAXIS\"><br>\n"
    "            ESP32 Y axis:<br>\n"
    "            <input type=\"text\" name=\"YAXIS\"><br>\n"
    "\n"
    "            <input type=\"submit\" value=\"Submit\">\n"
    "        </form>\n"
    "    </fieldset>\n"
    "</body>\n"
    "</html>\n"
    "";



    WiFi.softAP("ESP32_IOT");
    Serial.println("ESP32 acces point started");
    Serial.print("Server is at: ");
    Serial.println(WiFi.softAPIP());
    server.begin();
    boolean flag=false;
    boolean whileExit=false;
    String postBody="";                     //MAke a String to save post body data
    int contentLenght=0;
    int index=0;
    int requestDetected=NONE;               //Flag that shows if the request has been detected
                                            //- 0 : Not detected
                                            //- 1 : GET detected
                                            //- 2 : POST detected
    String currentLine = "";                // make a String to hold incoming data from the client

    while(true){
        // listen for incoming clients
        WiFiClient client = server.available();
        if (client) {
            Serial.println("new client");
            while (client.connected()) {            // loop while the client's connected
            if (client.available()) {             // if there's bytes to read from the client,
                char c = client.read();             // read a byte, then
                Serial.print(c);                    // print it out the serial monitor
                currentLine=currentLine+c;

                if(requestDetected==NONE){
                    if(currentLine.startsWith("GET")){
                        requestDetected=GET;
                    }
                    if(currentLine.startsWith("POST")){
                        requestDetected=POST;
                    }
                }
            }
            switch (requestDetected) {
                case NONE:     //RESQUEST NOT DETECTED
                    //??
                break;
                case GET:     //GET DETECTED
                    if(currentLine.endsWith("\r\n\r\n")){
                        if(flag==false){
                            flag=true;
                        }else{
                            flag=false;
                            Serial.println("END REQUEST");
                            whileExit=true; //while exit
                        }
                    }
                break;
                case POST:     //POST DETECTED
                    if(currentLine.endsWith("\r\n") && contentLenght== 0){
                        index=currentLine.indexOf("Content-Length: ");
                        if(index >=0){
                            contentLenght= currentLine.substring(index+16).toInt();
                        }
                    }
                    if(currentLine.indexOf("\r\n\r\n")>0){
                            contentLenght--;
                            if(contentLenght<0){
                                postBody=currentLine.substring(currentLine.indexOf("\r\n\r\n"));
                                contentLenght=0;
                                whileExit=true;
                            }
                    }
                break;
            }
            if(whileExit==true){
                break;
            }
        }//END client connected
        whileExit=false;
        // Check to see if the client request was "GET" or "POST":
        if (requestDetected==POST) {
            //CAUTION, PASSWORD NOT ENCRYPTED
            Serial.println("POST REQUEST");
            String httpResponse = "";
            httpResponse += "HTTP/1.1 200 OK\r\n";
            httpResponse += "Content-type:text/html\r\n\r\n";
            httpResponse += "<h1>POST OK</h1>";
            httpResponse += "\r\n";

            client.println(httpResponse);
            //Example of body of POST: SSID=TEST&PASS=123456789&type=light
            //Parse SSID
            int lastIndex=postBody.indexOf("&");
            ssid=postBody.substring(postBody.indexOf("=")+1,lastIndex);
            ssid.replace('+',' ');
            //Parse PASS
            int initIndex=postBody.indexOf("=",lastIndex);
            lastIndex=postBody.indexOf("&",lastIndex+1);
            password=postBody.substring(initIndex+1,lastIndex);
            //Parse TYPE
            initIndex=postBody.indexOf("=",lastIndex);
            lastIndex=postBody.indexOf("&",lastIndex+1);
            type=postBody.substring(initIndex+1,lastIndex);
            //Parse xPos
            initIndex=postBody.indexOf("=",lastIndex);
            lastIndex=postBody.indexOf("&",lastIndex+1);
            xPos=postBody.substring(initIndex+1,lastIndex);
            //Parse yPos
            initIndex=postBody.indexOf("=",lastIndex);
            yPos=postBody.substring(initIndex+1);


            Serial.println(ssid);
            Serial.println(password);
            Serial.println(type);
            Serial.println(xPos);
            Serial.println(yPos);
            requestDetected=NONE;
            currentLine="";
            client.stop();
            
            Serial.print("Attempting to connect to SSID: ");
            Serial.println(ssid
            );
            WiFi.begin(ssid.c_str(), password.c_str());
            int count=20;
            while (WiFi.status() != WL_CONNECTED && count>=0) {
                delay(500);
                Serial.print(".");
                count--;
            }
            Serial.println(" ");
            if(WiFi.status() == WL_CONNECTED){               
                Serial.println("WiFi connected");
                Serial.println("IP address: ");
                Serial.println(WiFi.localIP());
                break;
            }else{
                Serial.println("Unable to connect to: ");
                Serial.println(ssid);
            }
            
        }
        if (requestDetected==GET) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            Serial.println("GET REQUEST");
            String httpResponse = "";
            httpResponse += "HTTP/1.1 200 OK\r\n";
            httpResponse += "Content-type:text/html\r\n\r\n";

            // then the HTML page
            httpResponse += HTMLPage;

            // The HTTP response ends with a blank line:
            httpResponse += "\r\n";
            client.println(httpResponse);
            currentLine="";
            requestDetected=NONE;
            client.stop();
            Serial.println("Client Disconnected.");
        }
      }
    }//END WHILE
}//End setup


void loop(){
    delay(1000);
    Serial.println("loop");
}
