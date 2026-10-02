//==================== LEVEL 5 ====================
//Frozen Peak: a climb from base camp to the summit. The wind blows across the whole
//course and gets stronger the higher you go, and the ice barely slows the ball down.

//full global
Vector2 no_origin_global = {0,0};
int l5_width = 1920;
int l5_height = 1080;
float l5_u = 1;
int l5_stroke_base = 26;
int l5_stroke_limit = 26;
#define l5_max_speed 650

//colours
Color l5_snow = {237,244,250,255};
Color l5_snow_shade = {208,223,238,255};
Color l5_ice = {78,126,150,255};
Color l5_ice_light = {191,228,242,255};
Color l5_crevasse = {28,46,62,255};
Color l5_crevasse_deep = {14,24,34,255};
Color l5_rock = {92,98,110,255};
Color l5_rock_dark = {58,63,72,255};
Color l5_flag_red = {214,70,58,255};
Color l5_warm = {255,179,64,255};

//pictures
Texture2D l5_snow_texture;
Texture2D l5_ice_texture;
Texture2D l5_pine_texture;
Texture2D l5_snowball_texture;
Texture2D l5_gondola_texture;
Texture2D l5_bridge_texture;
Texture2D l5_campfire_texture;
Texture2D l5_icicle_texture;
Texture2D l5_ibex_texture;
Texture2D l5_hare_texture;
Texture2D l5_flags_texture;
Texture2D l5_toboggan_texture;
Texture2D l5_crate_texture;
Texture2D l5_boulder_texture;
Texture2D l5_spindrift_texture;
Texture2D l5_sign_texture;

//l5_ball and l5_pot
Vector2 l5_ball;
Vector2 l5_speed;
float l5_radius_ball;
Vector2 l5_pot;
float l5_radius_pot;
Vector2 l5_start_position;
Vector2 l5_last_shot_position;
int l5_stroke = 0;
int l5_game_state = 0;
int l5_aiming = 0;
float l5_animation_time = 0;
float l5_hud_height;

//messages: 1 fell in a crevasse, 2 buried in a drift, 3 hit by an icicle, 4 caught by the avalanche
int l5_message_type = 0;
float l5_message_timer = 0;

//the ground: snow is safe, everything else is a crevasse
Rectangle l5_snow_ground[9];
Rectangle l5_black_ice[4];
Rectangle l5_powder[3];

//drift pits: stand still in one for 3 seconds and the snow swallows the ball
Rectangle l5_drift[2];
float l5_drift_timer = 0;

//two crossings over the big crevasse: the gondola is slow and safe, the bridge is quick and breaks
//plank state: 0 solid, 1 cracking, 2 gone
Rectangle l5_plank[2][7];
int l5_plank_count[2] = {5,7};
int l5_plank_state[2][7];
float l5_plank_timer[2][7];
float l5_plank_crack_time[2] = {0.6,0.35};

//the gondola carries the ball across
Rectangle l5_gondola;
float l5_gondola_left;
float l5_gondola_right;
float l5_gondola_speed;

//the snowball grows as it rolls, then shatters at the bottom
Vector2 l5_snowball;
Vector2 l5_snowball_from;
Vector2 l5_snowball_to;
float l5_snowball_travel = 0;
float l5_snowball_radius;
float l5_snowball_small;
float l5_snowball_big;
float l5_snowball_wait = 0;
float l5_shatter_timer = 0;
Vector2 l5_shatter_at;

//icicles hanging over the top corridor, they drop when the ball goes under them
Vector2 l5_icicle[3];
int l5_icicle_state[3];            //0 hanging, 1 falling, 2 broken
float l5_icicle_fall[3];

//pines and campfires
Vector2 l5_pine[5];
float l5_pine_radius;
Vector2 l5_campfire[3];
float l5_campfire_radius;

//the wind: one clock for the whole mountain, breeze -> gust -> gale, direction flips each cycle
float l5_wind_clock = 0;
Vector2 l5_wind;
float l5_wind_strength = 0;
float l5_whiteout = 0;

//--- the mountain's own life: these keep the screen busy, most of them harmless ---
//ibex wander their ledge and nudge the ball, hares just hop about
Vector2 l5_ibex[3];
float l5_ibex_from[3];
float l5_ibex_to[3];
float l5_ibex_speed[3];
float l5_ibex_radius;
Vector2 l5_hare[4];
float l5_hare_home[4][2];
float l5_hare_hop[4];
float l5_hare_rest[4];

//strings of prayer flags: they lean with the wind, so they are the gauge you read
Vector2 l5_flagline[3];
float l5_flagline_width[3];

//the toboggan runs a lane and knocks the ball off it
Rectangle l5_toboggan;
float l5_toboggan_from;
float l5_toboggan_to;
float l5_toboggan_speed;

//supply crates are solid, ice boulders are bouncy, signposts are scenery
Rectangle l5_crate[3];
Vector2 l5_boulder[4];
float l5_boulder_radius;
float l5_boulder_hit[4];
Vector2 l5_sign[3];

//blowing snow drawn in bands across the course, and the plumes off the ridges
float l5_drift_offset = 0;

//the avalanche: once per game, it sweeps the left side and pushes the ball back down
float l5_avalanche_wait = 0;
int l5_avalanche_done = 0;
float l5_avalanche_timer = 0;
int l5_avalanche_state = 0;        //0 waiting, 1 warning, 2 sweeping
float l5_avalanche_y = 0;


//a rectangle in design units
Rectangle l5_make_rect(float x, float y, float w, float h)
{
    Rectangle rec = {x*l5_u,y*l5_u,w*l5_u,h*l5_u};
    return rec;
}

//a point in design units
Vector2 l5_make_point(float x, float y)
{
    Vector2 point = {x*l5_u,y*l5_u};
    return point;
}


