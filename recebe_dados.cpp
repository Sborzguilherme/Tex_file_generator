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

void Le_projetos(string arquivo, vector<Projeto> &vetor);               // Função para leitura do CSV
void Escreve_projetos(vector<Projeto> vetor, string arquivos);          // Função para gerar arquivos .TEX
vector<string> Trata_sobrenome(string sobrenome);                       // Encontra sobrenome e passa para maisculo
string Nome_concatenado(vector<string> nomes);                          // Concatenação de vetor de strings
string Transforma_maisculo_minusculo(string palavra, bool Maiusculo);   // Poe  string só em caixa alta ou baixa
string Trata_maiusculo_minusculo(string nome, bool palavra_chave);      // Primeira letra de cada palvra em maisculo
string Retira_ponto_final_espaco(string nome, bool palavra_chave);      // Retira ponto final e espaço do fim da string
string Retira_acentuacao(string nome);                                  // Função para tratamento de strings acentuadas
string Gera_string_arquivo_input(string inputs, int grande_area);       // Gera a string para arquivos de input
string Define_escola(string sigla);                                     // Gera nome completo da escola a partir da sigla
void Define_resumo(string diretorio, vector<Projeto>&vetor);            // Leitura de arquivos contendo resumo
string Sobrenome_arquivo(string nome, string sobrenome);                // Retorna nome e sobrenome na formatação para escrita do arquivo
void Cria_lista_arquivos(vector<Projeto>vetor);                         // Cria txt com nomes do arquivos.TEX
void Le_CSV_resumo(string arquivo);                                     // Leitura do CSV vindo do formulário
string Trata_autores_adicionais(vector<string> vetor_autores, string orientador, string bolsista);          // Função para definição dos autores adicionais
const vector<string> Explode(const string& s, const char& c);           // Função para separação de strings

/*Função para leitura do arquivo CSV
    O vetor passado por referência é atualizado com os dados lidos do arquivo
*/
void Le_projetos(string arquivo, vector<Projeto> &vetor){   // Vetor é passado por referência

    vector<string> linha_arquivo;                               // Vetor que recebe os elementos da linha do arquivo
    fstream file(arquivo, ios::in);                             // Abertura do arquivo

    if(!file.is_open()){                                        // Caso o arquivo a ser aberto não seja encontrado
            cout << "Arquivo não encontrado!\n"<<endl;
            return;
     }

    int linha_controle = 0;                     // Variável para garantir que a linha de labels do CSV seja ignorada
    string csvLinha, nome_projeto;
    //vector <string> palavra_chave;
    vector<string> nome_bolsista;               // Vetor que recebe nomes separados do bolsista
    vector<string> nome_orientador;             // Vetor que recebe nomes separados do orientador
    vector<string>vetor_nome_projeto;           // Vetor que identifica o projeto de pesquisa
    int i=0;
    while(getline(file, csvLinha)){
        if(linha_controle == 0){                            // Primeira linha (Título do projeto de pesquisa)
            vetor_nome_projeto = Explode(csvLinha, ';');
            nome_projeto = vetor_nome_projeto.front();

            const char *str = nome_projeto.c_str();  // string palavra deve ser convertida para const char
            char *pch;                          // Ponteiro que indica a posição onde o caracter acentuado foi encontrado
            //int pos;
            pch = strstr (str,"\uFEFF");                       // Tenta encontrar caracter especial
            //pos = pch-str;
            nome_projeto.erase(nome_projeto.begin());          // Deve-se retirar 3 caracteres naquela posição
            nome_projeto.erase(nome_projeto.begin());
            nome_projeto.erase(nome_projeto.begin());

            // Percorre a string para encontrar um '-' e coloca mais 2
            // Travessão no latex = "---"
            while(nome_projeto.at(i)!='-'){                 // Busca a posição em que o '-' se encontra
                i++;
            }
            nome_projeto.insert(i, "-");                   // Nessa posição são adicionas mais 2 '-'
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
                // Nome completo do bolsista
                novo_projeto.setBolsista_nome_completo(Trata_maiusculo_minusculo(csvColuna.at(1), false));
                // Vetor identificando separação entre nome e sobrenome
                nome_bolsista = Trata_sobrenome(csvColuna.at(1));
                // Sobrenome em letras maisculas
                novo_projeto.setBolsista_sobrenome(nome_bolsista.at(1));                // Define sobrenome
                // Nomes restantes em letra minuscula definem o nome do bolsista
                novo_projeto.setBolsista_nome(Trata_maiusculo_minusculo(nome_bolsista.at(0), false));
                // Curso
                novo_projeto.setCurso(Trata_maiusculo_minusculo(csvColuna.at(2), false));
                // Nome completo do orientador
                novo_projeto.setOrientador_nome_completo(Trata_maiusculo_minusculo(csvColuna.at(3), false));
                // Define sobrenome do orientador
                nome_orientador = Trata_sobrenome(csvColuna.at(3));
                novo_projeto.setOrientador_sobrenome(nome_orientador.at(1));
                // Nomes restantes definem o nome do orientador
                novo_projeto.setOrientador_nome(Trata_maiusculo_minusculo(nome_orientador.at(0), false));
                // Título
                novo_projeto.setTitulo(csvColuna.at(4));
                // Área
                novo_projeto.setArea((Trata_maiusculo_minusculo(csvColuna.at(5), false)));
                // Palavra-Chave (Tratada como uma só string)
                novo_projeto.setPalavra_chave(Trata_maiusculo_minusculo(csvColuna.at(6), true));
                // Escola 
                novo_projeto.setEscola(Define_escola(Transforma_maisculo_minusculo(csvColuna.at(7), true)));
                // Área de apresentação (Grande área)
                novo_projeto.setArea_apresentacao(Trata_maiusculo_minusculo(csvColuna.at(8), false));
                vetor.push_back(novo_projeto);  // Adiciona o novo projeto criado ao vetor de projetos
        }// else
    }// while
    file.close();
}// função

