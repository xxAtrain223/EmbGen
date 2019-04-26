#include "EmbGen/XmlElement.hpp"
#include <tinyxml2.h>

namespace emb::gen::parser
{
    XmlElement::XmlElement(const tinyxml2::XMLElement* xml)
    {

    }

    const tinyxml2::XMLAttribute* XmlElement::getAttribute(std::string name)
    {

    }

    bool XmlElement::isAttributesEmpty() const
    {

    }

    std::vector<const tinyxml2::XMLElement*> XmlElement::getElements(std::string name)
    {

    }

    bool XmlElement::isElementsEmpty() const
    {

    }

    std::string XmlElement::getText() const
    {

    }

    int XmlElement::getLineNum() const
    {

    }
}