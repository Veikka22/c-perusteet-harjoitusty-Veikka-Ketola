// Veikka Ketola 24tietoa
// Harjoitustyö

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib> // rand() ja srand()
#include <ctime>   // Satunnaislukugeneraattori
#include <map>     // Varausten tallentamiseen haun avulla
#include <limits>  // Numeeriset rajoitukset (käytetään syötteen vahvistamisessa)

using namespace std;

// Toiminto, joka luo satunnaisluvun annetulla alueella
int SatunnainenLuku(int min, int max) {
    return rand() % (max - min + 1) + min;
}

// Toiminto, joka tarkastaa, että onko huone vapaa
bool OnkoHuoneVapaa(const vector<bool>& Huoneet, int Huonenumero) {
    return !Huoneet[Huonenumero - 1];
}

// Saadaan kelvollinen kokonaisluku
int Kokonaisluku(const string& prompti, int min, int max) {
    int value;
    while (true) {
        cout << prompti;
        cin >> value;

        // Tarkistetaan syöte
        if (cin.fail() || value < min || value > max) {
            cout << "Virheellinen syote. Anna kokonaisluku valilla " << min << "-" << max << "." << endl;
            cin.clear(); // Clear error state
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Hylätään virheellinen syöte
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Tyhjennetään input buffer
            return value;
        }
    }
}

// Toiminto kelvollisen string syötteen saamiseksi
string StringSyote(const string& prompti) {
    string syote;
    cout << prompti;
    getline(cin, syote);

    // Tarkistetaan ettei syöte ole tyhjä
    while (syote.empty()) {
        cout << "Syote ei voi olla tyhja. Yrita uudelleen." << endl;
        cout << prompti;
        getline(cin, syote);
    }
    return syote;
}

int main() {
    srand(time(0)); // Satunnaislukugeneraattori

    const int MaxHuoneet = SatunnainenLuku(150, 300); // Huoneiden numerot (150-300)
    vector<bool> Huoneet(MaxHuoneet, false); // Huoneen saatavuus
    vector<int> Hinnat(MaxHuoneet);    // Huoneiden hinnat
    map<int, string> Varaukset;       // Säilyttää varausnumeron ja nimen
    int Kustannukset = 0;                   // Kokonaiskustannukset

    // Määrittää jokaiselle huoneelle satunnaiset hinnat välillä 80-100
    for (int i = 0; i < MaxHuoneet; i++) {
        Hinnat[i] = SatunnainenLuku(80, 100);
    }

    cout << "Tervetuloa hotelliin! Huoneita on " << MaxHuoneet << " (numerot 1-" << MaxHuoneet << ")." << endl;

    while (true) {
        cout << "\nValitse toiminto:\n"
            << "1. Tee varaus\n"
            << "2. Hae varausta varausnumerolla\n"
            << "3. Hae varausta varaajan nimella\n"
            << "4. Lopeta\n";
        int Valinta = Kokonaisluku("Valintasi: ", 1, 4);

        if (Valinta == 4) break; // Poistu ohjelmasta

        if (Valinta == 1) {
            int Huonenumero, yot;
            string nimi;
            int VarausNumero;

            // Pyytää käyttäjän nimen
            nimi = StringSyote("Anna varaajan nimi: ");

            // Pyytää käyttäjän valitsemaan huoneen
            Huonenumero = Kokonaisluku("Valitse huonenumero (1-" + to_string(MaxHuoneet) + "): ", 1, MaxHuoneet);

            // Tarkistaa onko huone vapaa
            if (!OnkoHuoneVapaa(Huoneet, Huonenumero)) {
                cout << "Huone " << Huonenumero << " on jo varattu. Valitse toinen huone." << endl;
                continue;
            }

            // Näyttää huoneen hinnan
            cout << "Huone " << Huonenumero << " maksaa " << Hinnat[Huonenumero - 1] << " euroa per yo." << endl;

            // Kysy öiden määrää
            yot = Kokonaisluku("Kuinka moneksi yoksi haluat varata huoneen " + to_string(Huonenumero) + ": ", 1, 365);

            // Luo satunnaisen varausnumeron
            do {
                VarausNumero = SatunnainenLuku(10000, 99999);
            } while (Varaukset.find(VarausNumero) != Varaukset.end()); // Tarkistaa ettei tule samaa varausnumeroa kuin jossain muussa

            // Varaa huoneen
            Huoneet[Huonenumero - 1] = true;
            int Hinta = yot * Hinnat[Huonenumero - 1];
            Kustannukset += Hinta;

            // Tallentaa varauksen
            Varaukset[VarausNumero] = nimi;
            cout << "Varaus tehty onnistuneesti! Varausnumero: " << VarausNumero << endl;
            cout << "Huone " << Huonenumero << " varattu " << yot << " yoksi. Hinta: " << Hinta << " euroa." << endl;

        }
        else if (Valinta == 2) {
            int Etsiluku = Kokonaisluku("Anna varausnumero: ", 10000, 99999);

            if (Varaukset.find(Etsiluku) != Varaukset.end()) {
                cout << "Varausnumero: " << Etsiluku << ", Varaajan nimi: " << Varaukset[Etsiluku] << endl;
            }
            else {
                cout << "Varausta ei loytynyt annetulla varausnumerolla." << endl;
            }

        }
        else if (Valinta == 3) {
            string Etsinimi = StringSyote("Anna varaajan nimi: ");

            bool Loytyyko = false;
            for (const auto& res : Varaukset) {
                if (res.second == Etsinimi) {
                    cout << "Varausnumero: " << res.first << ", Varaajan nimi: " << res.second << endl;
                    Loytyyko = true;
                }
            }

            if (!Loytyyko) {
                cout << "Varausta ei loytynyt annetulla nimella." << endl;
            }
        }
    }

    cout << "Varausten loppusumma: " << Kustannukset << " euroa." << endl;
    cout << "Kiitos ja hyvaa paivanjatkoa!" << endl;

    return 0;
}
