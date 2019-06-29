#ifndef EMBGEN_PARSER_LOOP_HPP
#define EMBGEN_PARSER_LOOP_HPP

#include "EmbGen/Parser/XmlElement.hpp"
#include <vector>
#include "EmbGen/Parser/Code.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            class Loop : public XmlElement
            {
                std::vector<Code> m_code;

            public:
                Loop(const tinyxml2::XMLElement* xml);

                std::vector<Code> getCode() const;
            };
        }
    }
}

#endif // EMBGEN_PARSER_LOOP_HPP