void l5_reset_level()
{
    //sizes
    l5_hud_height = 70*l5_u;
    l5_radius_ball = 7*l5_u;
    l5_radius_pot = 11*l5_u;

    //SIX LANES, climbed one at a time, alternating ends. The whole climb is about
    //10,000 design units of travel, so every lane can hold two or three things with
    //hundreds of units of clear snow between them.
    //   A bottom  -> right bridge -> B -> left bridge -> C -> right bridge
    //   -> D -> left bridge -> E -> right bridge -> F summit
    l5_snow_ground[0] = l5_make_rect(60,920,1640,130);    //A base camp
    l5_snow_ground[1] = l5_make_rect(220,750,1640,130);   //B the glacier
    l5_snow_ground[2] = l5_make_rect(60,580,1640,130);    //C the snowball run
    l5_snow_ground[3] = l5_make_rect(220,410,1640,130);   //D the ice cave
    l5_snow_ground[4] = l5_make_rect(60,240,1640,130);    //E the high traverse
    l5_snow_ground[5] = l5_make_rect(220,90,1640,120);    //F the summit ridge
    //the three snow ramps that join the lanes the bridges do not
    l5_snow_ground[6] = l5_make_rect(220,580,170,300);    //B -> C, left
    l5_snow_ground[7] = l5_make_rect(1530,410,170,300);   //C -> D, right
    l5_snow_ground[8] = l5_make_rect(1600,90,260,280);    //E -> F, right

    //one long stretch of black ice per lane, never the whole lane
    l5_black_ice[0] = l5_make_rect(880,920,420,130);
    l5_black_ice[1] = l5_make_rect(560,750,520,130);
    l5_black_ice[2] = l5_make_rect(900,580,560,130);
    l5_black_ice[3] = l5_make_rect(700,240,520,130);

    //powder to stop in, at the end of the three nastiest legs
    l5_powder[0] = l5_make_rect(1380,920,200,130);
    l5_powder[1] = l5_make_rect(480,580,190,130);
    l5_powder[2] = l5_make_rect(1640,90,200,120);

    //two drift pits, far apart
    l5_drift[0] = l5_make_rect(520,750,170,62);
    l5_drift[1] = l5_make_rect(1180,410,170,62);
    l5_drift_timer = 0;

    //the two crumbling bridges: short, over an obvious gap, and they are the only way
    //up at their end of the mountain
    for (int i=0; i<5; i++) l5_plank[0][i] = l5_make_rect(1546+i*34,856,34,84);     //A -> B, right
    for (int i=0; i<5; i++) l5_plank[1][i] = l5_make_rect(216+i*34,346,34,84);      //D -> E, left
    for (int b=0; b<2; b++)
    {
        for (int i=0; i<7; i++)
        {
            l5_plank_state[b][i] = 0;
            l5_plank_timer[b][i] = 0;
        }
    }

    //the gondola is a ferry along lane D, not a gate: catch it and it saves you shots
    l5_gondola = l5_make_rect(400,415,170,120);
    l5_gondola_left = 300*l5_u;
    l5_gondola_right = 1560*l5_u;
    l5_gondola_speed = -200*l5_u;

    //the snowball rolls the length of lane C, straight at you
    l5_snowball_from = l5_make_point(1620,645);
    l5_snowball_to = l5_make_point(180,645);
    l5_snowball_small = 15*l5_u;
    l5_snowball_big = 42*l5_u;
    l5_snowball = l5_snowball_from;
    l5_snowball_radius = l5_snowball_small;
    l5_snowball_travel = 0;
    l5_snowball_wait = 0;
    l5_shatter_timer = 0;

    //icicles over lane D only, spread right out
    l5_icicle[0] = l5_make_point(620,398);
    l5_icicle[1] = l5_make_point(1020,398);
    l5_icicle[2] = l5_make_point(1420,398);
    for (int i=0; i<3; i++)
    {
        l5_icicle_state[i] = 0;
        l5_icicle_fall[i] = 0;
    }

    //pines stand at the lane edges, two per lane at most, far apart
    l5_pine_radius = 40*l5_u;
    l5_pine[0] = l5_make_point(420,1040);
    l5_pine[1] = l5_make_point(1240,1040);
    l5_pine[2] = l5_make_point(760,866);
    l5_pine[3] = l5_make_point(1500,696);
    l5_pine[4] = l5_make_point(560,356);

    //a campfire at the start of a lane is a safe place to aim from
    l5_campfire_radius = 110*l5_u;
    l5_campfire[0] = l5_make_point(240,985);
    l5_campfire[1] = l5_make_point(1660,645);
    l5_campfire[2] = l5_make_point(360,305);

    //the weather
    l5_wind_clock = 0;
    l5_wind.x = 0;
    l5_wind.y = 0;
    l5_wind_strength = 0;
    l5_whiteout = 0;
    l5_avalanche_wait = 30;
    l5_avalanche_done = 0;
    l5_avalanche_state = 0;
    l5_avalanche_timer = 0;
    l5_avalanche_y = 0;

    //one ibex per lane, pacing a long beat
    l5_ibex_radius = 24*l5_u;
    l5_ibex[0] = l5_make_point(900,985);
    l5_ibex_from[0] = 700*l5_u;
    l5_ibex_to[0] = 1300*l5_u;
    l5_ibex_speed[0] = 85*l5_u;
    l5_ibex[1] = l5_make_point(1200,815);
    l5_ibex_from[1] = 1000*l5_u;
    l5_ibex_to[1] = 1700*l5_u;
    l5_ibex_speed[1] = -95*l5_u;
    l5_ibex[2] = l5_make_point(700,305);
    l5_ibex_from[2] = 500*l5_u;
    l5_ibex_to[2] = 1300*l5_u;
    l5_ibex_speed[2] = 105*l5_u;

    //hares off at the ends, pure scenery
    l5_hare_home[0][0] = 140; l5_hare_home[0][1] = 1040;
    l5_hare_home[1][0] = 1800; l5_hare_home[1][1] = 870;
    l5_hare_home[2][0] = 120; l5_hare_home[2][1] = 700;
    l5_hare_home[3][0] = 1810; l5_hare_home[3][1] = 150;
    for (int i=0; i<4; i++)
    {
        l5_hare[i] = l5_make_point(l5_hare_home[i][0],l5_hare_home[i][1]);
        l5_hare_hop[i] = 0;
        l5_hare_rest[i] = 1 + i*0.7;
    }

    //flag lines hang over the gaps between lanes, where they are easy to read
    l5_flagline[0] = l5_make_point(500,895);
    l5_flagline_width[0] = 420*l5_u;
    l5_flagline[1] = l5_make_point(900,555);
    l5_flagline_width[1] = 420*l5_u;
    l5_flagline[2] = l5_make_point(700,215);
    l5_flagline_width[2] = 420*l5_u;

    //the toboggan runs across lane E, a timing problem on its own
    l5_toboggan = l5_make_rect(1120,240,100,130);
    l5_toboggan_from = 240*l5_u;
    l5_toboggan_to = 370*l5_u;
    l5_toboggan_speed = 150*l5_u;

    //crates and boulders: one obstacle at a time, well spread
    l5_crate[0] = l5_make_rect(640,922,76,76);
    l5_crate[1] = l5_make_rect(1340,752,76,76);
    l5_crate[2] = l5_make_rect(1400,242,76,76);
    l5_boulder_radius = 34*l5_u;
    l5_boulder[0] = l5_make_point(1100,1012);
    l5_boulder[1] = l5_make_point(1480,502);
    l5_boulder[2] = l5_make_point(300,672);
    l5_boulder[3] = l5_make_point(1300,118);

    for (int i=0; i<4; i++) l5_boulder_hit[i] = 0;

    //a signpost at every turn
    l5_sign[0] = l5_make_point(1620,985);
    l5_sign[1] = l5_make_point(480,815);
    l5_sign[2] = l5_make_point(1620,645);
    l5_drift_offset = 0;

    //ball and pot: the whole climb apart
    l5_start_position = l5_make_point(140,985);
    l5_ball = l5_start_position;
    l5_last_shot_position = l5_start_position;
    l5_speed.x = 0;
    l5_speed.y = 0;
    l5_pot = l5_make_point(340,150);
    l5_stroke = 0;
    l5_game_state = 0;
    l5_aiming = 0;
    l5_message_type = 0;
    l5_message_timer = 0;
}


