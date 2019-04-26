#include "EmbGen/XmlElement.hpp"
#include <tinyxml2.h>

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            XmlElement::XmlElement(const tinyxml2::XMLElement* xml) :
                tinyElement(xml)
            {

            }

            const tinyxml2::XMLAttribute* XmlElement::getAttribute(std::string name)
            {
                return nullptr;
            }

            bool XmlElement::isAttributesEmpty() const
            {
                return false;
            }

            std::vector<const tinyxml2::XMLElement*> XmlElement::getElements(std::string name)
            {
                return std::vector<const tinyxml2::XMLElement*>{};
            }

            bool XmlElement::isElementsEmpty() const
            {
                return false;
            }

            std::string XmlElement::getText() const
            {
                return "";
            }

            int XmlElement::getLineNum() const
            {
                return 0;
            }
        }
    }
}