//==================== LEVEL 6 ====================
//Haunted Manor: the house is dark and the ball carries one small light. Candles it rolls
//over stay lit for the rest of the level, so the room gets easier to read the more strokes
//are spent in it. The clock in the hall rules the walls that come and go.

//full global
int l6_width = 1920;
int l6_height = 1080;
float l6_u = 1;
int l6_stroke_base = 26;
int l6_stroke_limit = 26;
#define l6_max_speed 650

//colours
Color l6_wall = {58,44,36,255};
Color l6_wall_light = {86,66,52,255};
Color l6_wall_dark = {31,23,18,255};
Color l6_rug = {96,32,38,255};
Color l6_candle_warm = {255,186,88,255};
Color l6_ghost_blue = {198,226,246,255};
Color l6_gold = {196,160,78,255};
Color l6_crypt_green = {120,190,130,255};

//pictures
Texture2D l6_floor_texture;
Texture2D l6_ghost_texture;
Texture2D l6_candle_texture;
Texture2D l6_chandelier_texture;
Texture2D l6_mirror_texture;
Texture2D l6_cobweb_texture;
Texture2D l6_bat_texture;
Texture2D l6_wisp_texture;
Texture2D l6_lantern_texture;
Texture2D l6_sparks_texture;
Texture2D l6_fireplace_texture;
Texture2D l6_rat_texture;
Texture2D l6_portrait_texture;
Texture2D l6_clock_texture;
Texture2D l6_curtain_texture;

//the darkness: everything is drawn, then this mask multiplies it down to nothing
//except where a light is
RenderTexture2D l6_light_mask;
int l6_mask_ready = 0;
//how much light there is with no candle at all. The menu card uses more, or the card
//would just be a black rectangle next to the other six.
int l6_ambient = 30;

//ball and pot
Vector2 l6_ball;
Vector2 l6_speed;
float l6_radius_ball;
Vector2 l6_pot;
float l6_radius_pot;
Vector2 l6_start_position;
Vector2 l6_last_shot_position;
int l6_stroke = 0;
int l6_game_state = 0;
int l6_aiming = 0;
float l6_animation_time = 0;
float l6_hud_height;

//messages: 1 fell through a trapdoor, 2 the ghosts got it, 3 took the hidden way down
int l6_message_type = 0;
float l6_message_timer = 0;

//the house: solid walls, and the floor is safe everywhere else
Rectangle l6_wall_rect[16];
#define l6_wall_count 16

//walls that are only there half the time, flipping on the clock's chime
Rectangle l6_phase_wall[3];
int l6_phase_on = 1;
float l6_clock_timer = 0;
float l6_chime_flash = 0;

//candles: unlit until the ball rolls over them, then lit for good
Vector2 l6_candle[7];
int l6_candle_lit[7];
float l6_candle_radius;

//wall lamps that flare for about a second, at random, and show a slice of the house
Vector2 l6_lamp[4];
float l6_lamp_wait[4];
float l6_lamp_on[4];

//the hidden trapdoor, only visible in a lamp flash: it drops to the far side of the library
Rectangle l6_secret;
Vector2 l6_secret_exit;
float l6_secret_glow = 0;

//trapdoors that open and shut: open means a hole in the floor
Rectangle l6_trapdoor[2];
float l6_trapdoor_clock[2];

//ghosts drift along a line and shove the ball, but only where it is dark
Vector2 l6_ghost_from[3];
Vector2 l6_ghost_to[3];
float l6_ghost_travel[3];
float l6_ghost_speed[3];
float l6_ghost_radius;

//bats cross the rooms on a loop
Rectangle l6_bat[3];
float l6_bat_left[3];
float l6_bat_right[3];
float l6_bat_speed[3];

//cobwebs slow the ball right down
Rectangle l6_cobweb[3];

//the chandelier drops when the ball crosses under it
Vector2 l6_chandelier;
int l6_chandelier_state = 0;      //0 hanging, 1 falling, 2 down
float l6_chandelier_fall = 0;

//a pair of mirrors: into one, out of the other
Vector2 l6_mirror[2];
float l6_mirror_facing[2];
float l6_mirror_cooldown = 0;

//--- everything that moves in here is a light, so the room is never still and never
//--- unreadable for long ---
//will-o-wisps drift their own routes carrying a cold green glow
Vector2 l6_wisp[3];
Vector2 l6_wisp_from[3];
Vector2 l6_wisp_to[3];
float l6_wisp_travel[3];
float l6_wisp_speed[3];

//a lantern swinging on its chain: the pool of light swings with it
Vector2 l6_lantern_anchor;
float l6_lantern_swing = 0;
Vector2 l6_lantern;

//a broken sconce that throws sparks every few seconds
Vector2 l6_sconce;
float l6_spark_wait = 0;
float l6_spark_stage = 0;

//the fireplace: a slow breathing glow at one end of the hall
Vector2 l6_fire;
float l6_fire_pulse = 0;

//lightning through the windows shows the whole house for a moment
float l6_storm_wait = 0;
float l6_storm_flash = 0;

//rats crossing the floor, portraits that follow the ball, a curtain at a broken window
Vector2 l6_rat[3];
Vector2 l6_rat_from[3];
Vector2 l6_rat_to[3];
float l6_rat_travel[3];
float l6_rat_speed[3];
Vector2 l6_portrait[3];
Vector2 l6_window[2];
Vector2 l6_clock_at;

//two plates hold the crypt shut until both are pressed
Vector2 l6_plate[2];
int l6_plate_down[2];
float l6_plate_radius;
Rectangle l6_crypt_door;
float l6_crypt_open = 0;


Rectangle l6_make_rect(float x, float y, float w, float h)
{
    Rectangle rec = {x*l6_u,y*l6_u,w*l6_u,h*l6_u};
    return rec;
}

Vector2 l6_make_point(float x, float y)
{
    Vector2 point = {x*l6_u,y*l6_u};
    return point;
}


