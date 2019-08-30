#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <tinyxml2.h>
#include "EmbGen/Appendage.hpp"
#include "EmbGen/Parser/Appendage.hpp"
#include "EmbGen/Exceptions.hpp"

using namespace testing;

namespace emb
{
    namespace gen
    {
        namespace test
        {
            TEST(embgen_Appendage, Includes)
            {
                tinyxml2::XMLDocument tinyDocument;
                ASSERT_EQ(tinyDocument.Parse(
                    "<appendage name='Include'>\n"
                    "    <include standard='true'>Alpha.hpp</include>\n"
                    "    <include standard='false'>Bravo.hpp</include>\n"
                    "</appendage>\n"
                ), tinyxml2::XMLError::XML_SUCCESS);
                tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("appendage");
                ASSERT_NE(tinyElement, nullptr);

                std::shared_ptr<parser::Appendage> xml = std::make_shared<parser::Appendage>(tinyElement);

                ASSERT_EQ(xml->getName(), "Include");
                ASSERT_EQ(xml->getVersion(), "");
                ASSERT_EQ(xml->getLibDeps(), "");
                ASSERT_EQ(xml->getIncludes().size(), 2);
                ASSERT_EQ(xml->getVariables().size(), 0);
                ASSERT_EQ(xml->getSetup(), nullptr);
                ASSERT_EQ(xml->getLoop(), nullptr);
                ASSERT_EQ(xml->getCommands().size(), 0);
                ASSERT_EQ(xml->getStop(), nullptr);

                nlohmann::json json = nlohmann::json::parse(
                    R"({)"
                    R"(    "Include": {)"
                    R"(        "include_alpha": {)"
                    R"(            "typeIndex": 0)"
                    R"(        },)"
                    R"(        "include_bravo": {)"
                    R"(            "typeIndex": 1)"
                    R"(        })"
                    R"(    })"
                    R"(})"
                );

                nlohmann::json includes = json["Include"];

                std::string str = includes.dump();

                Appendage appendage(xml, json);

                ASSERT_EQ(
                    appendage.getIncludes(),
                    "#include <Alpha.hpp>\n"
                    "#include \"Bravo.hpp\"\n"
                );
            }


            TEST(embgen_Appendage, Variables)
            {
                tinyxml2::XMLDocument tinyDocument;
                ASSERT_EQ(tinyDocument.Parse(
                    "<appendage name='Variable'>\n"
                    "    <variable type='uint8_t' name='val1' />\n"
                    "    <variable type='int16_t' name='val2' core='true' />\n"
                    "    <variable type='Foo' name='foo' core='false'>\n"
                    "        <parameter type='uint32_t' name='val3' />\n"
                    "        <parameter type='float' name='val4' />\n"
                    "        <parameter type='String' name='val5' />\n"
                    "    </variable>\n"
                    "    <variable type='Bar' name='bar' />\n"
                    "</appendage>\n"
                ), tinyxml2::XMLError::XML_SUCCESS);
                tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("appendage");
                ASSERT_NE(tinyElement, nullptr);

                std::shared_ptr<parser::Appendage> xml = std::make_shared<parser::Appendage>(tinyElement);

                ASSERT_EQ(xml->getName(), "Variable");
                ASSERT_EQ(xml->getVersion(), "");
                ASSERT_EQ(xml->getLibDeps(), "");
                ASSERT_EQ(xml->getIncludes().size(), 0);
                ASSERT_EQ(xml->getVariables().size(), 4);
                ASSERT_EQ(xml->getSetup(), nullptr);
                ASSERT_EQ(xml->getLoop(), nullptr);
                ASSERT_EQ(xml->getCommands().size(), 0);
                ASSERT_EQ(xml->getStop(), nullptr);

                nlohmann::json json = nlohmann::json::parse(
                    R"({)"
                    R"(    "Variable": {)"
                    R"(        "variable_alpha": {)"
                    R"(            "val1": 255,)"
                    R"(            "val2": -512,)"
                    R"(            "val3": 123456789,)"
                    R"(            "val4": 3.14,)"
                    R"(            "val5": "alpha",)"
                    R"(            "typeIndex": 0)"
                    R"(        },)"
                    R"(        "variable_bravo": {)"
                    R"(            "val1": 127,)"
                    R"(            "val2": -1024,)"
                    R"(            "val3": 987654321,)"
                    R"(            "val4": 6.28,)"
                    R"(            "val5": "bravo",)"
                    R"(            "typeIndex": 1)"
                    R"(        })"
                    R"(    })"
                    R"(})"
                );

                Appendage appendage(xml, json);

                ASSERT_EQ(
                    appendage.getVariables(),
                    "uint8_t Variable_val1[2] = {\n"
                    "    255,\n"
                    "    127\n"
                    "};\n"
                    "int16_t Variable_val2[2] = {\n"
                    "    -512,\n"
                    "    -1024\n"
                    "};\n"
                    "Foo Variable_foo[2] = {\n"
                    "    Foo(\n"
                    "        123456789,\n"
                    "        3.14f,\n"
                    "        \"alpha\"\n"
                    "    ),\n"
                    "    Foo(\n"
                    "        987654321,\n"
                    "        6.28f,\n"
                    "        \"bravo\"\n"
                    "    )\n"
                    "};\n"
                    "Bar Variable_bar[2] = {\n"
                    "    Bar(),\n"
                    "    Bar()\n"
                    "};\n"
                );
            }

