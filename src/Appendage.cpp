#include "EmbGen/Appendage.hpp"
#include "EmbGen/Utils.hpp"
#include <EmbGen/Parser/Appendage.hpp>
#include <set>
#include <sstream>
#include <regex>

namespace emb
{
    namespace gen
    {
        Appendage::Appendage(std::shared_ptr<parser::Appendage> xml, const nlohmann::json& json) :
            m_xml(xml), m_appendages(json), m_json(json[xml->getName()])
        {
            std::string str = json.dump();
        }

        std::string Appendage::getName() const
        {
            return m_xml->getName();
        }

        std::string Appendage::getIncludes() const
        {
            std::string rv = "";
            for (parser::Include& include : m_xml->getIncludes())
            {
                rv += "#include ";
                if (include.isStandard())
                {
                    rv += "<" + include.getValue() + ">";
                }
                else
                {
                    rv += "\"" + include.getValue() + "\"";
                }
                rv += "\n";
            }
            return rv;
        }

        bool isPrimitive(std::string type)
        {
            static std::set<std::string> types = {
                "char",
                "signed char",
                "unsigned char",
                "short",
                "short int",
                "signed short",
                "signed short int",
                "unsigned short",
                "unsigned short int",
                "int",
                "signed",
                "signed int",
                "unsigned",
                "unsigned int",
                "long",
                "long int",
                "signed long",
                "signed long int",
                "unsigned long",
                "unsigned long int",
                "long long",
                "long long int",
                "signed long long",
                "signed long long int",
                "unsigned long long",
                "unsigned long long int",
                "float",
                "double",
                "long double",
                "int8_t",
                "int16_t",
                "int32_t",
                "int64_t",
                "uint8_t",
                "uint16_t",
                "uint32_t",
                "uint64_t"
            };

            return types.find(type) != types.end();
        }

        std::string Appendage::getVariables() const
        {
            std::string rv = "";

            for (const parser::Variable& variable : m_xml->getVariables())
            {
                std::string otherAppendage = variable.getAppendage();
                std::string variableType = variable.getType();
                if ((isPrimitive(variableType) || variableType == "String") && otherAppendage.empty())
                {
                    rv += variableType + " " +
                        m_xml->getName() + "_" +
                        variable.getName() + "[" +
                        std::to_string(m_json.size()) + "]" + " = {\n";

                    for (nlohmann::json appendage : m_json)
                    {
                        std::string value;
                        try
                        {
                            value = appendage.at(variable.getName()).dump();
                        }
                        catch (nlohmann::json::out_of_range)
                        {
                            value = variable.getDefault();
                            if (value == "")
                            {
                                throw;
                            }
                        }
                        rv += "    " + value + ((variableType == "float") ? "f" : "") + ",\n";
                    }

                    rv = rv.substr(0, rv.size() - 2) + "\n";

                    rv += "};\n";
                }
                else if (!otherAppendage.empty())
                {
                    rv += "uint16_t " +
                        m_xml->getName() + "_" +
                        variable.getName() + "[" +
                        std::to_string(m_json.size()) + "]" + " = {\n";

                    for (auto& appendage : m_json.items())
                    {
                        std::string otherType = variable.getAppendage();
                        std::string variableName = variable.getName();
                        std::string otherAppendageName = appendage.value()[variableName].get<std::string>();
                        rv += "    " + m_appendages[otherType][otherAppendageName]["typeIndex"].dump() + "u,\n";
                    }

                    rv = rv.substr(0, rv.size() - 2) + "\n";

                    rv += "};\n";
                }
                else
                {
                    std::map<std::string, std::string> appendageIndexes;

                    std::string type = variableType + " " +
                        m_xml->getName() + "_" +
                        variable.getName() + "[" +
                        std::to_string(m_json.size()) + "]" + " = {\n";

                    int i = 0;
                    for (auto& appendage : m_json.items())
                    {
                        std::vector<parser::Parameter> parameters = variable.getParameters();
                        
                        if (parameters.empty())
                        {
                            type += "    " + variableType + "(),\n";
                        }
                        else
                        {
                            type += "    " + variableType + "(\n";

                            for (int j = 0; j < parameters.size(); ++j)
                            {
                                const parser::Parameter& parameter = parameters[j];

                                if (!parameter.getAppendage().empty())
                                {
                                    std::string otherType = parameter.getAppendage();
                                    std::string parameterName = parameter.getName();
                                    std::string otherAppendageName = appendage.value()[parameterName].get<std::string>();

                                    appendageIndexes[parameter.getName()] += "    " + m_appendages[otherType][otherAppendageName]["typeIndex"].dump() + "u,\n";

                                    char typeMod = parameter.getType().back();

                                    type += std::string("        ") + ((typeMod == '*' || typeMod == '&') ? "&" : "") + otherType + "_" +
                                        parameterName + "[" + m_xml->getName() + "_" + parameterName + "[" + std::to_string(i) + "]],\n";
                                }
                                else
                                {
                                    std::string value;
                                    try
                                    {
                                        value = appendage.value().at(parameter.getName()).dump();
                                    }
                                    catch (nlohmann::json::out_of_range)
                                    {
                                        value = parameter.getDefault();
                                        if (value == "")
                                        {
                                            throw;
                                        }
                                    }
                                    type += "        " + value + ((parameter.getType() == "float") ? "f" : "") + ",\n";
                                }
                            }

                            type = type.substr(0, type.size() - 2) + "\n";

                            type += "    ),\n";
                        }
                        i++;
                    }

                    type = type.substr(0, type.size() - 2) + "\n";

                    type += "};\n";

                    for (const parser::Parameter& parameter : variable.getParameters())
                    {
                        if (!parameter.getAppendage().empty())
                        {
                            rv += "uint16_t " +
                                m_xml->getName() + "_" +
                                parameter.getName() + "[" +
                                std::to_string(m_json.size()) + "]" + " = {\n";

                            rv += appendageIndexes[parameter.getName()];

                            rv = rv.substr(0, rv.size() - 2) + "\n";

                            rv += "};\n";
                        }
                    }

                    rv += type;
                }
            }

            return rv;
        }

