#include "raylib.h"
#include "raymath.h"
#include <math.h>

//==================== SHARED BY ALL LEVELS ====================
//difficulty settings, changed in the menu (chad is the default)
float obstacle_speed = 1;
float preview_seconds = 0.3;

//dotted line showing where a straight shot goes (no bounces), longer on easier difficulty
void draw_straight_preview(Vector2 ball, int aiming, float radius, float top_speed, float scale)
{
    if (preview_seconds<=0 || aiming==0 || IsMouseButtonDown(MOUSE_BUTTON_LEFT)==0) return;
    Vector2 mouse = {GetMouseX(),GetMouseY()};
    Vector2 drag = Vector2Subtract(ball,mouse);
    if (Vector2Length(drag)<2*radius) return;

    Vector2 shot = Vector2ClampValue(Vector2Scale(drag,3.125),0,top_speed);
    float shot_speed = Vector2Length(shot);
    Vector2 direction = Vector2Normalize(shot);
    float friction = 110*scale;

    //how far it rolls in the preview time, or until it stops
    float time = preview_seconds;
    if (shot_speed/friction<time) time = shot_speed/friction;
    float distance = shot_speed*time - 0.5*friction*time*time;

    for (int i=1; i<=12; i++)
    {
        Vector2 dot = Vector2Add(ball,Vector2Scale(direction,distance*i/12));
        DrawCircle(dot.x,dot.y,3*scale,Fade(WHITE,1-i/14.0));
    }
}

//the four original courses, one file each; the shell below owns everything they share
#include "levels/level1_foundry.c"
#include "levels/level2_shoreline.c"
#include "levels/level3_event_horizon.c"
#include "levels/level4_lost_temple.c"
#include "levels/level5_frozen_peak.c"
#include "levels/level6_haunted_manor.c"
#include "levels/level7_magma_core.c"

//==================== THE GAME: INTRO, MENU, DIFFICULTY ====================
int screen_width;
int screen_height;
float su = 1;
int game_mode = 0;          //0 intro, 1 menu, 2 playing, 3 briefing, 4 name, 5 how to play, 6 leaderboard, 7 credits
int level = 1;
int quit = 0;
int brief_page = 0;
int current_difficulty = 1;

//intro: the ball mascot hops through the 4 worlds (1.6s each), then the title drops in
Texture2D mascot_texture;
Texture2D developer_photo[2];     //the two faces on the credits page
float intro_time = 0;

//menu: 4 live level cards, then bot / chad / goat
#define LEVELS 7
RenderTexture2D card_texture[LEVELS];
int card_turn = 0;
int menu_page = 0;          //0 shows levels 1-4, 1 shows 5-7
int chosen_level = 0;       //0 picking a level, 1-4 picking the difficulty for that level
Color card_colour[LEVELS] = {{232,185,35,255},{63,180,207,255},{190,90,255,255},{120,200,90,255},
                            {160,214,240,255},{255,186,88,255},{255,106,31,255}};
const char *level_name[LEVELS] = {"FOUNDRY","SHORELINE","EVENT HORIZON","LOST TEMPLE",
                                  "FROZEN PEAK","HAUNTED MANOR","MAGMA CORE"};


//==================== THE PLAYER, THE SCORES AND THE SETTINGS ====================
//scores.txt sits next to the game: one line per player, NAME|level1|level2|level3|level4
//a level only writes a score when it is finished, and only when the score beats the old one
#define MAX_PLAYERS 50
char player_name[16] = "";
int name_length = 0;
int player_row = -1;              //which line of the table is this player, -1 = no line yet

char score_name[MAX_PLAYERS][16];
int score_points[MAX_PLAYERS][LEVELS];
int score_count = 0;

int sound_on = 1;              //the one switch for music AND sound effects
int score_saved = 0;              //the score for this run has already been written
int ui_press = 0;                 //a button on top of a level is held down, so no shot

int score_rate[3] = {10,25,50};        //points for every stroke you did NOT use
int score_bonus[3] = {100,250,500};    //for finishing the level at all


//the same sum for every level: what is left over, times the difficulty, plus the bonus
int score_for(int limit, int strokes, int difficulty)
{
    int left = limit - strokes;
    if (left<0) left = 0;
    return left*score_rate[difficulty] + score_bonus[difficulty];
}


int player_total(int row)
{
    int total = 0;
    for (int i=0; i<LEVELS; i++) total = total + score_points[row][i];
    return total;
}


//the level's own numbers, whichever level is being played
int level_state()
{
    if (level==1) return l1_game_state;
    if (level==2) return l2_game_state;
    if (level==3) return l3_game_state;
    if (level==4) return l4_game_state;
    if (level==5) return l5_game_state;
    if (level==6) return l6_game_state;
    return l7_game_state;
}


int level_strokes()
{
    if (level==1) return l1_stroke;
    if (level==2) return l2_stroke;
    if (level==3) return l3_stroke;
    if (level==4) return l4_stroke;
    if (level==5) return l5_stroke;
    if (level==6) return l6_stroke;
    return l7_stroke;
}


int level_limit()
{
    if (level==1) return l1_stroke_limit;
    if (level==2) return l2_stroke_limit;
    if (level==3) return l3_stroke_limit;
    if (level==4) return l4_stroke_limit;
    if (level==5) return l5_stroke_limit;
    if (level==6) return l6_stroke_limit;
    return l7_stroke_limit;
}


//one line out of the file
void read_score_line(const char *line)
{
    if (score_count>=MAX_PLAYERS) return;
    char name[16] = "";
    int points[LEVELS] = {0,0,0,0,0,0,0};
    int part = 0;
    int letters = 0;
    int number = 0;
    for (int i=0; ; i++)
    {
        char c = line[i];
        if (c=='|' || c==0)
        {
            if (part>0 && part<=LEVELS) points[part-1] = number;
            number = 0;
            part++;
            if (c==0) break;
        }
        else if (part==0)
        {
            if (letters<15)
            {
                name[letters] = c;
                letters++;
                name[letters] = 0;
            }
        }
        else if (c>='0' && c<='9') number = number*10 + (c-'0');
    }
    //a file written before the three new courses existed has fewer columns: the rest stay zero
    if (letters==0 || part<5) return;        //not a line this game wrote
    TextCopy(score_name[score_count],name);
    for (int i=0; i<LEVELS; i++) score_points[score_count][i] = points[i];
    score_count++;
}


void load_scores()
{
    score_count = 0;
    if (FileExists("scores.txt")==0) return;
    char *text = LoadFileText("scores.txt");
    if (text==0) return;
    char line[64] = "";
    int letters = 0;
    for (int i=0; ; i++)
    {
        char c = text[i];
        if (c=='\n' || c=='\r' || c==0)
        {
            line[letters] = 0;
            if (letters>0) read_score_line(line);
            letters = 0;
            if (c==0) break;
        }
        else if (letters<63)
        {
            line[letters] = c;
            letters++;
        }
    }
    UnloadFileText(text);
}


void save_scores()
{
    char text[MAX_PLAYERS*72+2] = "";
    int at = 0;
    for (int i=0; i<score_count; i++)
    {
        TextAppend(text,TextFormat("%s|%d|%d|%d|%d|%d|%d|%d\n",score_name[i],score_points[i][0],score_points[i][1],score_points[i][2],
                                   score_points[i][3],score_points[i][4],score_points[i][5],score_points[i][6]),&at);
    }
    SaveFileText("scores.txt",text);
}


int find_player(const char *name)
{
    for (int i=0; i<score_count; i++)
    {
        if (TextIsEqual(score_name[i],name)) return i;
    }
    return -1;
}


//typing a name that is already in the file picks that line up again
void pick_player(const char *name)
{
    player_row = find_player(name);
    if (player_row<0 && score_count<MAX_PLAYERS)
    {
        player_row = score_count;
        TextCopy(score_name[player_row],name);
        for (int i=0; i<LEVELS; i++) score_points[player_row][i] = 0;
        score_count++;
    }
}


int player_best(int n)
{
    if (player_row<0) return 0;
    return score_points[player_row][n-1];
}


//a worse run never wipes out a better one
void record_score(int n, int points)
{
    if (player_row<0) return;
    if (points<=score_points[player_row][n-1]) return;
    score_points[player_row][n-1] = points;
    save_scores();
}


//the mascot, standing on its feet at "feet" (outfit 0-3 = level 1-4)
void draw_mascot(Vector2 feet, int outfit, int jumping, float size, float squash)
{
    Rectangle source = {outfit*320,jumping*320,320,320};
    float w = size*(1+squash);
    float h = size*(1-squash);
    Rectangle dest = {feet.x,feet.y,w,h};
    Vector2 origin = {w/2,h*150/160};
    DrawTexturePro(mascot_texture,source,dest,origin,0,WHITE);
}


void background_step(int n, float dt)
{
    if (n==1) l1_background_step(dt);
    if (n==2) l2_background_step(dt);
    if (n==3) l3_background_step(dt);
    if (n==4) l4_background_step(dt);
    if (n==5) l5_background_step(dt);
    if (n==6) l6_background_step(dt);
    if (n==7) l7_background_step(dt);
}


void draw_scene(int n)
{
    if (n==1) l1_draw_scene();
    if (n==2) l2_draw_scene();
    if (n==3) l3_draw_scene();
    if (n==4) l4_draw_scene();
    if (n==5) l5_draw_scene();
    if (n==6) l6_draw_scene();
    if (n==7) l7_draw_scene();
}


void draw_intro()
{
    float t = intro_time;
    int part = t/1.6;
    if (part>3) part = 3;
    draw_scene(part+1);
    DrawRectangle(0,0,screen_width,screen_height,Fade(BLACK,0.35));

    float ground = screen_height*0.72;
    float p = (t - part*1.6)/1.6;
    Vector2 feet;
    float hop = 0;

    if (part<3)
    {
        //run across the world in hops (one big slow hop in space)
        float hops = 2;
        float hop_height = 220*su;
        if (part==2)
        {
            hops = 1;
            hop_height = 380*su;
        }
        hop = fabsf(sin(p*hops*PI));
        feet.x = -150*su + p*(screen_width+300*su);
        if (part==1) feet.x = screen_width+150*su - p*(screen_width+300*su);     //second world: right to left
        feet.y = ground - hop_height*hop;
    }
    else
    {
        //last world: hop in from the right to the middle and land
        float landing = (t - 3*1.6)/0.8;
        if (landing>1) landing = 1;
        hop = sin(landing*PI);
        feet.x = screen_width+150*su - landing*(screen_width/2+150*su);
        feet.y = ground - 260*su*hop;
    }

    int jumping = 0;
    if (hop>0.15) jumping = 1;
    float squash = 0;
    if (hop<0.15) squash = (0.15-hop)*1.2;
    //after the last landing the squash springs back to normal in a fifth of a second
    float after_landing = t - 3*1.6 - 0.8;
    if (part==3 && after_landing>0)
    {
        squash = 0.18 - after_landing*0.9;
        if (squash<0) squash = 0;
    }
    DrawEllipse(feet.x,ground,60*su*(1-hop*0.5),14*su,Fade(BLACK,0.35));
    draw_mascot(feet,part,jumping,240*su,squash);

    //white flash when the world changes
    float since_change = t - part*1.6;
    if (part>0 && since_change<0.15) DrawRectangle(0,0,screen_width,screen_height,Fade(WHITE,1-since_change/0.15));

    //title slams in: starts huge and see-through, hits full size at 0.15s, then the screen shakes
    float title_time = t - 3*1.6 - 0.8;
    if (title_time>0)
    {
        float grow = 1;
        float see = 1;
        if (title_time<0.15)
        {
            float left = 1 - title_time/0.15;
            grow = 1 + 2.5*left*left;
            see = title_time/0.15;
        }
        float shake_x = 0;
        float shake_y = 0;
        float since_hit = title_time - 0.15;
        if (since_hit>0 && since_hit<0.35)
        {
            float strength = 18*su*(1 - since_hit/0.35);
            shake_x = sin(since_hit*90)*strength;
            shake_y = cos(since_hit*70)*strength;
        }
        if (since_hit>0 && since_hit<0.12) DrawRectangle(0,0,screen_width,screen_height,Fade(WHITE,0.5*(1-since_hit/0.12)));
        int size = 130*su*grow;
        int title_width = MeasureText("THE ULTIMATE GOLF",size);
        float title_x = screen_width/2 - title_width/2 + shake_x;
        float title_y = screen_height*0.26 - size/2 + 65*su + shake_y;
        DrawText("THE ULTIMATE GOLF",title_x+6*su,title_y+6*su,size,Fade(BLACK,see));
        DrawText("THE ULTIMATE GOLF",title_x,title_y,size,Fade(GetColor(0xFFD34DFF),see));
        if (title_time>1 && fmod(t*2,2)<1.4)
        {
            int click_width = MeasureText("CLICK TO PLAY",50*su);
            DrawText("CLICK TO PLAY",screen_width/2-click_width/2,screen_height*0.86,50*su,WHITE);
        }
    }
}


