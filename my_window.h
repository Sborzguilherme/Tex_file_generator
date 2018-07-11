#ifndef MY_WINDOW_H
#define MY_WINDOW_H

#include <QMainWindow>

namespace Ui {
class MyWindow;
}

class MyWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyWindow(QWidget *parent = 0);
    ~MyWindow();

private slots:
    void on_pushButton_clicked();

    void on_browse_button_clicked();

    void on_file_generate_button_clicked();

    void on_browse_button_2_clicked();

private:
    Ui::MyWindow *ui;

};

#endif // MY_WINDOW_H
