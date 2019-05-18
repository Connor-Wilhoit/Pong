#include <U8g2lib.h>
#include "pitches.h"

/*  active buzzer setup  */
const int buzzer1 = 4;	// to be used for certain game outcomes
const int buzzer2 = 2;

/*  array for creating musical notes  */
const int notes[89] = {
	n_B0, C1, CS1, D1, DS1, E1, F1, FS1, G1, GS1, n_A1, n_AS1,
	n_B1, C2, CS2, D2, DS2, E2, F2, FS2, G2, GS2, n_A2, n_AS2,
	  B2, C3, CS3, D3, DS3, E3, F3, FS3, G3, GS3, n_A3, n_AS3,
	  B3, C4, CS4, D4, DS4, E4, F4, FS4, G4, GS4, n_A4, n_AS4,
	  B4, C5, CS5, D5, DS5, E5, F5, FS5, G5, GS5, n_A5, n_AS5,
	  B5, C6, CS6, D6, DS6, E6, F6, FS6, G6, GS6, n_A6, n_AS6,
	  B6, C7, CS7, D7, DS7, E7, F7, FS7, G7, GS7, n_A7, n_AS7,
	  B7, C8, CS8, D8, DS8
};

/*  array for the duration of the tones played by buzzer      */
/*  used in conjunction with the array located above 'notes'  */
const int durations[89] = {	8, 16, 16, 8, 8, 8, 8, 8,
							8, 16, 16, 8, 8, 8, 8, 8,
							8, 16, 16, 8, 8, 8, 8, 8,
							8, 16, 16, 8, 8, 8, 8, 8,
							8, 16, 16, 8, 8, 8, 8, 8,
							8, 16, 16, 8, 8, 8, 8, 8,
							8, 16, 16, 8, 8, 8, 8, 8,
							8, 16, 16, 8, 8, 8, 8, 8,
							8, 16, 16, 8, 8, 8, 8, 8,
							8, 16, 16, 8, 8, 8, 8, 8,
							8, 16, 16, 8, 8, 8, 8, 8,
							8
};

/*  Here are constants for the screen's boundaries  */
const int MAX_SCREEN_WIDTH 	= 128;	// furthest to the right, largest x-value
const int MAX_SCREEN_HEIGHT	= 64;	// lowest part of the screen, largest y-value
const int MIN_SCREEN_WIDTH 	= 0;	// furthest to the left, smallest x-value
const int MIN_SCREEN_HEIGHT	= 0;// upper-most part of the screen, smallest y-value

/*   that the x-values are logical (smallest of left, largest on right) just
 *  as when doing algebra and using a coordinate-plane. However, the y-values are
 *  reversed from what logically makes the most sense; the largest y-value is
 *  actually the bottom, and the smallest the top (reverse from an algebraic
 *  coordinate-plane)
 */

/*  here is the ball's (moving-pixel) variables  */
int x_coordinate  		  = 0;
int y_coordinate  		  = 0;
int last_x_coordinate   = x_coordinate;
int last_y_coordinate   = y_coordinate;

/*  gamespeed variables, used by the delay function call after drawing pixels  */
int default_gamespeed 	= 15;						// default value
//int hard			  	    = 5;						// difficulty level = hard
//int easy			  	    = 25;						// difficulty level = easy
int medium			  	    = default_gamespeed;		// difficulty level = medium

/*  player's scoring variables  */
int player1_score		= 0;		// left-paddle's score count; y-paddle-variable
int player2_score		= 0;		// right-paddle's score count; z-paddle-variable
int max_score			  = 200;		// if a player reaches this score, they win!

/*  OLED-screen setup   */
const int clk   = 9;
const int mosi  = 10;
const int res   = 11;
const int dc    = 12;
const int cs    = 13;
U8G2_SH1106_128X64_NONAME_2_4W_SW_SPI u8g2(U8G2_R0, clk, mosi, cs, dc, res);

/*  potentiometer (pot) setup   */
const int left_pot  = A0;      // value read from potentiometer - 1 (left-side)
const int right_pot	= A5;	    // value read from potentiometer - 2 (right-side)

int left_paddle  = 0;	// value to transform left_pot  value into useable integer
int right_paddle = 0;	// value to transform right_pot. value into useable integer

int y	 	  = 0;	// value to write out to place the left_paddle using 'map'
int z	  	= 0;	// value to write out to place the right_paddle using 'map'

