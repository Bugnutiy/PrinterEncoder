#include <Arduino.h>
// 17.02 добавлена эмуляция нажатия клавиш право-лево при удержании нажатия более заданной величины

#define ENC_A_PIN 2 // вход А
#define ENC_B_PIN 3 // вход Б

#define L_konc 5 // левый концевик (задний)
#define R_konc 4 // правый концевик (передний)

#define L_key 6     //  кнопка влево (движение вперед)
#define R_key 7     //  кнопка вправо (двиение назад)
#define Start_key 8 // кнопка стартовая позиция

#define STEP_PIN 13 // сигнал STEP
#define DIR_PIN 12  //  сигнал направления

#define DINT_ 0

int startfiltr;

int impa, impb;

const bool TESTENC = false; // если поставить true то тогда в режиме монитора порта будет выводить импульсы енкодера
// в рабочем варианты надо обязательно поставить false

const bool DEBG = false; //  выставить true если надо посмотреть импульсы от концевиков и кнопок, поставить false в рабочей программе

const bool TESTIMP = false;

// **********************************

#define VERT_STEP_PIN 9 // вертикальный STEP D9
#define VERT_DIR_PIN 10 // вертикальный DIR  D10

#define U_key 14 // Кнопка вверх  А0
#define D_key 15 // Кнопка вниз   А1

#define U_konc 16 // Концевик вверху А2
#define D_konc 17 // Концевик внизу А3

#define Laser 18 // Лазерный уровнемер A4

/******** CODEDAT0 ******/
#define DAT_LOTOK A5
#define DAT_PAPER 11
#define LOTOK(VAL) digitalWrite(DAT_LOTOK, VAL)
#define PAPER(VAL) digitalWrite(DAT_PAPER, VAL)

#define LOTOKON LOTOK(LOW)
#define LOTOKOFF LOTOK(HIGH)
#define PAPERON PAPER(LOW)
#define PAPEROFF PAPER(HIGH)
/*********** CODE DATE *********/

// **********************************

#define DIR_UP false  // если направление двигателя вверх-вниз при включении совпадает с кнопками вверх/вниз true, иначе false
#define DIR_DOWN true // если направление двигателя вверх-вниз при включении совпадает с кнопками вверх/вниз false, иначе true

// **********************************
bool U_konc_ON; // digitalRead(U_konc)==LOW
bool D_konc_ON; // digitalRead(D_konc)==LOW

bool U_pressed; // digitalRead(U_key)==LOW
bool D_pressed; // digitalRead(D_key)==LOW

bool L_trig; // триггеры удержания левой правой клавиши
bool R_trig; //
volatile unsigned long L_hold;
volatile unsigned long R_hold;

#define U_konc_ONN digitalRead(U_konc) == LOW
#define D_konc_ONN digitalRead(D_konc) == LOW

#define U_pressedN digitalRead(U_key) == LOW
#define D_pressedN digitalRead(D_key) == LOW

int ukoncfiltr;
int dkoncfiltr;

int ukeyfiltr;
int dkeyfiltr;

bool EmulPressUp = false;

#define Laser_OFF (digitalRead(Laser) == HIGH)

//*******************************************************************************************

bool U_trigger = false;
bool D_trigger = false;

//******************************************************************************************

unsigned long pps = 17; //   количество сигналов от энкодера для одного шага двигателя !!!меньше 5 не надо ставить - иначе не успеваетработать

unsigned long pps_pol = 17; //  регулировка скорости движка в режиме половинной скорости. Количество сигналов от энкодера для одного шага двигателя на режиме половинной скорости, если поставить 36 (2*pps), то скорость будет при старте в два раза медленнее, если 54 (3*pps) то в три раза медленнее

//****** HOLD **************

volatile unsigned long LR_hold_time = 1700; // количество миллисекунд при удержании клавиш право лево при которых они фиксируются и удерживаются до нажатия противоположной клавиши или до сработки любого концевика

/**** CODEDAT *********/
unsigned long pps_half = 6; //  нельзя ставить = 0!!! количество сигналов от энкодера после которых включается режим половинной скорости

