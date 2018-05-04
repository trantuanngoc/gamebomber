#include <iostream>
#include"console.h"
#include<windows.h>
#include<conio.h>
#include<string.h>
#include<time.h>
#include<ctime>
#include<cstdlib>

using namespace std;


struct Adress
{
    int x,y;
    int x0,y0;
};


    bool endGame ;                      //kết thúc game
    bool winGame;                       //thắng cuộc
    int tickSpeed;                      //tốc độ game
    clock_t startGame;                   //thời điểm bắt đầu game
    clock_t start;                    //thời điểm bắt đầu hẹn giờ cho bomb
    int boardWidth;                  //chiều rộng màn hình
    int boardHeight;                    //chiều cao màn hình
    Adress player;
    Adress direction;                     //hướng di chuyển người chơi
    Adress bomb;
    bool monsterDeath[5];                  //trạng thái sống hay chết của quái vật
    Adress monster[5];
    Adress directionMonster[5];     //hướng di chuyển của quái vật
    bool check;                        //trạng thái khởi động bomb hay chưa
    int numberMonster;     //số quái vật
    int numberMonsterLive ; //số quái vật còn sống
    char Map[20][52]={
        "  #     #   #         #     #     #     #     #   ",
        "# # ### # ### # ### # ##### ##### # ##### ### # # ",
        "# # # # #     #   # #   # #     #         # #   # ",
        "# # # # # ######### ### # ## ## ##### # ### ##### ",
        "# # # # #   # #   # # # #     #       # #   #     ",
        "# # # # # # # # # # # # ##### ####### # # ### ####",
        "#   # #   # #   # #   #           # # #     # #   ",
        "### # # ### ##### ############### # # ####### # # ",
        "#   #   #   #   #       #   #     #         #   # ",
        "# # # ####### # ####### # # # # # ### ##### ##### ",
        "# #   # #     #   #       #   # # #     #         ",
        "# # # # # ### ##### ##### ##### # # ### # ########",
        "# # #     #         #     #     #   #   # #       ",
        "# # ####### ####### # ####### ####### ### ### ### ",
        "# # #             # # #           #     #   #   # ",
        "# ### ####### ### # # # ######### # ####### ### ##",
        "#     #       #   #   # #       # # #         #   ",
        "### ### ############### # ##### ### # ######### # ",
        "#   #                     #         #           # "

    };



