/* Código para ligação do Wemos no Arduino e ativação do Alto Falante.
O sensor a ser utilizado para controlar o ALto Falante será o circuito de detectação de Pulso.

Esse Wemos não estará ligado diretamente ao relay e sim a um Arduino Uno que, usando a função
tone(), acionará um alto falante.
 
Usar a saída 'r' por questão de pinagem diferente do Wemos D1 mini para o Arduino. 
No caso caso o pino D1 do Wemos correspondo ao pino 5 do Arduino. Ver mapeação das pinagens 
em: https://github.com/esp8266/Arduino/issues/1243
   */

// --- ESP8266 ---
#include <ESP8266WiFi.h>

const char* ssid = "........";
const char* password = "........";

WiFiClient wemosd1mini;

// --- MQTT ---
#include <PubSubClient.h>

const char* mqtt_server = "iot.eclipse.org";
const char* ir = "danca/infravermelho/analogico/speaker";
const char* wemos1 = "danca/status/wemos/1/speaker";
const char* esp = "speaker";

PubSubClient client(wemosd1mini);

   
#define r 5   // o pino 5 corresponde ao pino D1 no Wemos
void setup (){

  Serial.begin(115200);
 
  setupWiFi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
	pinMode(r, OUTPUT);
	digitalWrite(r, LOW);
}

void setupWiFi(){
  delay(100);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println(".");
    
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  client.publish(wemos1, "Dançarino 1 conectado", true);
  
}

void reconnect(){

  while(!(client.connected())){

    client.publish(wemos1, "Reconectando...", true);

    if(client.connect(esp)){
      client.publish(wemos1, "Conectado");
      client.subscribe(ir);
    } else {
      
      String clientstatus = String(client.state()).c_str();
      String erro = "Falha ao reconectar, rc = " + clientstatus;
      client.publish(wemos1, String(erro).c_str());
      delay(1500);
      client.publish(wemos1, "Tentaremos de novo em 2 segundos");
      delay(2000);
      
    }
  } 
}

void passaValor(int valorAnalogico){
  
 /* O valor de XX deve ser bastante alto, afim de fazer a maquina de fumaça funcionar só em grande
 variação de saída analógica.
*/

  if (valorAnalogico >= 800 ){  
    digitalWrite(r, HIGH);
  }
else{
    digitalWrite(r, LOW);
  }  
}

void callback(char* topic, byte* payload, unsigned int length) {
  
  Serial.print("Mensagem recebida [");
  Serial.print(topic);
  Serial.print("]: ");

  for(int i = 0; i < length; i++){
    Serial.print((char)payload[i]);
  }
  Serial.println("--------------------------");

  int val = 0;

  for(int i = 0; i < length; i++){
    val += (int)payload[i];
  }

  passaValor(val);
  
}


void loop(){

  if(!(client.connected())){
    reconnect();
  }
  
   client.loop();
	

}