volatile unsigned long time_lotok = 1100;    // через сколько миллисекунд после старта режима половинной скорости срабоает датчик лотка
volatile unsigned long time_paper = 1110;    // через сколько миллисекунд после старта режима половинной скорости срабоает датчик бумаги
volatile unsigned long time_lotok1 = 600000; // через сколько миллисекунд после старта режима половинной скорости сбросится датчик лотка

volatile unsigned long time_half = 2300; // через сколько миллисекунд после старта режима половинной скорости движек начнет крутить с обычной скоростью
bool TRIG_CONTROL_CODEDAT = false;

/********* ftnl *************/

volatile unsigned long time_dlav = 320; // через сколько миллисекунд после пропадания сигнала наезда на препятствие отключится вертикальное перемещение должно быть меньше 3000!!!
bool FTNW;
//***************

unsigned int interval_key_hor = 2000; //  интервал между шагами при нажатии клавиши влево вправо !!!меньше 70 не надо ставить - иначе не успевает отработать
unsigned int step_from_start = 100;   // на сколько шагов отьезжать после прихода на стартовую позицию

unsigned int interval_key_vert = 200; // интервал между шагами при нажатии клавиши вверх/вниз

#define DIR_LEFT true   // если напрвление двигателя синхронно с енкодером true иначе false
#define DIR_RIGHT false // если направление двигателя синхронно с енкодером false иначе true

#define START_RIGHT false //  true - если стартовая позиция слева false - если стартовая позиция справа

#define START_LEFT true //  true - если стартовая позиция справа false - если стартовая позиция слева

//*******************************************************************************************

int lkoncfiltr;
int rkoncfiltr;

int lkeyfiltr;
int rkeyfiltr;

const int NUMFILTRC = 5; // здесь выставляется параметр фильра от помех на контактах. чем больше число, тем больше количество импульсов надочто бы сигнал был принят

bool L_konc_ON;
bool R_konc_ON;

bool L_pressed;
bool R_pressed;

#define L_konc_ONN digitalRead(L_konc) == LOW
#define R_konc_ONN digitalRead(R_konc) == LOW

unsigned LenghT = 0;

#define L_pressedN digitalRead(L_key) == LOW
#define R_pressedN digitalRead(R_key) == LOW

#define Start_pressed digitalRead(Start_key) == LOW

//*******************************************************************************************
bool L_trigger = false;
bool R_trigger = false;

bool Start_trigger1 = false;
bool Start_trigger2 = false;

bool WorkMode = false;

//

// Чтение значения с энкодера
#define ENC_A ((PIND & (1 << ENC_A_PIN)) > 0)
#define ENC_B ((PIND & (1 << ENC_B_PIN)) > 0)
// Пины STEP/DIR

// Отправка данных в порты STEP/DIR
// #define STEP(V) (PORTB = V ? PORTB | (1 << (STEP_PIN-8)) : PORTB & (~(1<<(STEP_PIN-8))))
// #define DIR(V) (PORTB = V ? PORTB | (1 << (DIR_PIN-8)) : PORTB & (~(1<<(DIR_PIN-8))))

#define STEP(V) digitalWrite(STEP_PIN, V)
#define DIR(V) digitalWrite(DIR_PIN, V)

void setup()
{
  intSetup();
  driveSetup();
  delay(3000);

  Serial.begin(115200);
  Serial.println("***** Принтер готов к работе *******");
}

//***************************************************

void VERT_STEP(bool st)
{
  digitalWrite(VERT_STEP_PIN, st);
}

void VERT_DIR(bool st)
{
  digitalWrite(VERT_DIR_PIN, st);
}

void driveSetup()
{
  pinMode(STEP_PIN, OUTPUT);
  STEP(0);
  pinMode(DIR_PIN, OUTPUT);
  DIR(0);

  pinMode(VERT_STEP_PIN, OUTPUT);
  VERT_STEP(0);
  pinMode(VERT_DIR_PIN, OUTPUT);
  VERT_DIR(0);
}

volatile bool A, B;
unsigned pps_var = 0;

