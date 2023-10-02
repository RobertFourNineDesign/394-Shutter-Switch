//*********FOUR NINE SHUTTER SWITCH*********
//******************************************
// COPYRIGHT ROBERT KNUTSON AT FOUR NINE DESIGN 09.22.23
// ROBERT@FOURNINEPRODUCTS.COM

// This program uses an electro magnet to move an aluminum bar that either blocks, or doesnt block a viewport on a customers cryostat.


// Phase low = forward,
// Phase hiigh = reverse
// PWM to enable determines speed

int EnablePin = 9;    // Pin 9 controlls the magnet speed
int PhasePin = 7;     // Pin 7 controlls the magnet direction
int LeftSwitch = 4;   // Pin 4 is the is connected to the Left override switch
int RightSwitch = 5;  // Pin 5 is connected to the right override switch
int SignalPin = 11;   // TTL Logic input pin
int LEDPin = 2;       // LED pin

byte PWMval = 255;  // The PWM duty cycle of the enable pin, BYTE, 0=Always OFF, 256=Always on
// Note: The motor dirver board needs to see a PWM signal, will not respond with duty cycle=256

bool PinState;             // Variable to store the current input button state
bool PrevousPinState = 1;  // Variable to store the prevous input button state

bool LeftSwitchState;
bool PrevousLeftSwitchState = 0;

bool RightSwitchState;
bool PreviousRightSwitchState = 0;


// variables to controll the time the switch is on
const int OnTime = 500;  // Time the selenoid should remain active, milliseconds

unsigned long BeginingMillis = 0;  // Store the time the switch was pressed

bool RunOnce = 0;  // Switch to ensure the starting time is only recorded once
bool PrevousRunOnce = 1;

// Variables to controll input Switching
bool InputFlag = 0;

// Variable to store current switch position
bool SwitchPosition = 0;  // 0 = Left, 1 = Right

bool IsMoving = 0;  // Variable to store if the switch is currently moving from one side to another


void setup() {
  // Begin Serial Port:
  Serial.begin(9600);
  delay(500);

  // Define Inputs and Outputs
  pinMode(LeftSwitch, INPUT_PULLUP);
  pinMode(RightSwitch, INPUT_PULLUP);
  pinMode(SignalPin, INPUT_PULLUP);
  pinMode(EnablePin, OUTPUT);
  pinMode(PhasePin, OUTPUT);
  pinMode(LEDPin, OUTPUT);

  // When the system is first turned on the window state is indeterminate
  // Close shutter so the current state is known
  digitalWrite(PhasePin, HIGH);
  analogWrite(EnablePin, PWMval);
  delay(OnTime);
  digitalWrite(PhasePin, LOW);
  analogWrite(EnablePin, 0);
  Serial.println("Setup Complete");

  digitalWrite(LEDPin,HIGH);
}

void loop() {
  // Read the state of the button pin at the begining of every cycle
  PinState = digitalRead(SignalPin);
  LeftSwitchState = digitalRead(LeftSwitch);
  RightSwitchState = digitalRead(RightSwitch);

  unsigned long currentMillis = millis();  // Save the current time
                                           //Serial.println(PinState);

  // If the switch state changes, execute tasks to begin moving shutter
  //if (PrevousPinState != PinState) {
  if ((PinState == 1 && PrevousPinState == 0) || LeftSwitchState == 0) {
    InputFlag = 1;
    digitalWrite(LEDPin,HIGH);
    SwitchPosition = 0;
    //Serial.println(digitalRead(PinState));
    if (RunOnce == 0) {
      BeginingMillis = millis();  // Store the time the switch was intiailly pressed

      Serial.print("Initial Time is ");
      Serial.println(BeginingMillis);
      Serial.println("");
      Serial.print("RunOnce Flag state is ");
      Serial.println(RunOnce);
      Serial.println("");
      Serial.print("Input Flag is ");
      Serial.println(InputFlag);
      Serial.println("");
      RunOnce = 1;  // This is to make sure the initial time is only saved once
    }
    PrevousPinState = PinState;
  } 
  else if ((PinState == 0 && PrevousPinState == 1) || RightSwitchState == 0) {
    InputFlag = 1;
    digitalWrite(LEDPin,LOW);
    SwitchPosition = 1;
    //Serial.println(digitalRead(PinState));
    if (RunOnce == 0) {
      BeginingMillis = millis();  // Store the time the switch was intiailly pressed

      Serial.print("Initial Time is ");
      Serial.println(BeginingMillis);
      Serial.println("");
      Serial.print("RunOnce Flag state is ");
      Serial.println(RunOnce);
      Serial.println("");
      Serial.print("Input Flag is ");
      Serial.println(InputFlag);
      Serial.println("");
      RunOnce = 1;  // This is to make sure the initial time is only saved once
    }

    PrevousPinState = PinState;
  }

  if (InputFlag == 1 && (currentMillis - BeginingMillis <= OnTime)) {
    //Serial.println("This Worked");
    if (IsMoving == 0) {
      Serial.println("Were Moving");
      if (SwitchPosition == 0) {
        digitalWrite(PhasePin, LOW);
        analogWrite(EnablePin, PWMval);
        Serial.println("Moving Right");
      } else if (SwitchPosition == 1) {
        digitalWrite(PhasePin, HIGH);
        analogWrite(EnablePin, PWMval);
        Serial.println("Moving Left");
      }
      IsMoving = 1;
    }

  } else if (InputFlag == 1 && (currentMillis - BeginingMillis >= OnTime)) {
    //SwitchPosition = !SwitchPosition;
    analogWrite(EnablePin, 0);
    InputFlag = 0;
    IsMoving = 0;
    RunOnce = 0;
    Serial.println("Move Done");
  }
}
