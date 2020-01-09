#include "EmbGen/EmbGen.hpp"
#include "EmbGen/Exceptions.hpp"
#include "EmbGen/Parser/Appendage.hpp"

#include <nlohmann/json.hpp>
#include <tinyxml2.h>

#include <regex>
#include <set>
#include <fstream>

namespace emb
{
    namespace gen
    {
        std::string EmbGen::generateIncludes() const
        {
            std::string rv = "#include <EmbMessenger/EmbMessenger.hpp>\n\n";

            for (const Appendage& appendage : m_appendages)
            {
                std::string includes = appendage.getIncludes();
                if (!includes.empty())
                {
                    rv += includes + "\n";
                }
            }

            rv = rv.substr(0, rv.size() - 2);

            return rv;
        }

        std::string EmbGen::getCommandCount() const
        {
            size_t commandCount = 0;
            
            for (const Appendage& appendage : m_appendages)
            {
                commandCount += appendage.getCommandNames().size();
            }

            return std::to_string(commandCount + 1) + "u";
        }

        std::string getMessengerName(std::string messengerConstructor)
        {
            std::regex nameRegex(R"(^(\w+)\()");
            std::smatch match;

            if (!std::regex_search(messengerConstructor, match, nameRegex))
            {
                throw InoTemplateException("EMBGEN_VARIABLES comment malformed, no messenger");
            }

            return match.str(1);
        }

        std::string getMessengerBufferName(std::string messengerConstructor)
        {
            std::regex bufferRegex(R"(\((.*?)[,)])");
            std::smatch match;

            if (!std::regex_search(messengerConstructor, match, bufferRegex))
            {
                throw InoTemplateException("EMBGEN_VARIABLES comment malfored, no buffer");
            }

            return match.str(1);
        }

        std::string EmbGen::generateVariables(std::string messengerName) const
        {
            std::string rv = "extern emb::device::EmbMessenger<0> " + messengerName + ";\n\n";

            for (const Appendage& appendage : m_appendages)
            {
                std::string variables = appendage.getVariables();
                if (!variables.empty())
                {
                    rv += variables + "\n";
                }
            }

            rv = rv.substr(0, rv.size() - 2);

            return rv;
        }

        std::string EmbGen::generateSetup() const
        {
            std::string rv = "";
            bool hadSetup = false;

            for (const Appendage& appendage : m_appendages)
            {
                std::string setup = appendage.getSetup();
                if (!setup.empty())
                {
                    hadSetup = true;
                    rv += appendage.getSetup() + "\n";
                }
            }

            rv = rv.substr(0, rv.size() - (hadSetup ? 2 : 0));

            return rv;
        }

        std::string EmbGen::generateLoop(std::string messengerName) const
        {
            std::string rv = "    " + messengerName + ".update();\n";
            bool hadLoop = false;

            for (const Appendage& appendage : m_appendages)
            {
                std::string loop = appendage.getLoop();
                if (!loop.empty())
                {
                    hadLoop = true;
                    rv += loop + "\n";
                }
            }

            rv = rv.substr(0, rv.size() - (hadLoop ? 2 : 1));

            return rv;
        }

        std::string EmbGen::generateCommandFunctions(std::string messengerConstructor) const
        {
            std::string rv = "";

            std::string messengerName = getMessengerName(messengerConstructor);

            for (const Appendage& appendage : m_appendages)
            {
                std::string functions = appendage.getCommandFunctions(messengerName);
                if (!functions.empty())
                {
                    rv += functions + "\n";
                }
            }

            if (!m_appendages.empty())
            {
                rv = rv.substr(0, rv.size() - 1);
            }

            rv += "void all_stop()\n{\n";
            for (const Appendage& appendage : m_appendages)
            {
                if (appendage.hasStop())
                {
                    rv += "    " + appendage.getName() + "_stop();\n";
                }
            }
            rv += "}\n\n";

            rv += "const emb::device::EmbMessenger<>::CommandFunction commands[] PROGMEM = {\n";
            for (const Appendage& appendage : m_appendages)
            {
                for (const std::string& command : appendage.getCommandNames())
                {
                    rv += "    " + command + ",\n";
                }
            }
            rv += "    all_stop\n};\n\n";


            rv += "emb::device::EmbMessenger<0> " + messengerName + "(" + getMessengerBufferName(messengerConstructor) + ", commands, " + getCommandCount() + ");\n";

            return rv;
        }

        std::string xmlEncode(const std::string& str)
        {
            std::string rv = "";

            for (char c : str)
            {
                switch (c)
                {
                case '"':
                    rv += "&quot;";
                    break;
                case '\'':
                    rv += "&apos;";
                    break;
                case '<':
                    rv += "&lt;";
                    break;
                case '>':
                    rv += "&gt;";
                    break;
                case '&':
                    rv += "&amp;";
                    break;
                default:
                    rv += c;
                    break;
                }
            }

            return rv;
        }