//how many cards this page shows: four on the first, three on the second
int page_cards(int page)
{
    if (page==0) return 4;
    return LEVELS-4;
}


//the first level number on a page
int page_first(int page)
{
    if (page==0) return 1;
    return 5;
}


//where card i of the current page sits: 2 x 2 under the title, the arrows live in the margins
Rectangle card_rect(int i)
{
    float gap = 40*su;
    float top = 150*su;
    float side = 90*su;                                   //room for the page arrows
    float card_w = (screen_width - 3*gap - 2*side)/2;
    float card_h = (screen_height - top - 2*gap)/2;
    Rectangle rec = {side + gap + (i%2)*(card_w+gap), top + (i/2)*(card_h+gap), card_w, card_h - gap/2};
    return rec;
}


//the arrow that turns the page: 0 is the left one, 1 the right one
Rectangle page_arrow(int which)
{
    float h = 150*su;
    float y = screen_height/2 - h/2;
    if (which==0)
    {
        Rectangle rec = {20*su,y,70*su,h};
        return rec;
    }
    Rectangle rec = {screen_width-90*su,y,70*su,h};
    return rec;
}


//drawn as a chunky triangle in a rounded box, greyed out when there is nowhere to go
void draw_page_arrow(int which, int live)
{
    Rectangle r = page_arrow(which);
    Color colour = GetColor(0xFFD34DFF);
    if (live==0) colour = Fade(GRAY,0.35);
    int hover = CheckCollisionPointRec(GetMousePosition(),r) && live==1;
    DrawRectangleRounded(r,0.3,8,Fade(colour,hover ? 0.3 : 0.12));
    DrawRectangleRoundedLinesEx(r,0.3,8,3*su,colour);
    float middle = r.y + r.height/2;
    if (which==0) DrawTriangle((Vector2){r.x+46*su,middle-34*su},(Vector2){r.x+22*su,middle},(Vector2){r.x+46*su,middle+34*su},colour);
    else DrawTriangle((Vector2){r.x+24*su,middle-34*su},(Vector2){r.x+24*su,middle+34*su},(Vector2){r.x+48*su,middle},colour);
}


//where difficulty button d (0 bot, 1 chad, 2 goat) sits on the panel
Rectangle difficulty_rect(int d)
{
    float panel_x = screen_width/2 - 620*su;
    float panel_y = screen_height/2 - 270*su;
    Rectangle rec = {panel_x + 60*su + d*390*su, panel_y + 150*su, 340*su, 320*su};
    return rec;
}


//==================== THE KEY BUTTONS ====================
Rectangle brief_panel();
Rectangle menu_button(int b);
//every keyboard shortcut in the game is also a button: a key cap, then what it does.
//the rectangle is worked out from the text, so the drawing and the click test agree.
float key_button_width(const char *key, const char *what, float h)
{
    float s = h*0.42;
    return h*0.22 + MeasureText(key,s)+s*0.7 + s*0.6 + MeasureText(what,s) + h*0.3;
}


Rectangle key_button(float x, float y, const char *key, const char *what, float h)
{
    Rectangle rec = {x,y,key_button_width(key,what,h),h};
    return rec;
}


//the same button, but placed by its right hand edge
Rectangle key_button_right(float right, float y, const char *key, const char *what, float h)
{
    float w = key_button_width(key,what,h);
    Rectangle rec = {right-w,y,w,h};
    return rec;
}


void draw_key_button(Rectangle r, const char *key, const char *what, Color colour, int hover)
{
    float s = r.height*0.42;
    DrawRectangleRounded(r,0.3,8,Fade(BLACK,0.8));
    DrawRectangleRounded(r,0.3,8,hover==1 ? Fade(colour,0.35) : Fade(colour,0.12));
    DrawRectangleRoundedLinesEx(r,0.3,8,2*su,hover==1 ? colour : Fade(colour,0.75));
    Rectangle cap = {r.x+r.height*0.22,r.y+r.height/2-s*0.78,MeasureText(key,s)+s*0.7,s*1.56};
    DrawRectangleRounded(cap,0.35,6,Fade(colour,0.28));
    DrawRectangleRoundedLinesEx(cap,0.35,6,2*su,Fade(colour,0.85));
    DrawText(key,cap.x+s*0.35,cap.y+s*0.28,s,colour);
    DrawText(what,cap.x+cap.width+s*0.6,r.y+r.height/2-s/2,s,hover==1 ? WHITE : Fade(WHITE,0.85));
}


int over(Rectangle r)
{
    return CheckCollisionPointRec(GetMousePosition(),r);
}


//--- where each one sits ---
//the scoreboard pair: 0 restart, 1 menu
Rectangle hud_key(int which)
{
    float h = 46*su;
    Rectangle esc = key_button_right(screen_width-24*su,12*su,"ESC","menu",h);
    if (which==1) return esc;
    return key_button_right(esc.x-10*su,12*su,"R","restart",h);
}


//the pair on the level clear / failed panel: 0 play again, 1 menu
Rectangle end_key(int which)
{
    float h = 54*su;
    float total = key_button_width("R","play again",h)+key_button_width("ESC","menu",h)+16*su;
    float left = screen_width/2-total/2;
    Rectangle again = key_button(left,screen_height/2+66*su,"R","play again",h);
    if (which==0) return again;
    return key_button(again.x+again.width+16*su,screen_height/2+66*su,"ESC","menu",h);
}


Rectangle menu_quit_key()
{
    return key_button_right(menu_button(0).x-14*su,46*su,"ESC","quit",62*su);
}


Rectangle difficulty_back_key()
{
    float x = screen_width/2+620*su-40*su;
    float y = screen_height/2+270*su-64*su;
    return key_button_right(x,y,"ESC","back",52*su);
}


Rectangle brief_menu_key()
{
    Rectangle p = brief_panel();
    return key_button(p.x+p.width/2-200*su,p.y+p.height-95*su,"ESC","menu",58*su);
}


//the name screen: 0 start, 1 delete a letter
Rectangle name_key(int which)
{
    float y = 150*su+470*su;
    float x = screen_width/2-760*su+80*su;
    Rectangle start = key_button(x,y,"ENTER","start playing",70*su);
    if (which==0) return start;
    return key_button(start.x+start.width+20*su,y,"BKSP","delete a letter",70*su);
}


//start level n with the chosen difficulty
void start_level(int n, int difficulty)
{
    float stroke_factor = 1.1;
    obstacle_speed = 1;
    preview_seconds = 0.3;
    if (difficulty==0)
    {
        stroke_factor = 1.5;
        obstacle_speed = 0.7;
        preview_seconds = 0.6;
    }
    if (difficulty==2)
    {
        stroke_factor = 0.75;
        obstacle_speed = 1.3;
        preview_seconds = 0;
    }
    if (n==1)
    {
        l1_stroke_limit = l1_stroke_base*stroke_factor + 0.5;
        l1_reset_level();
    }
    if (n==2)
    {
        l2_stroke_limit = l2_stroke_base*stroke_factor + 0.5;
        l2_reset_level();
    }
    if (n==3)
    {
        l3_stroke_limit = l3_stroke_base*stroke_factor + 0.5;
        l3_reset_level();
    }
    if (n==4)
    {
        l4_stroke_limit = l4_stroke_base*stroke_factor + 0.5;
        l4_reset_level();
    }
    if (n==5)
    {
        l5_stroke_limit = l5_stroke_base*stroke_factor + 0.5;
        l5_reset_level();
    }
    if (n==6)
    {
        l6_stroke_limit = l6_stroke_base*stroke_factor + 0.5;
        l6_reset_level();
    }
    if (n==7)
    {
        l7_stroke_limit = l7_stroke_base*stroke_factor + 0.5;
        l7_reset_level();
    }
    level = n;
    current_difficulty = difficulty;
    brief_page = 0;
    game_mode = 3;
}


void menu_step(float dt)
{
    //keep all seven worlds moving, redraw one card picture each frame
    for (int n=1; n<=LEVELS; n++) background_step(n,dt);
    BeginTextureMode(card_texture[card_turn]);
    ClearBackground(BLACK);
    draw_scene(card_turn+1);
    EndTextureMode();
    card_turn = (card_turn+1)%LEVELS;

    Vector2 mouse = GetMousePosition();
    if (chosen_level==0)
    {
        for (int i=0; i<page_cards(menu_page); i++)
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse,card_rect(i))) chosen_level = page_first(menu_page)+i;
        }
        //turning the page, by arrow or by key
        int go_right = IsKeyPressed(KEY_RIGHT) || (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse,page_arrow(1)));
        int go_left = IsKeyPressed(KEY_LEFT) || (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse,page_arrow(0)));
        if (go_right==1 && menu_page==0) menu_page = 1;
        if (go_left==1 && menu_page==1) menu_page = 0;
        if (IsKeyPressed(KEY_ESCAPE)) quit = 1;
    }
    else
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            int clicked = -1;
            for (int d=0; d<3; d++)
            {
                if (CheckCollisionPointRec(mouse,difficulty_rect(d))) clicked = d;
            }
            if (clicked>=0)
            {
                start_level(chosen_level,clicked);
                chosen_level = 0;
            }
        }
        if (IsKeyPressed(KEY_ESCAPE) || (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && over(difficulty_back_key()))) chosen_level = 0;
    }
}


//the four buttons along the top of the menu: 0 how to play, 1 leaderboard, 2 credits
Rectangle menu_button(int b)
{
    float w = 250*su;
    float h = 62*su;
    float gap = 14*su;
    float right = screen_width - 40*su - 76*su - gap;
    Rectangle rec = {right - (3-b)*(w+gap) + gap, 46*su, w, h};
    return rec;
}


Rectangle sound_button()
{
    Rectangle rec = {screen_width-40*su-76*su,46*su,76*su,64*su};
    return rec;
}


//the small one that sits under the scoreboard while a level is being played
Rectangle game_sound_button()
{
    Rectangle rec = {screen_width-76*su,90*su,52*su,44*su};
    return rec;
}


//back to the menu, bottom left of any page
Rectangle page_back_button()
{
    Rectangle rec = {screen_width/2-760*su+40*su,screen_height-120*su,340*su,64*su};
    return rec;
}


void draw_button(Rectangle r, const char *text, Color colour, int hover)
{
    if (hover==1) DrawRectangleRounded(r,0.25,8,Fade(colour,0.35));
    else DrawRectangleRounded(r,0.25,8,Fade(colour,0.12));
    DrawRectangleRoundedLinesEx(r,0.25,8,3*su,colour);
    int w = MeasureText(text,26*su);
    DrawText(text,r.x+r.width/2-w/2,r.y+r.height/2-13*su,26*su,hover==1 ? WHITE : colour);
}


