
#include "src/operators/verify_svnr.h"

#include <string>

#include "src/operators/operator.h"

namespace modsecurity {
namespace operators {

int VerifySVNR::convert_to_int(const char c)
{
    int n;
    if ((c>='0') && (c<='9'))
        n = c - '0';
    else if ((c>='A') && (c<='F'))
        n = c - 'A' + 10;
    else if ((c>='a') && (c<='f'))
        n = c - 'a' + 10;
    else
        n = 0;
    return n;
}

bool VerifySVNR::verify(const char *svnrnumber, int len) {
    int factor, part_1, part_2, var_len = len;
    unsigned int sum = 0, i = 0, svnr_len = 11, c;
    int svnr[11];
    char s_svnr[11];
    char bad_svnr[12][12] = { "00000000000",
        "01234567890",
        "11111111111",
        "22222222222",
        "33333333333",
        "44444444444",
        "55555555555",
        "66666666666",
        "77777777777",
        "88888888888",
        "99999999999"};

    while ((*svnrnumber != '\0') && ( var_len > 0))
    {
        if (*svnrnumber != '-' || *svnrnumber != '.')
        {
            if (i < svnr_len && isdigit(*svnrnumber))
            {
                s_svnr[i] = *svnrnumber;
                svnr[i] = convert_to_int(*svnrnumber);
                i++;
            }
        }
        svnrnumber++;
        var_len--;
    }


    if (i != svnr_len)
    {
        return 0;
    }
    else
    {
        for (i = 0; i< svnr_len; i++)
        {
            if (strncmp(s_svnr,bad_svnr[i],svnr_len) == 0)
            {
                return 0;
            }
        }
    }
    //Laufnummer mit 3, 7, 9
    //Geburtsdatum mit 5, 8, 4, 2, 1, 6
    sum = svnr[0] * 3 + svnr[1] * 7 + svnr[2] * 9 + svnr[4] * 5 + svnr[5] * 8 + svnr[6] * 4 + svnr[7] * 2 + svnr[8] * 1 + svnr[9] * 6;
    sum %= 11;

    if (sum == svnr[3])
    {
        return true;
    }

    return false;
}


bool VerifySVNR::evaluate(Transaction *transaction, Rule *rule,
    const std::string& input, std::shared_ptr<RuleMessage> ruleMessage) {
    std::list<SMatch> matches;
    bool is_svnr = false;
    int i;

    if (m_param.empty()) {
        return is_svnr;
    }

    for (i = 0; i < input.size() - 1; i++) {
        printf ("Decimal i: %d\n", i);
        matches = m_re->searchAll(input.substr(i, input.size()));


        for (const auto & i : matches) {
            is_svnr = verify(i.str().c_str(), i.str().size());
            if (is_svnr) {
	           logOffset(ruleMessage, i.offset(), i.str().size());
               break;
            }
        }
    }
    return is_svnr;
}


}  // namespace operators
}  // namespace modsecurity
