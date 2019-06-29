#include "EmbGen/Parser/XmlElement.hpp"

#include <tinyxml2.h>
#include "EmbGen/Parser/Exceptions.hpp"
#include <string>

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            XmlElement::XmlElement()
            {
            }

            XmlElement::XmlElement(const tinyxml2::XMLElement* xml) :
                m_tinyElement(xml)
            {
                for (auto attr = m_tinyElement->FirstAttribute(); attr != nullptr; attr = attr->Next())
                {
                    m_attributes.emplace(attr->Name(), attr);
                }

                for (auto ele = m_tinyElement->FirstChildElement(); ele != nullptr; ele = ele->NextSiblingElement())
                {
                    m_elements.emplace(ele->Name(), ele);
                }
            }

            std::string XmlElement::getName() const
            {
                return m_tinyElement->Name();
            }

            std::string XmlElement::getText() const
            {
                const char* text = m_tinyElement->GetText();
                if (text != nullptr)
                {
                    return text;
                }
                throw ElementException("Element does not have text on line " + std::to_string(getLineNum()));
            }

            int XmlElement::getLineNum() const
            {
                return m_tinyElement->GetLineNum();
            }

            const tinyxml2::XMLAttribute* XmlElement::getAttribute(std::string name)
            {
                try
                {
                    auto attribute = m_attributes.at(name);
                    m_attributes.erase(name);

                    return attribute;
                }
                catch (std::out_of_range)
                {
                    throw AttributeException("Attribute " + name + " missing on line " + std::to_string(getLineNum()));
                }
            }

            bool XmlElement::isAttributesEmpty() const
            {
                return m_attributes.empty();
            }

            std::vector<const tinyxml2::XMLElement*> XmlElement::getElements(std::string name)
            {
                std::vector<const tinyxml2::XMLElement*> elements;

                auto elements_range = m_elements.equal_range(name);

                elements.reserve(std::distance(elements_range.first, elements_range.second));

                for (auto it = elements_range.first; it != elements_range.second; it = m_elements.erase(it))
                {
                    elements.emplace_back(it->second);
                }

                return elements;
            }

            bool XmlElement::isElementsEmpty() const
            {
                return m_elements.empty();
            }
        }
    }
}