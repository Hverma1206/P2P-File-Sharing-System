#include "peers.hpp"
#include <cstdlib>

int main(int argc, char* argv[]){
    if(argc >= 2){
        g_peer_port = (uint16_t)atoi(argv[1]);
    }
    client client_instance;
    if(client_instance.execute()!= 0){
        perror("server execution fault");
    }
    return 0;
}
