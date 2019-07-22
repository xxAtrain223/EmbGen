#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <tinyxml2.h>
#include "EmbGen/EmbGen.hpp"
#include "EmbGen/Parser/Appendage.hpp"
#include "EmbGen/Exceptions.hpp"

using namespace testing;

namespace emb
{
    namespace gen
    {
        namespace test
        {
            TEST(embgen_EmbGen, no_appendages)
            {
                std::string config = "[]";
                std::string inoTemplate =
                    "#include <EmbMessenger/SerialBuffer.hpp>\n"
                    "/*** EMBGEN_INCLUDES ***/\n"
                    "\n"
                    "SerialBuffer<64> buffer(&Serial);\n"
                    "/*** EMBGEN_VARIABLES: myMessenger(&buffer, millis) ***/\n"
                    "\n"
                    "void setup()\n"
                    "{\n"
                    "    /*** EMBGEN_REGISTER: registerCommand(); ***/\n"
                    "    /*** EMBGEN_SETUP ***/\n"
                    "}\n"
                    "\n"
                    "void loop()\n"
                    "{\n"
                    "    /*** EMBGEN_LOOP ***/\n"
                    "}\n"
                    "\n"
                    "/*** EMBGEN_COMMANDS ***/\n";
                EmbGen embGen(config, "../data/appendages", inoTemplate);

                ASSERT_EQ(embGen.generateSource(),
                    "#include <EmbMessenger/SerialBuffer.hpp>\n"
                    "#include <EmbMessenger/EmbMessenger.hpp>\n"
                    "\n"
                    "SerialBuffer<64> buffer(&Serial);\n"
                    "emb::device::EmbMessenger<1u, 0> myMessenger(&buffer, millis);\n"
                    "\n"
                    "void setup()\n"
                    "{\n"
                    "    myMessenger.registerCommand(0u, all_stop);\n"
                    "\n"
                    "}\n"
                    "\n"
                    "void loop()\n"
                    "{\n"
                    "    myMessenger.update();\n"
                    "}\n"
                    "\n"
                    "void all_stop()\n"
                    "{\n"
                    "}\n"
                    "\n"
                );
            }