void init()
{
    //khởi tạo 1 số giá trị ban đầu
    startGame=clock();
     endGame =false;
     winGame=false;
     tickSpeed=70;               //tốc độ game
     boardWidth=50;              //chiều rộng map
     boardHeight=19;             //chiều cao map
     check=false;
     numberMonster=5;
     numberMonsterLive=5;


    //khởi tạo hướng di chuyển người chơi
   direction.x=0;direction.y=0;



     //khởi tạo màn hình thông tin
     SetBGColor(3);
     SetColor(15);
    for(int i=boardWidth;i<boardWidth+15;i++)
    {
        for(int j=0;j<boardHeight;j++)
        {
            gotoxy(i,j);
            cout<<ends;
        }
    }
        gotoxy(boardWidth+1,0);
        cout<<"Monsters:"<<ends<<numberMonster;
        gotoxy(boardWidth+1,2);
        cout<<"Time: ";
        gotoxy(boardWidth+1,8);
        cout<<"Creater :";
        gotoxy(boardWidth+1,9);
        SetColor(29);
        cout<<"Tran Tuan Ngoc";
        gotoxy(boardWidth+1,11);
        cout<<"Class: K62-IE3";
         gotoxy(3,6);

    gotoxy(3,boardHeight+2);
    SetColor(11);
    cout<<"Your character: ";
    SetColor(12);
    cout<<static_cast<char>(1);
    gotoxy(3,boardHeight+3);
    SetColor(11);
    cout<<"The monster: ";
    SetColor(14);
    cout<<static_cast<char>(2);
    gotoxy(3,boardHeight+4);
    SetColor(11);
    cout<<"The bomb: "<<static_cast<char>(79);
    gotoxy(3,boardHeight+5);
    cout<<"Press 'S' to put the bomb";
    gotoxy(3,boardHeight+6);
    cout<<"Press 'UP' 'DOWN' 'LEFT' 'RIGHT' to go";




    //cài màu map và tạo map
    SetColor(2);
    SetBGColor(7);                  //màu map
    for(int i=0;i<boardWidth;i++)
    {
        for (int j=0;j<boardHeight;j++)
            {
                gotoxy(i,j);
                cout<<ends;
            }
    }
    gotoxy(0,0);
    for (int i=0;i<19;i++)
    cout<<Map[i]<<endl;
    gotoxy(boardWidth,boardHeight-1);
    cout<<"      >>exit   ";


    //khởi tạo quái vật
    srand(time(NULL));
    for(int i=0;i<numberMonster;i++)
    {
    monsterDeath[i]=false;      //trang thái quái vật
    SetColor(14);         //màu quái vật
    //khởi tạo vị trí ngẫu nhiên của monster
    step:{
    monster[i].x=rand()%(boardWidth-5)+5;
    monster[i].y=rand()%(boardHeight-1);
    }
    //nếu khởi tạo trúng chướng ngại vật thì khởi tạo lại
    if(Map[monster[i].y][monster[i].x]=='#')  goto  step;

    monster[i].x0=monster[i].x;
    monster[i].y0=monster[i].y;
    //khởi tạo hướng đi của monster
    switch (rand()%4)
    {
    case 0:
        directionMonster[i].x=1;
        directionMonster[i].y=0;
        break;
    case 1:
        directionMonster[i].x=-1;
        directionMonster[i].y=0;
        break;
    case 2:
        directionMonster[i].x=0;
        directionMonster[i].y=1;
        break;
    case 3:
        directionMonster[i].x=0;
        directionMonster[i].y=-1;
        break;
    }
    gotoxy(monster[i].x,monster[i].y);
    cout<<static_cast<char>(2);

    }


    //khởi tạo  nhân vật
    player={0,0,0,0};
    SetColor(12);               //màu sắc nhân vật
    gotoxy(0,0);
    cout<<static_cast<char>(1);



}



void drawPlayer()
{
    //cài màu nhân vật
  SetColor(12);

  //vẽ nhân vật
   if((player.x!=player.x0||player.y!=player.y0))
    {
        gotoxy(player.x,player.y);
        cout<<static_cast<char>(1);
        gotoxy(player.x0,player.y0);
        cout<<ends;
    }
}

