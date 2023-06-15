#include <LedControl.h>

//Constants
#define MAX_SHIPS 1

//Keys
#define X 3
#define Y 5
#define SELECT 2
//States -> todos os states do player1 terminam com 0 e do player 2, com 1
#define MENU 19
#define CREATING_PLAYER_1 20 
#define CREATING_PLAYER_2 21
#define ATTACKING 29
#define ATTACKING_PLAYER_1 30
#define ATTACKING_PLAYER_2 31
#define WIN_PLAYER_1 40
#define WIN_PLAYER_2 41

LedControl lc = LedControl(12, 11, 10, 2);

int x = 0, y = 0, selected = 0, state = MENU;

int field[2][8][8];
int attack_field[2][8][8];


int ships_alive[2] = {0};

void setup() 
{
  //Matrix:
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
    {
      field[0][i][j] = 0;
      field[1][i][j] = 0;
      attack_field[0][i][j] = 0;
      attack_field[1][i][j] = 0;
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
  
  pinMode(SELECT, INPUT);

  Serial.begin(9600);
}
void clearScreen(int player)
{
    if (player > 1 || player < 0)
      return;
    for (int i = 0; i < 8; i++)
      for (int j = 0; j < 8; j++)
      {
        lc.setLed(player,i,j, false);  
      }  
}
void printX(int player)
{
    if (player > 1 || player < 0)
      return;
    for (int i = 0; i < 8; i++)
      for (int j = 0; j < 8; j++)
      {
        if (j = i || j == 7 - i)
          lc.setLed(player,i,j, true);  
      }
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
  //Serial.print("eitatsts");
  /*
  Serial.print("x:");
  Serial.println(analogRead(X));
  Serial.print("y:");
  Serial.println(analogRead(Y));
  */
  if (analogRead(X) > 900)
  {
    printSelection(state % 10, 0);
    x = (x + 1) % 8;
    printSelection(state % 10, 1);
    delay(500);
  }
  else if (analogRead(X) < 100)
  {
    printSelection(state % 10, 0);
    x = x - 1;
    if(x < 0)
      x = 7;
    printSelection(state % 10, 1);
    delay(500);
  }
  else if (analogRead(Y) > 900)
  {
    printSelection(state % 10, 0);
    y = (y + 1) % 8;
    printSelection(state % 10, 1);
    delay(500);
  }
  else if (analogRead(Y) < 100)
  {
    printSelection(state % 10, 0);
    y = y - 1;
    if(y < 0)
      y = 7;
    printSelection(state % 10, 1);
    delay(500);
  }
  else if (digitalRead(SELECT))
  {
    selected = 1;
    delay(500);
  }
}
void printField(int player)
{
   if (player > 1 || player < 0)
    return; 
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
    {
      if (field[player][i][j])
        lc.setLed(player,i,j, true);
      else 
        lc.setLed(player,i,j, false);
    }        
} 
void printAttack(int player)
{
   if (player > 1 || player < 0)
    return; 
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
    {
      if (i == 0 || i == 3)
        lc.setLed(player,i,j, true);
      else if (j == 0 || j == 7)
        lc.setLed(player,i,j, true);
    }
  delay(500);
  clearScreen(player);
}
void createField(int player)
{
  if (player > 1 || player < 0)
    return;
  while (true)
  {
    if (ships_alive[player] >= MAX_SHIPS)
    {
      transition();
      return;
    }  
    getInput();
    if (selected)
    {
      if (!field[player][y][x])
      {
        field[player][y][x] = 1;
        ships_alive[player]++;
      }
    }
    printField(player);
    delay(10);
  }
}
void printAttackField(int player)
{
  if (player > 1 || player < 0)
    return; 
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
    {
      if (attack_field[player][i][j])
        lc.setLed(player,i,j, true);
      else 
        lc.setLed(player,i,j, false);
    }  
}
void attack()
{
  while (ships_alive[0] > 0 && ships_alive[1] > 0)
  {
      printAttack(0);
      printField(1);
      state = ATTACKING_PLAYER_1;
      while(true)
      {
        getInput();
        if (selected)
        {
          if (attack_field[0][y][x] == 0)
          {
            attack_field[0][y][x] = 1;
            if (field[1][y][x] == 1)
            {
              ships_alive[1]--;
            }
            field[1][y][x] = 0;
            break;
          }
        }
        printAttackField(0);
      }
      if (ships_alive[1] < 0)
      {
        state = WIN_PLAYER_1;
        return;
      }
      state = ATTACKING_PLAYER_2;
      printAttack(1);
      printField(0);
      while (true)
      {
        getInput();
        if (selected)
        {
          if (attack_field[1][y][x] == 0)
          {
            attack_field[1][y][x] = 1;
            if (field[0][y][x] == 1)
            {
              ships_alive[0]--;
            }
            field[0][y][x] = 0;
            break;
          }
        }
        printAttackField(1);
      }
      if (ships_alive[0] <= 0)
      {
        state = WIN_PLAYER_2;
        return;
      }
  } 
  
  //Provavelmente irá sair:
  if (ships_alive[0] <= 0)
  {
    state = WIN_PLAYER_2;
    return;
  }
  else if (ships_alive[1] < 0)
  {
    state = WIN_PLAYER_1;
    return;
  }
}
void print_win(int player)
{
  for (int t = 0; t < 4; t++)
  {
    for (int i = t; i < 8 - t; i++)
    {
      lc.setLed(player,i,t, true);
      lc.setLed(player,i,7-t, true);
      delay(10);
    }
    for (int j = t; j < 8 - t; j++)
    {
      lc.setLed(player,t,j, true);
      lc.setLed(player,7-t,j, true);
      delay(10);
    }   
    delay(100);
  }
  delay(100);
  clearScreen(player);
}
void win(int player)
{
  if (player > 1 || player < 0)
    return; 
  selected = 0;
  while(!selected)
  {
    getInput();
    print_win(player);
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
      state = ATTACKING;
      break;
    case ATTACKING:
      attack();
      break;
    case WIN_PLAYER_1:
      win(0);
      break;
    case WIN_PLAYER_2:
      win(1);
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