void intSetup()
{

  /**** CODEDAT0 **********/
  pinMode(DAT_LOTOK, OUTPUT);
  pinMode(DAT_PAPER, OUTPUT);
  LOTOKOFF;
  PAPEROFF;
  /****** CODEDAT *******/

  pinMode(ENC_A_PIN, INPUT_PULLUP);
  A = ENC_A;
  attachInterrupt(0, onEncoderChannelA, CHANGE);
  pinMode(ENC_B_PIN, INPUT_PULLUP);
  B = ENC_B;
  attachInterrupt(1, onEncoderChannelB, CHANGE);

  // делаем контакты кнопок и концевиков входами
  pinMode(L_konc, INPUT_PULLUP);
  pinMode(R_konc, INPUT_PULLUP);
  pinMode(L_key, INPUT_PULLUP);
  pinMode(R_key, INPUT_PULLUP);
  pinMode(Start_key, INPUT_PULLUP);

  pinMode(Laser, INPUT_PULLUP);
  pinMode(U_konc, INPUT_PULLUP);
  pinMode(D_konc, INPUT_PULLUP);
  pinMode(U_key, INPUT_PULLUP);
  pinMode(D_key, INPUT_PULLUP);

  pps_var = pps_pol;
}

volatile unsigned long pulses = 0;
volatile bool gotDir = false;

/*** CODEDAT ********************/
volatile unsigned long pulses_trighalf = 0;
volatile unsigned long timer_lotok = 0;  // таймер лотка
volatile unsigned long timer_paper = 0;  //  таймер бумаги
volatile unsigned long timer_lotok1 = 0; // таймер лотка1
volatile unsigned long timer_half = 0;   // таймер половинной скорости
volatile unsigned long pps_V = 0;        // вспомогательная переменная
/*********************************/

//*************** FTNL ****************************
volatile unsigned long timer_dlav = 0; // таймер задерки после снятия сигнала лазера

//*************************************************

volatile bool cw = false;

int pos_x;

unsigned int step_frm_str;

unsigned int steptest;
bool dirtest;

bool LPR, RPR;

/******* CODEDAT ***************/
bool TRIG_CODEDAT;
/*****************************/
bool NOLOT = false;
bool NOBUM = false;
bool NOLOT1 = false;
bool NOHALF = false;

