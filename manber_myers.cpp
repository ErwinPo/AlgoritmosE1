#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <tuple>
#include <fstream>
#include <ctime>



using namespace std;

class SubstrRank{
public:
    int left_rank;
    int right_rank;
    int index;
    SubstrRank(int pleft_rank = 0, int pright_rank = 0, int pindex = 0){
        left_rank = pleft_rank;
        right_rank = pright_rank;
        index = pindex;
    }

};

vector<int> make_ranks(vector<SubstrRank> &substr_rank, int n){
    int r = 1;
    vector<int> rank(n, -1);
    rank[substr_rank[0].index] = r;

    for(int i = 0; i<n; i++){
        if(substr_rank[i].left_rank != substr_rank[i-1].left_rank || substr_rank[i].right_rank != substr_rank[i-1].right_rank){
            r += 1;
        }
        rank[substr_rank[i].index] = r;
    }

    return rank;
}

vector<int> suffix_array(string &T){
    int n = T.length();
    vector<int> rank;
    int codigoCaracter, codigoCaracterSiguiente;
    vector<SubstrRank> substr_rank;
    for(int i = 0; i< n;i++){
        codigoCaracter = T[i];
        codigoCaracterSiguiente = T[i+1];
        substr_rank.push_back(SubstrRank(codigoCaracter,(i < n-1) ?  codigoCaracterSiguiente: 0, i));
    }
    sort(substr_rank.begin(), substr_rank.end(), [](const SubstrRank& a, const SubstrRank& b) {
        // Ordenar primero por left_rank y luego por right_rank en caso de empate
        return tie(a.left_rank, a.right_rank) < tie(b.left_rank, b.right_rank);
    });
    int l = 2;
    while(l<n){
        rank = make_ranks(substr_rank, n);
        for(int i = 0; i< n;i++){
            substr_rank[i].left_rank = rank[i];
            substr_rank[i].right_rank = (i+l < n) ?  rank[i+l]: 0;
            substr_rank[i].index = i;
        }
        l *= 2;

        sort(substr_rank.begin(), substr_rank.end(), [](const SubstrRank& a, const SubstrRank& b) {
        // Ordenar primero por left_rank y luego por right_rank en caso de empate
        return tie(a.left_rank, a.right_rank) < tie(b.left_rank, b.right_rank);
        });
    }

    vector<int> SA;
    for(int i = 0; i<n; i++){
        SA.push_back(substr_rank[i].index);
    }
    return SA;
}

vector<int> search_substr(string patron, string &texto, vector<int> &suffray, int N){
    int M;
    int L = 0;
    int R = N-1;
    string subTexto;
    int tamanoPat = patron.length();
    int index;
    int firstocurrence;
    vector<int> ocurrences;
    while (L<=R)
    {
        M = (R+L)/2;
        subTexto = texto.substr(suffray[M],tamanoPat);
        if(patron == subTexto){
            //cout << subTexto << M << endl;
            index = suffray[M];
            firstocurrence = suffray[M];
            break;
        }
        else if(subTexto < patron){
            L = M + 1;
        }
        else{
            R = M - 1; 
        }
    }
    while(subTexto == patron){
        M = M-1;
        //cout << M << endl;
        subTexto = texto.substr(suffray[M],tamanoPat);
        //cout << subTexto << endl;
        if(subTexto == patron){
            firstocurrence = M;
            index = suffray[firstocurrence];
            //subTexto = texto.substr(suffray[M+1], tamanoPat);
        }
    }
    M++;
    subTexto = texto.substr(suffray[M],tamanoPat);
    while(subTexto == patron){
        M = M+1;
        subTexto = texto.substr(suffray[M],tamanoPat);
        if (subTexto == patron){
            ocurrences.push_back(M);
        }
    }

    return ocurrences;
    
}


int main(){
    
    string filename = "war.txt";
    ifstream file(filename);

    string texto;

    //Verifica si el archivo se ha abierto correctamente
    if (file.is_open()) {
        // Almacena el contenido del archivo en un string
        string contenido((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        texto = contenido;

        // El archivo se cierra automáticamente cuando sale del ámbito
    } else {
        cerr << "No se pudo abrir el archivo." << endl;
    }
    file.close();
    vector<int> SA;

    std::clock_t start = std::clock();
    

    SA = suffix_array(texto);
    cout << endl;
    std::clock_t end = std::clock();
    double elapsed = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    std::cout << "Tiempo de ejecución: " << elapsed << " segundos" << std::endl;
    //search("a", ejemplo, SA, ejemplo.length());
    
    string patron = "Nicholas";
    vector<int> indexes = search_substr(patron, texto, SA, texto.length());
    for(int i = 0; i < indexes.size(); i++){
        cout<< "El index numero " << i << " es: " << indexes[i] << endl;
        cout<< "El index numero " << indexes[i] << " es: " << texto.substr(SA[indexes[0]], patron.length()) << endl;
    }
    
    //cout<< "Los caracteres son: " << texto.substr(SA[indexes[0]], patron.length()) << endl;
    
    return 0;
}

