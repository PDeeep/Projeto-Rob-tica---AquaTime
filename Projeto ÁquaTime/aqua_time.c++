#include <LiquidCrystal.h>
#include <Keypad.h>

#define Luz_Fundo  7

// Define o número de linhas e colunas do teclado 4x4
const byte ROWS = 4;
const byte COLS = 4;

// Define os pinos do teclado
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Define os pinos do teclado
byte rowPins[ROWS] = {0, 1, 2, 7};
byte colPins[COLS] = {8, 10, 12, 13};

// Define o objeto keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Define o objeto LiquidCrystal
LiquidCrystal lcd(11, 9, 6, 5, 4, 3);

// Variáveis globais
String login = "";
bool chuveiroLigado = false;
unsigned long chuveiroStartTime;
const char user[] = "1234";
const int tempoChuveiro = 10; // 600 para 10 minutos
const byte ledPin = 13;
boolean blink = false;

void keypadEvent(KeypadEvent key);

void telaInicial() {
  lcd.clear();
  lcd.print("Bem-vindo!");
  login = ""; // Limpa o login
};
                      
  int led = A0;

void setup() {
  lcd.begin(16, 2);
  pinMode(Luz_Fundo,OUTPUT); //define o pino como saída
  digitalWrite(Luz_Fundo,HIGH); // Liga a luz do display.
  delay(100);
  lcd.clear();
  lcd.print("Bem-vindo!");
  keypad.addEventListener(keypadEvent);
}

void keypadEvent(KeypadEvent key) {
  switch (keypad.getState()) {
    case PRESSED:
      switch (key) {
        case '#':
          if (login.length() > 0) {
            login.remove(login.length() - 1);
            lcd.clear();
            lcd.print("Login: ");
            lcd.print(login);
          }
          break;
        case '*':
          digitalWrite(ledPin, !digitalRead(ledPin));
          break;
        case 'D':
          if (chuveiroLigado) {
            // Desliga o chuveiro
            chuveiroLigado = false;
              digitalWrite(led, LOW);
            // Encerre a contagem de tempo (se necessário)
            lcd.clear();
			lcd.print("Chuveiro desligado!");
			delay(3000);
			telaInicial();
          }
          break;
        default:
          if (!chuveiroLigado) {
            // Aceita outras teclas apenas se o chuveiro estiver desligado
            // Implemente a lógica específica para as teclas A, B e C aqui, se necessário
          }
          break;
      }
      break;
    case RELEASED:
      switch (key) {
        case '*':
          digitalWrite(ledPin, !digitalRead(ledPin));
          blink = false;
          break;
      }
      break;
    case HOLD:
      switch (key) {
        case '*':
          blink = true;
          break;
      }
      break;
  }
}


void loop() {
  while (!keypad.getKey()) {
    char key = keypad.getKey();
  
    if (key) {
      if (key == '*') {
        if (login == user) {
          chuveiroLigado = true;
          chuveiroStartTime = millis();
          lcd.clear();
          lcd.print("Chuveiro ligado!");
          digitalWrite(led, HIGH);
        } else {
          login = "";
          lcd.clear();
          lcd.print("Login inválido!");
          delay(1500);
          telaInicial();
        }
      } else {
        login += key;
        lcd.clear();
        lcd.print("Login: ");
        lcd.print(login);
      }
    }

    if (chuveiroLigado) {
      unsigned long tempoPassado = millis() - chuveiroStartTime;
      int tempoRestante = max(0, tempoChuveiro - (tempoPassado / 1000));

      lcd.setCursor(0, 1);
      lcd.print("Tempo: ");
      lcd.print(tempoRestante);

      if (tempoRestante == 0) {
        chuveiroLigado = false;
        digitalWrite(led, LOW);
        lcd.clear();
        lcd.print("Tempo esgotado!");
        delay(3000);
        telaInicial();
      }
    }
  } 	
}