//a speaker with two waves, or a cross when the music is off
void draw_speaker(Rectangle r, int on, int hover)
{
    float k = r.height/64;                      //so the same drawing works small, in the corner of a level
    Color colour = on==1 ? GetColor(0x6FE7FFFF) : GetColor(0xB9C0CCFF);
    DrawRectangleRounded(r,0.25,8,Fade(BLACK,0.6));       //a dark plate, so it reads over a bright level too
    if (hover==1) DrawRectangleRounded(r,0.25,8,Fade(colour,0.3));
    else DrawRectangleRounded(r,0.25,8,Fade(colour,0.12));
    DrawRectangleRoundedLinesEx(r,0.25,8,3*su,colour);
    float cx = r.x+r.width/2-10*k;
    float cy = r.y+r.height/2;
    DrawRectangle(cx-17*k,cy-8*k,11*k,16*k,colour);                                             //the box
    DrawTriangle((Vector2){cx+2*k,cy+18*k},(Vector2){cx+2*k,cy-18*k},(Vector2){cx-8*k,cy},colour);   //the cone
    DrawRectangle(cx-8*k,cy-8*k,12*k,16*k,colour);
    if (on==1)
    {
        DrawRing((Vector2){cx+2*k,cy},13*k,16*k,-55,55,20,colour);
        DrawRing((Vector2){cx+2*k,cy},21*k,24*k,-55,55,20,Fade(colour,0.65));
    }
    else
    {
        DrawLineEx((Vector2){cx+10*k,cy-11*k},(Vector2){cx+28*k,cy+11*k},4*k,colour);
        DrawLineEx((Vector2){cx+28*k,cy-11*k},(Vector2){cx+10*k,cy+11*k},4*k,colour);
    }
}


void draw_menu_buttons()
{
    Vector2 mouse = GetMousePosition();
    const char *name[3] = {"HOW TO PLAY","LEADERBOARD","CREDITS"};
    Color colour[3] = {{110,220,120,255},{255,211,77,255},{190,140,255,255}};
    for (int b=0; b<3; b++)
    {
        Rectangle r = menu_button(b);
        draw_button(r,name[b],colour[b],CheckCollisionPointRec(mouse,r));
    }
    Rectangle s = sound_button();
    draw_speaker(s,sound_on,CheckCollisionPointRec(mouse,s));
    Rectangle q = menu_quit_key();
    draw_key_button(q,"ESC","quit",GetColor(0xFF7A6EFF),CheckCollisionPointRec(mouse,q));
}


void draw_menu()
{
    float t = GetTime();
    Vector2 mouse = GetMousePosition();
    DrawRectangleGradientV(0,0,screen_width,screen_height,GetColor(0x131B2AFF),GetColor(0x05070DFF));

    DrawText("THE ULTIMATE GOLF",40*su+4*su,44*su+4*su,64*su,BLACK);
    DrawText("THE ULTIMATE GOLF",40*su,44*su,64*su,GetColor(0xFFD34DFF));
    if (player_row>=0) DrawText(TextFormat("player: %s      total %d",player_name,player_total(player_row)),44*su,124*su,28*su,GRAY);
    draw_menu_buttons();

    for (int i=0; i<page_cards(menu_page); i++)
    {
        int n = page_first(menu_page)+i-1;               //which level this card is
        Rectangle r = card_rect(i);
        int hover = 0;
        if (chosen_level==0 && CheckCollisionPointRec(mouse,r)) hover = 1;
        float grow = 0;
        if (hover==1) grow = 10*su;
        Rectangle d = {r.x-grow,r.y-grow,r.width+2*grow,r.height+2*grow};

        //glow, live picture (cut to the card's shape), name bar, border
        Rectangle glow = {d.x-8*su,d.y-8*su,d.width+16*su,d.height+16*su};
        if (hover==1) DrawRectangleRec(glow,Fade(card_colour[n],0.45));
        else DrawRectangleRec(glow,Fade(card_colour[n],0.12));
        float source_height = screen_width*d.height/d.width;
        Rectangle source = {0,(screen_height-source_height)/2,screen_width,-source_height};
        Vector2 no_origin = {0,0};
        DrawTexturePro(card_texture[n].texture,source,d,no_origin,0,WHITE);
        DrawRectangle(d.x,d.y+d.height-74*su,d.width,74*su,Fade(BLACK,0.7));
        DrawText(TextFormat("LEVEL %d  -  %s",n+1,level_name[n]),d.x+24*su,d.y+d.height-58*su,46*su,card_colour[n]);
        if (player_best(n+1)>0)
        {
            const char *best = TextFormat("BEST %d",player_best(n+1));
            float best_w = MeasureText(best,30*su)+36*su;
            Rectangle tag = {d.x+d.width-best_w-16*su,d.y+16*su,best_w,46*su};
            DrawRectangleRounded(tag,0.4,8,Fade(BLACK,0.7));
            DrawText(best,tag.x+18*su,tag.y+8*su,30*su,card_colour[n]);
        }
        if (hover==1) DrawRectangleLinesEx(d,5*su,card_colour[n]);
        else DrawRectangleLinesEx(d,3*su,Fade(card_colour[n],0.7));

        //the mascot in this world's outfit, jumping when you hover (the four outfits repeat)
        Vector2 feet = {d.x+d.width-100*su,d.y+d.height-10*su};
        if (hover==1) feet.y = feet.y - fabsf(sin(t*6))*30*su;
        draw_mascot(feet,n%4,hover,150*su,0);
    }

    //the page arrows and which page you are on
    draw_page_arrow(0,menu_page>0);
    draw_page_arrow(1,menu_page<1);
    for (int p=0; p<2; p++)
    {
        Rectangle dot = {screen_width/2-30*su+p*44*su,screen_height-30*su,30*su,10*su};
        if (p==menu_page) DrawRectangleRounded(dot,0.5,8,GetColor(0xFFD34DFF));
        else DrawRectangleRounded(dot,0.5,8,Fade(GRAY,0.5));
    }

    //difficulty panel
    if (chosen_level>0)
    {
        int c = chosen_level-1;
        DrawRectangle(0,0,screen_width,screen_height,Fade(BLACK,0.7));
        Rectangle panel = {screen_width/2-620*su,screen_height/2-270*su,1240*su,540*su};
        DrawRectangleRounded(panel,0.08,8,GetColor(0x151C2BFF));
        DrawRectangleRoundedLinesEx(panel,0.08,8,4*su,card_colour[c]);
        DrawText(TextFormat("%s  -  how good are you?",level_name[c]),panel.x+60*su,panel.y+50*su,56*su,card_colour[c]);
        Rectangle back = difficulty_back_key();
        draw_key_button(back,"ESC","back",card_colour[c],CheckCollisionPointRec(mouse,back));

        const char *name[3] = {"BOT","CHAD","GOAT"};
        const char *line1[3] = {"easy","medium","hard"};
        const char *line2[3] = {"+50% strokes","+10% strokes","-25% strokes"};
        const char *line3[3] = {"slow obstacles","normal obstacles","fast obstacles"};
        const char *line4[3] = {"long aim line","medium aim line","no aim line"};
        Color colour[3] = {{110,220,120,255},{255,200,60,255},{240,80,70,255}};
        for (int d=0; d<3; d++)
        {
            Rectangle b = difficulty_rect(d);
            int hover = CheckCollisionPointRec(mouse,b);
            if (hover==1) DrawRectangleRounded(b,0.1,8,Fade(colour[d],0.3));
            else DrawRectangleRounded(b,0.1,8,Fade(colour[d],0.1));
            DrawRectangleRoundedLinesEx(b,0.1,8,3*su,colour[d]);
            int name_width = MeasureText(name[d],80*su);
            DrawText(name[d],b.x+b.width/2-name_width/2,b.y+30*su,80*su,colour[d]);
            DrawText(line1[d],b.x+30*su,b.y+140*su,36*su,WHITE);
            DrawText(line2[d],b.x+30*su,b.y+185*su,30*su,LIGHTGRAY);
            DrawText(line3[d],b.x+30*su,b.y+225*su,30*su,LIGHTGRAY);
            DrawText(line4[d],b.x+30*su,b.y+265*su,30*su,LIGHTGRAY);
        }

        Vector2 feet = {panel.x+panel.width-120*su,panel.y+10*su - fabsf(sin(t*5))*40*su};
        draw_mascot(feet,c,1,170*su,0);
    }
}



//==================== NAME, HOW TO PLAY, CREDITS, LEADERBOARD ====================
//the same dark page every one of these screens is drawn on
Rectangle draw_page(const char *title, int with_back)
{
    DrawRectangleGradientV(0,0,screen_width,screen_height,GetColor(0x131B2AFF),GetColor(0x05070DFF));
    DrawText(title,40*su+4*su,40*su+4*su,70*su,BLACK);
    DrawText(title,40*su,40*su,70*su,GetColor(0xFFD34DFF));
    Rectangle panel = {screen_width/2-760*su,150*su,1520*su,screen_height-290*su};
    DrawRectangleRounded(panel,0.03,8,GetColor(0x151C2BFF));
    DrawRectangleRoundedLinesEx(panel,0.03,8,3*su,GetColor(0x2A3550FF));
    Vector2 mouse = GetMousePosition();
    if (with_back==1)
    {
        Rectangle back = page_back_button();
        draw_key_button(back,"ESC","back to the menu",GetColor(0xFFD34DFF),CheckCollisionPointRec(mouse,back));
    }
    Rectangle s = sound_button();
    draw_speaker(s,sound_on,CheckCollisionPointRec(mouse,s));
    return panel;
}


void draw_name_entry()
{
    Rectangle p = draw_page("WHO IS PLAYING?",0);
    float t = GetTime();
    DrawText("type your name, then press ENTER",p.x+80*su,p.y+90*su,40*su,LIGHTGRAY);
    DrawText("the same name again picks up the scores you already have",p.x+80*su,p.y+150*su,30*su,GRAY);

    Rectangle box = {p.x+80*su,p.y+240*su,900*su,120*su};
    DrawRectangleRounded(box,0.15,8,GetColor(0x0B1220FF));
    DrawRectangleRoundedLinesEx(box,0.15,8,4*su,GetColor(0xFFD34DFF));
    DrawText(player_name,box.x+30*su,box.y+30*su,64*su,WHITE);
    if (fmod(t,1.0)<0.5) DrawRectangle(box.x+40*su+MeasureText(player_name,64*su),box.y+28*su,4*su,64*su,WHITE);
    DrawText(TextFormat("%d / 12 letters",name_length),box.x,box.y+140*su,28*su,GRAY);
    Vector2 mouse = GetMousePosition();
    draw_key_button(name_key(0),"ENTER","start playing",GetColor(0xFFD34DFF),over(name_key(0)) && name_length>0);
    draw_key_button(name_key(1),"BKSP","delete a letter",GetColor(0xB9C0CCFF),over(name_key(1)));
    if (mouse.x<0) return;

    draw_mascot((Vector2){p.x+p.width-260*su,p.y+p.height-80*su},1,1,320*su,0);
}


