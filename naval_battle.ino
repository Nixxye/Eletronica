#include <LedControl.h>

//Constants
#define MAX_SHIPS 3

//Keys
#define LEFT 13
#define RIGHT 7
#define DOWN 8
#define SELECT 2
//States -> todos os states do player1 terminam com 0 e do player 2, com 1
#define MENU 19
#define CREATING_PLAYER_1 20 
#define CREATING_PLAYER_2 21

LedControl lc = LedControl(12, 11, 10, 2);

int x = 0, y = 0, selected = 0, state = MENU;

int field[2][8][8];

int ships_alive[2] = {0};

void setup() 
{
  //Matrix:
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
    {
      field[0][i][j] = 0;
      field[1][i][j] = 0;
    }
  //Player 1:
  lc.shutdown(0,false);
  lc.setIntensity(0,1);
  lc.clearDisplay(0);
  //Player 2:
  lc.shutdown(1,false);
  lc.setIntensity(1,1);
  lc.clearDisplay(1);
  //Botões
  pinMode(LEFT, INPUT);
  pinMode(RIGHT, INPUT);
  pinMode(DOWN, INPUT);
  pinMode(SELECT, INPUT);
}
void transition()
{
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
    {
        lc.setLed(0,i,j, true);
        lc.setLed(1,i,j, true);
        delay(10);
    }
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
    {
        lc.setLed(0,i,j, false);
        lc.setLed(1,i,j, false);
        delay(10);
    }
}
void printSelection(int player, int print)
{
  if (player > 1 || player < 0)
    return;
  if (print)
    lc.setLed(player,y,x,true);
  else 
    lc.setLed(player,y,x,false);
}
void getInput()
{
  //Colocar para piscar na seleção:
  selected = 0;

  if (digitalRead(RIGHT))
  {
    printSelection(state % 10, 0);
    x = (x + 1) % 8;
    printSelection(state % 10, 1);
    delay(500);
  }
  else if (digitalRead(LEFT))
  {
    printSelection(state % 10, 0);
    x = x - 1;
    if(x < 0)
      x = 7;
    printSelection(state % 10, 1);
    delay(500);
  }
  else if (digitalRead(DOWN))
  {
    printSelection(state % 10, 0);
    y = (y + 1) % 8;
    printSelection(state % 10, 1);
    delay(500);
  }
  else if (digitalRead(SELECT))
  {
    selected = 1;
    delay(500);
  }
}
void print()
{
  switch (state)
  {
    /*
    case CREATING_PLAYER_1:
      for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
          if (field[0][i][j])
            lc.setLed(0,i,j, true);
          else 
            lc.setLed(0,i,j, false);
        }
      break;
    case CREATING_PLAYER_2:
      for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
          if (field[1][i][j])
            lc.setLed(1,i,j, true);
          else 
            lc.setLed(1,i,j, false);
        }
      break;
      */
    default:
        for (int i = 0; i < 8; i++)
          for (int j = 0; j < 8; j++)
          {
            if (field[0][i][j])
              lc.setLed(0,i,j, true);
            else 
              lc.setLed(0,i,j, false);
            if (field[1][i][j])
              lc.setLed(1,i,j, true);
            else 
              lc.setLed(1,i,j, false);
        }
      break;
  } 
  return;
} 

void createField(int player)
{
  if (player > 1 || player < 0)
    return;
  while (true)
  {
    getInput();
    if (selected)
    {
      if (!field[player][y][x])
      {
        field[player][y][x] = 1;
        ships_alive[player]++;
      }
      if (ships_alive[player] >= MAX_SHIPS)
      {
        transition();
        return;
      }
    }
    print();
    delay(10);
  }
}
void stateManager()
{
  switch (state)
  {
    case MENU:
      transition();
      state = CREATING_PLAYER_1;
      break;
    case CREATING_PLAYER_1:
      createField(0);
      state = CREATING_PLAYER_2;
      break;
    case CREATING_PLAYER_2:
      createField(1);
      //Apenas para testes:
      state = MENU;
      break;
    default:
      break;
  }
}

void loop() 
{  
  getInput();
  stateManager();
  /*
  if (digitalRead(RIGHT))
  {
    lc.setLed(0,i,j,false);
    i = (i + 1) % 8;
    lc.setLed(0,i,j,true);
    delay(500);
  }
  else if (digitalRead(LEFT))
  {
    lc.setLed(0,i,j,false);
    i = i - 1;
    if(i < 0)
      i = 7;
    lc.setLed(0,i,j,true); 
    delay(500);
  }
  else if (digitalRead(DOWN))
  {
    lc.setLed(0,i,j,false);
    j = (j + 1) % 8;
    lc.setLed(0,i,j,true);   
    delay(500);  
  }
  else if (digitalRead(SELECT))
  {
    for (int y = 0; y < 8; y++)
      for (int x = 0; x < 8; x++)
          lc.setLed(0,y,x, selected);
    selected = !selected;
    delay(500);
  }
  */
}