//bounce off a straight rectangle, rec_speed is how fast the rectangle itself is moving
int l5_bounce_off_rectangle(Rectangle rec, Vector2 rec_speed)
{
    if (CheckCollisionCircleRec(l5_ball,l5_radius_ball,rec))
    {
        Vector2 collision_point;
        collision_point.x = Clamp(l5_ball.x,rec.x,rec.x+rec.width);
        collision_point.y = Clamp(l5_ball.y,rec.y,rec.y+rec.height);
        Vector2 normal = Vector2Subtract(l5_ball,collision_point);

        //centre is inside, go out through the nearest side
        if (normal.x==0 && normal.y==0)
        {
            float left = l5_ball.x - rec.x;
            float right = rec.x + rec.width - l5_ball.x;
            float top = l5_ball.y - rec.y;
            float bottom = rec.y + rec.height - l5_ball.y;
            if (left<=right && left<=top && left<=bottom) { normal.x = -1; collision_point.x = rec.x; }
            else if (right<=top && right<=bottom) { normal.x = 1; collision_point.x = rec.x + rec.width; }
            else if (top<=bottom) { normal.y = -1; collision_point.y = rec.y; }
            else { normal.y = 1; collision_point.y = rec.y + rec.height; }
        }
        normal = Vector2Normalize(normal);
        l5_ball = Vector2Add(collision_point,Vector2Scale(normal,l5_radius_ball));

        Vector2 relative_speed = Vector2Subtract(l5_speed,rec_speed);
        if (Vector2DotProduct(relative_speed,normal)<0)
        {
            relative_speed = Vector2Reflect(relative_speed,normal);
            l5_speed = Vector2ClampValue(Vector2Add(relative_speed,rec_speed),0,l5_max_speed*l5_u);
        }
        return 1;
    }
    return 0;
}


//bounce off a round thing, bounce is 1 for a normal one and more for a springy one
int l5_bounce_off_circle(Vector2 center, float radius, float bounce)
{
    Vector2 normal = Vector2Subtract(l5_ball,center);
    float distance = Vector2Length(normal);
    if (distance < radius + l5_radius_ball)
    {
        if (distance==0) normal.y = -1;
        normal = Vector2Normalize(normal);
        l5_ball = Vector2Add(center,Vector2Scale(normal,radius + l5_radius_ball));
        if (Vector2DotProduct(l5_speed,normal)<0)
        {
            l5_speed = Vector2ClampValue(Vector2Scale(Vector2Reflect(l5_speed,normal),bounce),0,l5_max_speed*l5_u);
            return 1;
        }
    }
    return 0;
}


//is this point standing on something, snow or a plank that has not fallen yet
int l5_on_safe_ground(Vector2 point)
{
    for (int i=0; i<9; i++)
    {
        if (CheckCollisionPointRec(point,l5_snow_ground[i])) return 1;
    }
    for (int b=0; b<2; b++)
    {
        for (int i=0; i<l5_plank_count[b]; i++)
        {
            if (l5_plank_state[b][i]<2 && CheckCollisionPointRec(point,l5_plank[b][i])) return 1;
        }
    }
    if (CheckCollisionPointRec(point,l5_gondola)) return 1;
    return 0;
}


void l5_update_obstacles(float dt)
{
    l5_animation_time = l5_animation_time + dt;

    //the wind clock: 12 seconds, breeze -> gust -> gale, and the direction flips every cycle
    l5_wind_clock = l5_wind_clock + dt;
    int cycle = (int)(l5_wind_clock/12);
    float inside = l5_wind_clock - cycle*12;
    if (inside<4) l5_wind_strength = 35;
    else if (inside<8) l5_wind_strength = 80;
    else l5_wind_strength = 135;
    float facing = 1;
    if (cycle%2==1) facing = -1;
    //the sideways push is the point; the up-down part is small and swings, so the wind
    //cannot simply sweep every ball off the bottom of the mountain
    l5_wind.x = l5_wind_strength*facing*l5_u;
    l5_wind.y = l5_wind_strength*0.22*sin(l5_wind_clock*0.8)*l5_u;

    //at the peak of the gale the snow comes across and you cannot see much
    l5_whiteout = 0;
    if (inside>10.2 && inside<11.4)
    {
        float part = (inside-10.2)/1.2;
        l5_whiteout = sin(part*PI)*0.62;
    }

    //blowing snow always travels with the wind
    l5_drift_offset = l5_drift_offset + l5_wind.x*dt*0.6;

    //ibex pacing
    for (int i=0; i<3; i++)
    {
        l5_ibex[i].x = l5_ibex[i].x + l5_ibex_speed[i]*dt;
        if (l5_ibex[i].x<l5_ibex_from[i])
        {
            l5_ibex[i].x = l5_ibex_from[i];
            l5_ibex_speed[i] = fabsf(l5_ibex_speed[i]);
        }
        if (l5_ibex[i].x>l5_ibex_to[i])
        {
            l5_ibex[i].x = l5_ibex_to[i];
            l5_ibex_speed[i] = -fabsf(l5_ibex_speed[i]);
        }
    }

    //hares: a rest, then a hop a little way, then a rest again
    for (int i=0; i<4; i++)
    {
        if (l5_hare_hop[i]>0)
        {
            l5_hare_hop[i] = l5_hare_hop[i] - dt;
            l5_hare[i].x = l5_hare[i].x + sin(i*2.1)*90*l5_u*dt;
            l5_hare[i].y = l5_hare[i].y + cos(i*1.7)*70*l5_u*dt;
            if (l5_hare_hop[i]<=0) l5_hare_rest[i] = 1.2 + GetRandomValue(0,25)/10.0;
        }
        else
        {
            l5_hare_rest[i] = l5_hare_rest[i] - dt;
            if (l5_hare_rest[i]<=0)
            {
                l5_hare_hop[i] = 0.5;
                //never let one wander off its patch
                Vector2 home = l5_make_point(l5_hare_home[i][0],l5_hare_home[i][1]);
                if (Vector2Distance(l5_hare[i],home)>120*l5_u) l5_hare[i] = home;
            }
        }
    }

    //the toboggan runs up and down its lane
    l5_toboggan.y = l5_toboggan.y + l5_toboggan_speed*dt;
    if (l5_toboggan.y<l5_toboggan_from)
    {
        l5_toboggan.y = l5_toboggan_from;
        l5_toboggan_speed = fabsf(l5_toboggan_speed);
    }
    if (l5_toboggan.y>l5_toboggan_to)
    {
        l5_toboggan.y = l5_toboggan_to;
        l5_toboggan_speed = -fabsf(l5_toboggan_speed);
    }

    for (int i=0; i<4; i++)
    {
        if (l5_boulder_hit[i]>0) l5_boulder_hit[i] = l5_boulder_hit[i] - dt;
    }

    //the gondola slides back and forth
    l5_gondola.x = l5_gondola.x + l5_gondola_speed*dt;
    if (l5_gondola.x<l5_gondola_left)
    {
        l5_gondola.x = l5_gondola_left;
        l5_gondola_speed = fabsf(l5_gondola_speed);
    }
    if (l5_gondola.x>l5_gondola_right)
    {
        l5_gondola.x = l5_gondola_right;
        l5_gondola_speed = -fabsf(l5_gondola_speed);
    }

    //the snowball: roll, grow, shatter, wait, start again
    if (l5_shatter_timer>0) l5_shatter_timer = l5_shatter_timer - dt;
    if (l5_snowball_wait>0)
    {
        l5_snowball_wait = l5_snowball_wait - dt;
        if (l5_snowball_wait<=0)
        {
            l5_snowball_travel = 0;
            l5_snowball = l5_snowball_from;
            l5_snowball_radius = l5_snowball_small;
        }
    }
    else
    {
        l5_snowball_travel = l5_snowball_travel + dt*0.22;
        if (l5_snowball_travel>=1)
        {
            l5_shatter_at = l5_snowball;
            l5_shatter_timer = 0.6;
            l5_snowball_wait = 2.2;
            l5_snowball_travel = 1;
        }
        l5_snowball = Vector2Lerp(l5_snowball_from,l5_snowball_to,l5_snowball_travel);
        l5_snowball_radius = l5_snowball_small + (l5_snowball_big-l5_snowball_small)*l5_snowball_travel;
    }

    //icicles that have been knocked loose
    for (int i=0; i<3; i++)
    {
        if (l5_icicle_state[i]==1)
        {
            l5_icicle_fall[i] = l5_icicle_fall[i] + dt;
            if (l5_icicle_fall[i]>1.2) l5_icicle_state[i] = 2;
        }
    }

    //plank timers: a plank that has been stepped on cracks, falls, and grows back after 5 seconds
    for (int b=0; b<2; b++)
    {
        for (int i=0; i<l5_plank_count[b]; i++)
        {
            if (l5_plank_state[b][i]==0) continue;
            l5_plank_timer[b][i] = l5_plank_timer[b][i] + dt;
            if (l5_plank_state[b][i]==1 && l5_plank_timer[b][i]>=l5_plank_crack_time[b])
            {
                l5_plank_state[b][i] = 2;
                l5_plank_timer[b][i] = 0;
            }
            else if (l5_plank_state[b][i]==2 && l5_plank_timer[b][i]>=5)
            {
                l5_plank_state[b][i] = 0;
                l5_plank_timer[b][i] = 0;
            }
        }
    }

    //the avalanche: it waits, warns, then sweeps down the left side. Only once.
    if (l5_avalanche_done==0)
    {
        if (l5_avalanche_state==0)
        {
            l5_avalanche_wait = l5_avalanche_wait - dt;
            if (l5_avalanche_wait<=0)
            {
                l5_avalanche_state = 1;
                l5_avalanche_timer = 0;
            }
        }
        else if (l5_avalanche_state==1)
        {
            l5_avalanche_timer = l5_avalanche_timer + dt;
            if (l5_avalanche_timer>=1.6)
            {
                l5_avalanche_state = 2;
                l5_avalanche_timer = 0;
                l5_avalanche_y = 100*l5_u;
            }
        }
        else
        {
            l5_avalanche_y = l5_avalanche_y + 520*l5_u*dt;
            if (l5_avalanche_y>l5_height+200*l5_u)
            {
                l5_avalanche_state = 0;
                l5_avalanche_done = 1;
            }
        }
    }
}


