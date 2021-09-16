// this piece of code rotates both motors after taking 
// input values from serial monitor
// direction and rpm of the rotation is given through single data

int val = 150;

void setup() {
  pinMode(5, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(11, OUTPUT);

  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);

  digitalWrite(10, LOW);
  digitalWrite(11, LOW);

  Serial.begin(9600);
}

void loop() {

  if (Serial.available() > 0)
  {
    val = Serial.parseInt();
    if (val < 0)
    {
      digitalWrite(10, HIGH);
      digitalWrite(11, HIGH);
      analogWrite(5, val + 255);
      analogWrite(6, val + 255);
    }

    else if (val > 0)
    {
      digitalWrite(10, LOW);
      digitalWrite(11, LOW);
      analogWrite(5, val);
      analogWrite(6, val);
    }

  }

  Serial.println(val);
  delay(10);
}
