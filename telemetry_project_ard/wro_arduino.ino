# include <dht.h>

enum HCSR04_PIN {
    TRIG = 2,
    ECHO = 3
};

const int DHT11_PIN = 0;

double humidity, temperature;
float speed_of_sound = 0.034f;
int duration, distance;

dht DHT;

void readDHT11() {
    DHT.read11(DHT11_PIN);

    humidity = DHT.humidity;
    temperature = DHT.temperature;
}

void readHCSR04() {
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    duration = pulseIn(ECHO, HIGH);
    distance = duration * speed_of_sound / 2;
}

void setup() {
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
}

void loop() {}