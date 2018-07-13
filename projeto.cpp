#include "projeto.h"

using namespace std;

string Projeto::getCodigo() const
{
    return codigo;
}

void Projeto::setCodigo(const string &value)
{
    codigo = value;
}

string Projeto::getBolsista_nome_completo() const
{
    return bolsista_nome_completo;
}

void Projeto::setBolsista_nome_completo(const string &value)
{
    bolsista_nome_completo = value;
}

string Projeto::getBolsista_nome() const
{
    return bolsista_nome;
}

void Projeto::setBolsista_nome(const string &value)
{
    bolsista_nome = value;
}

string Projeto::getBolsista_sobrenome() const
{
    return bolsista_sobrenome;
}

void Projeto::setBolsista_sobrenome(const string &value)
{
    bolsista_sobrenome = value;
}

string Projeto::getOrientador_nome_completo() const
{
    return orientador_nome_completo;
}

void Projeto::setOrientador_nome_completo(const string &value)
{
    orientador_nome_completo = value;
}

string Projeto::getOrientador_nome() const
{
    return orientador_nome;
}

void Projeto::setOrientador_nome(const string &value)
{
    orientador_nome = value;
}

string Projeto::getOrientador_sobrenome() const
{
    return orientador_sobrenome;
}

void Projeto::setOrientador_sobrenome(const string &value)
{
    orientador_sobrenome = value;
}

string Projeto::getCurso() const
{
    return curso;
}

void Projeto::setCurso(const string &value)
{
    curso = value;
}

string Projeto::getEscola() const
{
    return escola;
}

void Projeto::setEscola(const string &value)
{
    escola = value;
}

string Projeto::getArea() const
{
    return area;
}

void Projeto::setArea(const string &value)
{
    area = value;
}

string Projeto::getTitulo() const
{
    return titulo;
}

void Projeto::setTitulo(const string &value)
{
    titulo = value;
}

string Projeto::getPalavra_chave() const
{
    return palavra_chave;
}

void Projeto::setPalavra_chave(const string &value)
{
    palavra_chave = value;
}

string Projeto::getArea_apresentacao() const
{
    return area_apresentacao;
}

void Projeto::setArea_apresentacao(const string &value)
{
    area_apresentacao = value;
}

string Projeto::getTitulo_Projeto() const
{
    return titulo_Projeto;
}

void Projeto::setTitulo_Projeto(const string &value)
{
    titulo_Projeto = value;
}

string Projeto::getResumo() const
{
    return Resumo;
}

void Projeto::setResumo(const string &value)
{
    Resumo = value;
}

Projeto::Projeto()
{

}
