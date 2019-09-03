#ifndef EMBGEN_EMBGEN_HPP
#define EMBGEN_EMBGEN_HPP

#include <string>
#include <vector>

#include "EmbGen/Appendage.hpp"

#include <nlohmann/json_fwd.hpp>

namespace emb
{
    namespace gen
    {
        class EmbGen
        {
            std::string m_inoTemplate;

            nlohmann::json m_appendageJson;

            std::vector<Appendage> m_appendages;

            std::string generateIncludes() const;

            std::string getCommandCount() const;

            std::string generateVariables(std::string messengerConstructor) const;

            std::string generateSetup() const;

            std::string generateLoop(std::string messengerName) const;

            std::string generateCommandRegisters(std::string messengerName, std::string registerCall, int indentWidth) const;

            std::string generateCommandFunctions(std::string messengerName) const;

        public:
            EmbGen(std::string config, std::string appendage_folder, std::string inoTemplate);

            std::string generateSource() const;

            std::string generateCore() const;
        };
    }
}

#endif // EMBGEN_EMBGEN_HPP