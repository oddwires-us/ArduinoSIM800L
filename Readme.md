
# Arduino SIM800L library
A smart HTTP library based on SIM800 that implements AT HTTP commands to perform GET and POST requests. This is forked from carrascoacd/ArduinoSIM800L

## Support
* This version supports HardwareSerial so works easily with the ESP32. If you need a SoftwareSerial version please use the original.
* The API response has to be valid JSON.
* The library has been tested against IoT-Bus GSM modules but should work with any SIM800 module that supports SSL
* This version enables additional headers to be added (e.g. for authentication/authorization
* It also enables alternative content-types to be submitted
* pre-init has been modified to support IoT-Bus GSM modules - you may need to modify that for your power on sequence
* automatic redirect can now be enabled - see the Nest GET example

## Installation
Download the library and then import it.

## Quick start!

Here's some code to perform a GET request :+1:

``` c++

#define RST_PIN 33;
HTTP http(9600, RST_PIN, true); // true indicates debugging
http.configureBearer("pwg");    // for US Mobile service
http.connect();

http.setHeader("Authorization: bearer my-authorization-code"); // optional and you can add pairs of data separated by semi-colons 

char response[256]; // long enough for your expected response!
Result result = http.get("your.api.com", response);

Serial.println(response);

http.disconnect();

```

Here's some code to perform a POST request :+1:

``` c++
#define RST_PIN 33;
HTTP http(9600, RST_PIN, false); // no debugging data
http.configureBearer(pwg");      // for US Mobile service
http.connect();

http.setContentType("your-content-type"); // valid content required otherwise no content-type will be specified
http.setHeader("Authorization: bearer my-authorization-code"); // optional and you can add pairs of data separated by semi-colons 

char response[256];
Result result = http.post("your.api.com", "{\"date\":\"12345678\"}", response);

Serial.println(response);

http.disconnect();
```

I suggest the [ArduinoJSON](https://github.com/bblanchon/ArduinoJson) library for parsing the JSON response, then you can play with the values easily.


## How does it work?
In order to perform a request, the library follows these steps:

##### Configure Bearer:

  - AT+CREG? -> try until 0,1 (connected to the network)
  - AT+SAPBR=3,1,"Contype","GPRS" -> wait for OK
  - AT+SAPBR=3,1,"APN","movistar.es" -> wait for OK
  - AT+SAPBR=1,1 -> wait for OK

##### HTTP GET:

  - AT+HTTPINIT -> wait for OK
  - AT+HTTPPARA="CID",1 -> wait for OK
  - AT+HTTPPARA="URL","your.api.com"-> wait for OK
  - AT+HTTPPARA="USERDATA","your-header data name: value pairs separated by semi-colons"->wait for OK
  - AT+HTTPSSL=0 -> wait for OK (1 when URL starts with "https://")
  - AT+HTTPACTION=0 -> wait for 200
  - AT+HTTPREAD -> read buffer and parse it
  - AT+HTTPTERM -> wait for OK
  - AT+SAPBR=0,1

##### HTTP POST:
  - AT+HTTPINIT -> wait for OK
  - AT+HTTPPARA="CID",1 -> wait for OK
  - AT+HTTPPARA="URL","your.api.com" -> wait for OK

  For example, if we have this body: {"location_id": 238, "fill_percent": 90}

  - AT+HTTPPARA="CONTENT","your-content-type"
  - AT+HTTPPARA="USERDATA","your-header data name: value pairs separated by semi-colons"
  - AT+HTTPDATA=strlen(body),10000 -> wait for DOWNLOAD, then write the body and wait 10000
  - AT+HTTPSSL=0 -> wait for OK (1 when URL starts with "https://")
  - AT+HTTPACTION=1 -> wait for ,200,
  - AT+HTTPREAD -> read buffer and parse it
  - AT+HTTPTERM -> wait for OK
  - AT+SAPBR=0,1

## Future improvements

- Support both HardwareSerial and SoftwareSerial
