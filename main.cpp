#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
using namespace std;

struct Proceso {
    int id;
    int tiempo_llegada;
    int tiempo_ejecucion;
    int posicion;
    int tiempo_finalizacion;
    int tiempo_retorno;
    int tiempo_espera;
    bool atendido = false;
};

// Ordenar por tiempo de llegada para FCFS
bool ordenarPorLlegada(Proceso a, Proceso b) {
    return a.tiempo_llegada < b.tiempo_llegada;
}

// FCFS (First-Come, First-Served)
void FCFS(vector<Proceso> procesos) {
    sort(procesos.begin(), procesos.end(), ordenarPorLlegada);

    int tiempo_actual = 0;
    double total_retorno = 0, total_espera = 0;
    vector<int> secuencia;

    for (auto &p : procesos) {
        if (tiempo_actual < p.tiempo_llegada)
            tiempo_actual = p.tiempo_llegada;

        tiempo_actual += p.tiempo_ejecucion;
        p.tiempo_finalizacion = tiempo_actual;
        p.tiempo_retorno = p.tiempo_finalizacion - p.tiempo_llegada;
        p.tiempo_espera = p.tiempo_retorno - p.tiempo_ejecucion;

        total_retorno += p.tiempo_retorno;
        total_espera += p.tiempo_espera;
        secuencia.push_back(p.id);
    }

    cout << "\n--- Planificacion FCFS ---\n";
    cout << "Secuencia de ejecucion: ";
    for (size_t i = 0; i < secuencia.size(); i++) {
        cout << "P" << secuencia[i];
        if (i < secuencia.size() - 1) cout << " -> ";
    }
    cout << "\nTiempo promedio de espera: " << (total_espera / procesos.size()) << " unidades";
    cout << "\nTiempo promedio de retorno: " << (total_retorno / procesos.size()) << " unidades\n";
}

// SSTF (Shortest Seek Time First)
void SSTF(vector<Proceso> procesos, int posicion_inicial) {
    int n = procesos.size();
    int tiempo_actual = 0, completados = 0;
    double total_retorno = 0, total_espera = 0;
    vector<int> secuencia;
    int cabezal = posicion_inicial;

    while (completados < n) {
        int indice = -1;
        int menor_distancia = 1e9;

        for (int i = 0; i < n; i++) {
            if (!procesos[i].atendido && procesos[i].tiempo_llegada <= tiempo_actual) {
                int distancia = abs(procesos[i].posicion - cabezal);
                if (distancia < menor_distancia) {
                    menor_distancia = distancia;
                    indice = i;
                }
            }
        }

        if (indice == -1) {
            tiempo_actual++;
            continue;
        }

        procesos[indice].atendido = true;
        cabezal = procesos[indice].posicion;

        if (tiempo_actual < procesos[indice].tiempo_llegada)
            tiempo_actual = procesos[indice].tiempo_llegada;

        tiempo_actual += procesos[indice].tiempo_ejecucion;
        procesos[indice].tiempo_finalizacion = tiempo_actual;
        procesos[indice].tiempo_retorno = procesos[indice].tiempo_finalizacion - procesos[indice].tiempo_llegada;
        procesos[indice].tiempo_espera = procesos[indice].tiempo_retorno - procesos[indice].tiempo_ejecucion;

        total_retorno += procesos[indice].tiempo_retorno;
        total_espera += procesos[indice].tiempo_espera;
        secuencia.push_back(procesos[indice].id);
        completados++;
    }

    cout << "\n--- Planificacion SSTF ---\n";
    cout << "Secuencia de ejecucion: ";
    for (size_t i = 0; i < secuencia.size(); i++) {
        cout << "P" << secuencia[i];
        if (i < secuencia.size() - 1) cout << " -> ";
    }
    cout << "\nTiempo promedio de espera: " << (total_espera / n) << " unidades";
    cout << "\nTiempo promedio de retorno: " << (total_retorno / n) << " unidades \n";
}

// SCAN (Elevator Algorithm), cpacidad del disco limitada a 100, SCAN clasico hacia la derecha con extremos del disco
void SCAN(vector<Proceso> procesos, int posicion_inicial, bool direccion_derecha, int disco_max) {
    int n = procesos.size();
    int tiempo_actual = 0, completados = 0;
    double total_retorno = 0, total_espera = 0;
    vector<int> secuencia;
    int cabezal = posicion_inicial;

    while (completados < n) {
        vector<int> candidatos;
        for (int i = 0; i < n; i++) {
            if (!procesos[i].atendido && procesos[i].tiempo_llegada <= tiempo_actual) {
                if ((direccion_derecha && procesos[i].posicion >= cabezal) ||
                    (!direccion_derecha && procesos[i].posicion <= cabezal)) {
                    candidatos.push_back(i);
                }
            }
        }

        if (candidatos.empty()) {
            int distancia = direccion_derecha ? (disco_max - cabezal) : cabezal;
            tiempo_actual += distancia;
            cabezal = direccion_derecha ? disco_max : 0;
            direccion_derecha = !direccion_derecha;
            continue;
        }

        if (direccion_derecha) {
            sort(candidatos.begin(), candidatos.end(), [&](int a, int b) {
                return procesos[a].posicion < procesos[b].posicion;
            });
        } else {
            sort(candidatos.begin(), candidatos.end(), [&](int a, int b) {
                return procesos[a].posicion > procesos[b].posicion;
            });
        }

        int indice = candidatos.front();
        procesos[indice].atendido = true;
        cabezal = procesos[indice].posicion;

        if (tiempo_actual < procesos[indice].tiempo_llegada)
            tiempo_actual = procesos[indice].tiempo_llegada;

        tiempo_actual += procesos[indice].tiempo_ejecucion;
        procesos[indice].tiempo_finalizacion = tiempo_actual;
        procesos[indice].tiempo_retorno = procesos[indice].tiempo_finalizacion - procesos[indice].tiempo_llegada;
        procesos[indice].tiempo_espera = procesos[indice].tiempo_retorno - procesos[indice].tiempo_ejecucion;

        total_retorno += procesos[indice].tiempo_retorno;
        total_espera += procesos[indice].tiempo_espera;
        secuencia.push_back(procesos[indice].id);
        completados++;
    }

    cout << "\n--- Planificacion SCAN ---\n";
    cout << "Secuencia de ejecucion: ";
    for (size_t i = 0; i < secuencia.size(); i++) {
        cout << "P" << secuencia[i];
        if (i < secuencia.size() - 1) cout << " -> ";
    }
    cout << "\nTiempo promedio de espera: " << (total_espera / n) << " unidades";
    cout << "\nTiempo promedio de retorno: " << (total_retorno / n) << " unidades \n";
}

int main() {
    int n;
    cout << "Ingrese el numero de procesos: ";
    cin >> n;

    vector<Proceso> procesos(n);
    cout << "Ingrese los procesos (ID TiempoLlegada TiempoEjecucion Posicion):\n";
    for (int i = 0; i < n; i++) {
        cin >> procesos[i].id >> procesos[i].tiempo_llegada >> procesos[i].tiempo_ejecucion >> procesos[i].posicion;
    }

    int posicion_inicial;
    cout << "Ingrese la posicion inicial del cabezal: ";
    cin >> posicion_inicial;

    FCFS(procesos);
    SSTF(procesos, posicion_inicial);
    SCAN(procesos, posicion_inicial, true, 100);

    return 0;
}
