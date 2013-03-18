#ifndef CUKE_REGEX_HPP_
#define CUKE_REGEX_HPP_

#include <vector>

#ifdef USE_BOOST_ICU_REGEX
 // if boost::regex was built with ICU backend support => use this to enable UTF-8 support ..
 #include <boost/regex/icu.hpp>
#endif //ifdef USE_BOOST_ICU_REGEX

#include <boost/regex.hpp>

namespace cucumber {
namespace internal {

#ifdef USE_BOOST_ICU_REGEX
  typedef boost::u32regex boost_regex_type;
#else //ifdef USE_BOOST_ICU_REGEX
  typedef boost::regex boost_regex_type;
#endif //ifdef USE_BOOST_ICU_REGEX

struct RegexSubmatch {
    std::string value;
    int position;
};


class RegexMatch {
public:
    typedef std::vector<RegexSubmatch> submatches_type;

    virtual ~RegexMatch() {};

    bool matches();
    const submatches_type & getSubmatches();

protected:
    bool regexMatched;
    submatches_type submatches;
};

class FindRegexMatch : public RegexMatch {
public:
    FindRegexMatch(const boost_regex_type &regexImpl, const std::string &expression);
};

class FindAllRegexMatch : public RegexMatch {
public:
    FindAllRegexMatch(const boost_regex_type &regexImpl, const std::string &expression);
};


class Regex {
private:
    boost_regex_type regexImpl;
    std::string regexSpec;

public:
    Regex(std::string expr);

    RegexMatch *find(const std::string &expression);
    RegexMatch *findAll(const std::string &expression);

    std::string str() const;
};

}
}

#endif /* CUKE_REGEX_HPP_ */

