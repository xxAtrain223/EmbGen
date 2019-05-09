#include <gtest/gtest.h>
#include <tinyxml2.h>
#include "EmbGen/Command.hpp"
#include "EmbGen/ParserExceptions.hpp"

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
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("command");

                    tinyElement->SetAttribute("name", "NoParam");

                    tinyxml2::XMLElement* codeElement = tinyDocument.NewElement("code");
                    codeElement->SetText("// Command Code");
                    tinyElement->InsertEndChild(codeElement);

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
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("command");

                    tinyElement->SetAttribute("name", "OneParam");

                    tinyxml2::XMLElement* tinyParameter = tinyDocument.NewElement("parameter");
                    tinyParameter->SetAttribute("type", "uint8_t");
                    tinyParameter->SetAttribute("name", "val");
                    tinyElement->InsertEndChild(tinyParameter);

                    tinyxml2::XMLElement* codeElement = tinyDocument.NewElement("code");
                    codeElement->SetText("// Use val");
                    tinyElement->InsertEndChild(codeElement);

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
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("command");

                    tinyElement->SetAttribute("name", "TwoParam");

                    tinyxml2::XMLElement* tinyParameter = tinyDocument.NewElement("parameter");
                    tinyParameter->SetAttribute("type", "int16_t");
                    tinyParameter->SetAttribute("name", "int_val");
                    tinyElement->InsertEndChild(tinyParameter);

                    tinyParameter = tinyDocument.NewElement("parameter");
                    tinyParameter->SetAttribute("type", "float");
                    tinyParameter->SetAttribute("name", "float_val");
                    tinyElement->InsertEndChild(tinyParameter);

                    tinyxml2::XMLElement* codeElement = tinyDocument.NewElement("code");
                    codeElement->SetText("// Do something with int_val and float_val");
                    codeElement->SetAttribute("insert", "each");
                    tinyElement->InsertEndChild(codeElement);

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

                TEST(parser_Command, NoName)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("command");

                    tinyxml2::XMLElement* codeElement = tinyDocument.NewElement("code");
                    codeElement->SetText("// Command Code");
                    tinyElement->InsertEndChild(codeElement);

                    ASSERT_THROW(Command command(tinyElement), AttributeException);
                }

                TEST(parser_Command, NoCode)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("command");

                    tinyElement->SetAttribute("name", "NoCode");

                    ASSERT_THROW(Command command(tinyElement), ElementException);
                }

                TEST(parser_Command, ExtraAttribute)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("command");

                    tinyElement->SetAttribute("name", "ExtraAttribute");
                    tinyElement->SetAttribute("extra", "attribute");

                    tinyxml2::XMLElement* codeElement = tinyDocument.NewElement("code");
                    codeElement->SetText("// Extra Attribute");
                    tinyElement->InsertEndChild(codeElement);

                    ASSERT_THROW(Command command(tinyElement), AttributeException);
                }

                TEST(parser_Command, ExtraElement)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("command");

                    tinyElement->SetAttribute("name", "ExtraAttribute");

                    tinyxml2::XMLElement* codeElement = tinyDocument.NewElement("code");
                    codeElement->SetText("// Extra Attribute");
                    tinyElement->InsertEndChild(codeElement);

                    tinyElement->InsertEndChild(tinyDocument.NewElement("extra-element"));

                    ASSERT_THROW(Command command(tinyElement), ElementException);
                }
            }
        }
    }
}