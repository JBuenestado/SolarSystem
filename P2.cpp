// Code by Jorge Buenestado, 22/02/2023

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>

using namespace std;

void CSV(string arch);

int main() {
    int i, j, k;
    double A[10][7], P[10][2], Ac[10][2], V[10][2], W[10][2], T[10][2], E[10][3];
    double  pasos, n, timer, Ty, Ts, h, Ene, SumaE, geox, geoy, NumeroAstros;
    bool pi, helio;
    ifstream fich;
    ofstream print, pr, pr0, pr1, pr2, pr3, pr4, pr5, pr6, pr7, pr8, pr9, pre, Pr[10];
    string Astros[10] = {"PSun.txt.txt", "PMercury.txt", "PVenus.txt", "Ptierra.txt", "PMars.txt",
                         "PJupiter.txt", "PSaturn.txt", "PUranus.txt", "PNeptune.txt", "Pluto.txt"};
    NumeroAstros = 10;
    //definimos nuestras constantes
    const double G = 6.67e-11;
    const double Ms = 1.99e30;
    const double c = 1.496e11;

    //Abrimos los archivos
    print.open("Periodos y Prueba.txt");
    pre.open("Energia.txt");

    for(i = 0; i < NumeroAstros; i++){
        Pr[i].open(Astros[i]);
    }

    //leemos el fichero con los datos
    fich.open("./planetas.txt");
    if(fich.is_open()) cout<<"File opened successfuly."<<endl; 
    else cout<<"Error reading file."<<endl;
    for (i = 0; i < 10; i++) { for (j = 0; j < 7; j++)fich >> A[i][j]; }
    fich.close();

    //Probamos que se ha leido bien los datos del fichero, identificando cada número
    print << "//ID       //Radius(AU) //Mass(Ms)   //Excentricity//Period   // Radius(km) // Velocity" << endl;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 7; j++)print << scientific << setprecision(6) << A[i][j] << "   ";
        print << " " << endl;
    }

    //parametros de entrada
    cout << "Introduzca tiempo de la simulación en años" << endl;
    //cin >> Ty;
    Ty = 5;
    //años a segundos
    Ts = Ty * 24 * 365 * 3600;
    cout << "Introduzca timestep en días" << endl;
    //cin >> h;
    h = 1;
    h = h * 24 * 3600;

    //Cambio Variables
    Ts = Ts * sqrt(G * Ms / (c * c * c));
    h = h * sqrt(G * Ms / (c * c * c));
    pasos = Ts / h;
    //Cambiamos de unidades del fichero
    for (i = 0; i < 10; i++) {
        A[i][1] = A[i][1] * 10e8 / c; //radio
        A[i][2] = A[i][2] * 10e24 / Ms; //masa
        A[i][4] = A[i][4] * sqrt(G * Ms / (c * c * c)); //periodo
        A[i][5] = A[i][5] / 2; //radio
        A[i][6] = A[i][6] * 1000 / sqrt(G * Ms / c); //velocidad m*s
    }

    //Pi = posiciones iniciales, en una recta o en los cuatro ejes si true
    pi = false;
    //sistema de referencia heliocentrico o geocentrico
    helio = false;

    // Posiciones&velocidades iniciales
    if (pi) {
        for (i = 1; i < 3; i++) {
            P[i][1] = A[i][1];
            V[i][0] = -A[i][6];
            P[i][0] = 0;
            V[i][1] = 0;
        }
        for (i = 3; i < 5; i++) {
            P[i][0] = A[i][1];
            V[i][1] = A[i][6];
            P[i][1] = 0;
            V[i][0] = 0;
        }
        for (i = 5; i < 7; i++) {
            P[i][1] = -A[i][1];
            V[i][0] = -A[i][6];
            P[i][0] = 0;
            V[i][1] = 0;
        }
        for (i = 7; i < 9; i++) {
            P[i][0] = -A[i][1];
            V[i][1] = A[i][6];
            P[i][1] = 0;
            V[i][0] = 0;
        }
    } else {
        for (i = 1; i < 10; i++) {
            P[i][0] = A[i][1];
            V[i][1] = -A[i][6];
            P[i][1] = 0;
            V[i][0] = 0;
        }
    }

    //Sun position
    P[0][0] = 0;
    P[0][1] = 0;

    //Initialize to 0 some parameters
    for (i = 1; i < 10; i++) {
        for (k = 0; k < 2; k++) {
            Ac[i][k] = 0;
            T[i][k] = 0;
        }
    }
    timer = 0;

    //bucle k para eje x y para eje y //i coordinate means planet, j coordinates indicates x or y
    //Aceleración Inicial
    for (k = 0; k < 2; k++) {
        for (i = 1; i < 10; i++) {
            for (j = 0; j < 10; j++) {
                if (i != j) {
                    Ac[i][k] -= A[j][2] * (P[i][k] - P[j][k]) /
                                (pow(sqrt((pow(P[i][0] - P[j][0], 2)) + pow(P[i][1] - P[j][1], 2)), 3));
                }
            }
        }
    }
    //hacemos los pasos necesarios
    for (n = 1; n < pasos; n++) {

        //Posiciones nuevas
        for (i = 1; i < 10; i++) {
            for (k = 0; k < 2; k++) {
                P[i][k] = P[i][k] + h * V[i][k] + h * h * Ac[i][k] / 2;
                W[i][k] = V[i][k] + h * Ac[i][k] / 2;
            }
        }

        //Imprimimos la posicion de cada planeta segun  sistema de referencia
        if (!helio) {
            geox = P[3][0];
            geoy = P[3][1];
        } else {
            geox = 0;
            geoy = 0;
        }

        for (i = 1; i < 10; i++) {
            Pr[i] << P[i][0] -geox << " " << P[i][1]- geoy <<endl;
        };

        //Aceleracion Nueva
        for (i = 1; i < 10; i++) { for (k = 0; k < 2; k++) { Ac[i][k] = 0; }}

        for (k = 0; k < 2; k++) {
            for (i = 1; i < 10; i++) {
                for (j = 0; j < 10; j++) {
                    if (i != j) {
                        Ac[i][k] -= A[j][2] * (P[i][k] - P[j][k]) /
                                    (pow(sqrt((pow(P[i][0] - P[j][0], 2)) + pow(P[i][1] - P[j][1], 2)), 3));
                    }
                }
            }
        }

        //Velocidad nueva
        for (i = 1; i < 10; i++) {
            for (k = 0; k < 2; k++) {
                V[i][k] = W[i][k] + h * Ac[i][k] / 2;
            }
        }
        timer += h;

        //Calculo de periodo
        for (i = 1; i < 10; i++) {
            if (T[i][0] == 0) {
                if (P[i][1] >= 0) {
                    T[i][0] = 1;
                    T[i][1] = 2 * timer;
                }
            }
        }

        for (i = 1; i < 10; i++) {
            for (k = 0; k < 3; k++) {
                E[i][k] = 0;
            }
        }

        //Calculo de Energía
        SumaE = 0;
        Ene = 0;
        for (i = 1; i < 10; i++) {
            for (j = 0; j < 10; j++) {
                if (i != j) {
                    //Potencial
                    E[i][0] +=
                            G * (A[i][2] * A[j][2]) / (sqrt((pow(P[i][0] - P[j][0], 2)) + pow(P[i][1] - P[j][1], 2)));
                    Ene += E[i][1];
                }
                //Cinética
                E[i][1] = A[i][2] * (V[i][0] * V[i][0] + V[i][1] * V[i][1]) / (2 - Ene);
                SumaE += E[i][1];
            }
        }
        //Sacamos el output al fichero de energía con tiempo, reconvirtiendo al SI
        pre << " " << timer / (sqrt(G * Ms / (c * c * c)) * 3600 * 24) << " " << SumaE << endl;
    }

    //Imprimimos los periodos calculados
    print << endl
          << "Periodos calculados, si la segunda columna es 1, es que hemos podido calcular su periodo en su primera vuelta, la tercera columna indica el tiempo"
          << endl;
    for (i = 1; i < 10; i++) {
        print << fixed << setprecision(1) << "ID = " << i << "  " << T[i][0] << "   " << scientific << setprecision(6)
              << T[i][1] / (sqrt(G * Ms / (c * c * c)) * 3600 * 24) << endl;
    }

    //cerramos los ficheros
    print.close();
    pre.close();

    for(i = 0; i < NumeroAstros; i++){
        Pr[i].close();
    }

    return 0;
}