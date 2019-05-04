#ifndef EMBGEN_PARSER_LOOP_HPP
#define EMBGEN_PARSER_LOOP_HPP

#include "EmbGen/XmlElement.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            class Loop : public XmlElement
            {


            public:
                Loop(const tinyxml2::XMLElement* xml);
            };
        }
    }
}

#endif // EMBGEN_PARSER_LOOP_HPP