void draw_how_to_play()
{
    Rectangle p = draw_page("HOW TO PLAY",1);
    float t = GetTime();

    //--- the picture: a little course inside its own frame ---
    Rectangle shot = {p.x+60*su,p.y+50*su,p.width-120*su,250*su};
    DrawRectangleRounded(shot,0.06,8,GetColor(0x101A2CFF));
    DrawRectangleRoundedLinesEx(shot,0.06,8,2*su,GetColor(0x2A3550FF));

    float y = shot.y+150*su;
    DrawRectangle(shot.x+20*su,y+26*su,shot.width-40*su,6*su,GetColor(0x1E4D2BFF));    //the ground
    Vector2 ball = {shot.x+shot.width*0.30,y};
    Vector2 hole = {shot.x+shot.width*0.86,y};

    //the hole and its flag
    DrawEllipse(hole.x,hole.y+16*su,30*su,14*su,BLACK);
    DrawLineEx((Vector2){hole.x,hole.y+14*su},(Vector2){hole.x,hole.y-86*su},4*su,LIGHTGRAY);
    DrawTriangle((Vector2){hole.x,hole.y-86*su},(Vector2){hole.x,hole.y-38*su},(Vector2){hole.x+62*su,hole.y-62*su},RED);

    //the drag: a line of dots going back from the ball, and the shot line going forward
    float pull = 80*su+34*su*fabsf(sin(t*1.4));
    for (int i=1; i<=6; i++)
    {
        float q = (float)i/6;
        DrawCircleV((Vector2){ball.x-34*su-pull*q,y},5*su-q*2*su,Fade(WHITE,0.75-q*0.45));
    }
    DrawCircleV((Vector2){ball.x-34*su-pull,y},13*su,Fade(GetColor(0xFFD34DFF),0.9));
    DrawLineEx((Vector2){ball.x+30*su,y},(Vector2){ball.x+30*su+pull*2.4,y},5*su,Fade(GetColor(0x6FE7FFFF),0.85));
    DrawTriangle((Vector2){ball.x+40*su+pull*2.4,y},(Vector2){ball.x+22*su+pull*2.4,y-11*su},(Vector2){ball.x+22*su+pull*2.4,y+11*su},GetColor(0x6FE7FFFF));
    DrawCircleV(ball,20*su,WHITE);
    DrawCircleLines(ball.x,ball.y,20*su,GRAY);

    //labels, each one centred on the thing it names
    const char *drag_text = "1.  pull back with the mouse";
    const char *go_text = "2.  let go, and it flies";
    DrawText(drag_text,ball.x-pull/2-MeasureText(drag_text,28*su)/2-30*su,y-104*su,28*su,GetColor(0xFFD34DFF));
    DrawText(go_text,ball.x+30*su+pull*1.2-MeasureText(go_text,28*su)/2,y+62*su,28*su,GetColor(0x6FE7FFFF));
    DrawText("your ball",ball.x-MeasureText("your ball",26*su)/2,y-58*su,26*su,GRAY);
    DrawText("the hole",hole.x-MeasureText("the hole",26*su)/2,y+62*su,26*su,GRAY);

    //--- the rules ---
    const char *step[6] = {
        "The white ball is you. The hole with the flag is where it has to go.",
        "Every shot counts as one stroke, and each level gives you a limited number of them.",
        "The counter at the top of the screen shows the strokes you have used and the limit.",
        "Water, lava, the void, quicksand and the traps put the ball back where you shot from.",
        "R plays the level again from the start. ESC leaves it and goes back to the menu.",
        "Before every level a briefing names each obstacle in it. Press SKIP once you know them."};
    for (int i=0; i<6; i++)
    {
        float ry = p.y+350*su+i*54*su;
        DrawCircle(p.x+94*su,ry+15*su,19*su,Fade(GetColor(0xFFD34DFF),0.22));
        DrawText(TextFormat("%d",i+1),p.x+86*su,ry+1*su,28*su,GetColor(0xFFD34DFF));
        DrawText(step[i],p.x+140*su,ry,28*su,LIGHTGRAY);
    }

    //--- the scoring, behind its own line ---
    DrawLineEx((Vector2){p.x+70*su,p.y+686*su},(Vector2){p.x+p.width-70*su,p.y+686*su},2*su,GetColor(0x2A3550FF));
    DrawText("SCORING",p.x+86*su,p.y+706*su,30*su,GetColor(0x6FE7FFFF));
    DrawText("each stroke you did NOT use is worth 10 points on BOT, 25 on CHAD, 50 on GOAT,",p.x+280*su,p.y+706*su,26*su,LIGHTGRAY);
    DrawText("plus 100 / 250 / 500 for finishing. Your best on each level is kept in scores.txt.",p.x+280*su,p.y+742*su,26*su,LIGHTGRAY);
}


//a photo of any shape, drawn as a square: the source rectangle takes the middle of it
void draw_developer(Texture2D photo, float centre_x, float y, float size, const char *name)
{
    Color colour = GetColor(0x6FE7FFFF);
    float side = photo.width;
    if (photo.height<side) side = photo.height;
    Rectangle source = {(photo.width-side)/2,(photo.height-side)/2,side,side};
    Rectangle dest = {centre_x-size/2,y,size,size};
    Rectangle glow = {dest.x-6*su,y-6*su,size+12*su,size+12*su};
    Vector2 no_origin = {0,0};
    DrawRectangleRec(glow,Fade(colour,0.15));
    DrawTexturePro(photo,source,dest,no_origin,0,WHITE);
    DrawRectangleLinesEx(dest,3*su,Fade(colour,0.7));
    DrawText(name,centre_x-MeasureText(name,32*su)/2,y+size+16*su,32*su,WHITE);
}


void draw_credits()
{
    Rectangle p = draw_page("CREDITS",1);
    float x = p.x+100*su;
    draw_mascot((Vector2){p.x+p.width-150*su,p.y+195*su},3,0,180*su,0);
    DrawText("THE ULTIMATE GOLF",x,p.y+60*su,64*su,GetColor(0xFFD34DFF));

    DrawText("DEVELOPERS",x,p.y+170*su,32*su,GetColor(0x6FE7FFFF));
    draw_developer(developer_photo[0],p.x+410*su,p.y+220*su,160*su,"Abdullah Al Nafi  -  2505093");
    draw_developer(developer_photo[1],p.x+1030*su,p.y+220*su,160*su,"Syed Abdul Fahim  -  2505114");

    DrawText("SPRITES AND ARTWORK",x,p.y+465*su,32*su,GetColor(0x6FE7FFFF));
    DrawText("All sprites and textures in this game were generated with AI.",x,p.y+510*su,30*su,LIGHTGRAY);

    DrawText("SOUND AND MUSIC",x,p.y+575*su,32*su,GetColor(0x6FE7FFFF));
    DrawText("All sound effects and music tracks are from freesound.org.",x,p.y+620*su,30*su,LIGHTGRAY);

    DrawText("SPECIAL THANKS",x,p.y+685*su,32*su,GetColor(0x6FE7FFFF));
    DrawText("raylib, by Ramon Santamaria and its contributors - the library this game is built on.",x,p.y+730*su,30*su,LIGHTGRAY);


}


void draw_leaderboard()
{
    Rectangle p = draw_page("LEADERBOARD",1);
    if (score_count==0)
    {
        DrawText("no scores yet - go and finish a level",p.x+100*su,p.y+120*su,44*su,GRAY);
        return;
    }

    //sort the rows by total, biggest first
    int order[MAX_PLAYERS];
    for (int i=0; i<score_count; i++) order[i] = i;
    for (int i=0; i<score_count; i++)
    {
        for (int j=i+1; j<score_count; j++)
        {
            if (player_total(order[j])>player_total(order[i]))
            {
                int keep = order[i];
                order[i] = order[j];
                order[j] = keep;
            }
        }
    }

    float name_x = p.x+150*su;
    float first = p.x+520*su;
    float step = 112*su;
    float total_x = p.x+1320*su;
    float head = p.y+70*su;
    DrawText("#",p.x+90*su,head,32*su,GRAY);
    DrawText("NAME",name_x,head,32*su,GRAY);
    for (int i=0; i<LEVELS; i++) DrawText(TextFormat("L%d",i+1),first+i*step,head,28*su,card_colour[i]);
    DrawText("TOTAL",total_x,head,32*su,GetColor(0xFFD34DFF));
    DrawLineEx((Vector2){p.x+80*su,head+46*su},(Vector2){p.x+p.width-80*su,head+46*su},2*su,GetColor(0x2A3550FF));

    int rows = score_count;
    if (rows>10) rows = 10;
    for (int r=0; r<rows; r++)
    {
        int i = order[r];
        float y = head+76*su+r*62*su;
        if (i==player_row) DrawRectangleRounded((Rectangle){p.x+70*su,y-10*su,p.width-140*su,58*su},0.4,8,Fade(GetColor(0xFFD34DFF),0.15));
        Color shade = r==0 ? GetColor(0xFFD34DFF) : WHITE;
        DrawText(TextFormat("%d",r+1),p.x+90*su,y,36*su,shade);
        DrawText(score_name[i],name_x,y,36*su,shade);
        for (int n=0; n<LEVELS; n++)
        {
            if (score_points[i][n]>0) DrawText(TextFormat("%d",score_points[i][n]),first+n*step,y,30*su,LIGHTGRAY);
            else DrawText("-",first+n*step,y,30*su,DARKGRAY);
        }
        DrawText(TextFormat("%d",player_total(i)),total_x,y,36*su,shade);
    }
    DrawText("kept in scores.txt, next to the game",p.x+90*su,p.y+p.height-70*su,26*su,GRAY);
}


//the R key lives inside each level, so the button needs the same door
void restart_level()
{
    if (level==1) l1_reset_level();
    if (level==2) l2_reset_level();
    if (level==3) l3_reset_level();
    if (level==4) l4_reset_level();
    if (level==5) l5_reset_level();
    if (level==6) l6_reset_level();
    if (level==7) l7_reset_level();
    score_saved = 0;
}


//the two buttons drawn over the level's own "R restart  ESC menu" text
void draw_level_keys()
{
    //the level draws "R restart  ESC menu" itself; this plate hides it and the buttons take its place
    DrawRectangle(hud_key(0).x-32*su,6*su,screen_width-hud_key(0).x+32*su,58*su,GetColor(0x1A1E28FF));
    draw_key_button(hud_key(0),"R","restart",GetColor(0xFFD34DFF),over(hud_key(0)));
    draw_key_button(hud_key(1),"ESC","menu",GetColor(0xB9C0CCFF),over(hud_key(1)));
    if (level_state()!=0)
    {
        DrawRectangle(screen_width/2-330*su,screen_height/2+56*su,660*su,74*su,GetColor(0x1A1E28FF));
        draw_key_button(end_key(0),"R","play again",GetColor(0xFFD34DFF),over(end_key(0)));
        draw_key_button(end_key(1),"ESC","menu",GetColor(0xB9C0CCFF),over(end_key(1)));
    }
}


//the points for the run that has just finished, under the level's own panel
void draw_score_panel()
{
    int d = current_difficulty;
    int left = level_limit()-level_strokes();
    if (left<0) left = 0;
    int points = score_for(level_limit(),level_strokes(),d);
    Rectangle panel = {screen_width/2-340*su,screen_height/2+200*su,680*su,180*su};
    DrawRectangleRounded(panel,0.08,8,GetColor(0x151C2BFF));
    DrawRectangleRoundedLinesEx(panel,0.08,8,3*su,GetColor(0xFFD34DFF));
    DrawText(TextFormat("%d strokes left  x  %d",left,score_rate[d]),panel.x+36*su,panel.y+28*su,30*su,LIGHTGRAY);
    DrawText(TextFormat("finish bonus  + %d",score_bonus[d]),panel.x+36*su,panel.y+72*su,30*su,LIGHTGRAY);
    DrawText(TextFormat("your best: %d",player_best(level)),panel.x+36*su,panel.y+122*su,28*su,GRAY);
    const char *big = TextFormat("%d",points);
    DrawText(big,panel.x+panel.width-MeasureText(big,80*su)-40*su,panel.y+40*su,80*su,GetColor(0xFFD34DFF));
    DrawText("POINTS",panel.x+panel.width-MeasureText("POINTS",26*su)-40*su,panel.y+126*su,26*su,GRAY);
}


//==================== SOUNDS ====================
Sound snd_shot, snd_bounce, snd_pot, snd_fanfare, snd_game_over, snd_reset, snd_blip;
Sound snd_hop, snd_slam;
Sound snd_clang, snd_fan, snd_laser, snd_lava;
Sound snd_splash, snd_crab, snd_boing;
Sound snd_warp, snd_whoosh, snd_ufo;
Sound snd_chomp, snd_dart, snd_plank, snd_door, snd_plate, snd_gloop;
Music music[7];            //0 menu, 1-4 level ambience, 5 crabs walking (level 2), 6 extra intro track
float music_base[7] = {0.5,0.4,0.4,2.5,0.4,0.35,1};   //music[3] is a deep quiet hum, so it gets a boost


//each stream's own level, set once
void set_music_volumes()
{
    for (int i=0; i<7; i++) SetMusicVolume(music[i],music_base[i]);
}


//the speaker button and the M key: one call silences the whole audio device,
//music and sound effects together, without touching a single volume of its own
void apply_sound_switch()
{
    SetMasterVolume(sound_on ? 1.0 : 0.0);
}
int music_playing = -1;
float bounce_wait = 0;
float intro_last_hop = 0;
int last_hover = -1;