            TEST(embgen_EmbGen, one_servo)
            {
                std::string config =
                    R"([)"
                    R"(    {)"
                    R"(        "label": "servo_alpha",)"
                    R"(        "type": "Servo",)"
                    R"(        "pin": 10,)"
                    R"(        "init_value": 60)"
                    R"(    },)"
                    R"(    {)"
                    R"(        "label": "servo_bravo",)"
                    R"(        "type": "Servo",)"
                    R"(        "pin": 11,)"
                    R"(        "init_value": 120)"
                    R"(    })"
                    R"(])";
                std::string inoTemplate =
                    "#include <EmbMessenger/SerialBuffer.hpp>\n"
                    "/*** EMBGEN_INCLUDES ***/\n"
                    "\n"
                    "SerialBuffer<64> buffer(&Serial);\n"
                    "/*** EMBGEN_VARIABLES: myMessenger(&buffer, millis) ***/\n"
                    "\n"
                    "void setup()\n"
                    "{\n"
                    "    /*** EMBGEN_REGISTER: registerCommand(); ***/\n"
                    "    /*** EMBGEN_SETUP ***/\n"
                    "}\n"
                    "\n"
                    "void loop()\n"
                    "{\n"
                    "    /*** EMBGEN_LOOP ***/\n"
                    "}\n"
                    "\n"
                    "/*** EMBGEN_COMMANDS ***/\n";
                EmbGen embGen(config, "../data/appendages", inoTemplate);

                ASSERT_EQ(embGen.generateSource(),
                    "#include <EmbMessenger/SerialBuffer.hpp>\n"
                    "#include <EmbMessenger/EmbMessenger.hpp>\n"
                    "\n"
                    "#include \"Servo.h\"\n"
                    "\n"
                    "SerialBuffer<64> buffer(&Serial);\n"
                    "emb::device::EmbMessenger<5u, 0> myMessenger(&buffer, millis);\n"
                    "\n"
                    "Servo Servo_servo[2] = {\n"
                    "    Servo(),\n"
                    "    Servo()\n"
                    "};\n"
                    "uint8_t Servo_pin[2] = {\n"
                    "    10,\n"
                    "    11\n"
                    "};\n"
                    "uint8_t Servo_init_value[2] = {\n"
                    "    60,\n"
                    "    120\n"
                    "};\n"
                    "\n"
                    "void setup()\n"
                    "{\n"
                    "    myMessenger.registerCommand(0u, Servo_detach_adaptor);\n"
                    "    myMessenger.registerCommand(1u, Servo_get_adaptor);\n"
                    "    myMessenger.registerCommand(2u, Servo_set_adaptor);\n"
                    "    myMessenger.registerCommand(3u, Servo_stop);\n"
                    "    myMessenger.registerCommand(4u, all_stop);\n"
                    "    for (uint16_t i = 0; i <= 1u; ++i) {\n"
                    "        Servo_servo[i].attach(Servo_pin[i]); // Use the variable name defined above\n"
                    "        Servo_servo[i].write(Servo_init_value[i]);\n"
                    "    }\n"
                    "    delay(500);\n"
                    "    for (uint16_t i = 0; i <= 1u; ++i) {\n"
                    "        Servo_servo[i].detach();\n"
                    "    }\n"
                    "}\n"
                    "\n"
                    "void loop()\n"
                    "{\n"
                    "    myMessenger.update();\n"
                    "}\n"
                    "\n"
                    "void Servo_detach_adaptor()\n"
                    "{\n"
                    "    uint16_t i;\n"
                    "    myMessenger.read(\n"
                    "        i, [](uint16_t i){ return i <= 1u; }\n"
                    "    );\n"
                    "    Servo_detach(\n"
                    "        i\n"
                    "    );\n"
                    "}\n"
                    "\n"
                    "void Servo_detach(uint16_t i)\n"
                    "{\n"
                    "    Servo_servo[i].detach();\n"
                    "}\n"
                    "\n"
                    "void Servo_get_adaptor()\n"
                    "{\n"
                    "    uint16_t i;\n"
                    "    uint8_t value;\n"
                    "    myMessenger.read(\n"
                    "        i, [](uint16_t i){ return i <= 1u; }\n"
                    "    );\n"
                    "    Servo_get(\n"
                    "        i,\n"
                    "        value\n"
                    "    );\n"
                    "    myMessenger.write(\n"
                    "        value\n"
                    "    );\n"
                    "}\n"
                    "\n"
                    "void Servo_get(uint16_t i, uint8_t& value)\n"
                    "{\n"
                    "    value = Servo_servo[i].read();\n"
                    "}\n"
                    "\n"
                    "void Servo_set_adaptor()\n"
                    "{\n"
                    "    uint16_t i;\n"
                    "    uint8_t value;\n"
                    "    myMessenger.read(\n"
                    "        i, [](uint16_t i){ return i <= 1u; },\n"
                    "        value, [](uint8_t value){ return value <= 180; }\n"
                    "    );\n"
                    "    Servo_set(\n"
                    "        i,\n"
                    "        value\n"
                    "    );\n"
                    "}\n"
                    "\n"
                    "void Servo_set(uint16_t i, uint8_t value)\n"
                    "{\n"
                    "    if (!Servo_servo[i].attached()) {\n"
                    "        Servo_servo[i].attach(Servo_pin[i]);\n"
                    "    }\n"
                    "    Servo_servo[i].write(value);\n"
                    "}\n"
                    "\n"
                    "void Servo_stop()\n"
                    "{\n"
                    "    for (uint16_t i = 0; i <= 1u; ++i)\n"
                    "    {\n"
                    "        Servo_detach(i);\n"
                    "    }\n"
                    "}\n"
                    "\n"
                    "void all_stop()\n"
                    "{\n"
                    "    Servo_stop();\n"
                    "}\n"
                    "\n"
                );
            }
        }
    }
}