void l6_reset_level()
{
    l6_hud_height = 70*l6_u;
    l6_radius_ball = 7*l6_u;
    l6_radius_pot = 11*l6_u;

    //SIX ROOMS in two rows, with the doorways deliberately offset so the house is a
    //long circuit rather than one big hall:
    //   hall -> library -> conservatory -> up the east stair -> gallery -> study -> crypt
    //The outer walls
    l6_wall_rect[0] = l6_make_rect(40,90,1840,30);        //top
    l6_wall_rect[1] = l6_make_rect(40,1000,1840,30);      //bottom
    l6_wall_rect[2] = l6_make_rect(40,90,30,940);         //left
    l6_wall_rect[3] = l6_make_rect(1850,90,30,940);       //right

    //the wall between the two rows, with one way up at the east end
    l6_wall_rect[4] = l6_make_rect(70,540,1480,30);
    l6_wall_rect[5] = l6_make_rect(1700,540,150,30);

    //the bottom row, split into three rooms. The doorways are at opposite ends.
    l6_wall_rect[6] = l6_make_rect(660,570,30,290);       //hall | library, doorway low
    l6_wall_rect[7] = l6_make_rect(1250,730,30,300);      //library | conservatory, doorway high

    //the top row, split into three. Again the doorways alternate.
    l6_wall_rect[8] = l6_make_rect(660,270,30,270);       //study | gallery, doorway high
    l6_wall_rect[9] = l6_make_rect(1250,120,30,290);      //gallery | east stair, doorway low

    //a few short walls inside the rooms, to make the shots interesting
    l6_wall_rect[10] = l6_make_rect(260,760,250,30);
    l6_wall_rect[11] = l6_make_rect(900,640,230,30);
    l6_wall_rect[12] = l6_make_rect(1480,820,240,30);
    l6_wall_rect[13] = l6_make_rect(880,330,220,30);
    //the crypt is walled off in the far corner: a ceiling over it and a stub of wall
    //under the door, so the door has somewhere to slide down into
    l6_wall_rect[14] = l6_make_rect(70,280,360,30);
    l6_wall_rect[15] = l6_make_rect(400,430,30,110);

    //walls that come and go on the chime, one per row
    l6_phase_wall[0] = l6_make_rect(960,790,30,210);
    l6_phase_wall[1] = l6_make_rect(1560,120,30,220);
    l6_phase_wall[2] = l6_make_rect(480,120,30,160);
    l6_phase_on = 1;
    l6_clock_timer = 0;
    l6_chime_flash = 0;

    //candles: one or two per room, so every room can be lit as you pass through it
    l6_candle_radius = 14*l6_u;
    l6_candle[0] = l6_make_point(300,900);
    l6_candle[1] = l6_make_point(560,660);
    l6_candle[2] = l6_make_point(900,900);
    l6_candle[3] = l6_make_point(1150,620);
    l6_candle[4] = l6_make_point(1620,700);
    l6_candle[5] = l6_make_point(1000,200);
    l6_candle[6] = l6_make_point(540,470);
    for (int i=0; i<7; i++) l6_candle_lit[i] = 0;

    //the lamps that flare on their own, spread over the whole house
    l6_lamp[0] = l6_make_point(430,620);
    l6_lamp[1] = l6_make_point(1150,960);
    l6_lamp[2] = l6_make_point(1750,330);
    l6_lamp[3] = l6_make_point(780,170);
    for (int i=0; i<4; i++)
    {
        l6_lamp_wait[i] = 2 + i*2.6;
        l6_lamp_on[i] = 0;
    }

    //the hidden way down: in the conservatory, it drops you into the study
    l6_secret = l6_make_rect(1700,930,90,90);
    l6_secret_exit = l6_make_point(980,470);
    l6_secret_glow = 0;

    //trapdoors, one in each row
    l6_trapdoor[0] = l6_make_rect(820,760,110,110);
    l6_trapdoor[1] = l6_make_rect(1380,180,110,110);
    l6_trapdoor_clock[0] = 0;
    l6_trapdoor_clock[1] = 3;

    //ghosts drift the long corridors
    l6_ghost_radius = 24*l6_u;
    l6_ghost_from[0] = l6_make_point(180,640);
    l6_ghost_to[0] = l6_make_point(560,960);
    l6_ghost_speed[0] = 0.2;
    l6_ghost_from[1] = l6_make_point(1340,620);
    l6_ghost_to[1] = l6_make_point(1760,900);
    l6_ghost_speed[1] = 0.26;
    l6_ghost_from[2] = l6_make_point(760,200);
    l6_ghost_to[2] = l6_make_point(1150,440);
    l6_ghost_speed[2] = 0.23;
    for (int i=0; i<3; i++) l6_ghost_travel[i] = i*0.3;

    //bats cross three of the rooms
    l6_bat[0] = l6_make_rect(300,700,34,22);
    l6_bat_left[0] = 120*l6_u;
    l6_bat_right[0] = 600*l6_u;
    l6_bat_speed[0] = 170*l6_u;
    l6_bat[1] = l6_make_rect(1400,960,34,22);
    l6_bat_left[1] = 1320*l6_u;
    l6_bat_right[1] = 1800*l6_u;
    l6_bat_speed[1] = -150*l6_u;
    l6_bat[2] = l6_make_rect(800,440,34,22);
    l6_bat_left[2] = 720*l6_u;
    l6_bat_right[2] = 1200*l6_u;
    l6_bat_speed[2] = 160*l6_u;

    //cobwebs in three corners
    l6_cobweb[0] = l6_make_rect(80,580,150,130);
    l6_cobweb[1] = l6_make_rect(1300,580,150,130);
    l6_cobweb[2] = l6_make_rect(80,130,150,130);

    //the chandelier hangs over the library
    l6_chandelier = l6_make_point(1000,860);
    l6_chandelier_state = 0;
    l6_chandelier_fall = 0;

    //the mirrors: gallery to hall, the long way round in one hop
    l6_mirror[0] = l6_make_point(1700,180);
    l6_mirror_facing[0] = 90;
    l6_mirror[1] = l6_make_point(220,190);
    l6_mirror_facing[1] = 0;
    l6_mirror_cooldown = 0;

    //the plates that open the crypt, one in each row and far apart
    l6_plate_radius = 30*l6_u;
    l6_plate[0] = l6_make_point(1620,440);
    l6_plate[1] = l6_make_point(760,960);
    l6_plate_down[0] = 0;
    l6_plate_down[1] = 0;
    l6_crypt_door = l6_make_rect(400,310,30,120);
    l6_crypt_open = 0;

    //wisps drifting the two long rows
    l6_wisp_from[0] = l6_make_point(360,960);
    l6_wisp_to[0] = l6_make_point(1150,700);
    l6_wisp_speed[0] = 0.13;
    l6_wisp_from[1] = l6_make_point(1400,640);
    l6_wisp_to[1] = l6_make_point(1780,980);
    l6_wisp_speed[1] = 0.19;
    l6_wisp_from[2] = l6_make_point(700,180);
    l6_wisp_to[2] = l6_make_point(1200,470);
    l6_wisp_speed[2] = 0.16;
    for (int i=0; i<3; i++)
    {
        l6_wisp_travel[i] = i*0.4;
        l6_wisp[i] = l6_wisp_from[i];
    }

    //the swinging lantern hangs in the middle of the library
    l6_lantern_anchor = l6_make_point(960,700);
    l6_lantern = l6_lantern_anchor;
    l6_lantern_swing = 0;

    //the broken sconce, high in the gallery
    l6_sconce = l6_make_point(1480,140);
    l6_spark_wait = 3;
    l6_spark_stage = 0;

    //the fire in the hall
    l6_fire = l6_make_point(140,700);
    l6_fire_pulse = 0;

    //the storm outside
    l6_storm_wait = 7;
    l6_storm_flash = 0;

    //rats along the skirting
    l6_rat_from[0] = l6_make_point(120,980);
    l6_rat_to[0] = l6_make_point(560,980);
    l6_rat_speed[0] = 0.33;
    l6_rat_from[1] = l6_make_point(760,600);
    l6_rat_to[1] = l6_make_point(1200,600);
    l6_rat_speed[1] = 0.28;
    l6_rat_from[2] = l6_make_point(1400,140);
    l6_rat_to[2] = l6_make_point(1800,140);
    l6_rat_speed[2] = 0.4;
    for (int i=0; i<3; i++)
    {
        l6_rat_travel[i] = i*0.5;
        l6_rat[i] = l6_rat_from[i];
    }

    //portraits watch from three rooms, windows in two
    l6_portrait[0] = l6_make_point(430,580);
    l6_portrait[1] = l6_make_point(1480,620);
    l6_portrait[2] = l6_make_point(1120,150);
    l6_window[0] = l6_make_point(900,130);
    l6_window[1] = l6_make_point(1700,560);
    l6_clock_at = l6_make_point(620,180);

    //ball and pot: the hall at one end, the crypt at the other
    l6_start_position = l6_make_point(150,940);
    l6_ball = l6_start_position;
    l6_last_shot_position = l6_start_position;
    l6_speed.x = 0;
    l6_speed.y = 0;
    l6_pot = l6_make_point(200,470);
    l6_stroke = 0;
    l6_game_state = 0;
    l6_aiming = 0;
    l6_message_type = 0;
    l6_message_timer = 0;
}