void setup(void)
{
	pinMode(buzzer1, OUTPUT);
	pinMode(buzzer2, OUTPUT);
	Serial.begin(115200);
	u8g2.begin();
	u8g2.setContrast(255);
	u8g2.setFont(u8g2_font_profont11_tr);
	u8g2.firstPage();
	do
	{
		u8g2.drawStr(20, 20, "First Player");
		u8g2.drawStr(15, 40, "to 200 points wins!");
	} while(u8g2.nextPage());
	delay(1900);
	u8g2.firstPage();
	do
	{
		u8g2.drawStr(60, 30, "Ready");
	} while(u8g2.nextPage());
	delay(900);
	u8g2.firstPage();
	do
	{
		u8g2.drawStr(60, 30, "Player");
	} while(u8g2.nextPage());
	delay(900);
	u8g2.firstPage();
	do
	{
		u8g2.drawStr(60, 30, "One!");
	} while(u8g2.nextPage());
	delay(900);
}

void play_song(void)
{
	for (int i=0; i < 89; ++i)
	{
		int note_duration = 1000 / durations[i];
		tone(buzzer1, notes[i], note_duration);
		int pause = note_duration * 1.30;
		delay(pause);
	}
}

void draw_dice(void)
{
	u8g2.clear();
	u8g2.setFont(u8g2_font_unifont_t_symbols);
	u8g2.firstPage();
	do
	{
		u8g2.drawGlyph(5, 45, 0x2680);
		u8g2.drawGlyph(15, 45, 0x2681);
		u8g2.drawGlyph(25, 45, 0x2682);
		u8g2.drawGlyph(35, 45, 0x2683);
		u8g2.drawGlyph(45, 45, 0x2684);
		u8g2.drawGlyph(55, 45, 0x2685);
		delay(4000);
	} while ( u8g2.nextPage() );
}


void init_pong(void)
{
	player1_score = 0;
	player2_score = 0;
	u8g2.setFont(u8g2_font_u8glib_4_tr);
	u8g2.firstPage();
	x_coordinate = 64;
	y_coordinate = 32;
	last_x_coordinate = x_coordinate;
	last_y_coordinate = y_coordinate;// only in this function does this ordering work
	do
	{
		u8g2.drawPixel(x_coordinate, y_coordinate);
		draw_paddles();
	} while(u8g2.nextPage());
}

void draw_paddles(void)
{
	left_paddle  = analogRead(left_pot);
	right_paddle = analogRead(right_pot);
	y = map(left_paddle,  0, 1023, 0, 64);
	z = map(right_paddle, 0, 1023, 0, 64);
	u8g2.drawBox(3, y, 5, 15);
	u8g2.drawBox(123, z, 5, 15);
	u8g2.setCursor(3, 60);
	u8g2.print(y);
	u8g2.setCursor(18, 60);
	u8g2.print(player1_score);
	u8g2.setCursor(118, 60);
	u8g2.print(z);
	u8g2.setCursor(105, 60);
	u8g2.print(player2_score);
}
void go_right_up_diagonal(void)
{
	last_x_coordinate = x_coordinate;
	last_y_coordinate = y_coordinate;
	x_coordinate += 2;
	y_coordinate -= 1;
	u8g2.setFont(u8g2_font_u8glib_4_tr);
	u8g2.firstPage();
	do
	{
		y_coordinate -= 1;
		u8g2.drawPixel(x_coordinate, y_coordinate);
		draw_paddles();
		Serial.print(x_coordinate);
		Serial.print("\t");
		Serial.println(y_coordinate);
	} while(u8g2.nextPage());
}

void go_right_down_diagonal(void)
{
	last_x_coordinate = x_coordinate;
	last_y_coordinate = y_coordinate;
	x_coordinate += 2;
	y_coordinate += 1;
	u8g2.setFont(u8g2_font_u8glib_4_tr);
	u8g2.firstPage();
	do
	{
		u8g2.drawPixel(x_coordinate, y_coordinate);
		draw_paddles();
		Serial.print(x_coordinate);
		Serial.print("\t");
		Serial.println(y_coordinate);
	} while(u8g2.nextPage());
}

void go_left_up_diagonal(void)
{
	last_x_coordinate = x_coordinate;
	last_y_coordinate = y_coordinate;
	x_coordinate -= 2;
	y_coordinate -= 1;
	u8g2.setFont(u8g2_font_u8glib_4_tr);
	u8g2.firstPage();
	do
	{
		u8g2.drawPixel(x_coordinate, y_coordinate);
		draw_paddles();
		Serial.print(x_coordinate);
		Serial.print("\t");
		Serial.println(y_coordinate);
	} while(u8g2.nextPage());
}

void go_left_down_diagonal(void)
{
	last_x_coordinate = x_coordinate;
	last_y_coordinate = y_coordinate;
	x_coordinate -= 2;
	y_coordinate += 1;
	u8g2.setFont(u8g2_font_u8glib_4_tr);
	u8g2.firstPage();
	do
	{
		u8g2.drawPixel(x_coordinate, y_coordinate);
		draw_paddles();
		Serial.print(x_coordinate);
		Serial.print("\t");
		Serial.println(y_coordinate);
	} while(u8g2.nextPage());
}

