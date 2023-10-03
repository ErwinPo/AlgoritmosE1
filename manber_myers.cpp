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

    //SA = suffix_array(texto);
    SA = suffix_array("banana");
    //std::cout << texto.size();
    
    for(int i = 0; i<SA.size(); i++){
        std::cout << SA[i] << ", ";
    }

    std::string ejemplo = "banana";
    //search("a", ejemplo, SA, ejemplo.length());
    

    return 0;
}