int l6_bounce_off_rectangle(Rectangle rec, Vector2 rec_speed)
{
    if (CheckCollisionCircleRec(l6_ball,l6_radius_ball,rec))
    {
        Vector2 collision_point;
        collision_point.x = Clamp(l6_ball.x,rec.x,rec.x+rec.width);
        collision_point.y = Clamp(l6_ball.y,rec.y,rec.y+rec.height);
        Vector2 normal = Vector2Subtract(l6_ball,collision_point);
        if (normal.x==0 && normal.y==0)
        {
            float left = l6_ball.x - rec.x;
            float right = rec.x + rec.width - l6_ball.x;
            float top = l6_ball.y - rec.y;
            float bottom = rec.y + rec.height - l6_ball.y;
            if (left<=right && left<=top && left<=bottom) { normal.x = -1; collision_point.x = rec.x; }
            else if (right<=top && right<=bottom) { normal.x = 1; collision_point.x = rec.x + rec.width; }
            else if (top<=bottom) { normal.y = -1; collision_point.y = rec.y; }
            else { normal.y = 1; collision_point.y = rec.y + rec.height; }
        }
        normal = Vector2Normalize(normal);
        l6_ball = Vector2Add(collision_point,Vector2Scale(normal,l6_radius_ball));
        Vector2 relative_speed = Vector2Subtract(l6_speed,rec_speed);
        if (Vector2DotProduct(relative_speed,normal)<0)
        {
            relative_speed = Vector2Reflect(relative_speed,normal);
            l6_speed = Vector2ClampValue(Vector2Add(relative_speed,rec_speed),0,l6_max_speed*l6_u);
        }
        return 1;
    }
    return 0;
}


int l6_bounce_off_circle(Vector2 center, float radius, float bounce)
{
    Vector2 normal = Vector2Subtract(l6_ball,center);
    float distance = Vector2Length(normal);
    if (distance < radius + l6_radius_ball)
    {
        if (distance==0) normal.y = -1;
        normal = Vector2Normalize(normal);
        l6_ball = Vector2Add(center,Vector2Scale(normal,radius + l6_radius_ball));
        if (Vector2DotProduct(l6_speed,normal)<0)
        {
            l6_speed = Vector2ClampValue(Vector2Scale(Vector2Reflect(l6_speed,normal),bounce),0,l6_max_speed*l6_u);
            return 1;
        }
    }
    return 0;
}


//how much light reaches a spot: the ball's own light, every lit candle, every lamp that is flaring
float l6_light_at(Vector2 point)
{
    float best = 0;
    float d = Vector2Distance(point,l6_ball);
    if (d < 210*l6_u) best = 1 - d/(210*l6_u);
    for (int i=0; i<7; i++)
    {
        if (l6_candle_lit[i]==0) continue;
        d = Vector2Distance(point,l6_candle[i]);
        if (d < 230*l6_u)
        {
            float here = 1 - d/(230*l6_u);
            if (here>best) best = here;
        }
    }
    for (int i=0; i<4; i++)
    {
        if (l6_lamp_on[i]<=0) continue;
        d = Vector2Distance(point,l6_lamp[i]);
        if (d < 420*l6_u)
        {
            float here = (1 - d/(420*l6_u))*l6_lamp_on[i];
            if (here>best) best = here;
        }
    }
    //the lights that move count too: a wisp drifting past really does banish a ghost
    for (int i=0; i<3; i++)
    {
        d = Vector2Distance(point,l6_wisp[i]);
        if (d < 180*l6_u)
        {
            float here = (1 - d/(180*l6_u))*0.8;
            if (here>best) best = here;
        }
    }
    d = Vector2Distance(point,l6_lantern);
    if (d < 260*l6_u)
    {
        float here = 1 - d/(260*l6_u);
        if (here>best) best = here;
    }
    d = Vector2Distance(point,l6_fire);
    if (d < 300*l6_u)
    {
        float here = (1 - d/(300*l6_u))*l6_fire_pulse;
        if (here>best) best = here;
    }
    if (l6_storm_flash>0.3 && best<l6_storm_flash) best = l6_storm_flash;
    return best;
}


//the mask is built outside the drawing, because it writes into a texture of its own
void l6_build_light_mask()
{
    if (l6_mask_ready==0) return;
    BeginTextureMode(l6_light_mask);
    ClearBackground((Color){l6_ambient,(unsigned char)(l6_ambient*0.9),(unsigned char)(l6_ambient*0.8),255});   //never pitch black: shapes stay readable
    BeginBlendMode(BLEND_ADDITIVE);
    DrawCircleGradient(l6_ball,230*l6_u,Fade(WHITE,0.85),BLANK);
    DrawCircleGradient(l6_ball,90*l6_u,Fade(WHITE,0.75),BLANK);
    for (int i=0; i<7; i++)
    {
        if (l6_candle_lit[i]==1) DrawCircleGradient(l6_candle[i],250*l6_u,Fade(l6_candle_warm,0.9),BLANK);
    }
    for (int i=0; i<4; i++)
    {
        if (l6_lamp_on[i]>0) DrawCircleGradient(l6_lamp[i],420*l6_u,Fade(WHITE,0.9*l6_lamp_on[i]),BLANK);
    }
    //the moving lights: wisps, the swinging lantern, the fire, the sparks, the lightning
    for (int i=0; i<3; i++) DrawCircleGradient(l6_wisp[i],180*l6_u,Fade(GetColor(0x6FE7D0FF),0.75),BLANK);
    DrawCircleGradient(l6_lantern,260*l6_u,Fade(l6_candle_warm,0.85),BLANK);
    DrawCircleGradient(l6_fire,300*l6_u,Fade(GetColor(0xFF8A3CFF),0.75*l6_fire_pulse),BLANK);
    if (l6_spark_stage>0) DrawCircleGradient(l6_sconce,230*l6_u,Fade(GetColor(0xFFD27AFF),0.9*(1-l6_spark_stage/0.8)),BLANK);
    if (l6_storm_flash>0)
    {
        DrawRectangle(0,0,l6_width,l6_height,Fade(GetColor(0xBFD4E8FF),0.55*l6_storm_flash));
        for (int i=0; i<2; i++) DrawCircleGradient(l6_window[i],520*l6_u,Fade(WHITE,0.8*l6_storm_flash),BLANK);
    }
    if (l6_chime_flash>0) DrawRectangle(0,0,l6_width,l6_height,Fade(WHITE,0.22*l6_chime_flash));
    //the crypt is always a little lit, so the end of the level can be found
    DrawCircleGradient(l6_pot,180*l6_u,Fade(l6_crypt_green,0.55),BLANK);
    EndBlendMode();
    EndTextureMode();
}


