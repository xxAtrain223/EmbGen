#include "EmbGen/Parser/Command.hpp"
#include "EmbGen/Parser/Exceptions.hpp"
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

                for (auto returnValue : getElements("return_value"))
                {
                    m_returnValues.emplace_back(returnValue);
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

            Command::Command(std::string name, std::vector<Parameter>&& parameters, std::vector<ReturnValue>&& returnValues, std::shared_ptr<Code> code) :
                m_name(name), m_parameters(parameters), m_returnValues(returnValues), m_code(code)
            {
            }

            std::string Command::getName() const
            {
                return m_name;
            }

            std::vector<Parameter> Command::getParameters() const
            {
                return m_parameters;
            }

            std::vector<ReturnValue> Command::getReturnValues() const
            {
                return m_returnValues;
            }
            
            std::shared_ptr<Code> Command::getCode() const
            {
                return m_code;
            }
        }
    }
}