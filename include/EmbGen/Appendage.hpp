#ifndef EMBGEN_APPENDAGE_HPP
#define EMBGEN_APPENDAGE_HPP

#include <nlohmann/json.hpp>

#include <memory>

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            class Appendage;
            class Command;
            class Stop;
        }

        class Appendage
        {
            std::shared_ptr<parser::Appendage> m_xml;
            nlohmann::json m_json, m_appendages;

            std::string replaceVariables(std::string block, std::string append = "") const;

            std::string getAdaptorCode(std::string messengerName, std::shared_ptr<parser::Command> command) const;

            std::string getAdaptorFunction(std::string messengerName, std::shared_ptr<parser::Command> command) const;

            std::string getCommandCode(std::shared_ptr<parser::Command> command) const;

            std::string getCommandFunction(std::shared_ptr<parser::Command> command) const;

            std::string getStopFunction(std::shared_ptr<parser::Stop> stop) const;

        public:
            Appendage(std::shared_ptr<parser::Appendage> xml, const nlohmann::json& json);

            std::string getName() const;

            std::string getIncludes() const;

            std::string getVariables() const;

            std::string getSetup() const;

            std::string getLoop() const;

            std::vector<std::string> getCommandNames() const;

            std::string getCommandFunctions(std::string messengerName) const;

            std::shared_ptr<parser::Appendage> getXml() const;

            bool hasStop() const;
        };
    }
}

#endif // EMBGEN_APPENDAGE_HPP