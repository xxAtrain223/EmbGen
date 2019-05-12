#ifndef EMBGEN_PARSER_APPENDAGE_HPP
#define EMBGEN_PARSER_APPENDAGE_HPP

#include "EmbGen/XmlElement.hpp"
#include "EmbGen/Command.hpp"
#include "EmbGen/Include.hpp"
#include "EmbGen/Loop.hpp"
#include "EmbGen/Setup.hpp"
#include "EmbGen/Variable.hpp"

#include <vector>
#include <map>
#include <memory>

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            class Appendage : public XmlElement
            {


            public:
                Appendage(const tinyxml2::XMLElement* xml);

                std::string getName() const;

                std::string getVersion() const;

                std::string getLibDeps() const;

                std::vector<Include> getIncludes() const;

                std::vector<Variable> getVariables() const;

                std::shared_ptr<Setup> getSetup() const;

                std::shared_ptr<Loop> getLoop() const;

                std::map<std::string, Command> getCommands() const;
            };
        }
    }
}

#endif // EMBGEN_PARSER_APPENDAGE_HPP