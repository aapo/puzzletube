/*
 The contents of this file are subject to the Mozilla Public License        
 Version 1.1 (the "License"); you may not use this file except in           
 compliance with the License. You may obtain a copy of the License at       
 http://www.mozilla.org/MPL/                                                
                                                                            
 Software distributed under the License is distributed on an "AS IS"        
 basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the    
 License for the specific language governing rights and limitations         
 under the License.                                                         
                                                                            
 Alternatively, the contents of this file may be used under the terms       
 of the GNU Lesser General Public license (the  "LGPL License"), in which case the  
 provisions of LGPL License are applicable instead of those                  
 above.                                                                     
                                                                            
 For feedback and questions about my Files and Projects please mail me,     
 Alexander Matthes (Ziz) , zizsdl_at_googlemail.com                         
*/
#include "menu.h"
#include "game.h"

#define MENUSIZE 1200

int menu_counter = 0;
int state = 0;
int nextstate = 0;
Sint32 menu_choice;
int menu_move = 0;
int menu_fade;
int menu_wait = 0;

void draw_menu(void)
{
  Sint32* modellViewMatrix=engineGetModellViewMatrix();
  plight light=engineGetLightPointer();
  int engineWindowX=engineGetWindowX();
  int engineWindowY=engineGetWindowY();
  engineClearScreen(0);
  setModellViewMatrixIdentity();

  engineTranslate(0,0,-20<<ACCURACY);
  engineRotate(0,1<<ACCURACY,0,menu_counter*10);

  draw_stars();
  
  setModellViewMatrixIdentity();
  engineTranslate(0,0,-20<<ACCURACY);
  engineTranslate(menu_fade<<ACCURACY-6,0,0);
  
  switch (state)
  {
    case 0: //menu
      engineDrawTextMXMY(0,( 10<<ACCURACY-1)+mysin(menu_counter*300+5*MY_PI/3)/4,0,"PUZZLE TUBE");
      engineDrawTextMXMY(0,(  5<<ACCURACY-1)+mysin(menu_counter*300+4*MY_PI/3)/4,0,"Arcade");
      engineDrawTextMXMY(0,(- 0<<ACCURACY-1)+mysin(menu_counter*300+3*MY_PI/3)/4,0,"Tutorial");
      engineDrawTextMXMY(0,(- 5<<ACCURACY-1)+mysin(menu_counter*300+2*MY_PI/3)/4,0,"Free Game");
      engineDrawTextMXMY(0,(-10<<ACCURACY-1)+mysin(menu_counter*300+1*MY_PI/3)/4,0,"Settings");
      engineDrawTextMXMY(0,(-15<<ACCURACY-1)+mysin(menu_counter*300+0*MY_PI/3)/4,0,"Quit");
      //Left circle
      engineDrawSurface((-4<<ACCURACY)+mycos(menu_counter*700+5*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+5*MY_PI/10)-menu_choice*5/2,0,getBigParticle());
      engineDrawSurface((-4<<ACCURACY)+mycos(menu_counter*700+3*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+3*MY_PI/10)-menu_choice*5/2,0,getMiddleParticle());
      engineDrawSurface((-4<<ACCURACY)+mycos(menu_counter*700+2*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+2*MY_PI/10)-menu_choice*5/2,0,getMiddleParticle());
      engineDrawSurface((-4<<ACCURACY)+mycos(menu_counter*700+1*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+1*MY_PI/10)-menu_choice*5/2,0,getSmallParticle());
      engineDrawSurface((-4<<ACCURACY)+mycos(menu_counter*700+0*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+0*MY_PI/10)-menu_choice*5/2,0,getSmallParticle());
      
      engineDrawSurface((-4<<ACCURACY)+mycos(menu_counter*700+15*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+15*MY_PI/10)-menu_choice*5/2,0,getBigParticle());
      engineDrawSurface((-4<<ACCURACY)+mycos(menu_counter*700+13*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+13*MY_PI/10)-menu_choice*5/2,0,getMiddleParticle());
      engineDrawSurface((-4<<ACCURACY)+mycos(menu_counter*700+12*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+12*MY_PI/10)-menu_choice*5/2,0,getMiddleParticle());
      engineDrawSurface((-4<<ACCURACY)+mycos(menu_counter*700+11*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+11*MY_PI/10)-menu_choice*5/2,0,getSmallParticle());
      engineDrawSurface((-4<<ACCURACY)+mycos(menu_counter*700+10*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+10*MY_PI/10)-menu_choice*5/2,0,getSmallParticle());
      //Right circle
      engineDrawSurface(( 4<<ACCURACY)-mycos(menu_counter*700+5*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+5*MY_PI/10)-menu_choice*5/2,0,getBigParticle());
      engineDrawSurface(( 4<<ACCURACY)-mycos(menu_counter*700+3*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+3*MY_PI/10)-menu_choice*5/2,0,getMiddleParticle());
      engineDrawSurface(( 4<<ACCURACY)-mycos(menu_counter*700+2*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+2*MY_PI/10)-menu_choice*5/2,0,getMiddleParticle());
      engineDrawSurface(( 4<<ACCURACY)-mycos(menu_counter*700+1*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+1*MY_PI/10)-menu_choice*5/2,0,getSmallParticle());
      engineDrawSurface(( 4<<ACCURACY)-mycos(menu_counter*700+0*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+0*MY_PI/10)-menu_choice*5/2,0,getSmallParticle());

      engineDrawSurface(( 4<<ACCURACY)-mycos(menu_counter*700+15*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+15*MY_PI/10)-menu_choice*5/2,0,getBigParticle());
      engineDrawSurface(( 4<<ACCURACY)-mycos(menu_counter*700+13*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+13*MY_PI/10)-menu_choice*5/2,0,getMiddleParticle());
      engineDrawSurface(( 4<<ACCURACY)-mycos(menu_counter*700+12*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+12*MY_PI/10)-menu_choice*5/2,0,getMiddleParticle());
      engineDrawSurface(( 4<<ACCURACY)-mycos(menu_counter*700+11*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+11*MY_PI/10)-menu_choice*5/2,0,getSmallParticle());
      engineDrawSurface(( 4<<ACCURACY)-mycos(menu_counter*700+10*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+10*MY_PI/10)-menu_choice*5/2,0,getSmallParticle());
      break;
    case 1: //options
      engineDrawTextMXMY(0,( 10<<ACCURACY-1)+mysin(menu_counter*300+5*MY_PI/3)/4,0,"SETTINGS");
      switch (settings_get_stone_quality())
      {
        case 0: engineDrawTextMXMY(0,(  5<<ACCURACY-1)+mysin(menu_counter*300+4*MY_PI/3)/4,0,"Stone Quality: GP2X"); break;
        case 1: engineDrawTextMXMY(0,(  5<<ACCURACY-1)+mysin(menu_counter*300+4*MY_PI/3)/4,0,"Stone Quality: Okayect"); break;
        case 2: engineDrawTextMXMY(0,(  5<<ACCURACY-1)+mysin(menu_counter*300+4*MY_PI/3)/4,0,"Stone Quality: Perfect"); break;
      }
      if (settings_get_stars_rotating())
        engineDrawTextMXMY(0,(- 0<<ACCURACY-1)+mysin(menu_counter*300+3*MY_PI/3)/4,0,"Rotating Stars: On");
      else
        engineDrawTextMXMY(0,(- 0<<ACCURACY-1)+mysin(menu_counter*300+3*MY_PI/3)/4,0,"Rotating Stars: Off");
      if (settings_get_particles())
        engineDrawTextMXMY(0,(- 5<<ACCURACY-1)+mysin(menu_counter*300+2*MY_PI/3)/4,0,"Particles: On");
      else
        engineDrawTextMXMY(0,(- 5<<ACCURACY-1)+mysin(menu_counter*300+2*MY_PI/3)/4,0,"Particles: Off");
      char buffer[256];
      sprintf(buffer,"Volume %i%%",settings_get_volume());
      engineDrawTextMXMY(0,(-10<<ACCURACY-1)+mysin(menu_counter*300+1*MY_PI/3)/4,0,buffer);
      engineDrawTextMXMY(0,(-15<<ACCURACY-1)+mysin(menu_counter*300+0*MY_PI/3)/4,0,"Back to Menu");
      //Left circle
      engineDrawSurface((-7<<ACCURACY)+mycos(menu_counter*700+5*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+5*MY_PI/10)-menu_choice*5/2,0,getBigParticle());
      engineDrawSurface((-7<<ACCURACY)+mycos(menu_counter*700+3*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+3*MY_PI/10)-menu_choice*5/2,0,getMiddleParticle());
      engineDrawSurface((-7<<ACCURACY)+mycos(menu_counter*700+2*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+2*MY_PI/10)-menu_choice*5/2,0,getMiddleParticle());
      engineDrawSurface((-7<<ACCURACY)+mycos(menu_counter*700+1*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+1*MY_PI/10)-menu_choice*5/2,0,getSmallParticle());
      engineDrawSurface((-7<<ACCURACY)+mycos(menu_counter*700+0*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+0*MY_PI/10)-menu_choice*5/2,0,getSmallParticle());
      
      engineDrawSurface((-7<<ACCURACY)+mycos(menu_counter*700+15*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+15*MY_PI/10)-menu_choice*5/2,0,getBigParticle());
      engineDrawSurface((-7<<ACCURACY)+mycos(menu_counter*700+13*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+13*MY_PI/10)-menu_choice*5/2,0,getMiddleParticle());
      engineDrawSurface((-7<<ACCURACY)+mycos(menu_counter*700+12*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+12*MY_PI/10)-menu_choice*5/2,0,getMiddleParticle());
      engineDrawSurface((-7<<ACCURACY)+mycos(menu_counter*700+11*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+11*MY_PI/10)-menu_choice*5/2,0,getSmallParticle());
      engineDrawSurface((-7<<ACCURACY)+mycos(menu_counter*700+10*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+10*MY_PI/10)-menu_choice*5/2,0,getSmallParticle());
      //Right circle
      engineDrawSurface(( 7<<ACCURACY)-mycos(menu_counter*700+5*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+5*MY_PI/10)-menu_choice*5/2,0,getBigParticle());
      engineDrawSurface(( 7<<ACCURACY)-mycos(menu_counter*700+3*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+3*MY_PI/10)-menu_choice*5/2,0,getMiddleParticle());
      engineDrawSurface(( 7<<ACCURACY)-mycos(menu_counter*700+2*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+2*MY_PI/10)-menu_choice*5/2,0,getMiddleParticle());
      engineDrawSurface(( 7<<ACCURACY)-mycos(menu_counter*700+1*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+1*MY_PI/10)-menu_choice*5/2,0,getSmallParticle());
      engineDrawSurface(( 7<<ACCURACY)-mycos(menu_counter*700+0*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+0*MY_PI/10)-menu_choice*5/2,0,getSmallParticle());

      engineDrawSurface(( 7<<ACCURACY)-mycos(menu_counter*700+15*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+15*MY_PI/10)-menu_choice*5/2,0,getBigParticle());
      engineDrawSurface(( 7<<ACCURACY)-mycos(menu_counter*700+13*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+13*MY_PI/10)-menu_choice*5/2,0,getMiddleParticle());
      engineDrawSurface(( 7<<ACCURACY)-mycos(menu_counter*700+12*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+12*MY_PI/10)-menu_choice*5/2,0,getMiddleParticle());
      engineDrawSurface(( 7<<ACCURACY)-mycos(menu_counter*700+11*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+11*MY_PI/10)-menu_choice*5/2,0,getSmallParticle());
      engineDrawSurface(( 7<<ACCURACY)-mycos(menu_counter*700+10*MY_PI/10),( 3<<ACCURACY)+mysin(menu_counter*700+10*MY_PI/10)-menu_choice*5/2,0,getSmallParticle());
      break;
    
  }

  engineDrawList();
  draw_music();
  engineFlip();  
}

