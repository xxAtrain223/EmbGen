#include <gtest/gtest.h>
#include <tinyxml2.h>
#include "EmbGen/Include.hpp"
#include "EmbGen/Exceptions.hpp"

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
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("include");

                    tinyElement->SetText("test.h");

                    Include include(tinyElement);

                    ASSERT_EQ(include.getValue(), "test.h");
                    ASSERT_FALSE(include.isStandard());
                }

                TEST(parser_Include, GetValue_Standard)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("include");

                    tinyElement->SetText("test.h");
                    tinyElement->SetAttribute("standard", true);

                    Include include(tinyElement);

                    ASSERT_EQ(include.getValue(), "test.h");
                    ASSERT_TRUE(include.isStandard());
                }

                TEST(parser_Include, GetValue_NonStandard)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("include");

                    tinyElement->SetText("test.h");
                    tinyElement->SetAttribute("standard", false);

                    Include include(tinyElement);

                    ASSERT_EQ(include.getValue(), "test.h");
                    ASSERT_FALSE(include.isStandard());
                }

                TEST(parser_Include, ExtraAttribute)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("include");

                    tinyElement->SetText("test.h");
                    tinyElement->SetAttribute("extra", "attribute");

                    ASSERT_THROW(Include include(tinyElement), AttributeException);
                }

                TEST(parser_Include, ChildElements)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("include");

                    tinyElement->SetText("test.h");
                    tinyElement->InsertEndChild(tinyDocument.NewElement("foo"));

                    ASSERT_THROW(Include include(tinyElement), ElementException);
                }
            }
        }
    }
}