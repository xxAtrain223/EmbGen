#include "EmbGen/Parser/Setup.hpp"
#include <tinyxml2.h>
#include "EmbGen/Parser/Exceptions.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            Setup::Setup(const tinyxml2::XMLElement* xml) :
                XmlElement(xml)
            {
                for (auto code : getElements("code"))
                {
                    m_code.emplace_back(code);
                }

                if (m_code.empty())
                {
                    throw ElementException("No code for Setup on line " + std::to_string(getLineNum()));
                }

                if (!isAttributesEmpty())
                {
                    throw AttributeException("Extra attributes for Setup on line " + std::to_string(getLineNum()));
                }

                if (!isElementsEmpty())
                {
                    throw ElementException("Extra elements for Setup on line " + std::to_string(getLineNum()));
                }
            }

            std::vector<Code> Setup::getCode() const
            {
                return m_code;
            }
        }
    }
}