void go_up(void)	// no bounds checking for the generic directional-functions
{
	last_x_coordinate = x_coordinate;
	last_y_coordinate = y_coordinate;
	y_coordinate--;		// decremented, because y=0 at top of screen 
	u8g2.setFont(u8g2_font_u8glib_4_tr);
	u8g2.firstPage();
	do
	{
		u8g2.drawPixel(x_coordinate, y_coordinate);
		draw_paddles();
		Serial.print(x_coordinate);
		Serial.print("\t");
		Serial.println(y_coordinate);
	} while(u8g2.nextPage());
}

void go_down(void)		// no bounds checking for the generic directional-functions
{
	last_x_coordinate = x_coordinate;
	last_y_coordinate = y_coordinate;
	y_coordinate++;		                // incremented, because y=64 at bottom of screen
	u8g2.setFont(u8g2_font_u8glib_4_tr);
	u8g2.firstPage();
	do
	{
		u8g2.drawPixel(x_coordinate, y_coordinate);
		draw_paddles();
		Serial.print(x_coordinate);
		Serial.print("\t");
		Serial.println(y_coordinate);
	} while(u8g2.nextPage());
}

void go_right(void)		// no bounds checking for the generic directional-functions
{
	last_x_coordinate = x_coordinate;
	last_y_coordinate = y_coordinate;
	x_coordinate++;
	u8g2.setFont(u8g2_font_u8glib_4_tr);
	u8g2.firstPage();
	do
	{
		u8g2.drawPixel(x_coordinate, y_coordinate);
		draw_paddles();
		Serial.print(x_coordinate);
		Serial.print("\t");
		Serial.println(y_coordinate);
	} while(u8g2.nextPage());
}

void go_left(void)		// no bounds checking for the generic directional-functions
{
	last_x_coordinate = x_coordinate;
	last_y_coordinate = y_coordinate;
	x_coordinate--;
	u8g2.setFont(u8g2_font_u8glib_4_tr);
	u8g2.firstPage();
	do
	{
		u8g2.drawPixel(x_coordinate, y_coordinate);
		draw_paddles();
		Serial.print(x_coordinate);
		Serial.print("\t");
		Serial.println(y_coordinate);
	} while(u8g2.nextPage());
}

bool detect_left_paddle(void)
{
	int y01		= y-1;
	int y_val   = y;
	int max_y   = y+14;
	int y1		= y+1;
	int y2		= y+2;
	int y3		= y+3;
	int y4		= y+4;
	int y5		= y+5;
	int y6		= y+6;
	int y7		= y+7;
	int y8		= y+8;
	int y9		= y+9;
	int y10		= y+10;
	int y11		= y+11;
	int y12		= y+12;
	int y13		= y+13;
	if( ( x_coordinate == 9 || x_coordinate == 8 || x_coordinate == 7 ) &&
	  ( ( y_coordinate == y_val || y_coordinate == y1 || y_coordinate == y01 ||
		  y_coordinate == y2 || y_coordinate == y3 || y_coordinate == y4 	 ||
		  y_coordinate == y5 || y_coordinate == y6 || y_coordinate == y7 	 ||
		  y_coordinate == y8 || y_coordinate == y9 || y_coordinate == y10	 ||
		  y_coordinate == y11|| y_coordinate == y12|| y_coordinate == y13	 ||
		  y_coordinate == max_y )) )
	{
		return true;
	}
	else
	  return false;
}


bool detect_right_paddle(void)
{
	int z01		= z-1;
	int z_val   = z;
	int max_z   = z+14;
	int z1		= z+1;
	int z2		= z+2;
	int z3		= z+3;
	int z4		= z+4;
	int z5		= z+5;
	int z6		= z+6;
	int z7		= z+7;
	int z8		= z+8;
	int z9		= z+9;
	int z10		= z+10;
	int z11		= z+11;
	int z12		= z+12;
	int z13		= z+13;

	if( ( x_coordinate == 121 || x_coordinate == 120 || x_coordinate == 119 ) && 
	  ( ( y_coordinate == z_val || y_coordinate == z1 || y_coordinate == z01 ||
		  y_coordinate == z2 || y_coordinate == z3 || y_coordinate == z4	 ||
		  y_coordinate == z5 || y_coordinate == z6 || y_coordinate == z7	 ||
		  y_coordinate == z8 || y_coordinate == z9 || y_coordinate == z10	 ||
		  y_coordinate == z11|| y_coordinate == z12|| y_coordinate == z13	 ||
		  y_coordinate == max_z)) )
	{
		return true;
	}
	else
		return false;	
}