            TEST(embgen_Appendage, Setup)
            {
                tinyxml2::XMLDocument tinyDocument;
                ASSERT_EQ(tinyDocument.Parse(
                    "<appendage name='Servo'>\n"
                    "    <include>Servo.h</include>\n"
                    "    <variable type='Servo' name='servo' />\n"
                    "    <variable type='uint8_t' name='pin' />\n"
                    "    <variable type='uint8_t' name='init_value' />\n"
                    "    <setup>\n"
                    "        <code insert='each'>\n"
                    "            servo.attach(pin);\n"
                    "            servo.write(init_value);\n"
                    "        </code>\n"
                    "        <code insert='once'>\n"
                    "            delay(500);\n"
                    "        </code>\n"
                    "        <code insert='each'>\n"
                    "            servo.detach();\n"
                    "        </code>\n"
                    "    </setup>\n"
                    "</appendage>\n"
                ), tinyxml2::XMLError::XML_SUCCESS);
                tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("appendage");
                ASSERT_NE(tinyElement, nullptr);

                std::shared_ptr<parser::Appendage> xml = std::make_shared<parser::Appendage>(tinyElement);

                ASSERT_EQ(xml->getName(), "Servo");
                ASSERT_EQ(xml->getVersion(), "");
                ASSERT_EQ(xml->getLibDeps(), "");
                ASSERT_EQ(xml->getIncludes().size(), 1);
                ASSERT_EQ(xml->getVariables().size(), 3);
                ASSERT_NE(xml->getSetup(), nullptr);
                ASSERT_EQ(xml->getLoop(), nullptr);
                ASSERT_EQ(xml->getCommands().size(), 0);
                ASSERT_EQ(xml->getStop(), nullptr);

                nlohmann::json json = nlohmann::json::parse(
                    R"({)"
                    R"(    "Servo": {)"
                    R"(        "servo_alpha": {)"
                    R"(            "pin": 10,)"
                    R"(            "init_value": 60,)"
                    R"(            "typeIndex": 0)"
                    R"(        },)"
                    R"(        "servo_bravo": {)"
                    R"(            "pin": 11,)"
                    R"(            "init_value": 120,)"
                    R"(            "typeIndex": 1)"
                    R"(        })"
                    R"(    })"
                    R"(})"
                );

                Appendage appendage(xml, json);

                ASSERT_EQ(
                    appendage.getSetup(),
                    "    for (uint16_t i = 0u; i <= 1u; ++i) {\n"
                    "        Servo_servo[i].attach(Servo_pin[i]);\n"
                    "        Servo_servo[i].write(Servo_init_value[i]);\n"
                    "    }\n"
                    "    delay(500);\n"
                    "    for (uint16_t i = 0u; i <= 1u; ++i) {\n"
                    "        Servo_servo[i].detach();\n"
                    "    }\n"
                );
            }

