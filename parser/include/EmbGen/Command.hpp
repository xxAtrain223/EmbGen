#ifndef EMBGEN_PARSER_COMMAND_HPP
#define EMBGEN_PARSER_COMMAND_HPP

#include "EmbGen/XmlElement.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            class Command : public XmlElement
            {


            public:
                Command(const tinyxml2::XMLElement* xml);
            };
        }
    }
}

#endif // EMBGEN_PARSER_COMMAND_HPP