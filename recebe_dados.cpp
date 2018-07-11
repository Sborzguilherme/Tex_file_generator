#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <locale>
#include <locale.h>
#include <string.h>
#include "projeto.h"
#include <set>
#include <windows.h>
#include <direct.h>

using namespace std;

void le_projetos(string arquivo, vector<Projeto> &vetor);               // Função para leitura do CSV
void escreve_projetos(vector<Projeto> vetor, string arquivos);          // Função para gerar arquivos .TEX
string Trata_maiusculo_minusculo(string nome, bool palavra_chave);      // Primeira letra de cada palvra em maisculo
vector<string> trata_sobrenome(string sobrenome);                       // Encontra sobrenome e passa para maisculo
string nome_concatenado(vector<string> nomes);                          // Concatenação de vetor de strings
string Transforma_Maisculo_Minusculo(string palavra, bool Maiusculo);   // Poe  string só em caixa alta ou baixa
string Retira_Ponto_Final_Espaco(string nome, bool palavra_chave);      // Retira ponto final e espaço do fim da string
string Retira_Acentuacao(string nome);                                  // Função para tratamento de strings acentuadas
string Gera_string_arquivo_input(string inputs, int grande_area);       // Gera a string para arquivos de input
const vector<string> explode(const string& s, const char& c);           // Função para separação de strings

/*Função para leitura do arquivo CSV
    O vetor passado por referência é atualizado com os dados lidos do arquivo
*/
void le_projetos(string arquivo, vector<Projeto> &vetor){       // Vetor é passado por referência

    vector<string> linha_arquivo;                               // Vetor que recebe os elementos da linha do arquivo
    fstream file(arquivo, ios::in);                             // Abertura do arquivo

    if(!file.is_open()){                                        // Caso o arquivo a ser aberto não seja encontrado
            cout << "Arquivo não encontrado!\n";
            return;
         }

    int linha_controle = 0;                     // Variável para garantir que a linha de labels do CSV seja ignorada
    string csvLinha, nome_projeto;
    //vector <string> palavra_chave;
    vector<string> nome_bolsista;
    vector<string> nome_orientador;
    vector<string>vetor_nome_projeto;
    int i=0;

    while(getline(file, csvLinha)){
        if(linha_controle == 0){                            // Primeira linha (Título do projeto de pesquisa)
            vetor_nome_projeto = explode(csvLinha, ';');
            nome_projeto = vetor_nome_projeto.front();

            // Percorre a string para encontrar um '-' e coloca mais 2
            // Travessão no latex = "---"
            while(nome_projeto.at(i)!='-'){                 // Busca a posição em que o '-' se encontra
                i++;
            }
            nome_projeto.insert(i, "--");                   // Nessa posição são adicionas mais 2 '-'
            linha_controle++;                               // Incrementa linha de controle
        }else if(linha_controle == 1){                      // Pula segunda linha (Labels)
            linha_controle++;
        }else {                                         // Linha com dados
                istringstream csvStream(csvLinha);      // Recebe uma linha completa do CSV
                vector<string> csvColuna;               // Vetor cujo elementos são as colunas da linha do CSV
                string csvElemento;                     // Elemento da Coluna
                Projeto novo_projeto;                   // Nova instância da classe que será inserida no vetor

                //stringstream concatena_bolsista[10], concatena_orientador[10];

                while( getline(csvStream, csvElemento, ';') ){  // Separa itens na linha por ';'
                    csvColuna.push_back(csvElemento);           // Cada elemento do vetor csvColuna é um item diferente
                }

                // Definição do título do projeto de pesquisa (bolsa)
                 novo_projeto.setTitulo_Projeto(nome_projeto);;

                 // Determinação dos atributos a partir de cada coluna do arquivo CSV
                novo_projeto.setCodigo(csvColuna.at(0));

                novo_projeto.setBolsista_nome_completo(Trata_maiusculo_minusculo(csvColuna.at(1), false));

                // Vetor identificando separação entre nome e sobrenome
                nome_bolsista = trata_sobrenome(csvColuna.at(1));

                // Sobrenome em letras maisculas
                novo_projeto.setBolsista_sobrenome(nome_bolsista.at(1));                // Define sobrenome

                novo_projeto.setBolsista_nome(Trata_maiusculo_minusculo(nome_bolsista.at(0), false));

                novo_projeto.setOrientador_nome_completo(Trata_maiusculo_minusculo(csvColuna.at(2), false));

                nome_orientador = trata_sobrenome(csvColuna.at(2));
                novo_projeto.setOrientador_sobrenome(nome_orientador.at(1));

                novo_projeto.setOrientador_nome(Trata_maiusculo_minusculo(nome_orientador.at(0), false));

                novo_projeto.setCurso(Trata_maiusculo_minusculo(csvColuna.at(3), false));

                novo_projeto.setTitulo(csvColuna.at(4));

                novo_projeto.setArea((Trata_maiusculo_minusculo(csvColuna.at(5), false)));

                novo_projeto.setPalavra_chave(Trata_maiusculo_minusculo(csvColuna.at(6), true));

                novo_projeto.setEscola(Transforma_Maisculo_Minusculo(csvColuna.at(7), true));

                //aux = Retira_Ponto_Final_Espaco(csvColuna.at(8), false);
                novo_projeto.setArea_apresentacao(Trata_maiusculo_minusculo(csvColuna.at(8), false));
                vetor.push_back(novo_projeto);

        }// else
    }// while
    file.close();
}// função