//back to the last shot, and never straight into something that would throw the ball out again
void l5_send_ball_back()
{
    l5_ball = l5_last_shot_position;

    //not under a falling icicle and not where the snowball is right now
    if (Vector2Distance(l5_ball,l5_snowball) < l5_snowball_radius + 3*l5_radius_ball)
    {
        l5_ball.x = l5_ball.x - (l5_snowball_radius + 4*l5_radius_ball);
    }
    //not inside a drift, or it just sinks again
    for (int i=0; i<2; i++)
    {
        if (CheckCollisionPointRec(l5_ball,l5_drift[i]))
        {
            l5_ball.y = l5_drift[i].y - l5_radius_ball - 4*l5_u;
        }
    }
    //the old spot was a plank that has fallen: go to the bank the bridge starts from
    if (l5_on_safe_ground(l5_ball)==0) l5_ball = l5_start_position;

    l5_speed.x = 0;
    l5_speed.y = 0;
    l5_drift_timer = 0;
}


void l5_update_ball(float dt)
{
    int pushed = 0;
    int was_moving = 0;
    if (l5_speed.x!=0 || l5_speed.y!=0) was_moving = 1;

    //the wind only pushes a ball that is already rolling, so a shot can still be lined up.
    //it must NOT count as a push, or friction could never bring the ball to a stop again.
    if (was_moving==1 && Vector2Length(l5_speed)>30*l5_u)
    {
        l5_speed = Vector2Add(l5_speed,Vector2Scale(l5_wind,dt));
    }

    //riding the gondola: the cabin carries the ball along with it
    int on_gondola = 0;
    if (CheckCollisionPointRec(l5_ball,l5_gondola))
    {
        l5_ball.x = l5_ball.x + l5_gondola_speed*dt;
        on_gondola = 1;
    }

    //the avalanche shoves everything in its way down the mountain
    if (l5_avalanche_state==2 && l5_ball.x<520*l5_u)
    {
        if (fabsf(l5_ball.y-l5_avalanche_y)<120*l5_u)
        {
            l5_speed.y = l5_speed.y + 900*l5_u*dt;
            l5_speed.x = l5_speed.x + 160*l5_u*dt;
            pushed = 1;
            if (l5_message_type!=4)
            {
                l5_message_type = 4;
                l5_message_timer = 1.2;
            }
        }
    }

    //moving
    l5_ball = Vector2Add(l5_ball,Vector2Scale(l5_speed,dt));

    //friction by ground: snow normal, powder heavy, black ice almost none.
    //a campfire melts the ice around it back to something you can stop on.
    float friction = 150*l5_u;
    int on_ice = 0;
    for (int i=0; i<4; i++)
    {
        if (CheckCollisionPointRec(l5_ball,l5_black_ice[i])) on_ice = 1;
    }
    for (int i=0; i<3; i++)
    {
        if (Vector2Distance(l5_ball,l5_campfire[i])<l5_campfire_radius) on_ice = 0;
    }
    if (on_ice==1) friction = 18*l5_u;
    for (int i=0; i<3; i++)
    {
        if (CheckCollisionPointRec(l5_ball,l5_powder[i])) friction = 430*l5_u;
    }
    float ball_speed = Vector2Length(l5_speed);
    if (ball_speed>0)
    {
        float new_speed = ball_speed - friction*dt;
        if (new_speed<0) new_speed = 0;
        l5_speed = Vector2Scale(l5_speed,new_speed/ball_speed);
    }
    if (pushed==0 && Vector2Length(l5_speed)<6*l5_u)
    {
        l5_speed.x = 0;
        l5_speed.y = 0;
    }

    //off the mountain
    if (l5_on_safe_ground(l5_ball)==0)
    {
        l5_send_ball_back();
        l5_message_type = 1;
        l5_message_timer = 1;
        return;
    }

    //standing in a drift pit: three seconds and the snow has you
    int in_drift = 0;
    for (int i=0; i<2; i++)
    {
        if (CheckCollisionPointRec(l5_ball,l5_drift[i])) in_drift = 1;
    }
    if (in_drift==1 && Vector2Length(l5_speed)<10*l5_u)
    {
        l5_drift_timer = l5_drift_timer + dt;
        if (l5_drift_timer>=3)
        {
            l5_send_ball_back();
            l5_message_type = 2;
            l5_message_timer = 1;
            return;
        }
    }
    else l5_drift_timer = 0;

    //standing on a plank starts it cracking
    for (int b=0; b<2; b++)
    {
        for (int i=0; i<l5_plank_count[b]; i++)
        {
            if (l5_plank_state[b][i]==0 && CheckCollisionPointRec(l5_ball,l5_plank[b][i]))
            {
                l5_plank_state[b][i] = 1;
                l5_plank_timer[b][i] = 0;
            }
        }
    }

    //the snowball: it knocks the ball away, and the bigger it is the harder it hits
    float hit = 1 + l5_snowball_travel*0.6;
    if (l5_snowball_wait<=0 && l5_bounce_off_circle(l5_snowball,l5_snowball_radius,hit))
    {
        l5_shatter_at = l5_snowball;
    }

    //icicles: going under one knocks it loose, and a falling one sends the ball back
    for (int i=0; i<3; i++)
    {
        if (l5_icicle_state[i]==0 && fabsf(l5_ball.x-l5_icicle[i].x)<40*l5_u && l5_ball.y>l5_icicle[i].y && l5_ball.y<l5_icicle[i].y+260*l5_u)
        {
            l5_icicle_state[i] = 1;
            l5_icicle_fall[i] = 0;
        }
        if (l5_icicle_state[i]==1)
        {
            Vector2 tip = {l5_icicle[i].x,l5_icicle[i].y + l5_icicle_fall[i]*620*l5_u};
            if (Vector2Distance(l5_ball,tip)<26*l5_u+l5_radius_ball)
            {
                l5_icicle_state[i] = 2;
                l5_send_ball_back();
                l5_message_type = 3;
                l5_message_timer = 1;
                return;
            }
        }
    }

    //pines are solid
    for (int i=0; i<5; i++) l5_bounce_off_circle(l5_pine[i],l5_pine_radius,1);

    //ice boulders are springy, crates are not
    for (int i=0; i<4; i++)
    {
        if (l5_bounce_off_circle(l5_boulder[i],l5_boulder_radius,1.3)) l5_boulder_hit[i] = 0.2;
    }
    for (int i=0; i<3; i++) l5_bounce_off_rectangle(l5_crate[i],Vector2Zero());

    //an ibex will shoulder the ball out of its way
    for (int i=0; i<3; i++)
    {
        Vector2 ibex_speed = {l5_ibex_speed[i],0};
        if (Vector2Distance(l5_ball,l5_ibex[i]) < l5_ibex_radius+l5_radius_ball)
        {
            Vector2 away = Vector2Subtract(l5_ball,l5_ibex[i]);
            if (Vector2Length(away)<1) away = l5_make_point(0,1);
            l5_ball = Vector2Add(l5_ibex[i],Vector2Scale(Vector2Normalize(away),l5_ibex_radius+l5_radius_ball));
            l5_speed = Vector2ClampValue(Vector2Add(l5_speed,Vector2Scale(Vector2Normalize(away),180*l5_u)),0,l5_max_speed*l5_u);
            l5_speed.x = l5_speed.x + ibex_speed.x*0.3;
        }
    }

    //the toboggan knocks the ball down the lane
    Vector2 sled_speed = {0,l5_toboggan_speed};
    l5_bounce_off_rectangle(l5_toboggan,sled_speed);

    //the gondola's walls push the ball along when it slides into one
    if (on_gondola==0)
    {
        Vector2 cabin_speed = {l5_gondola_speed,0};
        l5_bounce_off_rectangle(l5_gondola,cabin_speed);
    }

    //in the hole
    if ((l5_ball.x>l5_pot.x-3*l5_radius_pot/4) && (l5_ball.x<l5_pot.x+3*l5_radius_pot/4) && (l5_ball.y>l5_pot.y-3*l5_radius_pot/4) && (l5_ball.y<l5_pot.y+3*l5_radius_pot/4))
    {
        l5_ball = l5_pot;
        l5_speed.x = 0;
        l5_speed.y = 0;
        l5_game_state = 1;
    }
}


