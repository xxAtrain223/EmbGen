#ifndef EMBGEN_PARSER_RETURNVALUE_HPP
#define EMBGEN_PARSER_RETURNVALUE_HPP

#include "EmbGen/Parser/XmlElement.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            class ReturnValue : public XmlElement
            {
                std::string m_type, m_name;

            public:
                ReturnValue(const tinyxml2::XMLElement* xml);

                std::string getType() const;

                std::string getName() const;
            };
        }
    }
}

#endif // EMBGEN_PARSER_RETURNVALUE_HPP