            TEST(embgen_Appendage, Loop)
            {
                tinyxml2::XMLDocument tinyDocument;
                ASSERT_EQ(tinyDocument.Parse(
                    "<appendage name='Loop'>\n"
                    "    <variable type='uint8_t' name='durA' />\n"
                    "    <variable type='uint8_t' name='durB' />\n"
                    "    <loop>\n"
                    "        <code insert='once'>\n"
                    "            func(durA, durB);\n"
                    "        </code>\n"
                    "        <code insert='each'>\n"
                    "            func2(durA, durB);\n"
                    "        </code>\n"
                    "    </loop>\n"
                    "</appendage>\n"
                ), tinyxml2::XMLError::XML_SUCCESS);
                tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("appendage");
                ASSERT_NE(tinyElement, nullptr);

                std::shared_ptr<parser::Appendage> xml = std::make_shared<parser::Appendage>(tinyElement);

                ASSERT_EQ(xml->getName(), "Loop");
                ASSERT_EQ(xml->getVersion(), "");
                ASSERT_EQ(xml->getLibDeps(), "");
                ASSERT_EQ(xml->getIncludes().size(), 0);
                ASSERT_EQ(xml->getVariables().size(), 2);
                ASSERT_EQ(xml->getSetup(), nullptr);
                ASSERT_NE(xml->getLoop(), nullptr);
                ASSERT_EQ(xml->getCommands().size(), 0);
                ASSERT_EQ(xml->getStop(), nullptr);

                nlohmann::json json = nlohmann::json::parse(
                    R"({)"
                    R"(    "Loop": {)"
                    R"(        "loop_alpha": {)"
                    R"(            "durA": 7,)"
                    R"(            "durB": 42,)"
                    R"(            "typeIndex": 0)"
                    R"(        },)"
                    R"(        "loop_bravo": {)"
                    R"(            "durA": 30,)"
                    R"(            "durB": 6,)"
                    R"(            "typeIndex": 1)"
                    R"(        },)"
                    R"(        "loop_charlie": {)"
                    R"(            "durA": 50,)"
                    R"(            "durB": 2,)"
                    R"(            "typeIndex": 2)"
                    R"(        })"
                    R"(    })"
                    R"(})"
                );

                Appendage appendage(xml, json);

                ASSERT_EQ(
                    appendage.getLoop(),
                    "    func(Loop_durA, Loop_durB);\n"
                    "    for (uint16_t i = 0u; i <= 2u; ++i) {\n"
                    "        func2(Loop_durA[i], Loop_durB[i]);\n"
                    "    }\n"
                );
            }

