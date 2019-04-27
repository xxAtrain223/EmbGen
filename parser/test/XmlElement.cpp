#include <gtest/gtest.h>
#include <tinyxml2.h>
#include "EmbGen/XmlElement.hpp"
#include "EmbGen/Exceptions.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            namespace test
            {
                TEST(parser_XmlElement, GetName)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("foo");

                    XmlElement element(tinyElement);

                    ASSERT_EQ(element.getName(), "foo");
                }

                TEST(parser_XmlElement, GetText)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("foo");

                    tinyElement->SetText("bar");

                    XmlElement element(tinyElement);

                    ASSERT_EQ(element.getText(), "bar");
                }

                TEST(parser_XmlElement, GetLineNum)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("foo");

                    tinyElement->SetText("bar");

                    XmlElement element(tinyElement);

                    ASSERT_EQ(element.getLineNum(), 0);
                }

                TEST(parser_XmlElement, GetAttribute)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("foo");

                    tinyElement->SetAttribute("bar", "bar");
                    tinyElement->SetAttribute("baz", "baz");

                    XmlElement element(tinyElement);

                    EXPECT_FALSE(element.isAttributesEmpty());
                    ASSERT_NE(element.getAttribute("bar"), nullptr);
                    EXPECT_FALSE(element.isAttributesEmpty());
                    ASSERT_NE(element.getAttribute("baz"), nullptr);
                    EXPECT_TRUE(element.isAttributesEmpty());
                }

                TEST(parser_XmlElement, GetAttribute_AttributeException)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("foo");

                    tinyElement->SetAttribute("bar", "bar");

                    XmlElement element(tinyElement);

                    EXPECT_FALSE(element.isAttributesEmpty());
                    ASSERT_THROW(element.getAttribute("baz"), AttributeException); // No attribute named baz
                    EXPECT_FALSE(element.isAttributesEmpty());
                    EXPECT_NE(element.getAttribute("bar"), nullptr);
                    EXPECT_TRUE(element.isAttributesEmpty());
                    ASSERT_THROW(element.getAttribute("bar"), AttributeException); // Already got attribute
                }

                TEST(parser_XmlElement, GetElements)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("foo");

                    tinyElement->InsertEndChild(tinyDocument.NewElement("bar"));
                    tinyElement->InsertEndChild(tinyDocument.NewElement("baz"));
                    tinyElement->InsertEndChild(tinyDocument.NewElement("bar"));

                    XmlElement element(tinyElement);

                    EXPECT_FALSE(element.isElementsEmpty());
                    EXPECT_EQ(element.getElements("bar").size(), 2);
                    EXPECT_FALSE(element.isElementsEmpty());
                    EXPECT_EQ(element.getElements("baz").size(), 1);
                    EXPECT_TRUE(element.isElementsEmpty());
                    EXPECT_EQ(element.getElements("bar").size(), 0);
                    EXPECT_TRUE(element.isElementsEmpty());
                }
            }
        }
    }
}