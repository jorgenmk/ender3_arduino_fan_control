
const byte OC1A_PIN = 9;  // Pin D9, connect to Noctua pwm in
const byte PWM_IN_PIN = 5;  // Pin D5, connect to ender3 fan negative
const word PWM_FREQ_HZ = 25000;
const word TCNT1_TOP = 16000000/(2*PWM_FREQ_HZ);

unsigned long pwm_low;
unsigned long pwm_high;
int pin_in;
float duty_cycle;

void setup() {
  // Make pwm output 25kHz
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  TCCR1A |= (1 << COM1A1)   | (1 << WGM11);
  TCCR1B |= (1 << WGM13) | (1 << CS10);
  ICR1 = TCNT1_TOP;

  Serial.begin(115200);
  Serial.println("Starting");
  pinMode(OC1A_PIN, OUTPUT);
  pinMode(PWM_IN_PIN, INPUT_PULLUP);
}

void loop() {
  delay(100);
  pwm_low = pulseIn(PWM_IN_PIN, HIGH, 1000000);
  pwm_high = pulseIn(PWM_IN_PIN, LOW, 1000000);
  pin_in = digitalRead(PWM_IN_PIN);

  Serial.print("high: ");
  Serial.print(pwm_high);
  Serial.print(" low: ");
  Serial.print(pwm_low);
  Serial.print(" ip: ");
  Serial.print(pin_in);
  Serial.print("\n");

  // At 100% and 0% duty cycle, there will be no pulses
  if ((pwm_high == 0) && (pin_in == LOW)) {
    Serial.print("duty_cycle: 100\n");
    set_pwm_duty_cycle(100);
    return;
  }

  if ((pwm_high == 0) && (pin_in == HIGH)) {
    Serial.print("duty_cycle: 0\n");
    set_pwm_duty_cycle(0);
    return;
  }
  
  duty_cycle = (float)pwm_high / (float)(pwm_low + pwm_high);
  set_pwm_duty_cycle((int)(duty_cycle*100));
  Serial.print("duty_cycle: ");
  Serial.print(duty_cycle);
  Serial.print("\n");
  
}

void set_pwm_duty_cycle(byte duty) {
  OCR1A = (word) (duty*TCNT1_TOP)/100;
}
