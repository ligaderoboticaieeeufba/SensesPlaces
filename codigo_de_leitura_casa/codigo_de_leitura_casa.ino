#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

#define pinFio 5

const char* ssid = "C-137";
const char* password = "livramento501";
const char* mqtt_server = "iot.eclipse.org";

const char* temp_analogica = "teste/temperatura/analogico";
const char* temp_celsius = "teste/temperatura/celsius";
const char* piezo_analogica = "teste/piezo/analogico";
const char* batimento_coracao = "teste/coracao";
//const char* temp_celsius = "teste/temperatura/celsius";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  //pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(pinFio, OUTPUT);
  
}

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

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void ligaLuz(int val){

  if(val > 0 && val < 100){
      
    digitalWrite(pinFio, HIGH);
    delay(1000);
    digitalWrite(pinFio, LOW);
  
  }

  else if(val > 100 && val < 500){
    
    for(int i = 0; i < 3; i++){

      digitalWrite(pinFio, HIGH);
      delay(500);
      digitalWrite(pinFio, LOW);
      
    }
    
  }

  else if(val > 500 && val <= 1023){
    
    digitalWrite(pinFio, HIGH);
    delay(3500);
    digitalWrite(pinFio, LOW); 
  
  
  }
  delay(1000);
}

void ligaFumaca(int val){

  
  if(val < 100){
      
    digitalWrite(pinFio, HIGH);
    delay(1000);
    digitalWrite(pinFio, LOW);
  
  }

  else if(val > 100 && val < 500){
    
    for(int i = 0; i < 3; i++){

      digitalWrite(pinFio, HIGH);
      delay(500);
      digitalWrite(pinFio, LOW);
      
    }
    
  }

  else if(val > 500 && val <= 1023){
    
    digitalWrite(pinFio, HIGH);
    delay(3500);
    digitalWrite(pinFio, LOW); 
  
  
  }
  

}


void ligaVentilador(int val){

  
  if(val < 100){
      
    digitalWrite(pinFio, HIGH);
    delay(1000);
    digitalWrite(pinFio, LOW);
  
  }

  else if(val > 100 && val < 500){
    
    for(int i = 0; i < 3; i++){

      digitalWrite(pinFio, HIGH);
      delay(500);
      digitalWrite(pinFio, LOW);
      
    }
    
  }

  else if(val > 500 && val <= 1023){
    
    digitalWrite(pinFio, HIGH);
    delay(3500);
    digitalWrite(pinFio, LOW); 
  
  
  }
  
}

void callback(char* topic, byte* payload, unsigned int length) {
  
  Serial.print("Mensagem recebida [");
  Serial.print(topic);
  Serial.print("]: ");

  int val = 0;
  //char msg[length];
  String msg = String(topic);
  
  for(int i = 0; i < length; i++){
    msg[i] = (char)payload[i];
  }

  
  for(int i = 0; i < length; i++){
      val += (int)payload[i];
   }

  if(msg.equals(piezo_analogica)){
    
    ligaLuz(val);
  }

  else if(msg.equals(temp_analogica)){
    ligaFumaca(val);
  }

  else if(msg.equals(batimento_coracao)){
    ligaVentilador(val);
  
  }
  
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  
  
  Serial.println(msg);
  Serial.println("--------------------");

  
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    //Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
     //Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish(topico_exemplo, "hello world");
      // ... and resubscribe
      
       client.subscribe(temp_analogica);
      //client.subscribe(piezo_analogica);
      //client.subscribe(coracao_batimento);
      //client.subscribe(batimento_coracao);
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  
  client.loop();

  
}

