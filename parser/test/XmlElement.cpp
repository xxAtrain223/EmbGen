#include <gtest/gtest.h>
#include <tinyxml2.h>
#include "EmbGen/Parser/XmlElement.hpp"
#include "EmbGen/Parser/Exceptions.hpp"

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
                    ASSERT_EQ(tinyDocument.Parse(
                        "<foo />\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("foo");
                    ASSERT_NE(tinyElement, nullptr);

                    XmlElement element(tinyElement);

                    ASSERT_EQ(element.getName(), "foo");
                }

                TEST(parser_XmlElement, GetText)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<foo>bar</foo>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("foo");
                    ASSERT_NE(tinyElement, nullptr);

                    XmlElement element(tinyElement);

                    ASSERT_EQ(element.getText(), "bar");
                }

                TEST(parser_XmlElement, GetLineNum)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<foo>\n"
                        "    <bar />\n"
                        "</foo>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("foo")->FirstChildElement("bar");
                    ASSERT_NE(tinyElement, nullptr);

                    XmlElement element(tinyElement);

                    ASSERT_EQ(element.getLineNum(), 2);
                }

                TEST(parser_XmlElement, GetAttribute)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<foo bar='bar' baz='baz' />\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("foo");
                    ASSERT_NE(tinyElement, nullptr);

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
                    ASSERT_EQ(tinyDocument.Parse(
                        "<foo bar='bar' />\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("foo");
                    ASSERT_NE(tinyElement, nullptr);

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
                    ASSERT_EQ(tinyDocument.Parse(
                        "<foo>\n"
                        "    <bar />\n"
                        "    <baz />\n"
                        "    <bar />\n"
                        "</foo>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("foo");
                    ASSERT_NE(tinyElement, nullptr);

                    XmlElement element(tinyElement);

                    EXPECT_FALSE(element.isElementsEmpty());
                    EXPECT_EQ(element.getElements("bar").size(), 2);
                    EXPECT_FALSE(element.isElementsEmpty());
                    EXPECT_EQ(element.getElements("baz").size(), 1);
                    EXPECT_TRUE(element.isElementsEmpty());
                    EXPECT_EQ(element.getElements("bar").size(), 0);
                    EXPECT_TRUE(element.isElementsEmpty());
                }

                TEST(parser_XmlElement, GetText_ElementException)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<foo />\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("foo");
                    ASSERT_NE(tinyElement, nullptr);

                    XmlElement element(tinyElement);

                    ASSERT_THROW(element.getText(), ElementException);
                }
            }
        }
    }
}