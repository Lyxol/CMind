#include <iostream>
#include <thread>
#include <sys/time.h>
#include <string>
#include <vector>
#include "socket.h"

using namespace stdsock;

class theMind
{
private:
    int idPartie;
    int nbVie;
    int maxNiveau;
    int nbShuriken = 1;
    int niveauActuel = 1;
    int listCard[98];
    int maxStack[50];//Pile croissonte
    int minStack[50];//Pile décroissante
public:
    theMind(int);
    ~theMind();

    void initialisation();
    void giveCard(StreamSocket*);
    void putOnMaxStack(std::string);
    void putOnMinStack(std::string);
    void endLevel();

    void setLevel(int );

    int getMaxStackValue();
    int getMinStackValue();
    int getMaxNiv(){
        return maxNiveau;
    };
    int gerCurrentNIv(){
        return niveauActuel;
    }
    int getNbVie(){
        return nbVie;
    }

};


