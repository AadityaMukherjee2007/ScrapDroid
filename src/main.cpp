#include <Arduino.h>

// Motor A
int in1 = 25;
int in2 = 26;

// Motor B
int in3 = 32;
int in4 = 33;

// put function declarations here:
void forward();
void backward();
void forward_right();
void forward_left();
void backward_right();
void backward_left();
void stop();

void setup()
{
  // put your setup code here, to run once:
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  Serial.begin(115200);

  // delay(3000);
  // forward();
  // delay(2000);
  // stop();
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0)
  {
    String data = Serial.readStringUntil('\n');
    data.trim();
    // Serial.println(data);

    // Forward & Backward
    if (data == "f")
    {
      forward();
    }
    else if (data == "b")
    {
      backward();
    }
    // Forward Turns
    else if (data == "fr")
    {
      forward_right();
    }
    else if (data == "fl")
    {
      forward_left();
    }
    // Backward Turns
    else if (data == "br")
    {
      backward_right();
    }
    else if (data == "bl")
    {
      backward_left();
    }
    // Full Stop
    else if (data == "s")
    {
      stop();
    }
    // Catch-all for unrecognized commands
    else
    {
      Serial.println("Invalid command. Use: f, b, fr, fl, br, bl, s");
    }
  }
}

// put function definitions here:
void forward()
{
  stop();
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void backward()
{
  stop();
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void forward_left()
{
  stop();
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void forward_right()
{
  stop();
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void backward_left()
{
  stop();
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void backward_right()
{
  stop();
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void stop()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}