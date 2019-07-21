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
            m_xml(xml), m_json(json)
        {
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
                if (isPrimitive(variable.getType()) || variable.getType() == "String")
                {
                    rv += variable.getType() + " " +
                        m_xml->getName() + "_" +
                        variable.getName() + "[" +
                        std::to_string(m_json.size()) + "]" + " = {\n";

                    for (nlohmann::json appendage : m_json)
                    {
                        rv += "    " + appendage.at(variable.getName()).dump() + 
                            ((variable.getType() == "float") ? "f" : "") + ",\n";
                    }

                    rv = rv.substr(0, rv.size() - 2) + "\n";

                    rv += "};\n";
                }
                else
                {
                    rv += variable.getType() + " " +
                        m_xml->getName() + "_" +
                        variable.getName() + "[" +
                        std::to_string(m_json.size()) + "]" + " = {\n";

                    for (nlohmann::json appendage : m_json)
                    {
                        std::vector<parser::Parameter> parameters = variable.getParameters();
                        
                        if (parameters.empty())
                        {
                            rv += "    " + variable.getType() + "(),\n";
                        }
                        else
                        {
                            rv += "    " + variable.getType() + "(\n";

                            for (const parser::Parameter& parameter : parameters)
                            {
                                rv += "        " + appendage.at(parameter.getName()).dump() +
                                    ((parameter.getType() == "float") ? "f" : "") + ",\n";
                            }

                            rv = rv.substr(0, rv.size() - 2) + "\n";

                            rv += "    ),\n";
                        }
                    }

                    rv = rv.substr(0, rv.size() - 2) + "\n";

                    rv += "};\n";
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
                block = std::regex_replace(block, std::regex(variable.getName()), 
                    m_xml->getName() + "_" + variable.getName() + append);
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
                    block += "for (uint16_t i = 0; i <= " + std::to_string(m_json.size() - 1) + "u; ++i) {\n";
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
                    block += "for (uint16_t i = 0; i < " + std::to_string(m_json.size()) + "u; ++i) {\n";
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
                block += "for (uint16_t i = 0; i <= " + std::to_string(m_json.size() - 1) + "u; ++i)\n{\n";
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

            rv += getStopFunction(m_xml->getStop()) + "\n";

            return rv;
        }

        bool Appendage::hasStop() const
        {
            return m_xml->getStop() != nullptr;
        }
    }
}
