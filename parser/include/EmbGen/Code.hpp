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
                enum class Insert
                {
                    Each,
                    Once
                };

                Code(const tinyxml2::XMLElement* xml);

                Insert getInsert() const;

                std::string getText() const;
            };
        }
    }
}

#endif // EMBGEN_PARSER_CODE_HPP