//what the level looked like before this frame, to spot what changed
Vector2 old_speed;
int old_stroke;
int old_state;
float old_message;
float old_extra[20];


//a quiet sound made louder while loading (gain 2 = twice as loud)
Sound load_sound_louder(const char *file, float gain)
{
    Wave wave = LoadWave(file);
    WaveFormat(&wave,wave.sampleRate,16,wave.channels);
    short *samples = (short *)wave.data;
    for (unsigned int i=0; i<wave.frameCount*wave.channels; i++)
    {
        float louder = samples[i]*gain;
        if (louder>32767) louder = 32767;
        if (louder<-32768) louder = -32768;
        samples[i] = louder;
    }
    Sound sound = LoadSoundFromWave(wave);
    UnloadWave(wave);
    return sound;
}


void load_sounds()
{
    snd_shot = LoadSound("assets/sounds-shared/shot_hit.wav");
    snd_bounce = LoadSound("assets/sounds-shared/wall_bounce.wav");
    snd_pot = load_sound_louder("assets/sounds-shared/ball_in_pot.wav",8);
    snd_fanfare = LoadSound("assets/sounds-shared/game-success-fanfare.wav");
    snd_game_over = LoadSound("assets/sounds-shared/game-over.wav");
    snd_reset = LoadSound("assets/sounds-shared/hazard_reset.wav");
    snd_blip = LoadSound("assets/sounds-shared/hover_blip.wav");
    snd_hop = LoadSound("assets/sounds-intro/mascot_hop.wav");
    snd_slam = load_sound_louder("assets/sounds-intro/title_slam.wav",1.6);
    snd_clang = LoadSound("assets/sounds-foundry/metal_clang.wav");
    snd_fan = load_sound_louder("assets/sounds-foundry/fan_whirr.ogg",8);
    snd_laser = LoadSound("assets/sounds-foundry/laser_zap.wav");
    snd_lava = LoadSound("assets/sounds-foundry/lava_sizzle.mp3");
    snd_splash = load_sound_louder("assets/sounds-shoreline/water_splash.wav",5);
    snd_crab = LoadSound("assets/sounds-shoreline/crab_hit.wav");
    snd_boing = LoadSound("assets/sounds-shoreline/ball_hit.wav");
    snd_warp = LoadSound("assets/sounds-event_horizon/wormhole_warp.wav");
    snd_whoosh = LoadSound("assets/sounds-event_horizon/commet_whoosh.wav");
    snd_ufo = LoadSound("assets/sounds-event_horizon/UFO.ogg");
    snd_chomp = LoadSound("assets/sounds-lost_temple/piranha.wav");
    snd_dart = LoadSound("assets/sounds-lost_temple/dart_thwip.wav");
    snd_plank = LoadSound("assets/sounds-lost_temple/plank_break.wav");
    snd_door = LoadSound("assets/sounds-lost_temple/stone_door_open.wav");
    snd_plate = LoadSound("assets/sounds-lost_temple/pressure_plate.wav");
    snd_gloop = LoadSound("assets/sounds-lost_temple/quicksand_gallop.wav");

    //music and ambience stream from the file instead of loading it all
    music[0] = LoadMusicStream("assets/sounds-intro/menu_music.ogg");
    music[1] = LoadMusicStream("assets/sounds-foundry/factory_ambience.ogg");
    music[2] = LoadMusicStream("assets/sounds-shoreline/beach_ambience.ogg");
    music[3] = LoadMusicStream("assets/sounds-event_horizon/space_ambience.ogg");
    music[4] = LoadMusicStream("assets/sounds-lost_temple/jungle_ambience.ogg");
    music[5] = LoadMusicStream("assets/sounds-shoreline/crab_walking.wav");
    music[6] = LoadMusicStream("assets/sounds-intro/additional_music_for_intro.ogg");
    set_music_volumes();
    apply_sound_switch();
}


void unload_sounds()
{
    Sound all[25] = {snd_shot,snd_bounce,snd_pot,snd_fanfare,snd_game_over,snd_reset,snd_blip,snd_hop,snd_slam,snd_clang,snd_fan,snd_laser,snd_lava,
                     snd_splash,snd_crab,snd_boing,snd_warp,snd_whoosh,snd_ufo,snd_chomp,snd_dart,snd_plank,snd_door,snd_plate,snd_gloop};
    for (int i=0; i<25; i++) UnloadSound(all[i]);
    for (int i=0; i<7; i++) UnloadMusicStream(music[i]);
}


void play(Sound sound, float volume)
{
    SetSoundVolume(sound,volume);
    PlaySound(sound);
}


//switch the background music (0 menu, 1-4 levels), level 2 also gets the crabs walking
void play_music(int m)
{
    if (m==music_playing) return;
    for (int i=0; i<6; i++) StopMusicStream(music[i]);
    PlayMusicStream(music[m]);
    if (m==2) PlayMusicStream(music[5]);
    music_playing = m;
}


void update_music()
{
    if (music_playing>=0) UpdateMusicStream(music[music_playing]);
    if (music_playing==2) UpdateMusicStream(music[5]);
}


//how high the intro mascot is (0 on the ground, 1 top of the hop), same maths as draw_intro
float intro_hop(float t)
{
    int part = t/1.6;
    if (part>3) part = 3;
    if (part<3)
    {
        float hops = 2;
        if (part==2) hops = 1;
        return fabsf(sin((t - part*1.6)/1.6*hops*PI));
    }
    float landing = (t - 3*1.6)/0.8;
    if (landing>1) landing = 1;
    return sin(landing*PI);
}


void remember_level()
{
    if (level==1)
    {
        old_speed = l1_speed;
        old_stroke = l1_stroke;
        old_state = l1_game_state;
        old_message = l1_message_timer;
        old_extra[0] = l1_laser_timer;
    }
    if (level==2)
    {
        old_speed = l2_speed;
        old_stroke = l2_stroke;
        old_state = l2_game_state;
        old_message = l2_message_timer;
    }
    if (level==3)
    {
        old_speed = l3_speed;
        old_stroke = l3_stroke;
        old_state = l3_game_state;
        old_message = l3_message_timer;
        old_extra[0] = l3_wormhole_cooldown;
        old_extra[1] = l3_event_timer;
        old_extra[2] = l3_laser_clock[0];
        old_extra[3] = l3_laser_clock[1];
    }
    if (level==5)
    {
        old_speed = l5_speed;
        old_stroke = l5_stroke;
        old_state = l5_game_state;
        old_message = l5_message_timer;
    }
    if (level==6)
    {
        old_speed = l6_speed;
        old_stroke = l6_stroke;
        old_state = l6_game_state;
        old_message = l6_message_timer;
    }
    if (level==7)
    {
        old_speed = l7_speed;
        old_stroke = l7_stroke;
        old_state = l7_game_state;
        old_message = l7_message_timer;
    }
    if (level==4)
    {
        old_speed = l4_speed;
        old_stroke = l4_stroke;
        old_state = l4_game_state;
        old_message = l4_message_timer;
        for (int i=0; i<5; i++)
        {
            old_extra[i] = l4_plank_timer[0][i];
            old_extra[5+i] = l4_plank_timer[1][i];
        }
        old_extra[10] = l4_gate_timer;
        old_extra[11] = l4_plate_down[1] + l4_plate_down[2];
        old_extra[12] = l4_door_velocity[0].y;
        old_extra[13] = l4_door_velocity[1].y;
        for (int i=0; i<4; i++) old_extra[14+i] = l4_dart_clock[i];
    }
}


//a bounce sound that fits what was hit, louder when the ball was faster
void bounce_sound(Vector2 at, float impact)
{
    float volume = impact/(650*su);
    if (volume<0.25) volume = 0.25;
    if (volume>1) volume = 1;
    Sound sound = snd_bounce;
    if (level==1) sound = snd_clang;
    if (level==2)
    {
        for (int i=0; i<4; i++)
        {
            if (Vector2Distance(at,l2_beach_ball[i]) < l2_beach_ball_radius+l2_radius_ball+12*su) sound = snd_boing;
            if (CheckCollisionCircleRec(at,l2_radius_ball+12*su,l2_crab[i])) sound = snd_crab;
        }
    }
    if (level==3)
    {
        for (int i=0; i<4; i++)
        {
            if (Vector2Distance(at,l3_bumper[i]) < l3_bumper_radius+l3_radius_ball+12*su) sound = snd_boing;
        }
    }
    if (level==4)
    {
        for (int i=0; i<6; i++)
        {
            if (Vector2Distance(at,l4_bumper[i]) < l4_bumper_radius+l4_radius_ball+12*su) sound = snd_boing;
        }
    }
    play(sound,volume);
}


//a timed trap (laser or darts) just switched on: only heard when it is close to the ball
void trap_sound(float old_clock, float new_clock, Rectangle trap, Vector2 at, Sound sound)
{
    Vector2 middle = {trap.x+trap.width/2,trap.y+trap.height/2};
    if (old_clock<1.6 && new_clock>=1.6 && Vector2Distance(middle,at)<450*su) play(sound,0.35);
}


//compare the level with how it was before this frame and play what happened
void level_sounds()
{
    Vector2 speed_now = l1_speed;
    Vector2 ball_now = l1_ball;
    int stroke_now = l1_stroke;
    int state_now = l1_game_state;
    float message_now = l1_message_timer;
    int type_now = l1_message_type;
    int teleported = 0;
    if (level==2)
    {
        speed_now = l2_speed;
        ball_now = l2_ball;
        stroke_now = l2_stroke;
        state_now = l2_game_state;
        message_now = l2_message_timer;
        type_now = 1;
    }
    if (level==3)
    {
        speed_now = l3_speed;
        ball_now = l3_ball;
        stroke_now = l3_stroke;
        state_now = l3_game_state;
        message_now = l3_message_timer;
        type_now = l3_message_type;
        if (l3_wormhole_cooldown>old_extra[0]+0.2)
        {
            teleported = 1;
            play(snd_warp,0.9);
        }
    }
    if (level==4)
    {
        speed_now = l4_speed;
        ball_now = l4_ball;
        stroke_now = l4_stroke;
        state_now = l4_game_state;
        message_now = l4_message_timer;
        type_now = l4_message_type;
    }
    //the three new courses: the shared sounds only, until their own ones arrive
    if (level==5)
    {
        speed_now = l5_speed;
        ball_now = l5_ball;
        stroke_now = l5_stroke;
        state_now = l5_game_state;
        message_now = l5_message_timer;
        type_now = l5_message_type;
    }
    if (level==6)
    {
        speed_now = l6_speed;
        ball_now = l6_ball;
        stroke_now = l6_stroke;
        state_now = l6_game_state;
        message_now = l6_message_timer;
        type_now = l6_message_type;
    }
    if (level==7)
    {
        speed_now = l7_speed;
        ball_now = l7_ball;
        stroke_now = l7_stroke;
        state_now = l7_game_state;
        message_now = l7_message_timer;
        type_now = l7_message_type;
    }

    //shot
    if (stroke_now>old_stroke) play(snd_shot,0.9);

    //bounce: the ball was moving and suddenly turned (not a shot, not a wormhole)
    bounce_wait = bounce_wait - GetFrameTime();
    float before = Vector2Length(old_speed);
    float after = Vector2Length(speed_now);
    if (bounce_wait<=0 && teleported==0 && stroke_now==old_stroke && before>40*su && after>20*su)
    {
        float turn = Vector2DotProduct(Vector2Normalize(old_speed),Vector2Normalize(speed_now));
        if (turn<0.6)
        {
            bounce_sound(ball_now,before);
            bounce_wait = 0.08;
        }
    }

    //hazards: whoosh back plus the level's own sound
    if (message_now>old_message+0.5)
    {
        play(snd_reset,0.6);
        if (level==1 && type_now==1) play(snd_lava,0.9);
        if (level==1 && type_now==2) play(snd_laser,0.9);
        if (level==2) play(snd_splash,1);
        if (level==3 && type_now==1) play(snd_whoosh,0.5);
        if (level==3 && type_now==2) play(snd_laser,0.9);
        if (level==3 && type_now==3) play(snd_ufo,0.9);
        if (level==4 && type_now==1) play(snd_chomp,1);
        if (level==4 && type_now==2) play(snd_dart,1);
        if (level==4 && type_now==3) play(snd_gloop,1);
    }

    //win and lose
    if (state_now==1 && old_state==0)
    {
        play(snd_pot,1);
        play(snd_fanfare,0.8);
    }
    if (state_now==2 && old_state==0) play(snd_game_over,0.8);

    //level 1: laser switching on nearby
    if (level==1) trap_sound(old_extra[0],l1_laser_timer,l1_laser,ball_now,snd_laser);

    //level 3: comet / meteors / ufo arriving, lasers switching on nearby
    if (level==3)
    {
        if (l3_event_type!=0 && old_extra[1]<1.5 && l3_event_timer>=1.5)
        {
            if (l3_event_type==3) play(snd_ufo,0.7);
            else play(snd_whoosh,0.8);
        }
        for (int i=0; i<2; i++) trap_sound(old_extra[2+i],l3_laser_clock[i],l3_laser_gate[i],ball_now,snd_laser);
    }

    //level 4: planks cracking, plates, doors starting to move, darts firing nearby
    if (level==4)
    {
        int cracked = 0;
        for (int i=0; i<5; i++)
        {
            if (old_extra[i]==0 && l4_plank_timer[0][i]>0) cracked = 1;
            if (old_extra[5+i]==0 && l4_plank_timer[1][i]>0) cracked = 1;
        }
        if (cracked==1) play(snd_plank,0.7);
        if (l4_gate_timer>old_extra[10]+1 || l4_plate_down[1]+l4_plate_down[2]>old_extra[11]) play(snd_plate,1);
        for (int i=0; i<2; i++)
        {
            if (old_extra[12+i]==0 && l4_door_velocity[i].y!=0) play(snd_door,0.8);
        }
        for (int i=0; i<4; i++) trap_sound(old_extra[14+i],l4_dart_clock[i],l4_dart_gate[i],ball_now,snd_dart);
    }
}


