#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "MQTTClient.h"
#include <json-c/json.h>
#include <wiringPi.h>
#include <unistd.h>

//#define ADDRESS     "tcp://192.168.1.32:1883"
#define ADDRESS     "tcp://192.168.43.176:1883"
//#define ADDRESS     "tcp://192.168.43.220:1883"
#define CLIENTID    "sat3nPi2"
#define AUTHMETHOD  "saten"
#define AUTHTOKEN   "sat3n"
#define TOPIC       "ee513/CPUTemp"
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L

using namespace std;

//json parse
void json_parse(json_object *jobj) {
 enum json_type type;
 json_object_object_foreach(jobj, key, val) {
 type = json_object_get_type(val);
 double temperature;
 switch (type) {
   case json_type_double: printf("type: json_type_double, \n");
   printf("value: %fn \n", json_object_get_double(val));
   temperature = json_object_get_double(val);
   break;
 }
 //gpio
 if (temperature >= 50){
     printf("raspberry pi gpio pin 11 is used \n");
	if (wiringPiSetup() == -1){
		printf("Exit");
	}
	pinMode (0,OUTPUT);
	digitalWrite(0, 1);
    usleep(100000);
	digitalWrite(0, 0);
    usleep(100000);
	
	//pinMode (0,OUTPUT);
 }
}
}


volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt) {
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    int i;
    char* payloadptr;
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: ");
    payloadptr = (char*) message->payload;
    for(i=0; i<message->payloadlen; i++) {
        putchar(*payloadptr++);
    }
    putchar('\n');
    json_object * jobj = json_tokener_parse((char*) message->payload);
    json_parse(jobj);
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void connlost(void *context, char *cause) {
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

int main(int argc, char* argv[]) {
    MQTTClient client;
    MQTTClient_connectOptions opts = MQTTClient_connectOptions_initializer;
    int rc;
    int ch;

    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_DEFAULT, NULL);
    opts.keepAliveInterval = 20;
    opts.cleansession = 0;
    opts.username = AUTHMETHOD;
    opts.password = AUTHTOKEN;

    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
    if ((rc = MQTTClient_connect(client, &opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }
    printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
           "Press Q<Enter> to quit\n\n", TOPIC, CLIENTID, QOS);
    MQTTClient_subscribe(client, TOPIC, QOS);

    do {
        ch = getchar();
    } while(ch!='Q' && ch != 'q');
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}

