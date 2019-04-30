#include <gtest/gtest.h>
#include <tinyxml2.h>
#include "EmbGen/Parameter.hpp"
#include "EmbGen/Exceptions.hpp"

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
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("parameter");

                    tinyElement->SetAttribute("type", "uint8_t");
                    tinyElement->SetAttribute("name", "val");

                    Parameter parameter(tinyElement);

                    ASSERT_EQ(parameter.getType(), "uint8_t");
                    ASSERT_EQ(parameter.getName(), "val");
                    ASSERT_FALSE(parameter.isCore());
                }

                TEST(parser_Parameter, VariableParameter)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("parameter");

                    tinyElement->SetAttribute("type", "int16_t");
                    tinyElement->SetAttribute("name", "val");
                    tinyElement->SetAttribute("core", true);

                    Parameter parameter(tinyElement);

                    ASSERT_EQ(parameter.getType(), "int16_t");
                    ASSERT_EQ(parameter.getName(), "val");
                    ASSERT_TRUE(parameter.isCore());
                }

                TEST(parser_Parameter, ParameterValidation)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("parameter");

                    tinyElement->SetAttribute("type", "int16_t");
                    tinyElement->SetAttribute("name", "val");
                    tinyElement->SetAttribute("min", -512);
                    tinyElement->SetAttribute("max", 1024);

                    Parameter parameter(tinyElement);

                    ASSERT_EQ(parameter.getType(), "int16_t");
                    ASSERT_EQ(parameter.getName(), "val");
                    ASSERT_FALSE(parameter.isCore());
                    ASSERT_EQ(parameter.getMin<int16_t>(), -512);
                    ASSERT_EQ(parameter.getMax<int16_t>(), 1024);
                }

                TEST(parser_Parameter, MissingType)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("parameter");

                    tinyElement->SetAttribute("name", "bar");

                    ASSERT_THROW(Parameter parameter(tinyElement), AttributeException);
                }

                TEST(parser_Parameter, MissingName)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("parameter");

                    tinyElement->SetAttribute("type", "Foo");

                    ASSERT_THROW(Parameter parameter(tinyElement), AttributeException);
                }

                TEST(parser_Parameter, ExtraAttribute)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("parameter");

                    tinyElement->SetAttribute("type", "uint8_t");
                    tinyElement->SetAttribute("name", "val");
                    tinyElement->SetAttribute("extra", "attribute");

                    ASSERT_THROW(Parameter parameter(tinyElement), AttributeException);
                }

                TEST(parser_Parameter, ExtraElements)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("parameter");

                    tinyElement->SetAttribute("type", "uint8_t");
                    tinyElement->SetAttribute("name", "val");

                    tinyElement->InsertEndChild(tinyDocument.NewElement("foo"));

                    ASSERT_THROW(Parameter parameter(tinyElement), ElementException);
                }

                TEST(parser_Parameter, InvalidValidationType)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("parameter");

                    tinyElement->SetAttribute("type", "float");
                    tinyElement->SetAttribute("name", "val");
                    tinyElement->SetAttribute("max", 3.14159f);

                    Parameter parameter(tinyElement);

                    ASSERT_EQ(parameter.getType(), "float");
                    ASSERT_EQ(parameter.getName(), "val");
                    ASSERT_FALSE(parameter.isCore());
                    ASSERT_THROW(parameter.getMin<float>(), AttributeException);
                    ASSERT_THROW(parameter.getMax<uint8_t>(), AttributeException);
                }
            }
        }
    }
}