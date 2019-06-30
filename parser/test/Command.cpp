#include <gtest/gtest.h>
#include <tinyxml2.h>
#include "EmbGen/Parser/Command.hpp"
#include "EmbGen/Parser/Exceptions.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            namespace test
            {
                TEST(parser_Command, NoParameters)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<command name='NoParam'>\n"
                        "    <code>\n"
                        "        // Command Code\n"
                        "    </code>\n"
                        "</command>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("command");
                    ASSERT_NE(tinyElement, nullptr);

                    Command command(tinyElement);
                    std::vector<Parameter> parameters = command.getParameters();
                    std::shared_ptr<Code> code = command.getCode();

                    ASSERT_EQ(command.getName(), "NoParam");
                    ASSERT_EQ(parameters.size(), 0);
                    ASSERT_EQ(code->getInsert(), Code::Insert::Each);
                    ASSERT_EQ(code->getText(), "    // Command Code");
                }

                TEST(parser_Command, OneParameter)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<command name='OneParam'>\n"
                        "    <parameter type='uint8_t' name='val' />\n"
                        "    <code>\n"
                        "        // Use val\n"
                        "    </code>\n"
                        "</command>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("command");
                    ASSERT_NE(tinyElement, nullptr);

                    Command command(tinyElement);
                    std::vector<Parameter> parameters = command.getParameters();
                    std::shared_ptr<Code> code = command.getCode();

                    ASSERT_EQ(command.getName(), "OneParam");
                    ASSERT_EQ(parameters.size(), 1);
                    ASSERT_EQ(parameters.at(0).getType(), "uint8_t");
                    ASSERT_EQ(parameters.at(0).getName(), "val");
                    ASSERT_EQ(code->getInsert(), Code::Insert::Each);
                    ASSERT_EQ(code->getText(), "    // Use val");
                }

                TEST(parser_Command, TwoParameters)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<command name='TwoParam'>\n"
                        "    <parameter type='int16_t' name='int_val' />\n"
                        "    <parameter type='float' name='float_val' />\n"
                        "    <code>\n"
                        "        // Do something with int_val and float_val\n"
                        "    </code>\n"
                        "</command>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("command");
                    ASSERT_NE(tinyElement, nullptr);

                    Command command(tinyElement);
                    std::vector<Parameter> parameters = command.getParameters();
                    std::shared_ptr<Code> code = command.getCode();

                    ASSERT_EQ(command.getName(), "TwoParam");
                    ASSERT_EQ(parameters.size(), 2);
                    ASSERT_EQ(parameters.at(0).getType(), "int16_t");
                    ASSERT_EQ(parameters.at(0).getName(), "int_val");
                    ASSERT_EQ(parameters.at(1).getType(), "float");
                    ASSERT_EQ(parameters.at(1).getName(), "float_val");
                    ASSERT_EQ(code->getInsert(), Code::Insert::Each);
                    ASSERT_EQ(code->getText(), "    // Do something with int_val and float_val");
                }

                TEST(parser_Command, OneReturnValue)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<command name='OneParam'>\n"
                        "    <return_value type='uint8_t' name='val' />\n"
                        "    <code>\n"
                        "        // Use val\n"
                        "    </code>\n"
                        "</command>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("command");
                    ASSERT_NE(tinyElement, nullptr);

                    Command command(tinyElement);
                    std::vector<ReturnValue> returnValues = command.getReturnValues();
                    std::shared_ptr<Code> code = command.getCode();

                    ASSERT_EQ(command.getName(), "OneParam");
                    ASSERT_EQ(returnValues.size(), 1);
                    ASSERT_EQ(returnValues.at(0).getType(), "uint8_t");
                    ASSERT_EQ(returnValues.at(0).getName(), "val");
                    ASSERT_EQ(code->getInsert(), Code::Insert::Each);
                    ASSERT_EQ(code->getText(), "    // Use val");
                }

                TEST(parser_Command, TwoReturnValues)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<command name='TwoParam'>\n"
                        "    <return_value type='int16_t' name='int_val' />\n"
                        "    <return_value type='float' name='float_val' />\n"
                        "    <code>\n"
                        "        // Do something with int_val and float_val\n"
                        "    </code>\n"
                        "</command>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("command");
                    ASSERT_NE(tinyElement, nullptr);

                    Command command(tinyElement);
                    std::vector<ReturnValue> returnValues = command.getReturnValues();
                    std::shared_ptr<Code> code = command.getCode();

                    ASSERT_EQ(command.getName(), "TwoParam");
                    ASSERT_EQ(returnValues.size(), 2);
                    ASSERT_EQ(returnValues.at(0).getType(), "int16_t");
                    ASSERT_EQ(returnValues.at(0).getName(), "int_val");
                    ASSERT_EQ(returnValues.at(1).getType(), "float");
                    ASSERT_EQ(returnValues.at(1).getName(), "float_val");
                    ASSERT_EQ(code->getInsert(), Code::Insert::Each);
                    ASSERT_EQ(code->getText(), "    // Do something with int_val and float_val");
                }

                TEST(parser_Command, NoName)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<command>\n"
                        "    <code>\n"
                        "        // Command code\n"
                        "    </code>\n"
                        "</command>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("command");
                    ASSERT_NE(tinyElement, nullptr);

                    ASSERT_THROW(Command command(tinyElement), AttributeException);
                }

                TEST(parser_Command, NoCode)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<command name='NoCode'>\n"
                        "</command>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("command");
                    ASSERT_NE(tinyElement, nullptr);

                    ASSERT_THROW(Command command(tinyElement), ElementException);
                }

                TEST(parser_Command, ExtraAttribute)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<command name='ExtraAttribute' extra='attribute'>\n"
                        "    <code>\n"
                        "        // Extra Attribute\n"
                        "    </code>\n"
                        "</command>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("command");
                    ASSERT_NE(tinyElement, nullptr);

                    ASSERT_THROW(Command command(tinyElement), AttributeException);
                }

                TEST(parser_Command, ExtraElement)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<command name='ExtraElement'>\n"
                        "    <code>\n"
                        "        // Extra Element\n"
                        "    </code>\n"
                        "    <extra-element />\n"
                        "</command>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("command");
                    ASSERT_NE(tinyElement, nullptr);

                    ASSERT_THROW(Command command(tinyElement), ElementException);
                }
            }
        }
    }
}