void restart(void)
{
	init_pong();
	play_game();
}


void player1_wins(void)
{
	play_song();	
	u8g2.clear();
	u8g2.setFont(u8g2_font_profont11_tr);
	u8g2.firstPage();
	do
	{
		u8g2.drawStr(10, 25, "Player One Wins!");
		delay(1500);
		u8g2.drawStr(10, 35, "Player Two Sucks Ass!");
		delay(1500);
	} while ( u8g2.nextPage() );
	delay(1000);
	draw_dice();
	delay(1000);
}

void player2_wins(void)
{
	play_song();
	u8g2.clear();
	u8g2.setFont(u8g2_font_profont11_tr);
	u8g2.firstPage();
	do
	{
		u8g2.drawStr(10, 25, "Player Two Wins!");
		delay(1500);
		u8g2.drawStr(10, 35, "Player One Sucks Ass!");
		delay(1500);
	} while ( u8g2.nextPage() );
	delay(1000);
	draw_dice();
	delay(1000);
}

void check_for_winner(void)
{
	if ( player1_score == max_score )
	{
		player1_wins();
		restart();
	}
	
	if ( player2_score == max_score )
	{
		player2_wins();
		restart();
	}
}

void check_left_side(void)
{
	// group # [3] --> see whiteboard (orange numbers)
	// [ 3.1-3.3 --> handles hitting the left-paddle ]
	// [ 3.4-3.6 --> handles hitting the left-border ]
		// [3.0.0] --> added 8/19/20118 --> attempting to fix the bottom-left-corner bug
	if ( x_coordinate <= 0 && y_coordinate == 64 ) {
		player2_score += 1;
		while ( !detect_right_paddle() && x_coordinate < MAX_SCREEN_WIDTH &&
				y_coordinate > MIN_SCREEN_HEIGHT ) {
		go_right_up_diagonal();
		}
	}

	// [3.1.1] --> ball hits middle of left-paddle, with slope == 0 (lasty == y)
	else if ( detect_left_paddle() && y_coordinate == y+7 &&
		 y_coordinate == last_y_coordinate ) {
		while ( x_coordinate < MAX_SCREEN_WIDTH && x_coordinate > MIN_SCREEN_WIDTH ) {
		//go_right();
		// Last modified: 8/20/2018
		// I'm commenting out any 'go_left()'s and any 'go_right()'s, and chaning them
		// to a go_down_left/right_diagonal().
		// The reasoning behind this is that I seem to keep getting errors with (in
		// particular) the right-paddle, in that it doesn't use diagonal-functions
		// when it's supposed to; so for the time being, the "bug" fix is to just
		// manually pick an angling function
		go_right_down_diagonal();
		}
	}

	// [3.1.2] --> ball hits middle of left-paddle, coming from above
	else if ( detect_left_paddle() && y_coordinate == y+7 &&
			  y_coordinate > last_y_coordinate ) {
		while ( x_coordinate < MIN_SCREEN_WIDTH && y_coordinate < MAX_SCREEN_WIDTH ) {
		go_right_down_diagonal();
		}
	}

	// [3.1.3] --> ball hits middle of left-paddle, coming from below
	else if ( detect_right_paddle() && y_coordinate == y+7 &&
			  y_coordinate < last_y_coordinate ) {
		while ( x_coordinate < MIN_SCREEN_WIDTH && y_coordinate > MIN_SCREEN_HEIGHT ) {
		go_right_up_diagonal();
		}
	}

	// [3.2.1] --> ball hits top-half of left-paddle, coming from above
	else if ( detect_left_paddle() && y_coordinate <= y+6 &&
			  y_coordinate > last_y_coordinate ) {
		while ( !detect_right_paddle() && y_coordinate < MAX_SCREEN_HEIGHT &&
				x_coordinate < MAX_SCREEN_WIDTH ) {
			go_right_down_diagonal();            // ball will only be capable of hitting the top border in this situtation
			delay(15);
		}
	}

	// [3.2.2] --> ball hits top-half of left-paddle, coming from below
	else if ( detect_left_paddle() && y_coordinate <= y+6 &&
			  y_coordinate < last_y_coordinate ) {
		while ( !detect_right_paddle() && y_coordinate > MIN_SCREEN_HEIGHT &&
				x_coordinate < MAX_SCREEN_WIDTH ) {
			go_right_up_diagonal();
			delay(15);
		}
	}

	// [3.2.3] --> ball hits top-half of left-paddle, with slope == 0 (flat-line)
	else if ( detect_left_paddle() && y_coordinate <= y+6 &&
			  y_coordinate == last_y_coordinate ) {
		while ( !detect_right_paddle() && y_coordinate > MIN_SCREEN_HEIGHT &&
				x_coordinate < MAX_SCREEN_WIDTH ) {
			go_right_up_diagonal();		// this fixes the 'flat-line' bug
			delay(15);
		}
	    // this function will use a different angling function, written specifically for this case!
	}

	// [3.3.1] --> ball hits bottom-half of left-paddle, coming from above
	else if ( detect_left_paddle() && y_coordinate >= y+8 &&
			  y_coordinate > last_y_coordinate ) {
		while ( !detect_right_paddle() && y_coordinate < MAX_SCREEN_HEIGHT &&
				x_coordinate < MAX_SCREEN_WIDTH ) {
			go_right_down_diagonal();	           // ball will only be capable of hitting the bottom border in this situation
			delay(15);
		}
	}

	// [3.3.2] --> ball hits bottom-half of left-paddle, coming from below
	else if ( detect_left_paddle() && y_coordinate >= y+8 &&
			  y_coordinate < last_y_coordinate ) {
		while ( !detect_right_paddle() && y_coordinate > MIN_SCREEN_HEIGHT &&
				x_coordinate < MAX_SCREEN_WIDTH ) {
			go_right_up_diagonal();
			delay(15);
		}
	}

	// [3.3.3] --> ball hits bottom-half of left-paddle, with slope == 0 (flat-line)
	else if ( detect_left_paddle() && y_coordinate >= y+8 &&
			  y_coordinate == last_y_coordinate ) {
		while ( !detect_right_paddle() && y_coordinate < MAX_SCREEN_HEIGHT &&
				x_coordinate < MAX_SCREEN_WIDTH ) {
			go_right_down_diagonal();
			delay(15);
		}
	}

	// [3.4] --> ball hits the middle of the left border
	else if ( x_coordinate <= MIN_SCREEN_WIDTH &&
			  y_coordinate == (MAX_SCREEN_HEIGHT/2) ) {
		player2_score += 1;
		while ( !detect_right_paddle() && x_coordinate < MAX_SCREEN_WIDTH ) { 
			go_right();
			delay(15);
		}
	}

	// [3.5] --> ball hits top-half of left-border
	else if ( x_coordinate <= MIN_SCREEN_WIDTH &&
			  y_coordinate < (MAX_SCREEN_HEIGHT/2) ) {
		player2_score += 1;
		while ( y_coordinate > MIN_SCREEN_HEIGHT && x_coordinate < MAX_SCREEN_WIDTH ) {
			go_right_up_diagonal();	            // ball can only hit the top border in this case
			delay(15);
		}
	}

	// [3.6] --> ball hits the bottom-half of left-border
	else if ( x_coordinate <= MIN_SCREEN_WIDTH &&
			  y_coordinate > (MAX_SCREEN_HEIGHT/2) ) {
		player2_score += 1;
		while ( y_coordinate < MAX_SCREEN_HEIGHT && x_coordinate < MAX_SCREEN_WIDTH ) {
			go_right_down_diagonal();	            // ball can only hit the bottom border next in this scenario
			delay(15);
		}
	}
}

