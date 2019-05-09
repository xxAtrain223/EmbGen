#include <gtest/gtest.h>
#include <tinyxml2.h>
#include "EmbGen/Stop.hpp"
#include "EmbGen/Exceptions.hpp"
#include "EmbGen/Code.hpp"

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
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("stop");

                    tinyElement->SetAttribute("command", "detach");

                    Stop stop(tinyElement);

                    ASSERT_EQ(stop.getCommand(), "detach");
                    ASSERT_EQ(stop.getCode(), nullptr);
                }

                TEST(parser_Stop, Code)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("stop");

                    tinyxml2::XMLElement* tinyCode = tinyDocument.NewElement("code");
                    tinyCode->SetText("// Code");
                    tinyElement->InsertEndChild(tinyCode);

                    Stop stop(tinyElement);

                    ASSERT_EQ(stop.getCommand(), "");
                    ASSERT_EQ(stop.getCode()->getText(), "    // Code");
                }

                TEST(parser_Stop, CommandAndCode)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("stop");

                    tinyxml2::XMLElement* tinyCode = tinyDocument.NewElement("code");
                    tinyCode->SetText("// Code");

                    tinyElement->SetAttribute("command", "detach");
                    tinyElement->InsertEndChild(tinyCode);

                    ASSERT_THROW(Stop stop(tinyElement), BaseException); // Not sure if this should throw Attribute or Element Exception
                }

                TEST(parser_Stop, NoCommandOrCode)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("stop");

                    ASSERT_THROW(Stop stop(tinyElement), BaseException); // Not sure if this should throw Attribute or Element Exception
                }

                TEST(parser_Stop, ExtraAttribute)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("stop");

                    tinyElement->SetAttribute("command", "detach");
                    tinyElement->SetAttribute("extra", "attribute");

                    ASSERT_THROW(parser::Stop stop(tinyElement), AttributeException);
                }

                TEST(parser_Stop, ExtraElements)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    tinyxml2::XMLElement* tinyElement = tinyDocument.NewElement("stop");

                    tinyElement->SetAttribute("command", "detach");

                    tinyElement->InsertEndChild(tinyDocument.NewElement("extra-element"));

                    ASSERT_THROW(parser::Stop stop(tinyElement), ElementException);
                }
            }
        }
    }
}