            TEST(embgen_Appendage, CommandNames)
            {
                tinyxml2::XMLDocument tinyDocument;
                ASSERT_EQ(tinyDocument.Parse(
                    "<appendage name='Servo'>\n"
                    "    <include>Servo.h</include>\n"
                    "    <variable type='Servo' name='servo' />\n"
                    "    <variable type='uint8_t' name='pin' />\n"
                    "    <variable type='uint8_t' name='init_value' core='true' />\n"
                    "    <setup>\n"
                    "        <code insert='each'>\n"
                    "            servo.attach(pin); // Use the variable name defined above\n"
                    "            servo.write(init_value);\n"
                    "        </code>\n"
                    "        <code insert='once'>\n"
                    "            delay(500);\n"
                    "        </code>\n"
                    "        <code insert='each'>\n"
                    "            servo.detach();\n"
                    "        </code>\n"
                    "    </setup>\n"
                    "    <command name='set'>\n"
                    "        <parameter type='uint8_t' name='value' max='180' />\n"
                    "        <code>\n"
                    "            if (!servo.attached()) {\n"
                    "                servo.attach(pin);\n"
                    "            }\n"
                    "            servo.write(value);\n"
                    "        </code>\n"
                    "    </command>\n"
                    "    <command name='detach'>\n"
                    "        <code>\n"
                    "            servo.detach();\n"
                    "        </code>\n"
                    "    </command>\n"
                    "    <stop command='detach' />\n"
                    "</appendage>\n"
                ), tinyxml2::XMLError::XML_SUCCESS);
                tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("appendage");
                ASSERT_NE(tinyElement, nullptr);

                std::shared_ptr<parser::Appendage> xml = std::make_shared<parser::Appendage>(tinyElement);

                ASSERT_EQ(xml->getName(), "Servo");
                ASSERT_EQ(xml->getVersion(), "");
                ASSERT_EQ(xml->getLibDeps(), "");
                ASSERT_EQ(xml->getIncludes().size(), 1);
                ASSERT_EQ(xml->getVariables().size(), 3);
                ASSERT_NE(xml->getSetup(), nullptr);
                ASSERT_EQ(xml->getLoop(), nullptr);
                ASSERT_EQ(xml->getCommands().size(), 2);
                ASSERT_NE(xml->getStop(), nullptr);

                nlohmann::json json = nlohmann::json::parse(
                    R"({)"
                    R"(    "Servo": {)"
                    R"(        "servo_alpha": {)"
                    R"(            "pin": 10,)"
                    R"(            "init_value": 60,)"
                    R"(            "typeIndex": 0)"
                    R"(        },)"
                    R"(        "servo_bravo": {)"
                    R"(            "pin": 11,)"
                    R"(            "init_value": 120,)"
                    R"(            "typeIndex": 1)"
                    R"(        })"
                    R"(    })"
                    R"(})"
                );

                Appendage appendage(xml, json);

                ASSERT_THAT(
                    appendage.getCommandNames(),
                    ElementsAre("Servo_detach_adaptor", "Servo_set_adaptor", "Servo_stop"));
            }

