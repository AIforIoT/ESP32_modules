# ESP32_AP_LOGIN

One Paragraph of project description goes here

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.To set up the environment you must install Arduino IDE in your computer (which can be found [here](https://www.arduino.cc/en/Main/Software)) and follow [these instructions](https://www.youtube.com/watch?v=mBaS3YnqDaU) to make it compatible with your ESP32.

### Prerequisites

* ESP32
* Arduino IDE

## Running the code

This module puts the ESP32 in AP mode and initiates a simple server (IP: 192.168.4.1 by default). This server can handle a GET request to serve a web page and a POST request to receive the login data from the user. This data will be later used to connect the ESP32 to the user WiFi.

You can modify the HTML code of the served web page editing the HTMLpage string inside ESP32_AP.ino. There is a python script that converts the html_document.html to a C string. For example in Linux:

```
python HTMLtoC.py WebPage.html > WebPage.txt
```

Note that the POST information parser is constructed using the form in AP_Login.html. If you modify the form input fields of the served WebPage you must edit the POST parser consequently. To check the progress you can use the serial monitor of the Arduino IDE or Putty.

## Authors

* **Sergi Mercadé** - *Initial work* - [smerca](https://github.com/smerca)

## License
