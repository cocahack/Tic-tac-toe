#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_selectdialog.h"
#include <QtGui>
#include <QDebug>
#include <QSizeGrip>
#include <algorithm>
#include <QMessageBox>
#include <QTextCodec>
#include <random>
#include <chrono>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), game_start(false), turn(false)
{
    ui->setupUi(this);
    ui->newbtn->setEnabled(false);
    init_game();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init_game()
{
    for(int i=0;i<3;++i)
        for(int j=0;j<3;++j)
        {
            board[i][j]=0;
            is_fill[i][j]=false;
        }
    game_start = turn = false;
    label_clear();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    int x, y;
    if(event->type()==QEvent::MouseButtonPress&&game_start&&turn)
    {
        x = event->localPos().x();
        y = event->localPos().y();
        qDebug()<<"x: "<<x<<" y: "<<y;
        set_board(x,y);

    }
}


SelectDialog::SelectDialog(QDialog *parent):
    QDialog(parent), ui(new Ui::SelectDialog)
{
    ui->setupUi(this);
}

SelectDialog::~SelectDialog()
{
    delete ui;
}


void MainWindow::on_firstbtn_clicked()
{
    ui->firstbtn->setEnabled(false);
    ui->secondbtn->setEnabled(false);
    ui->newbtn->setEnabled(true);
    ui->game_state->setText(QString::fromLocal8Bit("플레이어의 차례입니다."));
    game_start = turn = true;
    set_path();
}

void MainWindow::on_secondbtn_clicked()
{
    ui->firstbtn->setEnabled(false);
    ui->secondbtn->setEnabled(false);
    ui->newbtn->setEnabled(true);
    game_start = true;
    turn = false;
    set_path();
    random_select();
}

void MainWindow::set_board(int x, int y)
{
    QPixmap qpix;
    int who;
    if(turn){qpix.load(":images/o.png");who=PLAYER;}
    else {qpix.load(":images/x.png"); who=CPU;}
    if(x>=20&&x<140&&y>=110&&y<230&&!is_fill[0][0])
    {
        ui->label00->setPixmap(qpix);
        board[0][0] = who;
        is_fill[0][0] = true;
        turn ^= 1;
    }
    if(x>=140&&x<260&&y>=110&&y<230&&!is_fill[0][1])
    {
        ui->label01->setPixmap(qpix);
        board[0][1] = who;
        is_fill[0][1] = true;
        turn ^= 1;
    }
    if(x>=260&&x<=380&&y>=110&&y<230&&!is_fill[0][2])
    {
        ui->label02->setPixmap(qpix);
        board[0][2] = who;
        is_fill[0][2] = true;
        turn ^= 1;
    }
    if(x>=20&&x<140&&y>=230&&y<350&&!is_fill[1][0])
    {
        ui->label10->setPixmap(qpix);
        board[1][0] = who;
        is_fill[1][0] = true;
        turn ^= 1;
    }
    if(x>=140&&x<260&&y>=230&&y<350&&!is_fill[1][1])
    {
        ui->label11->setPixmap(qpix);
        board[1][1] = who;
        is_fill[1][1] = true;
        turn ^= 1;
    }
    if(x>=260&&x<=380&&y>230&&y<350&&!is_fill[1][2])
    {
        ui->label12->setPixmap(qpix);
        board[1][2] = who;
        is_fill[1][2] = true;
        turn ^= 1;
    }
    if(x>=20&&x<140&&y>=350&&y<=470&&!is_fill[2][0])
    {
        ui->label20->setPixmap(qpix);
        board[2][0] = who;
        is_fill[2][0] = true;
        turn ^= 1;
    }
    if(x>=140&&x<260&&y>=350&&y<=470&&!is_fill[2][1])
    {
        ui->label21->setPixmap(qpix);
        board[2][1] = who;
        is_fill[2][1] = true;
        turn ^= 1;
    }
    if(x>=260&&x<=380&&y>=350&&y<=470&&!is_fill[2][2])
    {
        ui->label22->setPixmap(qpix);
        board[2][2] = who;
        is_fill[2][2] = true;
        turn ^= 1;
    }
    int score = scoring();

    if(turn==false)
    {
        if(score!=0||!isMovesLeft()) game_over(score);
        else
        {
            cpu_turn();
            score = scoring();
            if(score!=0||!isMovesLeft()) game_over(score);
        }
    }
    else ui->game_state->setText(QString::fromLocal8Bit("플레이어의 차례입니다."));
}

int MainWindow::scoring()
{
    int win = whos_win();
    if(win==1) return 10;
    else if(win==2) return -10;
    else return 0;
}

// 1 is player win , 2 is cpu win
int MainWindow::whos_win()
{
    int i;
    for(i=0;i<3;++i)
    {
        if((board[i][0]==1)&&(board[i][1]==1)&&(board[i][2]==1)) return 1;
        if((board[i][0]==2)&&(board[i][1]==2)&&(board[i][2]==2)) return 2;
    }
    for(i=0;i<3;++i)
    {
        if((board[0][i]==1)&&(board[1][i]==1)&&(board[2][i]==1)) return 1;
        if((board[0][i]==2)&&(board[1][i]==2)&&(board[2][i]==2)) return 2;
    }

    if((board[0][0]==1)&&(board[1][1]==1)&&(board[2][2]==1)) return 1;
    if((board[0][0]==2)&&(board[1][1]==2)&&(board[2][2]==2)) return 2;

    if((board[0][2]==1)&&(board[1][1]==1)&&(board[2][0]==1)) return 1;
    if((board[0][2]==2)&&(board[1][1]==2)&&(board[2][0]==2)) return 2;

    return 0;
}


int MainWindow::minimax(int depth, bool isMax)
{
    int score = scoring();
    if(score!=0) return score;
    if(!isMovesLeft()) return score;

    if(isMax)
    {
        int best = -1000;
        for(int i=0;i<3;++i)
        {
            for(int j=0;j<3;++j)
            {
                if(board[i][j]==0)
                {
                    board[i][j]=PLAYER;
                    best = std::max(best,minimax(depth+1, !isMax));
                    board[i][j]=0;
                }
            }
        }
        return best;
    }
    else
    {
        int best = 1000;
        for(int i=0;i<3;++i)
        {
            for(int j=0;j<3;++j)
            {
                if(board[i][j]==0)
                {
                    board[i][j]=CPU;
                    best = std::min(best,minimax(depth+1, !isMax));
                    board[i][j]=0;
                }
            }
        }
        return best;
    }
}

bool MainWindow::isMovesLeft()
{
    int i,j;
    for(i=0;i<3;++i)
        for(j=0;j<3;++j)
            if(board[i][j]==0) return true; // game is not over;
    return false;
}

Move MainWindow::findBestMove()
{
    int bestVal = 1000;
    Move bestMove;
    bestMove.row = bestMove.col = -1;

    for (int i = 0; i<3; i++)
    {
        for (int j = 0; j<3; j++)
        {
            if (board[i][j]==0)
            {
                board[i][j]=CPU;
                int moveVal = minimax(0,true);
                board[i][j]=0;
                if (moveVal < bestVal)
                {
                    bestMove.row = i;
                    bestMove.col = j;
                    bestVal = moveVal;
                }
            }
        }
    }
    return bestMove;
}

void MainWindow::cpu_turn()
{
    ui->game_state->setText(QString::fromLocal8Bit("AI의 차례입니다."));
//    auto start = std::chrono::system_clock::now();
//    while(1)
//    {
//        auto now = std::chrono::system_clock::now();
//        auto dms = std::chrono::duration_cast<std::chrono::seconds>(now-start).count();
//        if(dms>=1) break;
//    }
    Move bestMove = findBestMove();
    qDebug()<<"Best point, i: "<<bestMove.row<<" j: "<<bestMove.col;
    int x = 80+bestMove.col*120;
    int y = 170+bestMove.row*120;
    set_board(x,y);
}

void MainWindow::game_over(int score)
{
    QString str;

//    if(score==10) str = "?÷???? ?¸???????.";
//    if(score==-10) str= "?????? ?¸???????.";
//    if(score==0) str = "???º?.";
    if(score==10) str = QString::fromLocal8Bit("플레이어가 이겼습니다.");
    if(score==-10) str = QString::fromLocal8Bit("AI가 이겼습니다.");
    if(score==0) str = QString::fromLocal8Bit("무승부입니다.");
    game_start = false;
    ui->game_state->setText(str);
    qDebug()<<str;
}

void MainWindow::label_clear()
{
    ui->label00->clear();
    ui->label01->clear();
    ui->label02->clear();
    ui->label10->clear();
    ui->label11->clear();
    ui->label12->clear();
    ui->label20->clear();
    ui->label21->clear();
    ui->label22->clear();
    ui->game_state->setText(QString::fromLocal8Bit("차례를 선택하세요."));
}

void MainWindow::random_select()
{
    std::random_device rn;
    std::mt19937 rnd(rn());
    std::uniform_int_distribution<int> range{ 0, 8 };
    int r =range(rnd);
    int i = r/3; int j = r%3;
    int x = 80+j*120;
    int y = 170+i*120;
    set_board(x,y);
}

void MainWindow::on_newbtn_clicked()
{
    ui->firstbtn->setEnabled(true);
    ui->secondbtn->setEnabled(true);
    init_game();
}