        std::string indent(const std::string& block)
        {
            std::istringstream iss(block);
            std::ostringstream oss("");

            std::string line;
            while (std::getline(iss, line))
            {
                oss << "    " << line << "\n";
            }

            return oss.str();
        }

        std::string Appendage::replaceVariables(std::string block, std::string append) const
        {
            for (const parser::Variable& variable : m_xml->getVariables())
            {
                std::string replaceStr;
                if (variable.getAppendage().empty())
                {
                    replaceStr = m_xml->getName() + "_" + variable.getName() + append;
                }
                else
                {
                    replaceStr = variable.getAppendage() + "_" + variable.getName() + "[" +
                        m_xml->getName() + "_" + variable.getName() + "[i]]";
                }

                block = std::regex_replace(block, std::regex(variable.getName()), replaceStr);
            }
            return block;
        }

        std::string Appendage::getSetup() const
        {
            std::string rv = "";

            const std::shared_ptr<parser::Setup> setup = m_xml->getSetup();

            if (setup == nullptr)
            {
                return rv;
            }

            for (const parser::Code& code : setup->getCode())
            {
                if (code.getInsert() == parser::Code::Insert::Each)
                {
                    std::string block = "";
                    block += "for (uint16_t i = 0u; i <= " + std::to_string(m_json.size() - 1) + "u; ++i) {\n";
                    block += replaceVariables(code.getText(), "[i]");
                    block += "\n}\n";
                    rv += indent(block);
                }
                else
                {
                    rv += replaceVariables(code.getText()) + "\n";
                }
            }

            return rv;
        }

        std::string Appendage::getLoop() const
        {
            std::string rv = "";

            const std::shared_ptr<parser::Loop> loop = m_xml->getLoop();
            
            if (loop == nullptr)
            {
                return rv;
            }

            for (const parser::Code& code : loop->getCode())
            {
                if (code.getInsert() == parser::Code::Insert::Each)
                {
                    std::string block = "";
                    block += "for (uint16_t i = 0u; i <= " + std::to_string(m_json.size() - 1) + "u; ++i) {\n";
                    block += replaceVariables(code.getText(), "[i]");
                    block += "\n}\n";
                    rv += indent(block);
                }
                else
                {
                    rv += replaceVariables(code.getText()) + "\n";
                }
            }

            return rv;
        }

        std::vector<std::string> Appendage::getCommandNames() const
        {
            std::vector<std::string> commandNames;
            for (std::string& key : getMapKeys(m_xml->getCommands()))
            {
                commandNames.emplace_back(m_xml->getName() + "_" + key + "_adaptor");
            }

            if (m_xml->getStop() != nullptr)
            {
                commandNames.emplace_back(m_xml->getName() + "_stop");
            }

            return commandNames;
        }