void l6_update_obstacles(float dt)
{
    l6_animation_time = l6_animation_time + dt;

    //wisps drift out and back along their line
    for (int i=0; i<3; i++)
    {
        l6_wisp_travel[i] = l6_wisp_travel[i] + dt*l6_wisp_speed[i];
        if (l6_wisp_travel[i]>2) l6_wisp_travel[i] = l6_wisp_travel[i] - 2;
        float t = l6_wisp_travel[i];
        if (t>1) t = 2 - t;
        l6_wisp[i] = Vector2Lerp(l6_wisp_from[i],l6_wisp_to[i],t);
    }

    //the lantern swings, and so does its light
    l6_lantern_swing = sin(l6_animation_time*1.6)*120*l6_u;
    l6_lantern.x = l6_lantern_anchor.x + l6_lantern_swing;
    l6_lantern.y = l6_lantern_anchor.y + fabsf(l6_lantern_swing)*0.12;

    //the sconce: a long wait, then a short shower of sparks
    if (l6_spark_stage>0)
    {
        l6_spark_stage = l6_spark_stage + dt;
        if (l6_spark_stage>0.8)
        {
            l6_spark_stage = 0;
            l6_spark_wait = 4 + GetRandomValue(0,40)/10.0;
        }
    }
    else
    {
        l6_spark_wait = l6_spark_wait - dt;
        if (l6_spark_wait<=0) l6_spark_stage = 0.0001;
    }

    //the fire breathes
    l6_fire_pulse = 0.78 + 0.22*sin(l6_animation_time*2.1) + 0.06*sin(l6_animation_time*7.3);

    //lightning: rare, bright, and it shows the whole house
    if (l6_storm_flash>0) l6_storm_flash = l6_storm_flash - dt*2.2;
    else
    {
        l6_storm_wait = l6_storm_wait - dt;
        if (l6_storm_wait<=0)
        {
            l6_storm_flash = 1;
            l6_storm_wait = 11 + GetRandomValue(0,90)/10.0;
        }
    }

    //rats scurry across and back
    for (int i=0; i<3; i++)
    {
        l6_rat_travel[i] = l6_rat_travel[i] + dt*l6_rat_speed[i];
        if (l6_rat_travel[i]>2) l6_rat_travel[i] = l6_rat_travel[i] - 2;
        float t = l6_rat_travel[i];
        if (t>1) t = 2 - t;
        l6_rat[i] = Vector2Lerp(l6_rat_from[i],l6_rat_to[i],t);
    }

    //the grandfather clock: every 8 seconds it chimes and the phasing walls swap over
    l6_clock_timer = l6_clock_timer + dt;
    if (l6_clock_timer>=8)
    {
        l6_clock_timer = l6_clock_timer - 8;
        l6_phase_on = !l6_phase_on;
        l6_chime_flash = 1;
    }
    if (l6_chime_flash>0) l6_chime_flash = l6_chime_flash - dt*2;

    //the lamps: a long wait, then about a second of light
    for (int i=0; i<4; i++)
    {
        if (l6_lamp_on[i]>0)
        {
            l6_lamp_on[i] = l6_lamp_on[i] - dt*1.1;
            if (l6_lamp_on[i]<0) l6_lamp_on[i] = 0;
        }
        else
        {
            l6_lamp_wait[i] = l6_lamp_wait[i] - dt;
            if (l6_lamp_wait[i]<=0)
            {
                l6_lamp_on[i] = 1;
                l6_lamp_wait[i] = 7 + GetRandomValue(0,60)/10.0;
            }
        }
    }
    if (l6_secret_glow>0) l6_secret_glow = l6_secret_glow - dt;

    //trapdoors: 6 seconds shut, 3 seconds open
    for (int i=0; i<2; i++)
    {
        l6_trapdoor_clock[i] = l6_trapdoor_clock[i] + dt;
        if (l6_trapdoor_clock[i]>=9) l6_trapdoor_clock[i] = l6_trapdoor_clock[i] - 9;
    }

    //ghosts drift up and down their line
    for (int i=0; i<3; i++)
    {
        l6_ghost_travel[i] = l6_ghost_travel[i] + dt*l6_ghost_speed[i];
        if (l6_ghost_travel[i]>2) l6_ghost_travel[i] = l6_ghost_travel[i] - 2;
    }

    //bats
    for (int i=0; i<3; i++)
    {
        l6_bat[i].x = l6_bat[i].x + l6_bat_speed[i]*dt;
        if (l6_bat[i].x<l6_bat_left[i])
        {
            l6_bat[i].x = l6_bat_left[i];
            l6_bat_speed[i] = fabsf(l6_bat_speed[i]);
        }
        if (l6_bat[i].x>l6_bat_right[i])
        {
            l6_bat[i].x = l6_bat_right[i];
            l6_bat_speed[i] = -fabsf(l6_bat_speed[i]);
        }
    }

    //the chandelier, once it has been knocked loose
    if (l6_chandelier_state==1)
    {
        l6_chandelier_fall = l6_chandelier_fall + dt;
        if (l6_chandelier_fall>1.4) l6_chandelier_state = 2;
    }

    //the crypt door slides open while both plates are down
    int both = l6_plate_down[0] && l6_plate_down[1];
    if (both==1 && l6_crypt_open<1) l6_crypt_open = l6_crypt_open + dt*2;
    if (both==0 && l6_crypt_open>0) l6_crypt_open = l6_crypt_open - dt*2;
    if (l6_crypt_open>1) l6_crypt_open = 1;
    if (l6_crypt_open<0) l6_crypt_open = 0;

    if (l6_mirror_cooldown>0) l6_mirror_cooldown = l6_mirror_cooldown - dt;
}


//where a ghost is at this moment (it goes out and comes back on one travel number)
Vector2 l6_ghost_at(int i)
{
    float t = l6_ghost_travel[i];
    if (t>1) t = 2 - t;
    return Vector2Lerp(l6_ghost_from[i],l6_ghost_to[i],t);
}


void l6_send_ball_back()
{
    l6_ball = l6_last_shot_position;
    //never straight back onto an open trapdoor
    for (int i=0; i<2; i++)
    {
        if (CheckCollisionPointRec(l6_ball,l6_trapdoor[i])) l6_ball = l6_start_position;
    }
    l6_speed.x = 0;
    l6_speed.y = 0;
    //both plates pop back up, the same rule the temple uses
    l6_plate_down[0] = 0;
    l6_plate_down[1] = 0;
}


