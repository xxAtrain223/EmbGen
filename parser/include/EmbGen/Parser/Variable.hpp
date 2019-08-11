#ifndef EMBGEN_PARSER_VARIABLE_HPP
#define EMBGEN_PARSER_VARIABLE_HPP

#include "EmbGen/Parser/XmlElement.hpp"
#include "EmbGen/Parser/Parameter.hpp"

#include <vector>

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            class Variable : private XmlElement
            {
                std::string m_type, m_name, m_appendage;
                bool m_core;
                std::vector<Parameter> m_parameters;

            public:
                Variable(const tinyxml2::XMLElement* xml);

                std::string getType() const;

                std::string getName() const;

                bool isCore() const;

                std::string getAppendage() const;

                std::vector<Parameter> getParameters() const;
            };
        }
    }
}

#endif // EMBGEN_PARSER_VARIABLE_HPP