const int VOL_PIN_0 = A0;
const int VOL_PIN_1 = A1;
const int VOL_PIN_2 = A2;

int value_0;
int value_1;
int value_2;

float volt_0;
float volt_1;
float volt_2;

static float forces[3] = {0, 0, 0};

// Reads and updates force array from sensors
void readForces() {
    // Read analog output from pins w/ sensors
    value_0 = analogRead( VOL_PIN_0 );
    value_1 = analogRead( VOL_PIN_1 );
    value_2 = analogRead( VOL_PIN_2 );

    // Convert analog reading to Voltage 0 - 3.3v
    volt_0 = (value_0 * (3.3 / 1023.0)) ;
    volt_1 = (value_1 * (3.3 / 1023.0)) ;
    volt_2 = (value_2 * (3.3 / 1023.0)) ;

    // Apply calibration curve to voltage reading to get force in newtons
    float sensorZeroForce = ((volt_0 / .1065) - 8.95) * 9.807; 
    float sensorOneForce = ((volt_1 / .1065) - 8.95) * 9.807; 
    float sensorTwoForce = ((volt_2 / .1065) - 8.95) * 9.807; 

    // Prints readings from sensor 0
    Serial.print( "         Sensor 0 Force (N): " );
    Serial.print( sensorZeroForce );

    // Prints readings from sensor 1
    Serial.print( "         Sensor 1 Force (N): " );
    Serial.print( sensorOneForce );

    // Prints readings from sensor 2
    Serial.print( "         Sensor 2 Force (N): " );
    Serial.println( sensorTwoForce );

    //Array of forces detected during loop
    forces[0] = sensorZeroForce;
    forces[1] = sensorOneForce;
    forces[2] = sensorTwoForce;

    delay( 500 );
}

// Actuates pump and valves
void actuatePump(){
  if (forces[0] > 10 || forces[1] > 10 || forces[2] > 10) {
    pumpItUp();
  }
  else {
    stall();
  }
}

// Turns on pump and opens valves to bags
void pumpItUp() {
  digitalWrite(12, HIGH);

  digitalWrite(11, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(8, LOW);
  delay( 500 );
}

// Holds pressure in bags and turns off pump
void stall() {
  digitalWrite(12, LOW);

  digitalWrite(11, LOW);
  digitalWrite(10, LOW);
  digitalWrite(9, LOW);
  digitalWrite(8, HIGH);
  delay( 500 );
}

void setup(){
    Serial.begin( 9600 );
    pinMode(12, OUTPUT); //Pump
    pinMode(11, OUTPUT); //Solenoid Valve 1
    pinMode(10, OUTPUT); //Solenoid Valve 2
    pinMode(9, OUTPUT); //Solenoid Valve 3
    pinMode(8, OUTPUT); //Solenoid Valve 4
}

void loop(){
  readForces();
  actuatePump();
}
