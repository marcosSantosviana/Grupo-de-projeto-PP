#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

 // CONFIGURAÇÂO DO BLYNK
#define BLYNK_TEMPLATE_ID "TMPL2FDj383R6"
#define BLYNK_TEMPLATE_NAME "card"
#define BLYNK_AUTH_TOKEN "QdOFdGLeZQHfIs-vs_O6fvwcwsXEY43w"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Daniela_2.4G";
char pass[] = "1q2w3e4r";

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int LOmais = 32;
int LOmenos = 33;
int sensorPin = 34;
int sensor = 34;
int threshold = 2000;
unsigned long tempoAnterior = 0;
int bpm;

void setup() {
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);  

  pinMode(LOmais, INPUT);
  pinMode(LOmenos, INPUT);

  Wire.begin(21,22);
// Liga o OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)){
    Serial.println("Erro OLED");
    while(true);
  }
}

void loop() {

  Blynk.run();

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
// VERIFICA SE OS ELETRODOS ESTAO CONECTADOS
  if(digitalRead(LOmais) == 1 || digitalRead(LOmenos) == 1) {

    display.println("ELETRODO");
    display.println("DESCONECTADO");

  } else {
    int valor = analogRead(sensorPin);

    display.print("ECG: ");
    display.println(valor);
    display.print("BPM:");
    display.println(bpm);

    Serial.println(valor);

    delay(100);

    Blynk.virtualWrite(V0, valor);
    Blynk.virtualWrite(V1, bpm);
  }
// manda aparecer na tela.
  display.display();

  delay(10);

  int valor = analogRead(sensor);

// Calcula BPM
  if (valor > threshold) {

    unsigned long tempoAtual = millis();
    unsigned long intervalo = tempoAtual - tempoAnterior;

    bpm = 60000 / intervalo;

    Serial.print("BPM: ");
    Serial.println(bpm);

    tempoAnterior = tempoAtual;

    delay(300); // evita contar o mesmo batimento 
}
}