/*  Função para geração dos arquivos .TEX
    Passa-se como parâmetro o vetor com os dados dos projetos lidos anteriormente
    O caminho do arquivo .TEX, bem como seu layout são definidos nesta função
*/
void Escreve_projetos(vector<Projeto> vetor, string arquivo){

    ofstream arquivo_escrita;                                                       // Arquivo a ser aberto
    string nome_arquivo, area_apresentacao, escreve_input, template_arquivo;        // String auxiliares
    stringstream concatena; // Variável utilizada para concatenação de strings (podem ser utilizados meétodos diferentes para concatenação)
    vector<string> identifica_nome;                                                 // Vetores auxiliares
    set<string>s_vida, s_humanas, s_exatas, nomes_utilizados, s_pos;   // Estruturas para ordenação dos autores por grande área
    Projeto escrita;
    string orientador_arq, bolsista_arq;

    while (!vetor.empty()){ /* A cada iteraçã lê-se uma posição do vetor gerando um arquivo .TEX para a mesma. As
                             posições lidas são retiradas do vetor. Processo se repete até que o mesmo esteja vazio*/

        escrita = vetor.at(0);                  // Recebe vetor na primeira posição

        while(!identifica_nome.empty()){        // Esvazia vetor para a próxima iteração
            identifica_nome.pop_back();
        }

        // Tratamento da área de apresentação (define o diretório de inclusão dos arquivos)
        area_apresentacao = Retira_ponto_final_espaco(escrita.getArea_apresentacao(), false);
        area_apresentacao = Transforma_maisculo_minusculo(area_apresentacao, false);

        if(area_apresentacao == "pos"){
            template_arquivo = "Pos-Graduacao";
        }else{
            template_arquivo = "conf-abstract";
        }

        string escrita_escola;

        if(escrita.getEscola().empty()){
            escrita_escola = "";
        }else{
            escrita_escola = Retira_ponto_final_espaco(escrita.getEscola(), false);
        }


        string escrita_autores_adicionais;

        if(escrita.getAutores_adicionais().empty()){
            //cout<<escrita.getBolsista_nome_completo()<<endl;
            escrita_autores_adicionais = "";
        }else{
            escrita_autores_adicionais = "";        // Limpa a string

            orientador_arq = Sobrenome_arquivo(escrita.getOrientador_nome(), escrita.getOrientador_sobrenome());
            bolsista_arq = Sobrenome_arquivo(escrita.getBolsista_nome(), escrita.getBolsista_sobrenome());

            escrita_autores_adicionais = Trata_autores_adicionais(escrita.getAutores_adicionais(), orientador_arq, bolsista_arq);
            //escrita_autores_adicionais = Retira_ponto_final_espaco(escrita_autores_adicionais, false);
        }
        concatena.str("");  // Reseta a string que define o nome do arquivo para a próxima iteração
        nome_arquivo = Sobrenome_arquivo(escrita.getBolsista_nome(), escrita.getBolsista_sobrenome());

        if(!nomes_utilizados.empty()){
            set<string>::iterator it;
            for(it = nomes_utilizados.begin(); it!=nomes_utilizados.end();it++){
                if(*it == nome_arquivo){
                    nome_arquivo+="_2";
                    break;
                }
            }
        }
        nomes_utilizados.insert(nome_arquivo);  // Adiciona nome ao set

        // Define diretório de escrita do arquivo .TEX
        concatena << arquivo<<"/"<<area_apresentacao<<"/"<<nome_arquivo <<".tex";
        arquivo_escrita.open(concatena.str());

        // Definição do formato do arquivo .TEX
        arquivo_escrita <<"\\begin{" << template_arquivo <<"}\n"
                       <<"% Título\n"
                      <<"{" << Retira_ponto_final_espaco(escrita.getTitulo(), false) << "}\n"
                     <<"% Autores\n"
                    <<"{" << Retira_ponto_final_espaco(escrita.getBolsista_sobrenome(), false) << ", "
                   << Retira_ponto_final_espaco(escrita.getBolsista_nome(), false) << "; "
                   << escrita_autores_adicionais
                   << Retira_ponto_final_espaco(escrita.getOrientador_sobrenome(), false)
                   << ", " << Retira_ponto_final_espaco(escrita.getOrientador_nome(), false) << "}\n"
                   << "% Curso\n"
                   << "{" << Retira_ponto_final_espaco(escrita.getCurso(),false) << "}\n"
                   <<"% Escola\n"
                  << "{" << escrita_escola << "}\n"
                  << "% Área\n"
                  << "{" << Retira_ponto_final_espaco(escrita.getArea(), false)<< "}\n"
                  << "% Resumo\n"
                  << "{" << escrita.getResumo()<<"}\n"
                     //<<"{\\blindtext[5][1]}\n" // Geração de parágrafos aleatórios no LaTeX
                  << "% Palavras-chave\n"
                  << "{" << Retira_ponto_final_espaco(escrita.getPalavra_chave(), true)
                  << "}\n"
                  << "% Programa de pesquisa\n"
                  << "{" << escrita.getTitulo_Projeto()<<"}\n"
                  << "% Indexação dos autores\n"
                  << "\\indexauthors{" << Retira_ponto_final_espaco(escrita.getBolsista_nome_completo(), false)
                  <<", " << Retira_ponto_final_espaco(escrita.getOrientador_nome_completo(), false)<<"}\n"
                 << "\\end{"<<template_arquivo<<"}";
        vetor.erase(vetor.begin());         // Apaga a primeira posição do vetor
        arquivo_escrita.close();            // Fecha arquivo atual para abertura e escrita do próximo

        // Nomes inseridos na estrutura set (Separados por área)

        escreve_input = concatena.str();    // Pega o nome completo do arquivo de escrita do projeto
        escreve_input.replace(escreve_input.begin(), escreve_input.begin()+arquivo.length()+1, ""); // Exclui o caminho inicial
        // A variável escreve_input recebe apenas a parte contendo o "sobrenome_primeiroNome" do diretório

        if(!escrita.getResumo().empty()){    // Só acrescenta no arquivo de input aqueles que enviaram resumo
            // Aqui os alunos são separados por área e ordenados pela estrutura set
            if(area_apresentacao == "vida"){s_vida.insert(nome_arquivo);}
            else if(area_apresentacao == "humanas"){s_humanas.insert(nome_arquivo);}
            else if(area_apresentacao == "exatas"){s_exatas.insert(nome_arquivo);}
            else(s_pos.insert(nome_arquivo));
        }
    }// End While

    // Geração dos Arquivos de Input
    ofstream vida, humanas, exatas, pos;                             // Controle individual de cada arquivo
    string input_vida ="", input_humanas="", input_exatas="", input_pos="";   // Strings para escrita das linhas de input

    // Após todas os nomes terem sido inseridos deve-se gerar a string para escrita no arquivo
    set<string>::iterator it_v;      // iterador para percorrer a estrutura set
    for(it_v = s_vida.begin(); it_v != s_vida.end(); it_v++){
        input_vida+= "\\input{vida/" + *it_v + "}\\clearpage\n";
    }
    //cout<<"INPUT VIDA"<<input_vida<<endl;
    set<string>::iterator it_h;
    for(it_h = s_humanas.begin(); it_h != s_humanas.end(); it_h++){
        input_humanas+= "\\input{humanas/" + *it_h + "}\\clearpage\n";
    }
     //cout<<"INPUT HUMANAS"<<input_humanas<<endl;
    set<string>::iterator it_e;
    for(it_e = s_exatas.begin(); it_e != s_exatas.end(); it_e++){
        input_exatas+= "\\input{exatas/" + *it_e + "}\\clearpage\n";
    }
    //cout<<"INPUT EXATAS"<<input_exatas<<endl;
    set<string>::iterator it_p;
    for(it_p = s_pos.begin(); it_p != s_pos.end(); it_p++){
        input_pos+= "\\input{pos/" + *it_p + "}\\clearpage\n";
    }
    //cout<<"INPUT POS"<<input_pos<<endl;
    string dir_vida, dir_hum, dir_exa, dir_pos;

    dir_vida = arquivo + "/grande_area_vida.tex";
    dir_hum  = arquivo + "/grande_area_humanas.tex";
    dir_exa  = arquivo + "/grande_area_exatas.tex";
    dir_pos  = arquivo + "/pos.tex";

    // Com as strings de input concatenadas deve-se gerar os arquivos que receberão estes dados
    vida.open(dir_vida);
    vida << Gera_string_arquivo_input(input_vida, 0);
    vida.close();
    humanas.open(dir_hum);
    humanas << Gera_string_arquivo_input(input_humanas, 1);
    humanas.close();
    exatas.open(dir_exa);
    exatas<<Gera_string_arquivo_input(input_exatas, 2);
    exatas.close();
    pos.open(dir_pos);
    pos<<Gera_string_arquivo_input(input_pos, 3);
    pos.close();
}
/* Função para tratar sobrenomes
   Parâmetro -> nome completo
   Retorno -> Sobrenome em caixa alta
*/
vector<string> Trata_sobrenome(string sobrenome){

    vector<string>retorno;                  // Vector de 2 posições. [0] -> Nome, [1]-> Sobrenome
    vector<string> nomes;
    //stringstream concatena;
    string aux;
    unordered_set <string> casos_especias;
    //unordered_set <string> preposicao;
    nomes = Explode(sobrenome, ' ');        //Divide o nome completo em espaçoes

    /*Casos especiais para nomes -> Se o último for uma das opções abaixo, deve-se pegar também o penúltimo nome
                                    para definição do sobrenome para citação*/
    casos_especias.insert("FILHO");
    casos_especias.insert("NETO");
    casos_especias.insert("JÚNIOR");
    casos_especias.insert("JUNIOR");
    // Preposições -> Não devem ser consideradas para definição do sobrenome no momento da citação do autor
    /*preposicao.insert("DE");
    preposicao.insert("DA");
    preposicao.insert("DO");
    preposicao.insert("DAS");
    preposicao.insert("DOS");
    */

    int ultima_posicao = nomes.size()-1;
    string ultimo_nome = nomes.at(ultima_posicao);              // Pega a última posição (sempre será usada)
    nomes.erase(nomes.end());                                   // Apaga a última posição

    string penultimo_nome = nomes.at(ultima_posicao-1);         // Penúltimo nome pode não ser utilizado

    ultimo_nome = Transforma_maisculo_minusculo(ultimo_nome, true);
    penultimo_nome = Transforma_maisculo_minusculo(penultimo_nome, true);

    unordered_set<string>::const_iterator it = casos_especias.find(ultimo_nome);

    if(it == casos_especias.end()){     // Iterador chegando no fim da estrutura significa que dado não foi encontrado
        //unordered_set<string>::const_iterator it2 = preposicao.find(penultimo_nome);
        //if(it2 == preposicao.end()){                   // Não possui preposicao ou caso espcecial de nome
            retorno.push_back(Nome_concatenado(nomes));// Nomes na posição 0
            retorno.push_back(ultimo_nome);            // Sobrenome (em caixa alta) na posição 1

        //}/*else{                                         // Possui preposicao
            //nomes.erase(nomes.end());                  // Apaga a penúltima posição (ja salva na variável penultimo_nome)
            //aux+= penultimo_nome + " " + ultimo_nome;
            //concatena << penultimo_nome<< " " << ultimo_nome;
            //retorno.push_back(Nome_concatenado(nomes));
            //retorno.push_back(aux);
        //}
    }else{                                             // Possui nome dos casos especiais
        nomes.erase(nomes.end());
        aux+= penultimo_nome + " " + ultimo_nome;
        retorno.push_back(Nome_concatenado(nomes));
        retorno.push_back(aux);
    }
    return retorno;
}
/* Função para transformação de string para Caixa alta ou baixa
   Parâmetros -> palavra a ser transformada, definição de transformação para maiusculo(true) ou minusculo(false)
   Utilizam-se as funções toupper e tolower para transformção de cada caracter individualmente para minusculo ou
   maiusculo. No entanto esta função não transforma caracteres especiais (acentuados ou "ç"). Para isso deve-se fazer
   esta transformação manualmente. Os valores utilizados para determinar os caracteres em maiusculo ou minusculo são
   referentes ao códigos UTF8.
*/
string Transforma_maisculo_minusculo(string palavra, bool Maiusculo){
    string retorno;
    // Letras com acentuação ou casos especiais devem ser tratados previamente
    const char *str = palavra.c_str();  // string palavra deve ser convertida para const char
    char *pch;                          // Ponteiro que indica a posição onde o caracter acentuado foi encontrado
    string insere;                      // Recebe o código utf8 que será inserido na string
    int posicao, contador = 0;
    while(contador < 11){
        switch(contador){
            case 0:
            /* Função strstr procura por uma determinada string em uma variável const char. Caso encotre, pch indica
             * a posição em que a mesma foi encontrada */
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
        contador++;     // Garante que todos os casos serão buscados na palavra

        if(pch != NULL){  // Indica que o caso sendo verificado atualmente foi encontrado na palavra
            posicao = pch-str;                          // Posição em que caractere especial foi encontrado
            palavra.erase(palavra.begin()+posicao);     // Deve-se retirar 2 caracteres naquela posição (caracter acentuado = 2 caracteres)
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
string Nome_concatenado(vector<string> nomes){
    //stringstream concatena;
    string concatena;
    while(!nomes.empty()){
        concatena+= nomes.front() + " ";
        nomes.erase(nomes.begin());
    }
    return concatena;
}
/* Função para que string receba somente a primeira letra maiuscula
    palavra_chave = True  -> Tratamento especial para palvras-chave, por conta da possível presença de siglas
    palavra_chave = False -> Põe todas as letras da string em minusculo, depois põe em maiusculo apenas a primeira letra
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
    preposicao.insert("e");
    preposicao.insert("em");
    preposicao.insert("no");

    vector<string> nome_separado = Explode(nome, ' ');             // Vetor com os nomes separados
    vector<string> nomes_tratados;                                 // Vetor com os nomes ja tratados
    string aux;
    string primeiro_nome;
    locale loc;

    while(!nome_separado.empty()){                                 // Percorre todos os nomes
        aux = "";                                                  // Zera a string auxiliar
        if(!palavra_chave){
            //primeiro_nome = Retira_acentuacao(nome_separado.front(), false);
            primeiro_nome = Transforma_maisculo_minusculo(nome_separado.front(), false);   // Passa o primeiro nome para letras minusculas
        }else{  // Tratamanto para as siglas das Palavras-Chave
            primeiro_nome = nome_separado.front();
        }
            it = preposicao.find(primeiro_nome);            // Tenta encontrar cada um dos nomes entre as preposições
        if(it == preposicao.end()){                         // Caso não encontre
            for(int i=0; i<primeiro_nome.length();i++){
                if(i==0){
                    aux.push_back(toupper(primeiro_nome[i],loc));       // Transforma primeira letra em maiúscula
                }else{
                    if(primeiro_nome[i] == '.' && palavra_chave == false){           // Se for sigla com '.'
                        aux.push_back(primeiro_nome[i]);                             // Inclui pontuação
                        if(primeiro_nome[i+1] != ' ' && primeiro_nome[i+1]!= NULL){
                            aux.push_back(toupper(primeiro_nome[i+1],loc));          // E próxima letra em maisculo

                        }else{
                            //aux.push_back(' ');
                        }                                                            // Atualiza o valor de i
                        i++;
                    }else aux.push_back(primeiro_nome[i]);                           // Outras letras não são alteradas
                }
            }
            nomes_tratados.push_back(aux);                              // Coloca string aux no vetor de nomes tratados
        }else{                                       // Caso encontre uma preposição
            nomes_tratados.push_back(primeiro_nome); // Adiciona no vetor a preposição (sem necessidade de tratamento)
        }
        nome_separado.erase(nome_separado.begin());   // Apaga a primeira posição
    }

    return Nome_concatenado(nomes_tratados);

}
/* Função para retirar ponto final e espaço do último caracter da string
    palavra_chave  = True -> Faz tratamento especial para consideração das palavras chave (devem terminar com ponto)
    palavra_chave = False -> Retira ponto final ou espaço do fim da string
*/
string Retira_ponto_final_espaco(string nome, bool palavra_chave){
    char t = nome.at(nome.length()-1);
    char t_ant = nome.at(nome.length()-2);
    if(!palavra_chave){                 // Se não for palavra chave retira caracteres ' ' e '.' do fim
        if(t == '.' || t == ' ' || t== '"'){
            nome.pop_back();
        }
        if(nome.front() == '"') nome.erase(nome.begin());        // Apaga aspas da primeira posição
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
string Retira_acentuacao(string nome){

    nome = Transforma_maisculo_minusculo(nome, false);

    // teste.insert(teste.begin()+posicao,237);
    const char *str = nome.c_str();
    char * pch, s;
    int posicao, contador = 0;
    while(contador < 12){
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
            case 11: pch = strstr (str,"ü"); s='u'; break;
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
    centro_Academico.push_back("CIÊNCIAS BIOLÓGICAS E DA SAÚDE");
    centro_Academico.push_back("LINGUÍSTICA, LETRAS, ARTES E CIÊNCIAS HUMANAS E SOCIAIS APLICADAS");
    centro_Academico.push_back("ENGENHARIAS E CIÊNCIAS AGRÁRIAS, EXATAS E DA TERRA");
    centro_Academico.push_back("PROGRAMAS DE PÓS-GRADUAÇÃO");
    retorno = "\\pagestyle{empty}\n\\chapter{" + centro_Academico.at(grande_area) + "}\n" +
              "\\begin{figure}[!h]\n\t\\centering\n\t\\includegraphics[scale=1.3]{imagens/sciences.png}\n"+
              "\\end{figure}\n\\vfill\n\\hrule\n\\pagebreak\n\\pagestyle{fancy}\n\n" + inputs;
    return retorno;
}
// Função para definir a string de escrita para a Escola
string Define_escola(string sigla){
    string retorno;
    if(sigla == "ECS") retorno = "Escola de Ciências da Saúde -- ECS";
    else if(sigla == "EMCT") retorno = "Escola do Mar, Ciência e Tecnologia -- EMCT";
    else if(sigla == "EACH") retorno = "Escola de Artes, Comunicação e Hospitalidade -- EACH";
    else if(sigla == "ECJS") retorno = "Escola de Ciências Jurídicas e Socias -- ECJS";
    else if(sigla == "EE")   retorno = "Escola de Educação -- EE";
    else if(sigla == "EN")   retorno = "Escola de Negócios -- EN";
    else if(sigla == "CAU")  retorno = "Colégio de Aplicação Univali - CAU";
    else retorno = "";
    return retorno;
}
/* Função para setar os resumos em cada um dos elementos do vetor
   Os arquivos lidos são os .tex gerados pelo script em python
   Nesta função, pega-se o atributo nome_completo_bolsista de cada elemento do vetor e tenta-se abrir o arquivo .tex
   com este nome no diretório informado. Caso a abertura do arquivo aconteça, o conteúdo do arquivo é setado como
   resumo daquele elemento.
*/
void Define_resumo(string diretorio, vector<Projeto>&vetor){

    string nome_padronizado, linha, resumo, input_sem_resumo="";
    ifstream arquivo;
    set<string> sem_resumo;
    ofstream arq_sem_resumo;
    set<string>nomes_utilizados;
    //static vector<string>nomes_utilizados;

    for(int i=0; i<vetor.size(); i++){                  // Percorre todos os projetos
        nome_padronizado =""; resumo = "";              // Reseta strings para próxima iteração

        // Formulário do Google
        //nome_padronizado = Sobrenome_arquivo(vetor.at(i).getBolsista_nome(), vetor.at(i).getBolsista_sobrenome());

        //ELIS
        nome_padronizado = vetor.at(i).getBolsista_nome_completo();
        nome_padronizado = Transforma_maisculo_minusculo(nome_padronizado,false);
        nome_padronizado = Retira_acentuacao(nome_padronizado);
        nome_padronizado = Retira_ponto_final_espaco(nome_padronizado,false);

        if(!nomes_utilizados.empty()){
            set<string>::iterator it = nomes_utilizados.begin();
            for(it; it!=nomes_utilizados.end();it++){
                if(*it == nome_padronizado){
                    nome_padronizado+="_2";
                    break;
                }
            }
        }
        nomes_utilizados.insert(nome_padronizado);  // Adiciona nome ao set

        nome_padronizado += ".tex";                             // Tipo do arquivo
        arquivo.open(diretorio+'/'+nome_padronizado);           // Abre arquivo

        //if(arquivo.is_open()) cout<<nome_padronizado<<endl;

        while(getline(arquivo, linha)){                         // Leitura de todas as linhas
            resumo+=linha;
        }// Após leitura de todas as linhas, resumo pode ser definido
        vetor.at(i).setResumo(resumo);
        arquivo.close();

        if(vetor.at(i).getResumo() == ""){
            sem_resumo.insert(nome_padronizado);
        }
    }
    set<string>::iterator it_sr;      // iterador para percorrer a estrutura set
    for(it_sr = sem_resumo.begin(); it_sr != sem_resumo.end(); it_sr++){
        input_sem_resumo+= *it_sr + "\n";
    }
    arq_sem_resumo.open("sem_resumo.txt");
    arq_sem_resumo << input_sem_resumo;
    arq_sem_resumo.close();
}
/* Função que gera o nome dos arquivos de saída no formato "sobrenome_nome.tex"
    Parâmetros: Nome completo e sobrenome do bolsista (atribustos da classe)
*/
string Sobrenome_arquivo(string nome, string sobrenome_c){

    vector<string> identifica_nome, v_sobrenome;
    string nome_atual, sobrenome, nome_completo="", retorno;
    identifica_nome = Explode(nome, ' ');              // Nomes separados
    v_sobrenome = Explode(sobrenome_c, ' ');                      // Sobrenomes separados

    while(!identifica_nome.empty()){
        nome_atual = identifica_nome.front();
        nome_atual = Retira_ponto_final_espaco(nome_atual, false);              // Retira espaços
        nome_atual = Retira_acentuacao(nome_atual);                             // Retira acentos (será usado como nome do arquivo)
        nome_atual = Transforma_maisculo_minusculo(nome_atual, false);          // Passa para minúsculo
        identifica_nome.erase(identifica_nome.begin());
        nome_completo+=nome_atual+"_";
    }

    nome_completo.pop_back();                                   // Retira último underline

    if(v_sobrenome.size()==1){                                  // Se houver um sobrenome
        sobrenome = v_sobrenome.at(0);
    }else{                                                      // Se houver 2 sobrenomes
        sobrenome = v_sobrenome.at(0) + "_" + v_sobrenome.at(1);
    }

    sobrenome = Retira_ponto_final_espaco(sobrenome, false);
    sobrenome = Retira_acentuacao(sobrenome);
    sobrenome = Transforma_maisculo_minusculo(sobrenome,false);

    retorno = sobrenome + "_" + nome_completo;

    return retorno;
}

/*  Função para criar um txt listando o nome de todos os bolsistas (Formatação : sobrenome_nome)
 *  Esse txt é necessário para utilizar o arquivo doc_r.py (Formulário do Google sendo utilizado)
 *  Com a utilização do sistema ELIS este arquivo é utilizado como controle da saída do sistema
*/
void Cria_lista_arquivos(vector<Projeto>vetor){
    // Cria txt com todos os nomes de arquivos que serão criados
    ofstream lista_arquivos;
    Projeto projeto_atual;
    set<string>nomes_bolsistas, bolsistas_sem_resumo;
    string input_escreve_arquivo, nome_padronizado, input_sem_resumo;
    set<string> nomes_utilizados;

    //static vector<string> nomes_utilizados;

    while(!vetor.empty()){
        projeto_atual = vetor.front();               // Pega primeira posição

        nome_padronizado = Sobrenome_arquivo(projeto_atual.getBolsista_nome(), projeto_atual.getBolsista_sobrenome());


        if(!nomes_utilizados.empty()){
            set<string>::iterator it;
            for(it= nomes_utilizados.begin(); it!=nomes_utilizados.end();it++){
                if(*it == nome_padronizado){
                    nome_padronizado+="_2";
                    break;
                }
            }
        }
        nomes_utilizados.insert(nome_padronizado);  // Adiciona nome ao set

        if(projeto_atual.getResumo()==""){
            bolsistas_sem_resumo.insert(nome_padronizado);
        }

        nomes_bolsistas.insert(nome_padronizado);

        //input_escreve_arquivo = Sobrenome_arquivo(projeto_atual.getBolsista_nome_completo(), projeto_atual.getBolsista_sobrenome());
        //lista_arquivos << input_escreve_arquivo + "\n";    // Escreve string no arquivo
        vetor.erase(vetor.begin());                  // Apaga primeira posição do vetor
    }

    set<string>::iterator it_n;      // iterador para percorrer a estrutura set
    for(it_n = nomes_bolsistas.begin(); it_n != nomes_bolsistas.end(); it_n++){
        input_escreve_arquivo+= *it_n + "\n";
    }
    // Escreve nome de todos os arquivos gerados
    lista_arquivos.open("lista_arquivos.txt");
    lista_arquivos << input_escreve_arquivo;
    lista_arquivos.close();

    set<string>::iterator it_s;      // iterador para percorrer a estrutura set
    for(it_s = bolsistas_sem_resumo.begin(); it_s != bolsistas_sem_resumo.end(); it_s++){
        input_sem_resumo+= *it_s + "\n";
    }
    // Escreve nome dos arquivos que não possuem resumo
    lista_arquivos.open("sem_resumo.txt");
    lista_arquivos << input_sem_resumo;
    lista_arquivos.close();

}
/* Função para leitura do CSV vindo do Formulário
 * Parâmetros -> (Local do arquivo, Vetor de projetos passado por referência)
*/
void Le_CSV_resumo(string arquivo, vector<Projeto>&vetor){

    string csvLinha, nome_bolsista_atual;
    fstream file(arquivo, ios::in);             // Abertura do arquivo
    int linha_controle = 0;                     // Variável para garantir que a linha de labels do CSV seja ignorada

    if(!file.is_open()){                        // Caso o arquivo a ser aberto não seja encontrado
            cout << "Arquivo c/ autores adicionais nao encontrado!\n";
            return;
         }

    while(getline(file, csvLinha)){
        if(linha_controle == 0){
            linha_controle++;                       // Pula primeira linha (Labels)
        }else{
            istringstream csvStream(csvLinha);      // Recebe uma linha completa do CSV
            vector<string> csvColuna;               // Vetor cujo elementos são as colunas da linha do CSV
            string csvElemento;                     // Elemento da Coluna

            while( getline(csvStream, csvElemento, ';') ){  // Separa itens na linha por ;,'
                csvColuna.push_back(csvElemento);           // Cada elemento do vetor csvColuna é um item diferente
            }
            // Nesse ponto os elementos do CSV estão separados nas posições do vetor csvColuna;

            /* NOVO FORMATO
               Coluna 0 -> Nome do bolsista
               Coluna 1 -> Autores adicionais separados por vírgula
            */

            nome_bolsista_atual = Retira_ponto_final_espaco(csvColuna.at(0), false);
            nome_bolsista_atual = Trata_maiusculo_minusculo(nome_bolsista_atual, false);

            for(unsigned int i=0; i<vetor.size();i++){
                if(vetor.at(i).getBolsista_nome_completo() == nome_bolsista_atual){
                    if(csvColuna.at(1) !="" && csvColuna.at(1) !="."){
                        vetor.at(i).setAutores_adicionais(Explode(csvColuna.at(1), ','));   // Vetor com os autores
                    }
                }
            }
        }
    }
    file.close();
    /*
    // Gera Arquivos txt separando alunos com apresentação oral e pôster
        ofstream arq_apresentacao_oral, arq_apresentacao_poster;
        set<string> apresentacao_oral, apresentacao_poster;
        string input_apresentacao_oral, input_apresentacao_poster;

        for(int i=0; i<vetor.size();i++){
            if(vetor.at(i).getModo_apresentacao() == "Oral"){
                apresentacao_oral.insert(vetor.at(i).getBolsista_nome_completo());
            }else if(vetor.at(i).getModo_apresentacao() == "Pôster"){
                apresentacao_poster.insert(vetor.at(i).getBolsista_nome_completo());
            }
        }

        arq_apresentacao_oral.open("Apresentacao_Oral.txt");
        set<string>::iterator it_o;      // iterador para percorrer a estrutura set
        for(it_o = apresentacao_oral.begin(); it_o != apresentacao_oral.end(); it_o++){
            input_apresentacao_oral+= *it_o + "\n";
        }
        arq_apresentacao_oral << input_apresentacao_oral;
        arq_apresentacao_oral.close();

        arq_apresentacao_poster.open("Apresentacao_Poster.txt");
        set<string>::iterator it_p;      // iterador para percorrer a estrutura set
        for(it_p = apresentacao_poster.begin(); it_p != apresentacao_poster.end(); it_p++){
            input_apresentacao_poster+= *it_p + "\n";
        }
        arq_apresentacao_poster << input_apresentacao_poster;
        arq_apresentacao_poster.close();*/
}

/*  Função que retorna a string de autores adicionais pronta para escrita no arquivo
 *  Parâmetro -> Vetor em que cada posição possui o nome completo de um autor adicional(string lida do csv, sem as aspas)
*/
string Trata_autores_adicionais(vector<string> vetor_autores, string orientador, string bolsista){

    if(vetor_autores.empty()){             // Caso não possua autores adicionais
        return "";
    }else{
       vector<string> nomes_autor_atual, aux;
       string nome, sobrenome, retorno="", primeiro_autor_adicional, nome_arq;

       while(!vetor_autores.empty()){

            primeiro_autor_adicional = vetor_autores.front();
            aux = Explode(primeiro_autor_adicional,' ');
            nome = aux.at(0);
            sobrenome = "";

            if(aux.size()>1){

                nomes_autor_atual = Trata_sobrenome(vetor_autores.front());       // Pega primeira posição
                nome = Retira_ponto_final_espaco(nomes_autor_atual.at(0),false);
                sobrenome = Retira_ponto_final_espaco(nomes_autor_atual.at(1),false);
                nome_arq = Sobrenome_arquivo(nome,sobrenome);
            }

            if(nome_arq == bolsista || nome_arq == orientador){
                retorno+="";
            }else
                retorno += sobrenome + ", " + nome + "; ";

            vetor_autores.erase(vetor_autores.begin());                       // Apaga primeira posição
        }
       return retorno;
    }
}

/* Função de separação dos caracteres
    Parâmetros(String a ser separada, caractere usado para separação)
    Retorno da função: Vetor em que cada posição corresponde a um elemento diferente
    (Elementos são definidos pelo caracter separadador passado por parâmetro)
*/
const vector<string> Explode(const string& s, const char& c){
    string buff{""};
    vector<string> v;

    for(auto n:s){
        if(n != c) buff+=n; else
        if(n == c && buff != "") { v.push_back(buff); buff = ""; }
    }
    if(buff != "") v.push_back(buff);
    return v;
}
