#include <ESP8266WiFi.h>

const char* ssid = "Sadhana";
const char* password = "123456789";

WiFiServer server(80);

#define PIR D2      // PIR Sensor OUT
#define LED D5      // LED Pin

void setup() {
  Serial.begin(115200);

  pinMode(PIR, INPUT);
  pinMode(LED, OUTPUT);

  digitalWrite(LED, LOW);

  // Connect to WiFi
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected Successfully!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {

  int motion = digitalRead(PIR);

  if (motion == HIGH) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }

  WiFiClient client = server.available();

  if (!client) {
    return;
  }

  while (!client.available()) {
    delay(1);
  }

  client.readStringUntil('\r');

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();

  client.println("<!DOCTYPE html>");
  client.println("<html>");
  client.println("<head>");
  client.println("<meta charset='UTF-8'>");
  client.println("<meta name='viewport' content='width=device-width, initial-scale=1'>");
  client.println("<meta http-equiv='refresh' content='2'>");

  client.println("<style>");
  client.println("body{margin:0;padding:0;background:linear-gradient(135deg,#74ebd5,#ACB6E5);font-family:Arial;text-align:center;}");
  client.println(".container{margin:60px auto;width:350px;background:white;padding:25px;border-radius:15px;box-shadow:0px 0px 20px rgba(0,0,0,0.3);}");
  client.println("h1{color:#2c3e50;}");
  client.println(".status{font-size:30px;font-weight:bold;margin-top:20px;}");
  client.println(".led{font-size:24px;margin-top:15px;color:#34495e;}");
  client.println("</style>");

  client.println("</head>");
  client.println("<body>");

  client.println("<div class='container'>");
  client.println("<h1>🚶 Motion Detection System</h1>");

  if (motion == HIGH) {
    client.println("<p class='status' style='color:red;'>🚨 Motion Detected</p>");
    client.println("<p class='led'>💡 LED Status : ON</p>");
  } else {
    client.println("<p class='status' style='color:green;'>✅ No Motion Detected</p>");
    client.println("<p class='led'>💡 LED Status : OFF</p>");
  }

  client.println("</div>");
  client.println("</body>");
  client.println("</html>");

  client.stop();
}