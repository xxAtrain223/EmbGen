#include <gtest/gtest.h>
#include <tinyxml2.h>
#include "EmbGen/Parser/Variable.hpp"
#include "EmbGen/Parser/Exceptions.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            namespace test
            {
                TEST(parser_Variable, uint8_t)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<variable type='uint8_t' name='val' />\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("variable");
                    ASSERT_NE(tinyElement, nullptr);

                    Variable variable(tinyElement);

                    ASSERT_EQ(variable.getType(), "uint8_t");
                    ASSERT_EQ(variable.getName(), "val");
                    ASSERT_FALSE(variable.isCore());
                    ASSERT_EQ(variable.getParameters().size(), 0);
                    ASSERT_EQ(variable.getAppendage(), "");
                }

                TEST(parser_Variable, CoreVal)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<variable type='int16_t' name='coreVal' core='true' />\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("variable");
                    ASSERT_NE(tinyElement, nullptr);

                    Variable variable(tinyElement);

                    ASSERT_EQ(variable.getType(), "int16_t");
                    ASSERT_EQ(variable.getName(), "coreVal");
                    ASSERT_TRUE(variable.isCore());
                    ASSERT_EQ(variable.getParameters().size(), 0);
                    ASSERT_EQ(variable.getAppendage(), "");
                }

                TEST(parser_Variable, Parameters)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<variable type='Foo' name='bar' core='false'>\n"
                        "    <parameter type='int16_t' name='baz' />\n"
                        "</variable>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("variable");
                    ASSERT_NE(tinyElement, nullptr);

                    Variable variable(tinyElement);

                    ASSERT_EQ(variable.getType(), "Foo");
                    ASSERT_EQ(variable.getName(), "bar");
                    ASSERT_FALSE(variable.isCore());
                    ASSERT_EQ(variable.getParameters().size(), 1);
                    ASSERT_EQ(variable.getAppendage(), "");
                }

                TEST(parser_Variable, Appendage)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<variable type='uint8_t' name='val' appendage='OtherAppendage' />\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("variable");
                    ASSERT_NE(tinyElement, nullptr);

                    Variable variable(tinyElement);

                    ASSERT_EQ(variable.getType(), "uint8_t");
                    ASSERT_EQ(variable.getName(), "val");
                    ASSERT_FALSE(variable.isCore());
                    ASSERT_EQ(variable.getParameters().size(), 0);
                    ASSERT_EQ(variable.getAppendage(), "OtherAppendage");
                }

                TEST(parser_Variable, AppendageParameter)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<variable type='Foo' name='bar' core='false'>\n"
                        "    <parameter type='int16_t*' name='baz' appendage='OtherAppendage' />\n"
                        "</variable>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("variable");
                    ASSERT_NE(tinyElement, nullptr);

                    Variable variable(tinyElement);

                    ASSERT_EQ(variable.getType(), "Foo");
                    ASSERT_EQ(variable.getName(), "bar");
                    ASSERT_FALSE(variable.isCore());
                    ASSERT_EQ(variable.getParameters().size(), 1);
                    ASSERT_EQ(variable.getAppendage(), "");
                }

                TEST(parser_Variable, MissingType)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<variable name='bar' />\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("variable");
                    ASSERT_NE(tinyElement, nullptr);

                    ASSERT_THROW(Variable variable(tinyElement), AttributeException);
                }

                TEST(parser_Variable, MissingName)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<variable type='Foo' />\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("variable");
                    ASSERT_NE(tinyElement, nullptr);

                    ASSERT_THROW(Variable variable(tinyElement), AttributeException);
                }

                TEST(parser_Variable, ExtraAttribute)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<variable type='uint8_t' name='val' core='true' extra='attribute' />\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("variable");
                    ASSERT_NE(tinyElement, nullptr);

                    ASSERT_THROW(Variable variable(tinyElement), AttributeException);
                }

                TEST(parser_Variable, ExtraElements)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<variable type='Foo' name='bar' core='false'>\n"
                        "    <parameter type='float' name='baz' />\n"
                        "    <extra-element />\n"
                        "</variable>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("variable");
                    ASSERT_NE(tinyElement, nullptr);

                    ASSERT_THROW(Variable variable(tinyElement), ElementException);
                }
            }
        }
    }
}