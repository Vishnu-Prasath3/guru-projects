#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Access Point (AP) credentials
const char* apSSID = "OIL_SEPARATOR";
const char* apPassword = "";

// Relay pins
#define r1 5 //d5
#define r2 4 //d6
#define r3 0 //d1
#define r4 2//d2

bool var1 =false;
bool var2 =false;
bool var3 =false;
bool var4 =false;

// Create an AsyncWebServer object on port 80
AsyncWebServer server(80);
void setup() {
  Serial.begin(115200);

  // Set up the ESP8266 as an access point
  WiFi.softAP(apSSID, apPassword);

  // Print the IP address of the access point
  Serial.print("Access Point IP address: ");
  Serial.println(WiFi.softAPIP());

  // Initialize relay pins as outputs
  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);
  pinMode(r3, OUTPUT);
  pinMode(r4, OUTPUT);
  

  
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", R"=====(
    <!DOCTYPE html>
<html>
<head>
  <title>Rover Control</title>
  <style>
  h1{
        font-size:100px;
      }
    .btn{
    border:4px solid white;
    height:200px;
    width:600px;
    display:block;
    color:white;
    background-color: rgb(48, 48, 48);;
    font-size: 80px;
    margin:auto;}
    .rm{
      color:red;
    }
   

body{
    background-color:black;
}
  </style>
  <script>
    async function controlRover(direction) {
      const response = await fetch(`/control?direction=${direction}`);
    }
  </script>
</head>
<body><center>
  <h1 class="rm">REMOTE_CONTROL</h1>
  </center>
  <form>
    <button class="btn forward" type='button' onclick='controlRover("forward")'>Forward</button>
    <button class="btn backward" type='button' onclick='controlRover("backward")'>Backward</button>
    <button class="btn left" type='button' onclick='controlRover("left")'>Left</button>
    <button class="btn right" type='button' onclick='controlRover("right")'>Right</button>
  </form>
</body>
</html>
    )=====");
  });

  // Route to handle rover control
  server.on("/control", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("direction")) {
      String direction = request->getParam("direction")->value();
      controlRover(direction);
    }
    request->send(200); // Send empty response
  });

  // Start the server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Nothing to do here for an asynchronous web server
}

void forward(){
  digitalWrite(r1, HIGH);
   digitalWrite(r2, LOW);
   digitalWrite(r3, HIGH);
   digitalWrite(r4, LOW);
}
void backward(){
  digitalWrite(r1, LOW);
   digitalWrite(r2, HIGH);
   digitalWrite(r3, LOW);
   digitalWrite(r4, HIGH);
}
void right(){
  digitalWrite(r1, HIGH);
   digitalWrite(r2, LOW);
   digitalWrite(r3, LOW);
   digitalWrite(r4, HIGH);
}
void left(){
  digitalWrite(r1, LOW);
   digitalWrite(r2, HIGH);
   digitalWrite(r3, HIGH);
   digitalWrite(r4, LOW);
}
void off(){
  digitalWrite(r1, LOW);
   digitalWrite(r2, LOW);
   digitalWrite(r3, LOW);
   digitalWrite(r4, LOW);
}




void controlRover(String direction) {


  // Determine which relay to turn on based on direction
  if ((direction == "forward")&&(var1==false)) {
    //turn off the 1sr relay
    forward();
    var1=true;
    Serial.println("FORWARD_RELAY");

    
  } else if ((direction == "backward")&&(var2==false)) {
    //turn off the 2nd relay
    backward();
    var2=true;
    Serial.println("BACKWARD_RELAY");

    
  } else if ((direction == "left")&&(var3==false)) {
    //turn off 3rd relay
    
   left();

    var3=true;
    Serial.println("left_RELAY");

  } else if ((direction == "right")&&(var4==false)) {
   //turn 4 th relay
    right();
    var4=true;
    Serial.println("Right_RELAY");

  }
   else if ((direction == ("forward")||("right")||("backward")||("left"))&&((var1==true)||(var2==true)||(var3==true)||(var4==true))) {
  //turn off all relays
    off();
    var1=false;
    var2=false;
    var4=false;
    var3=false;
    Serial.println("relay_of");
  } 
}
