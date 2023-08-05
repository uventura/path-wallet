// D E F I N E S
#define PATH_WALLET_STORAGE_PATH ".path_wallet.data"
#define TITLE_FOREGROUND 30
#define TITLE_BACKGROUND 104
#define TERMINAL "xfce4-terminal"

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

void clear_wallet(std::fstream& wallet)
{
   wallet.close();
   wallet.open(PATH_WALLET_STORAGE_PATH, std::ofstream::out | std::ofstream::trunc);
   wallet.close();
   wallet.open(PATH_WALLET_STORAGE_PATH, std::ios_base::app | std::ios_base::in);
}

bool name_already_exists(std::fstream& wallet, std::string name)
{
   PathWalletData result;
   std::string line;
   while(std::getline(wallet, line))
   {
      result = parse_wallet_data_string(line);
      if(result.name == name)
      {
         return true;
      }
   }
   
   wallet.clear();
   wallet.seekg(0);
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

PathWalletData remove_path(std::fstream& wallet, std::string name)
{
   PathWalletData result;

   std::string new_file_content = "";
   std::string line;
   bool founded = false;

   while(std::getline(wallet, line))
   {
      if(!founded)
      {
         result = parse_wallet_data_string(line); 
         founded = (bool)(result.name == name);
      }
      else
      {
         new_file_content += line + "\n";
      }
   }

   clear_wallet(wallet);
   wallet << new_file_content;

   return result;
}

void access_path(std::fstream& wallet, std::string name)
{
   std::string line;
   while(std::getline(wallet, line))
   {
      auto result = parse_wallet_data_string(line);
      if(result.name == name)
      {
         std::string command = "cd " + result.path;
         command += " && ";
         command += TERMINAL;
         system(command.c_str());
      }
   }
}


int main(int argc, char* argv[]) 
{
   std::fstream wallet_file(PATH_WALLET_STORAGE_PATH, std::ios_base::app | std::ios_base::in);

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
      else if(option == "-r")
      {
         PathWalletData result = remove_path(wallet_file, argv[2]);
         if(result.name != "NULL")
            std::cout << "[Path Wallet] Data > '" << result.name << ":" << result.path << "' Removed!" << std::endl;
      }
      else if(option == "-a")
      {
         access_path(wallet_file, argv[2]);
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
