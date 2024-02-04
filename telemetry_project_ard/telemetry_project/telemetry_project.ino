# include <Adafruit_MPU6050.h>
# include <Adafruit_Sensor.h>
# include <SoftwareSerial.h>
# include <DFRobot_SIM808.h>
# include <Servo.h>
# include <Wire.h>
# include "DHT.h"

# define DHTTYPE DHT11
# define POT A2

# define TRIG 3
# define ECHO 2
# define PIN_TX 10
# define PIN_RX 11

# define DHT11_PIN A1
# define SERVO_PIN 4
# define SERVO_TRANSISTOR 7
# define ESC_TRANSISTOR 5
# define ESC_PIN 8
# define buzzer 22
# define red_led 23
# define green_led 25

Servo servo;
Servo esc;

DHT dht(DHT11_PIN, DHTTYPE);

int humidity, temperature;
float speed_of_sound = 0.034f;
int duration, distance;

char new_device[] = "\ndate:%d-%d-%d\ntime:%d.%d.%d\nrequest_type:%s\nrequest_id:%d\ndevice_id:%d\ngps:%d\ngps_find_satellite:%d\ngps_data:%d,%d\ncompass_angle:%d\nultrasonic:%d\nhumidity:%d\ntemperature:%d\nservo_angle:%d\nheart_beat_rate:%hd\n";
char cur_req[] = "\ndate:%d-%d-%d\ntime:%d.%d.%d\nrequest_type:%s\nrequest_id:%d\ndevice_id:%d\ngps:%d\ngps_find_satellite:%d\ngps_data:%d,%d\nmust_go:%d,%d\ndistance:%d\ncompass_angle:%d\nmust_go_angle:%d\nultrasonic:%d\nhumidity:%d\ntemperature:%d\nservo_angle:%d\nheart_beat_rate:%hd\n";

char buf[350];

short day_s, month_s, year_s;
short hour_s, minute_s, second_s;
char * req_type;
short req_id, dev_id;
short gps_w;
short gps_f_s;
float x, y;
float m_x, m_y;
int x_, y_;
int m_x_, m_y_;
int dis;
short comp_ang, m_comp_ang;
short ultra, humid, temp;
short servo_ang;
short hb_rate;
short tresh = 580;
short raw_signal;
short display_data;
short i;

int safe_strt=0;
String msg = "";

SoftwareSerial ss(PIN_TX, PIN_RX);
DFRobot_SIM808 sim808(&ss);
Adafruit_MPU6050 mpu;

double gyro_value = 0;

void nf_values() {
  year_s = 2023;
  month_s = 11;
  day_s = 2;
  minute_s = 17;
  hour_s = 19;
  second_s = 12;

  x_ = 893;
  y_ = -7954;
}

void readSIM808() {
  year_s = sim808.GPSdata.year;
  month_s = sim808.GPSdata.month;
  day_s = sim808.GPSdata.day;
  minute_s = sim808.GPSdata.minute;
  hour_s = sim808.GPSdata.hour;
  second_s = sim808.GPSdata.second;

  x_ = sim808.GPSdata.lat * 100;
  y_ = sim808.GPSdata.lat * 100;
}

void readHBR() {
    hb_rate = max(min(random(hb_rate - 2, hb_rate + 2), 110), 70);
}

