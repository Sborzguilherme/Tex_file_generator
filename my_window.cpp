#include "my_window.h"
#include "ui_mywindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>
#include "recebe_dados.cpp"

using namespace std;

MyWindow::MyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyWindow)
{
    ui->setupUi(this);
    ui->label_end->setVisible(false);
}

MyWindow::~MyWindow()
{
    delete ui;
}
// Função para seleção do arquivo de leitura
void MyWindow::on_browse_button_clicked()   // Botão browse de leitura
{
    static vector<Projeto>vetor_projetos;

    // Só permite a leitura de arquivos .csv
    QString arquivo_leitura = QFileDialog::getOpenFileName(this, tr("Arquivo de Leitura"), ui->file_line->text(), tr("CSV (*.csv)"));
    // O caminho lido é salvo na variável arquivo_leitura
    if(!arquivo_leitura.isEmpty()){
        ui->file_line->setText(arquivo_leitura); // Coloca o caminho do arquivo de leitura no edit line
    }
    ui->label_end->setVisible(false);

    le_projetos(ui->file_line->text().toStdString(), vetor_projetos);   // Preenche vetor de projetos a partir do arquivo CSV
    Cria_lista_arquivos(vetor_projetos);                                // Nomes dos arquivos que serão abertos pelo script em python

    /* Script de leitura dos resumos, desenvolvido em python, é chamado a partir do terminal
        Comando para execução do arquivo é uma string de 3 partes

        Parte 1 -> "python" -> chamada da variável do sistema para execução do interpretador
        (deve-se utilizar um que possua os módulos necessários ("python-docx" e "document"))

        Parte 2 -> Caminho do script python. Atualmente na pasta de projetos do Pycharm. Deve ser alterada para a pasta
        deste projeto.

        Parte 3 -> Parâmetro que indica de onde os resumos serão lidos
    */

    //string command = "python C:\\Users\\Guilherme\\PycharmProjects\\DOC_READ\\doc_r.py C:\\Users\\Guilherme\\Documents\\Univali\\CSV_ARQUIVOS\\Resumos";
    string local_resumo = ui->file_line_3->text().toStdString().c_str();    // Diretório informado pelo usuário
    string command = "venv\\Scripts\\python doc_r.py";
    command+=(" " + local_resumo);                                          // Adiciona Parte 3 do comando
    //system(command.c_str());
    WinExec(command.c_str(), SW_HIDE);  // Invoca o "cmd" sem que ele apareça

    Define_Resumo(local_resumo+"\\tex", vetor_projetos);

    this->setVetor_projetos(vetor_projetos);

}
void MyWindow::on_browse_button_2_clicked() // Botão browse de escrita
{

    // Retornar o diretório de escrita
    QString diretorio_escrita = QFileDialog::getExistingDirectory(this);

    if(!diretorio_escrita.isEmpty()){
        ui->file_line_2->setText(diretorio_escrita);
    }
}

vector<Projeto> MyWindow::getVetor_projetos() const
{
    return vetor_projetos;
}

void MyWindow::setVetor_projetos(const vector<Projeto> &value)
{
    vetor_projetos = value;
}

void MyWindow::on_file_generate_button_clicked()
{
    // Pega os caminhos selecionados acima
    string arquivo_leitura = ui->file_line->text().toStdString();
    string diretorio_escrita = ui->file_line_2->text().toStdString();

    string cria_dir_vida, cria_dir_hum, cria_dir_exa;
    const char *dir_vida, *dir_humanas, *dir_exatas;

    cria_dir_vida = diretorio_escrita + "/vida";
    cria_dir_hum = diretorio_escrita + "/humanas";
    cria_dir_exa = diretorio_escrita + "/exatas";
    //vector <Projeto> vetor_projetos;                                   // Vetor do tipo da classe "Projeto"

    dir_vida = cria_dir_vida.c_str();
    dir_humanas = cria_dir_hum.c_str();
    dir_exatas = cria_dir_exa.c_str();

    // Criação dos diretórios para armazenar os arquivos gerados
    mkdir(dir_vida); mkdir(dir_humanas); mkdir(dir_exatas);
    escreve_projetos(this->getVetor_projetos(), diretorio_escrita);                // Escreve os dados do vetor em arquivos .TEX
    ui->label_end->setVisible(true);
}

void MyWindow::on_browse_button_3_clicked()   // Boão browse para diretório dos resumos
{
    QString diretorio_escrita = QFileDialog::getExistingDirectory(this);

    if(!diretorio_escrita.isEmpty()){
        ui->file_line_3->setText(diretorio_escrita);
    }

    mkdir((ui->file_line_3->text().toStdString()+"/tex").c_str());  // Cria pasta para inserção dos resumos em .TEX


}
