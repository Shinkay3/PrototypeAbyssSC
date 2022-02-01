#include <abyssnftgame.hpp>
#include <eosio/system.hpp>
#include <dataverification.hpp>
#include <utilities.hpp>
#include <xoshiro/XoshiroCpp.hpp>
#include <atomicassets/atomicassets-interface.hpp>

ACTION abyssnftgame::mintnft(name from)
{
  struct asset_s {
  } payload;

  int32_t template_id = 307868;
  int32_t asset_id = 1099526395482;
  
  action(
    permission_level(get_self(), "active"_n),
    "atomicassets"_n,
    "mintasset"_n,
    std::make_tuple(
      get_self(), // authorized_minter
      "abyssnftgame"_n, // collection_name
      "spirits"_n, // schema_name
      307868, // template_id
      "abyssnftgame"_n, // new_asset_owner
      std::map<std::string, atomicassets::ATOMIC_ATTRIBUTE>(), // immutable_data
      std::map<std::string, atomicassets::ATOMIC_ATTRIBUTE>(), // mutable_data
      std::vector<asset>() // tokens_to_back
    )
  ).send();
  
}


void abyssnftgame::regnft(name from, std::string collection, std::string schema, std::string template_id)
{
  nft_register_table nft_register(get_self(), get_self().value);
  auto msg = nft_register.find(from.value);
  if(msg == nft_register.end())
  {
    nft_register.emplace(from, [&](auto& msg) 
    {
      msg.user = from;
      msg.nft.push_back({collection, schema, template_id});
    });
  }
  else
  {
    nft_register.modify(msg, from, [&](auto& msg) {
      msg.nft.push_back({collection,schema, template_id});
      for (int i = 0; i < 10; ++i)
      {
        msg.nft.push_back({collection, schema, template_id});
      }

    });
  }
}

ACTION abyssnftgame::dailyquest(name from, std::string sig_in_hex, std::string data_in_hex)
{
  require_auth(from);
  bytes_view sig_in_bytes = from_hex(sig_in_hex);
  bytes_view data_in_bytes = from_hex(data_in_hex);
  check(dataverification::verify_signature(sig_in_bytes,data_in_bytes), ("Failed verifying signature"));

  std::string data_in_text = utilities::hex_to_string(data_in_hex,data_in_hex.length());
  vector<std::string> data_splitted = utilities::split(data_in_text,',');

  daily_quest_table daily_quest(get_self(), get_self().value);
  auto msg = daily_quest.find(from.value);
  if(msg == daily_quest.end())
  {
    daily_quest.emplace(from, [&](auto& msg) 
    {
      check((current_time_point() - msg.last_cleared) > seconds(5), "this action can only be performed once every day");
      //check(data_splitted[0] == from.to_string(), "Only the wallet connected to this account can perform this action");

      msg.user = from;
      msg.username = data_splitted[1];
      msg.quest_id = atoi(data_splitted[2].c_str());
      msg.clear_count = atoi(data_splitted[3].c_str());
      msg.last_cleared = current_time_point();
      //msg.days_after_epoch = (current_time_point().sec_since_epoch()) / (60*60*24);

    });
  }
  else
  {
    daily_quest.modify(msg, from, [&](auto& msg) 
    {
      check((current_time_point() - msg.last_cleared) > seconds(5), "this action can only be performed once every day");
      //check(data_splitted[0] == from.to_string(), "Only the wallet connected to this account can perform this action");
      check(atoi(data_splitted[3].c_str()) > msg.clear_count, "Something went wrong");

      msg.clear_count = atoi(data_splitted[3].c_str());
      msg.last_cleared = current_time_point();
      //msg.days_after_epoch = (current_time_point().sec_since_epoch()) / (60*60*24);
    });
  }

  const std::uint64_t seed = current_time_point().sec_since_epoch();
  XoshiroCpp::Xoshiro256PlusPlus rng(seed);
  std::uniform_int_distribution<int> dist(1, 10000);

  if(dist(rng) < 5000)
  {
    regnft(from,"aa","bb","cc");
    

    //tuple<name,std::string,std::string> test;
    //test = {from, "abyssnftgame", "spirits"};
    //action(
    //  permission_level{get_self(),"active"_n},
   //   get_self(),
    //  "regnfttest"_n,
    //  test
    //).send();
  }

  
}


ACTION abyssnftgame::dailyclear()
{
  require_auth(get_self());

    daily_quest_table daily_quest(get_self(), get_self().value);

    // Delete all records in _messages table
    auto msg_itr = daily_quest.begin();
    while (msg_itr != daily_quest.end()) {
      msg_itr = daily_quest.erase(msg_itr);
    }    
}

ACTION abyssnftgame::regnftclear()
{
  require_auth(get_self());

    nft_register_table nft_register(get_self(), get_self().value);

    // Delete all records in _messages table
    auto msg_itr = nft_register.begin();
    while (msg_itr != nft_register.end()) {
      msg_itr = nft_register.erase(msg_itr);
    }    
}
