// D E F I N E S
#define TITLE_FOREGROUND 30
#define TITLE_BACKGROUND 104

// I N C L U D E S
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <sstream>

// D A T A * R E P R E S E N T A T I O N
struct PathWalletData
{
   std::string name, path;
};

// M E T H O D S
PathWalletData parse_wallet_data_string(std::string entrypoint)
{
   char splitter = '|';
   std::stringstream substrings(entrypoint);
   
   PathWalletData result;
   std::getline(substrings, result.name, splitter);
   std::getline(substrings, result.path, splitter);

   return result;
}

bool name_already_exists(std::fstream& wallet, std::string name)
{
   PathWalletData result;
   std::string line;
   while(std::getline(wallet, line))
   {
      result = parse_wallet_data_string(line);
      if(result.name == name)
         return true;
   }
   return false;
}

bool create_path(std::fstream& wallet, std::string name, std::string path)
{
   if(path == ".")
   {
      path = std::filesystem::current_path();
   }

   if(name_already_exists(wallet, name))
   {
      std::cout << "[Path Wallet Error] Name Already Exists!" << std::endl;
      return false;
   }
   wallet << name << "|" << path << "\n"; 
   return true;
}

void list_paths(std::fstream& wallet)
{
   std::cout << "\033[" << TITLE_FOREGROUND << ";" << TITLE_BACKGROUND << "m Path Wallet \033[0m" << std::endl;

   std::string path_line;
   while(std::getline(wallet, path_line))
   {
     auto wallet_data = parse_wallet_data_string(path_line);
     std::cout << "    \033[1m" << wallet_data.name << "\033[0m : " << wallet_data.path << std::endl;
   }
}


int main(int argc, char* argv[]) 
{
   std::string path_storage = ".path_wallet.data";
   std::fstream wallet_file(path_storage, std::ios_base::app | std::ios_base::in);

   if(wallet_file.is_open())
   {
      std::string option(argv[1]);
      if(option == "-c")
      {
         if(create_path(wallet_file, argv[2], argv[3]))
            std::cout << "[Path Wallet] New Path Created!" << std::endl;
      }
      else if(option == "-l")
      {
         list_paths(wallet_file);
      }
      else
      {
         std::cout << "[ERROR] The given command doesn't exist" << std::endl;
         exit(-1);
      }

      wallet_file.close(); 
   }
   else
   {
      std::cout << "[Path Wallet] Error on file opening" << std::endl;
   }
}