void check_top(void)
{
	// group # [4] --> see whiteboard (orange numbers)

	// [4.1.1] --> ball hits left-half of top-border, coming from the right
	if ( y_coordinate <= MIN_SCREEN_HEIGHT &&
		 x_coordinate < (MAX_SCREEN_WIDTH/2) &&
		 x_coordinate < last_x_coordinate ) {
		while ( !detect_left_paddle() && x_coordinate > MIN_SCREEN_WIDTH &&
				y_coordinate < MAX_SCREEN_HEIGHT ) {
			go_left_down_diagonal();
			/* ball will hit either the left-paddle, or
			 * the left-border; nothing else should be possible.
			 * Also, this needs to be run with both <= >= and < > bounds-checking,
			 * because I'm not 100% sure which should be used and when... :'(
			 */
			delay(15);
		}
	}

	// [4.1.2] --> ball hits left-half of top-border, coming from the left
	else if ( y_coordinate <= MIN_SCREEN_HEIGHT &&
			  x_coordinate < (MAX_SCREEN_WIDTH/2) &&
		 	  x_coordinate > last_x_coordinate ) {
		while ( !detect_right_paddle() && y_coordinate < MAX_SCREEN_HEIGHT &&
				x_coordinate < MAX_SCREEN_WIDTH ) {
			go_right_down_diagonal();	          // ball should not hit anything before it hits the bottom border, thus the simplified while-conditional
			delay(15);
		}
	}

	// [4.2.1] --> ball hits right-half of top-border, coming from the right
	else if ( y_coordinate <= MIN_SCREEN_HEIGHT &&
			  x_coordinate > (MAX_SCREEN_WIDTH/2) &&
			  x_coordinate < last_x_coordinate ) {
		while ( !detect_left_paddle() && y_coordinate < MAX_SCREEN_HEIGHT  && 
				x_coordinate > MIN_SCREEN_WIDTH ) {
			go_left_down_diagonal();
			/* ball will hit the bottom border before anything else, because the ball has just come from either the right-border,
      *  or right-paddle, and there's no way it should hit anything
			 * other than the bottom border; however, depending upon what half of
			 * the bottom it hits, that will change its future trajectory.
			 */
			delay(15);
		}
	} 

	// [4.2.2] --> ball hits right-half of top-border, coming from the left
	else if ( y_coordinate <= MIN_SCREEN_HEIGHT &&
			  x_coordinate > (MAX_SCREEN_WIDTH/2) &&
			  x_coordinate > last_x_coordinate ) {
		while ( !detect_right_paddle() && x_coordinate < MAX_SCREEN_WIDTH  && 
				y_coordinate < MAX_SCREEN_HEIGHT ) {
			go_right_down_diagonal();
			/* ball will either hit the right-paddle,
			 * or it will hit the right border; it should most likely hit the top-
			 * half of the right border, but it may not necessarily
			 */
			delay(15);
		}
	}

	// [4.3.1] --> ball hits the middle of top-border, coming from the right
	else if ( y_coordinate <= MIN_SCREEN_HEIGHT &&
			  x_coordinate == (MAX_SCREEN_WIDTH/2) &&
			  x_coordinate < last_x_coordinate ) {
		while ( !detect_left_paddle() &&
				y_coordinate < MAX_SCREEN_HEIGHT &&
				x_coordinate > MIN_SCREEN_WIDTH ) {
			go_left_down_diagonal();
			delay(15);
		}
	}

	// [4.3.2] --> ball hits the middle of top-border, coming from the left
	else if ( y_coordinate <= MIN_SCREEN_HEIGHT &&
			  x_coordinate == (MAX_SCREEN_WIDTH/2) &&
			  x_coordinate > last_x_coordinate ) {
		while( !detect_right_paddle() &&
				y_coordinate < MAX_SCREEN_HEIGHT &&
				x_coordinate < MAX_SCREEN_WIDTH ) {
			go_right_down_diagonal();
			delay(15);
		}
	}
}