        tinyxml2::XMLError loadXmlFile(tinyxml2::XMLDocument& doc, const std::string& filename, const std::vector<std::string>& elements_to_escape)
        {
            std::string escaped, expression, suffix;

            std::ifstream file_stream(filename);
            std::string file((std::istreambuf_iterator<char>(file_stream)),
                std::istreambuf_iterator<char>());

            for (std::string element : elements_to_escape)
            {
                // https://regex101.com/r/ECLQuZ/1
                expression = "(<(?:" + element + ").*>)((?:[\\r\\n]|.)*?)(<\\/(?:" + element + ").*>)";
                std::regex reg(expression);
                std::sregex_iterator re_it(file.begin(), file.end(), reg), re_it_end;

                // Make sure there is a match
                if (re_it != re_it_end)
                {
                    escaped = "";

                    // Iterate through the matches
                    for (; re_it != re_it_end; ++re_it)
                    {
                        escaped += re_it->prefix().str();    // Everything before this match
                        escaped += re_it->str(1);            // Element Start Tag
                        escaped += xmlEncode(re_it->str(2)); // Element Contents
                        escaped += re_it->str(3);            // Element End Tag
                        suffix = re_it->suffix().str();      // Everything after this match
                    }
                    escaped += suffix;

                    file = escaped;
                }
            }

            return doc.Parse(file.c_str());;
        }
        
        EmbGen::EmbGen(std::string config, std::string appendage_folder, std::string inoTemplate) :
            m_inoTemplate(inoTemplate)
        {
            nlohmann::json jsonConfig = nlohmann::json::parse(config);

            for (auto& appendageType : jsonConfig)
            {
                int i = 0;
                for (auto& appendage : appendageType)
                {
                    appendage["typeIndex"] = i++;
                }
            }

            m_appendageJson = jsonConfig;

            for (auto& appendageType : jsonConfig.items())
            {
                tinyxml2::XMLDocument tinyDocument;
                if (loadXmlFile(tinyDocument, appendage_folder + "/" + appendageType.key() + ".xml", { "code" }) != tinyxml2::XMLError::XML_SUCCESS)
                {
                    throw XmlParseException(tinyDocument.ErrorStr());
                }
                m_appendages.emplace_back(
                    std::make_shared<parser::Appendage>(tinyDocument.FirstChildElement("appendage")),
                    jsonConfig
                );
            }
        }

        std::string getCommentValue(std::string inoTemplate, std::string commentName)
        {
            std::regex commentRegex(R"([ \t]*(?:\/\*+\s*)EMBGEN(?:_|-))" + commentName + R"(: *([^\n]*?)\s*(?:\*+\/)[ \t]*)");
            std::smatch match;

            if (!std::regex_search(inoTemplate, match, commentRegex))
            {
                throw InoTemplateException("EMBGEN_" + commentName + " comment not found or malformed");
            }

            return match.str(1);
        }

        std::string replaceComment(const std::string& inoTemplate, const std::string& commentName, const std::string& value)
        {
            return std::regex_replace(inoTemplate,
                std::regex(R"([ \t]*(?:\/\*+\s*)EMBGEN(?:_|-))" + commentName + R"((?:[^\n]*?\*+\/)[ \t]*)"),
                value);
        }

        std::string EmbGen::generateSource() const
        {
            std::string messengerConstructor = getCommentValue(m_inoTemplate, "VARIABLES");
            std::string messengerName = getMessengerName(messengerConstructor);
            std::string ino = m_inoTemplate;

            ino = replaceComment(ino, "INCLUDES", generateIncludes());
            ino = replaceComment(ino, "VARIABLES", generateVariables(messengerName));
            ino = replaceComment(ino, "SETUP", generateSetup());
            ino = replaceComment(ino, "LOOP", generateLoop(messengerName));
            ino = replaceComment(ino, "COMMANDS", generateCommandFunctions(messengerConstructor));

            return ino;
        }

        std::string EmbGen::generateCore() const
        {
            nlohmann::json core;

            core["appendages"] = nlohmann::json::object();
            for (const Appendage& type : m_appendages)
            {
                uint16_t typeIndex = 0;
                core["appendages"][type.getName()] = nlohmann::json::object();
                for (const auto& name : m_appendageJson[type.getName()].items())
                {
                    core["appendages"][type.getName()][name.key()]["typeIndex"] = typeIndex++;
                    for (const parser::Variable& variable : type.getXml()->getVariables())
                    {
                        if (variable.isCore())
                        {
                            core["appendages"][type.getName()][name.key()][variable.getName()] = name.value()[variable.getName()];
                        }

                        for (const parser::Parameter& parameter : variable.getParameters())
                        {
                            if (parameter.isCore())
                            {
                                core["appendages"][type.getName()][name.key()][parameter.getName()] = name.value()[parameter.getName()];
                            }
                        }
                    }
                }
            }

            uint16_t commands = 0;
            for (const Appendage& appendage : m_appendages)
            {
                for (const std::string& commandName : appendage.getCommandNames())
                {
                    core["commands"][commandName] = commands++;
                }
            }
            core["commands"]["all_stop"] = commands++;

            return core.dump(4);
        }

        std::vector<std::string> EmbGen::getAppendageNames() const
        {
            std::vector<std::string> rv;

            for (const Appendage& appendage : m_appendages)
            {
                rv.emplace_back(appendage.getName());
            }

            return rv;
        }
    }
}