//the snow picture, repeating, lined up with the screen so pieces join without seams
void l5_draw_snow(Rectangle rec)
{
    Rectangle source = {rec.x/l5_u*2,rec.y/l5_u*2,rec.width/l5_u*2,rec.height/l5_u*2};
    Vector2 no_origin = {0,0};
    DrawTexturePro(l5_snow_texture,source,rec,no_origin,0,WHITE);
}


//black ice, two frames swapping so the sheen creeps along
void l5_draw_ice(Rectangle rec)
{
    int frame = (int)(l5_animation_time*1.5)%2;
    Rectangle source = {frame*512 + rec.x/l5_u*2,rec.y/l5_u*2,rec.width/l5_u*2,rec.height/l5_u*2};
    if (source.x+source.width>frame*512+512) source.x = frame*512;
    Vector2 no_origin = {0,0};
    DrawTexturePro(l5_ice_texture,source,rec,no_origin,0,WHITE);
}


void l5_draw_ground()
{
    //the crevasse under everything
    DrawRectangle(0,0,l5_width,l5_height,l5_crevasse_deep);
    for (int i=0; i<60; i++)
    {
        float x = fmod(i*311*l5_u,l5_width);
        float y = fmod(i*527*l5_u,l5_height);
        DrawCircle(x,y,2*l5_u,Fade(l5_ice_light,0.18));
    }

    //snow platforms, with a rock lip and a blue shadow on the crevasse side
    for (int i=0; i<9; i++)
    {
        Rectangle r = l5_snow_ground[i];
        DrawRectangle(r.x-6*l5_u,r.y-6*l5_u,r.width+12*l5_u,r.height+12*l5_u,l5_rock_dark);
        DrawRectangle(r.x-3*l5_u,r.y-3*l5_u,r.width+6*l5_u,r.height+6*l5_u,l5_rock);
        l5_draw_snow(r);
        DrawRectangleLinesEx(r,2*l5_u,Fade(l5_snow_shade,0.8));
    }

    //black ice sheets
    for (int i=0; i<4; i++)
    {
        l5_draw_ice(l5_black_ice[i]);
        DrawRectangleLinesEx(l5_black_ice[i],2*l5_u,Fade(l5_ice_light,0.5));
    }

    //deep powder
    for (int i=0; i<3; i++)
    {
        Rectangle r = l5_powder[i];
        DrawRectangleRounded(r,0.25,8,Fade(WHITE,0.9));
        for (int k=0; k<10; k++)
        {
            float x = r.x + fmod(k*83*l5_u,r.width);
            float y = r.y + fmod(k*57*l5_u,r.height);
            DrawCircle(x,y,7*l5_u,Fade(l5_snow_shade,0.7));
        }
    }

    //drift pits: rings that turn, like the quicksand in the temple
    for (int i=0; i<2; i++)
    {
        Rectangle r = l5_drift[i];
        Vector2 middle = {r.x+r.width/2,r.y+r.height/2};
        DrawRectangleRounded(r,0.4,8,Fade(l5_snow_shade,0.95));
        for (int k=0; k<4; k++)
        {
            float radius = 14*l5_u + k*14*l5_u;
            float turn = l5_animation_time*30;
            if (k%2==1) turn = -turn;
            DrawRing(middle,radius,radius+3*l5_u,turn,turn+230,24,Fade(l5_ice,0.5));
        }
    }

    //the planks of the two crossings
    for (int b=0; b<2; b++)
    {
        for (int i=0; i<l5_plank_count[b]; i++)
        {
            if (l5_plank_state[b][i]==2) continue;
            Rectangle r = l5_plank[b][i];
            float shake = 0;
            if (l5_plank_state[b][i]==1) shake = sin(l5_animation_time*60)*3*l5_u;
            Rectangle shown = {r.x+shake,r.y,r.width,r.height};
            //frame 1 is the sound plank, frame 2 the one that has already split
            int frame = (l5_plank_state[b][i]==1) ? 1 : 0;
            Rectangle source = {frame*128,0,128,256};
            Vector2 no_plank_origin = {0,0};
            DrawTexturePro(l5_bridge_texture,source,shown,no_plank_origin,0,WHITE);
            if (l5_plank_state[b][i]==1) DrawRectangleLinesEx(shown,2*l5_u,Fade(l5_crevasse,0.7));
        }
    }
}


