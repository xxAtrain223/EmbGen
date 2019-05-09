#include <gtest/gtest.h>
#include <tinyxml2.h>
#include "EmbGen/Loop.hpp"
#include "EmbGen/ParserExceptions.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            namespace test
            {
                TEST(parser_Loop, OneCode)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("loop");

                    tinyxml2::XMLElement* tinyCode = tinyDocument.NewElement("code");
                    tinyCode->SetText("// Code");
                    tinyElement->InsertEndChild(tinyCode);

                    parser::Loop loop(tinyElement);

                    std::vector<Code> code = loop.getCode();
                    ASSERT_EQ(code.size(), 1);
                    ASSERT_EQ(code[0].getInsert(), Code::Insert::Each);
                    ASSERT_EQ(code[0].getText(), "    // Code");
                }

                TEST(parser_Loop, TwoCodes)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("loop");

                    tinyxml2::XMLElement* tinyCode = tinyDocument.NewElement("code");
                    tinyCode->SetAttribute("insert", "once");
                    tinyCode->SetText("// First Code");
                    tinyElement->InsertEndChild(tinyCode);

                    tinyCode = tinyDocument.NewElement("code");
                    tinyCode->SetAttribute("insert", "each");
                    tinyCode->SetText("// Second Code");
                    tinyElement->InsertEndChild(tinyCode);

                    parser::Loop loop(tinyElement);

                    std::vector<Code> code = loop.getCode();
                    ASSERT_EQ(code.size(), 2);
                    ASSERT_EQ(code[0].getInsert(), Code::Insert::Once);
                    ASSERT_EQ(code[0].getText(), "    // First Code");
                    ASSERT_EQ(code[1].getInsert(), Code::Insert::Each);
                    ASSERT_EQ(code[1].getText(), "    // Second Code");
                }

                TEST(parser_Loop, NoCode)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("loop");

                    ASSERT_THROW(parser::Loop loop(tinyElement), ElementException);
                }

                TEST(parser_Loop, ExtraAttribute)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("loop");

                    tinyxml2::XMLElement* tinyCode = tinyDocument.NewElement("code");
                    tinyCode->SetAttribute("insert", "once");
                    tinyCode->SetText("// First Code");
                    tinyElement->InsertEndChild(tinyCode);

                    tinyElement->SetAttribute("extra", "attribute");

                    ASSERT_THROW(parser::Loop loop(tinyElement), AttributeException);
                }

                TEST(parser_Loop, ExtraElements)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("loop");

                    tinyxml2::XMLElement* tinyCode = tinyDocument.NewElement("code");
                    tinyCode->SetAttribute("insert", "once");
                    tinyCode->SetText("// First Code");
                    tinyElement->InsertEndChild(tinyCode);

                    tinyElement->InsertEndChild(tinyDocument.NewElement("extra-element"));

                    ASSERT_THROW(parser::Loop loop(tinyElement), ElementException);
                }
            }
        }
    }
}