#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"
#include "textbox_LED.h"
using namespace std;

#define ADDRESS "tcp://io.adafruit.com:1883"
#define CLIENTID "Beagle2"
#define TOPIC "loganparanto/feeds/project.blink" 
#define AUTHMETHOD "loganparanto"
#define AUTHTOKEN "aio_nSRG05LoLvycfWIuGVJoF27iDstW"
#define QOS 1
#define TIMEOUT 10000L
#define THRESHOLD 30

volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    LED USR3 = LED(3); 
    int i;
    char *payloadptr;
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: ");
    payloadptr = (char *)message->payload;


    // blink USR3 
    int blink_num = atoi(payloadptr);
    printf("Blinking USR3", blink_num);
    USR3.blink(blink_num);

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

int main(int argc, char *argv[])
{
    MQTTClient client;
    MQTTClient_connectOptions opts = MQTTClient_connectOptions_initializer;
    int rc;
    int ch;

    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    opts.keepAliveInterval = 20;
    opts.cleansession = 1;
    opts.username = AUTHMETHOD;
    opts.password = AUTHTOKEN;
    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
    if ((rc = MQTTClient_connect(client, &opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }
    printf("Subscribing to topic %s\n for client %s using QoS%d\n\n"
           "Press Q<Enter> to quit\n\n",
           TOPIC, CLIENTID, QOS);
    MQTTClient_subscribe(client, TOPIC, QOS);


    do
    {
        ch = getchar();
    } while (ch != 'Q' && ch != 'q');
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}
