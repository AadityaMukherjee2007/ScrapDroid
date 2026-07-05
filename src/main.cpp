#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

WebServer server(80);

const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<title>ESP32 Car Control</title>
<style>
  body { text-align: center; font-family: Arial; margin-top: 50px; }
  .key { display: inline-block; padding: 20px; margin: 10px; background-color: #eee; border-radius: 5px; font-weight: bold; }
  .active { background-color: #ccc; }
</style>
<script>
  let currentKey = null;
  function sendCommand(cmd) {
    fetch('/' + cmd).catch(e => console.error(e));
  }
  document.addEventListener('keydown', function(event) {
    if (event.repeat) return;
    let cmd = null;
    let keyId = null;
    switch(event.key.toLowerCase()) {
      case 'w': cmd = 'f'; keyId = 'w'; break;
      case 'a': cmd = 'fl'; keyId = 'a'; break;
      case 's': cmd = 'b'; keyId = 's'; break;
      case 'd': cmd = 'fr'; keyId = 'd'; break;
      case 'q': cmd = 'lb'; keyId = 'q'; break;
      case 'e': cmd = 'rb'; keyId = 'e'; break;
    }
    if (cmd && currentKey !== cmd) {
      currentKey = cmd;
      sendCommand(cmd);
      if (keyId) document.getElementById('key-' + keyId).classList.add('active');
    }
  });
  document.addEventListener('keyup', function(event) {
    const key = event.key.toLowerCase();
    if (['w', 'a', 's', 'd', 'q', 'e'].includes(key)) {
      currentKey = null;
      sendCommand('s');
      if (document.getElementById('key-' + key)) {
        document.getElementById('key-' + key).classList.remove('active');
      }
    }
  });
</script>
</head>
<body>
  <h1>ESP32 Car Control</h1>
  <p>Use WASD to drive. Q for left-back, E for right-back.</p>
  <div>
    <div class="key" id="key-q">Q (Left Back)</div>
    <div class="key" id="key-w">W (Forward)</div>
    <div class="key" id="key-e">E (Right Back)</div><br>
    <div class="key" id="key-a">A (Left)</div>
    <div class="key" id="key-s">S (Backward)</div>
    <div class="key" id="key-d">D (Right)</div><br>
  </div>
</body>
</html>
)rawliteral";

// Motor A
int in1 = 25;
int in2 = 26;

// Motor B
int in3 = 32;
int in4 = 33;

// put function declarations here:
void forward();
void backward();
void forward_right();
void forward_left();
void backward_right();
void backward_left();
void stop();

void setup()
{
  // put your setup code here, to run once:
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  Serial.begin(115200);

  // Connect to WiFi
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Setup WebServer endpoints
  server.on("/", []() { server.send(200, "text/html", htmlPage); });
  server.on("/f", []() { forward(); server.send(200, "text/plain", "forward"); });
  server.on("/b", []() { backward(); server.send(200, "text/plain", "backward"); });
  server.on("/fl", []() { forward_left(); server.send(200, "text/plain", "forward left"); });
  server.on("/fr", []() { forward_right(); server.send(200, "text/plain", "forward right"); });
  server.on("/lb", []() { backward_left(); server.send(200, "text/plain", "left back"); });
  server.on("/rb", []() { backward_right(); server.send(200, "text/plain", "right back"); });
  server.on("/s", []() { stop(); server.send(200, "text/plain", "stop"); });

  server.begin();
}

void loop()
{
  // Handle web server requests
  server.handleClient();

  // put your main code here, to run repeatedly:
  if (Serial.available() > 0)
  {
    String data = Serial.readStringUntil('\n');
    data.trim();
    // Serial.println(data);

    // Forward & Backward
    if (data == "f")
    {
      forward();
    }
    else if (data == "b")
    {
      backward();
    }
    // Forward Turns
    else if (data == "fr")
    {
      forward_right();
    }
    else if (data == "fl")
    {
      forward_left();
    }
    // Backward Turns
    else if (data == "br")
    {
      backward_right();
    }
    else if (data == "bl")
    {
      backward_left();
    }
    // Full Stop
    else if (data == "s")
    {
      stop();
    }
    // Catch-all for unrecognized commands
    else
    {
      Serial.println("Invalid command. Use: f, b, fr, fl, br, bl, s");
    }
  }
}

// put function definitions here:
void forward()
{
  stop();
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void backward()
{
  stop();
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void forward_left()
{
  stop();
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void forward_right()
{
  stop();
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void backward_left()
{
  stop();
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void backward_right()
{
  stop();
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void stop()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}