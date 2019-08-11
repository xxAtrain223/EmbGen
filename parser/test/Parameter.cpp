#include <gtest/gtest.h>
#include <tinyxml2.h>
#include "EmbGen/Parser/Parameter.hpp"
#include "EmbGen/Parser/Exceptions.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            namespace test
            {
                TEST(parser_Parameter, CommandParameter)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<parameter type='uint8_t' name='val' />\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("parameter");
                    ASSERT_NE(tinyElement, nullptr);

                    Parameter parameter(tinyElement);

                    ASSERT_EQ(parameter.getType(), "uint8_t");
                    ASSERT_EQ(parameter.getName(), "val");
                    ASSERT_FALSE(parameter.isCore());
                    ASSERT_EQ(parameter.getAppendage(), "");
                }

                TEST(parser_Parameter, VariableParameter)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<parameter type='int16_t' name='val' core='true' />\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("parameter");
                    ASSERT_NE(tinyElement, nullptr);

                    Parameter parameter(tinyElement);

                    ASSERT_EQ(parameter.getType(), "int16_t");
                    ASSERT_EQ(parameter.getName(), "val");
                    ASSERT_TRUE(parameter.isCore());
                    ASSERT_EQ(parameter.getAppendage(), "");
                }

                TEST(parser_Parameter, Appendage)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<parameter type='int16_t' name='val' appendage='OtherAppendage' />\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("parameter");
                    ASSERT_NE(tinyElement, nullptr);

                    Parameter parameter(tinyElement);

                    ASSERT_EQ(parameter.getType(), "int16_t");
                    ASSERT_EQ(parameter.getName(), "val");
                    ASSERT_FALSE(parameter.isCore());
                    ASSERT_EQ(parameter.getAppendage(), "OtherAppendage");
                }

                TEST(parser_Parameter, ParameterValidation)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<parameter type='int16_t' name='val' min='-512' max='1024' />\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("parameter");
                    ASSERT_NE(tinyElement, nullptr);

                    Parameter parameter(tinyElement);

                    ASSERT_EQ(parameter.getType(), "int16_t");
                    ASSERT_EQ(parameter.getName(), "val");
                    ASSERT_FALSE(parameter.isCore());
                    ASSERT_EQ(parameter.getMin(), "-512");
                    ASSERT_EQ(parameter.getMax(), "1024");
                }

                TEST(parser_Parameter, MissingType)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<parameter name='bar' />\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("parameter");
                    ASSERT_NE(tinyElement, nullptr);

                    ASSERT_THROW(Parameter parameter(tinyElement), AttributeException);
                }

                TEST(parser_Parameter, MissingName)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<parameter type='Foo' />\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("parameter");
                    ASSERT_NE(tinyElement, nullptr);

                    ASSERT_THROW(Parameter parameter(tinyElement), AttributeException);
                }

                TEST(parser_Parameter, ExtraAttribute)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<parameter type='uint8_t' name='val' extra='attribute' />\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("parameter");
                    ASSERT_NE(tinyElement, nullptr);

                    ASSERT_THROW(Parameter parameter(tinyElement), AttributeException);
                }

                TEST(parser_Parameter, ExtraElements)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<parameter type='uint8_t' name='val'>\n"
                        "    <foo />\n"
                        "</parameter>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("parameter");
                    ASSERT_NE(tinyElement, nullptr);

                    ASSERT_THROW(Parameter parameter(tinyElement), ElementException);
                }
            }
        }
    }
}