#include <cucumber-cpp/internal/utils/Regex.hpp>

namespace cucumber {
namespace internal {

Regex::Regex(std::string regularExpression) :
    regexSpec(regularExpression),
#ifdef USE_BOOST_ICU_REGEX
    regexImpl(boost::make_u32regex(regularExpression.c_str()))
#else //ifdef USE_BOOST_ICU_REGEX
    regexImpl(regularExpression.c_str())
#endif //ifdef USE_BOOST_ICU_REGEX
{}

bool RegexMatch::matches() {
    return regexMatched;
}

const RegexMatch::submatches_type & RegexMatch::getSubmatches() {
    return submatches;
}

std::string Regex::str() const {
    return regexSpec;
}

RegexMatch *Regex::find(const std::string &expression) {
    return new FindRegexMatch(regexImpl, expression);
}

FindRegexMatch::FindRegexMatch(const boost_regex_type &regexImpl, const std::string &expression) {
    boost::cmatch matchResults;
#ifdef USE_BOOST_ICU_REGEX
    regexMatched = boost::u32regex_search(expression.c_str(), matchResults, regexImpl, boost::regex_constants::match_extra);
#else //ifdef USE_BOOST_ICU_REGEX
    regexMatched = boost::regex_search(expression.c_str(), matchResults, regexImpl, boost::regex_constants::match_extra);
#endif //ifdef USE_BOOST_ICU_REGEX
    if (regexMatched) {
        for (boost::cmatch::size_type i = 1; i < matchResults.size(); ++i) {
            RegexSubmatch s;
            s.value = matchResults.str(i);
            s.position = matchResults.position(i);
            submatches.push_back(s);
        }
    }
}

RegexMatch *Regex::findAll(const std::string &expression) {
    return new FindAllRegexMatch(regexImpl, expression);
}

FindAllRegexMatch::FindAllRegexMatch(const boost_regex_type &regexImpl, const std::string &expression) {
    regexMatched = false;
#ifdef USE_BOOST_ICU_REGEX
    boost::utf8regex_token_iterator i = boost::make_u32regex_token_iterator(expression.c_str(), regexImpl, 1, boost::regex_constants::match_continuous);
    boost::utf8regex_token_iterator j;
#else //ifdef USE_BOOST_ICU_REGEX
    boost::sregex_token_iterator i(expression.begin(), expression.end(), regexImpl, 1, boost::regex_constants::match_continuous);
    boost::sregex_token_iterator j;
#endif //ifdef USE_BOOST_ICU_REGEX
    while (i != j) {
        regexMatched = true;
        RegexSubmatch s;
        s.value = *i;
        s.position = -1;
        submatches.push_back(s);
        ++i;
    }
}

}
}
