#ifndef EMBGEN_PARSER_VARIABLE_HPP
#define EMBGEN_PARSER_VARIABLE_HPP

#include "EmbGen/XmlElement.hpp"
#include "EmbGen/Parameter.hpp"

#include <vector>

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            class Variable : private XmlElement
            {
                std::string m_type, m_name;
                bool m_core;
                std::vector<Parameter> m_parameters;

            public:
                Variable(const tinyxml2::XMLElement* xml);

                std::string getType() const;

                std::string getName() const;

                bool isCore() const;

                std::vector<Parameter> getParameters() const;
            };
        }
    }
}

#endif // EMBGEN_PARSER_VARIABLE_HPP