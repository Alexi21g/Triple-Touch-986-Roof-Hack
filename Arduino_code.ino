/*
Alexi's take on the OneTouch roof control unit.

Code to fully open or close a 986 Boxster's roof when the roof button is pressed three times in quick succession (within 800ms). 
A single button press during operation will stop the roof (so you can still get the roof into service mode}.

This contrasts with the ‘one touch’ approach, which is more convenient, but leaves a risk of accidentally closing the roof at high speed 
if you’ve also taken the steps to bypass the handbrake/no speed requirements.
 */


int roof_open = 0;                  // current state of the button
int roof_close = 0;
int last_roof_open_state = 0;       // previous state of the button
int last_roof_close_state = 0;
int oneTouch_open_count = 1;
int oneTouch_close_count = 1;
int o_start_time;
int c_start_time;
int current_time;
bool oneTouch_open;
bool oneTouch_close;
int operating_period = 15000;      // time taken for the roof to fully open or close

int out_pin_open = 8;
int out_pin_close = 7;
int in_pin_open = 6;
int in_pin_close = 5;

void setup()
{
  // declare the output pins
  pinMode(out_pin_open, OUTPUT); //Open//
  pinMode(out_pin_close, OUTPUT); //Close//

  // declare the input pins
  pinMode(in_pin_open, INPUT); //Open//
  pinMode(5, INPUT); //Close//
}

void loop()
{
  // Read the value of the switch
  roof_open = digitalRead(in_pin_open);
  roof_close = digitalRead(in_pin_close);
  current_time = millis();

  if (roof_open != last_roof_open_state)
  {
    Update_button_o();
  }
  else if (roof_close != last_roof_close_state)
  {
    Update_button_c();
  }

  if (oneTouch_open == true)
  {
    Run_OneTouch_open();
  }
  else if (oneTouch_close == true)
  {
    Run_OneTouch_close();
  }
  else
  {
    if (oneTouch_open_count > 1 || oneTouch_close_count > 1)
    {
      if ((current_time - o_start_time) > 800)
      {
        oneTouch_open_count = 1;
      }
      else if ((current_time - c_start_time) > 800)
      {
        oneTouch_close_count = 1;
      }
    }

    //Timer starts on first button press.
    if (oneTouch_open_count == 1)
    {
      o_start_time = millis();
    }

    if (oneTouch_close_count == 1)
    {
      c_start_time = millis();
    }

    //OneTouch operation
    if (oneTouch_open_count == 4)
    {
      if ((current_time - o_start_time) > 800)
      {
        oneTouch_close_count = 1;
        oneTouch_open_count = 1;
      }
      else
      {
        oneTouch_close_count = 1;
        oneTouch_open = true;
      }
    }
    else if (oneTouch_close_count == 4)
    {
      if ((current_time - c_start_time) > 800)
      {
        oneTouch_open_count = 1;
        oneTouch_close_count = 1;
      }
      else
      {
        oneTouch_open_count = 1;
        oneTouch_close = true;
      }
    }
  }

  //Save roof state to check for switch operation on next cycle
  last_roof_open_state = roof_open;
  last_roof_close_state = roof_close;
}

//FUNCTIONS//

void Update_button_o()
{
  delay (90);
  if (roof_open == LOW)
  {
    digitalWrite(7, LOW);
    oneTouch_open_count++;
  }
  else
  {
    digitalWrite(out_pin_open, LOW);
  }
}

void Update_button_c()
{
  delay (90);
  if (roof_close == LOW)
  {
    digitalWrite(out_pin_open, LOW);
    oneTouch_close_count++;
  }
  else
  {
    digitalWrite(out_pin_close, LOW);
  }
}

void Run_OneTouch_open()
{
  if (current_time - o_start_time > operating_period || oneTouch_close_count > 1 || oneTouch_open_count == 5)
  {
    oneTouch_open_count = 1;
    oneTouch_close_count = 1;
    oneTouch_open = false;
    digitalWrite(out_pin_open, LOW);
  }
  else
  {
    digitalWrite(out_pin_open, HIGH);
  }
}

void Run_OneTouch_close()
{
  if (current_time - c_start_time > operating_period || oneTouch_open_count > 1 || oneTouch_close_count == 5)
  {
    oneTouch_open_count = 1;
    oneTouch_close_count = 1;
    oneTouch_close = false;
    digitalWrite(7, LOW);
  }
  else
  {
    digitalWrite(out_pin_close, HIGH);
  }
}
