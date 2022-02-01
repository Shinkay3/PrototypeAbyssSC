#include <eosio/eosio.hpp>
#include <eosiock/eosiock.hpp>

#pragma once
#include <eosio/serialize.hpp>
#include "eosiock/types.hpp"
#include <eosio/crypto.hpp>
#include <eosiock/rsa.hpp>

#include <eosiock/utils.hpp>

using namespace eosiock;

static class utilities
{
    public:

        static std::string hex_to_string(std::string data, int len)
        {
            std::string newString;
            for(int i=0; i< len; i+=2)
            {
                std::string byte = data.substr(i,2);
                char chr = (char) (int)strtol(byte.c_str(), nullptr, 16);
                newString.push_back(chr);
            }
            return newString;
        }

        static vector<std::string> split(std::string str, char del)
        {
            std::string temp = "";
            vector<std::string> temp2;
   
            for(int i=0; i<(int)str.size(); i++)
            {
                if(str[i] != del)
                {
                    temp += str[i];
                }
                else
                {
                    temp2.push_back(temp);
                    cout << temp << " ";
                    temp = "";
                }
            }
            temp2.push_back(temp);
            cout << temp;
            return temp2;
        }

    private:

};