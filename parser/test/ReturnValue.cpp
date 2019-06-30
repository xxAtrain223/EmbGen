#include <gtest/gtest.h>
#include <tinyxml2.h>
#include "EmbGen/Parser/ReturnValue.hpp"
#include "EmbGen/Parser/Exceptions.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            namespace test
            {
                TEST(parser_ReturnValue, CommandReturnValue)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<return_value type='uint8_t' name='val' />\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("return_value");
                    ASSERT_NE(tinyElement, nullptr);

                    ReturnValue returnValue(tinyElement);

                    ASSERT_EQ(returnValue.getType(), "uint8_t");
                    ASSERT_EQ(returnValue.getName(), "val");
                }

                TEST(parser_ReturnValue, MissingType)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<return_value name='bar' />\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("return_value");
                    ASSERT_NE(tinyElement, nullptr);

                    ASSERT_THROW(ReturnValue returnValue(tinyElement), AttributeException);
                }

                TEST(parser_ReturnValue, MissingName)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<return_value type='Foo' />\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("return_value");
                    ASSERT_NE(tinyElement, nullptr);

                    ASSERT_THROW(ReturnValue returnValue(tinyElement), AttributeException);
                }

                TEST(parser_ReturnValue, ExtraAttribute)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<return_value type='uint8_t' name='val' extra='attribute' />\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("return_value");
                    ASSERT_NE(tinyElement, nullptr);

                    ASSERT_THROW(ReturnValue returnValue(tinyElement), AttributeException);
                }

                TEST(parser_ReturnValue, ExtraElements)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<return_value type='uint8_t' name='val'>\n"
                        "    <foo />\n"
                        "</return_value>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("return_value");
                    ASSERT_NE(tinyElement, nullptr);

                    ASSERT_THROW(ReturnValue returnValue(tinyElement), ElementException);
                }
            }
        }
    }
}