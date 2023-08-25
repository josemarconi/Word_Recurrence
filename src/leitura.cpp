#include "leitura.hpp"

void readPrincipal() 
{
    vector<string> filename;

    filename.push_back("dataset/Semana_Machado_Assis.txt");
    filename.push_back("dataset/DomCasmurro.txt"); 

    string line;
    string palavraLinha;

    bool decisao = false;

    int cont = 0, linhai = 1;
    int contPalavras = 0, quantidade = 1;

    unordered_map<string, int> Hash; // armazena todas as palavras do texto com todas as info
    unordered_map<string, bool>stopWords; // armazena as sw
    unordered_map<char, bool>pontos; // armazena os pts

    preencheMapPontos(pontos); 

    bool mudou = false;

    readStopWords(stopWords);

    for (size_t i = 0; i < filename.size(); i++)
    {
        ifstream input_file(filename[i], ios::in);

        if (!input_file.is_open()) 
        {
            cout << "Nao foi possivel abrir o arquivo " << filename[i] << " " << endl;
        }

        while (!input_file.eof()) 
        {
            while (getline(input_file, line, '\n')) 
            { // pega linha por linha
                cont++;
                for (auto &c : line) 
                {
                    c = tolower(c); // passa tudo pra minusculo
                }

                line = converterAcentuadasParaMinusculas(line);
                line = tiraReticencia(line);
                istringstream ln(line); // token

                if (line.size() > 1)   
                {// onde comeca o paragrafo
                    decisao = false;

                    while (ln >> palavraLinha) { // analisa palavra por palavra ate o final da linha
                        if (!temNumero(palavraLinha)) 
                        { // verifica se tem numero na linha e se tiver n salva no map

                            if (Pontuacao(&palavraLinha)) 
                            {
                                mudou = true; //pra falar q mudou de senteca
                            }

                            palavraLinha = removePontuacao(palavraLinha, pontos); // palavrinha recebe a palavra sem pontuacao
                            
                            auto it2 = stopWords.find(palavraLinha); //"posicao" verifica se a palavraLinha eh igual a alguma sw, se for igual retorna a posi da sw no map
                            if (it2 == stopWords.end()) 
                            { // se a  posicao do it2 for igual a posicao final, significa que a a palavinha n eh sw
                                //adiciona palavra no map//
                                if (palavraLinha != ""  && Remove_Caracter_Fora_ASCII(palavraLinha)) 
                                {
                                    insereHash(Hash, palavraLinha, contPalavras, quantidade);
                                }
                            }

                            if (mudou) 
                            { // pra mudar de sentenca
                                mudou = false;
                            }
                        }
                    }

                    line.clear();
                }
                else 
                {// entra quando acabar o paragrafo, quando a linha eh vazia

                    decisao = true;
                    linhai = cont + 1; // para que a linha incial do prox paragrafo n seja linha em branco
                }
            }
        }
        if (!(decisao))
        { // caso o ultimo paragrafo n seja lido, dependendo da formatacao do arq
            decisao = true;
            linhai = cont + 1;
        }
    }

    cout << endl << "Número total de palavras:" << contPalavras << endl << endl;

    heap(Hash);
}

void insereHash(unordered_map<string, int> &Hash, string palavraLinha, int &contPalavras, int quantidade) 
{
    auto it = Hash.find(palavraLinha); //MAPA FINAL

    if (it != Hash.end()) 
    {//palavra existe*
        it -> second++; //incrementa valor

    } else 
    {
        Hash.insert({palavraLinha, quantidade}); //insere se não existir

        contPalavras++;
    }
}

void readStopWords(unordered_map<string, bool> &stopWords) 
{
    string filename("dataset/stopwords.txt");
    ifstream input_file(filename, ios::in);
    string line;

    while (!input_file.eof()) {
        while (getline(input_file, line, '\n')) 
        {
            if (line.size() >= 1) 
            {
                for (auto &c : line) 
                {
                    c = tolower(c);
                }

                stopWords.insert({ line,true });
            }
        }
    }
}

void preencheMapPontos(unordered_map<char, bool> &pontos) 
{
    pontos.insert({
        {'-', false},
        {'.', false},
        {',', false},
        {';', false},
        {'"', false},
        {'/', false},
        {'(', false},
        {')', false},
        {'[', false},
        {']', false},
        {'+', false},
        {'-', false},
        {'=', false},
        {'$', false},
        {'%', false},
        {'&', false},
        {'*', false},
        {':', false},
        {'^', false},
        {'|', false},
        {'~', false},
        {'_', false},
        });
}

