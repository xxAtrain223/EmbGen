#ifndef EMBGEN_PARSER_INCLUDE_HPP
#define EMBGEN_PARSER_INCLUDE_HPP

#include "EmbGen/XmlElement.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            class Include : public XmlElement
            {


            public:
                Include(const tinyxml2::XMLElement* xml);
            };
        }
    }
}

#endif // EMBGEN_PARSER_INCLUDE_HPP