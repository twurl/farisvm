#ifndef ABPVM_C
#define ABPVM_C

// #define USE_RE2

#include <string>
#include <vector>
#include <exception>
#include <memory>

#ifdef USE_RE2
  #include <re2/re2.h>
#else
  #include <regex>
#endif

#include <boost/algorithm/searching/boyer_moore_horspool.hpp>

#define FLAG_MATCH_CASE             0x00000001
#define FLAG_NOT                   (0x00000001 <<  1)
#define FLAG_SCRIPT                (0x00000001 <<  2)
#define FLAG_NOT_SCRIPT            (0x00000001 <<  3)
#define FLAG_IMAGE                 (0x00000001 <<  4)
#define FLAG_NOT_IMAGE             (0x00000001 <<  5)
#define FLAG_STYLESHEET            (0x00000001 <<  6)
#define FLAG_NOT_STYLESHEET        (0x00000001 <<  7)
#define FLAG_OBJECT                (0x00000001 <<  8)
#define FLAG_NOT_OBJECT            (0x00000001 <<  9)
#define FLAG_XMLHTTPREQUEST        (0x00000001 << 10)
#define FLAG_NOT_XMLHTTPREQUEST    (0x00000001 << 11)
#define FLAG_OBJECT_SUBREQUEST     (0x00000001 << 12)
#define FLAG_NOT_OBJECT_SUBREQUEST (0x00000001 << 13)
#define FLAG_SUBDOCUMENT           (0x00000001 << 14)
#define FLAG_NOT_SUBDOCUMENT       (0x00000001 << 15)
#define FLAG_DOCUMENT              (0x00000001 << 16)
#define FLAG_NOT_DOCUMENT          (0x00000001 << 17)
#define FLAG_ELEMHIDE              (0x00000001 << 18)
#define FLAG_NOT_ELEMHIDE          (0x00000001 << 19)
#define FLAG_OTHER                 (0x00000001 << 20)
#define FLAG_NOT_OTHER             (0x00000001 << 21)
#define FLAG_THIRD_PARTY           (0x00000001 << 22)
#define FLAG_NOT_THIRD_PARTY       (0x00000001 << 23)
#define FLAG_COLLAPSE              (0x00000001 << 24)
#define FLAG_NOT_COLLAPSE          (0x00000001 << 25)
#define FLAG_DOMAIN                (0x00000001 << 26)

class abpvm_query {
public:
    void set_uri(const std::string &uri);
    const std::string &get_uri() const { return m_uri; }
    const std::string &get_domain() const { return m_domain; }

private:
    std::string m_uri;
    std::string m_domain;
};

class abpvm {
public:
    struct abpvm_match_result {
        std::string rule;
    };

    abpvm();
    virtual ~abpvm();

    void add_rule(const std::string &rule);
    void match(std::vector<std::string> &result, const abpvm_query *query, int size);

private:
    typedef boost::algorithm::boyer_moore_horspool<std::string::iterator> BMH;

    struct abpvm_domain {
        abpvm_domain(std::string d) : name(d), bmh(new BMH(d.begin(), d.end())) { }

        std::shared_ptr<BMH> bmh;
        std::string name;
    };

    struct abpvm_code {
        std::vector<abpvm_domain> domains;
        std::vector<abpvm_domain> ex_domains;
        std::string original_rule;
        std::string rule;
        uint32_t    flags;
#ifdef USE_RE2
        std::shared_ptr<RE2> re;
#else
        std::shared_ptr<std::regex>  re;
#endif
    };

    std::vector<abpvm_code> m_codes;

#ifdef USE_RE2
    std::shared_ptr<RE2> get_re(const std::string &rule);
#else
    std::shared_ptr<std::regex> get_re(const std::string &rule);
#endif

    void validate_rule(const std::string &rule);

    void split(const std::string &str, const std::string &delim,
               std::vector<std::string> &ret);
};

struct abpvm_exception : public std::exception
{
    abpvm_exception(const std::string msg);
    ~abpvm_exception() throw();

    const char* what() const throw();

    std::string m_msg;
};

#endif // ABPVM
