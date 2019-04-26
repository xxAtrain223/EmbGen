#ifndef EMBGEN_PARSER_SETUP_HPP
#define EMBGEN_PARSER_SETUP_HPP

#include "EmbGen/XmlElement.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            class Setup : public XmlElement
            {


            public:
                Setup(const tinyxml2::XMLElement* xml);
            };
        }
    }
}

#endif // EMBGEN_PARSER_SETUP_HPP