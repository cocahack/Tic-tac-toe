#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_selectdialog.h"
#include <QtGui>
#include <QDebug>
#include <QSizeGrip>
#include <algorithm>
#include <QMessageBox>
#include <QTextCodec>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), game_start(false), turn(false)
{
    ui->setupUi(this);
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
    game_start = turn = true;
    set_path();
}

void MainWindow::on_secondbtn_clicked()
{
    game_start = true;
    turn = false;
    set_path();
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
        else cpu_turn();
    }
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
    int i,j;
    for(i=0;i<3;++i)
    {
        if((board[i][0]==1)&&(board[i][1]==1)&&(board[i][2]==1)) return 1;
        if((board[i][0]==2)&&(board[i][1]==2)&&(board[i][2]==2)) return 2;
    }
    for(j=0;j<3;++j)
        for(i=0;i<3;++i)
        {
            if((board[i][j]==1)&&(board[i][j]==1)&&(board[i][j]==1)) return 1;
            if((board[i][j]==2)&&(board[i][j]==2)&&(board[i][j]==2)) return 2;
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
    Move bestMove = findBestMove();
    qDebug()<<"Best point, i: "<<bestMove.row<<" j: "<<bestMove.col;
    int x = 80+bestMove.col*120;
    int y = 170+bestMove.row*120;
    set_board(x,y);
}

void MainWindow::game_over(int score)
{
    QString str;

//    if(score==10) str = "플레이어가 승리했습니다.";
//    if(score==-10) str= "컴퓨터가 승리했습니다.";
//    if(score==0) str = "무승부.";
    if(score==10) str = QString::fromLocal8Bit("컴퓨터가 승리했습니다.");
    if(score==-10) str = QString::fromUtf8("컴퓨터가 승리했습니다.");
    if(score==0) str = QString::fromUtf8("무승부.");

    qDebug()<<str;
}


