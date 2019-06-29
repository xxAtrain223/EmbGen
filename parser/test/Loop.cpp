#include <gtest/gtest.h>
#include <tinyxml2.h>
#include "EmbGen/Parser/Loop.hpp"
#include "EmbGen/Parser/Exceptions.hpp"

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
                    ASSERT_EQ(tinyDocument.Parse(
                        "<loop>\n"
                        "    <code>\n"
                        "        // Code\n"
                        "    </code>\n"
                        "</loop>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("loop");
                    ASSERT_NE(tinyElement, nullptr);

                    parser::Loop loop(tinyElement);

                    std::vector<Code> code = loop.getCode();
                    ASSERT_EQ(code.size(), 1);
                    ASSERT_EQ(code[0].getInsert(), Code::Insert::Each);
                    ASSERT_EQ(code[0].getText(), "    // Code");
                }

                TEST(parser_Loop, TwoCodes)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<loop>\n"
                        "    <code insert='once'>\n"
                        "        // First Code\n"
                        "    </code>\n"
                        "    <code insert='each'>\n"
                        "        // Second Code\n"
                        "    </code>\n"
                        "</loop>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("loop");
                    ASSERT_NE(tinyElement, nullptr);

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
                    ASSERT_EQ(tinyDocument.Parse(
                        "<loop>\n"
                        "</loop>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("loop");
                    ASSERT_NE(tinyElement, nullptr);

                    ASSERT_THROW(parser::Loop loop(tinyElement), ElementException);
                }

                TEST(parser_Loop, ExtraAttribute)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<loop extra='attribute'>\n"
                        "    <code insert='once'>\n"
                        "        // First Code\n"
                        "    </code>\n"
                        "</loop>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("loop");
                    ASSERT_NE(tinyElement, nullptr);

                    ASSERT_THROW(parser::Loop loop(tinyElement), AttributeException);
                }

                TEST(parser_Loop, ExtraElements)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<loop>\n"
                        "    <code insert='once'>\n"
                        "        // First Code\n"
                        "    </code>\n"
                        "    <extra-element />\n"
                        "</loop>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("loop");
                    ASSERT_NE(tinyElement, nullptr);

                    ASSERT_THROW(parser::Loop loop(tinyElement), ElementException);
                }
            }
        }
    }
}