void l5_draw_obstacles()
{
    Vector2 no_origin = {0,0};

    //campfires: a warm circle on the ice, then the fire itself
    for (int i=0; i<3; i++)
    {
        DrawCircleV(l5_campfire[i],l5_campfire_radius,Fade(l5_warm,0.14));
        DrawCircleV(l5_campfire[i],l5_campfire_radius*0.55,Fade(l5_warm,0.12));
        int frame = (int)(l5_animation_time*8)%4;
        Rectangle source = {frame*128,0,128,128};
        Rectangle dest = {l5_campfire[i].x,l5_campfire[i].y,90*l5_u,90*l5_u};
        Vector2 origin = {45*l5_u,45*l5_u};
        DrawTexturePro(l5_campfire_texture,source,dest,origin,0,WHITE);
    }

    //the gondola and its cable
    DrawLineEx((Vector2){l5_gondola_left-120*l5_u,l5_gondola.y+10*l5_u},(Vector2){l5_gondola_right+270*l5_u,l5_gondola.y+10*l5_u},4*l5_u,l5_rock);
    Rectangle gondola_source = {0,0,256,128};
    Rectangle gondola_dest = {l5_gondola.x,l5_gondola.y,l5_gondola.width,l5_gondola.height};
    DrawTexturePro(l5_gondola_texture,gondola_source,gondola_dest,no_origin,0,WHITE);

    //the snowball, and the chunks it leaves when it shatters
    if (l5_snowball_wait<=0)
    {
        int frame = (int)(l5_snowball_travel*14)%4;
        Rectangle source = {frame*128,0,128,128};
        float size = l5_snowball_radius*2.15;
        Rectangle dest = {l5_snowball.x,l5_snowball.y,size,size};
        Vector2 origin = {size/2,size/2};
        DrawTexturePro(l5_snowball_texture,source,dest,origin,l5_snowball_travel*220,WHITE);
    }
    if (l5_shatter_timer>0)
    {
        for (int k=0; k<3; k++)
        {
            float spread = (0.6-l5_shatter_timer)*220*l5_u;
            float angle = k*2.1;
            Vector2 piece = {l5_shatter_at.x+cos(angle)*spread,l5_shatter_at.y+sin(angle)*spread};
            DrawCircleV(piece,16*l5_u*l5_shatter_timer/0.6,Fade(WHITE,l5_shatter_timer/0.6));
        }
    }

    //icicles: hanging, falling, or already broken
    for (int i=0; i<3; i++)
    {
        if (l5_icicle_state[i]==2) continue;
        int frame = 0;
        float drop = 0;
        if (l5_icicle_state[i]==1)
        {
            frame = 1;
            drop = l5_icicle_fall[i]*620*l5_u;
        }
        Rectangle source = {frame*128,0,128,128};
        Rectangle dest = {l5_icicle[i].x,l5_icicle[i].y+drop,64*l5_u,64*l5_u};
        Vector2 origin = {32*l5_u,32*l5_u};
        DrawTexturePro(l5_icicle_texture,source,dest,origin,0,WHITE);
    }

    //crates and signposts: the plain, readable furniture of the course
    for (int i=0; i<3; i++)
    {
        Rectangle source = {0,0,256,256};
        DrawTexturePro(l5_crate_texture,source,l5_crate[i],no_origin,0,WHITE);
    }
    for (int i=0; i<3; i++)
    {
        Rectangle source = {0,0,256,256};
        Rectangle dest = {l5_sign[i].x,l5_sign[i].y,90*l5_u,90*l5_u};
        Vector2 origin = {45*l5_u,45*l5_u};
        DrawTexturePro(l5_sign_texture,source,dest,origin,0,WHITE);
    }

    //ice boulders
    for (int i=0; i<4; i++)
    {
        float size = l5_boulder_radius*2.3;
        if (l5_boulder_hit[i]>0) size = size + 8*l5_u*l5_boulder_hit[i]/0.2;
        Rectangle source = {(i%3)*256,0,256,256};
        Rectangle dest = {l5_boulder[i].x,l5_boulder[i].y,size,size};
        Vector2 origin = {size/2,size/2};
        DrawTexturePro(l5_boulder_texture,source,dest,origin,i*23,WHITE);
    }

    //the toboggan
    Rectangle sled_source = {(l5_toboggan_speed>0 ? 256 : 0),0,256,256};
    DrawTexturePro(l5_toboggan_texture,sled_source,l5_toboggan,no_origin,0,WHITE);

    //hares and ibex
    for (int i=0; i<4; i++)
    {
        int frame = 0;
        if (l5_hare_hop[i]>0) frame = 1 + (int)((0.5-l5_hare_hop[i])*8)%3;
        Rectangle source = {frame*128,0,128,128};
        Rectangle dest = {l5_hare[i].x,l5_hare[i].y,56*l5_u,56*l5_u};
        Vector2 origin = {28*l5_u,28*l5_u};
        DrawTexturePro(l5_hare_texture,source,dest,origin,0,WHITE);
    }
    for (int i=0; i<3; i++)
    {
        int frame = (int)(l5_animation_time*7+i)%4;
        Rectangle source = {frame*128,0,128,128};
        float size = l5_ibex_radius*2.5;
        Rectangle dest = {l5_ibex[i].x,l5_ibex[i].y,size,size};
        Vector2 origin = {size/2,size/2};
        float facing = 90;
        if (l5_ibex_speed[i]<0) facing = -90;
        DrawTexturePro(l5_ibex_texture,source,dest,origin,facing,WHITE);
    }

    //pines
    for (int i=0; i<5; i++)
    {
        Rectangle source = {(i%3)*384,0,384,384};
        //the sprite fills about 0.9 of its cell, so this makes the picture match the circle
        float size = l5_pine_radius*2.25;
        Rectangle dest = {l5_pine[i].x,l5_pine[i].y,size,size};
        Vector2 origin = {size/2,size/2};
        DrawTexturePro(l5_pine_texture,source,dest,origin,i*37,WHITE);
    }
}


void l5_draw_ball_and_pot()
{
    //the hole, with a flag on a pole
    DrawCircleV(l5_pot,l5_radius_pot+3*l5_u,Fade(BLACK,0.45));
    DrawCircleV(l5_pot,l5_radius_pot,l5_crevasse_deep);
    DrawLineEx(l5_pot,(Vector2){l5_pot.x,l5_pot.y-54*l5_u},3*l5_u,l5_rock);
    Vector2 flag_a = {l5_pot.x,l5_pot.y-54*l5_u};
    Vector2 flag_b = {l5_pot.x,l5_pot.y-30*l5_u};
    Vector2 flag_c = {l5_pot.x+30*l5_u+sin(l5_animation_time*3)*5*l5_u,l5_pot.y-42*l5_u};
    DrawTriangle(flag_a,flag_b,flag_c,l5_flag_red);

    //the ball
    DrawCircleV(l5_ball,l5_radius_ball,WHITE);
    DrawCircleLines(l5_ball.x,l5_ball.y,l5_radius_ball,GRAY);
    DrawCircle(l5_ball.x-2*l5_u,l5_ball.y-2*l5_u,2*l5_u,WHITE);

    if (l5_aiming==1 && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        Vector2 mouse = {GetMouseX(),GetMouseY()};
        DrawLineEx(l5_ball,mouse,4*l5_u,Fade(WHITE,0.75));
        DrawCircle(mouse.x,mouse.y,5*l5_u,Fade(WHITE,0.75));
    }
}


