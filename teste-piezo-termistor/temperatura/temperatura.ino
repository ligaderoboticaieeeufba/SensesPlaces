#include <Thermistor.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

Thermistor temp(0);

const char* ssid = "C-137";
const char* password = "livramento501";
const char* mqtt_server = "iot.eclipse.org";
const char* teste_topico1 = "teste/temperatura/analogico";
const char* teste_topico2 = "teste/temperatura/celsius";
const char* teste_topico3 = "teste/impacto/analogico";
const char* mqtt_ClientID = "testeCI";

WiFiClient espClient;
PubSubClient client(espClient);

#define Z A0
int pin_seletores[] = {16, 5, 4}; // A(D0), B(D1), C(D2)
int val = 0;
int leitura_sensor = 0;

void setup_wifi() {

  delay(500);
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

void reconecta(){
  while(!client.connected()){
    client.connect(mqtt_ClientID);
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();

  Serial.println("");
  Serial.println("Criando o servidor..");
  delay(3000);
  client.setServer(mqtt_server, 1883);
  
  for(int pin = 0; pin < 3; pin++){
    pinMode(pin_seletores[pin], OUTPUT);
  }
  
}


void loop() {

  if(!client.connected()){
    reconecta();
  }

  //  C = 0   B = 0   A = 1
  digitalWrite(pin_seletores[0], HIGH);
  digitalWrite(pin_seletores[1], LOW);
  digitalWrite(pin_seletores[2], LOW);
  
  int temperatura_lida = temp.getTemp();
  
  Serial.print("A temperatura é: ");
  Serial.print(temperatura_lida);
  Serial.println("*C");

  String temperatura = String(temperatura_lida).c_str();
  String celsius = temperatura + "*C";
  String resultado = "A temperatura é: " + celsius;

  val = analogRead(Z);
  Serial.print("Leiturea analógica: ");
  Serial.println(val);

  String valor_lido = String(val).c_str();
  String analogico = "O valor analógico é: " + valor_lido;

  // C = 1  B = 0  A = 0
  digitalWrite(pin_seletores[0], LOW);
  digitalWrite(pin_seletores[1], LOW);
  digitalWrite(pin_seletores[2], HIGH);

  leitura_sensor = analogRead(Z);
  String leitura_analogica = String(leitura_sensor).c_str();
  String resultado_piezo = "O valor do piezo é: " + leitura_analogica;

  Serial.println(resultado_piezo);

  if(temperatura_lida > 110){
    //Serial.println("PERAI MEU PARTRÃO");
    client.publish(teste_topico1, "TA PEGANDO FOGO BIXO", true);
    //client.publish(teste_topico2, "TA PEGANDO FOGO BIXO", true);
    
  }

  else if(temperatura_lida < -200){
    //Serial.println("TA VENDO MEU PATRÃO, FUDEU O BIXO");
    client.publish(teste_topico1, "TA VENDO MEU PATRÃO, FUDEU O BIXO", true);
    //client.publish(teste_topico, "TA VENDO MEU PATRÃO, FUDEU O BIXO", true);
  
  }

  else{
    
    //Serial.println("VOU PUBLICAR, MEU PATRÃO");
    client.publish(teste_topico2, String(resultado).c_str(), true);
    client.publish(teste_topico1, String(analogico).c_str(), true);
    client.publish(teste_topico3, String(resultado_piezo).c_str(), true);
  }
  
  delay(500);

}
