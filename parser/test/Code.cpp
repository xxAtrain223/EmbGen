#include <gtest/gtest.h>
#include <tinyxml2.h>
#include "EmbGen/Code.hpp"
#include "EmbGen/Exceptions.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            namespace test
            {
                TEST(parser_Code, OneLine)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("code");

                    tinyElement->SetAttribute("insert", "each");
                    tinyElement->SetText("rv = 7;");

                    Code code(tinyElement);

                    ASSERT_EQ(code.getInsert(), Code::Insert::Each);
                    ASSERT_EQ(code.getText(), "    rv = 7;");
                }

                TEST(parser_Code, SecondLineIndented)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("code");

                    tinyElement->SetText("\n"
                        "if (x == 0)\n"
                        "    DoThing();");

                    Code code(tinyElement);

                    ASSERT_EQ(code.getInsert(), Code::Insert::Each);
                    ASSERT_EQ(code.getText(),
                        "    if (x == 0)\n"
                        "        DoThing();");
                }

                TEST(parser_Code, FirstLineIndented)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("code");

                    tinyElement->SetAttribute("insert", "once");
                    tinyElement->SetText(
                        "    im = a;\n"
                        "monster();");

                    Code code(tinyElement);

                    ASSERT_EQ(code.getInsert(), Code::Insert::Once);
                    ASSERT_EQ(code.getText(),
                        "        im = a;\n"
                        "    monster();");
                }

                TEST(parser_Code, TrimWhitespace)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("code");

                    tinyElement->SetAttribute("insert", "once");
                    tinyElement->SetText("\n\n\n// Whitespace Test\n\n\n");

                    Code code(tinyElement);

                    ASSERT_EQ(code.getInsert(), Code::Insert::Once);
                    std::string text = code.getText();
                    ASSERT_EQ(text, "    // Whitespace Test");
                }

                TEST(parser_Code, InvalidInsert)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("code");

                    tinyElement->SetAttribute("insert", "wrong");
                    tinyElement->SetText("// Whatever");

                    ASSERT_THROW(Code code(tinyElement), AttributeException);
                }
            }
        }
    }
}