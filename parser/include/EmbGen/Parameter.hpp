#ifndef EMBGEN_PARSER_PARAMETER_HPP
#define EMBGEN_PARSER_PARAMETER_HPP

#include "EmbGen/XmlElement.hpp"

#include "EmbGen/Exceptions.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            class Parameter : public XmlElement
            {
                std::string m_type, m_name;
                bool m_core;
                tinyxml2::XMLAttribute *m_min, *m_max;

                bool read(const tinyxml2::XMLAttribute*, uint8_t&) const;
                bool read(const tinyxml2::XMLAttribute*, uint16_t&) const;
                bool read(const tinyxml2::XMLAttribute*, uint32_t&) const;
                bool read(const tinyxml2::XMLAttribute*, uint64_t&) const;
                bool read(const tinyxml2::XMLAttribute*, int8_t&) const;
                bool read(const tinyxml2::XMLAttribute*, int16_t&) const;
                bool read(const tinyxml2::XMLAttribute*, int32_t&) const;
                bool read(const tinyxml2::XMLAttribute*, int64_t&) const;
                bool read(const tinyxml2::XMLAttribute*, float&) const;

            public:
                Parameter(const tinyxml2::XMLElement* xml);

                std::string getType() const;
                
                std::string getName() const;

                template <typename T>
                tinyxml2::XMLAttribute* getMin() const
                {
                    T value;
                    if (!read(m_min, T))
                    {
                        throw AttributeException("Could not read min value for Parameter on line " + std::to_string(getLineNum()));
                    }
                    return value;
                }

                template <typename T>
                tinyxml2::XMLAttribute* getMax() const
                {
                    T value;
                    if (!read(m_max, T))
                    {
                        throw AttributeException("Could not read max value for Parameter on line " + std::to_string(getLineNum()));
                    }
                    return value;
                }

                bool isCore() const;
            };
        }
    }
}

#endif // EMBGEN_PARSER_PARAMETER_HPP