//the weather on top of everything: blowing snow, then the whiteout at the peak of a gale
void l5_draw_weather()
{
    //flag lines lean the way the wind is blowing, so you can read the gale before it hits
    for (int i=0; i<3; i++)
    {
        float width = l5_flagline_width[i];
        DrawLineEx(l5_flagline[i],(Vector2){l5_flagline[i].x+width,l5_flagline[i].y},3*l5_u,Fade(GetColor(0x6E5B42FF),0.9));
        int frame = (int)(l5_animation_time*6+i)%4;
        Rectangle source = {frame*256,0,256,256};
        Rectangle dest = {l5_flagline[i].x+width/2,l5_flagline[i].y+18*l5_u,width,110*l5_u};
        Vector2 origin = {width/2,55*l5_u};
        float lean = l5_wind.x/l5_u*0.03;
        DrawTexturePro(l5_flags_texture,source,dest,origin,lean,WHITE);
    }

    //bands of blowing snow, travelling with the wind
    for (int band=0; band<4; band++)
    {
        int frame = (int)(l5_animation_time*9+band)%4;
        Rectangle source = {frame*256,0,256,256};
        float y = 120*l5_u + band*(l5_height-200*l5_u)/4;
        float offset = fmod(l5_drift_offset*(0.6+band*0.2),l5_width);
        for (int k=-1; k<=l5_width/(420*l5_u)+1; k++)
        {
            Rectangle dest = {offset + k*420*l5_u,y,420*l5_u,150*l5_u};
            DrawTexturePro(l5_spindrift_texture,source,dest,no_origin_global,0,Fade(WHITE,0.55));
        }
    }

    float drift = l5_wind_clock*l5_wind_strength*l5_u;
    for (int i=0; i<150; i++)
    {
        float speed = 0.6 + (i%5)*0.18;
        float x = fmod(i*173*l5_u + drift*speed,l5_width+200*l5_u) - 100*l5_u;
        float y = fmod(i*241*l5_u + drift*speed*0.25,l5_height);
        float size = 1.5*l5_u + (i%3)*0.9*l5_u;
        DrawCircle(x,y,size,Fade(WHITE,0.35+0.3*(i%3)/2.0));
    }
    if (l5_whiteout>0) DrawRectangle(0,0,l5_width,l5_height,Fade(WHITE,l5_whiteout));

    //the avalanche, warned first on the left edge, then a wall of snow coming down
    if (l5_avalanche_state==1 && fmod(l5_animation_time*6,2)<1.3)
    {
        DrawRectangle(0,0,60*l5_u,l5_height,Fade(WHITE,0.30));
        DrawText("AVALANCHE",70*l5_u,l5_height/2-40*l5_u,56*l5_u,Fade(l5_flag_red,0.9));
    }
    if (l5_avalanche_state==2)
    {
        for (int k=0; k<40; k++)
        {
            float x = fmod(k*137*l5_u,520*l5_u);
            float y = l5_avalanche_y + sin(k*1.7+l5_animation_time*8)*40*l5_u;
            DrawCircle(x,y,26*l5_u+(k%4)*8*l5_u,Fade(WHITE,0.75));
        }
        DrawRectangle(0,l5_avalanche_y-110*l5_u,520*l5_u,220*l5_u,Fade(WHITE,0.55));
    }
}


void l5_draw_hud()
{
    //dark edges
    DrawRectangleGradientH(0,l5_hud_height,90*l5_u,l5_height-l5_hud_height,Fade(BLACK,0.25),BLANK);
    DrawRectangleGradientH(l5_width-90*l5_u,l5_hud_height,90*l5_u,l5_height-l5_hud_height,BLANK,Fade(BLACK,0.25));
    DrawRectangleGradientV(0,l5_height-70*l5_u,l5_width,70*l5_u,BLANK,Fade(BLACK,0.25));

    //an ice bar across the top
    DrawRectangleGradientV(0,0,l5_width,l5_hud_height,l5_ice_light,l5_ice);
    DrawRectangle(0,l5_hud_height-4*l5_u,l5_width,4*l5_u,l5_crevasse);
    int studs = l5_width/(40*l5_u);
    for (int i=0; i<studs; i++)
    {
        DrawCircle(20*l5_u+i*40*l5_u,8*l5_u,3*l5_u,Fade(l5_crevasse,0.6));
        DrawCircle(20*l5_u+i*40*l5_u,l5_hud_height-12*l5_u,3*l5_u,Fade(l5_crevasse,0.6));
    }

    DrawText("LEVEL 5 - FROZEN PEAK",32*l5_u,20*l5_u,36*l5_u,BLACK);
    DrawText("LEVEL 5 - FROZEN PEAK",30*l5_u,18*l5_u,36*l5_u,WHITE);
    DrawText(TextFormat("STROKES %d / %d",l5_stroke,l5_stroke_limit),l5_width/2-298*l5_u,21*l5_u,32*l5_u,BLACK);
    DrawText(TextFormat("STROKES %d / %d",l5_stroke,l5_stroke_limit),l5_width/2-300*l5_u,19*l5_u,32*l5_u,WHITE);
    for (int i=0; i<l5_stroke_limit; i++)
    {
        Color pip = Fade(l5_crevasse,0.55);
        if (i<l5_stroke) pip = l5_flag_red;
        DrawCircle(l5_width/2+10*l5_u+i*22*l5_u,35*l5_u,7*l5_u,pip);
        DrawCircleLines(l5_width/2+10*l5_u+i*22*l5_u,35*l5_u,7*l5_u,BLACK);
    }
    int help_width = MeasureText("R restart   ESC menu",26*l5_u);
    DrawText("R restart   ESC menu",l5_width-help_width-28*l5_u,24*l5_u,26*l5_u,BLACK);
    DrawText("R restart   ESC menu",l5_width-help_width-30*l5_u,22*l5_u,26*l5_u,WHITE);

    //the weather vane: it says which way the wind is blowing and how hard, right now
    Vector2 vane = {l5_width/2-430*l5_u,35*l5_u};
    DrawCircleV(vane,20*l5_u,Fade(l5_crevasse,0.35));
    float point = 0;
    if (l5_wind.x<0) point = PI;
    float wave = sin(l5_animation_time*9)*0.08*(l5_wind_strength/290.0);
    Vector2 tip = {vane.x+cos(point+wave)*26*l5_u,vane.y+sin(point+wave)*26*l5_u};
    DrawLineEx(vane,tip,4*l5_u,WHITE);
    DrawCircleV(tip,5*l5_u,l5_flag_red);
    const char *weather = "BREEZE";
    if (l5_wind_strength>100) weather = "GUST";
    if (l5_wind_strength>200) weather = "GALE";
    DrawText(weather,vane.x+30*l5_u,26*l5_u,22*l5_u,WHITE);

    //what just happened
    if (l5_message_timer>0)
    {
        const char *say = "CREVASSE!";
        if (l5_message_type==2) say = "BURIED!";
        if (l5_message_type==3) say = "SMASHED!";
        if (l5_message_type==4) say = "AVALANCHE!";
        DrawText(say,l5_width/2-MeasureText(say,90*l5_u)/2+4*l5_u,l5_height/2-41*l5_u,90*l5_u,Fade(l5_crevasse,l5_message_timer));
        DrawText(say,l5_width/2-MeasureText(say,90*l5_u)/2,l5_height/2-45*l5_u,90*l5_u,Fade(WHITE,l5_message_timer));
    }

    //level clear or failed
    if (l5_game_state!=0)
    {
        DrawRectangle(0,0,l5_width,l5_height,Fade(BLACK,0.6));
        Rectangle panel = {l5_width/2-340*l5_u,l5_height/2-170*l5_u,680*l5_u,340*l5_u};
        DrawRectangleRec(panel,l5_crevasse);
        Rectangle panel_band = {panel.x,panel.y,panel.width,24*l5_u};
        DrawRectangleRec(panel_band,l5_ice_light);
        DrawRectangleLinesEx(panel,6*l5_u,l5_ice_light);
        if (l5_game_state==1)
        {
            DrawText("LEVEL CLEAR!",l5_width/2-MeasureText("LEVEL CLEAR!",80*l5_u)/2,panel.y+60*l5_u,80*l5_u,l5_ice_light);
        }
        else
        {
            DrawText("FAILED",l5_width/2-MeasureText("FAILED",80*l5_u)/2,panel.y+60*l5_u,80*l5_u,l5_flag_red);
        }
        int strokes_width = MeasureText(TextFormat("Strokes: %d / %d",l5_stroke,l5_stroke_limit),40*l5_u);
        DrawText(TextFormat("Strokes: %d / %d",l5_stroke,l5_stroke_limit),l5_width/2-strokes_width/2,panel.y+170*l5_u,40*l5_u,WHITE);
        DrawText("R play again   ESC menu",l5_width/2-MeasureText("R play again   ESC menu",30*l5_u)/2,panel.y+250*l5_u,30*l5_u,l5_ice_light);
    }
}