void readDHT11() {
    humidity = dht.readHumidity() * 100;
    temperature = dht.readTemperature() * 100;
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
void esc_safe() {
  if (safe_strt<45) {
    esc.write(safe_strt);
    safe_strt=safe_strt+5;
    delay(150);
    }
  else{
    esc.write(safe_strt);
    }
}

void servo_strt() {
  servo.write(90);
}

void send_msg(char buf[], short send_size) {
    Serial.print("begin");

    for (int i = 0; i < strlen(buf); i ++) {
        Serial.print(buf[i]);

        delay(1000 / send_size);
    }
    
    Serial.print("end");
}

void setup() {
    ss.begin(57600);
    Serial.begin(57600);
    dht.begin();


//    for (i = 0; i < 15 && !sim808.init(); i ++) {
//      delay(1000);
//      Serial.print("Sim808 init error\r\n");
//    }

    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    pinMode(SERVO_TRANSISTOR, OUTPUT);
    pinMode(ESC_TRANSISTOR, OUTPUT);
    pinMode(buzzer, OUTPUT);
    pinMode(red_led, OUTPUT);
    pinMode(green_led, OUTPUT);
    readDHT11();
    readHCSR04();

    req_type = "NEW_DEVICE";
    req_id = 0;
    dev_id = 1991;

    m_x = 1.0;
    m_y = 1.0;

    m_x_ = m_x * 100;
    m_y_ = m_y * 100;

    comp_ang = 90;
    servo_ang = 0;

    if(sim808.attachGPS())
      gps_w = 1;
    else 
      gps_w = 0;

    if(gps_w)
      gps_f_s = 1;
    else
      gps_f_s = 0;

    if (sim808.getGPS())
      readSIM808();
    else
      nf_values();

//    dht.begin();
    digitalWrite(buzzer, HIGH);
    delay(300);
    digitalWrite(buzzer, LOW);
    delay(100);
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    delay(100);
    digitalWrite(red_led, HIGH);
    digitalWrite(green_led, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(50);
    digitalWrite(buzzer, LOW);
    digitalWrite(red_led, LOW);
    digitalWrite(green_led, LOW);
    while (true) {
        Serial.write("break");
        
        if (Serial.available()) {
            while (Serial.available() > 0) { Serial.read(); }
            break;
        }
        
        delay(1000);
    }
    digitalWrite(buzzer, HIGH);
    delay(50);
    digitalWrite(buzzer, LOW);
    delay(50);
    digitalWrite(buzzer, HIGH);
    delay(50);
    digitalWrite(buzzer, LOW);
    delay(50);

    randomSeed(analogRead(A2));

    hb_rate = random(70, 110);

    sprintf(buf, new_device, year_s, month_s, day_s, hour_s, minute_s, second_s, req_type, req_id, dev_id, gps_w, gps_f_s, x_, y_, comp_ang, distance, humidity, temperature, servo_ang, hb_rate);

    send_msg(buf, 64);

    req_type = "CUR_DATA";

    servo.attach(SERVO_PIN);
    esc.attach(ESC_PIN, 1000, 2000);

     while (!mpu.begin()) {
         delay(10);
     }

     mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
     mpu.setGyroRange(MPU6050_RANGE_500_DEG);
     mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

      esc.write(safe_strt);
      //delay(1000);  //test_for_esc
      digitalWrite(ESC_TRANSISTOR, HIGH);
      digitalWrite(SERVO_TRANSISTOR, HIGH);
}

void loop() {
    //pot_value = analogRead(POT);
    //pot_value = map(pot_value, 0, 1023, 0, 180);

    //esc.write(pot_value)

     sensors_event_t a, g, temp;
     mpu.getEvent(&a, &g, &temp);
     gyro_value += (g.gyro.z - 0.03) * 30; // bu degerlerle oynayabilir cnm
    //Serial.println(gyro_value);

    // if (gyro_value > 0)
    //     gyro_value = gyro_value - ((int) gyro_value / 60)  * 60;
    // else
    //     gyro_value = gyro_value - ((int) gyro_value / - 60) * - 60;
    if (sim808.getGPS())
      readSIM808();
    
    digitalWrite(green_led, HIGH);
    raw_signal = analogRead(A0);

    //Serial.println(raw_signal);

    readHBR();

    display_data = hb_rate;

    if (raw_signal < tresh)
        display_data = 0;

    delay(100);

    readDHT11();
    readHCSR04();
    esc_safe();
    servo_strt();

    char c;

    while (Serial.available() > 0) {
        c = (char) Serial.read();
        msg.concat(c);
    }

    msg.trim();

    //Serial.println(msg);

    //Serial.println(msg);

    if (msg.indexOf("servo") != -1) {
        servo.write(20);
        delay(1000);
        servo.write(140);
        delay(1000);
        servo.write(90);
        delay(1000);
        msg = msg.substring(msg.indexOf("servo") + 5);
    } else if (msg.indexOf("motor") != -1) {
        esc.write(40);
        delay(2000);
        esc.write(0);
        delay(4000);
        esc.write(40);
        delay(2000);
        msg = msg.substring(msg.indexOf("motor") + 5);
      }
else if (msg.indexOf("auto") != -1) {
        digitalWrite(ESC_TRANSISTOR, HIGH);
        digitalWrite(SERVO_TRANSISTOR, HIGH);
        digitalWrite(red_led, LOW);
//        esc.write(0);
//        delay(2000);
//        esc.write(40);
        msg = msg.substring(msg.indexOf("auto") + 4);
      }
      else if (msg.indexOf("manual") != -1) {
        digitalWrite(ESC_TRANSISTOR, LOW);
        digitalWrite(SERVO_TRANSISTOR, LOW);
        digitalWrite(red_led, HIGH);
        esc.write(0);
        msg = msg.substring(msg.indexOf("manual") + 6);
      }

      Serial.println(msg);

    // comp_ang = gyro_value * 24;
    m_comp_ang = 90;
    //servo_ang = servo.read();

    servo.write(90 + min(max(gyro_value, -60), 50)); // bu degerleri not al bebegim

    //servo.write(45 + min(max(gyro_value * - 24, - 45), 45));
    // Serial.println(45 + min(max(gyro_value * - 24, - 45), 45));

    sprintf(buf, cur_req, year_s, month_s, day_s, hour_s, minute_s, second_s, req_type, req_id, dev_id, gps_w, gps_f_s, x_, y_, m_x_, m_y_, dis, comp_ang, m_comp_ang, distance, humidity, temperature, servo_ang, display_data);

    send_msg(buf, 256);

    req_id ++;
}
