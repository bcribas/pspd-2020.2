#include<bits/stdc++.h>

using namespace std;


long long n_variables,n_clauses;
const long int MAX {1100000};

void full(vector<vector<long int>> & clauses,bitset<MAX> & variable, vector<long int> & clause_value, map<long int,long int> &false_variables,set<long int>&false_clauses);
void resultado(map<long int,long int> &false_variables,set<long int>&false_clauses);
void flip(vector<vector<long int>> & clauses,map<long int,vector<long int>> & variable_in_clauses,bitset<MAX> &variable,map<long int,long int> &false_variables,set<long int>&false_clauses,vector<long int> & clause_value);

struct comp {
    bool operator() ( const pair<long int,long int> &a,  const pair<long int,long int> &b) const{
        if (a.second != b.second) {
            return a.second > b.second;
        }
        return abs(a.first) > abs(b.first);
    }
};




int main(){

    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    scanf("%lld %lld",&n_variables,&n_clauses);
   
    map<long int,vector<long int>>variable_in_clauses;
    vector<vector<long int>>clauses;
    bitset<MAX> variable;
    for(long int i=0;i<n_clauses;i++){
        long int t;
        vector<long int>temp_clause;
        while(scanf("%ld",&t)!=EOF,t!=0){
            temp_clause.push_back(t);
            variable_in_clauses[t].push_back(i);
        }
        clauses.push_back(temp_clause);
    }
   
    vector<long int>clause_value(n_clauses+1,0);
    vector<long int>temp(n_clauses+1,0);
    map<long int,long int> false_variables;
    set<long int>false_clauses;
    char s[30];
   
    while(scanf("%s",s)!=EOF){
        if(strcmp(s,"full")==0){
            variable.reset();
            clause_value =  temp;
            false_variables.clear();
            false_clauses.clear();
            full(clauses,variable,clause_value,false_variables,false_clauses); 
        } else if(strcmp(s,"flip")==0){
            flip(clauses,variable_in_clauses,variable,false_variables,false_clauses,clause_value);
        }
        resultado(false_variables,false_clauses);
    }


    return 0;

}

void full(vector<vector<long int>> & clauses,bitset<MAX> & variable, vector<long int> & clause_value,map<long int,long int> &false_variables,set<long int>&false_clauses){

    for(long int i=0;i<n_variables;i++){
        long int t;
        scanf("%ld",&t);
        if(t<0){
            t = abs(t);
            variable[t]=0;
        } else{
            variable[t]=1;
        }
    }

    for(size_t i=0;i<clauses.size();i++){
        for(size_t j=0;j<clauses[i].size();j++){
            long int t = clauses[i][j];
            if((t<0 && variable[abs(t)]==0) ||(t>0 && variable[t]==1) ){
                clause_value[i]+=1;
            }
        }
        if(clause_value[i]==0){
            false_clauses.insert(i);
            for(size_t j=0;j<clauses[i].size();j++){
                long int t = clauses[i][j];
                false_variables[t]++;
            }
           
        }
    }
    

}

void clauses_new_values(vector<vector<long int>> & clauses,vector<long int> & variable_in_clause,bool positive,vector<long int> & clause_value,map<long int,long int> &false_variables,set<long int>&false_clauses){
    long int k = positive?1:-1;
    for(auto & p: variable_in_clause){
       
        if(clause_value[p]==0 && k==1){
            for(size_t i=0;i<clauses[p].size();i++){
                long int u = clauses[p][i];
                false_variables[u]--;
                if(false_variables[u]<=0){
                    false_variables.erase(u);
                }
            }
            false_clauses.erase(p);
        } else if(clause_value[p]==1 && k==-1){
            for(size_t i=0;i<clauses[p].size();i++){
                long int u = clauses[p][i];
                false_variables[u]++;
            }
            false_clauses.insert(p);
        }
        
        clause_value[p]+=k;
        
    }
    

}



void flip (vector<vector<long int>> & clauses,map<long int,vector<long int>> & variable_in_clauses, bitset<MAX> &variable,map<long int,long int> &false_variables,set<long int>&false_clauses,vector<long int> & clause_value){
    long int t;
    scanf("%ld",&t);
    long int k = abs(t);
    variable[k]= variable[k]?0:1;
    
    if(variable[k] == 0){
        clauses_new_values(clauses, variable_in_clauses[k],false,clause_value,false_variables,false_clauses);
        clauses_new_values(clauses, variable_in_clauses[-k],true,clause_value,false_variables,false_clauses);
    } else{
        clauses_new_values(clauses,variable_in_clauses[-k],false,clause_value,false_variables,false_clauses);
        clauses_new_values(clauses, variable_in_clauses[k],true,clause_value,false_variables,false_clauses);
    }
    
}



void resultado(map<long int,long int> &false_variables,set<long int>&false_clauses){

    if(false_clauses.empty()){
        printf("SAT\n");
        return;
    }

    printf("[%ld clausulas falsas]",false_clauses.size());
    for(auto const &p: false_clauses){
        printf(" %ld",p);
    }
    printf("\n[lits]");
    set<pair<long int,long int>,comp> false_variables2(false_variables.begin(),false_variables.end());
    for(auto const &p: false_variables2){
        printf(" %ld",p.first);
    }
    printf("\n");
}