#include "EmbGen/EmbGen.hpp"
#include "EmbGen/Exceptions.hpp"
#include "EmbGen/Parser/Appendage.hpp"

#include <nlohmann/json.hpp>
#include <tinyxml2.h>

#include <regex>
#include <set>

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
                    rv += appendage.getIncludes() + "\n";
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

        std::string EmbGen::generateVariables(std::string messengerConstructor) const
        {
            std::string rv = "emb::device::EmbMessenger<" + getCommandCount() + ", 0> " + messengerConstructor + ";\n\n";

            for (const Appendage& appendage : m_appendages)
            {
                rv += appendage.getVariables() + "\n";
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

        std::string EmbGen::generateCommandRegisters(std::string messengerName, std::string registerCall, int indentWidth) const
        {
            std::regex registerRegex(R"((.*)registerCommand\(\)(.*))");
            std::smatch match;
            std::regex_search(registerCall, match, registerRegex);

            std::string rv = "";
            uint16_t count = 0;

            std::string indent = "";
            for (int i = 0; i < indentWidth; ++i)
            {
                indent += " ";
            }

            bool hasCommands = false;

            for (const Appendage& appendage : m_appendages)
            {
                for (const std::string& commandName : appendage.getCommandNames())
                {
                    rv += indent + match.str(1) + messengerName + ".registerCommand(" + std::to_string(count++) + "u, " + commandName + ")" + match.str(2) + "\n";
                }
            }

            rv += indent + match.str(1) + messengerName + ".registerCommand(" + std::to_string(count++) + "u, all_stop)" + match.str(2);

            return rv;
        }

        std::string EmbGen::generateCommandFunctions(std::string messengerName) const
        {
            std::string rv = "";

            for (const Appendage& appendage : m_appendages)
            {
                rv += appendage.getCommandFunctions(messengerName) + "\n";
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
            rv += "}\n";

            return rv;
        }

        EmbGen::EmbGen(std::string config, std::string appendage_folder, std::string inoTemplate) :
            m_inoTemplate(inoTemplate)
        {
            nlohmann::json jsonConfig = nlohmann::json::parse(config);

            std::map<std::string, uint16_t> appendageIndexes;
            nlohmann::json appendageTypes;
            for (nlohmann::json appendage : jsonConfig)
            {
                std::string appendageType = appendage.at("type").get<std::string>();
                uint16_t index;
                try
                {
                    index = appendageIndexes.at(appendageType);
                }
                catch (std::out_of_range)
                {
                    index = 0;
                }
                appendage["typeIndex"] = index;
                appendageIndexes[appendageType] = index + 1;
                appendageTypes[appendageType][appendage.at("label").get<std::string>()] = appendage;
            }

            for (auto& appendageType : appendageTypes.items())
            {
                tinyxml2::XMLDocument tinyDocument;
                if (tinyDocument.LoadFile((appendage_folder + "/" + appendageType.key() + ".xml").c_str()) != tinyxml2::XMLError::XML_SUCCESS)
                {
                    throw XmlParseException(tinyDocument.ErrorStr());
                }
                m_appendages.emplace_back(
                    std::make_shared<parser::Appendage>(tinyDocument.FirstChildElement("appendage")),
                    appendageTypes
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

        std::string getMessengerName(std::string messengerConstructor)
        {
            std::regex nameRegex(R"(^(\w+)\()");
            std::smatch match;
            
            if (!std::regex_search(messengerConstructor, match, nameRegex))
            {
                throw InoTemplateException("EMBGEN_VARIABLES comment malformed");
            }

            return match.str(1);
        }

        std::string replaceComment(const std::string& inoTemplate, const std::string& commentName, const std::string& value)
        {
            return std::regex_replace(inoTemplate,
                std::regex(R"([ \t]*(?:\/\*+\s*)EMBGEN(?:_|-))" + commentName + R"((?:[^\n]*?\*+\/)[ \t]*)"),
                value);
        }

        int getRegisterIndent(const std::string& inoTemplate)
        {
            std::regex indentRegex(R"(([^\n]*)(?:\/\*+\s*)EMBGEN(?:_|-)REGISTER(?:[^\n]*?\*+\/)[ \t]*)");
            std::smatch match;

            if (!std::regex_search(inoTemplate, match, indentRegex))
            {
                throw InoTemplateException("EMBGEN_REGISTER comment malformed");
            }

            return std::regex_replace(match.str(1), std::regex("\t"), "    ").size();
        }

        std::string EmbGen::generateSource() const
        {
            std::string messengerConstructor = getCommentValue(m_inoTemplate, "VARIABLES");
            std::string messengerName = getMessengerName(messengerConstructor);
            std::string registerCall = getCommentValue(m_inoTemplate, "REGISTER");
            std::string ino = m_inoTemplate;

            ino = replaceComment(ino, "INCLUDES", generateIncludes());
            ino = replaceComment(ino, "VARIABLES", generateVariables(messengerConstructor));
            ino = replaceComment(ino, "SETUP", generateSetup());
            ino = replaceComment(ino, "LOOP", generateLoop(messengerName));
            ino = replaceComment(ino, "REGISTER", generateCommandRegisters(messengerName, registerCall, getRegisterIndent(ino)));
            ino = replaceComment(ino, "COMMANDS", generateCommandFunctions(messengerName));

            return ino;
        }

        std::string EmbGen::generateCore() const
        {
            return std::string();
        }

    }
}