void drawBomb()
{
    //cài màu bomb
    SetColor(11);

    //vẽ bomb khi bomb đã khởi động và nhân vật ko còn đứng đó,tránh trường hợp bomb vẽ đè lên nhân vật
    if(check==true&&(player.x!=bomb.x||player.y!=bomb.y))
        {
            gotoxy(bomb.x,bomb.y);
            cout<<static_cast<char>(79);

        }

    //kích hoạt bomb nổ sau 3 giây
    if((double)(clock()-start)/CLOCKS_PER_SEC>=3&&check==true)
    {
        check=false;
        for(int j=0;j<5;j++)
        {
            if(Map[bomb.y][bomb.x+j]=='#')  break;
             if(bomb.x+j>=boardWidth)      break;
            gotoxy(bomb.x+j,bomb.y);
            cout<<static_cast<char>(219);
            if((player.x==bomb.x+j)&&(player.y==bomb.y))
                endGame=true;
            for(int i=0;i<numberMonster;i++)
            {
                if((monster[i].x==bomb.x+j)&&(monster[i].y==bomb.y))
                {
                    monsterDeath[i]=true;
                    monster[i].x=0;
                    monster[i].y=0;
                  numberMonsterLive--;

                }
            }
        }
         for(int j=0;j<5;j++)
        {

            if(Map[bomb.y+j][bomb.x]=='#')  break;
            if(bomb.y+j>=boardHeight)  break;
             gotoxy(bomb.x,bomb.y+j);
            cout<<static_cast<char>(219);
            if((player.y==bomb.y+j)&&(player.x==bomb.x))
                endGame=true;
           for(int i=0;i<numberMonster;i++)
           {
                if((monster[i].y==bomb.y+j)&&(monster[i].x==bomb.x))
                 {
                    monsterDeath[i]=true;
                    monster[i].x=0;
                    monster[i].y=0;
                     numberMonsterLive--;
                }
           }
        }
        for(int j=0;j>-5;j--)
        {
            if(Map[bomb.y][bomb.x+j]=='#')  break;
            if(bomb.x+j<0)  break;
            gotoxy(bomb.x+j,bomb.y);
            cout<<static_cast<char>(219);
            if((player.x==bomb.x+j)&&(player.y==bomb.y))
                endGame=true;
            for(int i=0;i<numberMonster;i++)
            {
                if((monster[i].x==bomb.x+j)&&(monster[i].y==bomb.y))
                  {
                    monsterDeath[i]=true;
                    monster[i].x=0;
                    monster[i].y=0;
                     numberMonsterLive--;
                }
            }

        }
        for(int j=0;j>-5;j--)
        {
            if(Map[bomb.y+j][bomb.x]=='#')  break;
            if(bomb.y+j<0)      break;
             gotoxy(bomb.x,bomb.y+j);
            cout<<static_cast<char>(219);
            if((player.y==bomb.y+j)&&(player.x==bomb.x))
                endGame=true;
           for(int i=0;i<numberMonster;i++)
           {
                if((monster[i].y==bomb.y+j)&&(monster[i].x==bomb.x))
                 {
                    monsterDeath[i]=true;
                    monster[i].x=0;
                    monster[i].y=0;
                    numberMonsterLive--;
                }
           }

        }


        //        xóa bomb và dấu tích bomb
         Sleep(100);
        for(int i=0;i<5;i++)
        {
            if(Map[bomb.y][bomb.x+i]=='#')  break;
            if(bomb.x+i>=boardWidth)      break;
            gotoxy(bomb.x+i,bomb.y);
            cout<<ends;
        }
         for(int i=0;i<5;i++)
        {

            if(Map[bomb.y+i][bomb.x]=='#')  break;
             if(bomb.y+i>=boardHeight)  break;
             gotoxy(bomb.x,bomb.y+i);
            cout<<ends;

        }
        for(int i=0;i>-5;i--)
        {
            if(Map[bomb.y][bomb.x+i]=='#')  break;
            if(bomb.x+i<0)  break;
            gotoxy(bomb.x+i,bomb.y);
            cout<<ends;

        }
        for(int i=0;i>-5;i--)
        {
            if(Map[bomb.y+i][bomb.x]=='#')  break;
            if(bomb.y+i<0)      break;
             gotoxy(bomb.x,bomb.y+i);
            cout<<ends;

        }
    }

}


void drawMonster(int i)
{
     //cài màu quái vật
      SetColor(14);

      //vẽ quái vật
      if(monster[i].x0!=monster[i].x||monster[i].y0!=monster[i].y)
      {
          gotoxy(monster[i].x,monster[i].y);
          cout<<static_cast<char>(2);
          gotoxy(monster[i].x0,monster[i].y0);
          cout<<ends;
      }


}



void moveMonster(int i)
{
    //khởi tạo hướng di chuyển ngẫu nhiên của monster
     switch (rand()%4)
    {
    case 0:
        directionMonster[i].x=1;
        directionMonster[i].y=0;
        break;
    case 1:
        directionMonster[i].x=-1;
        directionMonster[i].y=0;
        break;
    case 2:
        directionMonster[i].x=0;
        directionMonster[i].y=1;
        break;
    case 3:
        directionMonster[i].x=0;
        directionMonster[i].y=-1;
        break;
    }
    //sự di chuyển của monster
    monster[i].x0=monster[i].x;
    monster[i].y0=monster[i].y;
    monster[i].x+=directionMonster[i].x;
    monster[i].y+=directionMonster[i].y;


   //khi quát vật gặp tường thì sẽ quay ngược lại
    if(monster[i].x>=boardWidth)
    {
        monster[i].x=monster[i].x0;
        directionMonster[i].x=-1;
        directionMonster[i].y=0;
    }
    if(monster[i].x<0)
    {
         monster[i].x=monster[i].x0;
         directionMonster[i].x=1;
         directionMonster[i].y=0;
    }
    if(monster[i].y>=boardHeight)
    {
        monster[i].y=monster[i].y0;
        directionMonster[i].x=0;
        directionMonster[i].y=-1;
    }
    if(monster[i].y<0)
    {
        monster[i].y=monster[i].y0;
        directionMonster[i].x=0;
        directionMonster[i].y=1;
    }


     //khi gặp chướng ngại vật quái vật sẽ quay lại
    if(Map[monster[i].y][monster[i].x]=='#'||Map[monster[i].y][monster[i].x]=='0')
    {
        monster[i].x=monster[i].x0;
        monster[i].y=monster[i].y0;
        if(directionMonster[i].x==0&&directionMonster[i].y==1)
            {
                directionMonster[i].x=0;
                directionMonster[i].y=-1;
            }
        else if(directionMonster[i].x==0&&directionMonster[i].y==-1)
             {
                directionMonster[i].x=0;
                directionMonster[i].y=1;
            }
        else if(directionMonster[i].x==1&&directionMonster[i].y==0)
             {
                directionMonster[i].x=-1;
                directionMonster[i].y=0;
            }
        else if(directionMonster[i].x==-1&&directionMonster[i].y==0)
            {
                directionMonster[i].x=1;
                directionMonster[i].y=0;
            }

    }

}


