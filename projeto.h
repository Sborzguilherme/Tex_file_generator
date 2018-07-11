#ifndef PROJETO_H
#define PROJETO_H
#include <iostream>
#include <vector>
using namespace std;

class Projeto
{

private:                    // Atributos da classe

    string titulo_Projeto;
    string codigo;

    string bolsista_nome_completo;
    string bolsista_nome;
    string bolsista_sobrenome;

    string orientador_nome_completo;
    string orientador_nome;
    string orientador_sobrenome;

    string curso;
    string escola;
    string area;
    string titulo;

    string palavra_chave;
    string area_apresentacao;

public:
    Projeto();
    string getCodigo() const;
    void setCodigo(const string &value);
    string getBolsista_nome_completo() const;
    void setBolsista_nome_completo(const string &value);
    string getBolsista_nome() const;
    void setBolsista_nome(const string &value);
    string getBolsista_sobrenome() const;
    void setBolsista_sobrenome(const string &value);
    string getOrientador_nome_completo() const;
    void setOrientador_nome_completo(const string &value);
    string getOrientador_nome() const;
    void setOrientador_nome(const string &value);
    string getOrientador_sobrenome() const;
    void setOrientador_sobrenome(const string &value);
    string getCurso() const;
    void setCurso(const string &value);
    string getEscola() const;
    void setEscola(const string &value);
    string getArea() const;
    void setArea(const string &value);
    string getTitulo() const;
    void setTitulo(const string &value);
    string getPalavra_chave() const;
    void setPalavra_chave(const string &value);
    string getArea_apresentacao() const;
    void setArea_apresentacao(const string &value);
    string getTitulo_Projeto() const;
    void setTitulo_Projeto(const string &value);
};

#endif // PROJETO_H
