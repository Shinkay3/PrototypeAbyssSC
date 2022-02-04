#include <eosio/eosio.hpp>
#include <eosiock/eosiock.hpp>

using namespace std;
using namespace eosio;

CONTRACT abyssnftgame : public contract {

  public:
    using contract::contract;


    ACTION regnft(name from,string collection, string schema, int32_t template_id);
    ACTION regnftclear();
    ACTION mintnft(name from, name collection, name schema, int32_t template_id);


    ACTION dailyquest(name from, string sig, string data);
    ACTION dailyclear();
    //ACTION createnft();
    //ACTION unregnft(name from, string collection);

  private:

    struct nft
    {
      string collection, schema;
      int32_t template_id;
    };

    TABLE nft_registerv1
    {
      name user;
      vector<nft> nft;
      auto primary_key() const { return user.value;}
    };
    typedef multi_index<name("nftstore"), nft_registerv1> nft_register_table;

    TABLE daily_questv2
    {
      name user;
      string username;
      int quest_id;
      int clear_count;
      time_point_sec last_cleared;
      //int days_after_epoch;
      //string data;
      //unsigned char rsa;
      auto primary_key() const { return user.value;}

    };
    typedef multi_index<name("dailyquest"), daily_questv2> daily_quest_table;


};