/*  Função para geração dos arquivos .TEX
    Passa-se como parâmetro o vetor com os dados dos projetos lidos anteriormente
    O caminho do arquivo .TEX, bem como seu layout são definidos nesta função
*/
void escreve_projetos(vector<Projeto> vetor, string arquivo){

    ofstream arquivo_escrita;
    string nome, sobrenome, area_apresentacao, escreve_input;
    stringstream concatena;
    vector<string> identifica_nome, v_sobrenome;
    set<string>s_vida, s_humanas, s_exatas;           // Estruturas para ordenação dos autores por grande área
    Projeto escrita;

    while (!vetor.empty()) {

        escrita = vetor.at(0);                  // Recebe vetor na primeira posição

        while(!identifica_nome.empty()){        // Esvazia vetor
            identifica_nome.pop_back();
        }
        identifica_nome = explode(escrita.getBolsista_nome_completo(), ' ');

        // Nome do arquivo .TEX é "sobrenome_primeiroNome" do bolsista
        nome = identifica_nome.front();
        nome = Retira_Ponto_Final_Espaco(nome, false);
        nome = Retira_Acentuacao(nome);
        v_sobrenome = explode(escrita.getBolsista_sobrenome(),' ');
        if(v_sobrenome.size()==1){
            sobrenome = v_sobrenome.at(0);
        }else{
            sobrenome = v_sobrenome.at(0) + "_" + v_sobrenome.at(1);
        }
        sobrenome = Retira_Ponto_Final_Espaco(sobrenome, false);
        sobrenome = Retira_Acentuacao(sobrenome);
        area_apresentacao = Retira_Ponto_Final_Espaco(escrita.getArea_apresentacao(), false);
        area_apresentacao = Transforma_Maisculo_Minusculo(area_apresentacao, false);

        //cout<<Retira_Ponto_Final_Espaco(escrita.getBolsista_nome(), false)<<"-"<<endl;

        concatena.str("");
        concatena << arquivo<<"/"<<area_apresentacao<<"/"<<Transforma_Maisculo_Minusculo(sobrenome, false)
                     << "_" << Transforma_Maisculo_Minusculo(nome, false) <<".tex";
        arquivo_escrita.open(concatena.str());

        // Definição do formato do arquivo .TEX
        arquivo_escrita <<"\\begin{conf-abstract}\n"
                        <<"% Título\n"
                        <<"{" << Retira_Ponto_Final_Espaco(escrita.getTitulo(), false) << "}\n"
                        <<"% Autores\n"
                        <<"{" << Retira_Ponto_Final_Espaco(escrita.getBolsista_sobrenome(), false) << ", "
                        << Retira_Ponto_Final_Espaco(escrita.getBolsista_nome(), false) << "; "
                        << Retira_Ponto_Final_Espaco(escrita.getOrientador_sobrenome(), false)
                        << ", " << Retira_Ponto_Final_Espaco(escrita.getOrientador_nome(), false) << "}\n"
                        << "% Curso/Escola\n"
                        << "{" << Retira_Ponto_Final_Espaco(escrita.getCurso(),false) << "/"
                        << Retira_Ponto_Final_Espaco(escrita.getEscola(), false) << "}\n"
                        << "% Área/Subárea\n"
                        << "{" << Retira_Ponto_Final_Espaco(escrita.getArea(), false) << "/"
                        << Retira_Ponto_Final_Espaco(escrita.getArea(), false) << "}\n"
                        << "% Resumo\n"
                        << "{}\n"
                        << "% Palavras chave\n"
                        << "{" << Retira_Ponto_Final_Espaco(escrita.getPalavra_chave(), true)
                        << "}\n"
                        << "% Programa de pesquisa\n"
                        << "{" << escrita.getTitulo_Projeto()<<"}\n"
                        << "% Indexação dos autores\n"
                        << "\\indexauthors{" << Retira_Ponto_Final_Espaco(escrita.getBolsista_nome_completo(), false)
                        <<", " << Retira_Ponto_Final_Espaco(escrita.getOrientador_nome_completo(), false)<<"}\n"
                        << "\\end{conf-abstract}";
        vetor.erase(vetor.begin());         // Apaga a primeira posição do vetor
        arquivo_escrita.close();            // Fecha arquivo atual para abertura e escrita do próximo

        // Nomes inseridos na estrutura set (Separados por área)

        escreve_input = concatena.str();    // Pega o nome completo do arquivo de escrita do projeto
        escreve_input.replace(escreve_input.begin(), escreve_input.begin()+arquivo.length()+1, ""); // Exclui o caminho inicial
        // Aqui os alunos são separados por área e ordenados pela estrutura set
        if(area_apresentacao == "vida"){s_vida.insert(escreve_input);}
        else if(area_apresentacao == "humanas"){s_humanas.insert(escreve_input);}
        else{s_exatas.insert(escreve_input);}

        //cout<<escreve_input<<endl;
    }// End While
    // Geração dos Arquivos de Input
    ofstream vida, humanas, exatas;                             // Controle individual de cada arquivo
    string input_vida ="", input_humanas="", input_exatas="";   // Strings para escrita das linhas de input

    // Após todas os nomes terem sido inseridos deve-se gerar a string para escrita no arquivo
    set<string>::iterator it_v = s_vida.begin();      // iterador para percorrer a estrutura set
    for(it_v; it_v != s_vida.end(); it_v++){
        input_vida+= "\\input{" + *it_v + "}\n";
    }
    set<string>::iterator it_h = s_humanas.begin();
    for(it_h; it_h != s_humanas.end(); it_h++){
        input_humanas+= "\\input{" + *it_h + "}\n";
    }
    set<string>::iterator it_e = s_exatas.begin();
    for(it_e; it_e != s_exatas.end(); it_e++){
        input_exatas+= "\\input{" + *it_e + "}\n";
    }

    string dir_vida, dir_hum, dir_exa;

    dir_vida = arquivo + "/grande_area_vida.tex";
    dir_hum  = arquivo + "/grande_area_humanas.tex";
    dir_exa  = arquivo + "/grande_area_exatas.tex";

    // Com as strings de input concatenas deve-se gerar os arquivos que receberão estes dados
    vida.open(dir_vida);
    vida << Gera_string_arquivo_input(input_vida, 0);
    vida.close();
    humanas.open(dir_hum);
    humanas << Gera_string_arquivo_input(input_humanas, 1);
    humanas.close();
    exatas.open(dir_exa);
    exatas<<Gera_string_arquivo_input(input_exatas, 2);
    exatas.close();

}
/* Função para tratar sobrenomes
   Parâmetro -> nome completo
   Retorno -> Sobrenome em caixa alta
*/
vector<string> trata_sobrenome(string sobrenome){

    vector<string>retorno;                  // Vector de 2 posições. [0] -> Nome, [1]-> Sobrenome
    vector<string> nomes;
    //stringstream concatena;
    string aux;
    unordered_set <string> casos_especias;
    //unordered_set <string> preposicao;
    nomes = explode(sobrenome, ' ');        //Divide o nome completo em espaçoes

    // Casos especiais para nomes
    casos_especias.insert("FILHO");
    casos_especias.insert("NETO");
    casos_especias.insert("JÚNIOR");
    casos_especias.insert("JUNIOR");
    // Preposições
    /*preposicao.insert("DE");
    preposicao.insert("DA");
    preposicao.insert("DO");
    preposicao.insert("DAS");
    preposicao.insert("DOS");
    */

    int ultima_posicao = nomes.size()-1;
    string ultimo_nome = nomes.at(ultima_posicao);              // Pega a última posição
    nomes.erase(nomes.end());                                   // Apaga a última posição

    string penultimo_nome = nomes.at(ultima_posicao-1);

    ultimo_nome = Transforma_Maisculo_Minusculo(ultimo_nome, true);
    penultimo_nome = Transforma_Maisculo_Minusculo(penultimo_nome, true);

    unordered_set<string>::const_iterator it = casos_especias.find(ultimo_nome);

    if(it == casos_especias.end()){     // Iterador chegando no fim da estrutura significa que dado não foi encontrado
        //unordered_set<string>::const_iterator it2 = preposicao.find(penultimo_nome);
        //if(it2 == preposicao.end()){                   // Não possui preposicao ou caso espcecial de nome
            retorno.push_back(nome_concatenado(nomes));// Nomes na posição 0
            retorno.push_back(ultimo_nome);            // Sobrenome (em caixa alta) na posição 1

        //}/*else{                                         // Possui preposicao
            //nomes.erase(nomes.end());                  // Apaga a penúltima posição (ja salva na variável penultimo_nome)
            //aux+= penultimo_nome + " " + ultimo_nome;
            //concatena << penultimo_nome<< " " << ultimo_nome;
            //retorno.push_back(nome_concatenado(nomes));
            //retorno.push_back(aux);
        //}
    }else{                                             // Possui nome dos casos especiais
        nomes.erase(nomes.end());
        aux+= penultimo_nome + " " + ultimo_nome;
        retorno.push_back(nome_concatenado(nomes));
        retorno.push_back(aux);
    }
    return retorno;
}
/* Função para transformação de string para Caixa alta
    Não converte caracteres acentuados. Para isso deve-se utilizar a função Retira_Acentuacao
*/
string Transforma_Maisculo_Minusculo(string palavra, bool Maiusculo){
    string retorno;
    // Letras com acentuação ou casos especiais devem ser tratados previamente
    const char *str = palavra.c_str();  // string palavra deve ser convertida para const char
    char *pch;                          // Ponteiro que indica a posição onde o caracter acentuado foi encontrado
    string insere;                      // Recebe o código utf8 que será inserido na string
    int posicao, contador = 0;
    while(contador < 11){
        switch(contador){
            case 0:
                if(Maiusculo){pch = strstr (str,"á"); insere="\u00C1";}     // Código UTF8 - "Á"
                else{pch = strstr (str,"Á");insere = "\u00E1";}             // Código UTF8 - "á"
                break;
            case 1:
                if(Maiusculo){pch = strstr (str,"é"); insere = "\u00C9";}   // "É"
                else{pch = strstr (str,"É"); insere = "\u00E9";}            // "é"
                break;
            case 2:
                if(Maiusculo){pch = strstr (str,"í"); insere = "\u00CD";}   // "Í"
                else{pch = strstr (str,"Í"); insere = "\u00ED";}            // "í"
                break;
            case 3:
                if(Maiusculo){pch = strstr (str,"ó"); insere="\u00D3";}     // "Ó"
                else{pch = strstr (str,"Ó");insere = "\u00F3";}             // "ó"
                break;
            case 4:
                if(Maiusculo){pch = strstr (str,"ú"); insere="\u00DA";}     // "Ú"
                else{pch = strstr (str,"Ú"); insere = "\u00FA";}            // "ú"
                break;
            case 5:
                if(Maiusculo){pch = strstr (str,"ç"); insere="\u00C7";}     // "Ç"
                else{pch = strstr (str,"Ç"); insere = "\u00E7";}            // "ç"
                break;
            case 6:
                if(Maiusculo){pch = strstr (str,"ã"); insere="\u00C3";}     // "Ã"
                else{pch = strstr (str,"Ã"); insere = "\u00E3";}            // "ã"
                break;
            case 7:
                if(Maiusculo){pch = strstr (str,"â"); insere="\u00C2";}     // "Â"
                else{pch = strstr (str,"Â"); insere = "\u00E2";}            // "â"
                break;
            case 8:
                if(Maiusculo){pch = strstr (str,"ê"); insere="\u00CA";}     // "Ê"
                else{pch = strstr (str,"Ê"); insere = "\u00EA";}            // "ê"
                break;
            case 9:
                if(Maiusculo){pch = strstr (str,"ô"); insere="\u00D4";}     // "Ô"
                else{pch = strstr (str,"Ô"); insere = "\u00F4";}            // "ô"
                break;
            case 10:
                if(Maiusculo){pch = strstr (str,"õ"); insere="\u00D5";}     // "Õ"
                else{pch = strstr (str,"Õ"); insere = "\u00F5";}            // "õ"
                break;
        }
        contador++;

        if(pch != NULL){
            posicao = pch-str;                          // Posição em que caractere especial foi encontrado
            palavra.erase(palavra.begin()+posicao);     // Deve-se retirar 2 caracteres naquela posição
            palavra.erase(palavra.begin()+posicao);
            palavra.insert(posicao, insere);            // Insere na posição correta o caractere especial transformado
    }
 }

    // Transformação para maisculo/minusculo das letras sem acentuação
    locale loc;
    for(size_t i=0; i<palavra.length(); i++){
        if(Maiusculo)retorno.push_back(toupper(palavra[i], loc));
        else retorno.push_back(tolower(palavra[i], loc));
    }
    return retorno;
}