//==================== LEVEL BRIEFING ====================
//every obstacle: name, where to cut its live picture from the level (design units), what it does
//picture x = -1 draws the ufo sprite, -2 a comet, -3 a meteor shower (they have no fixed place)
int brief_count[LEVELS] = {13,9,13,11,12,12,12};
const char *brief_name[LEVELS][13] = {
    {"Valve wheel","Steel crate","Push-down belt","Warning diamond","Steel girders","Oil slick","Spinning fan","Crane beam","Hydraulic piston","Magnet","Push-up belt","Molten pit","Laser gate"},
    {"The sea","Soft sand","Wet sand","Tidal sand bar","Rip current","Whirlpool","Crab","Palm tree","Beach ball"},
    {"The void","Wormhole","Black hole","Planet","Asteroid","Laser gate","Satellite","Energy bumper","Vacuum strip","Solar wind","Comet","Meteor shower","UFO"},
    {"Piranha river","Plank bridge","Mud","Moss","Quicksand","Boulder","Mushroom","Gate plate","Dart trap","Altar plates","Totem"},
    {"Open water","Frozen lake","Deep powder","Snow drift","Boardwalk","Growing snowball","Icicle","Ski gondola","Pine tree","Campfire","The wind","Avalanche"},
    {"The dark","Candle","Blink lamp","Hidden way down","Phasing wall","Ghost","Cobweb","Trapdoor","Chandelier","Mirror pair","Bat","Crypt plates"},
    {"The lava lake","Rising lava","Cooling crust","Rock bridge","Eruption vent","Geyser","Lava tube","Rock raft","Steam vent","Obsidian crystal","Rockfall","The heart"}};
float brief_x[LEVELS][13] = {
    {226,126,171,486,418,486,801,801,1116,1116,1116,1376,1431},
    {390,225,465,520,915,730,1200,1180,1000},
    {800,1840,960,600,700,1810,1200,1330,675,110,-2,-3,-1},
    {690,690,340,835,950,330,250,1080,1280,1810,1810},
    {1580,420,1720,1205,1190,1000,1000,485,220,1440,1080,300},
    {980,250,470,355,860,290,170,700,860,180,760,1000},
    {620,620,575,1120,250,620,960,1140,400,900,900,1700}};
float brief_y[LEVELS][13] = {
    {692,842,460,428,607,882,478,731,568,818,380,722,358},
    {520,515,165,490,630,820,140,225,225},
    {250,700,640,720,150,454,990,580,990,490,0,0,0},
    {600,210,920,350,795,570,220,570,334,200,570},
    {910,390,665,920,676,470,404,660,700,560,460,400},
    {620,860,220,235,385,205,220,480,480,200,600,880},
    {480,1000,690,250,300,960,660,240,250,640,500,760}};
const char *brief_text[LEVELS][13] = {
    {"Round bumper. Bounces the ball\nstraight back off it.",
     "Solid block. Use its flat sides\nfor bank shots.",
     "Drags the ball down while it's on it.\nHit it hard to get across.",
     "Pointy block: sends the ball\noff at an angle.",
     "Fixed bars sticking out of the walls.\nAim around them.",
     "Almost no friction: the ball\nslides much further.",
     "Blades whack the ball away.\nTime your shot between them.",
     "Slides left and right and\nknocks the ball.",
     "Punches out of the wall every few\nseconds. Red light = incoming.",
     "Pulls a moving ball in. Touch the\ncore and it sticks.",
     "Pushes the ball back up this lane.\nNeeds a strong shot.",
     "Roll into the lava = MELTED.\nBack to your last shot.",
     "Deadly while red. Blinking yellow\nmeans it's about to fire."},
    {"Ball's centre in the water = SPLASH.\nBack to your last shot.",
     "Kills the ball's speed fast.",
     "Slippery: the ball slides\nmuch further.",
     "Dry at low tide, sea at high tide.\nThe rising tide pushes you back.",
     "Shallow water that drags the ball\nsideways into the sea.",
     "Pulls the ball off the sand\ninto the water.",
     "Walks side to side and kicks\nthe ball away.",
     "Solid trunk. The leaves hide\nyour ball underneath.",
     "Super bouncy: sends the ball\naway faster than it came."},
    {"Roll off a walkway = LOST IN SPACE.\nBack to your last shot.",
     "Enter a ring, pop out of its twin going\nthe way the arrow points. Orange = trap!",
     "Bends moving balls toward it.\nThe pot is right next to it.",
     "Its gravity bends your shot.\nThe core is solid.",
     "Drifts across the walkway\nand knocks you off.",
     "Deadly while red. Blinking\nmeans it's about to fire.",
     "Spinning solar panels\nwhack the ball.",
     "Bounces the ball back faster\nthan it came.",
     "Almost no friction: the ball\nslides much further.",
     "Blows the ball toward the void.",
     "Streaks across without stopping.\nA red arrow warns you first.",
     "Once per game a pack of 5 rocks flies\nthrough together. Each one knocks you.",
     "Flies over once. Sit still under it and\nit beams your ball somewhere else."},
    {"Ball in the river = CHOMP.\nBack to your last shot.",
     "Planks crack when you roll on them and\nfall a moment later. Keep rolling!",
     "Slows the ball a lot.",
     "Slippery: the ball slides\nmuch further.",
     "Stop in it and you sink in\n3 seconds = SUNK.",
     "Rolls back and forth and\nknocks the ball.",
     "Bouncy: sends the ball away\nfaster than it came.",
     "Roll over it: the temple gate\nopens for 6 seconds.",
     "Fires darts while its eyes are red\n= DARTED.",
     "Press BOTH corner plates to open the\naltar door. Falling un-presses them.",
     "Spinning log arms guard\nthe altar door."},
    {"The lakes have melted through in places.\nGo in and the shot starts again.",
     "Four of them, and almost no friction.\nThe ball keeps going. So does the wind.",
     "Kills the speed at once. A safe place\nto stop and line up a shot.",
     "Stand still in one for three seconds\nand the snow swallows the ball.",
     "Laid over the open water. It cracks when\nyou roll on it and comes back in five seconds.",
     "Crosses the basin and GROWS as it goes.\nThe bigger it is, the harder it hits.",
     "Hangs off the mountain. Go under one\nand it comes down.",
     "Rides the cable across the middle lake.\nStep on and let it carry you.",
     "Solid. Useful: it is the only thing\nthat will stop you on ice.",
     "Melts the ice around it back to grip.\nThree of them, one on each big lake.",
     "Pushes every rolling ball. Breeze, then\ngust, then gale, and the way it blows flips.",
     "Once a game it sweeps the west of the\nbasin and shoves you back towards the camp."},
    {"You only see what your light reaches.\nThe briefing is the map you get.",
     "Roll over one and it stays lit for the\nrest of the level. Light is progress.",
     "Flares for a second on its own and shows\nyou a slice of the house you have not seen.",
     "A trapdoor you can only find in a flash.\nDrop through it and skip half the house.",
     "There, then not there. It swaps every\ntime the hall clock chimes.",
     "Shoves the ball off its line - but only\nin the dark. In candlelight it fades away.",
     "Thick enough to stop the ball\nalmost dead.",
     "Open for three seconds in every nine.\nOpen means a hole in the floor.",
     "Comes down when the ball crosses\nunder it, and stays down.",
     "Into one, out of the other, going\nthe way the second one faces.",
     "Crosses the room on a loop\nand knocks the ball off line.",
     "Both plates down opens the crypt.\nFall anywhere and they pop back up."},
    {"Everything that is not rock is lava.\nTouch it and you start the shot again.",
     "It climbs from the bottom and never\ngoes back. You start low. Finish high.",
     "Grey is safe, glowing is not.\nFour seconds on, two seconds deadly.",
     "Cracks fast over the lake, and what is\nunderneath is not water.",
     "The hole is always deadly. Now and then\nit floods a circle around itself, then drains.",
     "Throws the ball in one shove instead\nof pushing it. Watch the arrow.",
     "In one end, out of the other, fast,\nsomewhere you cannot walk to.",
     "A slab of crust drifting on the lake.\nRide it across.",
     "Blows sideways in bursts.\nTime your run between them.",
     "Hard and springy: it sends the ball\naway faster than it arrived.",
     "The ceiling gives way once or twice\na game. The warning is on the left.",
     "The hole is flooded most of the time.\nIt opens for a few seconds after an eruption."}};
Color brief_accent[LEVELS] = {{232,185,35,255},{255,214,110,255},{80,230,255,255},{232,184,64,255},
                             {191,228,242,255},{255,186,88,255},{255,197,61,255}};
Color brief_text_colour[LEVELS] = {{235,235,235,255},{250,235,205,255},{210,240,255,255},{240,236,220,255},
                                  {236,246,255,255},{255,236,205,255},{255,230,200,255}};


Rectangle brief_panel()
{
    Rectangle panel = {screen_width/2-760*su,screen_height/2-470*su,1520*su,940*su};
    return panel;
}


//buttons: 0 skip, 1 back, 2 next / play
Rectangle brief_button(int b)
{
    Rectangle p = brief_panel();
    Rectangle rec = {p.x+p.width-230*su,p.y+40*su,190*su,60*su};
    if (b==1)
    {
        rec.x = p.x+40*su;
        rec.y = p.y+p.height-100*su;
        rec.width = 220*su;
        rec.height = 70*su;
    }
    if (b==2)
    {
        rec.x = p.x+p.width-300*su;
        rec.y = p.y+p.height-100*su;
        rec.width = 260*su;
        rec.height = 70*su;
    }
    return rec;
}


int brief_pages()
{
    return (brief_count[level-1]+4)/5;
}


