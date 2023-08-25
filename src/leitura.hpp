#ifndef LEITURA_HPP
#define LEITURA_HPP

#include <bits/stdc++.h>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <locale>
#define endl '\n'

using namespace std;

void readPrincipal();

void preencheMapPontos(unordered_map<char, bool> &pontos);
void readStopWords(unordered_map<string, bool> &stopWords);

void insereHash(unordered_map<string, int> &hash, string palavraLinha, int &contPalavras, int quantidade);

string tiraReticencia(string linha);
string removePontuacao(string num, unordered_map<char, bool>pontos);
string converterAcentuadasParaMinusculas(string num);
bool temNumero(string palavra);
bool Pontuacao(string *num);
bool isPonto(char c, unordered_map<char, bool>pontos);

void heap(unordered_map<string, int> &hash);

bool Remove_Caracter_Fora_ASCII(string s);

#endif  