            TEST(embgen_Appendage, CommandFunctions)
            {
                tinyxml2::XMLDocument tinyDocument;
                ASSERT_EQ(tinyDocument.Parse(
                    "<appendage name='Servo'>\n"
                    "    <include>Servo.h</include>\n"
                    "    <variable type='Servo' name='servo' />\n"
                    "    <variable type='uint8_t' name='pin' />\n"
                    "    <variable type='uint8_t' name='init_value' core='true' />\n"
                    "    <setup>\n"
                    "        <code insert='each'>\n"
                    "            servo.attach(pin); // Use the variable name defined above\n"
                    "            servo.write(init_value);\n"
                    "        </code>\n"
                    "        <code insert='once'>\n"
                    "            delay(500);\n"
                    "        </code>\n"
                    "        <code insert='each'>\n"
                    "            servo.detach();\n"
                    "        </code>\n"
                    "    </setup>\n"
                    "    <command name='set'>\n"
                    "        <parameter type='uint8_t' name='value' max='180' />\n"
                    "        <code>\n"
                    "            if (!servo.attached()) {\n"
                    "                servo.attach(pin);\n"
                    "            }\n"
                    "            servo.write(value);\n"
                    "        </code>\n"
                    "    </command>\n"
                    "    <command name='get'>\n"
                    "        <return_value type='uint8_t' name='value' />\n"
                    "        <code>\n"
                    "            value = servo.read();\n"
                    "        </code>\n"
                    "    </command>\n"
                    "    <command name='detach'>\n"
                    "        <code>\n"
                    "            servo.detach();\n"
                    "        </code>\n"
                    "    </command>\n"
                    "    <stop command='detach' />\n"
                    "</appendage>\n"
                ), tinyxml2::XMLError::XML_SUCCESS);
                tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("appendage");
                ASSERT_NE(tinyElement, nullptr);

                std::shared_ptr<parser::Appendage> xml = std::make_shared<parser::Appendage>(tinyElement);

                ASSERT_EQ(xml->getName(), "Servo");
                ASSERT_EQ(xml->getVersion(), "");
                ASSERT_EQ(xml->getLibDeps(), "");
                ASSERT_EQ(xml->getIncludes().size(), 1);
                ASSERT_EQ(xml->getVariables().size(), 3);
                ASSERT_NE(xml->getSetup(), nullptr);
                ASSERT_EQ(xml->getLoop(), nullptr);
                ASSERT_EQ(xml->getCommands().size(), 3);
                ASSERT_NE(xml->getStop(), nullptr);

                nlohmann::json json = nlohmann::json::parse(
                    R"({)"
                    R"(    "Servo": {)"
                    R"(        "servo_alpha": {)"
                    R"(            "pin": 10,)"
                    R"(            "init_value": 60,)"
                    R"(            "typeIndex": 0)"
                    R"(        },)"
                    R"(        "servo_bravo": {)"
                    R"(            "pin": 11,)"
                    R"(            "init_value": 120,)"
                    R"(            "typeIndex": 1)"
                    R"(        })"
                    R"(    })"
                    R"(})"
                );

                Appendage appendage(xml, json);

                ASSERT_EQ(
                    appendage.getCommandFunctions("messenger"),
                    "void Servo_detach_adaptor()\n"
                    "{\n"
                    "    uint16_t i;\n"
                    "    messenger.read(\n"
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
                    "    messenger.read(\n"
                    "        i, [](uint16_t i){ return i <= 1u; }\n"
                    "    );\n"
                    "    Servo_get(\n"
                    "        i,\n"
                    "        value\n"
                    "    );\n"
                    "    messenger.write(\n"
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
                    "    messenger.read(\n"
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
                    "    for (uint16_t i = 0u; i <= 1u; ++i) {\n"
                    "        Servo_detach(i);\n"
                    "    }\n"
                    "}\n"
                    "\n");
            }

            TEST(embgen_Appendage, StopCode)
            {
                tinyxml2::XMLDocument tinyDocument;
                ASSERT_EQ(tinyDocument.Parse(
                    "<appendage name='Servo'>\n"
                    "    <variable type='Servo' name='servo' />\n"
                    "    <stop>\n"
                    "        <code>\n"
                    "            servo.detach();\n"
                    "        </code>\n"
                    "    </stop>\n"
                    "</appendage>\n"
                ), tinyxml2::XMLError::XML_SUCCESS);
                tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("appendage");
                ASSERT_NE(tinyElement, nullptr);

                std::shared_ptr<parser::Appendage> xml = std::make_shared<parser::Appendage>(tinyElement);

                ASSERT_EQ(xml->getName(), "Servo");
                ASSERT_EQ(xml->getVersion(), "");
                ASSERT_EQ(xml->getLibDeps(), "");
                ASSERT_EQ(xml->getIncludes().size(), 0);
                ASSERT_EQ(xml->getVariables().size(), 1);
                ASSERT_EQ(xml->getSetup(), nullptr);
                ASSERT_EQ(xml->getLoop(), nullptr);
                ASSERT_EQ(xml->getCommands().size(), 0);
                ASSERT_NE(xml->getStop(), nullptr);

                nlohmann::json json = nlohmann::json::parse(
                    R"({)"
                    R"(    "Servo": {)"
                    R"(        "servo_alpha": {)"
                    R"(            "pin": 10,)"
                    R"(            "init_value": 60,)"
                    R"(            "typeIndex": 0)"
                    R"(        },)"
                    R"(        "servo_bravo": {)"
                    R"(            "pin": 11,)"
                    R"(            "init_value": 120,)"
                    R"(            "typeIndex": 1)"
                    R"(        })"
                    R"(    })"
                    R"(})"
                );

                Appendage appendage(xml, json);

                ASSERT_EQ(
                    appendage.getCommandFunctions("messenger"),
                    "void Servo_stop()\n"
                    "{\n"
                    "    for (uint16_t i = 0u; i <= 1u; ++i) {\n"
                    "        Servo_servo[i].detach();\n"
                    "    }\n"
                    "}\n"
                    "\n");
            }

