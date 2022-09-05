//link for TinkerCAD project with wiring already done: 
//https://www.tinkercad.com/things/3rvEBeJtwjs-interrupt-7-seg/editel?sharecode=dcgKCXlS6gKDj3L1YP8PupkpBMwxjCBrDhsh5hUepRA

#define TOTAL_DIGITS (3)

typedef struct {
  char nbr;
  char val;
  uint8_t ticks;
  bool isActive;
} Digit;

Digit digits[TOTAL_DIGITS];

//segment for the digits
static uint8_t num[10] = {
  //GFEDCBA
  B00111111,
  B00000110,
  B01011011,
  B01001111,
  B01100110,
  B01101101,
  B01111101,
  B00000111,
  B01111111,
  B01101111
};

int idx = 0;

static void inline handle
  (volatile uint8_t *TCNTn, volatile uint8_t* OCRnA){
  if (idx == -1){ //global disable mode
    *TCNTn = 0;
  } else {
  
    digitalWrite(digits[idx].nbr, HIGH);
    idx = (idx + 1) % TOTAL_DIGITS;
    if (digits[idx].isActive){
      PORTD = 0;
      PORTD = num[digits[idx].val];
      digitalWrite(digits[idx].nbr, LOW);
    }
    *OCRnA = (*TCNTn + digits[idx].ticks) % 256;
  }
}

ISR(TIMER2_COMPA_vect){
  handle(&TCNT2, &OCR2A); //allows code reuse
}

void setup(){
  //pinmodes, using pin 0-7 as segments, 11-13 as cathode
  cli();
  
  DDRD = B11111111;
  DDRB = B00111111;
  //cathode number, displayed value, display length, is active
  digits[0] = {13, 5, 500, true};
  digits[1] = {12, 3, 500, false};
  digits[2] = {11, 4, 500, true};
  //timers
  TCCR2A = 0;//normal counting mode
  TCCR2B = (1 << CS21);//8 prescaler
  TCNT2 = 0;//initialize counter value to 0
  OCR2A = 10; //default value
  TIMSK2 |= (1 << OCIE2A); //allow interrupts to happen
  digitalWrite(11, 0);
  digitalWrite(12, 0);
  digitalWrite(13, 0);
  
  sei();
}

int value = 0;

void loop(){
  //do things here
}
