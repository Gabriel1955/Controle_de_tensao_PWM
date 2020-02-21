#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x3F, 16, 2);

const int analogIn = A0;
const int pwm = 3;

float voltInput = 5;

int voltCurrentPWM = 255;
int maxVoltagePWM = 255;
float chain = 0;
float maxChain = 0.4;


void printInDisplay(float V, float A){//mostra do LCD valor da tensao de currente

  lcd.setCursor(0, 0); 
  lcd.print("Tensao "); 
  lcd.setCursor(7, 0);
  lcd.print(V);
  lcd.setCursor(0, 1); 
  lcd.print("Corrente ");
  lcd.setCursor(9, 1); 
  lcd.print(A);
}
float getChain(){//calcula a media, aplica a funcao de calibracao e retorna a currente
  float media = 0;
  int fator = 1000;
  for(int i =0; i< fator; i++){
    media = media + analogRead(analogIn);
  }
  media = media/ fator;
  return (media*0.026)-13.0;
  
}
void setVoltage(int V){//seta o valor da voltagem entre 0 a 255
  voltCurrentPWM = V;
}
float getVoltage(){//pega o valor da voltagem entre 0 a 255
  return voltCurrentPWM;
}
float getShowVoltage(){//pega o valor de voltagem em volts
   return (voltInput*getVoltage())/255;
}



void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(pwm, OUTPUT);
  
}

void loop() {
  chain = getChain();
  printInDisplay(getShowVoltage(),chain);

  if(getVoltage()> maxVoltagePWM)//usuario espefica quanto da tensao de entrada repassar para a carga
    setVoltage(maxVoltagePWM);//se tensao maior que o pemitido, set tensao no limiar

  
  if(chain > maxChain)//estou de corrente
    setVoltage(getVoltage()-1);//diminuir tensao
  else//se corrente estabilizada
    if(getVoltage() < maxVoltagePWM)//verificar se voltagem esta menor que tensao maxima
       setVoltage(getVoltage()+1);// aumentar tensao ate a maxima permitida
  if(getVoltage()< 0)// se tensao estiver abaixo de zero
    setVoltage(0);// set tensao para zero
    
  analogWrite(pwm, getVoltage());//passando tensao para o pino do arduino
}
