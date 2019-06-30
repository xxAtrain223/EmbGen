#ifndef EMBGEN_PARSER_COMMAND_HPP
#define EMBGEN_PARSER_COMMAND_HPP

#include "EmbGen/Parser/XmlElement.hpp"
#include <vector>
#include <memory>
#include "EmbGen/Parser/Parameter.hpp"
#include "EmbGen/Parser/ReturnValue.hpp"
#include "EmbGen/Parser/Code.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            class Appendage;

            class Command : public XmlElement
            {
                std::string m_name;
                std::vector<Parameter> m_parameters;
                std::vector<ReturnValue> m_returnValues;
                std::shared_ptr<Code> m_code;

            public:
                Command(const tinyxml2::XMLElement* xml);
                Command(std::string name, std::vector<Parameter>&& parameters, std::vector<ReturnValue>&& returnValues, std::shared_ptr<Code> code);

                std::string getName() const;

                std::vector<Parameter> getParameters() const;

                std::vector<ReturnValue> getReturnValues() const;

                std::shared_ptr<Code> getCode() const;

            };
        }
    }
}

#endif // EMBGEN_PARSER_COMMAND_HPP