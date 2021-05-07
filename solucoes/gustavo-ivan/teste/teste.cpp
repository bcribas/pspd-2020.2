#include <bits/stdc++.h>

using namespace std;
vector<string> name;
using filesystem::directory_iterator;

ofstream myfile;
double solve(string source, char delimiter)
{
    string dest = "";
    long long time = 0;
    for (int i = 0; i < source.size(); i++)
    {
        if (source[i] == ':')
        {
            //cout << dest << endl;
            //cout << "entrou no minnuto\n";
            time = stoll(dest, nullptr, 10) * 60;
            //cout << time << endl;
            dest = "";
        }
        else if (source[i] != delimiter)
        {
            dest += source[i];
        }
        else
        {
            break;
        }
    }
    double time2 = 0.0;
    if (!dest.empty())
    {
        time2 = stod(dest);
    }

    return time + time2;
}

void imprimeVar(string temp)
{
    // cout << temp << endl;
    ifstream cin(temp);
    long long variaveis, clausulas;
    cin >> variaveis >> clausulas;
    string s;
    long long i = 0;
    bool ver = false;
    while (getline(cin, s))
    {
        if (s[0] == 'f' && ver == false)
        {
            ver = true;
            getline(cin, s);
        }
        if (ver)
        {
            i++;
        }
    }
    cout << "Casos de teste = " << i << " variaveis = " << variaveis << " clausulas = " << clausulas << endl;
    myfile << i << "," << variaveis << "," << clausulas << ",";
    return;
}

void getFileNames()
{
    string path = "../pspd-2020.2/input";

    for (const auto &file : directory_iterator(path))
    {
        string a = file.path();
        name.push_back(a);
    }
    // for(auto p: name){
    //     cout << p << endl;
    // }
}

string getNomeArquivo(string temp)
{
    string b = "";
    for (int i = temp.size() - 1; temp[i] != '/'; i--)
    {
        b += temp[i];
    }
    reverse(b.begin(), b.end());
    // cout << b << endl;

    return b;
}

void openFile()
{
    myfile.open("planilha.csv");
    myfile << "Nome arquivo,Casos de Teste, Variáveis, Cláusulas, Tempo Real, Tempo User, Tempo Sys" << endl;
}

int main(int argc, char *argv[])
{

    openFile();
    system("mkdir diff");
    getFileNames();
    //compila o arquivo de teste
    int j = 0;
    while (j < name.size())
    {

        string nomeArquivo = getNomeArquivo(name[j]);
        myfile << nomeArquivo << ",";

        // monta a string para executar o teste com time

        char executavel[120];
        sprintf(executavel, "{ time ./%s <%s > out;} 2>time.txt", argv[1],name[j].c_str());
        cout << "Comando = " << executavel << endl;

        // monta a string pro comando diff
        char diff[120];
        sprintf(diff, "diff out ../pspd-2020.2/output/%s > diff/%s ", nomeArquivo.c_str(),nomeArquivo.c_str());
        cout << "Comando diff = " << diff << endl;

        // joga casos de teste, variaveis e clausulas no csv
        imprimeVar(name[j]);

        double real3 = 0, user3 = 0, system3 = 0;
        system(executavel);
        int checaDiff  =system(diff);
        if(checaDiff == 0){
            cout << "output/" << nomeArquivo << " ok" << endl; 
        } else{
            cout << "output/" << nomeArquivo << " fail" << endl; 
        }
        string line;
        ifstream cin("time.txt");
        string real, user, system, lixo;
        cin >> user >> system >> real;
        real3 = solve(real, 'e');
        user3 = solve(user, 'u');
        system3 = solve(system, 's');


        cout << fixed << setprecision(2) << "real = " << real3 << " user = " << user3 << " system = " << system3 << "\n\n";
        myfile << real3 << "," << user3 << "," << system3 << endl;
        j++;
    }
    myfile.close();

    return 0;
}
