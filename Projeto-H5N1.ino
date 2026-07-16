#include <Wire.h>
#include <Keypad_I2C.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

#define BUZZER_PIN 6     // Pino para o buzzer ativo
#define LED_RED_PIN 2    // Pino para o transistor que controla o LED vermelho
#define LED_GREEN_PIN 4  // Pino para o transistor que controla o LED verde
#define LED_BLUE_PIN 5   // Pino para o transistor que controla o LED azul

// Definindo display
#define endereco 0x27  // Definição de endereço I2C do display no canal 0x27
#define colunas 16     // Definimos número de colunas do LCD
#define linhas 2       // Definimos número de linhas do LCD
#define button 2
#define debounceTimeout 100
LiquidCrystal_I2C lcd(endereco, colunas, linhas);  // Define tela LCD a usar

// Definindo teclado matricial
#define enderecoTeclado 0x26  // Definição de endereço I2C do teclado no canal 0x26
const byte ROWS = 4;          // número de linhas no teclado
const byte COLS = 3;          // número de colunas no teclado
// array que armazena as teclas (Mapeação de teclas)
char keys[ROWS][COLS] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};

// Definição dos pinos do PCF8574 i/o port
byte rowPins[ROWS] = { 3, 2, 1, 4 };  // conecta aos pinos de linha do teclado
byte colPins[COLS] = { 5, 6, 7 };     // conecta aos pinos de coluna do teclado

// Cria um objeto para o teclado, com os parâmetros mapa de teclas, nº de linhas no teclado, nº de colunas no teclado,
// endereço I2C do teclado e o tipo de Expansor de Porta utilizado (Esta biblioteca, em específico, suporta os chips PCF8574, PCF8574A e PCF8575).
Keypad_I2C keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS, enderecoTeclado);
//Keypad_I2C keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS, enderecoTeclado, PCF8574 );
//Keypad_I2C teclado(makeKeymap(keys), rowPins, colPins, linhas, colunas, enderecoTeclado, PCF8574 );
//Keypad_I2C keypad(0x26, 3, 2, 1, 4, 5, 6, 7); // Endereço e pinos do teclado matricial 3x4

const int BUZZER_FREQUENCY = 261;  // Frequência do tom do buzzer em Hz 2000

char customKey;
const char* colors[] = { "Vermelho", "Verde", "Azul" };

void setup() {
  lcd.init();  // Inicializa o Display LCD
  //lcd.begin();
  keypad.begin();
  lcd.clear();      // Limpa a tela LCD
  lcd.backlight();  // Deixa a luz de fundo do display LCD ligada

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_BLUE_PIN, OUTPUT);
  //digitalWrite (BUZZER_PIN, HIGH);
  lcd.init();       // INICIA A COMUNICAÇÃO COM O DISPLAY
  lcd.backlight();  // LIGA A ILUMINAÇÃO DO DISPLAY
  lcd.clear();      // LIMPA O DISPLAY
  lcd.print("SELECIONE A COR:");
  delay(100);
}

void loop() {

  lcd.init();       // INICIA A COMUNICAÇÃO COM O DISPLAY
  lcd.backlight();  // LIGA A ILUMINAÇÃO DO DISPLAY
  lcd.clear();      // LIMPA O DISPLAY
  lcd.print("1.Vermelho");
  lcd.setCursor(0, 1);
  lcd.print("2.Verde 3.Azul");

  while (true) {
    customKey = keypad.getKey();
    if (customKey != NO_KEY) {
      tone(BUZZER_PIN, BUZZER_FREQUENCY);  // Feedback do buzzer
      delay(100);
      noTone(BUZZER_PIN);

      switch (customKey) {
        case '1':
          setColor(255, 0, 0);  // Vermelho
          break;
        case '2':
          setColor(0, 255, 0);  // Verde
          break;
        case '3':
          setColor(0, 0, 255);  // Azul
          break;
        case '#':
          return;  // Enter para sair
        case '*':
          lcd.clear();  // Delete para reiniciar a seleção
          break;
        default:
          // Erro de seleção
          tone(BUZZER_PIN, BUZZER_FREQUENCY * 2);  // Tom de erro
          delay(200);
          noTone(BUZZER_PIN);
          break;
      }
    }
  }
}

void setColor(int red, int green, int blue) {
  analogWrite(LED_RED_PIN, red);
  analogWrite(LED_GREEN_PIN, green);
  analogWrite(LED_BLUE_PIN, blue);
}
