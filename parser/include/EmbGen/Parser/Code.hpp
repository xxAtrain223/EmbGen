#ifndef EMBGEN_PARSER_CODE_HPP
#define EMBGEN_PARSER_CODE_HPP

#include "EmbGen/Parser/XmlElement.hpp"

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

                Insert getInsert() const;
                virtual std::string getText() const override;

                Code(const tinyxml2::XMLElement* xml);
                Code(std::string text, Insert insert = Insert::Each);

            private:
                std::string m_text;
                Insert m_insert;

                std::string processCode(const std::string& text);
            };
        }
    }
}

#endif // EMBGEN_PARSER_CODE_HPP