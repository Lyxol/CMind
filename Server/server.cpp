#include <iostream>
#include <thread>
#include <sys/time.h>
#include <string>
#include <vector>
#include "socket.h"
#include "../game/theMind.h"

using namespace std;
using namespace stdsock;

void traitement(StreamSocket*,theMind*);

int main(int argc, char *argv[])
{
    int nbClients = 0;
    vector<thread> thPool;
    int port;
    if(argc!=2 || sscanf(argv[1], "%d", &port)!=1)
    {
            printf("usage: %s port\n", argv[0]);
            // default port, if none provided
            port= 3490;
    }

    ConnectionPoint *server=new ConnectionPoint(port);
    int err= server->init();
    if (err != 0) {
        std::cout << strerror(err) << std::endl;
        exit(err);
    }

    cout << "Waiting clients on port " << port << " ..." << endl;
    // accepting connexion
    // and preparing communication points
    StreamSocket *clients[5]; 
    //creating party
    theMind* Partie= new theMind(5);
    for (int i = 0; i < 5; i++)
    {
        clients[i]=server->accept();
        thPool.push_back(thread(traitement,ref(clients[i]),ref(Partie)));
    }

    size_t i = 0;
    while (i< thPool.size())
    {
        thPool[i].join();
    }
    
    
    delete server;
    cout << "stop\n";
    return 0;
}

void traitement(StreamSocket* client,theMind* Partie){
    cout<<Partie->gerCurrentNIv();
    cout<<Partie->getMaxNiv();
    cout<<Partie->getNbVie();

    while (Partie->gerCurrentNIv() != Partie->getMaxNiv() && Partie->getNbVie() != 0)
    {
        string CARD,PILE;
        string MAXS = to_string(Partie->getMaxStackValue());
        string MINS = to_string(Partie->getMinStackValue());
        Partie->giveCard(client);
        client->send(MAXS);
        for (int  i = 0; i < Partie->gerCurrentNIv(); i++)
        {
            client->read(CARD);
            client->read(PILE);
            if(PILE == "Max"){
                Partie->putOnMaxStack(CARD);
                cout<<"Max Stack:"<<Partie->getMaxStackValue()<<'\n';
            }else if(PILE == "Min"){
                Partie->putOnMinStack(CARD);
                cout<<"Min Stack:"<<Partie->getMinStackValue()<<'\n';
            }
        }
        Partie->endLevel();
    }
}
