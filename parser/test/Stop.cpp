#include <gtest/gtest.h>
#include <tinyxml2.h>
#include "EmbGen/Parser/Stop.hpp"
#include "EmbGen/Parser/Exceptions.hpp"
#include "EmbGen/Parser/Code.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            namespace test
            {
                TEST(parser_Stop, Command)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<stop command='detach' />\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("stop");
                    ASSERT_NE(tinyElement, nullptr);

                    Stop stop(tinyElement);

                    ASSERT_EQ(stop.getCommand(), "detach");
                    ASSERT_EQ(stop.getCode(), nullptr);
                }

                TEST(parser_Stop, Code)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<stop>\n"
                        "    <code>\n"
                        "        // Code\n"
                        "    </code>\n"
                        "</stop>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("stop");
                    ASSERT_NE(tinyElement, nullptr);

                    Stop stop(tinyElement);

                    ASSERT_EQ(stop.getCommand(), "");
                    ASSERT_EQ(stop.getCode()->getText(), "    // Code");
                }

                TEST(parser_Stop, CommandAndCode)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<stop command='detach'>\n"
                        "    <code>\n"
                        "        // Code\n"
                        "    </code>\n"
                        "</stop>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("stop");
                    ASSERT_NE(tinyElement, nullptr);

                    ASSERT_THROW(Stop stop(tinyElement), ParserException);
                }

                TEST(parser_Stop, NoCommandOrCode)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<stop />\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("stop");
                    ASSERT_NE(tinyElement, nullptr);

                    ASSERT_THROW(Stop stop(tinyElement), ParserException);
                }

                TEST(parser_Stop, MultipleCodes)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<stop>\n"
                        "    <code>\n"
                        "        // Code 1\n"
                        "    </code>\n"
                        "    <code>\n"
                        "        // Code 2\n"
                        "    </code>\n"
                        "</stop>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("stop");
                    ASSERT_NE(tinyElement, nullptr);

                    ASSERT_THROW(Stop stop(tinyElement), ElementException);
                }

                TEST(parser_Stop, ExtraAttribute)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<stop command='detach' extra='attribute' />\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("stop");
                    ASSERT_NE(tinyElement, nullptr);

                    ASSERT_THROW(parser::Stop stop(tinyElement), AttributeException);
                }

                TEST(parser_Stop, ExtraElements)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<stop command='detach'>\n"
                        "    <extra-element />\n"
                        "</stop>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("stop");
                    ASSERT_NE(tinyElement, nullptr);

                    ASSERT_THROW(parser::Stop stop(tinyElement), ElementException);
                }
            }
        }
    }
}