void brief_step(float dt)
{
    //the level keeps moving behind the page and in the pictures
    background_step(level,dt);
    BeginTextureMode(card_texture[level-1]);
    ClearBackground(BLACK);
    draw_scene(level);
    EndTextureMode();

    Vector2 mouse = GetMousePosition();
    int last_page = brief_pages()-1;
    int next = IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        if (CheckCollisionPointRec(mouse,brief_button(0)))
        {
            play(snd_blip,0.9);
            game_mode = 2;
            return;
        }
        if (brief_page>0 && CheckCollisionPointRec(mouse,brief_button(1)))
        {
            play(snd_blip,0.9);
            brief_page--;
        }
        if (CheckCollisionPointRec(mouse,brief_button(2))) next = 1;
    }
    if (next)
    {
        play(snd_blip,0.9);
        if (brief_page<last_page) brief_page++;
        else game_mode = 2;
    }
    if (IsKeyPressed(KEY_ESCAPE) || (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && over(brief_menu_key()))) game_mode = 1;
}


//the page itself, in the level's own style
void draw_brief_panel(Rectangle p, int n)
{
    if (n==1)
    {
        //riveted steel with hazard stripes
        DrawRectangleGradientV(p.x,p.y,p.width,p.height,GetColor(0x5A5F66FF),GetColor(0x2E3136FF));
        Rectangle band = {p.x,p.y,p.width,18*su};
        l1_draw_hazard_stripes(band);
        band.y = p.y+p.height-18*su;
        l1_draw_hazard_stripes(band);
        for (int i=0; i<p.width/(60*su); i++)
        {
            DrawCircle(p.x+30*su+i*60*su,p.y+30*su,4*su,GetColor(0x8A9099FF));
            DrawCircle(p.x+30*su+i*60*su,p.y+p.height-30*su,4*su,GetColor(0x8A9099FF));
        }
        DrawRectangleLinesEx(p,5*su,BLACK);
    }
    if (n==2)
    {
        //driftwood planks with a rope edge
        DrawRectangleRec(p,GetColor(0x8B5E34FF));
        for (int i=1; i<p.height/(64*su); i++) DrawLine(p.x,p.y+i*64*su,p.x+p.width,p.y+i*64*su,GetColor(0x6E4526FF));
        for (int i=0; i<p.width/(24*su); i++)
        {
            DrawCircle(p.x+i*24*su,p.y,5*su,GetColor(0xD8B878FF));
            DrawCircle(p.x+i*24*su,p.y+p.height,5*su,GetColor(0xD8B878FF));
        }
        for (int i=0; i<p.height/(24*su); i++)
        {
            DrawCircle(p.x,p.y+i*24*su,5*su,GetColor(0xD8B878FF));
            DrawCircle(p.x+p.width,p.y+i*24*su,5*su,GetColor(0xD8B878FF));
        }
    }
    if (n==3)
    {
        //neon hologram with scanlines
        Rectangle glow = {p.x-8*su,p.y-8*su,p.width+16*su,p.height+16*su};
        DrawRectangleLinesEx(glow,8*su,Fade(brief_accent[2],0.2));
        DrawRectangleRec(p,Fade(GetColor(0x0A1030FF),0.9));
        for (int i=0; i<p.height/(4*su); i++) DrawLine(p.x,p.y+i*4*su,p.x+p.width,p.y+i*4*su,Fade(brief_accent[2],0.04));
        DrawRectangleLinesEx(p,2*su,brief_accent[2]);
        //corner brackets
        Vector2 top_left = {p.x,p.y};
        Vector2 top_right = {p.x+40*su,p.y};
        Vector2 left_down = {p.x,p.y+40*su};
        Vector2 bottom_right = {p.x+p.width,p.y+p.height};
        Vector2 bottom_left = {p.x+p.width-40*su,p.y+p.height};
        Vector2 right_up = {p.x+p.width,p.y+p.height-40*su};
        DrawLineEx(top_left,top_right,6*su,brief_accent[2]);
        DrawLineEx(top_left,left_down,6*su,brief_accent[2]);
        DrawLineEx(bottom_right,bottom_left,6*su,brief_accent[2]);
        DrawLineEx(bottom_right,right_up,6*su,brief_accent[2]);
    }
    if (n==4)
    {
        //carved stone tablet with gold trim and moss
        DrawRectangleGradientV(p.x,p.y,p.width,p.height,GetColor(0x7C8078FF),GetColor(0x4E534AFF));
        DrawRectangleLinesEx(p,6*su,brief_accent[3]);
        Rectangle inner = {p.x+14*su,p.y+14*su,p.width-28*su,p.height-28*su};
        DrawRectangleLinesEx(inner,2*su,Fade(brief_accent[3],0.7));
        Vector2 corner[4] = {{p.x,p.y},{p.x+p.width,p.y},{p.x,p.y+p.height},{p.x+p.width,p.y+p.height}};
        for (int i=0; i<4; i++)
        {
            DrawCircle(corner[i].x,corner[i].y,22*su,brief_accent[3]);
            DrawCircle(corner[i].x,corner[i].y,12*su,GetColor(0x4E534AFF));
        }
        for (int i=0; i<9; i++) DrawCircle(p.x+fmod(i*397*su,p.width),p.y+p.height-10*su-fmod(i*53*su,40*su),(10+i%4*4)*su,Fade(GetColor(0x5E7F3AFF),0.6));
    }
    if (n==5)
    {
        //a sheet of ice with frost creeping in from the corners
        DrawRectangleGradientV(p.x,p.y,p.width,p.height,GetColor(0x2E4C5EFF),GetColor(0x16242EFF));
        DrawRectangleLinesEx(p,6*su,brief_accent[4]);
        Rectangle inner = {p.x+14*su,p.y+14*su,p.width-28*su,p.height-28*su};
        DrawRectangleLinesEx(inner,2*su,Fade(brief_accent[4],0.5));
        for (int i=0; i<22; i++)
        {
            float x = p.x + fmod(i*331*su,p.width);
            float y = p.y + fmod(i*197*su,p.height);
            float reach = (26+i%5*10)*su;
            DrawLineEx((Vector2){x,y},(Vector2){x+reach,y-reach*0.6},2*su,Fade(brief_accent[4],0.25));
            DrawLineEx((Vector2){x,y},(Vector2){x-reach*0.7,y-reach*0.4},2*su,Fade(brief_accent[4],0.18));
        }
    }
    if (n==6)
    {
        //old wallpaper in candlelight, with a dark vignette
        DrawRectangleGradientV(p.x,p.y,p.width,p.height,GetColor(0x3A2C22FF),GetColor(0x1C1510FF));
        for (int i=0; i<16; i++)
        {
            float x = p.x + 40*su + i*((p.width-80*su)/15);
            DrawLineEx((Vector2){x,p.y+10*su},(Vector2){x,p.y+p.height-10*su},2*su,Fade(brief_accent[5],0.10));
        }
        DrawRectangleLinesEx(p,6*su,brief_accent[5]);
        Rectangle inner = {p.x+14*su,p.y+14*su,p.width-28*su,p.height-28*su};
        DrawRectangleLinesEx(inner,2*su,Fade(brief_accent[5],0.45));
        DrawCircleGradient((Vector2){p.x+p.width/2,p.y+p.height/2},p.width*0.62,BLANK,Fade(BLACK,0.55));
    }
    if (n==7)
    {
        //a slab of basalt with the cracks still glowing
        DrawRectangleGradientV(p.x,p.y,p.width,p.height,GetColor(0x34333AFF),GetColor(0x1A1A1EFF));
        for (int i=0; i<12; i++)
        {
            float x = p.x + fmod(i*521*su,p.width);
            float y = p.y + fmod(i*331*su,p.height);
            DrawLineEx((Vector2){x,y},(Vector2){x+(80+i%4*40)*su,y+(50+i%3*30)*su},3*su,Fade(brief_accent[6],0.35));
        }
        DrawRectangleLinesEx(p,6*su,brief_accent[6]);
        Rectangle inner = {p.x+14*su,p.y+14*su,p.width-28*su,p.height-28*su};
        DrawRectangleLinesEx(inner,2*su,Fade(brief_accent[6],0.6));
        DrawRectangleGradientV(p.x+6*su,p.y+p.height-70*su,p.width-12*su,64*su,BLANK,Fade(GetColor(0xFF6A1FFF),0.45));
    }
}


void draw_brief()
{
    float t = GetTime();
    int n = level;
    Color accent = brief_accent[n-1];
    Color text = brief_text_colour[n-1];
    Vector2 mouse = GetMousePosition();

    //the live level, dimmed, behind the page
    Rectangle whole = {0,0,screen_width,-screen_height};
    Rectangle screen = {0,0,screen_width,screen_height};
    Vector2 no_origin = {0,0};
    DrawTexturePro(card_texture[n-1].texture,whole,screen,no_origin,0,WHITE);
    DrawRectangle(0,0,screen_width,screen_height,Fade(BLACK,0.6));

    Rectangle p = brief_panel();
    draw_brief_panel(p,n);

    //title, difficulty and strokes, page number
    const char *difficulty_name[3] = {"BOT","CHAD","GOAT"};
    int strokes = l1_stroke_limit;
    if (n==2) strokes = l2_stroke_limit;
    if (n==3) strokes = l3_stroke_limit;
    if (n==4) strokes = l4_stroke_limit;
    if (n==5) strokes = l5_stroke_limit;
    if (n==6) strokes = l6_stroke_limit;
    if (n==7) strokes = l7_stroke_limit;
    DrawText(TextFormat("%s  -  what's waiting for you",level_name[n-1]),p.x+50*su+4*su,p.y+44*su+4*su,56*su,BLACK);
    DrawText(TextFormat("%s  -  what's waiting for you",level_name[n-1]),p.x+50*su,p.y+44*su,56*su,accent);
    DrawText(TextFormat("%s   |   %d strokes to sink it   |   page %d / %d",difficulty_name[current_difficulty],strokes,brief_page+1,brief_pages()),p.x+52*su,p.y+112*su,30*su,text);

    //up to 5 obstacles: live picture and name on the left, what it does on the right
    for (int row=0; row<5; row++)
    {
        int i = brief_page*5 + row;
        if (i>=brief_count[n-1]) break;
        float y = p.y + 170*su + row*128*su;
        Rectangle box = {p.x+50*su,y,184*su,115*su};
        DrawRectangleRec(box,BLACK);
        if (brief_x[n-1][i]>=0)
        {
            //cut the obstacle's spot out of the live level picture (it is stored upside down)
            float crop_w = 240*su;
            float crop_h = 150*su;
            float crop_x = Clamp(brief_x[n-1][i]*su - crop_w/2,0,screen_width-crop_w);
            float crop_y = Clamp(brief_y[n-1][i]*su - crop_h/2,0,screen_height-crop_h);
            Rectangle source = {crop_x,screen_height-crop_y-crop_h,crop_w,-crop_h};
            DrawTexturePro(card_texture[n-1].texture,source,box,no_origin,0,WHITE);
        }
        else if (brief_x[n-1][i]==-1)
        {
            //ufo sprite, lights chasing
            Rectangle source = {((int)(t*6)%3)*320,0,320,320};
            Rectangle dest = {box.x+box.width/2,box.y+box.height/2,105*su,105*su};
            Vector2 origin = {52.5*su,52.5*su};
            DrawRectangleRec(box,GetColor(0x06081AFF));
            DrawTexturePro(l3_ufo_texture,source,dest,origin,0,WHITE);
        }
        else if (brief_x[n-1][i]==-3)
        {
            //a pack of little rocks with orange tails flying through the box
            DrawRectangleRec(box,GetColor(0x06081AFF));
            BeginScissorMode(box.x,box.y,box.width,box.height);
            for (int j=0; j<5; j++)
            {
                float along = fmod(t*110*su + j*30*su,box.width+80*su) - 40*su;
                Vector2 rock = {box.x+along - j*12*su,box.y+18*su+j*20*su};
                Vector2 tail = {rock.x-26*su,rock.y-10*su};
                DrawLineEx(tail,rock,4*su,Fade(GetColor(0xFF9628FF),0.6));
                Rectangle source = {(j%3)*192,0,192,192};
                Rectangle dest = {rock.x,rock.y,20*su,20*su};
                Vector2 origin = {10*su,10*su};
                DrawTexturePro(l3_asteroid_texture,source,dest,origin,t*200+j*40,WHITE);
            }
            EndScissorMode();
        }
        else
        {
            //comet streaking across the little box
            DrawRectangleRec(box,GetColor(0x06081AFF));
            Vector2 head = {box.x+fmod(t*120*su,box.width+60*su)-20*su,box.y+box.height/2};
            BeginScissorMode(box.x,box.y,box.width,box.height);
            for (int j=10; j>0; j--) DrawCircle(head.x-j*9*su,head.y,(9-j*0.7)*su,Fade(brief_accent[2],0.5-j*0.04));
            DrawCircle(head.x,head.y,9*su,WHITE);
            EndScissorMode();
        }
        DrawRectangleLinesEx(box,3*su,accent);
        DrawText(brief_name[n-1][i],box.x+box.width+24*su,y+40*su,36*su,accent);
        DrawText(brief_text[n-1][i],p.x+640*su,y+22*su,30*su,text);
        if (row<4 && i+1<brief_count[n-1]) DrawLine(p.x+50*su,y+122*su,p.x+p.width-50*su,y+122*su,Fade(accent,0.3));
    }

    //buttons
    const char *label[3] = {"SKIP","BACK","NEXT"};
    if (brief_page==brief_pages()-1) label[2] = "PLAY!";
    for (int b=0; b<3; b++)
    {
        if (b==1 && brief_page==0) continue;
        Rectangle r = brief_button(b);
        if (CheckCollisionPointRec(mouse,r)) DrawRectangleRounded(r,0.3,8,Fade(accent,0.55));
        else DrawRectangleRounded(r,0.3,8,Fade(accent,0.25));
        DrawRectangleRoundedLinesEx(r,0.3,8,3*su,accent);
        int w = MeasureText(label[b],40*su);
        DrawText(label[b],r.x+r.width/2-w/2,r.y+r.height/2-20*su,40*su,text);
    }
    Rectangle menu_key = brief_menu_key();
    draw_key_button(menu_key,"ESC","menu",text,over(menu_key));
    DrawText("ENTER  next page",p.x+p.width/2+60*su,p.y+p.height-78*su,26*su,Fade(text,0.7));
}


