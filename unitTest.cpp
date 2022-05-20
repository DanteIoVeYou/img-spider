#include "TcpMudule.hpp"

int main(int argc, char* argv[])
{
  if(argc != 3) {
    return 1;
  }
  TcpModule* tcpm = new TcpModule(atoi(argv[2]), argv[1]);
  tcpm->TcpModuleStart();
  return 0;
}
