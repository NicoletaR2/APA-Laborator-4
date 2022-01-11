#include <iostream>
#include <queue>
#include<vector>
#include<time.h>

using namespace std;

double iteratiiDijkstra = 0, iteratiiFW = 0;

enum class DensitateGraf
{
    Rar = 0,
    Mediu = 1,
    Dens = 2
};

enum class NumarNoduriGraf
{
    nrNod100 = 100,
    nrNod200 = 200,
    nrNod300 = 300
};

struct DataGraf
{
public:
    int** MatriceAdiacenta;
    int nrMuchii;
    int nrNoduri;
};

DataGraf GenerareGrafConexNeorientatPonderat(NumarNoduriGraf gp, DensitateGraf gs)
{
    int** _MatriceAdiacenta;

    bool* pArrayNrNoduri;
    int nrCurentDeNoduri = 0;
    int nrCurentDeMuchii = 0;
    int nrNoduriGenerate = (int)gp;
    int nrMuchiiGenerate;

    int minMuchii, maxMuchii, totalMuchii;
    float minFi = 0, maxFi = 0;

    switch (gs)
    {
    case DensitateGraf::Rar:
    {
        minFi = 0.32f;
        maxFi = 0.41f;
    }
    break;

    case DensitateGraf::Mediu:
    {
        minFi = 0.41f;
        maxFi = 0.68f;
    }
    break;

    case DensitateGraf::Dens:
    {
        minFi = 0.68f;
        maxFi = 1;
    }
    break;

    default:

        break;
    }

    totalMuchii = (nrNoduriGenerate * (nrNoduriGenerate - 1)) / 2;

    minMuchii = (int)(minFi * totalMuchii);
    maxMuchii = (int)(maxFi * totalMuchii);

    nrMuchiiGenerate = minMuchii;


    _MatriceAdiacenta = new int* [nrNoduriGenerate];

    for (int ex = 0; ex < nrNoduriGenerate; ++ex)
    {
        int* pArray = new int[nrNoduriGenerate];

        for (int ix = 0; ix < nrNoduriGenerate; ++ix)
        {
            pArray[ix] = INT_MAX;
        }

        _MatriceAdiacenta[ex] = pArray;
    }

    pArrayNrNoduri = new bool[nrNoduriGenerate] {false};

    bool GenerareFinisata = false;

    while (GenerareFinisata == false)
    {
        int newStart = rand() % nrNoduriGenerate;
        int newEnd = rand() % nrNoduriGenerate;

        int verificaMuchiaDirecta;
        int verificaMuchiaInversa;

        if (newStart == newEnd)
        {
            continue;
        }

        if (newStart == nrNoduriGenerate)
        {
            newStart -= 1;
        }


        if (newEnd == nrNoduriGenerate)
        {
            newEnd -= 1;
        }

        verificaMuchiaDirecta = _MatriceAdiacenta[newStart][newEnd];
        verificaMuchiaInversa = _MatriceAdiacenta[newEnd][newStart];

        if (verificaMuchiaDirecta != verificaMuchiaInversa)
        {
            throw "verificaMuchiaDirecta != verificaMuchiaInversa";
        }


        if (verificaMuchiaDirecta != INT_MAX)
        {
            continue;
        }

        int pondere = abs(newEnd - newStart);

        _MatriceAdiacenta[newStart][newEnd] = pondere;
        _MatriceAdiacenta[newEnd][newStart] = pondere;

        nrCurentDeMuchii += 1;


        if (pArrayNrNoduri[newStart] == false)
        {
            pArrayNrNoduri[newStart] = true;

            nrCurentDeNoduri += 1;
        }

        if (pArrayNrNoduri[newEnd] == false)
        {
            pArrayNrNoduri[newEnd] = true;

            nrCurentDeNoduri += 1;
        }

        GenerareFinisata = (nrCurentDeMuchii >= nrMuchiiGenerate && nrCurentDeNoduri >= nrNoduriGenerate);

    }

    DataGraf DataGraf;
    DataGraf.nrMuchii = nrCurentDeMuchii;
    DataGraf.nrNoduri = nrCurentDeNoduri;
    DataGraf.MatriceAdiacenta = _MatriceAdiacenta;

    return DataGraf;
}

int DistantaMinim(int arrayDistanta[], bool SetMuchiiNeincluse[], int nrNoduri)
{
    int min = INT_MAX, min_index;

    for (int nod_de_vizitat = 0; nod_de_vizitat < nrNoduri; nod_de_vizitat++)
    {
        if (SetMuchiiNeincluse[nod_de_vizitat] == false && arrayDistanta[nod_de_vizitat] <= min)
        {
            min = arrayDistanta[nod_de_vizitat];
            min_index = nod_de_vizitat;
            iteratiiDijkstra++;
        }
    }

    return min_index;
}

