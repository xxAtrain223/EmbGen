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

                std::string getType() const;
                
                std::string getName() const;

                tinyxml2::XMLAttribute* getMin() const;

                tinyxml2::XMLAttribute* getMax() const;

                bool isCore() const;
            };
        }
    }
}

#endif // EMBGEN_PARSER_PARAMETER_HPP