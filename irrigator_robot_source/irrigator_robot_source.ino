#include <SevenSegmentTM1637.h>
// Пин для помпы
#define POMP_PIN        4
// Пин для датчика влажности 0
#define HUMIDITY_PIN0    A0
// Пин для датчика влажности 1
#define HUMIDITY_PIN1    A1
// Пин для датчика влажности 2
#define HUMIDITY_PIN2    A2
// Пин для датчика влажности 3
#define HUMIDITY_PIN3    A3
// Минимальный порог влажности почвы
#define HUMIDITY_MIN    400
// Максимальный порог влажности почвы
#define HUMIDITY_MAX    600
// Интервал между проверкой на полив растения
#define INTERVAL        60000 * 5 
// Пины для дисплея
#define CLK 9
#define DIO 8 
// Переменная для хранения показания влажности почвы
unsigned int humidityOv = 0;
 
// Статическая переменная для хранения времени
unsigned long waitTime = 0;

// Инициализация дисплея
SevenSegmentTM1637    display(CLK, DIO);
 
void setup(void)
{
  // пин помпы в режим выхода
  pinMode(POMP_PIN, OUTPUT);
  Serial.begin(9600);
  display.begin();            
  display.setBacklight(100);  
  display.print("INIT");
}
 
void loop(void)
{
  // Считываем текущие показания датчиков влажности 
  int humidityNow0 = analogRead(HUMIDITY_PIN0);
  int humidityNow1 = analogRead(HUMIDITY_PIN1);
  int humidityNow2 = analogRead(HUMIDITY_PIN2);
  int humidityNow3 = analogRead(HUMIDITY_PIN3);

  int humidityOvNow = (humidityNow0 + humidityNow1 + humidityNow2 /* + humidityNow3 */) / 3;
  // если показания текущей влажности почвы
  // не равняется предыдущему запросу 
  if(humidityOvNow != humidityOv) {
    // сохраняем текущие показания влажности
    humidityOv = humidityOvNow;
    if (humidityOvNow > HUMIDITY_MAX || humidityOvNow < HUMIDITY_MIN)
      display.print("POOR");
    else
      display.print("NICE");
    delay(1000);
    display.clear();
  }

  Serial.print("1 - ");
  Serial.print(humidityNow0);
  Serial.print(" 2 - ");
  Serial.print(humidityNow1);
  Serial.print(" 3 - ");
  Serial.print(humidityNow2);
  Serial.print(" 4 - ");
  Serial.print(humidityNow3);
  Serial.print(" Overall - ");
  Serial.println(humidityOv);
  
  // если прошёл заданный интервал времени
  // и значения датчика влажности меньше допустимой границы
  if ((waitTime == 0 || millis() - waitTime > INTERVAL) && 
  humidityOv > HUMIDITY_MIN /*|| humidityOv > HUMIDITY_MAX*/) {
    // включаем помпу
    digitalWrite(POMP_PIN, HIGH);
    // ждём 10 секунд
    delay(10000);
    // выключаем помпу
    digitalWrite(POMP_PIN, LOW);
    // приравниваем переменной waitTime
    // значение текущего времени плюс 3 минуты 
    waitTime = millis();
  }
}
