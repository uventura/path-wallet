#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

void create_path(std::ofstream& wallet, std::string name, std::string path)
{
   if(path == ".")
   {
      path = std::filesystem::current_path();
   }
   wallet << name << "|" << path << "\n"; 
}

int main(int argc, char* argv[]) 
{
   std::string path_storage = ".path_wallet.data";
   std::ofstream wallet_file(path_storage, std::ios_base::app);

   if(wallet_file.is_open())
   {
      std::string option(argv[1]);
      if(option == "-c")
      {
         create_path(wallet_file, argv[2], argv[3]);
         std::cout << "[Path Wallet] New Path Created!" << std::endl;
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
