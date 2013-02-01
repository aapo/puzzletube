/*
 The contents of this file are subject to the Mozilla Public License				
 Version 1.1 (the "License",font); you may not use this file except in					 
 compliance with the License. You may obtain a copy of the License at			 
 http://www.mozilla.org/MPL/																								
																																						
 Software distributed under the License is distributed on an "AS IS"				
 basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the		
 License for the specific language governing rights and limitations				 
 under the License.																												 
																																						
 Alternatively, the contents of this file may be used under the terms			 
 of the GNU Lesser General Public license (the	"LGPL License"), in which case the	
 provisions of LGPL License are applicable instead of those									
 above.																																		 
																																						
 For feedback and questions about my Files and Projects please mail me,		 
 Alexander Matthes (Ziz) , zizsdl_at_googlemail.com												 
*/
#include "menu.h"
#include "game.h"
#include "settings.h"
#include "particle.h"

#define MENUSIZE 1200

void (*menu_resize)(Uint16 w,Uint16 h);

int menu_counter = 0;
int star_counter = 0;
int state = 0;
int nextstate = 0;
Sint32 menu_choice;
int menu_move = 0;
int menu_fade;
int menu_wait = 0;
int menu_block = 0;
spBundlePointer menu_bundle;
SDL_Surface* flag;