void l6_update_ball(float dt)
{
    //moving
    l6_ball = Vector2Add(l6_ball,Vector2Scale(l6_speed,dt));

    //friction: boards normally, cobwebs almost stop the ball
    float friction = 150*l6_u;
    for (int i=0; i<3; i++)
    {
        if (CheckCollisionPointRec(l6_ball,l6_cobweb[i])) friction = 480*l6_u;
    }
    float ball_speed = Vector2Length(l6_speed);
    if (ball_speed>0)
    {
        float new_speed = ball_speed - friction*dt;
        if (new_speed<0) new_speed = 0;
        l6_speed = Vector2Scale(l6_speed,new_speed/ball_speed);
    }
    if (Vector2Length(l6_speed)<6*l6_u)
    {
        l6_speed.x = 0;
        l6_speed.y = 0;
    }

    //candles light up when the ball touches them, and stay lit
    for (int i=0; i<7; i++)
    {
        if (l6_candle_lit[i]==0 && Vector2Distance(l6_ball,l6_candle[i])<l6_candle_radius+l6_radius_ball)
        {
            l6_candle_lit[i] = 1;
        }
    }

    //the hidden way down: it only works if something is lighting it when you arrive
    if (CheckCollisionPointRec(l6_ball,l6_secret) && l6_light_at(l6_ball)>0.25)
    {
        l6_ball = l6_secret_exit;
        l6_speed = Vector2Scale(l6_speed,0.4);
        l6_last_shot_position = l6_secret_exit;
        l6_message_type = 3;
        l6_message_timer = 1.2;
        l6_secret_glow = 1;
        return;
    }

    //an open trapdoor is a hole in the floor
    for (int i=0; i<2; i++)
    {
        if (l6_trapdoor_clock[i]>=6 && CheckCollisionPointRec(l6_ball,l6_trapdoor[i]))
        {
            l6_send_ball_back();
            l6_message_type = 1;
            l6_message_timer = 1;
            return;
        }
    }

    //ghosts: only solid in the dark. In candlelight they fade and the ball goes straight through.
    for (int i=0; i<3; i++)
    {
        Vector2 here = l6_ghost_at(i);
        if (l6_light_at(here)>0.35) continue;
        if (Vector2Distance(l6_ball,here) < l6_ghost_radius+l6_radius_ball)
        {
            Vector2 away = Vector2Subtract(l6_ball,here);
            if (Vector2Length(away)<1) away = l6_make_point(1,0);
            l6_speed = Vector2Add(l6_speed,Vector2Scale(Vector2Normalize(away),260*l6_u));
            l6_speed = Vector2ClampValue(l6_speed,0,l6_max_speed*l6_u);
            l6_message_type = 2;
            l6_message_timer = 0.8;
        }
    }

    //the chandelier drops when the ball goes under it
    if (l6_chandelier_state==0 && Vector2Distance(l6_ball,l6_chandelier)<120*l6_u)
    {
        l6_chandelier_state = 1;
        l6_chandelier_fall = 0;
    }
    if (l6_chandelier_state==1 && l6_chandelier_fall>0.5)
    {
        if (Vector2Distance(l6_ball,l6_chandelier)<70*l6_u)
        {
            l6_chandelier_state = 2;
            l6_send_ball_back();
            l6_message_type = 1;
            l6_message_timer = 1;
            return;
        }
    }

    //mirrors: in one, out of the other, keeping the speed but taking the exit's direction
    if (l6_mirror_cooldown<=0)
    {
        for (int i=0; i<2; i++)
        {
            if (Vector2Distance(l6_ball,l6_mirror[i])<34*l6_u)
            {
                int other = 1-i;
                float facing = l6_mirror_facing[other]*DEG2RAD;
                Vector2 direction = {cos(facing),sin(facing)};
                l6_ball = Vector2Add(l6_mirror[other],Vector2Scale(direction,52*l6_u));
                l6_speed = Vector2Scale(direction,Vector2Length(l6_speed));
                l6_mirror_cooldown = 0.5;
                break;
            }
        }
    }

    //plates
    for (int i=0; i<2; i++)
    {
        if (Vector2Distance(l6_ball,l6_plate[i])<l6_plate_radius) l6_plate_down[i] = 1;
    }

    //solid things: the house, the walls that are currently there, the crypt door, the bats
    for (int i=0; i<l6_wall_count; i++) l6_bounce_off_rectangle(l6_wall_rect[i],Vector2Zero());
    if (l6_phase_on==1)
    {
        for (int i=0; i<3; i++) l6_bounce_off_rectangle(l6_phase_wall[i],Vector2Zero());
    }
    if (l6_crypt_open<1)
    {
        Rectangle door = l6_crypt_door;
        door.y = door.y + door.height*l6_crypt_open;
        l6_bounce_off_rectangle(door,Vector2Zero());
    }
    for (int i=0; i<3; i++)
    {
        Vector2 bat_speed = {l6_bat_speed[i],0};
        l6_bounce_off_rectangle(l6_bat[i],bat_speed);
    }
    if (l6_chandelier_state==2) l6_bounce_off_circle(l6_chandelier,34*l6_u,1);

    //in the hole
    if ((l6_ball.x>l6_pot.x-3*l6_radius_pot/4) && (l6_ball.x<l6_pot.x+3*l6_radius_pot/4) && (l6_ball.y>l6_pot.y-3*l6_radius_pot/4) && (l6_ball.y<l6_pot.y+3*l6_radius_pot/4))
    {
        l6_ball = l6_pot;
        l6_speed.x = 0;
        l6_speed.y = 0;
        l6_game_state = 1;
    }
}


void l6_draw_floor()
{
    Rectangle source = {0,0,l6_width/l6_u*2,l6_height/l6_u*2};
    Rectangle dest = {0,0,l6_width,l6_height};
    Vector2 no_origin = {0,0};
    DrawTexturePro(l6_floor_texture,source,dest,no_origin,0,WHITE);

    //a long rug down the hall, so the room reads even in the dark
    DrawRectangle(90*l6_u,590*l6_u,540*l6_u,390*l6_u,Fade(l6_rug,0.5));
    DrawRectangleLinesEx(l6_make_rect(90,590,540,390),3*l6_u,Fade(l6_gold,0.3));

    //cobwebs in the corners
    for (int i=0; i<3; i++)
    {
        Rectangle web_source = {0,0,512,512};
        if (i==1) web_source.width = -512;
        DrawTexturePro(l6_cobweb_texture,web_source,l6_cobweb[i],no_origin,0,Fade(WHITE,0.75));
    }
}


void l6_draw_walls()
{
    //the walls are panelled, and they throw a shadow onto the floor, so the rooms
    //read as rooms instead of as marks on the boards
    for (int i=0; i<l6_wall_count; i++)
    {
        Rectangle r = l6_wall_rect[i];
        DrawRectangle(r.x+6*l6_u,r.y+8*l6_u,r.width,r.height,Fade(BLACK,0.45));
        DrawRectangleRec(r,l6_wall_dark);
        DrawRectangle(r.x+3*l6_u,r.y+3*l6_u,r.width-6*l6_u,r.height-6*l6_u,l6_wall);
        DrawRectangle(r.x,r.y,r.width,5*l6_u,l6_wall_light);

        //panel seams every 90 units along the long side
        if (r.width>r.height)
        {
            for (float x=r.x+45*l6_u; x<r.x+r.width-20*l6_u; x=x+90*l6_u)
            {
                DrawRectangle(x,r.y+6*l6_u,2*l6_u,r.height-12*l6_u,Fade(BLACK,0.35));
                DrawRectangle(x+2*l6_u,r.y+6*l6_u,1*l6_u,r.height-12*l6_u,Fade(l6_wall_light,0.6));
            }
        }
        else
        {
            for (float y=r.y+45*l6_u; y<r.y+r.height-20*l6_u; y=y+90*l6_u)
            {
                DrawRectangle(r.x+6*l6_u,y,r.width-12*l6_u,2*l6_u,Fade(BLACK,0.35));
                DrawRectangle(r.x+6*l6_u,y+2*l6_u,r.width-12*l6_u,1*l6_u,Fade(l6_wall_light,0.6));
            }
        }
        DrawRectangleLinesEx(r,2*l6_u,GetColor(0x16100CFF));
    }

    //the phasing walls: solid, or just an outline where they will be
    for (int i=0; i<3; i++)
    {
        Rectangle r = l6_phase_wall[i];
        if (l6_phase_on==1)
        {
            DrawRectangleRec(r,l6_wall);
            DrawRectangleLinesEx(r,2*l6_u,Fade(l6_ghost_blue,0.6));
        }
        else
        {
            DrawRectangleLinesEx(r,2*l6_u,Fade(l6_ghost_blue,0.22));
        }
    }

    //the crypt door
    Rectangle door = l6_crypt_door;
    door.y = door.y + door.height*l6_crypt_open;
    DrawRectangleRec(door,l6_wall_dark);
    DrawRectangleLinesEx(door,3*l6_u,l6_gold);
}


