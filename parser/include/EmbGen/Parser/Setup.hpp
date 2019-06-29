#ifndef EMBGEN_PARSER_SETUP_HPP
#define EMBGEN_PARSER_SETUP_HPP

#include "EmbGen/Parser/XmlElement.hpp"
#include <vector>
#include "EmbGen/Parser/Code.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            class Setup : public XmlElement
            {
                std::vector<Code> m_code;

            public:
                Setup(const tinyxml2::XMLElement* xml);

                std::vector<Code> getCode() const;
            };
        }
    }
}

#endif // EMBGEN_PARSER_SETUP_HPP