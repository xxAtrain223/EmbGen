#include "EmbGen/Parser/Parameter.hpp"
#include "EmbGen/Parser/Exceptions.hpp"
#include <tinyxml2.h>
#include <string>

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            Parameter::Parameter(const tinyxml2::XMLElement* xml) :
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

                try
                {
                    m_min = getAttribute("min")->Value();
                }
                catch (AttributeException)
                {
                    m_min = "";
                }

                try
                {
                    m_max = getAttribute("max")->Value();
                }
                catch (AttributeException)
                {
                    m_max = "";
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
                    throw AttributeException("Extra attributes for Parameter on line " + std::to_string(getLineNum()));
                }

                if (!isElementsEmpty())
                {
                    throw ElementException("Extra elements for Parameter on line " + std::to_string(getLineNum()));
                }
            }

            std::string Parameter::getType() const
            {
                return m_type;
            }

            std::string Parameter::getName() const
            {
                return m_name;
            }

            std::string Parameter::getMin() const
            {
                return m_min;
            }

            std::string Parameter::getMax() const
            {
                return m_max;
            }

            bool Parameter::isCore() const
            {
                return m_core;
            }

            std::string Parameter::getAppendage() const
            {
                return m_appendage;
            }
        }
    }
}