bool isPonto(char c, unordered_map<char, bool>pontos) 
{ // recebe um char e ve se ele eh um ponto a partir da procura do map de pontos
    auto it = pontos.find(c);
    if (it != pontos.end()) return true;
    return false;
}

string removePontuacao(string num, unordered_map<char, bool>pontos) 
{ // remove pontuacao e char estranho para n aparacer no output
    string palavraSemPonto;
    for (char c : num) { // percorre cada char da palavar pra ver se eh um ponto
        if (!isPonto(c, pontos)) { // se o char n for ponto, ele coloca na string palavraSemponto
            palavraSemPonto += c;
        }
    }
    return palavraSemPonto;
}

string tiraReticencia(string linha) 
{// retorna a linha sem reticencias
    string retiBug = ". . .";
    size_t pos = linha.find(retiBug);
    if (pos != std::string::npos)
        linha.erase(pos, retiBug.length());

    string reti = "...";
    size_t pos2 = linha.find(reti);
    if (pos2 != std::string::npos)
        linha.erase(pos2, reti.length());

    string reti2 = "..";
    size_t pos3 = linha.find(reti2);
    if (pos3 != std::string::npos)
        linha.erase(pos3, reti2.length());

    return linha;
}

bool temNumero(string palavra) 
{// verifica se tem numero
    for (int i = 0;i < palavra.size();i++) 
    {
        if (isdigit(palavra[i])) return true;
    }

    return false;
}

bool Pontuacao(string *num) 
{ // verifica se terminou a sentenca de acordo com os delimitadores
    bool encontrar = false;

    char ponto = '.', exclamacao = '!', interrogacao = '?';

    size_t pos1 = (*num).find(ponto);
    if (pos1 != std::string::npos) 
    {  // Verifica se o caractere foi encontrado
        if (!ispunct((*num)[pos1 - 1])) {
            (*num).erase(pos1, 1);
            encontrar = true;
        }
    }
    size_t pos2 = (*num).find(exclamacao);
    if (pos2 != std::string::npos) 
    {  // Verifica se o caractere foi encontrado
        (*num).erase(pos2, 1);
        encontrar = true;
    }
    size_t pos3 = (*num).find(interrogacao);
    if (pos3 != std::string::npos) 
    {  // Verifica se o caractere foi encontrado
        (*num).erase(pos3, 1);
        encontrar = true;
    }

    return encontrar;
}

string converterAcentuadasParaMinusculas(string num) 
{
    string maiuscula = "ÁÀÃÉÈÍÌÓÒÚÙ";
    string minuscula = "áàãéèíìóòúù";

    for (size_t i = 0; i < maiuscula.size(); i++) 
    {
        replace(num.begin(), num.end(), maiuscula[i], minuscula[i]);
    }
    return num;
}

void heap(unordered_map<string, int> &Hash)
{
    int i = 0;

    vector<pair<string, int>> vetor;

    auto it = Hash.begin();

    for (int i = 0; i < 20 && it != Hash.end(); i++, it++)
    {
        vetor.push_back(*it);
    }
    
    make_heap(vetor.begin(), vetor.end(), [](const auto& a, const auto& b){ return a.second > b.second;});

    while(it != Hash.end())
    {
        if(it -> second > vetor.front().second)
        {
            pop_heap(vetor.begin(), vetor.end());
            vetor.pop_back();
            vetor.push_back(*it);
            make_heap(vetor.begin(), vetor.end(), [](const auto& a, const auto& b){ return a.second > b.second;});
        }

        it++;
    }

    int contadorHeap = 1;

    for (const auto& pair : vetor)
    {
        cout << contadorHeap << "- Palavra: " << pair.first << "     Aparições: " << pair.second << endl;
        contadorHeap++;
    }
}

bool Remove_Caracter_Fora_ASCII(string s) 
{
    auto acentos = {"^" ,"--", " — ", " —", "—", "”", "-", "——", "“", "-”"};
    auto acentos_2 = {'"','-', '\xE2', '\x9D','\x80', '\x9C', '\x94'};

    for (auto a : acentos) {
        if (s.find(a) != s.npos) {
            return false;
        }
    }
    for (auto a : acentos_2) {
        if (s.find(a) != s.npos) {
            return false;
        }
    }

    return true;
}