#include "EmbGen/Parser/ReturnValue.hpp"
#include "EmbGen/Parser/Exceptions.hpp"
#include <tinyxml2.h>
#include <string>

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            ReturnValue::ReturnValue(const tinyxml2::XMLElement* xml) :
                XmlElement(xml)
            {
                m_type = getAttribute("type")->Value();
                m_name = getAttribute("name")->Value();

                if (!isAttributesEmpty())
                {
                    throw AttributeException("Extra attributes for ReturnValue on line " + std::to_string(getLineNum()));
                }

                if (!isElementsEmpty())
                {
                    throw ElementException("Extra elements for ReturnValue on line " + std::to_string(getLineNum()));
                }
            }

            std::string ReturnValue::getType() const
            {
                return m_type;
            }

            std::string ReturnValue::getName() const
            {
                return m_name;
            }
        }
    }
}