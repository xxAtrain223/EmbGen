#include "EmbGen/Command.hpp"
#include <tinyxml2.h>

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            Command::Command(const tinyxml2::XMLElement* xml) :
                XmlElement(xml)
            {
                m_name = getAttribute("name")->Value();

                for (auto parameter : getElements("parameter"))
                {
                    m_parameters.emplace_back(parameter);
                }

                std::vector<const tinyxml2::XMLElement*> code = getElements("code");

                if (code.size() == 1)
                {
                    m_code = std::make_shared<Code>(code.at(0));
                }
                else if (code.size() == 0)
                {
                    throw ElementException("No Code element for Command '" + m_name + "' on line " + std::to_string(getLineNum()));
                }
                else
                {
                    throw ElementException("Too many Code elements for Command '" + m_name + "' on line " + std::to_string(getLineNum()));
                }

                if (!isAttributesEmpty())
                {
                    throw AttributeException("Extra attributes for Command on line " + std::to_string(getLineNum()));
                }

                if (!isElementsEmpty())
                {
                    throw ElementException("Extra elements for Command on line " + std::to_string(getLineNum()));
                }
            }

            std::string Command::getName() const
            {
                return m_name;
            }

            std::vector<Parameter> Command::getParameters() const
            {
                return m_parameters;
            }
            
            std::shared_ptr<Code> Command::getCode() const
            {
                return m_code;
            }
        }
    }
}