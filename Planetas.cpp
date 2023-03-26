// Code by Jorge Buenestado, 22/02/2023

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
 
using namespace std;

int main() {
    int i, j, k, n, numeroAstros;
    double A[10][7], P[10][2], Ac[10][2], V[10][2], W[10][2], T[10][2], E[10][3];
    double  steps, timer, timestep, Ene, SumaE, geoX, geoY, totalTime;
    bool lineStart, helio;
    ifstream fich;
    ofstream print,pre, Pr[10];
    string Astros[10] = {"PSun.txt.txt", "PMercury.txt", "PVenus.txt", "Ptierra.txt", "PMars.txt",
                         "PJupiter.txt", "PSaturn.txt", "PUranus.txt", "PNeptune.txt", "Pluto.txt"
    };

    //definimos nuestras constantes
    const double G = 6.67e-11;
    const double Ms = 1.99e30;
    const double c = 1.496e11;
    numeroAstros = 10;

    //Abrimos los archivos
    print.open("Periodos y Prueba.txt");
    pre.open("Energia.txt");

    for(i = 0; i < numeroAstros; i++){
        Pr[i].open(Astros[i]);
    }

    //leemos el fichero con los datos
    fich.open("./planetas.txt");
        if(fich.is_open()) cout<<"File opened successfuly."<<endl; 
        else cout<<"Error reading file."<<endl;
        for (i = 0; i < numeroAstros; i++) { for (j = 0; j < 7; j++)fich >> A[i][j]; }
    fich.close();

    //Probamos que se ha leido bien los datos del fichero, identificando cada número
    print << "//ID       //Radius(AU) //Mass(Ms)   //Excentricity//Period   // Radius(km) // Velocity" << endl;
    for (i = 0; i < numeroAstros; i++) {
        for (j = 0; j < 7; j++)print << scientific << setprecision(6) << A[i][j] << "   ";
        print << " " << endl;
    }

    //parametros de entrada
    cout << "Introduzca tiempo de la simulación en años" << endl;
    cin >> totalTime;
    totalTime = totalTime * 24 * 365 * 3600;
    cout << "Introduzca timestep en días" << endl;
    cin >> timestep;
    timestep = timestep * 24 * 3600;

    //Cambio Variables
    totalTime = totalTime * sqrt(G * Ms / (c * c * c));
    timestep = timestep * sqrt(G * Ms / (c * c * c));
    steps = totalTime / timestep;

    //Cambiamos de unidades del fichero
    for (i = 0; i < numeroAstros; i++) {
        A[i][1] = A[i][1] * 10e8 / c; //radio
        A[i][2] = A[i][2] * 10e24 / Ms; //masa
        A[i][4] = A[i][4] * sqrt(G * Ms / (c * c * c)); //periodo
        A[i][5] = A[i][5] / 2; //radio
        A[i][6] = A[i][6] * 1000 / sqrt(G * Ms / c); //velocidad m*s
    }

    //lineStart si quieres que todos los planetas empiecen en una misma línea
    lineStart = true;
    //sistema de referencia heliocentrico o geocentrico
    helio = true;

    // Posiciones&velocidades iniciales
    if (lineStart) {
        for(i = 1; i < numeroAstros; i++)
        {
            P[i][0] = A[i][1];
            V[i][1] = -A[i][6];
            P[i][1] = 0;
            V[i][0] = 0;
        }
    } else {
        for (i = 1; i < numeroAstros; i++) {
            if(i%4 >= 2){
                P[i][0] = pow(-1, i)*A[i][1];
                P[i][1] = 0;
                V[i][1] = 0;
                V[i][1] = pow(-1, i+1)*A[i][6];
            }
            else{
                P[i][0] = 0;
                P[i][1] = pow(-1, i)*A[i][1];
                V[i][0] = 0;
                V[i][1] = pow(-1, i+1)*A[i][6];
            }    
        }
    }

    //Sun position
    P[0][0] = 0;
    P[0][1] = 0;

    //Initialize to 0 some parameters
    for (i = 1; i < numeroAstros; i++) {
        for (k = 0; k < 2; k++) {
            Ac[i][k] = 0;
            T[i][k] = 0;
        }
    }
    timer = 0;

    //Aceleración Inicial
    for (k = 0; k < 2; k++) {
        for (i = 1; i < numeroAstros; i++) {
            for (j = 0; j < numeroAstros; j++) {
                if (i != j) {
                    Ac[i][k] -= A[j][2] * (P[i][k] - P[j][k]) /
                                (pow(sqrt((pow(P[i][0] - P[j][0], 2)) + pow(P[i][1] - P[j][1], 2)), 3));
                }
            }
        }
    }
    //hacemos los steps necesarios
    for (n = 1; n < steps; n++) {

        //Posiciones nuevas
        for (i = 0; i < numeroAstros; i++) {
            for (k = 0; k < 2; k++) {
                P[i][k] = P[i][k] + timestep * V[i][k] + timestep * timestep * Ac[i][k] / 2;
                W[i][k] = V[i][k] + timestep * Ac[i][k] / 2;
            }
        }

        //Imprimimos la posicion de cada planeta segun  sistema de referencia
        if (!helio) {
            geoX = P[3][0];
            geoY = P[3][1];
        } else {
            geoX = 0;
            geoY = 0;
        }

        for (i = 1; i < 10; i++) {
            Pr[i] << P[i][0] - geoX << " " << P[i][1] - geoY <<endl;
        };

        //Aceleracion Nueva
        for (i = 1; i < numeroAstros; i++) { for (k = 0; k < 2; k++) { Ac[i][k] = 0; }}

        for (k = 0; k < 2; k++) {
            for (i = 1; i < numeroAstros; i++) {
                for (j = 0; j < numeroAstros; j++) {
                    if (i != j) {
                        Ac[i][k] -= A[j][2] * (P[i][k] - P[j][k]) /
                                    (pow(sqrt((pow(P[i][0] - P[j][0], 2)) + pow(P[i][1] - P[j][1], 2)), 3));
                    }
                }
            }
        }

        //Velocidad nueva
        for (i = 1; i < numeroAstros; i++) {
            for (k = 0; k < 2; k++) {
                V[i][k] = W[i][k] + timestep * Ac[i][k] / 2;
            }
        }
        timer += timestep;

        //Calculo de periodo
        for (i = 1; i < numeroAstros; i++) {
            if (T[i][0] == 0) {
                if (P[i][1] >= 0) {
                    T[i][0] = 1;
                    T[i][1] = 2 * timer;
                }
            }
        }

        for (i = 1; i < numeroAstros; i++) {
            for (k = 0; k < 3; k++) {
                E[i][k] = 0;
            }
        }

        //Calculo de Energía
        SumaE = 0;
        Ene = 0;
        for (i = 1; i < numeroAstros; i++) {
            for (j = 0; j < numeroAstros; j++) {
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
          << "Periodos calculados. 1ra Columna = ID. 2da Columna == 1, Periodo encontrado. 3ra Columna, Periodo Calculado"
          << endl;
    for (i = 1; i < numeroAstros; i++) {
        print << fixed << setprecision(1) << "ID = " << i << "  " << T[i][0] << "   " << scientific << setprecision(6)
              << T[i][1] / (sqrt(G * Ms / (c * c * c)) * 3600 * 24) << endl;
    }

    //cerramos los ficheros

    print.close();
    pre.close();
    for(i = 0; i < numeroAstros; i++){
        Pr[i].close();
    }
    return 0;
}