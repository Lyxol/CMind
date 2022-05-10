#include <iostream>
#include <thread>
#include <sys/time.h>
#include <string>
#include <vector>
#include "socket.h"
#include "theMind.h"

using namespace std;
using namespace stdsock;

theMind::theMind(int nbPlayer)
{
    if(nbPlayer <= 2){
        maxNiveau = 12;
    }else if(nbPlayer == 3){
        maxNiveau = 10;
    }else{
        maxNiveau = 8;
    }
    nbVie = nbPlayer;
    maxStack[0] = 1;
    minStack[0]=100;
    initialisation();
}

void theMind::initialisation(){
    for(int i=1;i<50;i++){
        maxStack[i] = 0;
        minStack[i] = 0;
    }
    for (int j = 0; j < 98; j++)
    {
        listCard[j] = 0;
    }
    
}

theMind::~theMind()
{
    cout<<"Destruction du game\n";
}

void theMind::giveCard(StreamSocket* client){
    srand(time(NULL));
    string CARD;
    string receive;
    for (int i = 0; i < niveauActuel; i++)
    {
        int cardValue = rand()%100;
        while (listCard[cardValue-2] == cardValue || cardValue<2)
        {
            cardValue = rand()%100;
        }
        listCard[cardValue-2] = cardValue;
        CARD = to_string(cardValue);
        cout<<"Card send: "<<CARD<<'\n';
        client->send(CARD);
        client->read(receive);
    }
    CARD = "NoCard";
    client->send(CARD);
    client->read(receive);

}

void theMind::putOnMaxStack(string CARD){
    int value = stoi(CARD);
    int i = 0;
    while(maxStack[i] != 0 && i<50){
        i++;
    }
    if(i >= 50){
        cout<<"Partie terminée\n";
    }
    else if(value<maxStack[i-1]){
        cout<<"carte trop petite\n";
    }else{
        for (int j = 0; j < 98; j++)
        {
            if(listCard[j]>maxStack[i-1] && listCard[j]<value){
                cout<<"-1 Vue\n";
                nbVie--;
                break;
            }
        }
        maxStack[i] = value;
        listCard[value-2] = 0;
    }
}

void theMind::putOnMinStack(string CARD){
    int value = stoi(CARD);
    int i = 0;
    while(minStack[i] != 0 && i<50){
        i++;
    }
    if(i >= 50){
        cout<<"Partie terminée\n";
    }
    else if(value>minStack[i-1]){
        cout<<"Valeur trop grande\n";
    }else{
        for (int j = 100; j > 98; j--)
        {
            if(listCard[j]<minStack[i-1] && listCard[j]>value){
                cout<<"-1 Vue\n";
                nbVie--;
                break;
            }
        }
        
        minStack[i] = value;
        listCard[value-2] = 0;
    }
}

void theMind::endLevel(){
    niveauActuel++;
}

void theMind::setLevel(int level){
    if(level>0 && level<maxNiveau){
        niveauActuel = level;
    }else{
        cout<<"Le niveau ne peut pas être en dessous de 0 ou dépassé "<<maxNiveau<<'\n';
    }
}

int theMind::getMaxStackValue(){
    for(int i=0;i<50;i++){
        if (maxStack[i] == 0)
        {
            return maxStack[i-1];
        }
        
    }
    return maxStack[49];
}

int theMind::getMinStackValue(){
    for (int i = 0; i < 50; i++)
    {
        if(minStack[i] == 0){
            return minStack[i-1];
        }
    }
    return minStack[49];
}