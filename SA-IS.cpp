//Traduccion a c++ del algoritmo SAIS
//A01736214-Cruz Daniel Pérez Jiménez

#include<bits/stdc++.h>
using namespace std;
template<class p>
void imprimeVec(vector<p> vec){
    for(auto i=vec.begin();i<vec.end();++i)
        cout<<*i<<" ";
    cout<<endl;
}

map<int,pair<int,int>> getBuckets(vector<int> &T){
    map<int,int>count;
    map<int,pair<int,int>> buckets;
    int start=0;

    for(int i:T)
        count[i]=count[i]+1;
    
    for (auto it = count.begin(); it != count.end(); ++it) {
        buckets[it->first] = make_pair(start, start + it->second);
        start += it->second;
    }
    return buckets;
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
    for(int i=T.size()-1; i>0; --i){
        if((t[i]=="S")&&(t[i-1]=="L")){
            revoffset=count[T[i]]=count[T[i]]+1;
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
        if(SA[i]>=0){
            if(t[SA[i]-1]=="L"){
                symbol=T[SA[i]-1];
                offset=count[symbol];
                SA[buckets[symbol].first + offset]=SA[i]-1;
                count[symbol]=offset+1;

            }
        }
    }

    count.clear();
    for(int i=T.size()-1;i>0;--i){
        if(SA[i]>0){
            if(t[SA[i]-1]=="S"){
                symbol=T[SA[i]-1];
                int revoffset=count[symbol]=count[symbol]+1;
                SA[buckets[symbol].second-revoffset]=SA[i]-1;
            }
        }
    }
    //NOMBRES
    vector<int> namesp(T.size(),-1);
    int name=0,prev= -1;
    for(int i=0;i<SA.size();i++){
        if((t[SA[i]]=="S") && (t[SA[i]-1]=="L")){
            if ((prev != -1) && ((vector<int>(T.begin()+SA[prev],T.begin()+LMS[SA[prev]])) !=
             (vector<int>(T.begin()+SA[i],T.begin()+LMS[SA[i]]))))
                name++;
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
    cout<<name<<names.size()-1<<endl;
    if(name < names.size()-1)
        names=sais(names);
    reverse(names.begin(),names.end());
    SA.assign(T.size(),-1);
    count.clear();
    for(int i=0;i<names.size();i++){
        int pos=SapIdx[names[i]];
        revoffset=count[T[pos]]=count[T[pos]]+1;
        SA[buckets[T[pos]].second-revoffset]=pos;
    }
    count.clear();
    for(int i=0;i<T.size();i++){
        if(SA[i]>=0){
            if(t[SA[i]-1] == "L"){
                symbol=T[SA[i]-1];
                offset=count[symbol];
                SA[buckets[symbol].first+offset]=SA[i]-1; 
                count[symbol]=offset+1;
            }
        }
    }
    count.clear();
    for(int i=T.size()-1;i>0;--i){
        if(SA[i]>0){
            if(t[SA[i]-1]=="S"){
                symbol=T[SA[i]-1];
                revoffset=count[symbol]=count[symbol]+1;
                SA[buckets[symbol].second-revoffset]=SA[i]-1;
            }
        }
    }
    return SA;
}

int main(){
    vector<int> T;
    string text="GT.CCCGAT,GT CATGTCAGGA";
    for(int i=0;text[i]!='\0';i++)
        T.push_back(int(text[i]));
    T.push_back(0);
    vector<int> SA=sais(T);
    imprimeVec(SA);
    return 0;
}