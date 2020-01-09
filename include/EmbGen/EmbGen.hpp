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

            std::string generateVariables(std::string messengerName) const;

            std::string generateSetup() const;

            std::string generateLoop(std::string messengerName) const;

            std::string generateCommandFunctions(std::string messengerConstructor) const;

        public:
            EmbGen(std::string config, std::string appendage_folder, std::string inoTemplate);

            std::string generateSource() const;

            std::string generateCore() const;

            std::vector<std::string> getAppendageNames() const;
        };
    }
}

#endif // EMBGEN_EMBGEN_HPP