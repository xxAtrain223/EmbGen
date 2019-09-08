#ifndef EMBGEN_PARSER_EXCEPTIONS_HPP
#define EMBGEN_PARSER_EXCEPTIONS_HPP

#include <stdexcept>

#define NEW_EMB_EX(name)                                  \
    class name : public emb::gen::BaseException           \
    {                                                     \
    public:                                               \
        name(const std::string& message = "") :           \
            emb::gen::BaseException(#name ": " + message) \
        {                                                 \
        }                                                 \
    };                                                    \

namespace emb
{
    namespace gen
    {
        class BaseException : public std::runtime_error
        {
        public:
            BaseException(const std::string& message) :
                std::runtime_error(message)
            {
            }
        };

        NEW_EMB_EX(XmlParseException);
        NEW_EMB_EX(InoTemplateException);
    }
}

#endif // EMBGEN_PARSER_EXCEPTIONS_HPP