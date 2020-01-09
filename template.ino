#include "SerialBuffer.hpp"
/*** EMBGEN_INCLUDES ***/

SerialBuffer<64> buffer(&Serial);
/*** EMBGEN_VARIABLES: myMessenger(&buffer) ***/

void setup()
{
    Serial.begin(9600);
    /*** EMBGEN_SETUP ***/
}

void loop()
{
    /*** EMBGEN_LOOP ***/
}

/*** EMBGEN_COMMANDS ***/