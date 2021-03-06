#ifndef GAME_H
#define GAME_H

#include <Arduino.h>
#include "globals.h"
#include "inputs.h"
#include "inventory.h"
#include "items.h"
#include "worlddata.h"


void stateGameNew()
{
  arduboy.fillScreen(1);
  byte alphabetX = 12;
  byte alphabetY = 16;
  for (byte i = 0; i < 36; i++)
  {
    sprites.drawErase( alphabetX, alphabetY, font, i + 1);
    alphabetX += 12;
    if (alphabetX > 119)
    {
      alphabetY += 10;
      alphabetX = 12;
    }
  }

  if (arduboy.justPressed(UP_BUTTON)) cursorY += (cursorY > 0) ? -1 : 4;
  else if (arduboy.justPressed(DOWN_BUTTON)) cursorY += (cursorY < 4) ? 1 : -4;
  else if (arduboy.justPressed(LEFT_BUTTON))cursorX += (cursorX > 0) ? -1 : 8;
  else if (arduboy.justPressed(RIGHT_BUTTON)) cursorX += (cursorX < 8) ? 1 : -8;
  if (cursorY == 4) cursorX = 7;
  if (arduboy.justPressed(B_BUTTON))
  {
    byte selectedLetter = cursorX + (cursorY * 9) + 1;
    if (selectedLetter > 36)
    {
      player.name[0] = currentLetter;
      if (!currentLetter)
      {
        player.name[0] = 4;
        player.name[1] = 12;
        player.name[2] = 5;
        player.name[3] = 14;
        player.name[4] = 1;
      }
      currentLetter = 0;
      cursorX = 0;
      cursorY = 0;
      gameState = STATE_GAME_PLAYING;
      ATM.stop();
      return;
    }
    else
    {
      player.name[currentLetter + 1] = selectedLetter;
      currentLetter = min(currentLetter + 1, 5);
    }
  }

  else if (arduboy.justPressed(A_BUTTON))
  {
    if (currentLetter > 0)
    {
      player.name[currentLetter] = 50;
      currentLetter--;
    }
  }

  drawSentence(3, 12, 4, BLACK, ALIGN_LEFT);
  drawWordRam(player.name, 84, 4, BLACK, ALIGN_LEFT);
  sprites.drawErase(6 + (cursorX * 12), 16 + (cursorY * 10), font, 44);
  drawWord(9, 96, 56, BLACK, ALIGN_LEFT);
};

void showPlayField()
{
  if (arduboy.everyXFrames(15)) waterframe = (++waterframe) % 2;
  drawTiles();
  //checkDoors();
  drawPlayer();
}


void stateGamePlaying()
{
  checkInputs();
};

void stateGameInventory()
{
  drawRectangle(83, 0, 130, 64, BLACK);
  drawSentence(4, 93, 2, WHITE, ALIGN_LEFT);
  sprites.drawSelfMasked(86, 2 + (cursorY * 12), font, 44);
  checkInputs();
};

void stateGameEquip()
{
  drawRectangle(83, 0, 130, 64, BLACK);
  drawSentence(10, 93, 2, WHITE, ALIGN_LEFT);
  sprites.drawSelfMasked(86, 2 + (cursorY * 12), font, 44);
  checkInputs();
}

void stateGameStats()
{
  arduboy.fillScreen(0);
  drawPlayerStats();
  checkInputs();
}

void stateGameSave()
{
  drawRectangle(0, 48, 130, 64, BLACK);
  drawSentence(5, 6, 50, WHITE, ALIGN_LEFT);
  yesNo = true;
  checkInputs();
}

void stateGameEnd()
{
  gameState = STATE_MENU_MAIN;
  menuSelection = 2;
}

void showSubMenuStuff()
{
  arduboy.fillScreen(1);
  drawRectangle(0, 0, 130, 8, BLACK);
  drawRectangle(0, 57, 130, 64, BLACK);
  
  drawList();
  
  if (player.hasStuff[(2 * (gameState - STATE_GAME_ITEMS))]) sprites.drawErase(5, 9 + (6 * cursorY), font, 44);
  
  drawWord(93 + (gameState - STATE_GAME_ITEMS), 6, 0, WHITE, ALIGN_LEFT);

  checkInputs();
}


void stateGameOver()
{
  arduboy.fillScreen(1);
};



#endif
