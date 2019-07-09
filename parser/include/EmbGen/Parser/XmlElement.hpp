#ifndef EMBGEN_PARSER_XMLELEMENT_HPP
#define EMBGEN_PARSER_XMLELEMENT_HPP

#include <map>
#include <vector>
#include <string>

namespace tinyxml2
{
    class XMLElement;
    class XMLAttribute;
}

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            class XmlElement
            {
                const tinyxml2::XMLElement* m_tinyElement;
                std::map<std::string, const tinyxml2::XMLAttribute*> m_attributes;
                std::multimap<std::string, const tinyxml2::XMLElement*> m_elements;

            protected:
                XmlElement();

            public:
                XmlElement(const tinyxml2::XMLElement* xml);

                std::string getName() const;

                virtual std::string getText() const;

                int getLineNum() const;

                const tinyxml2::XMLAttribute* getAttribute(std::string name);

                bool isAttributesEmpty() const;

                std::vector<const tinyxml2::XMLElement*> getElements(std::string name);

                bool isElementsEmpty() const;
            };
        }
    }
}

#endif // EMBGEN_PARSER_XMLELEMENT_HPP