int calc_menu(Uint32 steps)
{
  menu_counter += steps;
  calc_music(steps);
  pEngineInput engineInput = engineGetInput();
  if (menu_fade>0)
  {
    menu_fade-=steps*2;
    if (menu_fade<=0)
      menu_fade = 0;
    return 0;
  }
  if (menu_fade<0)
  {
    menu_fade-=steps*2;
    if (menu_fade<=-MENUSIZE)
    {
      menu_fade = MENUSIZE;
      if (nextstate>=0)
      {
        state = nextstate;
        menu_choice = 0;
      }
      else
      switch (nextstate)
      {
        case -3: //Free Game
          run_game(1,timeMode,9);
          engineInput->button[BUTTON_START] = 0;
          engineInput->button[BUTTON_A] = 0;
          engineInput->button[BUTTON_B] = 0;
          engineInput->button[BUTTON_X] = 0;
          engineInput->button[BUTTON_Y] = 0;
          state = 0;
          break;
        case -4: //Free Game
          return 1;
      }
      return 0;
    }
  }

  int i;
  
  switch (state)
  {
  case 0: //menu
    if (engineInput->axis[1]<0 && menu_move==0 && (menu_choice>>ACCURACY)<4)
      menu_move = 1;
    if (engineInput->axis[1]>0 && menu_move==0 && (menu_choice>>ACCURACY)>0)
      menu_move = -1;
    
    if (menu_move != 0)
    {
      for (i=0;i<steps;i++)
      {
        menu_choice += menu_move*4 << ACCURACY-10;
        if (menu_choice == ((menu_choice >>ACCURACY)<<ACCURACY))
          menu_move = 0;
      }
    }
    if (menu_move == 0 && (engineInput->button[BUTTON_START] ||
        engineInput->button[BUTTON_A] || engineInput->button[BUTTON_B] ||
        engineInput->button[BUTTON_X] || engineInput->button[BUTTON_Y]))
    {
      switch (menu_choice>>ACCURACY)
      {
        case 2: // Free Game
          nextstate = -3;
          menu_fade = -1;
          break;
        case 3: // Options
          nextstate = 1;
          menu_fade = -1;
          break;
        case 4: // Quit
          nextstate = -4;
          menu_fade = -1;
          break;
      }
    }
    break;
  case 1: //options
    if (engineInput->axis[1]<0 && menu_move==0 && (menu_choice>>ACCURACY)<4)
      menu_move = 1;
    if (engineInput->axis[1]>0 && menu_move==0 && (menu_choice>>ACCURACY)>0)
      menu_move = -1;
    
    if (menu_move != 0)
    {
      for (i=0;i<steps;i++)
      {
        menu_choice += menu_move*4 << ACCURACY-10;
        if (menu_choice == ((menu_choice >>ACCURACY)<<ACCURACY))
          menu_move = 0;
      }
    }
    for (i = 0;i < steps;i++)
    {
      menu_wait--;
      if (menu_move == 0 && (menu_choice>>ACCURACY) == 3 && engineInput->axis[0]<0 && menu_wait <= 0 && settings_get_volume()>0)
      {
        settings_set_volume(settings_get_volume()-1);
        Mix_VolumeMusic(settings_get_volume()*128/100);
        menu_wait = 25;
      }
      if (menu_move == 0 && (menu_choice>>ACCURACY) == 3 && engineInput->axis[0]>0 && menu_wait <= 0 && settings_get_volume()<100)
      {
        settings_set_volume(settings_get_volume()+1);
        Mix_VolumeMusic(settings_get_volume()*128/100);
        menu_wait = 25;
      }
    }
    if (menu_move == 0 && (menu_choice>>ACCURACY) == 0 && engineInput->axis[0]<0 && menu_wait <= 0 && settings_get_stone_quality()>0)
    {
      settings_set_stone_quality(settings_get_stone_quality()-1);
      engineInput->axis[0] = 0;
    }
    if (menu_move == 0 && (menu_choice>>ACCURACY) == 0 && engineInput->axis[0]>0 && menu_wait <= 0 && settings_get_stone_quality()<2)
    {
      settings_set_stone_quality(settings_get_stone_quality()+1);
      engineInput->axis[0] = 0;
    }
    if (menu_move == 0 && (engineInput->button[BUTTON_START] ||
        engineInput->button[BUTTON_A] || engineInput->button[BUTTON_B] ||
        engineInput->button[BUTTON_X] || engineInput->button[BUTTON_Y]))
    {
      engineInput->button[BUTTON_START] = 0;
      engineInput->button[BUTTON_A] = 0;
      engineInput->button[BUTTON_B] = 0;
      engineInput->button[BUTTON_X] = 0;
      engineInput->button[BUTTON_Y] = 0;
      switch (menu_choice>>ACCURACY)
      {
        case 0: //Stone Quality
          settings_set_stone_quality((settings_get_stone_quality()+1)%3);
          break;
        case 1: //Stone Quality
          settings_set_stars_rotating((settings_get_stars_rotating()+1)%2);
          break;
        case 2: //Stone Quality
          settings_set_particles((settings_get_particles()+1)%2);
          break;
        case 4: //Back
          settings_save();
          nextstate = 0;
          menu_fade = -1;
          break;
      }
    }
    break;
  }
  return 0;
}

void run_menu()
{
  menu_fade = MENUSIZE;
  menu_choice = 0;
  engineLoop(draw_menu,calc_menu,10);
}
