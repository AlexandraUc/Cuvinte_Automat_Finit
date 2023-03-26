#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
using namespace std;

/*
    Format fisier: lungime maxima cuvinte, nr stari, card alfabet, stare initiala, 
                   nr stari finale, stari finale, nr tranzitii, tranzitii
    Format linie matrice: stare de plecare, stare in care ajungem, litera (nimic pentru lambda) 
*/

class Automat{
    int nrStari, cardAlfabet, nrStariFinale, tranzitii, lungimeCuvinte;
    char stareCurenta;
    char* stariFinale;
    char** matriceAutomat;
    string drum;
    bool ok;

    void citireAutomat(const string&);
    void toateCuvintele(int, char, string, string);
public:
    Automat(const string&);
    Automat(const Automat&);
    Automat& operator = (const Automat&);

    void cheamaFunctie();

    ~Automat();
};

// Constructor
Automat::Automat(const string& numeFisier = "in.txt"){
    this->drum = "";
    this->ok = 0;
    this->citireAutomat(numeFisier);
}

// Copy constructor
Automat::Automat(const Automat& obj){
    this->nrStari = obj.nrStari;
    this->cardAlfabet = obj.cardAlfabet;
    this->nrStariFinale = obj.nrStariFinale;
    this->tranzitii = obj.tranzitii;
    this->lungimeCuvinte = obj.lungimeCuvinte;
    this->stareCurenta = obj.stareCurenta;
    this->drum = obj.drum;
    this->ok = obj.ok;

    this->stariFinale = new char[strlen(obj.stariFinale) + 1];
    strcpy(this->stariFinale, obj.stariFinale);

    this->matriceAutomat = new char*[this->tranzitii];
    for(int i = 0; i < obj.tranzitii; i++){
        this->matriceAutomat[i] = new char[4];
        strcpy(this->matriceAutomat[i], obj.matriceAutomat[i]);
    }
}

// Operatorul de atribuire 
Automat& Automat::operator = (const Automat& obj){
    if(this != &obj){
        this->nrStari = obj.nrStari;
        this->cardAlfabet = obj.cardAlfabet;
        this->nrStariFinale = obj.nrStariFinale;
        this->tranzitii = obj.tranzitii;
        this->lungimeCuvinte = obj.lungimeCuvinte;
        this->stareCurenta = obj.stareCurenta;
        this->drum = obj.drum;
        this->ok = obj.ok;

        this->stariFinale = new char[strlen(obj.stariFinale) + 1];
        strcpy(this->stariFinale, obj.stariFinale);

        this->matriceAutomat = new char*[this->tranzitii];
        for(int i = 0; i < obj.tranzitii; i++){
            this->matriceAutomat[i] = new char[4];
            strcpy(this->matriceAutomat[i], obj.matriceAutomat[i]);
        }
    }
    return *this;
}

// Citeste din fisier
void Automat::citireAutomat(const string& numeFisier){
    ifstream f(numeFisier);
    if(!f){
        cout << "Nu exista fisierul\n";
        exit(1);
    }

    f >> this->lungimeCuvinte >> this->nrStari >> this->cardAlfabet >> this->stareCurenta >> this->nrStariFinale;
    this->drum += this->stareCurenta;

    this->stariFinale = new char[this->nrStariFinale];
    for(int i = 0; i < this->nrStariFinale; i++)
        f >> this->stariFinale[i];
    this->stariFinale[this->nrStariFinale] = '\0';

    f >> this->tranzitii;
    this->matriceAutomat = new char*[this->tranzitii];
    for(int i = 0; i < this->tranzitii; i++)
        this->matriceAutomat[i] = new char[4];

    string linie;
    getline(f, linie); // Se citeste linia goala
    for(int i = 0; i < this->tranzitii; i++){
        getline(f, linie);
        this->matriceAutomat[i][0] = linie[0];
        this->matriceAutomat[i][1] = linie[2];
        if(linie.size() > 3)
            this->matriceAutomat[i][2] = linie[4];
        else
            this->matriceAutomat[i][2] = ' ';
        this->matriceAutomat[i][3] = '\0';
    }
    f.close();
}

// Cheama functia de afisare a cuvintelor
void Automat::cheamaFunctie(){
    this->ok = 0;
    this->toateCuvintele(this->lungimeCuvinte, this->stareCurenta, "", this->drum);
    if(this->ok == 0)
        cout << "Nu se accepta niciun cuvant de lungime " << this->lungimeCuvinte << endl;
}

// Afiseaza toate cuvintele (+drumul) de o lungime maxima data
void Automat::toateCuvintele(int lungimeCuvinte, char stareCurenta, string cuvant, string drum){
    if(lungimeCuvinte >= 0){
        for(int i = 0; i < this->nrStariFinale; i++)
            if(stareCurenta == this->stariFinale[i]){
                this->ok = 1;
                cout << cuvant << ' ' << drum << endl;
            }
    }
    if(lungimeCuvinte > 0){
        for(int i = 0; i < this->tranzitii; i++){
            if(stareCurenta == this->matriceAutomat[i][0])
                if(this->matriceAutomat[i][2] != ' ')
                    toateCuvintele(lungimeCuvinte - 1, this->matriceAutomat[i][1], 
                        cuvant + this->matriceAutomat[i][2], drum + this->matriceAutomat[i][1]);
                else
                    toateCuvintele(lungimeCuvinte, this->matriceAutomat[i][1], 
                        cuvant, drum + this->matriceAutomat[i][1]);
        }
    } 
}

// Destructor
Automat::~Automat(){
    if(this->stariFinale != NULL){
        delete[] this->stariFinale;
        this->stariFinale = NULL;
    }

    for(int i = 0; i < this->tranzitii; i++)
        if(this->matriceAutomat[i] != NULL){
            delete[] this->matriceAutomat[i];
            this->matriceAutomat[i] = NULL;
        }
    if(this->matriceAutomat != NULL){
        delete[] this->matriceAutomat;
        this->matriceAutomat = NULL;
    }
}

int main(){
    string numeFisier2 = "in2.txt";
    Automat a, a2(numeFisier2);
    a.cheamaFunctie();
    //a2.cheamaFunctie();
    return 0;
}