#ifndef BASE64_H
#define BASE64_H

#include <vector>
#include <string>

/*
*   BASE64 ENCODING : Learn more here https://en.wikipedia.org/wiki/Base64
*
*   This file can be modified to create a more strong and secure algorithim for encrypting the logs.
*
*   As BASE64 can be easily decoded , here i have made some random modifications to make this
*   algorithim a bit stronger.
*/ 

namespace Base64
{
    std::string base64_encode (const std::string &);

    /*
    *   SALTS   :   Theses are some random salts which can be changed to some more stronger combinations
    *
    */

    const std::string &SALT1 = "LM::TB::BB";
    const std::string &SALT2 = "_:/_77";
    const std::string &SALT3 = "UUi_KL::b32";

    std::string encryptB64(std::string s)
    {
        /*
        *   Here are some random changes made to the default algorithim.
        *   However if you change something here the same shall be done in the decryptor
        *
        */
        
        s = SALT1 + s + SALT2 + SALT3;
        s = base64_encode(s);
        s.insert(5, SALT3);
        s += SALT1;
        s = base64_encode(s);
        s = SALT2 + SALT3 + s + SALT1;
        s = base64_encode(s);
        s.insert(1, "K");
        s.insert(7,"L");

        return s;
    }

    const std::string &BASE64_CODES = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    std::string base64_encode(const std::string &s)
    {
        std::string ens;
        int val = 0, bits = -6;
        const unsigned int b63 = 0x3F;

        for (const auto &c : s)
        {
            val = (val << 8) + c;
            bits += 8;

            while (bits >= 0)
            {
                ens.push_back(BASE64_CODES[(val >> bits) & b63 ]);
                bits -= 6;
            }

        }

        if (bits > -6)
            ens.push_back(BASE64_CODES[( (val << 8) >> (bits + 8) ) & b63]);

        while(ens.size() % 4)
            ens.push_back('=');

        return ens;
    }
}


#endif

