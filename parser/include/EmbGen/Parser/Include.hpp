#ifndef EMBGEN_PARSER_INCLUDE_HPP
#define EMBGEN_PARSER_INCLUDE_HPP

#include "EmbGen/Parser/XmlElement.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            class Include : private XmlElement
            {
                std::string m_value;
                bool m_standard;

            public:
                Include(const tinyxml2::XMLElement* xml);

                std::string getValue() const;

                bool isStandard() const;
            };
        }
    }
}

#endif // EMBGEN_PARSER_INCLUDE_HPP