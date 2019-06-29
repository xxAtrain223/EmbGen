#include "EmbGen/Parser/Loop.hpp"
#include "EmbGen/Parser/Exceptions.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            Loop::Loop(const tinyxml2::XMLElement* xml) :
                XmlElement(xml)
            {
                for (auto code : getElements("code"))
                {
                    m_code.emplace_back(code);
                }

                if (m_code.empty())
                {
                    throw ElementException("No code for Loop on line " + std::to_string(getLineNum()));
                }

                if (!isAttributesEmpty())
                {
                    throw AttributeException("Extra attributes for Loop on line " + std::to_string(getLineNum()));
                }

                if (!isElementsEmpty())
                {
                    throw ElementException("Extra elements for Loop on line " + std::to_string(getLineNum()));
                }
            }

            std::vector<Code> Loop::getCode() const
            {
                return m_code;
            }
        }
    }
}