void AfisareDijkstra(int arrayDistanta[], int nrNoduri, int nod_sursa)
{
    cout << "N-Src Dist   Nod" << endl <<endl;
    for (int i = 0; i < nrNoduri; i++)

        cout<< "[" << nod_sursa << "]" << "--" << "(" << arrayDistanta[i] << ")" << "-->" << "[" << i << "]" << endl;
}


void AlgoritmDijkstra(DataGraf DataGraf, int nod_sursa)
{
    int* distanta = new int[DataGraf.nrNoduri];

    bool* SetMuchiiNeincluse = new bool[DataGraf.nrNoduri];

    for (int i = 0; i < DataGraf.nrNoduri; i++)
    {
        distanta[i] = INT_MAX, SetMuchiiNeincluse[i] = false;
    }

    distanta[nod_sursa] = 0;

    for (int count = 0; count < DataGraf.nrNoduri; count++)
    {
        int NodMin = DistantaMinim(distanta, SetMuchiiNeincluse, DataGraf.nrNoduri);

        SetMuchiiNeincluse[NodMin] = true;

        if (distanta[NodMin] == INT_MAX)
        {
            continue;
        }

        for (int nod_adiacent = 0; nod_adiacent < DataGraf.nrNoduri; nod_adiacent++)
        {

            iteratiiDijkstra++;

            if (!SetMuchiiNeincluse[nod_adiacent] &&
                    DataGraf.MatriceAdiacenta[NodMin][nod_adiacent] &&
                    DataGraf.MatriceAdiacenta[NodMin][nod_adiacent] != INT_MAX &&
                    distanta[NodMin] + DataGraf.MatriceAdiacenta[NodMin][nod_adiacent] < distanta[nod_adiacent])
            {
                distanta[nod_adiacent] = distanta[NodMin] + DataGraf.MatriceAdiacenta[NodMin][nod_adiacent];


            }

        }
    }

    AfisareDijkstra(distanta, DataGraf.nrNoduri, nod_sursa);



}

void AfisareFloydWarshall(int** dist, int nrNoduri)
{
    cout << "Matricea celor mai scurte distante intre fiecare noduri" << endl << endl;

    cout << "    ";

    for (int i = 0; i < nrNoduri; i++)
    {
        cout << '[' << i << ']' << " ";

        for (int j = 0; j < nrNoduri; j++)
        {
            if (dist[i][j] == 0)
            {
                cout << "->" << " ";
            }
            else
            {
                cout << dist[i][j] << " ";
            }
        }

        cout << endl;
    }
}

void AlgoritmFloydWarshall(DataGraf DataGraf)
{
    int i, j, k;

    int** dist = new int* [DataGraf.nrNoduri];
    for (int ex = 0; ex < DataGraf.nrNoduri; ++ex)
    {
        int* pArray = new int[DataGraf.nrNoduri];

        for (int ix = 0; ix < DataGraf.nrNoduri; ++ix)
        {
            pArray[ix] = INT_MAX;
        }

        dist[ex] = pArray;
    }

//pasul 1
    for (i = 0; i < DataGraf.nrNoduri; i++)
    {
        for (j = 0; j < DataGraf.nrNoduri; j++)
        {
            dist[i][j] = DataGraf.MatriceAdiacenta[i][j];
        }
    }

    for (k = 0; k < DataGraf.nrNoduri; k++)
    {

        for (i = 0; i < DataGraf.nrNoduri; i++)
        {

            for (j = 0; j < DataGraf.nrNoduri; j++)
            {
                if (dist[i][j] > (dist[i][k] + dist[k][j]) &&
                        (dist[k][j] != INT_MAX && dist[i][k] != INT_MAX))
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    iteratiiFW++;
                }

            }
        }
    }


    AfisareFloydWarshall(dist, DataGraf.nrNoduri);
}

void ExecutaDijkstra(NumarNoduriGraf gp, DensitateGraf gs)
{
    DataGraf DataGraf = GenerareGrafConexNeorientatPonderat(gp, gs);

    int NodDeStart = 0;

    int* nrArray = new int[DataGraf.nrNoduri];

    for (int ex = 0; ex < DataGraf.nrNoduri; ex++)
    {
        nrArray[ex] = 0;
    }

    for (int ex = 0; ex < DataGraf.nrNoduri; ex++)
    {
        for (int ix = 0; ix < DataGraf.nrNoduri; ix++)
        {
            if (DataGraf.MatriceAdiacenta[ex][ix] != INT_MAX)
            {
                nrArray[ex] += 1;
            }
        }
    }

    int valoarea_max = 0;
    int index_max = 0;

    for (int ex = 0; ex < DataGraf.nrNoduri; ex++)
    {
        if (valoarea_max < nrArray[ex])
        {
            index_max = ex;
            valoarea_max = nrArray[ex];
        }
    }

    clock_t start1 = clock();
    AlgoritmDijkstra(DataGraf, index_max);
    clock_t end1 = clock();
    double elapsed1 = double(end1 - start1) / CLOCKS_PER_SEC;
    cout << "\nTimpul de executie Dijkstra: " << elapsed1 << " secunde" << endl;


    cout<<"Nr. iteratii Dijkstra: "<<iteratiiDijkstra;
}

