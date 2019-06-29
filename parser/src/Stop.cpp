#include "EmbGen/Parser/Stop.hpp"
#include "EmbGen/Parser/Exceptions.hpp"
#include <tinyxml2.h>

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            Stop::Stop(const tinyxml2::XMLElement* xml) :
                XmlElement(xml)
            {
                try
                {
                    m_command = getAttribute("command")->Value();
                }
                catch (AttributeException)
                {
                    m_command = "";
                }

                std::vector<const tinyxml2::XMLElement*> code = getElements("code");

                if (code.size() == 1)
                {
                    m_code = std::make_shared<Code>(code.at(0));
                }
                else if (code.size() == 0)
                {
                    m_code = nullptr;
                }
                else
                {
                    throw ElementException("Too many Code elements for Stop on line " + std::to_string(getLineNum()));
                }

                if (m_command == "" && m_code == nullptr)
                {
                    throw ParserException("Neither command nor code are defined for Stop on line " + std::to_string(getLineNum()));
                }
                if (m_command != "" && m_code != nullptr)
                {
                    throw ParserException("Both command and code are defined for Stop on line " + std::to_string(getLineNum()));
                }

                if (!isAttributesEmpty())
                {
                    throw AttributeException("Extra attributes for Stop on line " + std::to_string(getLineNum()));
                }

                if (!isElementsEmpty())
                {
                    throw ElementException("Extra elements for Stop on line " + std::to_string(getLineNum()));
                }
            }
            
            std::string Stop::getCommand() const
            {
                return m_command;
            }
            
            std::shared_ptr<Code> Stop::getCode() const
            {
                return m_code;
            }
        }
    }
}