        std::string Appendage::getAdaptorCode(std::string messengerName, std::shared_ptr<parser::Command> command) const
        {
            std::string rv = "";
            std::vector<parser::Parameter> parameters = command->getParameters();
            std::vector<parser::ReturnValue> returnValues = command->getReturnValues();

            rv += "uint16_t i;\n";

            for (const parser::Parameter& parameter : parameters)
            {
                rv += parameter.getType() + " " + parameter.getName() + ";\n";
            }

            for (const parser::ReturnValue& returnValue : returnValues)
            {
                rv += returnValue.getType() + " " + returnValue.getName() + ";\n";
            }

            rv += messengerName + ".read(\n";
            rv += "    i, [](uint16_t i){ return i <= " + std::to_string(m_json.size() - 1) + "u; },\n";
            for (const parser::Parameter& parameter : parameters)
            {
                std::string pName = parameter.getName();
                rv += "    " + pName + ",";
                std::string min = parameter.getMin();
                std::string max = parameter.getMax();
                if (!min.empty() || !max.empty())
                {
                    rv += " [](" + parameter.getType() + " " + pName + "){ return ";
                    if (!min.empty())
                    {
                        rv += pName + " >= " + min;
                        if (!max.empty())
                        {
                            rv += " && ";
                        }
                    }
                    if (!max.empty())
                    {
                        rv += pName + " <= " + max;
                    }

                    rv += "; },";
                }
                rv += "\n";
            }
            rv = rv.substr(0, rv.size() - 2) + "\n";
            rv += ");\n";

            rv += m_xml->getName() + "_" + command->getName() + "(\n";
            rv += "    i,\n";
            for (const parser::Parameter& parameter : parameters)
            {
                rv += "    " + parameter.getName() + ",\n";
            }
            for (const parser::ReturnValue& returnValue : returnValues)
            {
                rv += "    " + returnValue.getName() + ",\n";
            }
            rv = rv.substr(0, rv.size() - 2) + "\n";
            rv += ");\n";

            if (!returnValues.empty())
            {
                rv += messengerName + ".write(\n";
                for (const parser::ReturnValue& returnValue : returnValues)
                {
                    rv += "    " + returnValue.getName() + ",\n";
                }
                rv = rv.substr(0, rv.size() - 2) + "\n";
                rv += ");\n";
            }

            return indent(rv);
        }

        std::string Appendage::getAdaptorFunction(std::string messengerName, std::shared_ptr<parser::Command> command) const
        {
            std::string rv = "";

            rv += "void " + m_xml->getName() + "_" + command->getName() + "_adaptor()\n{\n";
            rv += getAdaptorCode(messengerName, command);
            rv += "}\n";

            return rv;
        }

        std::string Appendage::getCommandCode(std::shared_ptr<parser::Command> command) const
        {
            return replaceVariables(command->getCode()->getText(), "[i]") + "\n";
        }

        std::string Appendage::getCommandFunction(std::shared_ptr<parser::Command> command) const
        {
            std::string rv = "";
            std::vector<parser::Parameter> parameters = command->getParameters();
            std::vector<parser::ReturnValue> returnValues = command->getReturnValues();

            rv += "void " + m_xml->getName() + "_" + command->getName() + "(";
            rv += "uint16_t i, ";
            for (const parser::Parameter& parameter : parameters)
            {
                rv += parameter.getType() + " " + parameter.getName() + ", ";
            }
            for (const parser::ReturnValue& returnValue : returnValues)
            {
                rv += returnValue.getType() + "& " + returnValue.getName() + ", ";
            }
            rv = rv.substr(0, rv.size() - 2);
            rv += ")\n{\n";
            rv += getCommandCode(command);
            rv += "}\n";

            return rv;
        }

        std::string Appendage::getStopFunction(std::shared_ptr<parser::Stop> stop) const
        {
            std::string rv = "";

            if (stop != nullptr)
            {
                rv += "void " + m_xml->getName() + "_stop()\n{\n";
                std::string block = "";
                std::shared_ptr<parser::Code> code = stop->getCode();
                block += "for (uint16_t i = 0u; i <= " + std::to_string(m_json.size() - 1) + "u; ++i) {\n";
                if (code != nullptr)
                {
                    block += replaceVariables(code->getText(), "[i]") + "\n";
                }
                else
                {
                    block += "    " + m_xml->getName() + "_" + stop->getCommand() + "(i);\n";
                }
                block += "}\n";
                rv += indent(block);
                rv += "}\n";
            }

            return rv;
        }

        std::string Appendage::getCommandFunctions(std::string messengerName) const
        {
            std::string rv = "";

            for (auto command : m_xml->getCommands())
            {
                rv += getAdaptorFunction(messengerName, command.second) + "\n";
                rv += getCommandFunction(command.second) + "\n";
            }

            std::shared_ptr<parser::Stop> stopCommand = m_xml->getStop();
            if (stopCommand != nullptr)
            {
                rv += getStopFunction(stopCommand) + "\n";
            }

            return rv;
        }

        std::shared_ptr<parser::Appendage> Appendage::getXml() const
        {
            return m_xml;
        }

        bool Appendage::hasStop() const
        {
            return m_xml->getStop() != nullptr;
        }
    }
}