void movePlayer()
{
    //sự di chuyển của người chơi
    player.x0 = player.x; player.y0 = player.y;
    player.x += direction.x; player.y += direction.y;

  //không cho nhân vật đi ra khỏi tường
    if(player.x>=boardWidth)
        player.x=player.x0;
     if(player.x<0)
        player.x=player.x0;
     if(player.y>=boardHeight)
        player.y=player.y0;
   if(player.y<0)
        player.y=player.y0;
   //không cho nhân vật đi xuyên qua chướng ngại vật
    if(Map[player.y][player.x]=='#'||Map[player.y][player.x]=='0')
    {
        player.x=player.x0;
        player.y=player.y0;
    }


   for(int i=0;i<numberMonster;i++)
   {
        if(player.x==monster[i].x&&player.y==monster[i].y)
            {
                endGame=true;
            }
   }
}


void drawAllMonster()
  {
   for(int i=0;i<numberMonster;i++)
       {
         if(monsterDeath[i]==false)         drawMonster(i);
       }
  }


void drawgame()
{
    drawPlayer();
    drawBomb();
    drawAllMonster();
    gotoxy(boardWidth+10,2);                                
        cout<<(int)(clock()-startGame)/CLOCKS_PER_SEC;                      //in thời gian chơi
    SetColor(15);
     gotoxy(boardWidth+11,0);
        cout<<numberMonsterLive;                                            //in số lượng quái vật còn lại



    //điều chỉnh hướng theo chỉ định phím của người dùng
    if (checkKey(KEY_LEFT)) {

            direction.x = -1; direction.y = 0;
    }
    else if (checkKey(KEY_RIGHT)) {

            direction.x = 1; direction.y = 0;
    }
    else if (checkKey(KEY_UP)) {

            direction.y = -1; direction.x = 0;
    }
    else if (checkKey(KEY_DOWN)) {

            direction.y = 1; direction.x = 0;
    }

    else {
        direction.x=0;direction.y=0;
    }

    //khởi động bomb
    if(checkKey(KEY_S))
    {
        check=true;
        bomb.x=player.x;
        bomb.y=player.y;
         start = clock();
    }


}

void moveAllMonster ()
{
    for(int i=0;i<numberMonster;i++)
   {
       if(monsterDeath[i]==false)
     moveMonster(i);
   }

}
void mainloop()
{
 movePlayer();
 moveAllMonster();
 if((player.x==boardWidth-1&&player.y==boardHeight-1))
    {
        endGame=true;
        winGame=true;
    }

}
void drawScreen1()
{
    //in ra màn hình chờ đầu tiên
    char background[11][80]= {          "00000        000       00      00       000000     0000000    0000000",
                                        "00  00      00 00      00 0  0 00       00   00    00         00    00",
                                        "00000      00   00     00  00  00       000000     0000000    00    00",
                                        "00  00     00   00     00      00       00   00    00         0000000",
                                        "00  00      00 00      00      00       00   00    00         00    00",
                                        "00000        000       00      00       000000     0000000    00     00",
                                        "                                                                       ",
                                        "                                                                       ",
                                        "                                                                       ",
                                        "                                                                       "
                            };
    char warning[16][80]= {             "You losted in the maze. Monsters will kill you,if you don't be careful.",
                                        "You must kill all monsters by bombs and look for exit of the maze to go out",
                                        "Good luck!",
                                        "                                                                      ",
                                        "                                                                      ",
                                        "Press 'E' to continue"
                            };
    SetColor(11);
    for (int i=0;i<11;i++)
    {   gotoxy(5,10+i);
        cout<<background[i];
        Sleep(200);
    }
    SetColor(12);
    for (int i=0;i<11;i++)
    {   gotoxy(5,20+i);
        cout<<warning[i];
        Sleep(200);
    }
    while(1)
    {
        if(kbhit())
        {
                  char key=_getch();
                if(key=='e'||key=='E')    break;
        }

    }
    system("cls");//xóa màn hình chờ thứ nhất
}


