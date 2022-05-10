#include <iostream>
#include <thread>
#include <vector>
#include <sys/time.h>
#include "socket.h"

using namespace std;
using namespace stdsock;

string playCard (vector<string>);
void showCard(vector<string>);


int main(int argc, char* argv[])
{
    int port;
    vector<string> PileJoueur;
    string CARD,PILE,MAXS,MINS;
    string response;

    if(argc!=3)
    {
        printf("usage: %s server_address port\n", argv[0]);
        return 0;
    }

    if(sscanf(argv[2], "%d", &port)!=1)
    {
        printf("usage: %s server_address port\n", argv[0]);
        return 1;
    }

    //
    cout << "Attemping to reach " << argv[1] << ":" << port << endl;
    StreamSocket *sock=new StreamSocket(std::string(argv[1]),port);

    int err= sock->connect();
    if (err!=0) {
        delete sock;
        perror("[-]Error in connection: ");
        return(err);
    }
    cout << "[+]Connected to Server.\n";

    while (PileJoueur.empty())
    {
        sock->read(CARD);
        sock->send("card receive");
        while (CARD != "NoCard")
        {
            PileJoueur.push_back(CARD);
            cout<<"Carte reçu: "<<CARD<<'\n';
            CARD = "";
            sock->read(CARD);
            sock->send("card receive");
        }
        sock->read(MAXS);
        cout<<MAXS<<'\n';
        playCard(PileJoueur);
        cout<<"Sur la pile:[Max/Min]";
        cin>>PILE;//Add verif
        sock->send(CARD);
        sock->send(PILE);   
    }

    delete sock;
    cout << "stop\n";

    return 0;
}
string playCard (vector<string> listCard){
    string cardToPlay;
    bool canBePlay = false;
    while (canBePlay == false)
    {
        showCard(listCard);
        cout<<"Quel carte voulez-vous jouer ?\n";
        cin>>cardToPlay;   
        for (size_t i = 0; i < listCard.size(); i++)
        {
            if(listCard.at(i) == cardToPlay){
                canBePlay = true;
                listCard.erase(listCard.begin()+i);
                break;
            }
        }
        cout<<"Carte non présente dans votre jeu \n";
    }
    return cardToPlay;
}

void showCard(vector<string> listCard){
    cout<<"Votre main: ";
    for (size_t i = 0; i < listCard.size(); i++)
    {
        cout<<listCard.at(i)<<" ";
    }
    cout<<'\n';
}