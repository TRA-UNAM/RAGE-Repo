

#include <WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>

// Update these with values suitable for your network.

const char* ssid = "Connectify-me";//"Connectify-me";//"IZZI-0723";
const char* password ="hola1234";//"hola1234"; //"704FB8270723";
const char* mqtt_server = "192.168.4.154";


//int LED_BUILTIN = 2;


WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

static const int servoPin = 4; //availible {4, 16, 18, 19, 21}
Servo servo1;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  //Serial.println(byteTFloat( payload,length));

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '0') {
    servo1.write(90);
    digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
    delay(1000);
  } else if ((char)payload[0] == '1'){
    digitalWrite(LED_BUILTIN, LOW);  // Turn the LED off by making the voltage HIGH
    servo1.write(180);
    delay(1000);
  } else if((char)payload[0] == '2'){
    digitalWrite(LED_BUILTIN, LOW);  // Turn the LED off by making the voltage HIGH
    servo1.write(0);
    delay(1000);
  }

}
/*
float byteTFloat(byte* start,unsigned int length){
  int point;
  float res=0.0;
  for(unsigned int i=0;i<length; i++){
      if((char)start[i]=='.'){
        point=i;
        Serial.println(point);
        break;
      }
  }
  unsigned int pot=10;
  for(unsigned int i=point+1;i<length; i++){
      int temp=(int) start[i]-48;
      res+=(float)temp/pot;
      pot*=10;
  }
  pot=1;
  int beg;
  if((char)start[0]=='-'){
    beg=0;
  }else{
    beg=-1;
  }
  for( int i=point-1;i>beg; i--){
      
      //Serial.println((int) start[i]);
      int temp=(int) start[i]-48;
      res+=(float)temp*pot;
      pot*=10;
  }
  res=(beg==0)?res*-1:res;
  return res;
  
}*/

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1884);
  client.setCallback(callback);
  servo1.attach(
        servoPin, 
        Servo::CHANNEL_NOT_ATTACHED, 
        0,
        180,
        1000,
        2000
    );
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

}
