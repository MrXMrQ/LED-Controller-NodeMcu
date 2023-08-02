# led-controller-NodeMcu

This is the program running on the NodeMcu ESP8266. It starts a web server and handles GET and POST requests. Make sure that the NodeMcu can connect to your Wi-Fi network by changing the following variables:

```bash
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "WIFI_PASSWORD";
```
Also, ensure that the NodeMcu retains its IP address. Modify the following variables; you can find the numbers in your Wi-Fi dashboard.

```bash
const IPAddress staticIP(123, 456, 7, 890); -> Add a free static IP address
const IPAddress gateway(192, 168, 2, 1); -> Gateway
const IPAddress subnet(255, 255, 255, 0); -> Subnet address
```

In most cases, the gateway and subnet address data are correct, but still verify them!
