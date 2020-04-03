#include <Keyboard.h>
#include <Mouse.h>
#include <Encoder.h>

#define ENCODER_COUNT 2
#define BUTTON_COUNT 7

typedef struct
{
  Encoder enc;
  char axis;
  char minus;
  char plus;
} encoder_t;

encoder_t encoders[ENCODER_COUNT] = 
{
  {Encoder(0,1), 'x', 'q', 'w'},
  {Encoder(2,3), 'y', 'o', 'p'},
};
long before[ENCODER_COUNT];
typedef struct 
{
  uint8_t pin;
  char key;
} button_t;

button_t btns[BUTTON_COUNT] =
{
  {7, KEY_RETURN}, //START
  {8, 'd'}, // Accccmmmmmmmmmmmmm
  {9, 'f'}, // B
  {10, 'j'}, // C
  {11, 'k'}, // D
  {12, 'c'}, // FX A
  {13, 'm'}, // FX B
};

int type = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for(int i = 0;i<BUTTON_COUNT; i++)
  {
    pinMode(btns[i].pin, INPUT_PULLUP);
  }
  for(int i = 0; i<4; i++)
  {
    pinMode(i, INPUT_PULLUP);
  }
  Keyboard.begin();
  delay(1);
}
long enc_position_tr1[ENCODER_COUNT] = {0};
long enc_position_tr2[ENCODER_COUNT] = {0};
long enc_position_tr3[ENCODER_COUNT] = {0};
void loop()
{
  String result = "";
  for(int i  = 0; i < BUTTON_COUNT; i++)
  {
    if(digitalRead(btns[i].pin) == LOW) {
    Keyboard.press(btns[i].key);
    Serial.println(i);
  } else if(digitalRead(btns[i].pin) == HIGH) {
    Keyboard.release(btns[i].key);
  }
    
    
    for(int i = 0; i < ENCODER_COUNT; i++)  {
    long encR = encoders[i].enc.read();    
      if(encR != 0) {
        if(enc_position_tr3[i]<=enc_position_tr2[i] && enc_position_tr2[i]<=enc_position_tr1[i] && enc_position_tr1[i]<encR)
        {
          if(type == 0)
          {
            if(encoders[i].axis == 'x')
              Mouse.move(1,0,0); // Right
            else
              Mouse.move(0,1,0);
          }
          else if(type == 1)
          {
            Keyboard.release(encoders[i].minus);
            Keyboard.press(encoders[i].plus);
          }
        }
        else if(enc_position_tr3[i]>=enc_position_tr2[i] && enc_position_tr2[i]>=enc_position_tr1[i] && enc_position_tr1[i]>encR) 
        {
          if(type == 0)
          {
            if(encoders[i].axis == 'x')
              Mouse.move(-1,0,0);
            else
              Mouse.move(0,-1,0);
          }
          else if(type == 1)
          {
            Keyboard.release(encoders[i].plus);
            Keyboard.press(encoders[i].minus);
          }
        }
        else
        {
          if(type == 1)
          {
            Keyboard.release(encoders[i].plus);
            Keyboard.release(encoders[i].minus);
          }
        }
 
        enc_position_tr3[i]=enc_position_tr2[i];
        enc_position_tr2[i]=enc_position_tr1[i];
        enc_position_tr1[i]=encR;
      }
    }
    if(digitalRead(7) == HIGH && digitalRead(8) == HIGH && digitalRead(11) == HIGH)
    {
      changeType();
    }
  }
}
void changeType()
{
  if(type == 1)
    type = 0;
  else
    type = 1;

}
