#ifndef EMBGEN_PARSER_STOP_HPP
#define EMBGEN_PARSER_STOP_HPP

#include "EmbGen/XmlElement.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            class Stop : public XmlElement
            {


            public:
                Stop(const tinyxml2::XMLElement* xml);
            };
        }
    }
}

#endif // EMBGEN_PARSER_STOP_HPP