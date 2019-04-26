#ifndef EMBGEN_PARSER_PARAMETER_HPP
#define EMBGEN_PARSER_PARAMETER_HPP

#include "EmbGen/XmlElement.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            class Parameter : public XmlElement
            {


            public:
                Parameter(const tinyxml2::XMLElement* xml);
            };
        }
    }
}

#endif // EMBGEN_PARSER_PARAMETER_HPP