void loop()
{
  // LOTOKON;
  // PAPERON;
  // return;

  // проверка на концевики

  if ((L_trigger == false) && (R_trigger == false) && !L_pressed && !R_pressed)
  {
    // управление вращением по энкодеру только если концевики сброшены

    //********** CODEDAT2 *****************
    //******  если начали движение, то взводим таймеры сработки бумаги и лотка, а также таймер половинной скорости движения
    if (TRIG_CODEDAT == true)
    {
      if (pulses_trighalf > pps_half) // как только энкодер отсчитает нужное количество шагов
      {
        timer_lotok = millis();  // взвели таймер лотка
        timer_paper = millis();  // взвели таймер бумаги
        timer_lotok1 = millis(); // взвели таймер лотка1
        timer_half = millis();   // взвели таймер половинной скорости
        pulses_trighalf = 0;
        TRIG_CODEDAT = false;        // сбросить триггер половинной скорости и датчиков
        TRIG_CONTROL_CODEDAT = true; // запустили триггер контроля режим половинной скорости
        Serial.println("Пошел отсчет времени");
      }
    }
    /*******************************************/

    //************ CODEDAT3 ***************************/
    if (TRIG_CONTROL_CODEDAT)
    {
      if (((millis() - timer_lotok) >= time_lotok) && !NOLOT)
      {
        LOTOKON;
        Serial.println("Сработал датчик лотка");
        NOLOT = true;
      }

      if (((millis() - timer_paper) >= time_paper) && !NOBUM)
      {
        PAPERON;
        Serial.println("Сработал датчик бумаги");
        NOBUM = true;
      }

      if (((millis() - timer_lotok1) >= time_lotok1) && !NOLOT1)
      {
        LOTOKOFF;
        Serial.println("Сброшен датчик лотка");
        NOLOT1 = true;
      }

      if (((millis() - timer_half) >= time_half) && !NOHALF)
      {
        Serial.println("Полная скорость");
        pulses=0;
        NOHALF = true;
        pps_var = pps;
      }
    }
    /******************************************/

    if (pulses >= pps_var && NOHALF)
    {
      pulses = 0;
      STEP(1);
      delayMicroseconds(10);
      STEP(0);

      if (gotDir)
      {
        DIR(!cw);
        gotDir = false;
      }

      if (cw)
        pos_x++;
      else
        pos_x--;

      // Serial.println(pos_x);
    }
  }

  // ******************  Управление по нажатию клавиш вправо влево  **********************************************

  // ***********************************  \test \8888888888888888888888888
  if (steptest == 3000)
  {

    dirtest = false;
    char vsp[40];
    sprintf(vsp, "impa - %d , impb - %d, time - %u ", impa, impb, millis());
    if (TESTIMP)
      Serial.println(vsp);
  }

  if (steptest == 0)
  {

    dirtest = true;
    char vsp[40];
    sprintf(vsp, "impa - %d , impb - %d, time - %u ", impa, impb, millis());
    if (TESTIMP)
      Serial.println(vsp);
  }

  if (dirtest)

  {
    ++steptest;
    // L_pressed= false;
    // R_pressed= true;
  }
  else
  {
    --steptest;
    // L_pressed = true;
    // R_pressed = false;
  }

  if (TESTIMP && (steptest % 1000) == 0)
  {
  }

  // *****************************************  TEST *****************************************

  if ((L_trigger == false && L_pressed) || (R_trigger == false && R_pressed) || (U_trigger == false && U_pressed) || (D_trigger == false && D_pressed))
  {
    if (L_trigger == false && L_pressed)
    {
      // управление вращением по нажатиям клавиш лево право
      STEP(1);
    }

    if (R_trigger == false && R_pressed)
    {
      // управление вращением по нажатиям клавиш лево право
      STEP(1);
    }

    // *************** управление по нажатию клавиш вверх вниз *******************
    // ***************************************************************************

    // ********* ERROR ********************  лазер ошибка

    if (U_trigger == false && U_pressed && !U_konc_ON)
    {
      // управление вращением по нажатиям клавиш вверх
      VERT_DIR(DIR_UP);
      VERT_STEP(1);
    }

    if (D_trigger == false && D_pressed && !D_konc_ON)
    {
      // управление вращением по нажатиям клавиш вниз
      VERT_DIR(DIR_DOWN);
      VERT_STEP(1);
    }

    //// ************************** вторая фаза ***************************************

    if ((L_pressed || R_pressed) && (D_pressed || U_pressed))
      delayMicroseconds(interval_key_hor);
    else
    {
      if (L_pressed || R_pressed)
        delayMicroseconds(interval_key_hor);
      if (D_pressed || U_pressed)
        delayMicroseconds(interval_key_vert);
    }
    //***************************************************

    if (L_trigger == false && L_pressed)
    {
      // управление вращением по нажатиям клавиш лево право
      STEP(0);
    }

    if (R_trigger == false && R_pressed)
    {
      // управление вращением по нажатиям клавиш лево право
      STEP(0);
    }

    // *************** управление по нажатию клавиш вверх вниз *******************
    // ***************************************************************************

    if (U_trigger == false && U_pressed)
    {
      // управление вращением по нажатиям клавиш вверх
      VERT_STEP(0);
    }

    if (D_trigger == false && D_pressed)
    {
      // управление вращением по нажатиям клавиш вниз
      VERT_STEP(0);
    }

    //// ************************** третья фаза ***************************************

    if ((L_pressed || R_pressed) && (D_pressed || U_pressed))
      delayMicroseconds(interval_key_hor);
    else
    {
      if (L_pressed || R_pressed)
        delayMicroseconds(interval_key_hor);
      if (D_pressed || U_pressed)
        delayMicroseconds(interval_key_vert);
    }

    /// *******************************************************************************

    if (L_trigger == false && L_pressed)
    {
      // управление вращением по нажатиям клавиш лево право
      DIR(DIR_LEFT);
    }

    if (R_trigger == false && R_pressed)
    {
      // управление вращением по нажатиям клавиш лево право
      DIR(DIR_RIGHT);
    }

    // *************** управление по нажатию клавиш вверх вниз *******************
    // ***************************************************************************

    if (U_trigger == false && U_pressed)
    {
      // управление вращением по нажатиям клавиш вверх
      VERT_DIR(DIR_UP);
    }

    if (D_trigger == false && D_pressed)
    {
      // управление вращением по нажатиям клавиш вниз
      VERT_DIR(DIR_DOWN);
    }
  }

  // алгоритм выставления уровня
  // запускается когда уходит до концевика противоположного стартовой позиции
  // Фаза 1. каретка опускается вниз до сработки лазера ***** сделано *****

  // Фаза 2. при нажатии на стартовую позицию идет цикл отработки стартовой позиции
  // при его выполнении система контролирует сработку лазера
  // если сработка есть то блокируем вертикальное перемещение, эмулируем нажатие вверх пока не прекратится сработка
  // если сработка прекратилась, то разблокируем вертикальное перемещение.

  // управление по отработке стартовой позиции

  if (Start_trigger1)

    //**************** FTNW *******************

    if (Laser_OFF || ((millis() - timer_dlav) < time_dlav))
    // если нет наезда на препятствие едем на стартовую, иначе подымаем вверх
    {

      EmulPressUp = true;

      if (Laser_OFF)
      {                        // перевзводим таймер задержки выключения после отмены сигнала лазера
        timer_dlav = millis(); // взводим/перевзводим таймер лотка
      }

      // Фиксируем факт первой сработки лазера
      if (FTNW == false)
      {
        FTNW = true;
      }
    }
    else
    {

      FTNW = false;

      STEP(1);
      delayMicroseconds(interval_key_hor);
      STEP(0);
      delayMicroseconds(interval_key_hor);

      if (START_LEFT)
        DIR(DIR_LEFT);
      if (START_RIGHT)
        DIR(DIR_RIGHT);
    }

  if (Start_trigger2)
  {

    // delayMicroseconds(3000);

    STEP(1);
    delayMicroseconds(interval_key_hor);
    STEP(0);
    delayMicroseconds(interval_key_hor);

    if (START_LEFT)
      DIR(DIR_RIGHT);
    if (START_RIGHT)
      DIR(DIR_LEFT);

    if (step_frm_str > 0)
      step_frm_str--;
    else
    {
      Start_trigger2 = false;
      R_trigger = false;
      L_trigger = false;

      /**** CODEDAT1 ***********/
      //**** после окогчагния позиционирования стартовой позиции взводим таймер подготовки половинной скорости
      TRIG_CODEDAT = true;
      pulses_trighalf = 0;
      pps_var = pps_pol; // установлен режим половинной скорости
      Serial.println("Взвод по стартовой позиции");
      /* здесь взводится старт процедур половинной скорости и отработки датчиков */
    }
  }

  //************  фильтрация дискретных входов ******************

  //******************** концевики ******************************
  if (L_konc_ONN)
  {
    if (DEBG)
      Serial.println("Левый концевик импульс");
    lkoncfiltr = lkoncfiltr + 1;
    if (lkoncfiltr >= NUMFILTRC)
    {
      L_konc_ON = true;
      lkoncfiltr = NUMFILTRC;
    }
  }
  else
  {
    lkoncfiltr = 0;
    L_konc_ON = false;
  }

  if (R_konc_ONN)
  {
    if (DEBG)
      Serial.println("Правый концевик импульс");
    rkoncfiltr = rkoncfiltr + 1;
    if (rkoncfiltr >= NUMFILTRC)
    {
      R_konc_ON = true;
      rkoncfiltr = NUMFILTRC;
    }
  }
  else
  {
    rkoncfiltr = 0;
    R_konc_ON = false;
  }

  //************************** верх низ
  if (U_konc_ONN)
  {
    if (DEBG)
      Serial.println("Верхний концевик импульс");
    ukoncfiltr = ukoncfiltr + 1;
    if (ukoncfiltr >= NUMFILTRC)
    {
      U_konc_ON = true;
      ukoncfiltr = NUMFILTRC;
    }
  }
  else
  {
    ukoncfiltr = 0;
    U_konc_ON = false;
  }

  if (D_konc_ONN)
  {
    if (DEBG)
      Serial.println("Нижний концевик импульс");
    dkoncfiltr = dkoncfiltr + 1;
    if (dkoncfiltr >= NUMFILTRC)
    {
      D_konc_ON = true;
      dkoncfiltr = NUMFILTRC;
    }
  }
  else
  {
    dkoncfiltr = 0;
    D_konc_ON = false;
  }

  //********************** клавиши *************************

  if (L_pressedN)
  {
    if (DEBG)
      Serial.println("Левая кнопка импульс");

    lkeyfiltr = lkeyfiltr + 1;
    if (lkeyfiltr >= NUMFILTRC)
    {
      L_pressed = true;
      R_trig = false;
      lkeyfiltr = NUMFILTRC;

      // здесь проверка на удержание левой клавиши
      if ((millis() - L_hold) >= LR_hold_time)
        L_trig = true;
    }
  }
  else
  {
    lkeyfiltr = 0;
    L_pressed = false;

    // сброс счетчика нажатых клавиш для определения удержания
    L_hold = millis();
  }

  if (R_pressedN)
  {
    if (DEBG)
      Serial.println("Правая кнопка импульс");
    rkeyfiltr = rkeyfiltr + 1;
    if (rkeyfiltr >= NUMFILTRC)
    {
      R_pressed = true;
      L_trig = false;
      rkeyfiltr = NUMFILTRC;

      // здесь проверка на удержание правой клавиши
      if ((millis() - R_hold) >= LR_hold_time)
        R_trig = true;
    }
  }
  else
  {
    rkeyfiltr = 0;
    R_pressed = false;

    // сброс счетчика нажатых клавиш для определения удержания
    R_hold = millis();
  }

  //*************** EMULHOLD *************
  if (L_trig)
    L_pressed = true;
  if (R_trig)
    R_pressed = true;

  // клавиши вверх низ

  if (U_pressedN)
  {
    if (DEBG)
      Serial.println("вверх кнопка импульс");

    ukeyfiltr = ukeyfiltr + 1;
    if (ukeyfiltr >= NUMFILTRC)
    {
      U_pressed = true;
      ukeyfiltr = NUMFILTRC;
    }
  }
  else
  {
    ukeyfiltr = 0;
    U_pressed = false;
  }

  //********** эмуляция вверх при отработке стартовой позиции и наезде на препятствие **********
  if (EmulPressUp)
  {
    EmulPressUp = false;
    U_pressed = true;
  }

  if (D_pressedN)
  {
    if (DEBG)
      Serial.println("вниз кнопка импульс");
    dkeyfiltr = dkeyfiltr + 1;
    if (dkeyfiltr >= NUMFILTRC)
    {
      D_pressed = true;
      dkeyfiltr = NUMFILTRC;
    }
  }
  else
  {
    dkeyfiltr = 0;
    D_pressed = false;
  }

  //*****************************************************************
  //******************* окончание блока фильтрации ******************
  //*****************************************************************

  //******************* CODEDAT4 ***********************
  // выключение датчиков лотка и бумаги по сработке концевиков

  // сброс триггеров эмуляции нажатия клавиш при удержании более какого то времени

  if (L_konc_ON || R_konc_ON)
  {
    LOTOKOFF;
    PAPEROFF;

    L_trig = false; // триггеры удержания клавиш право лево сбрасывааем если есть конечники
    R_trig = false;

    NOLOT = false;
    NOBUM = false;
    NOLOT1 = false;
    NOHALF = false;

    pps_var = pps_pol;

    pulses_trighalf = 0;
    TRIG_CONTROL_CODEDAT = false;
    TRIG_CODEDAT = false;
  }

  if (L_konc_ON)
  {

    if (DEBG)
      Serial.println("Левый концевик нажат");
    L_trigger = true;

    WorkMode = false;

    // если START_RIGHT, то отрабатываем цикл опускания до сработки уровня
    if (START_RIGHT && !Laser_OFF)
      D_pressed = true;

    // Laser_ON наезд на препятствие

    if (Start_trigger1 && START_LEFT)
    {
      Start_trigger1 = false;
      Start_trigger2 = true;
      step_frm_str = step_from_start;
      delay(500);
      // надо разблокировать левый триггер потом
    }
  }
  if (R_konc_ON)
  {
    if (DEBG)
      Serial.println("Правый концевик нажат");
    R_trigger = true;

    WorkMode = false;

    // если START_LEFT, то отрабатываем цикл опускания до сработки уровня
    if (START_LEFT && !Laser_OFF)
      D_pressed = true;

    if (Start_trigger1 && START_RIGHT)
    {
      Start_trigger1 = false;
      Start_trigger2 = true;
      step_frm_str = step_from_start;
      delay(500);
      // надо разблокировать правый триггер потом
    }
  }

  // *********************************

  if (U_konc_ON)
  {

    if (DEBG)
      Serial.println("верхний концевик нажат");
    U_trigger = true;
  }

  if (D_konc_ON)
  {

    if (DEBG)
      Serial.println("нижний концевик нажат");
    D_trigger = true;
  }

  // *********************************

  if (L_pressed)
  {

    if (DEBG)
      Serial.println("Влево нажато");
    R_trigger = false;
  }
  if (R_pressed)
  {

    if (DEBG)
      Serial.println("Вправо нажато");
    L_trigger = false;
  }

  // *********************************

  if (U_pressed)
  {

    if (DEBG)
      Serial.println("Вверх нажато");
    D_trigger = false;
  }
  if (D_pressed)
  {

    if (DEBG)
      Serial.println("Вниз нажато");
    U_trigger = false;
  }

  //***********************************

  if (Start_pressed)
  {

    startfiltr = startfiltr + 1;
    if (startfiltr >= 5000)
    {
      Start_trigger1 = true;
      if (DEBG)
        Serial.println("Стартовая позиция нажато");
      startfiltr = 0;
    }
  }
  else
    startfiltr = 0;
}
// ************************************************************
// ************************************************************
// *******    окончание большого цикла ************************
// ************************************************************
// ************************************************************

