//Traduccion a c++ del algoritmo SAIS
//A01736214-Cruz Daniel Pérez Jiménez

#include<bits/stdc++.h>
using namespace std;



map<int,pair<int,int>> getBuckets(vector<int> &T){
    map<int,int>count;
    map<int,pair<int,int>> buckets;
    vector<int>keys;
    int start=0;

    for(int c:T)
        count[c]++;

    for (auto countP:count) 
        keys.push_back(countP.first);
    
    sort(keys.begin(),keys.end());

    for(auto c:keys){
        buckets.insert({c,{start,start+count[c]}});
        start+=count[c];
    }
    return buckets;
}

bool equalSA(vector<int> &T, int s1, int e1, int s2, int e2){
    if(e1-s1!= e2-s1)
    return false;
    for(int i =s1,j=s2;i<e1&&j<e2;i++,j++){
        if(T[i]!=T[j])
            return false;
    }
    return true;
}

vector<int> sais(vector<int> &T){
    vector<string> t(T.size(),"_");
    t[T.size()-1]="S";
    for(int i=T.size()-1;i>0;i--){
        if(T[i-1]==T[i])
            t[i-1]=t[i];
        else{
            if(T[i-1]<T[i])
                t[i-1]="S";
            else
                t[i-1]="L";  
        }
    }
    map<int,pair<int,int>> buckets=getBuckets(T);
    map<int,int> count;
    map<int,int> LMS;
    vector<int> SA(T.size(),-1);
    int end=-1,revoffset=0;
    for(int i=T.size()-1; i>0; i--){
        if((t[i]=="S")&&(t[i-1]=="L")){
            count[T[i]]++;
            revoffset=count[T[i]];
            SA[buckets[T[i]].second-revoffset]=i;
            if (end != -1)
                LMS[i]=end;
            
            end=i;
        }
    }
    LMS[T.size()-1]=T.size()-1;

    count.clear();
    int symbol=0,offset=0;
    for(int i=0;i<T.size();i++){
        if(SA[i]>0){
            if(t[SA[i]-1]=="L"){
                symbol=T[SA[i]-1];
                offset=count[symbol];
                count[symbol]=count[symbol]+1;
                SA[buckets[symbol].first + offset]=SA[i]-1;
            }
        }else if(SA[i]==0){
            if(t[t.size()-1]=="L"){
                symbol=T[T.size()-1];
                offset=count[symbol];
                count[symbol]=count[symbol]+1;
                SA[buckets[symbol].first + offset]=SA[i]-1;
            }
        }

    }

    count.clear();
    for(int i=T.size()-1;i>0;i--){
        if(SA[i]>0){
            if(t[SA[i]-1]=="S"){
                symbol=T[SA[i]-1];
                count[symbol]=count[symbol]+1;
                int revoffset=count[symbol];
                SA[buckets[symbol].second-revoffset]=SA[i]-1;
            }
        }
    }
    //NOMBRES
    vector<int> namesp(T.size(),-1);
    int name=0,prev= -1;
    for(int i=0;i<SA.size();i++){
        string v1=t[SA[i]];
        string v2;
        if(SA[i]>0)
            v2=t[SA[i]-1];
        else if(SA[i]==0)
            v2=t[t.size()-1];
        if((v1=="S") && (v2=="L")){
            if (prev != -1){
                if(!equalSA(T,SA[prev],LMS[SA[prev]],SA[i],LMS[SA[i]]))
                name++;
        }
        prev=i;
        namesp[SA[i]]=name;
        }
    }
    //PARTE 2 NOMBRES
    vector<int> names;
    vector<int>SapIdx;
    for(int i=0;i<T.size();i++){
        if(namesp[i] != -1){
            names.push_back(namesp[i]);
            SapIdx.push_back(i);
        }
    }
    if(name < names.size()-1)
        names=sais(names);
    reverse(names.begin(),names.end());
    SA.assign(T.size(),-1);
    count.clear();
    for(int i=0;i<names.size();i++){
        int pos=SapIdx[names[i]];
        count[T[pos]]=count[T[pos]]+1;
        revoffset=count[T[pos]];
        SA[buckets[T[pos]].second-revoffset]=pos;
    }
    count.clear();
    for(int i=0;i<T.size();i++){
        if(SA[i]>0){
            if(t[SA[i]-1] == "L"){
                symbol=T[SA[i]-1];
                offset=count[symbol];
                SA[buckets[symbol].first+offset]=SA[i]-1; 
                count[symbol]=offset+1;
            }
        }
    }
    count.clear();
    for(int i=T.size()-1;i>0;i--){
        if(SA[i]>0){
            if(t[SA[i]-1]=="S"){
                symbol=T[SA[i]-1];
                count[symbol]=count[symbol]+1;
                revoffset=count[symbol];
                SA[buckets[symbol].second-revoffset]=SA[i]-1;
            }
        }
    }
    return SA;
}
string ReadFile(string lib){
    ifstream file(lib);
    if(!file.is_open())
        cout<<"No se pudo abrir "<<lib<<endl;
    string texto;
    string linea;
    while(getline(file,linea))
        texto += linea;
    file.close();
    return texto;
}

int main(){
    //Se introduce el nombre del archivo
    string nlibro;
    cout<<"Introduzca el libro: \n";
    cin>>nlibro;
    //Abro el archivo
    string text=ReadFile(nlibro);
    vector<int> T(text.begin(),text.end());
    T.push_back(0);
  
    vector<int> SA=sais(T);
    for(int i=0;i<SA.size();i++)
        cout<<SA[i]<<" ";
    cout<<endl;
    return 0; 
}