            TEST(embgen_Appendage, AppendageDependencies)
            {
                tinyxml2::XMLDocument tinyDocument;
                ASSERT_EQ(tinyDocument.Parse(
                    "<appendage name='Alpha'>\n"
                    "    <variable type='uint8_t' name='a' appendage='Bravo' />\n"
                    "    <variable type='NonPrimitive' name='nonPrimitive'>\n"
                    "        <parameter type='int16_t*' name='b' appendage='Bravo' />\n"
                    "    </variable>\n"
                    "    <setup>\n"
                    "        <code>\n"
                    "            nonPrimitive.func(a);\n"
                    "        </code>\n"
                    "    </setup>\n"
                    "</appendage>\n"
                ), tinyxml2::XMLError::XML_SUCCESS);
                tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("appendage");
                ASSERT_NE(tinyElement, nullptr);

                std::shared_ptr<parser::Appendage> xml = std::make_shared<parser::Appendage>(tinyElement);

                ASSERT_EQ(xml->getName(), "Alpha");
                ASSERT_EQ(xml->getVersion(), "");
                ASSERT_EQ(xml->getLibDeps(), "");
                ASSERT_EQ(xml->getIncludes().size(), 0);
                ASSERT_EQ(xml->getVariables().size(), 2);
                ASSERT_NE(xml->getSetup(), nullptr);
                ASSERT_EQ(xml->getLoop(), nullptr);
                ASSERT_EQ(xml->getCommands().size(), 0);
                ASSERT_EQ(xml->getStop(), nullptr);

                nlohmann::json json = nlohmann::json::parse(
                    R"({)"
                    R"(    "Bravo": {)"
                    R"(        "bravo_one": {)"
                    R"(            "a": 10,)"
                    R"(            "b": -1024,)"
                    R"(            "typeIndex": 0)"
                    R"(        },)"
                    R"(        "bravo_two": {)"
                    R"(            "a": 30,)"
                    R"(            "b": -2048,)"
                    R"(            "typeIndex": 1)"
                    R"(        })"
                    R"(    },)"
                    R"(    "Alpha": {)"
                    R"(        "alpha_three": {)"
                    R"(            "a": "bravo_two",)"
                    R"(            "b": "bravo_one",)"
                    R"(            "typeIndex": 0)"
                    R"(        },)"
                    R"(        "alpha_four": {)"
                    R"(            "a": "bravo_one",)"
                    R"(            "b": "bravo_two",)"
                    R"(            "typeIndex": 1)"
                    R"(        })"
                    R"(    })"
                    R"(})"
                );

                Appendage appendage(xml, json);

                ASSERT_EQ(
                    appendage.getVariables(),
                    "uint16_t Alpha_a[2] = {\n"
                    "    1u,\n"
                    "    0u\n"
                    "};\n"
                    "uint16_t Alpha_b[2] = {\n"
                    "    0u,\n"
                    "    1u\n"
                    "};\n"
                    "NonPrimitive Alpha_nonPrimitive[2] ={\n"
                    "    NonPrimitive(&Bravo_b[Alpha_b[0]]),\n"
                    "    NonPrimitive(&Bravo_b[Alpha_b[1]])\n"
                    "};\n"
                );

                ASSERT_EQ(
                    appendage.getSetup(),
                    "    for (uint16_t i = 0u; i <= 1u; ++i) {\n"
                    "        Alpha_nonPrimitive[i].func(Bravo_a[Alpha_a[i]]);\n"
                    "    }\n"
                );
            }
        }
    }
}