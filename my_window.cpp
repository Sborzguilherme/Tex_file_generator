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
    // Só permite a leitura de arquivos .csv
    QString arquivo_leitura = QFileDialog::getOpenFileName(this, tr("Arquivo de Leitura"), ui->file_line->text(), tr("CSV (*.csv)"));
    // O caminho lido é salvo na variável arquivo_leitura
    if(!arquivo_leitura.isEmpty()){
        ui->file_line->setText(arquivo_leitura); // Coloca o caminho do arquivo de leitura no edit line
    }
    ui->label_end->setVisible(false);

}
void MyWindow::on_browse_button_2_clicked() // Botão browse de escrita
{
    // Retornar o diretório de escrita
    QString diretorio_escrita = QFileDialog::getExistingDirectory(this);

    if(!diretorio_escrita.isEmpty()){
        ui->file_line_2->setText(diretorio_escrita);
    }
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
    vector <Projeto> vetor_projetos;                                   // Vetor do tipo da classe "Projeto"

    dir_vida = cria_dir_vida.c_str();
    dir_humanas = cria_dir_hum.c_str();
    dir_exatas = cria_dir_exa.c_str();

    // Criação dos diretórios para armazenar os arquivos gerados
    mkdir(dir_vida); mkdir(dir_humanas); mkdir(dir_exatas);

    le_projetos(arquivo_leitura, vetor_projetos);                       // Preenche vetor de projetos a partir do arquivo CSV
    escreve_projetos(vetor_projetos, diretorio_escrita);                // Escreve os dados do vetor em arquivos .TEX
    ui->label_end->setVisible(true);
}