//the pictures and the sounds are read out of assets/, so the game has to stand in its own
//folder, whatever folder it was started from (a shortcut, the command line, anywhere)
void go_to_my_own_folder()
{
    const char *home = GetApplicationDirectory();
    if (home!=0) ChangeDirectory(home);
}


//running the exe straight out of the zip leaves assets behind, and everything looks broken.
//say so, in a window, because there is no console to print to
void complain_about_missing_assets()
{
    InitWindow(960,460,"The Ultimate Golf");
    SetTargetFPS(30);
    SetExitKey(KEY_NULL);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(GetColor(0x131B2AFF));
        DrawText("The assets folder is missing",50,60,42,GetColor(0xFFD34DFF));
        DrawText("The game could not find its pictures and sounds.",50,140,26,RAYWHITE);
        DrawText("This happens when the exe is started from inside the zip.",50,180,26,RAYWHITE);
        DrawText("Right click the zip, choose \"Extract All\", open the folder",50,250,26,GetColor(0x6FE7FFFF));
        DrawText("it makes, and run TheUltimateGolf.exe from in there.",50,290,26,GetColor(0x6FE7FFFF));
        DrawText("The exe and the assets folder have to sit side by side.",50,360,24,GRAY);
        DrawText("ESC or the X closes this.",50,405,22,GRAY);
        if (IsKeyPressed(KEY_ESCAPE)) break;
        EndDrawing();
    }
    CloseWindow();
}


int main()
{
    go_to_my_own_folder();
    if (FileExists("assets/intro/intro_ball.png")==0)
    {
        complain_about_missing_assets();
        return 0;
    }
    InitWindow(1280,720,"the ultimate golf");
    int monitor = GetCurrentMonitor();
    screen_width = GetMonitorWidth(monitor);
    screen_height = GetMonitorHeight(monitor);
    ToggleBorderlessWindowed();
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    su = screen_height/1080.0;
    if (screen_width/1920.0 < su) su = screen_width/1920.0;

    l1_start(screen_width,screen_height);
    l2_start(screen_width,screen_height);
    l3_start(screen_width,screen_height);
    l4_start(screen_width,screen_height);
    l5_start(screen_width,screen_height);
    l6_start(screen_width,screen_height);
    l7_start(screen_width,screen_height);
    mascot_texture = LoadTexture("assets/intro/intro_ball.png");
    SetTextureFilter(mascot_texture,TEXTURE_FILTER_BILINEAR);
    developer_photo[0] = LoadTexture("assets/abdullah.png");
    developer_photo[1] = LoadTexture("assets/fahim.png");
    for (int i=0; i<2; i++) SetTextureFilter(developer_photo[i],TEXTURE_FILTER_BILINEAR);
    for (int i=0; i<LEVELS; i++) card_texture[i] = LoadRenderTexture(screen_width,screen_height);
    InitAudioDevice();
    load_sounds();
    load_scores();

    while(!WindowShouldClose() && quit==0)
    {
        float dt = GetFrameTime();
        if (dt>1.0/30) dt = 1.0/30;

        //the sound switch: M anywhere, the big speaker on the menu pages, the small one in a level
        Rectangle speaker = game_mode==2 ? game_sound_button() : sound_button();
        int on_speaker = CheckCollisionPointRec(GetMousePosition(),speaker) && game_mode!=3;
        int sound_clicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && on_speaker;
        if ((IsKeyPressed(KEY_M) && game_mode!=4) || sound_clicked)
        {
            sound_on = !sound_on;
            apply_sound_switch();
            play(snd_blip,0.9);
        }
        //the buttons drawn on top of a level: the same things R and ESC do
        if (game_mode==2 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            int restart = over(hud_key(0)) || (level_state()!=0 && over(end_key(0)));
            int to_menu = over(hud_key(1)) || (level_state()!=0 && over(end_key(1)));
            if (restart==1 || to_menu==1)
            {
                play(snd_blip,0.9);
                ui_press = 1;
                if (restart==1) restart_level();
                if (to_menu==1) game_mode = 1;
            }
        }
        //a press on any of them must not also become a golf shot, so the level is left
        //alone until that press is let go again
        if (sound_clicked && game_mode==2) ui_press = 1;
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) ui_press = 0;

        if (game_mode==0)
        {
            intro_time = intro_time + dt;
            int part = intro_time/1.6;
            if (part>3) part = 3;
            background_step(part+1,dt);

            //boing when the mascot lands, bang when the title lands
            float hop = intro_hop(intro_time);
            if (intro_last_hop>=0.05 && hop<0.05) play(snd_hop,0.7);
            intro_last_hop = hop;
            float title_time = intro_time - 3*1.6 - 0.8;
            if (title_time>=0.15 && title_time-dt<0.15) play(snd_slam,1);

            //click or any key skips to the menu
            if (intro_time>0.3 && (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || GetKeyPressed()!=0))
            {
                game_mode = 4;
                while (GetCharPressed()>0);     //the key that skipped the intro must not land in the name box
            }
        }
        else if (game_mode==1)
        {
            //blip when the mouse moves onto a card or button, and on a click that does something
            int chosen_before = chosen_level;
            Vector2 mouse = GetMousePosition();
            int hover = -1;
            for (int i=0; i<4; i++)
            {
                if (chosen_level==0 && CheckCollisionPointRec(mouse,card_rect(i))) hover = i;
            }
            for (int d=0; d<3; d++)
            {
                if (chosen_level>0 && CheckCollisionPointRec(mouse,difficulty_rect(d))) hover = 10+d;
            }
            if (hover>=0 && hover!=last_hover) play(snd_blip,0.4);
            last_hover = hover;

            if (chosen_level==0 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && over(menu_quit_key())) quit = 1;
            if (chosen_level==0 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                for (int b=0; b<3; b++)
                {
                    if (CheckCollisionPointRec(mouse,menu_button(b)))
                    {
                        play(snd_blip,0.9);
                        game_mode = 5+b;
                    }
                }
            }
            if (game_mode!=1) continue;

            menu_step(dt);
            if (chosen_level!=chosen_before || game_mode!=1) play(snd_blip,0.9);
        }
        else if (game_mode==4)
        {
            //typing the name: letters, digits and spaces only, so the '|' in the file is safe
            int key = GetCharPressed();
            while (key>0)
            {
                if (key>='a' && key<='z') key = key-32;
                int allowed = (key>='A' && key<='Z') || (key>='0' && key<='9') || key==' ';
                if (allowed==1 && name_length<12)
                {
                    player_name[name_length] = key;
                    name_length++;
                    player_name[name_length] = 0;
                    play(snd_blip,0.4);
                }
                key = GetCharPressed();
            }
            int click = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
            if ((IsKeyPressed(KEY_BACKSPACE) || (click==1 && over(name_key(1)))) && name_length>0)
            {
                name_length--;
                player_name[name_length] = 0;
                play(snd_blip,0.4);
            }
            if ((IsKeyPressed(KEY_ENTER) || (click==1 && over(name_key(0)))) && name_length>0)
            {
                pick_player(player_name);
                play(snd_blip,0.9);
                game_mode = 1;
            }
        }
        else if (game_mode>=5 && game_mode<=7)
        {
            int back = IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(),page_back_button());
            if (IsKeyPressed(KEY_ESCAPE) || back==1)
            {
                play(snd_blip,0.9);
                game_mode = 1;
            }
        }
        else if (game_mode==3) brief_step(dt);
        else
        {
            if (IsKeyPressed(KEY_ESCAPE)) game_mode = 1;
            else if (ui_press==0)
            {
                remember_level();
                if (level==1) l1_update();
                if (level==2) l2_update();
                if (level==3) l3_update();
                if (level==4) l4_update();
                if (level==5) l5_update();
                if (level==6) l6_update();
                if (level==7) l7_update();
                level_sounds();
                if (level_state()==0) score_saved = 0;
                if (level_state()==1 && score_saved==0)
                {
                    record_score(level,score_for(level_limit(),level_strokes(),current_difficulty));
                    score_saved = 1;
                }
            }
        }

        //music only belongs to a level: the intro, the menu and the pages are quiet
        if ((game_mode==2 || game_mode==3) && level<=4) play_music(level);
        else if (music_playing>=0)
        {
            StopMusicStream(music[music_playing]);
            StopMusicStream(music[5]);
            music_playing = -1;
        }
        update_music();
        if (game_mode==2 && level==1)
        {
            if (IsSoundPlaying(snd_fan)==0) play(snd_fan,0.5);
        }
        else if (IsSoundPlaying(snd_fan)) StopSound(snd_fan);

        BeginDrawing();
        if (game_mode==0) draw_intro();
        if (game_mode==1) draw_menu();
        if (game_mode==3) draw_brief();
        if (game_mode==4) draw_name_entry();
        if (game_mode==5) draw_how_to_play();
        if (game_mode==6) draw_leaderboard();
        if (game_mode==7) draw_credits();
        if (game_mode==2)
        {
            if (level==1) l1_draw();
            if (level==2) l2_draw();
            if (level==3) l3_draw();
            if (level==4) l4_draw();
            if (level==5) l5_draw();
            if (level==6) l6_draw();
            if (level==7) l7_draw();
            if (level_state()==1) draw_score_panel();
            draw_level_keys();
            Rectangle small = game_sound_button();
            draw_speaker(small,sound_on,CheckCollisionPointRec(GetMousePosition(),small));
        }
        EndDrawing();
    }

    unload_sounds();
    CloseAudioDevice();
    for (int i=0; i<LEVELS; i++) UnloadRenderTexture(card_texture[i]);
    UnloadTexture(mascot_texture);
    for (int i=0; i<2; i++) UnloadTexture(developer_photo[i]);
    l2_unload();
    l3_unload();
    l4_unload();
    l5_unload();
    l6_unload();
    l7_unload();
    CloseWindow();
    return 0;
}
