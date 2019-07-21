#include "EmbGen/Parser/Include.hpp"
#include <tinyxml2.h>
#include "EmbGen/Parser/Exceptions.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            Include::Include(const tinyxml2::XMLElement* xml) :
                XmlElement(xml)
            {
                try
                {
                    m_standard = getAttribute("standard")->BoolValue();
                }
                catch (AttributeException)
                {
                    m_standard = false;
                }

                m_value = getText();

                if (!isAttributesEmpty())
                {
                    throw AttributeException("Extra attributes for Include on line " + std::to_string(getLineNum()));
                }

                if (!isElementsEmpty())
                {
                    throw ElementException("Extra elements for Include on line " + std::to_string(getLineNum()));
                }
            }

            std::string Include::getValue() const
            {
                return m_value;
            }

            bool Include::isStandard() const
            {
                return m_standard;
            }
        }
    }
}