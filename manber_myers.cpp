#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <tuple>
#include <fstream>

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

std::vector<int> make_ranks(std::vector<SubstrRank> &substr_rank, int n){
    int r = 1;
    std::vector<int> rank(n, -1);
    rank[substr_rank[0].index] = r;

    for(int i = 0; i<n; i++){
        if(substr_rank[i].left_rank != substr_rank[i-1].left_rank || substr_rank[i].right_rank != substr_rank[i-1].right_rank){
            r += 1;
        }
        rank[substr_rank[i].index] = r;
    }

    return rank;
}

std::vector<int> suffix_array(std::string T){
    int n = T.length();
    std::vector<int> rank;
    int codigoCaracter, codigoCaracterSiguiente;
    std::vector<SubstrRank> substr_rank;
    for(int i = 0; i< n;i++){
        codigoCaracter = T[i];
        codigoCaracterSiguiente = T[i+1];
        substr_rank.push_back(SubstrRank(codigoCaracter,(i < n-1) ?  codigoCaracterSiguiente: 0, i));
    }
    std::sort(substr_rank.begin(), substr_rank.end(), [](const SubstrRank& a, const SubstrRank& b) {
        // Ordenar primero por left_rank y luego por right_rank en caso de empate
        return std::tie(a.left_rank, a.right_rank) < std::tie(b.left_rank, b.right_rank);
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

        std::sort(substr_rank.begin(), substr_rank.end(), [](const SubstrRank& a, const SubstrRank& b) {
        // Ordenar primero por left_rank y luego por right_rank en caso de empate
        return std::tie(a.left_rank, a.right_rank) < std::tie(b.left_rank, b.right_rank);
        });
    }

    std::vector<int> SA;
    for(int i = 0; i<n; i++){
        SA.push_back(substr_rank[i].index);
    }
    return SA;
}

int search_substr(std::string patron, std::string texto, std::vector<int> &suffray, int N){
    int M;
    int L = 0;
    int R = N-1;
    std::string subTexto;
    int tamanoPat = patron.length();
    while (L<=R)
    {
        M = (R+L)/2;
        subTexto = texto.substr(suffray[M],tamanoPat);
        if(patron == subTexto){
            //std::cout << subTexto << M << std::endl;
            return suffray[M];
        }
        else if(subTexto < patron){
            L = M + 1;
        }
        else{
            R = M - 1; 
        }
    }

    return -1;
    
}


int main(){

    std::string filename = "Dracula.txt";
    std::ifstream file(filename);

    std::string texto;

    // Verifica si el archivo se ha abierto correctamente
    if (file.is_open()) {
        // Almacena el contenido del archivo en un std::string
        std::string contenido((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        texto = contenido;

        // El archivo se cierra automáticamente cuando sale del ámbito
    } else {
        std::cerr << "No se pudo abrir el archivo." << std::endl;
    }
    std::vector<int> SA;

    SA = suffix_array(texto);
    //std::string texto = "banana";
    //SA = suffix_array(ejemplo);
    //std::cout << texto.size();
    
    /*
    for(int i = 0; i<SA.size(); i++){
        std::cout << SA[i] << ", ";
    }
    */
    std::cout << std::endl;
    //search("a", ejemplo, SA, ejemplo.length());
    std::string patron = "Bram";
    int index = search_substr(patron, texto, SA, texto.length());
    std::cout<< "El index es: " << index 
    << "\nLos caracteres son: " << texto.substr(index, patron.length());
    return 0;
}

