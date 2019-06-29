#include <gtest/gtest.h>
#include <tinyxml2.h>
#include "EmbGen/Parser/Include.hpp"
#include "EmbGen/Parser/Exceptions.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            namespace test
            {
                TEST(parser_Include, GetValue_DefaultStandard)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<include>test.h</include>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("include");
                    ASSERT_NE(tinyElement, nullptr);

                    Include include(tinyElement);

                    ASSERT_EQ(include.getValue(), "test.h");
                    ASSERT_FALSE(include.isStandard());
                }

                TEST(parser_Include, GetValue_Standard)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<include standard='true'>test.h</include>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("include");
                    ASSERT_NE(tinyElement, nullptr);

                    Include include(tinyElement);

                    ASSERT_EQ(include.getValue(), "test.h");
                    ASSERT_TRUE(include.isStandard());
                }

                TEST(parser_Include, GetValue_NonStandard)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<include standard='false'>test.h</include>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("include");
                    ASSERT_NE(tinyElement, nullptr);

                    Include include(tinyElement);

                    ASSERT_EQ(include.getValue(), "test.h");
                    ASSERT_FALSE(include.isStandard());
                }

                TEST(parser_Include, ExtraAttribute)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<include extra='attribute'>test.h</include>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("include");
                    ASSERT_NE(tinyElement, nullptr);

                    ASSERT_THROW(Include include(tinyElement), AttributeException);
                }

                TEST(parser_Include, ChildElements)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<include>\n"
                        "    <extra-element />"
                        "</include>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("include");
                    ASSERT_NE(tinyElement, nullptr);

                    ASSERT_THROW(Include include(tinyElement), ElementException);
                }
            }
        }
    }
}