void l6_draw_obstacles()
{

    //trapdoors: shut, about to open, or open
    for (int i=0; i<2; i++)
    {
        Rectangle r = l6_trapdoor[i];
        if (l6_trapdoor_clock[i]>=6)
        {
            DrawRectangleRec(r,BLACK);
            DrawRectangleLinesEx(r,3*l6_u,Fade(l6_gold,0.5));
        }
        else
        {
            DrawRectangleRec(r,l6_wall_light);
            DrawRectangleLinesEx(r,3*l6_u,l6_wall_dark);
            DrawLineEx((Vector2){r.x,r.y+r.height/2},(Vector2){r.x+r.width,r.y+r.height/2},2*l6_u,l6_wall_dark);
            if (l6_trapdoor_clock[i]>5 && fmod(l6_animation_time*8,2)<1)
            {
                DrawRectangleLinesEx(r,4*l6_u,Fade(l6_candle_warm,0.8));
            }
        }
    }

    //the hidden trapdoor is only drawn when something is lighting it
    float secret_light = l6_light_at((Vector2){l6_secret.x+l6_secret.width/2,l6_secret.y+l6_secret.height/2});
    if (secret_light>0.12 || l6_secret_glow>0)
    {
        float show = secret_light;
        if (l6_secret_glow>show) show = l6_secret_glow;
        DrawRectangleRec(l6_secret,Fade(BLACK,0.8*show));
        DrawRectangleLinesEx(l6_secret,3*l6_u,Fade(l6_gold,show));
        DrawText("?",l6_secret.x+l6_secret.width/2-8*l6_u,l6_secret.y+l6_secret.height/2-18*l6_u,36*l6_u,Fade(l6_gold,show));
    }

    //plates
    for (int i=0; i<2; i++)
    {
        Color plate = l6_wall_light;
        if (l6_plate_down[i]==1) plate = l6_gold;
        DrawCircleV(l6_plate[i],l6_plate_radius,plate);
        DrawCircleLines(l6_plate[i].x,l6_plate[i].y,l6_plate_radius,l6_wall_dark);
        DrawRing(l6_plate[i],l6_plate_radius*0.45,l6_plate_radius*0.6,0,360,24,Fade(l6_wall_dark,0.8));
    }

    //candles
    for (int i=0; i<7; i++)
    {
        int frame = 0;
        if (l6_candle_lit[i]==1) frame = 1 + (int)(l6_animation_time*9)%3;
        Rectangle source = {frame*128,0,128,128};
        Rectangle dest = {l6_candle[i].x,l6_candle[i].y,42*l6_u,42*l6_u};
        Vector2 origin = {21*l6_u,21*l6_u};
        DrawTexturePro(l6_candle_texture,source,dest,origin,0,WHITE);
    }

    //the wall lamps
    for (int i=0; i<4; i++)
    {
        DrawCircleV(l6_lamp[i],12*l6_u,Fade(l6_wall_light,0.9));
        DrawCircleLines(l6_lamp[i].x,l6_lamp[i].y,12*l6_u,l6_wall_dark);
        if (l6_lamp_on[i]>0) DrawCircleV(l6_lamp[i],9*l6_u,Fade(l6_candle_warm,l6_lamp_on[i]));
    }

    //mirrors
    for (int i=0; i<2; i++)
    {
        Rectangle source = {0,0,256,128};
        Rectangle dest = {l6_mirror[i].x,l6_mirror[i].y,84*l6_u,42*l6_u};
        Vector2 origin = {42*l6_u,21*l6_u};
        DrawTexturePro(l6_mirror_texture,source,dest,origin,l6_mirror_facing[i],WHITE);
        float shimmer = 0.3+0.2*sin(l6_animation_time*3+i);
        DrawCircleV(l6_mirror[i],20*l6_u,Fade(l6_ghost_blue,shimmer));
    }

    //bats
    for (int i=0; i<3; i++)
    {
        int frame = (int)(l6_animation_time*12+i)%4;
        Rectangle source = {frame*128,0,128,128};
        Rectangle dest = {l6_bat[i].x+l6_bat[i].width/2,l6_bat[i].y+l6_bat[i].height/2,46*l6_u,46*l6_u};
        Vector2 origin = {23*l6_u,23*l6_u};
        float facing = 0;
        if (l6_bat_speed[i]<0) facing = 180;
        DrawTexturePro(l6_bat_texture,source,dest,origin,facing,WHITE);
    }

    //the fireplace, the clock, the portraits and the windows: the furniture that moves
    int fire_frame = (int)(l6_animation_time*8)%4;
    Rectangle fire_source = {fire_frame*256,0,256,256};
    Rectangle fire_dest = {l6_fire.x,l6_fire.y,150*l6_u,150*l6_u};
    Vector2 fire_origin = {75*l6_u,75*l6_u};
    DrawTexturePro(l6_fireplace_texture,fire_source,fire_dest,fire_origin,0,WHITE);

    int clock_frame = 0;
    float swing = sin(l6_animation_time*2);
    if (swing<-0.4) clock_frame = 1;
    else if (swing>0.4) clock_frame = 2;
    Rectangle clock_source = {clock_frame*256,0,256,256};
    Rectangle clock_dest = {l6_clock_at.x,l6_clock_at.y,92*l6_u,92*l6_u};
    Vector2 clock_origin = {46*l6_u,46*l6_u};
    DrawTexturePro(l6_clock_texture,clock_source,clock_dest,clock_origin,0,WHITE);

    for (int i=0; i<3; i++)
    {
        //the eyes follow the ball
        int look = 0;
        if (l6_ball.x > l6_portrait[i].x) look = 1;
        Rectangle source = {look*256,0,256,256};
        Rectangle dest = {l6_portrait[i].x,l6_portrait[i].y,72*l6_u,72*l6_u};
        Vector2 origin = {36*l6_u,36*l6_u};
        DrawTexturePro(l6_portrait_texture,source,dest,origin,0,WHITE);
    }

    for (int i=0; i<2; i++)
    {
        int frame = (int)(l6_animation_time*4+i)%4;
        Rectangle source = {frame*256,0,256,256};
        Rectangle dest = {l6_window[i].x,l6_window[i].y,120*l6_u,120*l6_u};
        Vector2 origin = {60*l6_u,15*l6_u};
        DrawTexturePro(l6_curtain_texture,source,dest,origin,0,WHITE);
    }

    //the swinging lantern and its chain
    DrawLineEx(l6_lantern_anchor,l6_lantern,3*l6_u,Fade(GetColor(0x6E6A60FF),0.9));
    int lantern_frame = 0;
    if (l6_lantern_swing<-40*l6_u) lantern_frame = 1;
    else if (l6_lantern_swing>40*l6_u) lantern_frame = 3;
    else lantern_frame = 2;
    Rectangle lantern_source = {lantern_frame*256,0,256,256};
    Rectangle lantern_dest = {l6_lantern.x,l6_lantern.y,92*l6_u,92*l6_u};
    Vector2 lantern_origin = {46*l6_u,46*l6_u};
    DrawTexturePro(l6_lantern_texture,lantern_source,lantern_dest,lantern_origin,0,WHITE);

    //the broken sconce and its sparks
    DrawCircleV(l6_sconce,14*l6_u,Fade(l6_wall_light,0.9));
    if (l6_spark_stage>0)
    {
        int frame = (int)(l6_spark_stage/0.8*4);
        if (frame>3) frame = 3;
        Rectangle source = {frame*256,0,256,256};
        Rectangle dest = {l6_sconce.x,l6_sconce.y,150*l6_u,150*l6_u};
        Vector2 origin = {75*l6_u,75*l6_u};
        DrawTexturePro(l6_sparks_texture,source,dest,origin,0,WHITE);
    }

    //rats
    for (int i=0; i<3; i++)
    {
        int frame = (int)(l6_animation_time*14+i)%4;
        Rectangle source = {frame*128,0,128,128};
        Rectangle dest = {l6_rat[i].x,l6_rat[i].y,40*l6_u,40*l6_u};
        Vector2 origin = {20*l6_u,20*l6_u};
        float facing = 90;
        if (l6_rat_travel[i]>1) facing = -90;
        DrawTexturePro(l6_rat_texture,source,dest,origin,facing,WHITE);
    }

    //wisps
    for (int i=0; i<3; i++)
    {
        int frame = (int)(l6_animation_time*6+i*2)%4;
        Rectangle source = {frame*256,0,256,256};
        Rectangle dest = {l6_wisp[i].x,l6_wisp[i].y,110*l6_u,110*l6_u};
        Vector2 origin = {55*l6_u,55*l6_u};
        DrawTexturePro(l6_wisp_texture,source,dest,origin,0,WHITE);
    }

    //ghosts, fainter where it is light
    for (int i=0; i<3; i++)
    {
        Vector2 here = l6_ghost_at(i);
        float light = l6_light_at(here);
        float show = 1;
        if (light>0.35) show = 0.25;
        int frame = (int)(l6_animation_time*6+i)%4;
        Rectangle source = {frame*192,0,192,192};
        Rectangle dest = {here.x,here.y,62*l6_u,62*l6_u};
        Vector2 origin = {31*l6_u,31*l6_u};
        DrawTexturePro(l6_ghost_texture,source,dest,origin,0,Fade(WHITE,show));
    }

    //the chandelier
    if (l6_chandelier_state!=2 || 1)
    {
        float drop = 0;
        if (l6_chandelier_state==1) drop = l6_chandelier_fall*0.9;
        if (l6_chandelier_state==2) drop = 1;
        float size = (96 - drop*30)*l6_u;
        Rectangle source = {0,0,256,256};
        Rectangle dest = {l6_chandelier.x,l6_chandelier.y,size,size};
        Vector2 origin = {size/2,size/2};
        float wobble = 0;
        if (l6_chandelier_state==1) wobble = sin(l6_animation_time*40)*8;
        DrawTexturePro(l6_chandelier_texture,source,dest,origin,wobble,WHITE);
    }
}


