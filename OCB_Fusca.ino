// Programa : OCB - OnBoard Computer Beetles
// Autor    : Jackson Alessandro dos Santos
// Data     : 12/05/2017 - retomado 06/02/2018
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

#define DEBUG                                                // Flag de debug 
#define DEBUG_DTL
                                                             // Inclusao de Biblioteca utilizadas
#include <Arduino.h>                                            // Biblioteca Padrao Arduino
#include <Wire.h>                                               //
#include <I2Cdev.h>                                             // Biblioteca Auxiliar para controle das portas I2C
#include <Adafruit_GFX.h>                                       // Biblioteca Grafica Principal
#include <Adafruit_TFTLCD.h>                                    // Biblioteca Especifica do Hardware da Controladora
#include <Fonts/Org_01.h>                                       // Biblioteca de Fonte para o Monitor do OCB
#include <SoftwareSerial.h>
#include <Adafruit_INA219.h>                                    // Biblioteca do monitor de corrente i2c com um ADC de 12 bit, capaz de monitorar Tensao e Corrente


//#include <EmonLib.h>                                            // Biblioteca Sensor Corrente e Tensão - SCT013 não Invasivo
#include <TinyGPS++.h>                                          // Controle do GPS
#include <Sim800L.h>                                            // Controle do GSM

uint16_t                  vg_identifier  = 0x9488;              // Variavel de identificacao do Monitor LCD
float                     vg_volt_placa  = 5.00;                // Valor referencia da Voltagem dos pinos da placa em uso medido em AREF do Arduino
// global variables
float vg_shuntvoltage = 0;
float vg_busvoltage = 0;
float vg_current_mA = 0;
float vg_loadvoltage = 0;
                                                                // Variaveis de medicao dos sensores
float                     v_colAnte      = 0;           

double                    HOME_LAT       = -30.011997;          // Latitude da Casa
double                    HOME_LON       = -51.182666;          // Longitude da Casa
const int                 INA_addr       = 0x40;                // Endereço I2C do INA219 

                                                             // Configuracao das Portas 
                                                                // Definicao Variaveis
                                                                   // 22-29          - Digital     - Dados o Monitor TFT LCD
Adafruit_TFTLCD           monitor(40, 38, 39, 42, 41);             // 38-42          - Digital     - Controle o Monitor TFT LCD       


int                       pinoCorrente   = A6;                     // A6             - Analogica   - Entrada Corrente nao Invasiva - SCT013
int                       pinoTensao     = A7;                     // A7             - Analogica   - Entrada Voltagem da Bateria
int                       pinoOxig       = A8;                     // A8             - Analogica   - Entrada Sonda Lambda Universal
                                                                // Definicao Objetos
//                        Serial1                                  // 18,19          - Digital     - Configura pinos Rs e Tx do GPS
Sim800L                   GSM(17,16);                              // A15,A14        - Analogica   - Configura Rx e Tx de GSM - 69,68

TinyGPSPlus               gps;                               // Objeto de controle do GPS
//EnergyMonitor             emon1;                             // Objeto de controle da Corrente e Tensao/Bateria
Adafruit_INA219           ina219(INA_addr);                  // Objeto de controle da Tensao e Corrente


// Define nomes legiveis das Cores dos valores de 16-bit - https://stackoverflow.com/questions/13720937/c-defined-16bit-high-color
#define BLACK            0x0000      /*   0,   0,   0 */
#define NAVY             0x000F      /*   0,   0, 128 */
#define DARKGREEN        0x03E0      /*   0, 128,   0 */
#define DARKCYAN         0x03EF      /*   0, 128, 128 */
#define MAROON           0x7800      /* 128,   0,   0 */
#define PURPLE           0x780F      /* 128,   0, 128 */
#define OLIVE            0x7BE0      /* 128, 128,   0 */
#define LIGHTGREY        0xC618      /* 192, 192, 192 */
#define DARKGREY         0x7BEF      /* 128, 128, 128 */
#define BLUE             0x001F      /*   0,   0, 255 */
#define GREEN            0x07E0      /*   0, 255,   0 */
#define CYAN             0x07FF      /*   0, 255, 255 */
#define RED              0xF800      /* 255,   0,   0 */
#define MAGENTA          0xF81F      /* 255,   0, 255 */
#define YELLOW           0xFFE0      /* 255, 255,   0 */
#define WHITE            0xFFFF      /* 255, 255, 255 */
#define ORANGE           0xFD20      /* 255, 165,   0 */
#define GREENYELLOW      0xAFE5      /* 173, 255,  47 */ 

///////////
// Setup //
///////////

void setup() 
{  

  Wire.begin();                                   // 

#ifdef DEBUG  
  Serial.begin(38400);                            // Inicializa Serial
#endif

  iniciaTFT();

  preparaMonitor();

  ina219.begin();                                 // Inicializa o sensor de Tensao e Corrente
  ina219.setCalibration_16V_400mA();
  
  Serial1.begin(9600);                            // Inicializa a porta Serial1 para GPS

  /*GSM.begin(4800);     
  Serial.println("GET PRODUCT INFO: ");
  Serial.println(GSM.getProductInfo());
  delay(50);
  Serial.println("GET OPERATORS LIST: ");
  Serial.println(GSM.getOperatorsList());
  delay(50);
  Serial.println("GET OPERATOR: ");
  Serial.println(GSM.getOperator());
  delay(50);

  char* text;     //text for the message. 
  char* number;    //change to a valid number.
  bool error;

  text="Teste do Arduino do Pai";     //text for the message. 
  number="+5551999775512";    //change to a valid number.
  error=GSM.sendSms(number,text);

  number="51999775512";    //change to a valid number.
  error=GSM.sendSms(number,text);
*/

  pinMode(pinoOxig     , INPUT);                  // Define Pino Lambda                como Entrada
  pinMode(pinoCorrente , INPUT);                  // Define Pino Corrente nao Invasiva como Entrada
  pinMode(pinoTensao   , INPUT);                  // Define Pino Voltagem da Bateria   como Entrada


#ifdef DEBUG  
  i2c_scanner(); 
#endif

} 
//////////
// Loop //
//////////
void loop() 
{ 
  monitor.setTextSize(2);
  monitor.setTextColor(WHITE, BLUE);
  imprimeTexto("Monitor","C",8);
  monitor.setTextColor(WHITE);
  
  apresentaMonitor(); 
  
  limpaArea();
}

///////////////////
// i2c_scanner
///////////////////
void i2c_scanner()
{
  byte error, address;
  
  Serial.println("Scanning I2C...");
 
  int nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    
    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
 
      nDevices++;
    }
    else if (error==4)
    {
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
}
