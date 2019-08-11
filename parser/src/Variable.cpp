#include "EmbGen/Parser/Variable.hpp"
#include <tinyxml2.h>
#include "EmbGen/Parser/Exceptions.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            Variable::Variable(const tinyxml2::XMLElement* xml) :
                XmlElement(xml)
            {
                m_type = getAttribute("type")->Value();
                m_name = getAttribute("name")->Value();

                try
                {
                    m_core = getAttribute("core")->BoolValue();
                }
                catch (AttributeException)
                {
                    m_core = false;
                }

                for (auto parameter : getElements("parameter"))
                {
                    m_parameters.emplace_back(parameter);
                }

                try
                {
                    m_appendage = getAttribute("appendage")->Value();
                }
                catch (AttributeException)
                {
                    m_appendage = "";
                }

                if (!isAttributesEmpty())
                {
                    throw AttributeException("Extra attributes for Include on line " + std::to_string(getLineNum()));
                }

                if (!isElementsEmpty())
                {
                    throw ElementException("Extra elements for Include on line " + std::to_string(getLineNum()));
                }
            }

            std::string Variable::getType() const
            {
                return m_type;
            }

            std::string Variable::getName() const
            {
                return m_name;
            }

            bool Variable::isCore() const
            {
                return m_core;
            }

            std::string Variable::getAppendage() const
            {
                return m_appendage;
            }

            std::vector<Parameter> Variable::getParameters() const
            {
                return m_parameters;
            }
        }
    }
}