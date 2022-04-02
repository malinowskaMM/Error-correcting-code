#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

using namespace std;
const int X = 8;
const int Y = 16;


bool H[X][Y]{{1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
             {1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0},
             {1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0},
             {1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0},
             {0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0},
             {0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0},
             {0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0},
             {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1}};


vector<bool> stringToBoolean(string text) {     //funkcja konwertuje string postaci binarnej do vectora wartosci logicznych
    vector<bool> booleanText;                   //unikamy zwielokrotlenia wagi pliku przez przechodznie przez stringa
    for(int i = 0; i < text.size(); i++) {
        if(text[i] == '1') booleanText.push_back( 1 );
        else booleanText.push_back( 0 );
    }
    return booleanText;
}

vector<bool> asciiToBinary(string text) {       //funkcja zamienia znaki ascii na ciąg binarny działajac na poszczegolnych znakach
    vector<bool> binary;
    int sign;
    for(int i = 0; i < text.size(); i++) {
        sign = text[i];
        for( int n = 0; n < 8; n++) {
            binary.push_back( sign%2 );
            sign = sign/2;
        }
    }
    return binary;
}

char binaryToAscii(vector<bool> binary) {       //funkcja zamienia ciąg binarny na jeden znak ascii
    char character = 0;                         // 1 znak ascii = 8 bitów
    for(int i = 0; i < 8; i++) {
        character += binary[i]*pow(2, i);
    }
    return character;
}

int calculateParityBit(vector<bool> message, int rowNumber) { //funkcja oblicza bit parzystości na postawie macierzy h
    int parityBit = 0;
    for(int i = 0; i < message.size(); i++) {
        parityBit += H[rowNumber][i] * message[i];
    }
    parityBit = parityBit % 2;
    return parityBit;
}

void addParityBites(vector<bool> &message) {
    vector<bool> copy = message;
    for(int i = 0; i< X; i++) {
        message.push_back(calculateParityBit(copy, i));
    }
}

vector<bool> coding(vector<bool> message) {
    vector<bool> code;
    vector<bool> bits;
    for(int i = 0; i < message.size(); i++) {
        code.push_back(message[i]);
        bits.push_back((message[i]));
        if((i+1)%8 == 0 && i != 0) {
            addParityBites(bits);
            for(int n = 0; n < X; n++)
                code.push_back(bits[n+8]);
            bits.clear();
        }
    }
    return code;
}

void correctMistakes(vector<bool> &incorrect, vector<bool> E) {
    bool correction = false;
    for(int i = 0; i < Y; i++) {
        for(int j = 0; j < X; j++) {
            if(E[j] == H[j][i]) correction = true;
            else {
                correction = false;
                break;
            }
        }
        if (correction) incorrect[i] = (~incorrect[i]) % 2;
    }
}

void verify(vector<bool> &message, int length) {
    bool verification = true;
    if(message.size() != length) cout << "WRONG NUMBER OF BITS" << endl;
    vector<bool> E;
    int number;
    for(int i = 0; i < X; i++) {
        number = calculateParityBit(message,i);
        E.push_back(number);
        if(number == 1) verification = false;
    }
    if(!verification) correctMistakes(message, E);
}

int main() {
    vector<bool> message;
    vector<bool> encoded;
    vector<bool> incorrect;

    ifstream plik_wej;
    ofstream plik_binarny;
    fstream plik_zakodowany;
    string text;
    int size;

    plik_wej.open("C:\\Users\\madzi\\CLionProjects\\Zad1_telekom\\wyslane.txt");
    plik_binarny.open("C:\\Users\\madzi\\CLionProjects\\Zad1_telekom\\binarnieWyslane.txt");
    plik_zakodowany.open("C:\\Users\\madzi\\CLionProjects\\Zad1_telekom\\zakodowane.txt", ios::out);
    
    
  bool shouldContinue = true;
  int caseNumber;
    while(shouldContinue) {
        cout << "0. Zakoncz         1. Zakoduj         2. Dekoduj" << endl;
        cin >> caseNumber;
        switch(caseNumber) {
            case 1: {
                while(!plik_wej.eof()) getline(plik_wej, text);
                size = text.size();

                message = asciiToBinary(text);
                for(int i = 0; i < message.size(); i++) {
                    plik_binarny << message[i];
                }

                encoded = coding(message);
                for(int i =0; i < encoded.size(); i++) {
                    plik_zakodowany << encoded[i];
                }

                plik_wej.close();
                plik_binarny.close();
                plik_zakodowany.close();
                break;
            }
            case 2: {
                plik_zakodowany.open("C:\\Users\\madzi\\CLionProjects\\Zad1_telekom\\zakodowane.txt", ios::in);
                while(!plik_zakodowany.eof()) {
                    getline(plik_zakodowany, text);
                }
                incorrect = stringToBoolean(text);

                vector<vector<bool>> twoDimVector;
                for(int i = 0; i < size; i++) {
                    vector<bool> row;
                    twoDimVector.push_back(row);
                }

                vector<bool> correct;
                int number = 0;
                string answer;

                for(int i = 0; i < incorrect.size(); ++i) {
                    twoDimVector[number].push_back(incorrect[i]);
                    if( (i+1)%16 == 0 ) number++;
                }
                incorrect.clear();

                fstream plik_wyj;
                plik_wyj.open("C:\\Users\\madzi\\CLionProjects\\Zad1_telekom\\odebrane.txt", ios::out);

                for(int i = 0; i < size; i++) {
                    verify(twoDimVector[i],encoded.size()/size);
                    incorrect.insert(incorrect.end(), twoDimVector[i].begin(), twoDimVector[i].end());
                    answer += binaryToAscii(twoDimVector[i]);
                    plik_wyj << binaryToAscii(twoDimVector[i]);
                }

                plik_zakodowany.close();
                plik_wyj.close();
                break;
            }
            case 0:
                shouldContinue = false;
                break;
        };
    }

    return 0;
}
