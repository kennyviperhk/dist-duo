/*  Serial Decode

  0: 0 CW 1 ACW
  1: val 0-255
  2: duration 1000 = 1s
*/

String inString = "";    // string to hold input char
String inString_buffer = "";    // string to hold input Serial msg
String inString_display_buffer = "";    // string to hold Display info

bool exclude_print_val = true;


int current_index = 0;//index of current decoding number
//long input_value[Input_size];  //inputs value(s) buffer -> moved to main tab
boolean update_flag = false; //Flag for end of success input string follow /n

void serial_decode()   // Read serial input: //TODO add serial1
{
  byte Read_Flag = 0;
  while (Serial.available() > 0)
  {
    int inChar = Serial.read();
    char_decode(inChar) ;
  }

  /*
    while (Serial1.available() > 0)
    {
      int inChar = Serial1.read();
      char_decode(inChar) ;
    }
  */
}




void char_decode(int inChar)
{
  // Read_Flag=0;
  inString_buffer += (char)inChar;
  if (isDigit(inChar))       //if get a number
  {
    // convert the incoming byte to a char
    // and add it to the string:
    inString += (char)inChar;
    //Serial.println(inString);
  }
  else if (inChar == '-')       // if you get a comma, convert to a number,
  {
    if (current_index < Input_size) // within size
    {
      input_value[current_index] = inString.toInt(); //

      //  pdInput_value[current_index] = inString.toInt(); //
      current_index++;  //increase index
    }




    //            else// giveup the value to avoide over access array
    //            {
    //              Serial.println("overflow");
    //            }
    inString = "";   // clear the string buffer for new input:


  }
  else if (inChar == '\n' || inChar == 'N')       //if you get a newline, convert to a number and print all value
  {
    if (current_index < Input_size) // within size
    {
      input_value[current_index] = inString.toInt(); //final value

    }
    //            else// giveup the value to avoide over access array
    //            {
    //              Serial.println("overflow(end)");
    //            }
setSpeedController();

    inString = "";   // clear the string buffer for new input:
    current_index = 0;// reset the index
    //Set update_Flag
    update_flag = true;

  }
  else if (inChar == 'C') //Check if online
  {
    //DO STH HERE
    inString = "";   // clear the string buffer for new input:

    clockwise = !clockwise;

    Serial.print("change direction");
    if (clockwise) {
      Serial.print("CW");
    } else {
      Serial.print("AWC");
    }
    update_flag = true;
    exclude_print_val = true;
  }
  else if (inChar == 'E') //Check if online
  {
    //DO STH HERE
    inString = "";   // clear the string buffer for new input:

    enableMotor = !enableMotor;

    Serial.print("Enable / Disable : ");
    if (enableMotor) {
      Serial.print("Enable M");
    } else {
      Serial.print("Disable M");
    }


    update_flag = true;
    exclude_print_val = true;
  }
  else if (inChar == 'P') //Check if online
  {
    //DO STH HERE
    inString = "";   // clear the string buffer for new input:
    long pulse = pulseIn(motorHzPin,HIGH);
    Serial.println(pulse/3);
    update_flag = true;
    exclude_print_val = true;
  }



}


void check_update()   //Check update flag and write value when string finish
{
  if ( update_flag == true) //update value
  {

    if (exclude_print_val == false) {
      //print all value
      Serial.print("hz\t");
      for (int index = 0; index < Input_size; index++)
      {

        Serial.print(input_value[index]);
        Serial.print("\t");

      }

    }
    //Finish Serial.print, Next Line
    Serial.println("|");

    exclude_print_val = false;
    update_flag = false; //clear flag
    inString_display_buffer = inString_buffer;    // string to hold input Serial msg
    inString_buffer = "";   // clear the string buffer for new input:
  }


}

