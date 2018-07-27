#ifndef MY_WINDOW_H
#define MY_WINDOW_H

#include <QMainWindow>
#include <vector>
#include "projeto.h"

namespace Ui {
class MyWindow;
}

class MyWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyWindow(QWidget *parent = 0);
    ~MyWindow();

    vector<Projeto> getVetor_projetos() const;
    void setVetor_projetos(const vector<Projeto> &value);

private slots:
    void on_pushButton_clicked();

    void on_browse_button_clicked();

    void on_file_generate_button_clicked();

    void on_browse_button_2_clicked();

    void on_browse_button_3_clicked();

    void on_browse_button_4_clicked();

private:
    Ui::MyWindow *ui;
    vector<Projeto> vetor_projetos;
};

#endif // MY_WINDOW_H