void check_right_side(void)
{
	/* group # [6] --> see whiteboard (orange numbers)
	 * 8/17/2018 --> adding some functionality to when the ball hits either paddle in
	 * the middle.  Instead of just going left or right, we'll comment out those blocks,
	 * and attempt to add-in some angling (just like the other functions), and that 
	 * should allow better gameplay.
   */
   
	// [6.1.1] --> ball hits middle of right-paddle, where slope == 0 (lasty == y)
	if ( detect_right_paddle() && y_coordinate == z+7 &&
		 y_coordinate == last_y_coordinate ) {
		while ( !detect_left_paddle() && x_coordinate > MIN_SCREEN_WIDTH &&
				x_coordinate < MAX_SCREEN_WIDTH ) {
		// changing this from just 'go_left' to something custom, because 
		go_left();	// this conditional basically stays the same, only because there is no angle with which the ball hits the paddle
		}
	}

	// [6.1.2] --> ball hits middle of right-paddle, coming from above
	else if ( detect_right_paddle() && y_coordinate == z+7 &&
		 y_coordinate > last_y_coordinate ) {
		while ( y_coordinate < MAX_SCREEN_HEIGHT && x_coordinate > MIN_SCREEN_WIDTH ) {
		go_left_down_diagonal();
		}
	}

	// [6.1.3] --> ball hits middle of right-paddle, coming from below
	else if ( detect_right_paddle() && y_coordinate == z+7 &&
			  y_coordinate < last_y_coordinate ) {
		while ( y_coordinate > MIN_SCREEN_HEIGHT && x_coordinate > MIN_SCREEN_WIDTH ) {
		go_left_up_diagonal();
		}
	}

	// [6.2.1] --> ball hits top-half of right-paddle, coming from above
	else if ( detect_right_paddle() && y_coordinate <= z+6 &&
			  y_coordinate > last_y_coordinate ) {
		while ( !detect_left_paddle() && y_coordinate < MAX_SCREEN_HEIGHT &&
				x_coordinate > MIN_SCREEN_WIDTH ) {
			go_left_down_diagonal();
			delay(15);
		}
	}

	// [6.2.2] --> ball hits top-half of right-paddle, coming from below
	else if ( detect_right_paddle() && y_coordinate <= z+6 &&
			  y_coordinate < last_y_coordinate ) {
		while ( !detect_left_paddle() && y_coordinate > MIN_SCREEN_HEIGHT &&
				x_coordinate > MIN_SCREEN_WIDTH ) {
			go_left_up_diagonal();	// ball should very quickly hit the top, and richochet off towards the bottom/right
			delay(15);
		}
	}

	// [6.2.3] --> ball hits top-half of right-paddle, with slope == 0 (flat-lined)
	else if ( detect_right_paddle() && y_coordinate <= z+6 &&
			  y_coordinate == last_y_coordinate ) {
		while ( !detect_left_paddle() && y_coordinate > MIN_SCREEN_HEIGHT &&
				x_coordinate > MIN_SCREEN_WIDTH ) {
			go_left_up_diagonal();	// this fixes the 'flat-line' bug
			delay(15);
		}
	}

	// [6.3.1] --> ball hits bottom-half of right-paddle, coming from above
	else if ( detect_right_paddle() && y_coordinate >= z+8 &&
			  y_coordinate > last_y_coordinate ) {
		while ( !detect_left_paddle() && y_coordinate < MAX_SCREEN_HEIGHT && 
				x_coordinate > MIN_SCREEN_WIDTH ) {
			go_left_down_diagonal();	            // ball will only be able to hit the bottom border next, in this scenario
			delay(15);
		}
	}

	// [6.3.2] --> ball hits bottom-half of right-paddle, coming from below
	else if ( detect_right_paddle() && y_coordinate >= z+8 &&
			  y_coordinate < last_y_coordinate ) {
		while ( !detect_left_paddle() && y_coordinate > MIN_SCREEN_HEIGHT &&
				x_coordinate > MIN_SCREEN_WIDTH ) {
			go_left_up_diagonal();
			delay(15);
		}
	}

	// [6.3.3] --> ball hits bottom-half of right-paddle, with slope==0 (flat-lined)
	else if ( detect_right_paddle() && y_coordinate >= z+8 &&
			  y_coordinate == last_y_coordinate ) {
		while ( !detect_left_paddle() && y_coordinate < MAX_SCREEN_HEIGHT &&
				x_coordinate > MIN_SCREEN_WIDTH ) { 
			go_left_down_diagonal();	// this fixes the 'flat-line' bug
			delay(15);
		}
	}

	// [6.4] --> ball hits middle of right-border
	else if ( x_coordinate >= MAX_SCREEN_WIDTH &&
			  y_coordinate == (MAX_SCREEN_HEIGHT/2) ) {
		player1_score += 1;
		while ( x_coordinate > MIN_SCREEN_WIDTH && x_coordinate < MAX_SCREEN_WIDTH ) {
			go_left();	// ball will either be blocked by player1's paddle, or will hit the left-border, giving player2 a point!
			delay(15);
		}
	}

	// [6.5] --> ball hits top-half of right-border
	else if ( x_coordinate >= MAX_SCREEN_WIDTH &&
			  y_coordinate < (MAX_SCREEN_HEIGHT/2) ) {
		player1_score += 1;
		while ( !detect_left_paddle() && y_coordinate > MIN_SCREEN_HEIGHT &&
				x_coordinate > MIN_SCREEN_WIDTH ) {
			go_left_up_diagonal();	// ball should only be capable of hitting the top border next
			delay(15);
		}
	}

	// [6.6] --> ball hits bottom-half of right-border
	else if ( x_coordinate >= MAX_SCREEN_WIDTH &&
			  y_coordinate > (MAX_SCREEN_HEIGHT/2) ) {
		player1_score += 1;
		while ( !detect_left_paddle() && y_coordinate < MAX_SCREEN_HEIGHT &&
				x_coordinate > MIN_SCREEN_WIDTH ) {
			go_left_down_diagonal();	// ball should hit the bottom border next; I don't think there should be any exceptions on this one
			delay(15);
		}
	}
}