void l6_draw_ball_and_pot()
{
    //the crypt, with its own faint green glow so it can be found
    DrawCircleV(l6_pot,l6_radius_pot+16*l6_u,Fade(l6_crypt_green,0.18));
    DrawCircleV(l6_pot,l6_radius_pot+3*l6_u,Fade(BLACK,0.6));
    DrawCircleV(l6_pot,l6_radius_pot,BLACK);
    DrawCircleLines(l6_pot.x,l6_pot.y,l6_radius_pot,Fade(l6_crypt_green,0.8));
    //a flag on the crypt, the same marker every other level puts on its hole
    Vector2 pole_top = {l6_pot.x,l6_pot.y-62*l6_u};
    DrawLineEx(l6_pot,pole_top,3*l6_u,Fade(l6_gold,0.9));
    for (int i=0; i<5; i++)
    {
        float wave = sin(l6_animation_time*6 - i*0.8)*2.5*l6_u;
        DrawRectangle(l6_pot.x+1*l6_u+i*6*l6_u,pole_top.y+wave,6*l6_u,18*l6_u,Fade(l6_crypt_green,0.95));
    }

    DrawCircleV(l6_ball,l6_radius_ball,WHITE);
    DrawCircleLines(l6_ball.x,l6_ball.y,l6_radius_ball,GRAY);

    if (l6_aiming==1 && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        Vector2 mouse = {GetMouseX(),GetMouseY()};
        DrawLineEx(l6_ball,mouse,4*l6_u,Fade(WHITE,0.75));
        DrawCircle(mouse.x,mouse.y,5*l6_u,Fade(WHITE,0.75));
    }
}


//multiply the whole room down to nothing except where the lights are
void l6_draw_darkness()
{
    if (l6_mask_ready==0) return;
    BeginBlendMode(BLEND_MULTIPLIED);
    Rectangle source = {0,0,(float)l6_light_mask.texture.width,-(float)l6_light_mask.texture.height};
    Rectangle dest = {0,0,(float)l6_width,(float)l6_height};
    Vector2 no_origin = {0,0};
    DrawTexturePro(l6_light_mask.texture,source,dest,no_origin,0,WHITE);
    EndBlendMode();
}


void l6_draw_hud()
{
    DrawRectangleGradientV(0,0,l6_width,l6_hud_height,l6_wall_light,l6_wall);
    DrawRectangle(0,l6_hud_height-4*l6_u,l6_width,4*l6_u,l6_wall_dark);

    DrawText("LEVEL 6 - HAUNTED MANOR",32*l6_u,20*l6_u,36*l6_u,BLACK);
    DrawText("LEVEL 6 - HAUNTED MANOR",30*l6_u,18*l6_u,36*l6_u,l6_candle_warm);
    DrawText(TextFormat("STROKES %d / %d",l6_stroke,l6_stroke_limit),l6_width/2-298*l6_u,21*l6_u,32*l6_u,BLACK);
    DrawText(TextFormat("STROKES %d / %d",l6_stroke,l6_stroke_limit),l6_width/2-300*l6_u,19*l6_u,32*l6_u,WHITE);
    for (int i=0; i<l6_stroke_limit; i++)
    {
        Color pip = l6_wall_dark;
        if (i<l6_stroke) pip = l6_rug;
        DrawCircle(l6_width/2+10*l6_u+i*22*l6_u,35*l6_u,7*l6_u,pip);
        DrawCircleLines(l6_width/2+10*l6_u+i*22*l6_u,35*l6_u,7*l6_u,BLACK);
    }
    int help_width = MeasureText("R restart   ESC menu",26*l6_u);
    DrawText("R restart   ESC menu",l6_width-help_width-28*l6_u,24*l6_u,26*l6_u,BLACK);
    DrawText("R restart   ESC menu",l6_width-help_width-30*l6_u,22*l6_u,26*l6_u,WHITE);

    //how many candles are lit, and the clock counting down to the next chime
    int lit = 0;
    for (int i=0; i<7; i++) lit = lit + l6_candle_lit[i];
    DrawText(TextFormat("CANDLES %d / 7",lit),34*l6_u,86*l6_u,24*l6_u,Fade(l6_candle_warm,0.85));
    float to_chime = 8 - l6_clock_timer;
    DrawText(TextFormat("CHIME IN %.0f",to_chime),34*l6_u,118*l6_u,24*l6_u,Fade(l6_ghost_blue,0.75));

    if (l6_message_timer>0)
    {
        const char *say = "DOWN THE TRAPDOOR!";
        if (l6_message_type==2) say = "BOO!";
        if (l6_message_type==3) say = "A WAY DOWN!";
        DrawText(say,l6_width/2-MeasureText(say,80*l6_u)/2+4*l6_u,l6_height/2-36*l6_u,80*l6_u,Fade(BLACK,l6_message_timer));
        DrawText(say,l6_width/2-MeasureText(say,80*l6_u)/2,l6_height/2-40*l6_u,80*l6_u,Fade(l6_candle_warm,l6_message_timer));
    }

    if (l6_game_state!=0)
    {
        DrawRectangle(0,0,l6_width,l6_height,Fade(BLACK,0.6));
        Rectangle panel = {l6_width/2-340*l6_u,l6_height/2-170*l6_u,680*l6_u,340*l6_u};
        DrawRectangleRec(panel,l6_wall_dark);
        DrawRectangle(panel.x,panel.y,panel.width,24*l6_u,l6_gold);
        DrawRectangleLinesEx(panel,6*l6_u,l6_gold);
        if (l6_game_state==1) DrawText("LEVEL CLEAR!",l6_width/2-MeasureText("LEVEL CLEAR!",80*l6_u)/2,panel.y+60*l6_u,80*l6_u,l6_candle_warm);
        else DrawText("FAILED",l6_width/2-MeasureText("FAILED",80*l6_u)/2,panel.y+60*l6_u,80*l6_u,l6_rug);
        int strokes_width = MeasureText(TextFormat("Strokes: %d / %d",l6_stroke,l6_stroke_limit),40*l6_u);
        DrawText(TextFormat("Strokes: %d / %d",l6_stroke,l6_stroke_limit),l6_width/2-strokes_width/2,panel.y+170*l6_u,40*l6_u,WHITE);
        DrawText("R play again   ESC menu",l6_width/2-MeasureText("R play again   ESC menu",30*l6_u)/2,panel.y+250*l6_u,30*l6_u,l6_gold);
    }
}


