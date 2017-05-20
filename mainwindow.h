#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>

#define PADDING 100
#define LENGTH 120
#define PLAYER 1
#define CPU 2

namespace Ui {
class MainWindow;
class SelectDialog;
}

struct Move
{
int row,col;
};
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void init_game();
    void set_path(){ if(turn) imgPath=":/images/o.png"; else imgPath=":/images/x.png";}
    QString get_path(){return imgPath;}
    void set_board(int x, int y);

    int scoring();
    int whos_win();
    int minimax(int depth, bool isMax);
    bool isMovesLeft();
    Move findBestMove();
    void cpu_turn();
    void game_over(int score);
    void label_clear();
    void random_select();
protected:
    void mousePressEvent(QMouseEvent* event);
private slots:
    void on_firstbtn_clicked();
    void on_secondbtn_clicked();

    void on_newbtn_clicked();

private:
    Ui::MainWindow *ui;
    bool game_start;
    int board[3][3]; // 0 is nothing. 1 is player's choice, 2 is CPU's choice
    bool is_fill[3][3]; // was 3x3 game board filled with image?
    bool turn; // true - player's turn, false - cpu's turn
    QString imgPath;
    int score;
};


class SelectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectDialog(QDialog* parent=0);
    virtual ~SelectDialog();

private:
    Ui::SelectDialog* ui;

};



#endif // MAINWINDOW_H
