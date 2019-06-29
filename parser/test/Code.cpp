#include <gtest/gtest.h>
#include <tinyxml2.h>
#include "EmbGen/Parser/Code.hpp"
#include "EmbGen/Parser/Exceptions.hpp"

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
                    ASSERT_EQ(tinyDocument.Parse(
                        "<code insert='each'>\n"
                        "    rv = 7;\n"
                        "</code>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("code");
                    ASSERT_NE(tinyElement, nullptr);

                    Code code(tinyElement);

                    ASSERT_EQ(code.getInsert(), Code::Insert::Each);
                    ASSERT_EQ(code.getText(), "    rv = 7;");
                }

                TEST(parser_Code, SecondLineIndented)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<code insert='each'>\n"
                        "    if (x == 0)\n"
                        "        DoThing();\n"
                        "</code>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("code");
                    ASSERT_NE(tinyElement, nullptr);

                    Code code(tinyElement);

                    ASSERT_EQ(code.getInsert(), Code::Insert::Each);
                    ASSERT_EQ(code.getText(),
                        "    if (x == 0)\n"
                        "        DoThing();");
                }

                TEST(parser_Code, FirstLineIndented)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<code insert='once'>\n"
                        "        im = a;\n"
                        "    monster();\n"
                        "</code>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("code");
                    ASSERT_NE(tinyElement, nullptr);

                    Code code(tinyElement);

                    ASSERT_EQ(code.getInsert(), Code::Insert::Once);
                    ASSERT_EQ(code.getText(),
                        "        im = a;\n"
                        "    monster();");
                }

                TEST(parser_Code, TrimWhitespace)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<code insert='once'>\n"
                        "\n\n\n"
                        "    // Whitespace Test\n"
                        "\n\n\n"
                        "</code>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("code");
                    ASSERT_NE(tinyElement, nullptr);

                    Code code(tinyElement);

                    ASSERT_EQ(code.getInsert(), Code::Insert::Once);
                    std::string text = code.getText();
                    ASSERT_EQ(text, "    // Whitespace Test");
                }

                TEST(parser_Code, InvalidInsert)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<code insert='wrong'>\n"
                        "    // Whatever\n"
                        "</code>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("code");
                    ASSERT_NE(tinyElement, nullptr);

                    ASSERT_THROW(Code code(tinyElement), AttributeException);
                }
            }
        }
    }
}