void draw_menu(void)
{
	#ifdef MENU_DEBUG
		printf("%i:	 Start drawing menu\n",SDL_GetTicks());
	#endif
	Sint32* modellViewMatrix=spGetMatrix();
	int engineWindowX=spGetWindowSurface()->w;
	int engineWindowY=spGetWindowSurface()->h;
	spFontPointer font = settings_get_font();
	spFontPointer small_font = settings_get_small_font();
	spFontPointer middle_font = settings_get_middle_font();
	
	spSetAlphaTest(1);
	spSetZSet(0);
	spSetZTest(0);
	spResetZBuffer();
	
	switch (settings_get_stars_rotating())
	{
		case 2: case 1:draw_stars(star_counter*10); break;
		default:
			spClearTarget(BACKGROUND_COLOR);
	}
	#ifdef MENU_DEBUG
		printf("%i:	 Drew stars\n",SDL_GetTicks());
	#endif
	
	spIdentity();
	draw_music();
	#ifdef MENU_DEBUG
		printf("%i:	 Drew music\n",SDL_GetTicks());
	#endif
	spTranslate(0,0,-20<<SP_ACCURACY);
	spTranslate(menu_fade<<SP_ACCURACY-6,0,0);	
	int mode;
	int colors;
	int difficult;
	int i,j;
	char buffer[256];
	Sint32 factor;
	switch (state)
	{
		case 0: //menu
			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),1*engineWindowY/7+(spSin(menu_counter*300+6*SP_PI*2/7)>>SP_ACCURACY-2),-1,"PUZZLE TUBE",font);
			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),2*engineWindowY/7+(spSin(menu_counter*300+4*SP_PI*2/7)>>SP_ACCURACY-2),-1,spGetTranslationFromCaption(menu_bundle,"Free Game"),font);
			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),3*engineWindowY/7+(spSin(menu_counter*300+3*SP_PI*2/7)>>SP_ACCURACY-2),-1,spGetTranslationFromCaption(menu_bundle,"How to Play"),font);
			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),4*engineWindowY/7+(spSin(menu_counter*300+2*SP_PI*2/7)>>SP_ACCURACY-2),-1,spGetTranslationFromCaption(menu_bundle,"Highscore"),font);
			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),5*engineWindowY/7+(spSin(menu_counter*300+1*SP_PI*2/7)>>SP_ACCURACY-2),-1,spGetTranslationFromCaption(menu_bundle,"Settings"),font);
			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),6*engineWindowY/7+(spSin(menu_counter*300+0*SP_PI*2/7)>>SP_ACCURACY-2),-1,spGetTranslationFromCaption(menu_bundle,"Quit"),font);
			
			spTranslate(0,1<<SP_ACCURACY-2,3<<SP_ACCURACY-1);
			spTranslate(-7<<SP_ACCURACY,(3<<SP_ACCURACY)-menu_choice*5/2,0);
			draw_particle_circle(-1,menu_counter);
			spTranslate(14<<SP_ACCURACY,0,0);
			draw_particle_circle(+1,menu_counter);

			break;
		case 1: //options
			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),1*engineWindowY/10+(spSin(menu_counter*300+6*SP_PI/4)>>SP_ACCURACY-2),-1,spGetTranslationFromCaption(menu_bundle,"SETTINGS"),font);
			spFontDrawMiddle(engineWindowX/2+((menu_fade-32)*spGetSizeFactor()>>SP_ACCURACY+2),2*engineWindowY/10+(spSin(menu_counter*300+5*SP_PI/4)>>SP_ACCURACY-2),-1,spGetTranslationFromCaption(menu_bundle,"Stone Quality:"),font);
			factor = spDiv(27 << SP_ACCURACY - 8,spGetSizeFactor());
			spTranslate(( ((menu_fade-32)*spGetSizeFactor()>>SP_ACCURACY+2)+spFontWidth(spGetTranslationFromCaption(menu_bundle,"Stone Quality:"),font)/2)*factor, (2*engineWindowY/10-(spSin(menu_counter*300+5*SP_PI/4)>>SP_ACCURACY-2))*factor,-1<<SP_ACCURACY-2);
			spRotateZ(-spSin(menu_counter << 8) >> 2);
			draw_stone(6,get_type_color_h(6,(menu_counter*16)%(2*SP_PI)),get_type_color_s(6,(menu_counter*16)%(2*SP_PI)),get_type_color_v(6,(menu_counter*16)%(2*SP_PI)),4,0,(menu_counter*16)%(2*SP_PI));
			spRotateZ(spSin(menu_counter << 8) >> 2);
			spTranslate((-((menu_fade-32)*spGetSizeFactor()>>SP_ACCURACY+2)-spFontWidth(spGetTranslationFromCaption(menu_bundle,"Stone Quality:"),font)/2)*factor,-(2*engineWindowY/10-(spSin(menu_counter*300+5*SP_PI/4)>>SP_ACCURACY-2))*factor, 1<<SP_ACCURACY-2);
			switch (settings_get_stars_rotating())
			{
				case 0: spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),3*engineWindowY/10+(spSin(menu_counter*300+4*SP_PI/4)>>SP_ACCURACY-2),-1,spGetTranslationFromCaption(menu_bundle,"Stars: Off"),font); break;
				case 1: spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),3*engineWindowY/10+(spSin(menu_counter*300+4*SP_PI/4)>>SP_ACCURACY-2),-1,spGetTranslationFromCaption(menu_bundle,"Stars: Still"),font); break;
				case 2: spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),3*engineWindowY/10+(spSin(menu_counter*300+4*SP_PI/4)>>SP_ACCURACY-2),-1,spGetTranslationFromCaption(menu_bundle,"Stars: Rotating"),font); break;
			}
			/*if (settings_get_particles())
				spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),4*engineWindowY/10+(spSin(menu_counter*300+3*SP_PI/4)>>SP_ACCURACY-2),-1,spGetTranslationFromCaption(menu_bundle,"Particles: On"),font);
			else
				spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),4*engineWindowY/10+(spSin(menu_counter*300+3*SP_PI/4)>>SP_ACCURACY-2),-1,spGetTranslationFromCaption(menu_bundle,"Particles: Off"),font);*/

			switch (settings_get_control())
			{
				case 0:
					spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),4*engineWindowY/10+(spSin(menu_counter*300+3*SP_PI/4)>>SP_ACCURACY-2),-1,spGetTranslationFromCaption(menu_bundle,"Control: Normal"),font);
					spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),19*engineWindowY/40+(spSin(menu_counter*300+9*SP_PI/12)>>SP_ACCURACY-2),-1,spGetTranslationFromCaption(menu_bundle,"Description Normal 1"),small_font);
					spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),22*engineWindowY/40+(spSin(menu_counter*300+7*SP_PI/12)>>SP_ACCURACY-2),-1,spGetTranslationFromCaption(menu_bundle,"Description Normal 2"),small_font);
					break;
				case 1:
					spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),4*engineWindowY/10+(spSin(menu_counter*300+3*SP_PI/4)>>SP_ACCURACY-2),-1,spGetTranslationFromCaption(menu_bundle,"Control: Snake"),font);
					spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),19*engineWindowY/40+(spSin(menu_counter*300+9*SP_PI/12)>>SP_ACCURACY-2),-1,spGetTranslationFromCaption(menu_bundle,"Description Snake 1"),small_font);
					spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),22*engineWindowY/40+(spSin(menu_counter*300+7*SP_PI/12)>>SP_ACCURACY-2),-1,spGetTranslationFromCaption(menu_bundle,"Description Snake 2"),small_font);
					break;
				case 2:
					spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),4*engineWindowY/10+(spSin(menu_counter*300+3*SP_PI/4)>>SP_ACCURACY-2),-1,spGetTranslationFromCaption(menu_bundle,"Control: Padmode"),font);
					spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),19*engineWindowY/40+(spSin(menu_counter*300+9*SP_PI/12)>>SP_ACCURACY-2),-1,spGetTranslationFromCaption(menu_bundle,"Description Padmode 1"),small_font);
					spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),22*engineWindowY/40+(spSin(menu_counter*300+7*SP_PI/12)>>SP_ACCURACY-2),-1,spGetTranslationFromCaption(menu_bundle,"Description Padmode 2"),small_font);
					break;
			}
			
			spFontDrawMiddle(engineWindowX/2+((menu_fade-80)*spGetSizeFactor()>>SP_ACCURACY+2),6*engineWindowY/10+(spSin(menu_counter*300+1*SP_PI/4)>>SP_ACCURACY-2),-1,spGetPossibleLanguageName(settings_get_language()),font);
			spRotozoomSurface(spFontWidth(spGetPossibleLanguageName(settings_get_language()),font)/2+engineWindowX/2+((menu_fade+40)*spGetSizeFactor()>>SP_ACCURACY+2),6*engineWindowY/10+(spSin(menu_counter*300+1*SP_PI/4)>>SP_ACCURACY-2)+font->maxheight/2,-1,flag,spGetSizeFactor() >> 3,spGetSizeFactor() >> 3,spSin(menu_counter << 8) >> 2);

			sprintf(buffer,"Volume %i%%",settings_get_volume());
			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),7*engineWindowY/10+(spSin(menu_counter*300+0*SP_PI/4)>>SP_ACCURACY-2),-1,buffer,font);
			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),8*engineWindowY/10+(spSin(menu_counter*300-1*SP_PI/4)>>SP_ACCURACY-2),-1,spGetTranslationFromCaption(menu_bundle,"Back to Menu"),font);

			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),30*engineWindowY/32+(spSin(menu_counter*300+-1*SP_PI/4)>>SP_ACCURACY-2),-1,spGetTranslationFromCaption(menu_bundle,"Select or Start: back"),small_font);


			spTranslate(0,1<<SP_ACCURACY-2,-1<<SP_ACCURACY-1);
			spTranslate(-11<<SP_ACCURACY,(5<<SP_ACCURACY)-menu_choice*4/2,0);
			draw_particle_circle(-1,menu_counter);
			spTranslate( 22<<SP_ACCURACY,0,0);
			draw_particle_circle(+1,menu_counter);
			break;
		case 2: //Free Game
			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),1*engineWindowY/7+(spSin(menu_counter*300+5*SP_PI/3)>>SP_ACCURACY-2),-1,"FREE GAME",font);
			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),2*engineWindowY/7+(spSin(menu_counter*300+1*SP_PI/3)>>SP_ACCURACY-2),-1,"<<< Play >>>",font);
			switch (settings_get_mode())
			{
				case 0: spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),3*engineWindowY/7+(spSin(menu_counter*300+4*SP_PI/3)>>SP_ACCURACY-2),-1,"Game Mode: Points",font); break;
				case 1: spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),3*engineWindowY/7+(spSin(menu_counter*300+4*SP_PI/3)>>SP_ACCURACY-2),-1,"Game Mode: Survival",font); break;
				case 2: spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),3*engineWindowY/7+(spSin(menu_counter*300+4*SP_PI/3)>>SP_ACCURACY-2),-1,"Game Mode: Race",font); break;
			}
			if (settings_get_color())
				spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),4*engineWindowY/7+(spSin(menu_counter*300+3*SP_PI/3)>>SP_ACCURACY-2),-1,"Difficulty: Hard",font);
			else
				spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),4*engineWindowY/7+(spSin(menu_counter*300+3*SP_PI/3)>>SP_ACCURACY-2),-1,"Difficulty: Normal",font);
			
			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),6*engineWindowY/7+(spSin(menu_counter*300+1*SP_PI/3)>>SP_ACCURACY-2),-1,"Back to Menu",font);

			spTranslate(0,1<<SP_ACCURACY-2,3<<SP_ACCURACY-1);
			spTranslate(-10<<SP_ACCURACY,(3<<SP_ACCURACY)-menu_choice*5/2,0);
			draw_particle_circle(-1,menu_counter);
			spTranslate(20<<SP_ACCURACY,0,0);
			draw_particle_circle(+1,menu_counter);
			break;
		case 3: //High Score
			#define HIGHSCORE_GAP_X		(engineWindowX/32)
			#define HIGHSCORE_GAP_IN	(engineWindowX/64)
			#define HIGHSCORE_WIDTH		((engineWindowX-(4*HIGHSCORE_GAP_X))/3)
			#define HIGHSCORE_GAP_ABOVE (engineWindowY/5)
			#define HIGHSCORE_GAP_Y	 	(engineWindowX/64)
			#define HIGHSCORE_HEIGHT 	((engineWindowY-HIGHSCORE_GAP_ABOVE-(4*HIGHSCORE_GAP_Y))/2)
					
			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),engineWindowY/48+(spSin(menu_counter*300+6*SP_PI*2/7)>>SP_ACCURACY-2),-1,spGetTranslationFromCaption(menu_bundle,"Highscore"),font);
			spFontDrawMiddle(1*HIGHSCORE_GAP_X + 1*HIGHSCORE_WIDTH/2 +(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),5*engineWindowY/48+(spSin(menu_counter*300+5*SP_PI*2/7)>>SP_ACCURACY-2),-1,spGetTranslationFromCaption(menu_bundle,"Points"),font);
			spFontDrawMiddle(2*HIGHSCORE_GAP_X + 3*HIGHSCORE_WIDTH/2 +(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),5*engineWindowY/48+(spSin(menu_counter*300+5*SP_PI*2/7)>>SP_ACCURACY-2),-1,spGetTranslationFromCaption(menu_bundle,"Survival"),font);
			spFontDrawMiddle(3*HIGHSCORE_GAP_X + 5*HIGHSCORE_WIDTH/2 +(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),5*engineWindowY/48+(spSin(menu_counter*300+5*SP_PI*2/7)>>SP_ACCURACY-2),-1,spGetTranslationFromCaption(menu_bundle,"Race"),font);
			
			draw_border(1*HIGHSCORE_GAP_X+                  (menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),HIGHSCORE_GAP_ABOVE+                 (spSin(menu_counter*300+4*SP_PI*2/7)>>SP_ACCURACY-2),
						1*HIGHSCORE_GAP_X+  HIGHSCORE_WIDTH+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),HIGHSCORE_GAP_ABOVE+HIGHSCORE_HEIGHT+(spSin(menu_counter*300+4*SP_PI*2/7)>>SP_ACCURACY-2),65535);
			draw_border(2*HIGHSCORE_GAP_X+  HIGHSCORE_WIDTH+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),HIGHSCORE_GAP_ABOVE+                 (spSin(menu_counter*300+4*SP_PI*2/7)>>SP_ACCURACY-2),
						2*HIGHSCORE_GAP_X+2*HIGHSCORE_WIDTH+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),HIGHSCORE_GAP_ABOVE+HIGHSCORE_HEIGHT+(spSin(menu_counter*300+4*SP_PI*2/7)>>SP_ACCURACY-2),65535);
			draw_border(3*HIGHSCORE_GAP_X+2*HIGHSCORE_WIDTH+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),HIGHSCORE_GAP_ABOVE+                 (spSin(menu_counter*300+4*SP_PI*2/7)>>SP_ACCURACY-2),
						3*HIGHSCORE_GAP_X+3*HIGHSCORE_WIDTH+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),HIGHSCORE_GAP_ABOVE+HIGHSCORE_HEIGHT+(spSin(menu_counter*300+4*SP_PI*2/7)>>SP_ACCURACY-2),65535);
			
			draw_border(1*HIGHSCORE_GAP_X+                  (menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),HIGHSCORE_GAP_ABOVE+HIGHSCORE_GAP_Y+1*HIGHSCORE_HEIGHT+(spSin(menu_counter*300+3*SP_PI*2/7)>>SP_ACCURACY-2),
						1*HIGHSCORE_GAP_X+  HIGHSCORE_WIDTH+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),HIGHSCORE_GAP_ABOVE+HIGHSCORE_GAP_Y+2*HIGHSCORE_HEIGHT+(spSin(menu_counter*300+3*SP_PI*2/7)>>SP_ACCURACY-2),65535);
			draw_border(2*HIGHSCORE_GAP_X+  HIGHSCORE_WIDTH+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),HIGHSCORE_GAP_ABOVE+HIGHSCORE_GAP_Y+1*HIGHSCORE_HEIGHT+(spSin(menu_counter*300+3*SP_PI*2/7)>>SP_ACCURACY-2),
						2*HIGHSCORE_GAP_X+2*HIGHSCORE_WIDTH+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),HIGHSCORE_GAP_ABOVE+HIGHSCORE_GAP_Y+2*HIGHSCORE_HEIGHT+(spSin(menu_counter*300+3*SP_PI*2/7)>>SP_ACCURACY-2),65535);
			draw_border(3*HIGHSCORE_GAP_X+2*HIGHSCORE_WIDTH+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),HIGHSCORE_GAP_ABOVE+HIGHSCORE_GAP_Y+1*HIGHSCORE_HEIGHT+(spSin(menu_counter*300+3*SP_PI*2/7)>>SP_ACCURACY-2),
						3*HIGHSCORE_GAP_X+3*HIGHSCORE_WIDTH+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),HIGHSCORE_GAP_ABOVE+HIGHSCORE_GAP_Y+2*HIGHSCORE_HEIGHT+(spSin(menu_counter*300+3*SP_PI*2/7)>>SP_ACCURACY-2),65535);
			
			
			for (j = 0; j < 3; j++)
			{
				spFontDrawMiddle((j+1)*HIGHSCORE_GAP_X + (2*j+1)*HIGHSCORE_WIDTH/2 +(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),0*HIGHSCORE_HEIGHT/4+HIGHSCORE_GAP_ABOVE+(spSin(menu_counter*300+4*SP_PI*2/7)>>SP_ACCURACY-2),-1,spGetTranslationFromCaption(menu_bundle,"Normal"),font);	
				for (i = 0; i < 3; i++)
				{
					Uint16 color = 65535; //for trophies :)
					char* name = get_highscore_name(j,0,i);
					sprintf(buffer,"%i. %c%c%c",i+1,name[0],name[1],name[2]);
					spFontDraw((j+1)*HIGHSCORE_GAP_X+j*HIGHSCORE_WIDTH+HIGHSCORE_GAP_IN+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),(i+1)*HIGHSCORE_HEIGHT/4+HIGHSCORE_GAP_ABOVE+(spSin(menu_counter*300+4*SP_PI*2/7)>>SP_ACCURACY-2),-1,buffer,middle_font);
					if (j==0)
						sprintf(buffer,"%i",get_highscore(j,0,i));
					else
						sprintf(buffer,"%i.%i",get_highscore(j,0,i)/10,get_highscore(j,0,i)%10);
					spFontDrawRight((j+1)*(HIGHSCORE_GAP_X+HIGHSCORE_WIDTH)-HIGHSCORE_GAP_IN+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),(i+1)*HIGHSCORE_HEIGHT/4+HIGHSCORE_GAP_ABOVE+(spSin(menu_counter*300+4*SP_PI*2/7)>>SP_ACCURACY-2),-1,buffer,middle_font);
					spRectangle((j+1)*HIGHSCORE_GAP_X+(2*j+1)*HIGHSCORE_WIDTH/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),
								middle_font->maxheight+2+(i+1)*HIGHSCORE_HEIGHT/4+HIGHSCORE_GAP_ABOVE+(spSin(menu_counter*300+4*SP_PI*2/7)>>SP_ACCURACY-2),0,
								HIGHSCORE_WIDTH-2*HIGHSCORE_GAP_IN,engineWindowY/128,color);
				}			
				spFontDrawMiddle((j+1)*HIGHSCORE_GAP_X + (2*j+1)*HIGHSCORE_WIDTH/2 +(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),4*HIGHSCORE_HEIGHT/4+HIGHSCORE_GAP_ABOVE+HIGHSCORE_GAP_Y+(spSin(menu_counter*300+3*SP_PI*2/7)>>SP_ACCURACY-2),-1,spGetTranslationFromCaption(menu_bundle,"Hard"),font);	
				for (i = 0; i < 3; i++)
				{
					Uint16 color = 65535; //for trophies :)
					char* name = get_highscore_name(j,1,i);
					sprintf(buffer,"%i. %c%c%c",i+1,name[0],name[1],name[2]);
					spFontDraw((j+1)*HIGHSCORE_GAP_X+j*HIGHSCORE_WIDTH+HIGHSCORE_GAP_IN+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),(i+5)*HIGHSCORE_HEIGHT/4+HIGHSCORE_GAP_ABOVE+HIGHSCORE_GAP_Y+(spSin(menu_counter*300+3*SP_PI*2/7)>>SP_ACCURACY-2),-1,buffer,middle_font);
					if (j==0)
						sprintf(buffer,"%i",get_highscore(j,1,i));
					else
						sprintf(buffer,"%i.%i",get_highscore(j,1,i)/10,get_highscore(j,1,i)%10);
					spFontDrawRight((j+1)*(HIGHSCORE_GAP_X+HIGHSCORE_WIDTH)-HIGHSCORE_GAP_IN+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),(i+5)*HIGHSCORE_HEIGHT/4+HIGHSCORE_GAP_Y+HIGHSCORE_GAP_ABOVE+(spSin(menu_counter*300+3*SP_PI*2/7)>>SP_ACCURACY-2),-1,buffer,middle_font);
					spRectangle((j+1)*HIGHSCORE_GAP_X+(2*j+1)*HIGHSCORE_WIDTH/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),
								middle_font->maxheight+2+(i+5)*HIGHSCORE_HEIGHT/4+HIGHSCORE_GAP_Y+HIGHSCORE_GAP_ABOVE+(spSin(menu_counter*300+3*SP_PI*2/7)>>SP_ACCURACY-2),0,
								HIGHSCORE_WIDTH-2*HIGHSCORE_GAP_IN,engineWindowY/128,color);
				}			
				spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),30*engineWindowY/32+(spSin(menu_counter*300+2*SP_PI*2/7)>>SP_ACCURACY-2),-1,spGetTranslationFromCaption(menu_bundle,"Any button: back"),small_font);
			}
			break;

		case 4: //Help
			/*spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),1*engineWindowY/9+(spSin(menu_counter*300+6*SP_PI*2/7)>>SP_ACCURACY-2),-1,"ABOUT",font);
			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),2*engineWindowY/9+(spSin(menu_counter*300+5*SP_PI*2/7)>>SP_ACCURACY-2),-1,"Code Monkey: Ziz",font);
			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),3*engineWindowY/9+(spSin(menu_counter*300+4*SP_PI*2/7)>>SP_ACCURACY-2),-1,"Sound: Nick May",font);
			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),4*engineWindowY/9+(spSin(menu_counter*300+3*SP_PI*2/7)>>SP_ACCURACY-2),-1,"Backers of \"RIOTdigital\":",font);
			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),5*engineWindowY/9+(spSin(menu_counter*300+2*SP_PI*2/7)>>SP_ACCURACY-2),-1,"Pandora",font);
			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),6*engineWindowY/9+(spSin(menu_counter*300+1*SP_PI*2/7)>>SP_ACCURACY-2),-1,"GPH",font);
			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),7*engineWindowY/9+(spSin(menu_counter*300+0*SP_PI*2/7)>>SP_ACCURACY-2),-1,"DINGOO DIGITAL USA",font);
			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),8*engineWindowY/9+(spSin(menu_counter*300+-1*SP_PI*2/7)>>SP_ACCURACY-2),-1,"Etronixmart",font);*/
			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2), 1*engineWindowY/18+(spSin(menu_counter*300+15*SP_PI*1/7)>>SP_ACCURACY-1),-1,"HOW TO PLAY",font);
			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2), 3*engineWindowY/18+(spSin(menu_counter*300+14*SP_PI*1/7)>>SP_ACCURACY-1),-1,"Puzzletube	has	3	game	modes.	The	game	modes	are",middle_font);
			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2), 4*engineWindowY/18+(spSin(menu_counter*300+13*SP_PI*1/7)>>SP_ACCURACY-1),-1,"explained	at	game	start.	In	every	mode	you	have",middle_font);
			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2), 5*engineWindowY/18+(spSin(menu_counter*300+12*SP_PI*1/7)>>SP_ACCURACY-1),-1,"to	crush	stones	to	get	points	or	time.	A	square",middle_font);
			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2), 6*engineWindowY/18+(spSin(menu_counter*300+11*SP_PI*1/7)>>SP_ACCURACY-1),-1,"of	stones	of	the	same	color	explodes.	If	a	stone",middle_font);
			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2), 7*engineWindowY/18+(spSin(menu_counter*300+10*SP_PI*1/7)>>SP_ACCURACY-1),-1,"explodes	&	a	stone	beside	has	the	same	color,	it",middle_font);
			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2), 8*engineWindowY/18+(spSin(menu_counter*300+ 9*SP_PI*1/7)>>SP_ACCURACY-1),-1,"explodes,	too.	The	more	stone	you	crush	the	more",middle_font);
			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2), 9*engineWindowY/18+(spSin(menu_counter*300+ 8*SP_PI*1/7)>>SP_ACCURACY-1),-1,"Points	you	get.	If	a	stone	of	the	same	color	is",middle_font);
			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),10*engineWindowY/18+(spSin(menu_counter*300+ 7*SP_PI*1/7)>>SP_ACCURACY-1),-1,"one	the	other	side	it	wiggles	and	will	explode,",middle_font);
			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),11*engineWindowY/18+(spSin(menu_counter*300+ 6*SP_PI*1/7)>>SP_ACCURACY-1),-1,"too.	To	get	squares	of	the	same	color	you	have",middle_font);
			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),12*engineWindowY/18+(spSin(menu_counter*300+ 5*SP_PI*1/7)>>SP_ACCURACY-1),-1,"3	controls	adjustable	in	the	settings	menu.	In",middle_font);
			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),13*engineWindowY/18+(spSin(menu_counter*300+ 4*SP_PI*1/7)>>SP_ACCURACY-1),-1,"normal	control	you	move	a	selection	&	are	able",middle_font);
			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),14*engineWindowY/18+(spSin(menu_counter*300+ 3*SP_PI*1/7)>>SP_ACCURACY-1),-1,"to	switch	2	stone	beside	with	pressing	the	named",middle_font);
			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),15*engineWindowY/18+(spSin(menu_counter*300+ 2*SP_PI*1/7)>>SP_ACCURACY-1),-1,"buttons.	For	the	others	mode	read	the	README.",middle_font);
			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),16*engineWindowY/18+(spSin(menu_counter*300+ 1*SP_PI*1/7)>>SP_ACCURACY-1),-1,"Made	by	Nick	May	(Music)	&	Ziz	(Code).",middle_font);
			spFontDrawMiddle(engineWindowX/2+(menu_fade*spGetSizeFactor()>>SP_ACCURACY+2),17*engineWindowY/18+(spSin(menu_counter*300+ 0*SP_PI*1/7)>>SP_ACCURACY-1),-1,"Any button: back",small_font);
			break;
	}
	#ifdef MENU_DEBUG
		printf("%i:	 Drew menu\n",SDL_GetTicks());
	#endif

	//sprintf(buffer,"fps %i",spGetFPS());
	//spFontDrawRight(engineWindowX,engineWindowY-small_font->maxheight,-1,buffer,small_font);
	spFontDrawRight(engineWindowX-1,engineWindowY-small_font->maxheight,-1,"made by Ziz",small_font);
	#ifdef MENU_DEBUG
		printf("%i:	 Drew fps\n",SDL_GetTicks());
	#endif
	spFlip();
}