void drawScreen2()
{
    //in ra màn hình chờ thứ hai
    gotoxy(3,6);
    SetColor(11);
    cout<<"Your character: ";
    SetColor(12);
    cout<<static_cast<char>(1);
    gotoxy(3,7);
    SetColor(11);
    cout<<"The monster: ";
    SetColor(14);
    cout<<static_cast<char>(2);
    gotoxy(3,8);
    SetColor(11);
    cout<<"The bomb: "<<static_cast<char>(79);
    char option[20][80]={
        "Guide :",
        "You maybe come in the maze,find monsters and put bombs to kill them.",
        "Then,bombs will explod during 3 seconds,monsters will die if bomb nearly them",
        "You will win if kill all monsters and go out, or you die if monsters kill you",
        "You also will die if bomb explod nearly you",
        "                                     ",
        "                                     ",
        "You will begin in left up screen",
        "Press 'S' to put bombs and press 'UP' 'DOWN' 'LEFT' 'RIGHT' to go",
        "                                          ",
        "                                           ",
        "Press 'E' to continue",
        "                    ",
        "                     ",
        "                       "
    };
      SetColor(11);
    for (int i=0;i<20;i++)
    {   gotoxy(3,10+i);
        cout<<option[i];
        Sleep(200);
    }
    while(1)
    {
        if(kbhit())
        {
                  char key=_getch();
                if(key=='e'||key=='E')    break;
        }

    }
    system("cls");//xóa màn hình chờ thứ hai
}
void drawLastScreen()
{
    char win[7][80]={
                          "00      00   0000    00    00     00      00  00       00  000000",
                          " 00    00   00  00   00    00     00      00  00       00  00",
                          "  00  00   00    00  00    00     00      00   00     00   00",
                          "   0000    00    00  00    00     00      00    00   00    000000",
                          "    00      00  00    00  00      00      00     00 00     00",
                          "    00       0000      0000       000000  00      000      000000"
                    };
    char lose[7][80]={
                         "00      00   0000    00    00     00000    00    000000",
                         " 00    00   00  00   00    00     00  00   00    00",
                         "  00  00   00    00  00    00     00   00  00    00",
                         "   0000    00    00  00    00     00   00  00    000000",
                         "    00      00  00    00  00      00   00  00    00",
                         "    00       0000      0000       000000   00    000000"
                    };

    if(winGame==true)
   {
       SetColor(11);
        for (int i=0;i<7;i++)
            {
                gotoxy(5,10+i);
                cout<<win[i];
                Sleep(200);
            }

   }
   else
   {
       SetColor(12);
       for (int i=0;i<7;i++)
            {
                gotoxy(5,10+i);
                cout<<lose[i];
                Sleep(200);
            }
   }

   Sleep(1000);
    while (1)
    {
        if (kbhit())
            break;
    }

}
int main()
{

    ShowCur(false);         //ẩn con trỏ;


       drawScreen1();       //in ra màn hình chờ đầu tiên


      drawScreen2();            //in ra màn hình chờ thứ hai


      init();                 //khởi tạo các giá trị ban đầu của game
      while(endGame==false)
        {
            Tick(4,mainloop,drawgame);
        }
    SetBGColor(16);    //đổi lại màu màn về đen
    system("cls");   //xóa màn hình game
    drawLastScreen();
    return 0;
}
