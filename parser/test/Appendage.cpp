#include <gtest/gtest.h>
#include <tinyxml2.h>
#include "EmbGen/Parser/Appendage.hpp"
#include "EmbGen/Parser/Exceptions.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            namespace test
            {
                TEST(parser_Appendage, NoElements)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<appendage name='NoElements'>\n"
                        "</appendage>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("appendage");
                    ASSERT_NE(tinyElement, nullptr);

                    Appendage appendage(tinyElement);

                    ASSERT_EQ(appendage.getName(), "NoElements");
                    ASSERT_EQ(appendage.getVersion(), "");
                    ASSERT_EQ(appendage.getLibDeps(), "");
                    ASSERT_EQ(appendage.getIncludes().size(), 0);
                    ASSERT_EQ(appendage.getVariables().size(), 0);
                    ASSERT_EQ(appendage.getSetup(), nullptr);
                    ASSERT_EQ(appendage.getLoop(), nullptr);
                    ASSERT_EQ(appendage.getCommands().size(), 0);
                }
                
                TEST(parser_Appendage, Include)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<appendage name='Include' lib_deps='Wire'>\n"
                        "    <include standard='true'>Wire.h</include>\n"
                        "</appendage>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("appendage");
                    ASSERT_NE(tinyElement, nullptr);

                    Appendage appendage(tinyElement);

                    ASSERT_EQ(appendage.getName(), "Include");
                    ASSERT_EQ(appendage.getVersion(), "");
                    ASSERT_EQ(appendage.getLibDeps(), "Wire");

                    std::vector<Include> includes = appendage.getIncludes();
                    ASSERT_EQ(includes.size(), 1);
                    ASSERT_EQ(includes.at(0).getValue(), "Wire.h");
                    ASSERT_EQ(includes.at(0).isStandard(), true);

                    ASSERT_EQ(appendage.getVariables().size(), 0);
                    ASSERT_EQ(appendage.getSetup(), nullptr);
                    ASSERT_EQ(appendage.getLoop(), nullptr);
                    ASSERT_EQ(appendage.getCommands().size(), 0);
                }

                TEST(parser_Appendage, Variable)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<appendage name='Variable' version='1.0'>\n"
                        "    <variable type='uint8_t' name='pin' />\n"
                        "</appendage>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("appendage");
                    ASSERT_NE(tinyElement, nullptr);

                    Appendage appendage(tinyElement);

                    ASSERT_EQ(appendage.getName(), "Variable");
                    ASSERT_EQ(appendage.getVersion(), "1.0");
                    ASSERT_EQ(appendage.getLibDeps(), "");
                    ASSERT_EQ(appendage.getIncludes().size(), 0);

                    std::vector<Variable> variables = appendage.getVariables();
                    ASSERT_EQ(variables.size(), 1);
                    ASSERT_EQ(variables.at(0).getType(), "uint8_t");
                    ASSERT_EQ(variables.at(0).getName(), "pin");

                    ASSERT_EQ(appendage.getSetup(), nullptr);
                    ASSERT_EQ(appendage.getLoop(), nullptr);
                    ASSERT_EQ(appendage.getCommands().size(), 0);
                }

                TEST(parser_Appendage, Setup)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<appendage name='Setup'>\n"
                        "    <setup>\n"
                        "        <code insert='each'>\n"
                        "            // Setup Code Each\n"
                        "        </code>\n"
                        "        <code insert='once'>\n"
                        "            // Setup Code Once\n"
                        "        </code>\n"
                        "    </setup>\n"
                        "</appendage>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("appendage");
                    ASSERT_NE(tinyElement, nullptr);
                    
                    Appendage appendage(tinyElement);

                    ASSERT_EQ(appendage.getName(), "Setup");
                    ASSERT_EQ(appendage.getVersion(), "");
                    ASSERT_EQ(appendage.getLibDeps(), "");
                    ASSERT_EQ(appendage.getIncludes().size(), 0);
                    ASSERT_EQ(appendage.getVariables().size(), 0);

                    std::shared_ptr<parser::Setup> setup = appendage.getSetup();
                    ASSERT_NE(setup, nullptr);
                    std::vector<Code> codes = setup->getCode();
                    ASSERT_EQ(codes.size(), 2);
                    ASSERT_EQ(codes.at(0).getInsert(), Code::Insert::Each);
                    ASSERT_EQ(codes.at(0).getText(), "    // Setup Code Each");
                    ASSERT_EQ(codes.at(1).getInsert(), Code::Insert::Once);
                    ASSERT_EQ(codes.at(1).getText(), "    // Setup Code Once");

                    ASSERT_EQ(appendage.getLoop(), nullptr);
                    ASSERT_EQ(appendage.getCommands().size(), 0);
                }

                TEST(parser_Appendage, Loop)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<appendage name='Loop'>\n"
                        "    <loop>\n"
                        "        <code insert='once'>\n"
                        "            // Loop Code Once\n"
                        "        </code>\n"
                        "        <code insert='each'>\n"
                        "            // Loop Code Each\n"
                        "        </code>\n"
                        "    </loop>\n"
                        "</appendage>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("appendage");
                    ASSERT_NE(tinyElement, nullptr);

                    Appendage appendage(tinyElement);

                    ASSERT_EQ(appendage.getName(), "Loop");
                    ASSERT_EQ(appendage.getVersion(), "");
                    ASSERT_EQ(appendage.getLibDeps(), "");
                    ASSERT_EQ(appendage.getIncludes().size(), 0);
                    ASSERT_EQ(appendage.getVariables().size(), 0);
                    ASSERT_EQ(appendage.getSetup(), nullptr);
                    
                    std::shared_ptr<Loop> loop = appendage.getLoop();
                    ASSERT_NE(loop, nullptr);
                    std::vector<Code> codes = loop->getCode();
                    ASSERT_EQ(codes.size(), 2);
                    ASSERT_EQ(codes.at(0).getInsert(), Code::Insert::Once);
                    ASSERT_EQ(codes.at(0).getText(), "    // Loop Code Once");
                    ASSERT_EQ(codes.at(1).getInsert(), Code::Insert::Each);
                    ASSERT_EQ(codes.at(1).getText(), "    // Loop Code Each");

                    ASSERT_EQ(appendage.getCommands().size(), 0);
                }

                TEST(parser_Appendage, Command)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<appendage name='Command'>\n"
                        "    <command name='SetValue'>\n"
                        "        <parameter type='int16_t' name='val' />\n"
                        "        <code>\n"
                        "            value = val;\n"
                        "        </code>\n"
                        "    </command>\n"
                        "</appendage>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("appendage");
                    ASSERT_NE(tinyElement, nullptr);

                    Appendage appendage(tinyElement);

                    ASSERT_EQ(appendage.getName(), "Command");
                    ASSERT_EQ(appendage.getVersion(), "");
                    ASSERT_EQ(appendage.getLibDeps(), "");
                    ASSERT_EQ(appendage.getIncludes().size(), 0);
                    ASSERT_EQ(appendage.getVariables().size(), 0);
                    ASSERT_EQ(appendage.getSetup(), nullptr);
                    ASSERT_EQ(appendage.getLoop(), nullptr);

                    std::map<std::string, std::shared_ptr<Command>> commands = appendage.getCommands();
                    ASSERT_EQ(commands.size(), 1);
                    std::shared_ptr<Command> SetValue = commands.at("SetValue");
                    ASSERT_EQ(SetValue->getName(), "SetValue");
                    std::vector<Parameter> parameters = SetValue->getParameters();
                    ASSERT_EQ(parameters.size(), 1);
                    ASSERT_EQ(parameters.at(0).getType(), "int16_t");
                    ASSERT_EQ(parameters.at(0).getName(), "val");
                    ASSERT_EQ(SetValue->getCode()->getText(), "    value = val;");
                }

                TEST(parser_Appendage, StopWithCode)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<appendage name='StopWithCode'>\n"
                        "    <stop>\n"
                        "        <code>\n"
                        "            servo.detach();\n"
                        "        </code>\n"
                        "    </stop>\n"
                        "</appendage>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("appendage");
                    ASSERT_NE(tinyElement, nullptr);

                    Appendage appendage(tinyElement);

                    ASSERT_EQ(appendage.getName(), "StopWithCode");
                    ASSERT_EQ(appendage.getVersion(), "");
                    ASSERT_EQ(appendage.getLibDeps(), "");
                    ASSERT_EQ(appendage.getIncludes().size(), 0);
                    ASSERT_EQ(appendage.getVariables().size(), 0);
                    ASSERT_EQ(appendage.getSetup(), nullptr);
                    ASSERT_EQ(appendage.getLoop(), nullptr);
                    
                    std::shared_ptr<Stop> stop = appendage.getStop();
                    ASSERT_NE(stop, nullptr);
                    ASSERT_EQ(stop->getCode()->getText(), "    servo.detach();");
                    ASSERT_EQ(stop->getCommand(), "");
                }

                TEST(parser_Appendage, StopCommandName)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<appendage name='StopCommandName'>\n"
                        "    <command name='Detach'>\n"
                        "        <code>\n"
                        "            servo.detach();\n"
                        "        </code>\n"
                        "    </command>\n"
                        "    <stop command='Detach' />\n"
                        "</appendage>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("appendage");
                    ASSERT_NE(tinyElement, nullptr);

                    Appendage appendage(tinyElement);

                    ASSERT_EQ(appendage.getName(), "StopCommandName");
                    ASSERT_EQ(appendage.getVersion(), "");
                    ASSERT_EQ(appendage.getLibDeps(), "");
                    ASSERT_EQ(appendage.getIncludes().size(), 0);
                    ASSERT_EQ(appendage.getVariables().size(), 0);
                    ASSERT_EQ(appendage.getSetup(), nullptr);
                    ASSERT_EQ(appendage.getLoop(), nullptr);

                    std::shared_ptr<Stop> stop = appendage.getStop();
                    ASSERT_NE(stop, nullptr);
                    ASSERT_EQ(stop->getCode(), nullptr);
                    ASSERT_EQ(stop->getCommand(), "Detach");
                }

                TEST(parser_Appendage, NoName)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<appendage>\n"
                        "</appendage>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("appendage");
                    ASSERT_NE(tinyElement, nullptr);

                    ASSERT_THROW(Appendage appendage(tinyElement), AttributeException);
                }

                TEST(parser_Appendage, ExtraAttribute)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<appendage name='ExtraAttribute' extra='attribute'>\n"
                        "</appendage>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("appendage");
                    ASSERT_NE(tinyElement, nullptr);

                    ASSERT_THROW(Appendage appendage(tinyElement), AttributeException);
                }

                TEST(parser_Appendage, ExtraElement)
                {
                    tinyxml2::XMLDocument tinyDocument;
                    ASSERT_EQ(tinyDocument.Parse(
                        "<appendage name='ExtraElement'>\n"
                        "    <extra-element />\n"
                        "</appendage>\n"
                    ), tinyxml2::XMLError::XML_SUCCESS);
                    tinyxml2::XMLElement* tinyElement = tinyDocument.FirstChildElement("appendage");
                    ASSERT_NE(tinyElement, nullptr);

                    ASSERT_THROW(Appendage appendage(tinyElement), ElementException);
                }
            }
        }
    }
}