bool previousA;

void onEncoderChannelA()
{

  int a1, a0, i;

  if (DINT_ == 0)
    goto next;

  for (i = 0; i < DINT_; i++)
    if (ENC_A)
      ++a1;
    else
      ++a0;

  //  тут количество считанных 1/0

  if ((a1 >= DINT_ / 2) && (previousA == false))
  {
    previousA = true;
    goto next;
  }
  if ((a0 >= DINT_ / 2) && (previousA == true))
  {
    previousA = false;
    goto next;
  }

  return;

next:

  if (TESTENC)
  {
    Serial.print("Импульс А");
    ++impa;
    Serial.print(impa);
    Serial.print(" ");
    Serial.println(digitalRead(ENC_A_PIN));
  }

  if (TESTIMP)
  {

    ++impa;
  }

  A = ENC_A;

  if ((A && B) || (!A && !B))
  {
    if (cw)
      gotDir = true;
    cw = true;
  }
  else
  {
    if (!cw)
      gotDir = true;
    cw = false;
  }
  pulses++;
  pulses_trighalf++;
}

bool previousB;

void onEncoderChannelB()
{

  int b1, b0, i;

  if (DINT_ == 0)
    goto next;

  for (i = 0; i < DINT_; i++)
    if (ENC_B)
      ++b1;
    else
      ++b0;

  //  тут количество считанных 1/0

  if ((b1 >= DINT_ / 2) && (previousB == false))
  {
    previousB = true;
    goto next;
  }
  if ((b0 >= DINT_ / 2) && (previousB == true))
  {
    previousB = false;
    goto next;
  }

  return;

next:

  if (TESTENC)
  {
    Serial.print("Импульс B");
    ++impb;
    Serial.print(impb);
    Serial.print(" ");
    Serial.println(digitalRead(ENC_B_PIN));
  }

  if (TESTIMP)
  {

    ++impb;
  }

  B = ENC_B;

  if ((B && A) || (!B && !A))
  {
    if (!cw)
      gotDir = true;
    cw = false;
  }
  else
  {
    if (cw)
      gotDir = true;
    cw = true;
  }
  pulses++;
  pulses_trighalf++;
}
