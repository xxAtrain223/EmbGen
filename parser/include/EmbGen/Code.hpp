#ifndef EMBGEN_PARSER_CODE_HPP
#define EMBGEN_PARSER_CODE_HPP

#include "EmbGen/XmlElement.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            class Code : public XmlElement
            {


            public:
                Code(const tinyxml2::XMLElement* xml);
            };
        }
    }
}

#endif // EMBGEN_PARSER_CODE_HPP