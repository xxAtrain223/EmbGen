#ifndef EMBGEN_PARSER_VARIABLE_HPP
#define EMBGEN_PARSER_VARIABLE_HPP

#include "EmbGen/XmlElement.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            class Variable : public XmlElement
            {


            public:
                Variable(const tinyxml2::XMLElement* xml);
            };
        }
    }
}

#endif // EMBGEN_PARSER_VARIABLE_HPP