void ExecutaFloydWarshall(NumarNoduriGraf gp, DensitateGraf gs)
{
    DataGraf DataGraf = GenerareGrafConexNeorientatPonderat(gp, gs);

    clock_t start2 = clock();
    AlgoritmFloydWarshall(DataGraf);
    clock_t end2 = clock();
    double elapsed2 = double(end2 - start2) / CLOCKS_PER_SEC;
    cout << "\nTimpul de executie F-W: " << elapsed2 << " secunde" << endl;


    cout<<"Nr. iteratii FW: "<<iteratiiFW;
}

int main()
{
    NumarNoduriGraf nrNod100 = NumarNoduriGraf::nrNod100;
    NumarNoduriGraf nrNod200 = NumarNoduriGraf::nrNod200;
    NumarNoduriGraf nrNod300 = NumarNoduriGraf::nrNod300;

    DensitateGraf gsRar = DensitateGraf::Rar;
    DensitateGraf gsMediu = DensitateGraf::Mediu;
    DensitateGraf gsDens = DensitateGraf::Dens;

    int optiune;

    cout << "\t\t\tMENIU"<<endl;
    cout<< "Alegeti una dintre optiuni: "<<endl;
    cout<< "1.Dijkstra 100 noduri graf rar"<<endl<<"2.Dijkstra 100 noduri graf mediu" <<endl<<"3.Dijkstra 100 noduri graf dens"<<endl;
    cout<<endl<<endl;
    cout<< "4.Dijkstra 200 noduri graf rar"<<endl<<"5.Dijkstra 200 noduri graf mediu" <<endl<<"6.Dijkstra 200 noduri graf dens"<<endl;
    cout<<endl<<endl;
    cout<< "7.Dijkstra 300 noduri graf rar"<<endl<<"8.Dijkstra 300 noduri graf mediu" <<endl<<"9.Dijkstra 300 noduri graf dens"<<endl;
    cout<<endl<<endl<<"--------------------------------"<<endl<<endl;
    cout<< "10.F-W 100 noduri graf rar"<<endl<<"11.F-W 100 noduri graf mediu" <<endl<<"12.F-W 100 noduri graf dens"<<endl;
    cout<<endl<<endl;
    cout<< "13.F-W 200 noduri graf rar"<<endl<<"14.F-W 200 noduri graf mediu" <<endl<<"15.F-W 200 noduri graf dens"<<endl;
    cout<<endl<<endl;
    cout<< "16.F-W 300 noduri graf rar"<<endl<<"17.F-W 300 noduri graf mediu" <<endl<<"18.F-W 300 noduri graf dens"<<endl;
    cout<<"Optiunea aleasa: ";
    cin>>optiune;

    switch(optiune)
    {
    case 1:
        ExecutaDijkstra(nrNod100, gsRar);
        break;
    case 2:
        ExecutaDijkstra(nrNod100, gsMediu);
        break;
    case 3:
        ExecutaDijkstra(nrNod100, gsDens);
        break;

    case 4:
        ExecutaDijkstra(nrNod200, gsRar);
        break;
    case 5:
        ExecutaDijkstra(nrNod200, gsMediu);
        break;
    case 6:
        ExecutaDijkstra(nrNod200, gsDens);
        break;

    case 7:
        ExecutaDijkstra(nrNod300, gsRar);
        break;
    case 8:
        ExecutaDijkstra(nrNod300, gsMediu);
        break;
    case 9:
        ExecutaDijkstra(nrNod300, gsDens);
        break;

    case 10:
        ExecutaFloydWarshall(nrNod100, gsRar);
        break;
    case 11:
        ExecutaFloydWarshall(nrNod100, gsMediu);
        break;
    case 12:
        ExecutaFloydWarshall(nrNod100, gsDens);
        break;

    case 13:
        ExecutaFloydWarshall(nrNod200, gsRar);
        break;
    case 14:
        ExecutaFloydWarshall(nrNod200, gsMediu);
        break;
    case 15:
        ExecutaFloydWarshall(nrNod200, gsDens);
        break;

    case 16:
        ExecutaFloydWarshall(nrNod300, gsRar);
        break;
    case 17:
        ExecutaFloydWarshall(nrNod300, gsMediu);
        break;
    case 18:
        ExecutaFloydWarshall(nrNod300, gsDens);
        break;


    }
}
