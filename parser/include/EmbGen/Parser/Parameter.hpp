#ifndef EMBGEN_PARSER_PARAMETER_HPP
#define EMBGEN_PARSER_PARAMETER_HPP

#include "EmbGen/Parser/XmlElement.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            class Parameter : public XmlElement
            {
                std::string m_type, m_name, m_min, m_max, m_appendage;
                bool m_core;

            public:
                Parameter(const tinyxml2::XMLElement* xml);

                std::string getType() const;
                
                std::string getName() const;

                std::string getMin() const;

                std::string getMax() const;

                bool isCore() const;

                std::string getAppendage() const;
            };
        }
    }
}

#endif // EMBGEN_PARSER_PARAMETER_HPP