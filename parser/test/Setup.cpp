#include <gtest/gtest.h>
#include <tinyxml2.h>
#include "EmbGen/Parser/Setup.hpp"
#include "EmbGen/Parser/Exceptions.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            namespace test
            {
                TEST(parser_Setup, OneCode)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<setup>\n"
                        "    <code>\n"
                        "        // Code\n"
                        "    </code>\n"
                        "</setup>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("setup");
                    ASSERT_NE(tinyElement, nullptr);

                    parser::Setup setup(tinyElement);

                    std::vector<Code> code = setup.getCode();
                    ASSERT_EQ(code.size(), 1);
                    ASSERT_EQ(code[0].getInsert(), Code::Insert::Each);
                    ASSERT_EQ(code[0].getText(), "    // Code");
                }

                TEST(parser_Setup, TwoCodes)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<setup>\n"
                        "    <code insert='once'>\n"
                        "        // First Code\n"
                        "    </code>\n"
                        "    <code insert='each'>\n"
                        "        // Second Code\n"
                        "    </code>\n"
                        "</setup>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("setup");
                    ASSERT_NE(tinyElement, nullptr);

                    parser::Setup setup(tinyElement);

                    std::vector<Code> code = setup.getCode();
                    ASSERT_EQ(code.size(), 2);
                    ASSERT_EQ(code[0].getInsert(), Code::Insert::Once);
                    ASSERT_EQ(code[0].getText(), "    // First Code");
                    ASSERT_EQ(code[1].getInsert(), Code::Insert::Each);
                    ASSERT_EQ(code[1].getText(), "    // Second Code");
                }

                TEST(parser_Setup, NoCode)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<setup>\n"
                        "</setup>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("setup");
                    ASSERT_NE(tinyElement, nullptr);

                    ASSERT_THROW(parser::Setup setup(tinyElement), ElementException);
                }

                TEST(parser_Setup, ExtraAttribute)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<setup extra='attribute'>\n"
                        "    <code insert='once'>\n"
                        "        // First Code\n"
                        "    </code>\n"
                        "</setup>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("setup");
                    ASSERT_NE(tinyElement, nullptr);

                    ASSERT_THROW(parser::Setup setup(tinyElement), AttributeException);
                }

                TEST(parser_Setup, ExtraElements)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<setup>\n"
                        "    <code insert='once'>\n"
                        "        // First Code\n"
                        "    </code>\n"
                        "    <extra-element />\n"
                        "</setup>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("setup");
                    ASSERT_NE(tinyElement, nullptr);

                    ASSERT_THROW(parser::Setup setup(tinyElement), ElementException);
                }
            }
        }
    }
}