int calc_menu(Uint32 steps)
{
	menu_counter += steps;
	if (settings_get_stars_rotating()==2)
		star_counter += steps;
	calc_music(steps);
	PspInput engineInput = spGetInput();
	if (menu_fade>0)
	{
		menu_fade-=steps*3;
		if (menu_fade<=0) {
			menu_fade = 0;
			rotating_sound_off();
		}
		return 0;
	}
	if (menu_fade<0)
	{
		menu_fade-=steps*3;
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
				case -2: //Free Game
					prepare_game_objects(0);
					rotating_sound_off();
					settings_save();
					star_counter = run_game(1,star_counter*10,menu_resize)/10;
					engineInput->button[SP_BUTTON_START] = 0;
					engineInput->button[SP_BUTTON_A] = 0;
					engineInput->button[SP_BUTTON_B] = 0;
					engineInput->button[SP_BUTTON_X] = 0;
					engineInput->button[SP_BUTTON_Y] = 0;
					state = 0;
					break;
				case -4:
					rotating_sound_off();
					return 1;
			}
		}
		return 0;
	}

	int i;
	
	switch (state)
	{
	case 0: //menu
		if (engineInput->axis[1]>0 && menu_move==0 && (menu_choice>>SP_ACCURACY)<4)
			menu_move = 1;
		if (engineInput->axis[1]<0 && menu_move==0 && (menu_choice>>SP_ACCURACY)>0)
			menu_move = -1;
		
		if (menu_move != 0)
		{
			move_sound_on();
			for (i=0;i<steps;i++)
			{
				menu_choice += menu_move*4 << SP_ACCURACY-10;
				if (menu_choice == ((menu_choice >>SP_ACCURACY)<<SP_ACCURACY))
					menu_move = 0;
			}
		}
		else
			move_sound_off();
		if (menu_move == 0 && (engineInput->button[SP_BUTTON_START] ||
				engineInput->button[SP_BUTTON_A] || engineInput->button[SP_BUTTON_B] ||
				engineInput->button[SP_BUTTON_X] || engineInput->button[SP_BUTTON_Y]))
		{
			move_sound_off();
			switch (menu_choice>>SP_ACCURACY)
			{
				case 0: // Free Game
					nextstate = 2;
					menu_fade = -1;
					rotating_sound_on();
					break;
				case 2: // Highscore
					nextstate = 3;
					menu_fade = -1;
					rotating_sound_on();
					break;
				case 3: // Options
					nextstate = 1;
					menu_fade = -1;
					rotating_sound_on();
					break;
				case 1: // Help
					nextstate = 4;
					menu_fade = -1;
					rotating_sound_on();
					break;
				case 4: // Quit
					nextstate = -4;
					menu_fade = -1;
					rotating_sound_on();
					break;
			}
		}
		break;
	case 1: //options
		if (engineInput->axis[1]>0 && menu_move==0 && (menu_choice>>SP_ACCURACY)<6)
			menu_move = 1;
		if (engineInput->axis[1]<0 && menu_move==0 && (menu_choice>>SP_ACCURACY)>0)
			menu_move = -1;
		if (menu_move != 0)
		{
			move_sound_on();
			for (i=0;i<steps;i++)
			{
				menu_choice += menu_move*4 << SP_ACCURACY-10;
				if (menu_choice == ((menu_choice >>SP_ACCURACY)<<SP_ACCURACY))
				if ((menu_choice>>SP_ACCURACY) != 3)
					menu_move = 0;
			}
		}
		else
			move_sound_off();
		for (i = 0;i < steps;i++)
		{
			menu_wait--;
			if (menu_move == 0 && (menu_choice>>SP_ACCURACY) == 5 && engineInput->axis[0]<0 && menu_wait <= 0 && settings_get_volume()>0)
			{
				settings_set_volume(settings_get_volume()-1);
				set_volume(settings_get_volume());
				menu_wait = 25;
			}
			if (menu_move == 0 && (menu_choice>>SP_ACCURACY) == 5 && engineInput->axis[0]>0 && menu_wait <= 0 && settings_get_volume()<100)
			{
				settings_set_volume(settings_get_volume()+1);
				set_volume(settings_get_volume());
				menu_wait = 25;
			}
		}
		if (!menu_block && menu_move == 0 && (menu_choice>>SP_ACCURACY) == 0 && engineInput->axis[0]<0 && menu_wait <= 0 && settings_get_stone_quality()>0)
		{
			settings_set_stone_quality(settings_get_stone_quality()-1);
			menu_block = 1;
		}
		if (!menu_block && menu_move == 0 && (menu_choice>>SP_ACCURACY) == 0 && engineInput->axis[0]>0 && menu_wait <= 0 && settings_get_stone_quality()<2)
		{
			settings_set_stone_quality(settings_get_stone_quality()+1);
			menu_block = 1;
		}
		if (!menu_block && menu_move == 0 && (menu_choice>>SP_ACCURACY) == 1 && engineInput->axis[0]<0 && menu_wait <= 0 && settings_get_stars_rotating()>0)
		{
			settings_set_stars_rotating(settings_get_stars_rotating()-1);
			menu_block = 1;
		}
		if (!menu_block && menu_move == 0 && (menu_choice>>SP_ACCURACY) == 1 && engineInput->axis[0]>0 && menu_wait <= 0 && settings_get_stars_rotating()<2)
		{
			settings_set_stars_rotating(settings_get_stars_rotating()+1);
			menu_block = 1;
		}
		/*if (!menu_block && menu_move == 0 && (menu_choice>>SP_ACCURACY) == 2 && engineInput->axis[0]<0 && menu_wait <= 0 && settings_get_particles()>0)
		{
			settings_set_particles(settings_get_particles()-1);
			menu_block = 1;
		}
		if (!menu_block && menu_move == 0 && (menu_choice>>SP_ACCURACY) == 2 && engineInput->axis[0]>0 && menu_wait <= 0 && settings_get_particles()<1)
		{
			settings_set_particles(settings_get_particles()+1);
			menu_block = 1;
		}*/
		if (!menu_block && menu_move == 0 && (menu_choice>>SP_ACCURACY) == 2 && engineInput->axis[0]<0 && menu_wait <= 0 && settings_get_control()>0)
		{
			settings_set_control(settings_get_control()-1);
			menu_block = 1;
		}
		if (!menu_block && menu_move == 0 && (menu_choice>>SP_ACCURACY) == 2 && engineInput->axis[0]>0 && menu_wait <= 0 && settings_get_control()<2)
		{
			settings_set_control(settings_get_control()+1);
			menu_block = 1;
		}
		if (!menu_block && menu_move == 0 && (menu_choice>>SP_ACCURACY) == 4 && engineInput->axis[0]<0 && menu_wait <= 0)
		{
			settings_set_language((settings_get_language()+spGetPossibleLanguagesCount()-1)%spGetPossibleLanguagesCount());
			spDeleteSurface(flag);
			flag = spLoadSurface(spGetTranslationFromCaption(menu_bundle,"flag.png"));
			menu_block = 1;
		}
		if (!menu_block && menu_move == 0 && (menu_choice>>SP_ACCURACY) == 4 && engineInput->axis[0]>0 && menu_wait <= 0)
		{
			settings_set_language((settings_get_language()+1)%spGetPossibleLanguagesCount());
			spDeleteSurface(flag);
			flag = spLoadSurface(spGetTranslationFromCaption(menu_bundle,"flag.png"));
			menu_block = 1;
		}
		
		if (engineInput->axis[0] == 0)
			menu_block = 0;
		if (engineInput->button[SP_BUTTON_START] || engineInput->button[SP_BUTTON_SELECT])
		{
			engineInput->button[SP_BUTTON_START] = 0;
			engineInput->button[SP_BUTTON_SELECT] = 0;
			//Back:
			settings_save();
			nextstate = 0;
			menu_fade = -1;
			rotating_sound_on();			
		}
		if (menu_move == 0 &&
			 (engineInput->button[SP_BUTTON_A] || engineInput->button[SP_BUTTON_B] ||
				engineInput->button[SP_BUTTON_X] || engineInput->button[SP_BUTTON_Y]))
		{
			engineInput->button[SP_BUTTON_A] = 0;
			engineInput->button[SP_BUTTON_B] = 0;
			engineInput->button[SP_BUTTON_X] = 0;
			engineInput->button[SP_BUTTON_Y] = 0;
			switch (menu_choice>>SP_ACCURACY)
			{
				case 0: //Stone Quality
					settings_set_stone_quality((settings_get_stone_quality()+1)%3);
					break;
				case 1: //Stars Rotating
					settings_set_stars_rotating((settings_get_stars_rotating()+1)%3);
					break;
				/*case 2: //Particles
					settings_set_particles((settings_get_particles()+1)%2);
					break;*/
				case 2: //Control
					settings_set_control((settings_get_control()+1)%3);
					break;
				case 4: //Language
					settings_set_language((settings_get_language()+1)%spGetPossibleLanguagesCount());
					spDeleteSurface(flag);
					flag = spLoadSurface(spGetTranslationFromCaption(menu_bundle,"flag.png"));
					break;
				case 6: //Back
					settings_save();
					nextstate = 0;
					menu_fade = -1;
					rotating_sound_on();
					break;
			}
		}
		break;
case 2: //free game
		if (engineInput->axis[1]>0 && menu_move==0 && (menu_choice>>SP_ACCURACY)<4)
			menu_move = 1;
		if (engineInput->axis[1]<0 && menu_move==0 && (menu_choice>>SP_ACCURACY)>0)
			menu_move = -1;
		
		if (menu_move != 0)
		{
			move_sound_on();
			for (i=0;i<steps;i++)
			{
				menu_choice += menu_move*4 << SP_ACCURACY-10;
				if (menu_choice == ((menu_choice >>SP_ACCURACY)<<SP_ACCURACY))
					menu_move = 0;
			}
		}
		else
			move_sound_off();
		if (!menu_block && menu_move == 0 && (menu_choice>>SP_ACCURACY) == 1 && engineInput->axis[0]>0 && menu_wait <= 0)
		{
			settings_set_mode((settings_get_mode()+1)%3);
			menu_block = 1;
		}
		if (!menu_block && menu_move == 0 && (menu_choice>>SP_ACCURACY) == 1 && engineInput->axis[0]<0 && menu_wait <= 0)
		{
			settings_set_mode((settings_get_mode()+2)%3);
			menu_block = 1;
		}
		if (!menu_block && menu_move == 0 && (menu_choice>>SP_ACCURACY) == 2 && engineInput->axis[0]<0 && menu_wait <= 0 && settings_get_color()>0)
		{
			settings_set_color(settings_get_color()-1);
			menu_block = 1;
		}
		if (!menu_block && menu_move == 0 && (menu_choice>>SP_ACCURACY) == 2 && engineInput->axis[0]>0 && menu_wait <= 0 && settings_get_color()<1)
		{
			settings_set_color(settings_get_color()+1);
			menu_block = 1;
		}
		if (engineInput->axis[0] == 0)
			menu_block = 0;
		if (menu_move == 0 && (engineInput->button[SP_BUTTON_START] ||
				engineInput->button[SP_BUTTON_A] || engineInput->button[SP_BUTTON_B] ||
				engineInput->button[SP_BUTTON_X] || engineInput->button[SP_BUTTON_Y]))
		{
			engineInput->button[SP_BUTTON_START] = 0;
			engineInput->button[SP_BUTTON_A] = 0;
			engineInput->button[SP_BUTTON_B] = 0;
			engineInput->button[SP_BUTTON_X] = 0;
			engineInput->button[SP_BUTTON_Y] = 0;
			switch (menu_choice>>SP_ACCURACY)
			{
				case 0: //Play
					settings_save();
					nextstate = -2;
					menu_fade = -1;
					rotating_sound_on();
					break;
				case 1: //Game Mode
					settings_set_mode((settings_get_mode()+1)%3);
					break;
				case 2: //colors
					settings_set_color((settings_get_color()+1)%2);
					break;
				case 4: //Back
					nextstate = 0;
					menu_fade = -1;
					rotating_sound_on();
					break;
			}
		}
		break;
	case 3: //high score
		if (engineInput->button[SP_BUTTON_START] || engineInput->button[SP_BUTTON_SELECT] ||
				engineInput->button[SP_BUTTON_A] || engineInput->button[SP_BUTTON_B] ||
				engineInput->button[SP_BUTTON_X] || engineInput->button[SP_BUTTON_Y])
		{
			engineInput->button[SP_BUTTON_START] = 0;
			engineInput->button[SP_BUTTON_SELECT] = 0;
			engineInput->button[SP_BUTTON_A] = 0;
			engineInput->button[SP_BUTTON_B] = 0;
			engineInput->button[SP_BUTTON_X] = 0;
			engineInput->button[SP_BUTTON_Y] = 0;
			nextstate = 0;
			menu_fade = -1;
			rotating_sound_on();
		}
		break;
	case 4: //about
		if (engineInput->button[SP_BUTTON_START] ||
				engineInput->button[SP_BUTTON_A] || engineInput->button[SP_BUTTON_B] ||
				engineInput->button[SP_BUTTON_X] || engineInput->button[SP_BUTTON_Y])
		{
			engineInput->button[SP_BUTTON_START] = 0;
			engineInput->button[SP_BUTTON_A] = 0;
			engineInput->button[SP_BUTTON_B] = 0;
			engineInput->button[SP_BUTTON_X] = 0;
			engineInput->button[SP_BUTTON_Y] = 0;
			nextstate = 0;
			menu_fade = -1;
			rotating_sound_on();
		}
		break;
	}
	return 0;
}

void run_menu(void (*resize)(Uint16 w,Uint16 h))
{
	spReadPossibleLanguages("./translations/languages.txt");
	menu_bundle = spLoadBundle("./translations/translations.txt",1);
	menu_fade = MENUSIZE;
	spSetDefaultLanguage(spGetPossibleLanguage(settings_get_language()));	
	flag = spLoadSurface(spGetTranslationFromCaption(menu_bundle,"flag.png"));
	if (settings_get_first_start())
	{
		menu_choice = 1<<SP_ACCURACY;
		settings_save();
	}
	else
		menu_choice = 0;
	rotating_sound_on();
	menu_resize = resize;
	spLoop(draw_menu,calc_menu,10,resize,NULL);
	spDeleteBundle(menu_bundle,0);
	spDeleteSurface(flag);
}

