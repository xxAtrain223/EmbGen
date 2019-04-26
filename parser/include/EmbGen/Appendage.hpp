#ifndef EMBGEN_PARSER_APPENDAGE_HPP
#define EMBGEN_PARSER_APPENDAGE_HPP

#include "EmbGen/XmlElement.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            class Appendage : public XmlElement
            {


            public:
                Appendage(const tinyxml2::XMLElement* xml);
            };
        }
    }
}

#endif // EMBGEN_PARSER_APPENDAGE_HPP