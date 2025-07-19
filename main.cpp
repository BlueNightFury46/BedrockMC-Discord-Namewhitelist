#include <dpp/appcommand.h>
#include <dpp/cluster.h>
#include <dpp/dispatcher.h>
#include <dpp/dpp.h>
#include <dpp/misc-enum.h>
#include <dpp/once.h>
#include <string>
#include <curl/curl.h>
#include <curl/easy.h>

#define BOT_TOKEN ""

const std::string UUID_START = "00000000-0000-0000-0009-";

const std::string GEYSER_URL = "https://api.geysermc.org/v2/xbox/xuid/";

std::string GET(std::string url);

size_t write_data(char* buffer, size_t size, size_t nmemb, std::string* userp){
  for(int i = 0; i < nmemb; ++i){
    *userp+=(buffer)[i];
  }

  return (size*nmemb);
}

std::string GET_XUID(std::string username){

  std::string raw_data = GET(GEYSER_URL+username);

  int pos = raw_data.find(":");
  std::string reconstructed_data = "";

  if(pos==std::string::npos){
    return "ERROR";
  }
  
  for(int i = pos+1; i < (raw_data.length()-1); ++i){

    reconstructed_data+=raw_data[i];

  }

  return reconstructed_data;
}; 

std::string GET(std::string url){
  CURL* curl = curl_easy_init();

  if(!curl){
    return "ERROR";
  }

  std::string data;

  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

  CURLcode code = curl_easy_perform(curl);
  
  if(code!=CURLE_OK){
    return "ERROR";
  }

  curl_easy_cleanup(curl);

  return data;
}




int main (int argc, char *argv[]) {


  dpp::cluster bot(BOT_TOKEN);

  dpp::command_option player_opt(dpp::command_option_type::co_string, "player", "the Bedrock player name without prefix (such as BlueNightFury46)", true);

  dpp::slashcommand genplayer("genplayer", "generates Bedrock player json", bot.me.id);
  genplayer.add_option(player_opt);


  bot.on_log(dpp::utility::cout_logger());

  bot.on_slashcommand([&genplayer](const dpp::slashcommand_t& event){
    if(event.command.get_command_name()==genplayer.name){

     auto player = event.get_parameter("player");

      if(player.index()!=0){
        std::string data = std::get<std::string>(player);

        std::string reply_value = "```json\n{\n\"uuid\": \"" + UUID_START + GET_XUID(data) + "\",\n" + "\"name\": \"."+data+"\"\n}    ```";

        event.reply(reply_value);
      } else{
        event.reply("An error occured");
      }
      


    }
  });

  bot.on_ready([&bot, &genplayer](const dpp::ready_t& event){
    bot.log(dpp::loglevel::ll_info, "Started bot");
    
    if(dpp::run_once<struct command_register_>()){
      bot.global_command_create(genplayer);
    }
  });

  bot.start(dpp::st_wait);

  return 0;
}
