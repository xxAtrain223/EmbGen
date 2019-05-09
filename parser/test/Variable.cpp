#include <gtest/gtest.h>
#include <tinyxml2.h>
#include "EmbGen/Variable.hpp"
#include "EmbGen/ParserExceptions.hpp"

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
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("variable");

                    tinyElement->SetAttribute("type", "uint8_t");
                    tinyElement->SetAttribute("name", "val");

                    Variable variable(tinyElement);

                    ASSERT_EQ(variable.getType(), "uint8_t");
                    ASSERT_EQ(variable.getName(), "val");
                    ASSERT_FALSE(variable.isCore());
                    ASSERT_EQ(variable.getParameters().size(), 0);
                }

                TEST(parser_Variable, CoreVal)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("variable");

                    tinyElement->SetAttribute("type", "int16_t");
                    tinyElement->SetAttribute("name", "coreVal");
                    tinyElement->SetAttribute("core", true);

                    Variable variable(tinyElement);

                    ASSERT_EQ(variable.getType(), "int16_t");
                    ASSERT_EQ(variable.getName(), "coreVal");
                    ASSERT_TRUE(variable.isCore());
                    ASSERT_EQ(variable.getParameters().size(), 0);
                }

                TEST(parser_Variable, Parameters)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("variable");

                    tinyElement->SetAttribute("type", "Foo");
                    tinyElement->SetAttribute("name", "bar");
                    tinyElement->SetAttribute("core", false);

                    tinyxml2::XMLElement* tinyParameter = tinyDocument.NewElement("parameter");
                    tinyParameter->SetAttribute("type", "int16_t");
                    tinyParameter->SetAttribute("name", "baz");
                    tinyElement->InsertEndChild(tinyParameter);

                    Variable variable(tinyElement);

                    ASSERT_EQ(variable.getType(), "Foo");
                    ASSERT_EQ(variable.getName(), "bar");
                    ASSERT_FALSE(variable.isCore());
                    ASSERT_EQ(variable.getParameters().size(), 1);
                }

                TEST(parser_Variable, MissingType)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("variable");

                    tinyElement->SetAttribute("name", "bar");

                    ASSERT_THROW(Variable variable(tinyElement), AttributeException);
                }

                TEST(parser_Variable, MissingName)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("variable");

                    tinyElement->SetAttribute("type", "Foo");

                    ASSERT_THROW(Variable variable(tinyElement), AttributeException);
                }

                TEST(parser_Variable, ExtraAttribute)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("variable");

                    tinyElement->SetAttribute("type", "uint8_t");
                    tinyElement->SetAttribute("name", "val");
                    tinyElement->SetAttribute("core", true);
                    tinyElement->SetAttribute("extra", "attribute");

                    ASSERT_THROW(Variable variable(tinyElement), AttributeException);
                }

                TEST(parser_Variable, ExtraElements)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("variable");

                    tinyElement->SetAttribute("type", "Foo");
                    tinyElement->SetAttribute("name", "bar");
                    tinyElement->SetAttribute("core", false);

                    tinyxml2::XMLElement* tinyParameter = tinyDocument.NewElement("parameter");
                    tinyParameter->SetAttribute("type", "float");
                    tinyParameter->SetAttribute("name", "baz");
                    tinyElement->InsertEndChild(tinyParameter);
                    tinyElement->InsertEndChild(tinyDocument.NewElement("extraElement"));

                    ASSERT_THROW(Variable variable(tinyElement), ElementException);
                }
            }
        }
    }
}