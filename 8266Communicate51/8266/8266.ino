#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
char DataPackage_temp[12]= {'t','e','m','p',':'};
char DataPackage_prad[10] = {'p','r','a','d',':'};           
//SoftwareSerial serial(2, 3); //RX=2,TX=3
//WiFi config
const char *ssid = "mooc"; 
const char *password = "moo cmooc";  

//mqtt config
const char *mqtt_broker = "39.103.183.108";
const char *mqtt_sub_topic = "/iot/40/app";
const char *mqtt_pub_topic = "/iot/40/device";
// const char *mqtt_username = "8266";
// const char *mqtt_password = "123";
const int mqtt_port = 1883;
WiFiClient espClient;
PubSubClient client(espClient);

int led=2;                          //led io
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

String commond ;
String message ;

void setup() {

  Serial.begin(9600);
  Serial.println("start");

  pinMode(led,OUTPUT); 
  digitalWrite(led,1);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
   delay(500);
   Serial.println("Connecting to WiFi..");
  }

  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);

  while (!client.connected()) {
    String client_id = "esp8266-client";
    // client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the  mqtt broker\n", client_id.c_str());
    // if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      if (client.connect(client_id.c_str())) {
    } else {
        Serial.print("failed with state ");
        Serial.print(client.state());
        delay(2000);
    }
  }

  // publish and subscribe
  client.publish(mqtt_pub_topic, "hello emqx");
  client.subscribe(mqtt_sub_topic);
}
void callback(char *topic, byte *payload, unsigned int length) {
  //APP->8266
   Serial.print("Message arrived ");
  // Serial.print("in topic: "): 
  // Serial.println(topic);
  // Serial.print("Message:");
  //  for (int i = 0; i < length; i++) {
  //      Serial.print((char) payload[i]);
  //  }
   //Serial.println();
   message = String((char*)payload);
   //Serial.println("message is :"+message);
   message = message.substring(2,message.length()-4);
   //Serial.println("new message is:"+message);
   if(message.substring(0,7).equals("set_led")){
     int t = (int)message.charAt(message.length()-2)-'0';
     digitalWrite(led,t);
   // Serial.println("MQTT set led");

     if(t==0){
     Serial.print("$01_Ligt01On__#");
     }
     if(t==1){
     Serial.print("$01_Ligt01Off_#");
     }
     delay(1000);
   }
   
   //Serial.println("-----------------------");
}

void loop() {
  client.loop();

  // Serial.println("first");
  
  if (stringComplete) {// 单片机已通过串口传送过来数据包
   // Serial.println(inputString.substring(1,inputString.length()-1));
    commond = inputString.substring(4,8);
    message = inputString.substring(8,inputString.length()-1);
   // Serial.println("commond is:"+commond);

    switch (inputString[2]){
      case '2':{//means 51 to 8266
        if(commond.equals("Ligt")){
    //      Serial.println("message.substring:"+message.substring(2));
          if(message.substring(2).equals("On_"))
            digitalWrite(led,0);
          else
            digitalWrite(led,1);
          Serial.println("Ligt is ok");
        }
        if(commond.equals("Temp")){
    //        Serial.println("Temperature is got from 51");
            char buf[7];
            message.toCharArray(buf,7,0);
            for (int i = 0; i < 7; i++)
            {
              DataPackage_temp[i+5] = buf[i];
            }
            client.publish(mqtt_pub_topic,DataPackage_temp);
            delay(100);
            //send teperature by MQTT;
        }
        if(commond.equals("Prad")){
     //     Serial.println("Photo sensor value is got from 51");
          char buf[5];
          message.toCharArray(buf,5,0);
          for (int i = 0; i < 5; i++)
          {
            DataPackage_prad[i+5] = buf[i];
          }
          client.publish(mqtt_pub_topic,DataPackage_prad);
          delay(100);
        }
        break;
      }
    }
    // clear the string:
    inputString = "";
    commond = "";
    message = "";
    stringComplete = false;
}
  
while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
     // 下面就是串口的通讯协议了
    if (inputString[0]!='$'){
      inputString = "";
      continue;
    }
    if (inChar == '#') {
      stringComplete = true;
    }
  }
}