//level 6: keep the house moving with no ball (intro and menu)
void l6_background_step(float dt)
{
    l6_ambient = 92;                 //on the menu card the house is shown off, not played
    l6_update_obstacles(dt);
    l6_build_light_mask();
}


//level 6: draw everything except the scoreboard (intro and menu)
void l6_draw_scene()
{
    l6_draw_floor();
    l6_draw_walls();
    l6_draw_obstacles();
    l6_draw_ball_and_pot();
    l6_draw_darkness();
}


void l6_start(int screen_width, int screen_height)
{
    l6_width = screen_width;
    l6_height = screen_height;

    l6_floor_texture = LoadTexture("assets/manor/manor_floor_tile.png");
    l6_ghost_texture = LoadTexture("assets/manor/manor_ghost.png");
    l6_candle_texture = LoadTexture("assets/manor/manor_candle.png");
    l6_chandelier_texture = LoadTexture("assets/manor/manor_chandelier.png");
    l6_mirror_texture = LoadTexture("assets/manor/manor_mirror.png");
    l6_cobweb_texture = LoadTexture("assets/manor/manor_cobweb.png");
    l6_bat_texture = LoadTexture("assets/manor/manor_bat.png");
    l6_wisp_texture = LoadTexture("assets/manor/manor_wisp.png");
    l6_lantern_texture = LoadTexture("assets/manor/manor_lantern.png");
    l6_sparks_texture = LoadTexture("assets/manor/manor_sparks.png");
    l6_fireplace_texture = LoadTexture("assets/manor/manor_fireplace.png");
    l6_rat_texture = LoadTexture("assets/manor/manor_rat.png");
    l6_portrait_texture = LoadTexture("assets/manor/manor_portrait.png");
    l6_clock_texture = LoadTexture("assets/manor/manor_clock.png");
    l6_curtain_texture = LoadTexture("assets/manor/manor_curtain.png");
    SetTextureWrap(l6_floor_texture,TEXTURE_WRAP_REPEAT);
    SetTextureFilter(l6_floor_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l6_ghost_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l6_candle_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l6_chandelier_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l6_mirror_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l6_cobweb_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l6_bat_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l6_wisp_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l6_lantern_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l6_sparks_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l6_fireplace_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l6_rat_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l6_portrait_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l6_clock_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l6_curtain_texture,TEXTURE_FILTER_BILINEAR);

    l6_light_mask = LoadRenderTexture(screen_width,screen_height);
    l6_mask_ready = 1;

    l6_u = l6_height/1080.0;
    if (l6_width/1920.0 < l6_u) l6_u = l6_width/1920.0;
    l6_reset_level();
    l6_build_light_mask();
}


void l6_update()
{
    float dt = GetFrameTime();
    if (dt>1.0/30) dt = 1.0/30;
    if (l6_message_timer>0) l6_message_timer = l6_message_timer - dt;

    if (IsKeyPressed(KEY_R)) l6_reset_level();

    int ball_stopped = 0;
    if (l6_speed.x==0 && l6_speed.y==0) ball_stopped = 1;
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && ball_stopped==1 && l6_game_state==0) l6_aiming = 1;
    if (l6_game_state!=0) l6_aiming = 0;
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && l6_aiming==1)
    {
        l6_aiming = 0;
        Vector2 mouse = {GetMouseX(),GetMouseY()};
        Vector2 drag = Vector2Subtract(l6_ball,mouse);
        if (ball_stopped==1 && l6_stroke<l6_stroke_limit && Vector2Length(drag)>=2*l6_radius_ball)
        {
            l6_last_shot_position = l6_ball;
            l6_speed = Vector2ClampValue(Vector2Scale(drag,3.125),0,l6_max_speed*l6_u);
            l6_stroke++;
        }
    }

    for (int i=0; i<4; i++)
    {
        l6_update_obstacles(dt/4*obstacle_speed);
        if (l6_game_state==0) l6_update_ball(dt/4);
    }

    if (l6_game_state==0 && l6_stroke>=l6_stroke_limit && l6_speed.x==0 && l6_speed.y==0) l6_game_state = 2;

    //the mask is built here, outside the drawing, because it writes into a texture
    l6_ambient = 30;                 //playing: you only have your own light
    l6_build_light_mask();
}


void l6_draw()
{
    l6_draw_floor();
    l6_draw_walls();
    l6_draw_obstacles();
    l6_draw_ball_and_pot();
    l6_draw_darkness();
    draw_straight_preview(l6_ball,l6_aiming,l6_radius_ball,l6_max_speed*l6_u,l6_u);
    l6_draw_hud();
}


void l6_unload()
{
    UnloadTexture(l6_floor_texture);
    UnloadTexture(l6_ghost_texture);
    UnloadTexture(l6_candle_texture);
    UnloadTexture(l6_chandelier_texture);
    UnloadTexture(l6_mirror_texture);
    UnloadTexture(l6_cobweb_texture);
    UnloadTexture(l6_bat_texture);
    UnloadTexture(l6_wisp_texture);
    UnloadTexture(l6_lantern_texture);
    UnloadTexture(l6_sparks_texture);
    UnloadTexture(l6_fireplace_texture);
    UnloadTexture(l6_rat_texture);
    UnloadTexture(l6_portrait_texture);
    UnloadTexture(l6_clock_texture);
    UnloadTexture(l6_curtain_texture);
    if (l6_mask_ready==1) UnloadRenderTexture(l6_light_mask);
    l6_mask_ready = 0;
}