void check_bottom(void)
{
	// group # [5] --> see whiteboard (orange numbers)
	// [5.1.1] --> ball hits left-half of bottom-border, coming from the right
	if ( y_coordinate >= MAX_SCREEN_HEIGHT &&
		 x_coordinate < (MAX_SCREEN_WIDTH/2) &&
		 x_coordinate < last_x_coordinate ) {
		while ( !detect_left_paddle() && x_coordinate > MIN_SCREEN_WIDTH &&
				x_coordinate < MAX_SCREEN_WIDTH && y_coordinate < MAX_SCREEN_HEIGHT
			&& y_coordinate > MIN_SCREEN_HEIGHT ) {
			go_left_up_diagonal();
			/* ball will hit either the left-paddle, or it will
		   * hit somewhere on the left border; it should not be capable of hitting the
		   * top border, or anything else (from this conditional anyways)
			*/
			delay(15);
		}
	}

	// [5.1.2] --> ball hits left-half of bottom-border, coming from the left
	else if ( y_coordinate >= MAX_SCREEN_HEIGHT &&
			  x_coordinate < (MAX_SCREEN_WIDTH/2) &&
			  x_coordinate > last_x_coordinate ) {
		while ( !detect_right_paddle() && y_coordinate > MIN_SCREEN_HEIGHT
				&& x_coordinate < MAX_SCREEN_WIDTH ) {
			go_right_up_diagonal();	// depending upon where the ball hits, it could very well end up hitting any of the 3 possible "conditions"
			delay(15);
		}
	}

	// [5.2.1] --> ball hits right-half of bottom-border, coming from the right
	else if ( y_coordinate >= MAX_SCREEN_HEIGHT &&
			  x_coordinate > (MAX_SCREEN_WIDTH/2) &&
			  x_coordinate < last_x_coordinate ) {
		while ( !detect_left_paddle() && x_coordinate > MIN_SCREEN_WIDTH &&
				y_coordinate > MIN_SCREEN_HEIGHT  ) {
			go_left_up_diagonal();	                // this and the above conditional are ones where there are the most possible outcomes for the while-loop to handle
			delay(15);
		}
	}

	// [5.2.2] --> ball hits right-half of bottom-border, coming from the left
	else if ( y_coordinate >= MAX_SCREEN_HEIGHT &&
			  x_coordinate > (MAX_SCREEN_WIDTH/2) &&
			  x_coordinate > last_x_coordinate ) {
		while ( !detect_right_paddle() && x_coordinate < MAX_SCREEN_WIDTH &&
				y_coordinate > MIN_SCREEN_HEIGHT ) {
			go_right_up_diagonal();	// ball should hit either the right-paddle, or the right-border
			delay(15);
		}
	}

	// [5.3.1] -->  ball hits middle of bottom-border, coming from the right
	else if ( y_coordinate >= MAX_SCREEN_HEIGHT &&
			  x_coordinate == (MAX_SCREEN_WIDTH/2) &&
			  x_coordinate < last_x_coordinate ) {
		while ( !detect_left_paddle() &&
				x_coordinate > MIN_SCREEN_WIDTH &&
				y_coordinate > MIN_SCREEN_HEIGHT ) {
			go_left_up_diagonal();	               // ball will likely hit somewhere towards the upper-lefthand corner
			delay(15);
		}
	}

	// [5.3.2] --> ball hits middle of bottom-border, coming from the left
	else if ( y_coordinate >= MAX_SCREEN_HEIGHT &&
			  x_coordinate == (MAX_SCREEN_WIDTH/2) &&
			  x_coordinate > last_x_coordinate ) {
		while ( !detect_right_paddle() &&
				x_coordinate < MAX_SCREEN_WIDTH &&
				y_coordinate > MIN_SCREEN_HEIGHT ) {
			go_right_up_diagonal();	             // ball will likely hit somewhere towards the upper-righthand corner
			delay(15);
		}
	}

	/* [5.4.1] --> added 8/19/2018 --> this is to fix a particular bug, where the ball hits
	 *             the bottom border, and keeps going left without changing the y-coordinate.
	 *             the issue seems to arise when: x==63 and y==63, so (hopefully) this is one of the only
	 *             cases where the conditional statement is using specific x,y coordinates for what to do.
	 */
	else if ( y_coordinate == 63 && x_coordinate == 63 ) {
		while ( !detect_left_paddle() && x_coordinate > MIN_SCREEN_WIDTH &&
				y_coordinate < MIN_SCREEN_HEIGHT) {
		go_left_up_diagonal();
		}
	}
}

void play_game(void)
{ 
	/*  gameplay variable:
	 *	[1] --> player1_score (int)	<--- set to 0 (starting value)
	 *	[2] --> player2_score (int)	<--- set to 0 (starting value)
	 *	[3] --> max_score (int) 	<--- set to 200
	 *	The printing out of these variables are handled by
	 *  the 'draw_paddles' function
	 */

	// start off the movement!
	go_left();
	delay(medium);

	// Check to see if anything has occured on the left side:
	check_left_side();

	// Check to see if anything has occured at the bottom:
	check_bottom();

	// Check to see if anything has occured on the right side:
	check_right_side();

	// Check to see if anything has occured at the top:
	check_top();

	// Check to see if either player has reached the score limit:
	check_for_winner();

}

void loop(void)
{
	init_pong();

	while(1)
	{
	  play_game();
	}
}
