#ifndef EMBGEN_PARSER_COMMAND_HPP
#define EMBGEN_PARSER_COMMAND_HPP

#include "EmbGen/XmlElement.hpp"
#include <vector>
#include <memory>
#include "EmbGen/Parameter.hpp"
#include "EmbGen/Code.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            class Command : public XmlElement
            {


            public:
                Command(const tinyxml2::XMLElement* xml);

                std::string getName() const;

                std::vector<Parameter> getParameters() const;

                std::shared_ptr<Code> getCode() const;

            };
        }
    }
}

#endif // EMBGEN_PARSER_COMMAND_HPP