// Função para concatenação de vetor de strings
string nome_concatenado(vector<string> nomes){
    //stringstream concatena;
    string concatena;
    while(!nomes.empty()){
        concatena+= nomes.front() + " ";
        nomes.erase(nomes.begin());
    }
    return concatena;
}
/* Função para que string receba somente a primeira letra maiuscula
    palavra_chave = True  -> Tratamento especial para palvras-chave por conta da possível presença de siglas
    palavra_chave = False -> Poe todas as letras da string em minusculo, depois poe em maisculo apenas a primeira letra
*/
string Trata_maiusculo_minusculo(string nome, bool palavra_chave){

    unordered_set <string> preposicao;              // Estrutura de dado com as preposições consideradas
    unordered_set<string>::const_iterator it;       // Iterador para busca de strings na estrutura

    // Preposições consideradas
    preposicao.insert("de");
    preposicao.insert("da");
    preposicao.insert("do");
    preposicao.insert("das");
    preposicao.insert("dos");

    vector<string> nome_separado = explode(nome, ' ');             // Vetor com os nomes separados
    vector<string> nomes_tratados;                                 // Vetor com os nomes ja tratados
    string aux;
    string primeiro_nome;
    locale loc;

    while(!nome_separado.empty()){                                 // Percorre todos os nomes
        aux = "";                                                  // Zera a string auxiliar
        if(!palavra_chave){
            //primeiro_nome = Retira_Acentuacao(nome_separado.front(), false);
            primeiro_nome = Transforma_Maisculo_Minusculo(nome_separado.front(), false);   // Passa o primeiro nome para letras minusculas
        }else{  // Tratamanto para as siglas das Palavras-Chave
            primeiro_nome = nome_separado.front();
        }
            it = preposicao.find(primeiro_nome);            // Tenta encontrar cada um dos nomes entre as preposições
        if(it == preposicao.end()){                         // Caso não encontre
            for(int i=0; i<primeiro_nome.length();i++){
                if(i==0){
                    aux.push_back(toupper(primeiro_nome[i],loc));       // Transforma primeira letra em maiúscula
                }else aux.push_back(primeiro_nome[i]);                  // Outras letras não são alteradas
            }
            nomes_tratados.push_back(aux);                              // Coloca string aux no vetor de nomes tratados
        }else{                                       // Caso encontre uma preposição
            nomes_tratados.push_back(primeiro_nome); // Adiciona no vetor a preposição (sem necessidade de tratamento)
        }
        nome_separado.erase(nome_separado.begin());   // Apaga a primeira posição
    }

    return nome_concatenado(nomes_tratados);

}
/* Função para retirar ponto final e espaço do último caracter da string
    palavra_chave  = True -> Faz tratamento especial para consideração das palavras chave (devem terminar com ponto)
    palavra_chave = False -> Retira ponto final ou espaço do fim da string
*/
string Retira_Ponto_Final_Espaco(string nome, bool palavra_chave){
    char t = nome.at(nome.length()-1);
    char t_ant = nome.at(nome.length()-2);
    if(!palavra_chave){                 // Se não for palavra chave retira caracteres ' ' e '.' do fim
        if(t == '.' || t == ' '){
            nome.pop_back();
        }
        return nome;
    }else{                              // Tratamento especial para palavras-chave
        if(t == '.'){
            if(t_ant == ' '){           // Se terminar em espaço + ponto
                nome.pop_back();
                nome.pop_back();
                nome.push_back('.');
                return nome;
            }else return nome;          // Se terminar em ponto
        }else if(t == ' '){
            if(t_ant == '.'){           // Se terminar em ponto + espaço
                nome.pop_back();
                return nome;
            }else{                      // Se terminar em letra + espaço
                nome.pop_back();
                nome.push_back('.');
                return nome;
            }
        }else{                          // Se terminar com letra
            nome.push_back('.');
        }
    }

    return nome;
}
/* Função para tratamento de acentos e cedilhas
    retira =  True -> Verifica a presença de acentos em letras minúsculas e subtitui por letra minuscula sem acento
    retira = False -> Verifica a presença de acentos em letras maisculas e subtitui por letras minusculas acentuadas
*/
string Retira_Acentuacao(string nome){

    // teste.insert(teste.begin()+posicao,237);
    const char *str = nome.c_str();
    char * pch, s;
    int posicao, contador = 0;
    while(contador < 11){
        switch(contador){
            case 0:  pch = strstr (str,"á"); s='a'; break;
            case 1:  pch = strstr (str,"é"); s='e'; break;
            case 2:  pch = strstr (str,"í"); s='i'; break;
            case 3:  pch = strstr (str,"ó"); s='o'; break;
            case 4:  pch = strstr (str,"ú"); s='u'; break;
            case 5:  pch = strstr (str,"ç"); s='c'; break;
            case 6:  pch = strstr (str,"ã"); s='a'; break;
            case 7:  pch = strstr (str,"â"); s='a'; break;
            case 8:  pch = strstr (str,"ê"); s='e'; break;
            case 9:  pch = strstr (str,"ô"); s='o'; break;
            case 10: pch = strstr (str,"õ"); s='o'; break;
        }
        contador++;

        if(pch != NULL){
            posicao = pch-str;
            nome.erase(nome.begin()+posicao);
            nome.erase(nome.begin()+(posicao));
            nome.insert(nome.begin()+posicao,1,s);
        }
    }
    return nome;
}
// Função para geração da string que será escrita nos arquivos grande_area.tex
string Gera_string_arquivo_input(string inputs, int grande_area){
    string retorno;
    vector<string>centro_Academico;                             // Vector com strings (constantes) dos nomes dos centros
    centro_Academico.push_back("Ciências Biológicas");
    centro_Academico.push_back("Ciências Humanas");
    centro_Academico.push_back("Ciências Exatas e da Terra");

    retorno = "\\chapter{" + centro_Academico.at(grande_area) + "}\n" +
              "\\begin{figure}[!h]\n\t\\centering\n\t\\includegraphics[scale=1.3]{imagens/sciences.png}\n"+
              "\\end{figure}\n\\vfill\n\\hrule\n\\pagebreak\n\n" + inputs;
    return retorno;
}

/* Função de separação dos caracteres
    Parâmetros(String a ser separada, caractere usado para separação)
    Retorno da função: Vetor em que cada posição corresponde a um elemento diferente
    (Elementos são definidos pelo caracter sepradador passado por parâmetro)
*/
const vector<string> explode(const string& s, const char& c){
    string buff{""};
    vector<string> v;

    for(auto n:s){
        if(n != c) buff+=n; else
        if(n == c && buff != "") { v.push_back(buff); buff = ""; }
    }
    if(buff != "") v.push_back(buff);
    return v;
}
