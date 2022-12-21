#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

class API {
public:
  API (const String baseURL)
    : baseURL{baseURL}
  { }

  int post (const String route, const String data) {
    WiFiClient client;
    HTTPClient http;
    http.begin(client, baseURL + route);
    return http.POST(data);
  }

private:
  const String baseURL;
};
