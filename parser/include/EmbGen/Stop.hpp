#ifndef EMBGEN_PARSER_STOP_HPP
#define EMBGEN_PARSER_STOP_HPP

#include "EmbGen/XmlElement.hpp"
#include "EmbGen/Code.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            class Stop : public XmlElement
            {
                std::string m_command;
                std::shared_ptr<Code> m_code;

            public:
                Stop(const tinyxml2::XMLElement* xml);

                std::string getCommand() const;

                std::shared_ptr<Code> getCode() const;
            };
        }
    }
}

#endif // EMBGEN_PARSER_STOP_HPP