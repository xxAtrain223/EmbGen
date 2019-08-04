#include <EmbMessenger/SerialBuffer.hpp>
/*** EMBGEN_INCLUDES ***/

SerialBuffer<64> buffer(&Serial);
/*** EMBGEN_VARIABLES: myMessenger(&buffer, millis) ***/

void setup()
{
    if (!registerCommands())
    {
        pinMode(LED_BUILTIN, OUTPUT);
        while (true)
        {
            digitalWrite(LED_BUILTIN, HIGH);
            delay(250);
            digitalWrite(LED_BUILTIN, LOW);
            delay(250);
        }
    }
    
    /*** EMBGEN_SETUP ***/
}

void loop()
{
    /*** EMBGEN_LOOP ***/
}

bool registerCommands()
{
    bool success = true;
    /*** EMBGEN_REGISTER: success &= registerCommand(); ***/
    return success;
}

/*** EMBGEN_COMMANDS ***/