//level 5: keep the mountain moving with no ball (intro and menu)
void l5_background_step(float dt)
{
    l5_update_obstacles(dt);
}


//level 5: draw everything except the scoreboard (intro and menu)
void l5_draw_scene()
{
    l5_draw_ground();
    l5_draw_obstacles();
    l5_draw_ball_and_pot();
    l5_draw_weather();
}


//level 5: set the screen size, load pictures, start the level
void l5_start(int screen_width, int screen_height)
{
    l5_width = screen_width;
    l5_height = screen_height;

    l5_snow_texture = LoadTexture("assets/frozen/frozen_snow_tile.png");
    l5_ice_texture = LoadTexture("assets/frozen/frozen_ice_tile.png");
    l5_pine_texture = LoadTexture("assets/frozen/frozen_pine.png");
    l5_snowball_texture = LoadTexture("assets/frozen/frozen_snowball.png");
    l5_gondola_texture = LoadTexture("assets/frozen/frozen_gondola.png");
    l5_bridge_texture = LoadTexture("assets/frozen/frozen_bridge.png");
    l5_campfire_texture = LoadTexture("assets/frozen/frozen_campfire.png");
    l5_icicle_texture = LoadTexture("assets/frozen/frozen_icicle.png");
    l5_ibex_texture = LoadTexture("assets/frozen/frozen_ibex.png");
    l5_hare_texture = LoadTexture("assets/frozen/frozen_hare.png");
    l5_flags_texture = LoadTexture("assets/frozen/frozen_flags.png");
    l5_toboggan_texture = LoadTexture("assets/frozen/frozen_toboggan.png");
    l5_crate_texture = LoadTexture("assets/frozen/frozen_crate.png");
    l5_boulder_texture = LoadTexture("assets/frozen/frozen_boulder.png");
    l5_spindrift_texture = LoadTexture("assets/frozen/frozen_spindrift.png");
    l5_sign_texture = LoadTexture("assets/frozen/frozen_sign.png");
    SetTextureWrap(l5_snow_texture,TEXTURE_WRAP_REPEAT);
    SetTextureWrap(l5_ice_texture,TEXTURE_WRAP_REPEAT);
    SetTextureFilter(l5_snow_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l5_ice_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l5_pine_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l5_snowball_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l5_gondola_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l5_bridge_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l5_campfire_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l5_icicle_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l5_ibex_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l5_hare_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l5_flags_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l5_toboggan_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l5_crate_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l5_boulder_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l5_spindrift_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l5_sign_texture,TEXTURE_FILTER_BILINEAR);

    //every size is N*l5_u, so it looks the same on any screen
    l5_u = l5_height/1080.0;
    if (l5_width/1920.0 < l5_u) l5_u = l5_width/1920.0;
    l5_reset_level();
}


//level 5: one frame of input and movement
void l5_update()
{
    float dt = GetFrameTime();
    if (dt>1.0/30) dt = 1.0/30;
    if (l5_message_timer>0) l5_message_timer = l5_message_timer - dt;

    //restart
    if (IsKeyPressed(KEY_R)) l5_reset_level();

    //shooting (the click has to start while the ball is still)
    int ball_stopped = 0;
    if (l5_speed.x==0 && l5_speed.y==0) ball_stopped = 1;
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && ball_stopped==1 && l5_game_state==0) l5_aiming = 1;
    if (l5_game_state!=0) l5_aiming = 0;
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && l5_aiming==1)
    {
        l5_aiming = 0;
        Vector2 mouse = {GetMouseX(),GetMouseY()};
        Vector2 drag = Vector2Subtract(l5_ball,mouse);
        if (ball_stopped==1 && l5_stroke<l5_stroke_limit && Vector2Length(drag)>=2*l5_radius_ball)
        {
            l5_last_shot_position = l5_ball;
            l5_speed = Vector2ClampValue(Vector2Scale(drag,3.125),0,l5_max_speed*l5_u);
            l5_stroke++;
        }
    }

    //moving everything in 4 small steps, so nothing jumps through anything
    for (int i=0; i<4; i++)
    {
        l5_update_obstacles(dt/4*obstacle_speed);
        if (l5_game_state==0) l5_update_ball(dt/4);
    }

    //out of strokes
    if (l5_game_state==0 && l5_stroke>=l5_stroke_limit && l5_speed.x==0 && l5_speed.y==0) l5_game_state = 2;
}


//level 5: one frame of drawing
void l5_draw()
{
    l5_draw_ground();
    l5_draw_obstacles();
    l5_draw_ball_and_pot();
    draw_straight_preview(l5_ball,l5_aiming,l5_radius_ball,l5_max_speed*l5_u,l5_u);
    l5_draw_weather();
    l5_draw_hud();
}


void l5_unload()
{
    UnloadTexture(l5_snow_texture);
    UnloadTexture(l5_ice_texture);
    UnloadTexture(l5_pine_texture);
    UnloadTexture(l5_snowball_texture);
    UnloadTexture(l5_gondola_texture);
    UnloadTexture(l5_bridge_texture);
    UnloadTexture(l5_campfire_texture);
    UnloadTexture(l5_icicle_texture);
    UnloadTexture(l5_ibex_texture);
    UnloadTexture(l5_hare_texture);
    UnloadTexture(l5_flags_texture);
    UnloadTexture(l5_toboggan_texture);
    UnloadTexture(l5_crate_texture);
    UnloadTexture(l5_boulder_texture);
    UnloadTexture(l5_spindrift_texture);
    UnloadTexture(l5_sign_texture);
}
