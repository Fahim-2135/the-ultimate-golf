//==================== LEVEL 7 ====================
//Magma Core: a descent into an erupting volcano. The lava climbs the whole time, so the
//bottom of the course stops existing, and the hole only opens for a few seconds after an
//eruption drains away. The one level you cannot take your time over.

//full global
int l7_width = 1920;
int l7_height = 1080;
float l7_u = 1;
int l7_stroke_base = 26;
int l7_stroke_limit = 18;
#define l7_max_speed 650

//colours
Color l7_rock = {58,58,62,255};
Color l7_rock_dark = {26,26,30,255};
Color l7_rock_light = {85,85,92,255};
Color l7_lava = {255,106,31,255};
Color l7_lava_hot = {255,233,168,255};
Color l7_ember = {255,197,61,255};
Color l7_steam = {222,226,232,255};
Color l7_crystal_purple = {138,111,209,255};

//pictures
Texture2D l7_rock_texture;
Texture2D l7_lava_texture;
Texture2D l7_crust_texture;
Texture2D l7_vent_texture;
Texture2D l7_eruption_texture;
Texture2D l7_steam_texture;
Texture2D l7_catwalk_texture;
Texture2D l7_tube_texture;
Texture2D l7_heart_texture;
Texture2D l7_geyser_texture;
Texture2D l7_crystal_texture;
Texture2D l7_raft_texture;
Texture2D l7_salamander_texture;
Texture2D l7_lavafall_texture;

//ball and pot
Vector2 l7_ball;
Vector2 l7_speed;
float l7_radius_ball;
Vector2 l7_pot;
float l7_radius_pot;
Vector2 l7_start_position;
Vector2 l7_last_shot_position;
int l7_stroke = 0;
int l7_game_state = 0;
int l7_aiming = 0;
float l7_animation_time = 0;
float l7_hud_height;

//messages: 1 into the lava, 2 down a vent, 3 burnt on hot crust, 4 crushed by falling rock
int l7_message_type = 0;
float l7_message_timer = 0;

//the ground: rock ledges are safe, everything else is the lava lake
Rectangle l7_ledge[10];

//the lava level climbs the screen: anything below this line is gone
float l7_lava_line;
float l7_lava_rise;

//cooling crust: safe while grey, deadly while it glows, on its own clock
Rectangle l7_crust[3];
float l7_crust_clock[3];

//cracked bridges over the lake
Rectangle l7_plank[2][7];
int l7_plank_count[2] = {6,7};
int l7_plank_state[2][7];
float l7_plank_timer[2][7];

//eruption vents: the hole is always deadly, and now and then it floods a circle around it
Vector2 l7_vent[3];
float l7_vent_hole;
float l7_vent_reach;
float l7_vent_wait[3];
float l7_vent_stage[3];        //0 while dormant, then counts up through the eruption
int l7_vent_live[3];

//geysers throw the ball across the room in one shove
Vector2 l7_geyser[2];
Vector2 l7_geyser_kick[2];
float l7_geyser_clock[2];

//rafts of cooled crust drifting over the lake
Rectangle l7_raft[2];
float l7_raft_from[2];
float l7_raft_to[2];
float l7_raft_speed[2];

//a lava tube: in one end, out of the other
Vector2 l7_tube_in;
Vector2 l7_tube_out;
float l7_tube_facing;
float l7_tube_cooldown = 0;

//steam vents push sideways in bursts
Rectangle l7_steam_jet[2];
Vector2 l7_steam_push[2];
float l7_steam_clock[2];

//obsidian crystals, hard and bouncy
Vector2 l7_crystal[4];
float l7_crystal_radius;
float l7_crystal_hit[4];

//--- the lake is never still ---
//bubbles swell and pop all over the lava, each on its own clock

//salamanders skitter along the ledges: harmless, but always moving
Vector2 l7_salamander[3];
float l7_salamander_radius;
Vector2 l7_salamander_from[3];
Vector2 l7_salamander_to[3];
float l7_salamander_travel[3];
float l7_salamander_speed[3];

//smoke plumes drifting up off the hot spots

//pumice bobbing on the lake, and a lava fall pouring down one wall
Rectangle l7_lavafall;
float l7_lavafall_clock = 0;     //2.5 seconds pouring, 2.5 seconds dry: a curtain to time

//the rock shower: once or twice a game the ceiling gives way
float l7_shower_wait = 0;
int l7_shower_state = 0;       //0 waiting, 1 warning, 2 falling
float l7_shower_timer = 0;
float l7_shower_x = 0;

//the heart: the hole is flooded most of the time and opens after an eruption drains
float l7_heart_open = 0;


Rectangle l7_make_rect(float x, float y, float w, float h)
{
    Rectangle rec = {x*l7_u,y*l7_u,w*l7_u,h*l7_u};
    return rec;
}

Vector2 l7_make_point(float x, float y)
{
    Vector2 point = {x*l7_u,y*l7_u};
    return point;
}


void l7_reset_level()
{
    l7_hud_height = 70*l7_u;
    l7_radius_ball = 7*l7_u;
    l7_radius_pot = 11*l7_u;

    //SIX TERRACES, descending, joined at alternating ends. Each terrace is one idea
    //with room to breathe: crust, vents, the lava fall, geysers, crystals, the heart.
    l7_ledge[0] = l7_make_rect(60,100,1640,120);     //1 the rim, where you come in
    l7_ledge[1] = l7_make_rect(220,270,1640,120);    //2 the crust shelf
    l7_ledge[2] = l7_make_rect(60,440,1640,120);     //3 the vent terrace
    l7_ledge[3] = l7_make_rect(220,610,1640,120);    //4 the lava fall
    l7_ledge[4] = l7_make_rect(60,780,1640,120);     //5 the geyser run
    l7_ledge[5] = l7_make_rect(220,940,1640,110);    //6 the heart chamber
    //the joins the bridges do not make: three rock ramps
    l7_ledge[6] = l7_make_rect(220,270,170,290);     //2 -> 3, left
    l7_ledge[7] = l7_make_rect(220,610,170,290);     //4 -> 5, left
    l7_ledge[8] = l7_make_rect(1530,780,170,270);    //5 -> 6, right
    l7_ledge[9] = l7_make_rect(-400,-400,10,10);

    //the lava climbs and stops under the heart chamber, so the floor you came in on
    //goes first and the level stays winnable
    l7_lava_line = 1080*l7_u;
    l7_lava_rise = 1.6*l7_u;

    //terrace 2 is the crust: three slabs with long gaps between them
    l7_crust[0] = l7_make_rect(480,280,150,100);
    l7_crust[1] = l7_make_rect(900,280,150,100);
    l7_crust[2] = l7_make_rect(1320,280,150,100);
    l7_crust_clock[0] = 0;
    l7_crust_clock[1] = 2;
    l7_crust_clock[2] = 4;

    //two catwalks over the lake, each at the end of its terrace
    for (int i=0; i<5; i++) l7_plank[0][i] = l7_make_rect(1546+i*34,200,34,100);  //1 -> 2, right
    for (int i=0; i<5; i++) l7_plank[1][i] = l7_make_rect(1546+i*34,540,34,110);  //3 -> 4, right
    for (int b=0; b<2; b++)
    {
        for (int i=0; i<7; i++)
        {
            l7_plank_state[b][i] = 0;
            l7_plank_timer[b][i] = 0;
        }
    }

    //terrace 3 is the vents: three of them, far apart, with room to go round
    l7_vent_hole = 30*l7_u;
    l7_vent_reach = 100*l7_u;
    l7_vent[0] = l7_make_point(520,455);
    l7_vent[1] = l7_make_point(980,545);
    l7_vent[2] = l7_make_point(1440,455);
    for (int i=0; i<3; i++)
    {
        l7_vent_wait[i] = 5 + i*3.5;
        l7_vent_stage[i] = 0;
        l7_vent_live[i] = 0;
    }

    //terrace 5 is the geysers: they throw you along the run
    l7_geyser[0] = l7_make_point(420,840);
    l7_geyser_kick[0] = l7_make_point(540,0);
    l7_geyser[1] = l7_make_point(1120,840);
    l7_geyser_kick[1] = l7_make_point(540,0);
    l7_geyser_clock[0] = 0;
    l7_geyser_clock[1] = 1.6;

    //rafts drift in the lake between terraces: scenery with a purpose, they show you
    //the lake is moving
    l7_raft[0] = l7_make_rect(500,230,170,110);
    l7_raft_from[0] = 420*l7_u;
    l7_raft_to[0] = 1000*l7_u;
    l7_raft_speed[0] = 90*l7_u;
    l7_raft[1] = l7_make_rect(900,900,170,110);
    l7_raft_from[1] = 700*l7_u;
    l7_raft_to[1] = 1300*l7_u;
    l7_raft_speed[1] = -80*l7_u;

    //the lava tube: it takes you from the end of terrace 4 down onto terrace 5, which
    //is the one shortcut on the way down
    l7_tube_in = l7_make_point(1000,800);
    l7_tube_out = l7_make_point(1700,670);
    l7_tube_facing = 180;
    l7_tube_cooldown = 0;

    //steam across terrace 4 and the heart chamber
    l7_steam_jet[0] = l7_make_rect(900,610,140,120);
    l7_steam_push[0] = l7_make_point(0,-420);
    l7_steam_jet[1] = l7_make_rect(1120,940,140,110);
    l7_steam_push[1] = l7_make_point(420,0);
    l7_steam_clock[0] = 0;
    l7_steam_clock[1] = 2;

    //crystals: one per terrace at most, as bumpers you can use
    l7_crystal_radius = 34*l7_u;
    l7_crystal[0] = l7_make_point(900,122);
    l7_crystal[1] = l7_make_point(1300,632);
    l7_crystal[2] = l7_make_point(760,802);
    l7_crystal[3] = l7_make_point(700,958);
    for (int i=0; i<4; i++) l7_crystal_hit[i] = 0;

    l7_shower_wait = 24;
    l7_shower_state = 0;
    l7_shower_timer = 0;
    l7_shower_x = 0;
    l7_heart_open = 0;

    //salamanders patrol three of the terraces and shoulder the ball aside, the same
    //way the ibex do up on the peak. Nothing on this level is only scenery.
    l7_salamander_radius = 22*l7_u;
    l7_salamander_from[0] = l7_make_point(300,160);
    l7_salamander_to[0] = l7_make_point(1100,160);
    l7_salamander_speed[0] = 0.26;
    l7_salamander_from[1] = l7_make_point(500,670);
    l7_salamander_to[1] = l7_make_point(1200,670);
    l7_salamander_speed[1] = 0.22;
    l7_salamander_from[2] = l7_make_point(900,995);
    l7_salamander_to[2] = l7_make_point(1600,995);
    l7_salamander_speed[2] = 0.3;
    for (int i=0; i<3; i++)
    {
        l7_salamander_travel[i] = i*0.4;
        l7_salamander[i] = l7_salamander_from[i];
    }


    //the lava fall pours across terrace 4, on for 2.5 seconds, off for 2.5
    l7_lavafall = l7_make_rect(600,610,110,120);
    l7_lavafall_clock = 0;

    //ball and heart, a whole descent apart
    l7_start_position = l7_make_point(340,995);
    l7_ball = l7_start_position;
    l7_last_shot_position = l7_start_position;
    l7_speed.x = 0;
    l7_speed.y = 0;
    l7_pot = l7_make_point(200,160);
    l7_stroke = 0;
    l7_game_state = 0;
    l7_aiming = 0;
    l7_message_type = 0;
    l7_message_timer = 0;
}


int l7_bounce_off_rectangle(Rectangle rec, Vector2 rec_speed)
{
    if (CheckCollisionCircleRec(l7_ball,l7_radius_ball,rec))
    {
        Vector2 collision_point;
        collision_point.x = Clamp(l7_ball.x,rec.x,rec.x+rec.width);
        collision_point.y = Clamp(l7_ball.y,rec.y,rec.y+rec.height);
        Vector2 normal = Vector2Subtract(l7_ball,collision_point);
        if (normal.x==0 && normal.y==0)
        {
            float left = l7_ball.x - rec.x;
            float right = rec.x + rec.width - l7_ball.x;
            float top = l7_ball.y - rec.y;
            float bottom = rec.y + rec.height - l7_ball.y;
            if (left<=right && left<=top && left<=bottom) { normal.x = -1; collision_point.x = rec.x; }
            else if (right<=top && right<=bottom) { normal.x = 1; collision_point.x = rec.x + rec.width; }
            else if (top<=bottom) { normal.y = -1; collision_point.y = rec.y; }
            else { normal.y = 1; collision_point.y = rec.y + rec.height; }
        }
        normal = Vector2Normalize(normal);
        l7_ball = Vector2Add(collision_point,Vector2Scale(normal,l7_radius_ball));
        Vector2 relative_speed = Vector2Subtract(l7_speed,rec_speed);
        if (Vector2DotProduct(relative_speed,normal)<0)
        {
            relative_speed = Vector2Reflect(relative_speed,normal);
            l7_speed = Vector2ClampValue(Vector2Add(relative_speed,rec_speed),0,l7_max_speed*l7_u);
        }
        return 1;
    }
    return 0;
}


int l7_bounce_off_circle(Vector2 center, float radius, float bounce)
{
    Vector2 normal = Vector2Subtract(l7_ball,center);
    float distance = Vector2Length(normal);
    if (distance < radius + l7_radius_ball)
    {
        if (distance==0) normal.y = -1;
        normal = Vector2Normalize(normal);
        l7_ball = Vector2Add(center,Vector2Scale(normal,radius + l7_radius_ball));
        if (Vector2DotProduct(l7_speed,normal)<0)
        {
            l7_speed = Vector2ClampValue(Vector2Scale(Vector2Reflect(l7_speed,normal),bounce),0,l7_max_speed*l7_u);
            return 1;
        }
    }
    return 0;
}


//standing on rock, on a plank that has not fallen, or on a raft - and above the rising lava
int l7_on_safe_ground(Vector2 point)
{
    if (point.y > l7_lava_line) return 0;
    for (int i=0; i<10; i++)
    {
        if (CheckCollisionPointRec(point,l7_ledge[i])) return 1;
    }
    for (int i=0; i<3; i++)
    {
        if (CheckCollisionPointRec(point,l7_crust[i])) return 1;
    }
    for (int b=0; b<2; b++)
    {
        for (int i=0; i<l7_plank_count[b]; i++)
        {
            if (l7_plank_state[b][i]<2 && CheckCollisionPointRec(point,l7_plank[b][i])) return 1;
        }
    }
    for (int i=0; i<2; i++)
    {
        if (CheckCollisionPointRec(point,l7_raft[i])) return 1;
    }
    return 0;
}


void l7_update_obstacles(float dt)
{
    l7_animation_time = l7_animation_time + dt;

    //the lava climbs, and never goes back down
    l7_lava_line = l7_lava_line - l7_lava_rise*dt;
    if (l7_lava_line < 400*l7_u) l7_lava_line = 400*l7_u;   //it stops two terraces below the heart: the level has to stay winnable

    //the lava fall pours, then stops, then pours again
    l7_lavafall_clock = l7_lavafall_clock + dt;
    if (l7_lavafall_clock>=5) l7_lavafall_clock = l7_lavafall_clock - 5;

    //salamanders
    for (int i=0; i<3; i++)
    {
        l7_salamander_travel[i] = l7_salamander_travel[i] + dt*l7_salamander_speed[i];
        if (l7_salamander_travel[i]>2) l7_salamander_travel[i] = l7_salamander_travel[i] - 2;
        float t = l7_salamander_travel[i];
        if (t>1) t = 2 - t;
        l7_salamander[i] = Vector2Lerp(l7_salamander_from[i],l7_salamander_to[i],t);
    }


    //cooling crust: 4 seconds safe, 2 seconds deadly
    for (int i=0; i<3; i++)
    {
        l7_crust_clock[i] = l7_crust_clock[i] + dt;
        if (l7_crust_clock[i]>=6) l7_crust_clock[i] = l7_crust_clock[i] - 6;
    }

    //plank timers, the same three states the temple bridges use
    for (int b=0; b<2; b++)
    {
        for (int i=0; i<l7_plank_count[b]; i++)
        {
            if (l7_plank_state[b][i]==0) continue;
            l7_plank_timer[b][i] = l7_plank_timer[b][i] + dt;
            if (l7_plank_state[b][i]==1 && l7_plank_timer[b][i]>=0.55)
            {
                l7_plank_state[b][i] = 2;
                l7_plank_timer[b][i] = 0;
            }
            else if (l7_plank_state[b][i]==2 && l7_plank_timer[b][i]>=5)
            {
                l7_plank_state[b][i] = 0;
                l7_plank_timer[b][i] = 0;
            }
        }
    }

    //the vents: a long wait, then build, erupt, and drain back in
    for (int i=0; i<3; i++)
    {
        if (l7_vent_live[i]==0)
        {
            l7_vent_wait[i] = l7_vent_wait[i] - dt;
            if (l7_vent_wait[i]<=0)
            {
                l7_vent_live[i] = 1;
                l7_vent_stage[i] = 0;
            }
        }
        else
        {
            l7_vent_stage[i] = l7_vent_stage[i] + dt;
            if (l7_vent_stage[i]>4.4)
            {
                l7_vent_live[i] = 0;
                l7_vent_stage[i] = 0;
                l7_vent_wait[i] = 11 + GetRandomValue(0,80)/10.0;
            }
        }
    }

    //the heart is open only while the vent beside it has just drained
    float heart_stage = l7_vent_stage[2];
    if (l7_vent_live[2]==1 && heart_stage>3.2) l7_heart_open = 1;
    else if (l7_vent_live[2]==0) l7_heart_open = 1;
    else l7_heart_open = 0;

    //geysers fire on their own clock
    for (int i=0; i<2; i++)
    {
        l7_geyser_clock[i] = l7_geyser_clock[i] + dt;
        if (l7_geyser_clock[i]>=3.2) l7_geyser_clock[i] = l7_geyser_clock[i] - 3.2;
    }

    //steam bursts
    for (int i=0; i<2; i++)
    {
        l7_steam_clock[i] = l7_steam_clock[i] + dt;
        if (l7_steam_clock[i]>=4) l7_steam_clock[i] = l7_steam_clock[i] - 4;
    }

    //rafts drift
    for (int i=0; i<2; i++)
    {
        l7_raft[i].x = l7_raft[i].x + l7_raft_speed[i]*dt;
        if (l7_raft[i].x<l7_raft_from[i])
        {
            l7_raft[i].x = l7_raft_from[i];
            l7_raft_speed[i] = fabsf(l7_raft_speed[i]);
        }
        if (l7_raft[i].x>l7_raft_to[i])
        {
            l7_raft[i].x = l7_raft_to[i];
            l7_raft_speed[i] = -fabsf(l7_raft_speed[i]);
        }
    }

    //crystals flash when they have just been hit
    for (int i=0; i<4; i++)
    {
        if (l7_crystal_hit[i]>0) l7_crystal_hit[i] = l7_crystal_hit[i] - dt;
    }

    //the ceiling gives way, with a warning first
    if (l7_shower_state==0)
    {
        l7_shower_wait = l7_shower_wait - dt;
        if (l7_shower_wait<=0)
        {
            l7_shower_state = 1;
            l7_shower_timer = 0;
            l7_shower_x = (300 + GetRandomValue(0,1200))*l7_u;
        }
    }
    else if (l7_shower_state==1)
    {
        l7_shower_timer = l7_shower_timer + dt;
        if (l7_shower_timer>=1.5)
        {
            l7_shower_state = 2;
            l7_shower_timer = 0;
        }
    }
    else
    {
        l7_shower_timer = l7_shower_timer + dt;
        if (l7_shower_timer>=2.4)
        {
            l7_shower_state = 0;
            l7_shower_timer = 0;
            l7_shower_wait = 22 + GetRandomValue(0,120)/10.0;
        }
    }

    if (l7_tube_cooldown>0) l7_tube_cooldown = l7_tube_cooldown - dt;
}


//how far an eruption has spread right now, 0 when the vent is quiet
float l7_vent_flood(int i)
{
    if (l7_vent_live[i]==0) return 0;
    float stage = l7_vent_stage[i];
    if (stage<1.5) return 0;                       //still building
    if (stage<2.2) return l7_vent_reach*(stage-1.5)/0.7;
    if (stage<3.4) return l7_vent_reach;
    return l7_vent_reach*(1 - (stage-3.4)/1.0);    //draining back in
}


void l7_send_ball_back()
{
    l7_ball = l7_last_shot_position;

    //never back into lava that has risen over the old spot, and never into a vent
    for (int i=0; i<3; i++)
    {
        if (Vector2Distance(l7_ball,l7_vent[i]) < l7_vent_hole + 3*l7_radius_ball)
        {
            l7_ball.x = l7_ball.x + l7_vent_hole + 4*l7_radius_ball;
        }
    }
    if (l7_on_safe_ground(l7_ball)==0)
    {
        //the ground it came from is gone: put it on the highest ledge that is still there
        l7_ball = l7_start_position;
        if (l7_on_safe_ground(l7_ball)==0)
        {
            Vector2 high = {l7_ledge[2].x+l7_ledge[2].width/2,l7_ledge[2].y+l7_ledge[2].height/2};
            l7_ball = high;
        }
    }
    l7_speed.x = 0;
    l7_speed.y = 0;
}


void l7_update_ball(float dt)
{
    int pushed = 0;

    //riding a raft
    for (int i=0; i<2; i++)
    {
        if (CheckCollisionPointRec(l7_ball,l7_raft[i])) l7_ball.x = l7_ball.x + l7_raft_speed[i]*dt;
    }

    //steam bursts push sideways
    for (int i=0; i<2; i++)
    {
        if (l7_steam_clock[i]<1.4 && CheckCollisionPointRec(l7_ball,l7_steam_jet[i]))
        {
            l7_speed = Vector2Add(l7_speed,Vector2Scale(l7_steam_push[i],dt));
            pushed = 1;
        }
    }

    //moving
    l7_ball = Vector2Add(l7_ball,Vector2Scale(l7_speed,dt));

    //friction
    float friction = 155*l7_u;
    float ball_speed = Vector2Length(l7_speed);
    if (ball_speed>0)
    {
        float new_speed = ball_speed - friction*dt;
        if (new_speed<0) new_speed = 0;
        l7_speed = Vector2Scale(l7_speed,new_speed/ball_speed);
    }
    if (pushed==0 && Vector2Length(l7_speed)<6*l7_u)
    {
        l7_speed.x = 0;
        l7_speed.y = 0;
    }

    //into the lake, or under the rising lava
    if (l7_on_safe_ground(l7_ball)==0)
    {
        l7_send_ball_back();
        l7_message_type = 1;
        l7_message_timer = 1;
        return;
    }

    //a vent hole swallows the ball, and the flood around an erupting one burns it
    for (int i=0; i<3; i++)
    {
        float d = Vector2Distance(l7_ball,l7_vent[i]);
        if (d < l7_vent_hole)
        {
            l7_send_ball_back();
            l7_message_type = 2;
            l7_message_timer = 1;
            return;
        }
        if (d < l7_vent_flood(i))
        {
            l7_send_ball_back();
            l7_message_type = 1;
            l7_message_timer = 1;
            return;
        }
    }

    //crust that is glowing right now
    for (int i=0; i<3; i++)
    {
        if (l7_crust_clock[i]>=4 && CheckCollisionPointRec(l7_ball,l7_crust[i]))
        {
            l7_send_ball_back();
            l7_message_type = 3;
            l7_message_timer = 1;
            return;
        }
    }

    //the lava fall itself: a curtain of lava across the terrace, but only while it pours
    if (l7_lavafall_clock<2.5 && CheckCollisionPointRec(l7_ball,l7_lavafall))
    {
        l7_send_ball_back();
        l7_message_type = 1;
        l7_message_timer = 1;
        return;
    }

    //rocks coming down
    if (l7_shower_state==2 && fabsf(l7_ball.x-l7_shower_x)<120*l7_u)
    {
        l7_send_ball_back();
        l7_message_type = 4;
        l7_message_timer = 1;
        return;
    }

    //standing on a plank cracks it
    for (int b=0; b<2; b++)
    {
        for (int i=0; i<l7_plank_count[b]; i++)
        {
            if (l7_plank_state[b][i]==0 && CheckCollisionPointRec(l7_ball,l7_plank[b][i]))
            {
                l7_plank_state[b][i] = 1;
                l7_plank_timer[b][i] = 0;
            }
        }
    }

    //geysers: one hard shove, not a push. Only while the plume is actually up.
    for (int i=0; i<2; i++)
    {
        if (l7_geyser_clock[i]<0.45 && Vector2Distance(l7_ball,l7_geyser[i])<70*l7_u)
        {
            l7_speed = Vector2ClampValue(Vector2Add(l7_speed,l7_geyser_kick[i]),0,l7_max_speed*l7_u);
        }
    }

    //the lava tube
    if (l7_tube_cooldown<=0 && Vector2Distance(l7_ball,l7_tube_in)<36*l7_u)
    {
        float facing = l7_tube_facing*DEG2RAD;
        Vector2 direction = {cos(facing),sin(facing)};
        float carried = Vector2Length(l7_speed);
        if (carried<220*l7_u) carried = 220*l7_u;
        l7_ball = Vector2Add(l7_tube_out,Vector2Scale(direction,46*l7_u));
        l7_speed = Vector2Scale(direction,carried);
        l7_tube_cooldown = 0.5;
    }

    //crystals
    for (int i=0; i<4; i++)
    {
        if (l7_bounce_off_circle(l7_crystal[i],l7_crystal_radius,1.3)) l7_crystal_hit[i] = 0.2;
    }

    //the rafts shove the ball when they catch it on the side
    for (int i=0; i<2; i++)
    {
        Vector2 raft_speed = {l7_raft_speed[i],0};
        l7_bounce_off_rectangle(l7_raft[i],raft_speed);
    }

    //salamanders shoulder the ball out of their way
    for (int i=0; i<3; i++)
    {
        if (Vector2Distance(l7_ball,l7_salamander[i]) < l7_salamander_radius+l7_radius_ball)
        {
            Vector2 away = Vector2Subtract(l7_ball,l7_salamander[i]);
            if (Vector2Length(away)<1) away = l7_make_point(0,1);
            away = Vector2Normalize(away);
            l7_ball = Vector2Add(l7_salamander[i],Vector2Scale(away,l7_salamander_radius+l7_radius_ball));
            l7_speed = Vector2ClampValue(Vector2Add(l7_speed,Vector2Scale(away,170*l7_u)),0,l7_max_speed*l7_u);
        }
    }

    //the heart, open only while the vent beside it is quiet
    if (l7_heart_open>0 && (l7_ball.x>l7_pot.x-3*l7_radius_pot/4) && (l7_ball.x<l7_pot.x+3*l7_radius_pot/4) && (l7_ball.y>l7_pot.y-3*l7_radius_pot/4) && (l7_ball.y<l7_pot.y+3*l7_radius_pot/4))
    {
        l7_ball = l7_pot;
        l7_speed.x = 0;
        l7_speed.y = 0;
        l7_game_state = 1;
    }
}


void l7_draw_lake()
{
    //the lake, drawn small and DARK. At full brightness it drowns the whole screen and
    //nothing else can be read on top of it.
    int frame = (int)(l7_animation_time*2)%2;
    float tile = 170*l7_u;
    int columns = l7_width/tile + 2;
    int rows = l7_height/tile + 2;
    Vector2 no_origin = {0,0};
    for (int x=0; x<columns; x++)
    {
        for (int y=0; y<rows; y++)
        {
            Rectangle source = {frame*512,0,512,512};
            Rectangle dest = {x*tile,y*tile,tile,tile};
            DrawTexturePro(l7_lava_texture,source,dest,no_origin,0,GetColor(0x8A6048FF));
        }
    }
    //a dark wash and a vignette, so the middle is calm and the edges fall away
    DrawRectangle(0,0,l7_width,l7_height,Fade(BLACK,0.42));
    DrawRectangleGradientV(0,0,l7_width,220*l7_u,Fade(BLACK,0.55),BLANK);
    DrawRectangleGradientV(0,l7_height-220*l7_u,l7_width,220*l7_u,BLANK,Fade(BLACK,0.55));
    DrawRectangleGradientH(0,0,260*l7_u,l7_height,Fade(BLACK,0.5),BLANK);
    DrawRectangleGradientH(l7_width-260*l7_u,0,260*l7_u,l7_height,BLANK,Fade(BLACK,0.5));

    //embers rising off the lake, the slow background motion
    for (int i=0; i<70; i++)
    {
        float speed = 30 + (i%5)*16;
        float x = fmod(i*271*l7_u + sin(l7_animation_time*0.4+i)*40*l7_u,l7_width);
        float y = l7_height - fmod(l7_animation_time*speed*l7_u + i*137*l7_u,l7_height+200*l7_u);
        float fade = 0.5 - 0.4*(1 - y/l7_height);
        if (fade<0) fade = 0;
        DrawCircle(x,y,(2+(i%3))*l7_u,Fade(l7_ember,fade));
    }
}


void l7_draw_rock(Rectangle rec)
{
    Rectangle source = {rec.x/l7_u*2,rec.y/l7_u*2,rec.width/l7_u*2,rec.height/l7_u*2};
    Vector2 no_origin = {0,0};
    DrawTexturePro(l7_rock_texture,source,rec,no_origin,0,WHITE);
}


void l7_draw_ground()
{
    l7_draw_lake();

    //the ledges: a hot halo, a molten rim, a dark lip, then the rock, then an inner
    //shadow. Five thin layers are what make them sit ON the lake instead of in it.
    for (int i=0; i<10; i++)
    {
        Rectangle r = l7_ledge[i];
        if (r.x<0) continue;
        DrawRectangle(r.x-18*l7_u,r.y-18*l7_u,r.width+36*l7_u,r.height+36*l7_u,Fade(l7_lava,0.16));
        DrawRectangle(r.x-9*l7_u,r.y-9*l7_u,r.width+18*l7_u,r.height+18*l7_u,Fade(l7_lava,0.5));
        DrawRectangle(r.x-4*l7_u,r.y-4*l7_u,r.width+8*l7_u,r.height+8*l7_u,l7_rock_dark);
        l7_draw_rock(r);
        DrawRectangleGradientV(r.x,r.y,r.width,26*l7_u,Fade(BLACK,0.45),BLANK);
        DrawRectangleGradientV(r.x,r.y+r.height-20*l7_u,r.width,20*l7_u,BLANK,Fade(BLACK,0.35));
        DrawRectangleLinesEx(r,2*l7_u,Fade(l7_rock_light,0.5));
    }

    //the planks
    for (int b=0; b<2; b++)
    {
        for (int i=0; i<l7_plank_count[b]; i++)
        {
            if (l7_plank_state[b][i]==2) continue;
            Rectangle r = l7_plank[b][i];
            float shake = 0;
            if (l7_plank_state[b][i]==1) shake = sin(l7_animation_time*70)*3*l7_u;
            Rectangle shown = {r.x+shake,r.y,r.width,r.height};
            //frame 1 is the sound grating, frame 2 the one that is already tearing open
            int frame = (l7_plank_state[b][i]==1) ? 1 : 0;
            Rectangle source = {frame*128,0,128,256};
            Vector2 no_plank_origin = {0,0};
            DrawTexturePro(l7_catwalk_texture,source,shown,no_plank_origin,0,WHITE);
            if (l7_plank_state[b][i]==1) DrawRectangleLinesEx(shown,2*l7_u,Fade(l7_ember,0.7));
        }
    }

    //cooling crust, grey or glowing, drawn from the two frames of the one picture
    for (int i=0; i<3; i++)
    {
        int frame = 0;
        if (l7_crust_clock[i]>=4) frame = 1;
        else if (l7_crust_clock[i]>=3.4 && fmod(l7_animation_time*10,2)<1) frame = 1;
        Rectangle source = {frame*512,0,512,512};
        Vector2 no_origin = {0,0};
        DrawTexturePro(l7_crust_texture,source,l7_crust[i],no_origin,0,WHITE);
    }

    //rafts
    for (int i=0; i<2; i++)
    {
        Rectangle source = {0,0,640,512};
        Vector2 no_origin = {0,0};
        DrawTexturePro(l7_raft_texture,source,l7_raft[i],no_origin,0,WHITE);
    }

    //the lava fall: a curtain pouring across the middle terrace, with a glow either side
    int fall_frame = (int)(l7_animation_time*10)%4;
    Rectangle fall_source = {fall_frame*256,0,256,512};
    Vector2 no_origin_fall = {0,0};
    if (l7_lavafall_clock<2.5)
    {
        DrawRectangle(l7_lavafall.x-16*l7_u,l7_lavafall.y,l7_lavafall.width+32*l7_u,l7_lavafall.height,Fade(l7_lava,0.3));
        DrawTexturePro(l7_lavafall_texture,fall_source,l7_lavafall,no_origin_fall,0,WHITE);
    }
    else
    {
        //dry: the channel it pours down, and a warning as it builds again
        DrawRectangleLinesEx(l7_lavafall,2*l7_u,Fade(l7_ember,0.45));
        if (l7_lavafall_clock>4.2 && fmod(l7_animation_time*8,2)<1.2)
            DrawRectangle(l7_lavafall.x,l7_lavafall.y,l7_lavafall.width,l7_lavafall.height,Fade(l7_lava,0.25));
    }
}


void l7_draw_obstacles()
{

    //steam jets: a vent grate in the rock, and the blast when it blows
    for (int i=0; i<2; i++)
    {
        Rectangle r = l7_steam_jet[i];
        //the grate itself, so you can see where it is even when it is quiet
        DrawRectangleRec(r,Fade(l7_rock_dark,0.9));
        DrawRectangleLinesEx(r,3*l7_u,Fade(l7_rock_light,0.8));
        int bars = r.width/(18*l7_u);
        for (int k=0; k<bars; k++)
        {
            DrawRectangle(r.x+6*l7_u+k*18*l7_u,r.y+6*l7_u,8*l7_u,r.height-12*l7_u,Fade(l7_rock,0.95));
        }
        if (l7_steam_clock[i]<1.4)
        {
            //one burst drawn three times along the way it blows, so the cloud reads as
            //going somewhere. The four frames are the same puffs expanding.
            float strength = 1 - l7_steam_clock[i]/1.4;
            Vector2 way = Vector2Normalize(l7_steam_push[i]);
            Vector2 middle = {r.x+r.width/2,r.y+r.height/2};
            for (int k=0; k<3; k++)
            {
                float t = fmod(l7_animation_time*1.4 + k*0.33,1.0);
                int frame = t*4;
                if (frame>3) frame = 3;
                Vector2 puff = Vector2Add(middle,Vector2Scale(way,t*210*l7_u));
                float size = (130 + t*150)*l7_u;
                Rectangle source = {frame*256,0,256,256};
                Rectangle dest = {puff.x,puff.y,size,size};
                Vector2 origin = {size/2,size/2};
                DrawTexturePro(l7_steam_texture,source,dest,origin,t*40+k*25,Fade(WHITE,0.9*strength*(1-t*0.7)));
            }
        }
        else if (l7_steam_clock[i]>3.4 && fmod(l7_animation_time*8,2)<1.2)
        {
            //about to blow
            DrawRectangleLinesEx(r,4*l7_u,Fade(l7_steam,0.8));
        }
    }

    //the lava tube: a pipe mouth in the rock at one end, and where it spits you out at
    //the other. The arrow says which way you leave.
    //frame 1 is the mouth that swallows, frame 2 the one that spits you back out. The
    //ring of stone is 112 of the sprite's 128 px, so this size puts the stonework around
    //the circle that actually catches the ball.
    float mouth = 36*l7_u*2*128/112;
    Rectangle tube_in_source = {0,0,256,256};
    Rectangle tube_in_dest = {l7_tube_in.x,l7_tube_in.y,mouth,mouth};
    Vector2 tube_origin = {mouth/2,mouth/2};
    //both turn, slowly, so the swirl reads as pulling in and throwing out
    DrawTexturePro(l7_tube_texture,tube_in_source,tube_in_dest,tube_origin,l7_animation_time*30,WHITE);

    float facing = l7_tube_facing*DEG2RAD;
    Vector2 nose = {l7_tube_out.x+cos(facing)*58*l7_u,l7_tube_out.y+sin(facing)*58*l7_u};
    Rectangle tube_out_source = {256,0,256,256};
    Rectangle tube_out_dest = {l7_tube_out.x,l7_tube_out.y,mouth,mouth};
    DrawTexturePro(l7_tube_texture,tube_out_source,tube_out_dest,tube_origin,-l7_animation_time*40,WHITE);
    DrawPoly(nose,3,18*l7_u,l7_tube_facing,Fade(l7_ember,0.9));
    DrawPolyLines(nose,3,18*l7_u,l7_tube_facing,l7_lava_hot);

    //vents: dormant, building, erupting - plus the circle of lava an eruption throws out
    for (int i=0; i<3; i++)
    {
        //the flood an eruption throws out: layered, with a crust edge and thrown blobs,
        //so it reads as spreading molten rock rather than a coloured circle
        float flood = l7_vent_flood(i);
        if (flood>0)
        {
            //the four frames are one pool at four sizes, 110, 170, 220 and 248 of their
            //256, so the frame comes from how far the flood has spread and is then scaled
            //so the painted crust edge sits on the edge that actually burns.
            float pool[4] = {110,170,220,248};
            float heat = flood/l7_vent_reach;
            int frame = heat*4;
            if (frame>3) frame = 3;
            if (frame<0) frame = 0;
            float size = flood*2*256/pool[frame];
            Rectangle source = {frame*512,0,512,512};
            Rectangle dest = {l7_vent[i].x,l7_vent[i].y,size,size};
            Vector2 origin = {size/2,size/2};
            DrawCircleGradient(l7_vent[i],flood*1.4,Fade(l7_lava,0.25*heat),BLANK);
            DrawTexturePro(l7_eruption_texture,source,dest,origin,i*37,WHITE);
        }
        int frame = 0;
        if (l7_vent_live[i]==1 && l7_vent_stage[i]<1.5) frame = 1;
        else if (l7_vent_live[i]==1) frame = 2;
        Rectangle source = {frame*512,0,512,512};
        //the sprite's hole is 300 of its 512 px, so this size makes the drawn hole match
        //the hole that actually swallows the ball
        float size = l7_vent_hole*512/300;
        Rectangle dest = {l7_vent[i].x,l7_vent[i].y,size,size};
        Vector2 origin = {size/2,size/2};
        DrawTexturePro(l7_vent_texture,source,dest,origin,0,WHITE);
        if (l7_vent_live[i]==1 && l7_vent_stage[i]<1.5 && fmod(l7_animation_time*8,2)<1.2)
        {
            DrawCircleLines(l7_vent[i].x,l7_vent[i].y,l7_vent_reach,Fade(l7_ember,0.5));
        }
    }

    //geysers
    for (int i=0; i<2; i++)
    {
        int frame = -1;
        if (l7_geyser_clock[i]<0.2) frame = 0;
        else if (l7_geyser_clock[i]<0.45) frame = 1;
        else if (l7_geyser_clock[i]<0.8) frame = 2;
        if (frame>=0)
        {
            Rectangle source = {frame*256,0,256,256};
            Rectangle dest = {l7_geyser[i].x,l7_geyser[i].y,150*l7_u,150*l7_u};
            Vector2 origin = {75*l7_u,75*l7_u};
            DrawTexturePro(l7_geyser_texture,source,dest,origin,0,WHITE);
        }
        else
        {
            DrawCircleV(l7_geyser[i],22*l7_u,l7_rock_dark);
            DrawRing(l7_geyser[i],16*l7_u,22*l7_u,0,360,20,Fade(l7_ember,0.6));
            //an arrow showing where it will throw the ball
            Vector2 way = Vector2Normalize(l7_geyser_kick[i]);
            DrawLineEx(l7_geyser[i],Vector2Add(l7_geyser[i],Vector2Scale(way,40*l7_u)),3*l7_u,Fade(l7_ember,0.45));
        }
    }

    //salamanders, running the ledges
    for (int i=0; i<3; i++)
    {
        int frame = (int)(l7_animation_time*12+i)%4;
        Rectangle source = {frame*128,0,128,128};
        float size = l7_salamander_radius*2.4;
        Rectangle dest = {l7_salamander[i].x,l7_salamander[i].y,size,size};
        Vector2 origin = {size/2,size/2};
        float facing = 90;
        if (l7_salamander_travel[i]>1) facing = -90;
        DrawTexturePro(l7_salamander_texture,source,dest,origin,facing,WHITE);
    }

    //crystals
    for (int i=0; i<4; i++)
    {
        int frame = i%3;
        Rectangle source = {frame*384,0,384,384};
        float size = l7_crystal_radius*2.3;
        if (l7_crystal_hit[i]>0) size = size + 8*l7_u*l7_crystal_hit[i]/0.2;
        Rectangle dest = {l7_crystal[i].x,l7_crystal[i].y,size,size};
        Vector2 origin = {size/2,size/2};
        DrawTexturePro(l7_crystal_texture,source,dest,origin,i*29,WHITE);
    }
}


void l7_draw_ball_and_pot()
{
    //the heart: shut while the vent is working, open afterwards
    //the basin: frame 1 is plugged, frame 2 is the hole you have to land in. The hole is
    //72 of the sprite's 256 px, so this size puts it over the hole that counts.
    int heart_frame = (l7_heart_open>0) ? 1 : 0;
    float basin = l7_radius_pot*2*256/72;
    Rectangle heart_source = {heart_frame*512,0,512,512};
    Rectangle heart_dest = {l7_pot.x,l7_pot.y,basin,basin};
    Vector2 heart_origin = {basin/2,basin/2};
    //a beacon, the same idea as the altar in the temple: a halo, a turning ring and a
    //flag on a pole, so the end of the course is obvious from the other side of the screen
    float t = l7_animation_time;
    DrawCircleGradient(l7_pot,basin*2.6,Fade(l7_lava,0.30+0.10*sin(t*2)),BLANK);
    DrawCircleGradient(l7_pot,basin*1.3,Fade(l7_ember,0.25+0.08*sin(t*2)),BLANK);
    DrawTexturePro(l7_heart_texture,heart_source,heart_dest,heart_origin,0,WHITE);
    DrawRing(l7_pot,basin*0.62,basin*0.68,t*40,t*40+300,48,Fade(l7_ember,0.8));
    for (int j=0; j<6; j++)
    {
        Vector2 arm = {basin*0.8,0};
        Vector2 sparkle = Vector2Add(l7_pot,Vector2Rotate(arm,(t*50+j*60)*DEG2RAD));
        DrawCircleV(sparkle,(2+sin(t*5+j))*l7_u,l7_lava_hot);
    }

    //the flag
    Vector2 pole_top = {l7_pot.x,l7_pot.y-48*l7_u};
    DrawLineEx(l7_pot,pole_top,3*l7_u,l7_rock_light);
    for (int i=0; i<5; i++)
    {
        float wave = sin(t*6 - i*0.8)*2.5*l7_u;
        DrawRectangle(l7_pot.x+1*l7_u+i*6*l7_u,pole_top.y+wave,6*l7_u,18*l7_u,l7_lava);
    }

    DrawCircleV(l7_ball,l7_radius_ball,WHITE);
    DrawCircleLines(l7_ball.x,l7_ball.y,l7_radius_ball,GRAY);

    if (l7_aiming==1 && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        Vector2 mouse = {GetMouseX(),GetMouseY()};
        DrawLineEx(l7_ball,mouse,4*l7_u,Fade(WHITE,0.75));
        DrawCircle(mouse.x,mouse.y,5*l7_u,Fade(WHITE,0.75));
    }
}


//the lava that has already risen, drawn over everything below its line
void l7_draw_rising_lava()
{
    if (l7_lava_line>=l7_height) return;
    int frame = (int)(l7_animation_time*2)%2;
    Rectangle source = {frame*512,0,512,512};
    Vector2 no_origin = {0,0};
    //tiled at the same size as the lake in the background, so the risen lava reads as the
    //same stuff and not as a pattern laid over the level
    int columns = l7_width/(170*l7_u) + 1;
    float band = l7_height - l7_lava_line;
    int rows = band/(170*l7_u) + 1;
    for (int x=0; x<columns; x++)
    {
        for (int y=0; y<rows; y++)
        {
            Rectangle dest = {x*170*l7_u,l7_lava_line + y*170*l7_u,170*l7_u,170*l7_u};
            DrawTexturePro(l7_lava_texture,source,dest,no_origin,0,WHITE);
        }
    }
    //a wash over it, and it gets darker the deeper down you look
    DrawRectangle(0,l7_lava_line,l7_width,band,Fade(GetColor(0x6A2A10FF),0.34));
    DrawRectangleGradientV(0,l7_lava_line+40*l7_u,l7_width,band,BLANK,Fade(BLACK,0.45));
    //the edge of it, with a bright lip and a haze above
    DrawRectangle(0,l7_lava_line-6*l7_u,l7_width,8*l7_u,l7_lava_hot);
    DrawRectangleGradientV(0,l7_lava_line-70*l7_u,l7_width,70*l7_u,BLANK,Fade(l7_lava,0.55));
    for (int k=0; k<26; k++)
    {
        float x = fmod(k*197*l7_u + l7_animation_time*40*l7_u,l7_width);
        float lift = fmod(l7_animation_time*60*l7_u + k*37*l7_u,120*l7_u);
        DrawCircle(x,l7_lava_line-lift,(5-lift/(40*l7_u))*l7_u+2*l7_u,Fade(l7_ember,0.5-lift/(260*l7_u)));
    }
}


void l7_draw_shower()
{
    if (l7_shower_state==1 && fmod(l7_animation_time*7,2)<1.3)
    {
        DrawRectangle(l7_shower_x-120*l7_u,l7_hud_height,240*l7_u,l7_height,Fade(l7_ember,0.15));
        DrawText("ROCKFALL",l7_shower_x-MeasureText("ROCKFALL",40*l7_u)/2,l7_hud_height+20*l7_u,40*l7_u,Fade(l7_lava_hot,0.9));
    }
    if (l7_shower_state==2)
    {
        for (int k=0; k<16; k++)
        {
            float speed = 500 + (k%5)*120;
            float y = fmod(l7_shower_timer*speed*l7_u + k*160*l7_u,l7_height+200*l7_u) - 100*l7_u;
            float x = l7_shower_x + sin(k*2.3)*110*l7_u;
            DrawCircleV((Vector2){x,y},(14+(k%4)*6)*l7_u,l7_rock);
            DrawCircleLines(x,y,(14+(k%4)*6)*l7_u,l7_rock_dark);
        }
    }
}


void l7_draw_hud()
{
    DrawRectangleGradientH(0,l7_hud_height,90*l7_u,l7_height-l7_hud_height,Fade(BLACK,0.3),BLANK);
    DrawRectangleGradientH(l7_width-90*l7_u,l7_hud_height,90*l7_u,l7_height-l7_hud_height,BLANK,Fade(BLACK,0.3));

    DrawRectangleGradientV(0,0,l7_width,l7_hud_height,l7_rock_light,l7_rock_dark);
    DrawRectangle(0,l7_hud_height-4*l7_u,l7_width,4*l7_u,l7_lava);

    DrawText("LEVEL 7 - MAGMA CORE",32*l7_u,20*l7_u,36*l7_u,BLACK);
    DrawText("LEVEL 7 - MAGMA CORE",30*l7_u,18*l7_u,36*l7_u,l7_ember);
    DrawText(TextFormat("STROKES %d / %d",l7_stroke,l7_stroke_limit),l7_width/2-298*l7_u,21*l7_u,32*l7_u,BLACK);
    DrawText(TextFormat("STROKES %d / %d",l7_stroke,l7_stroke_limit),l7_width/2-300*l7_u,19*l7_u,32*l7_u,WHITE);
    for (int i=0; i<l7_stroke_limit; i++)
    {
        Color pip = l7_rock_dark;
        if (i<l7_stroke) pip = l7_lava;
        DrawCircle(l7_width/2+10*l7_u+i*22*l7_u,35*l7_u,7*l7_u,pip);
        DrawCircleLines(l7_width/2+10*l7_u+i*22*l7_u,35*l7_u,7*l7_u,BLACK);
    }
    int help_width = MeasureText("R restart   ESC menu",26*l7_u);
    DrawText("R restart   ESC menu",l7_width-help_width-28*l7_u,24*l7_u,26*l7_u,BLACK);
    DrawText("R restart   ESC menu",l7_width-help_width-30*l7_u,22*l7_u,26*l7_u,WHITE);

    //how far the lava has come up, as a bar down the left edge
    float climbed = (l7_height-l7_lava_line)/(l7_height-140*l7_u);
    DrawRectangle(20*l7_u,l7_hud_height+20*l7_u,16*l7_u,300*l7_u,Fade(BLACK,0.5));
    DrawRectangle(20*l7_u,l7_hud_height+20*l7_u+300*l7_u*(1-climbed),16*l7_u,300*l7_u*climbed,l7_lava);
    DrawRectangleLines(20*l7_u,l7_hud_height+20*l7_u,16*l7_u,300*l7_u,Fade(l7_ember,0.8));
    DrawText("LAVA",14*l7_u,l7_hud_height+330*l7_u,20*l7_u,Fade(l7_ember,0.9));

    //whether the heart is open right now
    const char *heart = "THE HEART IS OPEN";
    Color heart_colour = l7_lava_hot;
    if (l7_heart_open<=0)
    {
        heart = "THE HEART IS FLOODED";
        heart_colour = Fade(l7_lava,0.9);
    }
    DrawText(heart,l7_width-MeasureText(heart,24*l7_u)-30*l7_u,l7_hud_height+20*l7_u,24*l7_u,heart_colour);

    if (l7_message_timer>0)
    {
        const char *say = "MELTED!";
        if (l7_message_type==2) say = "DOWN THE VENT!";
        if (l7_message_type==3) say = "TOO HOT!";
        if (l7_message_type==4) say = "ROCKFALL!";
        DrawText(say,l7_width/2-MeasureText(say,90*l7_u)/2+4*l7_u,l7_height/2-41*l7_u,90*l7_u,Fade(BLACK,l7_message_timer));
        DrawText(say,l7_width/2-MeasureText(say,90*l7_u)/2,l7_height/2-45*l7_u,90*l7_u,Fade(l7_lava_hot,l7_message_timer));
    }

    if (l7_game_state!=0)
    {
        DrawRectangle(0,0,l7_width,l7_height,Fade(BLACK,0.6));
        Rectangle panel = {l7_width/2-340*l7_u,l7_height/2-170*l7_u,680*l7_u,340*l7_u};
        DrawRectangleRec(panel,l7_rock_dark);
        DrawRectangle(panel.x,panel.y,panel.width,24*l7_u,l7_lava);
        DrawRectangleLinesEx(panel,6*l7_u,l7_ember);
        if (l7_game_state==1) DrawText("LEVEL CLEAR!",l7_width/2-MeasureText("LEVEL CLEAR!",80*l7_u)/2,panel.y+60*l7_u,80*l7_u,l7_ember);
        else DrawText("FAILED",l7_width/2-MeasureText("FAILED",80*l7_u)/2,panel.y+60*l7_u,80*l7_u,l7_lava);
        int strokes_width = MeasureText(TextFormat("Strokes: %d / %d",l7_stroke,l7_stroke_limit),40*l7_u);
        DrawText(TextFormat("Strokes: %d / %d",l7_stroke,l7_stroke_limit),l7_width/2-strokes_width/2,panel.y+170*l7_u,40*l7_u,WHITE);
        DrawText("R play again   ESC menu",l7_width/2-MeasureText("R play again   ESC menu",30*l7_u)/2,panel.y+250*l7_u,30*l7_u,l7_ember);
    }
}


//level 7: keep the volcano moving with no ball (intro and menu)
void l7_background_step(float dt)
{
    l7_update_obstacles(dt);
    //the menu must not flood itself over a few minutes, so the lava is held down there
    if (l7_lava_line < 500*l7_u) l7_lava_line = 1080*l7_u;
}


//level 7: draw everything except the scoreboard (intro and menu)
void l7_draw_scene()
{
    l7_draw_ground();
    l7_draw_obstacles();
    l7_draw_ball_and_pot();
    l7_draw_rising_lava();
    l7_draw_shower();
}


void l7_start(int screen_width, int screen_height)
{
    l7_width = screen_width;
    l7_height = screen_height;

    l7_rock_texture = LoadTexture("assets/magma/magma_rock_tile.png");
    l7_lava_texture = LoadTexture("assets/magma/magma_lava_tile.png");
    l7_crust_texture = LoadTexture("assets/magma/magma_crust.png");
    l7_vent_texture = LoadTexture("assets/magma/magma_vent.png");
    l7_eruption_texture = LoadTexture("assets/magma/magma_eruption.png");
    l7_steam_texture = LoadTexture("assets/magma/magma_steam.png");
    l7_catwalk_texture = LoadTexture("assets/magma/magma_catwalk.png");
    l7_tube_texture = LoadTexture("assets/magma/magma_tube.png");
    l7_heart_texture = LoadTexture("assets/magma/magma_heart.png");
    l7_geyser_texture = LoadTexture("assets/magma/magma_geyser.png");
    l7_crystal_texture = LoadTexture("assets/magma/magma_crystal.png");
    l7_raft_texture = LoadTexture("assets/magma/magma_raft.png");
    l7_salamander_texture = LoadTexture("assets/magma/magma_salamander.png");
    l7_lavafall_texture = LoadTexture("assets/magma/magma_lavafall.png");
    SetTextureWrap(l7_rock_texture,TEXTURE_WRAP_REPEAT);
    SetTextureWrap(l7_lava_texture,TEXTURE_WRAP_REPEAT);
    SetTextureFilter(l7_rock_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l7_lava_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l7_crust_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l7_vent_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l7_eruption_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l7_steam_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l7_catwalk_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l7_tube_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l7_heart_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l7_geyser_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l7_crystal_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l7_raft_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l7_salamander_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l7_lavafall_texture,TEXTURE_FILTER_BILINEAR);

    l7_u = l7_height/1080.0;
    if (l7_width/1920.0 < l7_u) l7_u = l7_width/1920.0;
    l7_reset_level();
}


void l7_update()
{
    float dt = GetFrameTime();
    if (dt>1.0/30) dt = 1.0/30;
    if (l7_message_timer>0) l7_message_timer = l7_message_timer - dt;

    if (IsKeyPressed(KEY_R)) l7_reset_level();

    int ball_stopped = 0;
    if (l7_speed.x==0 && l7_speed.y==0) ball_stopped = 1;
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && ball_stopped==1 && l7_game_state==0) l7_aiming = 1;
    if (l7_game_state!=0) l7_aiming = 0;
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && l7_aiming==1)
    {
        l7_aiming = 0;
        Vector2 mouse = {GetMouseX(),GetMouseY()};
        Vector2 drag = Vector2Subtract(l7_ball,mouse);
        if (ball_stopped==1 && l7_stroke<l7_stroke_limit && Vector2Length(drag)>=2*l7_radius_ball)
        {
            l7_last_shot_position = l7_ball;
            l7_speed = Vector2ClampValue(Vector2Scale(drag,3.125),0,l7_max_speed*l7_u);
            l7_stroke++;
        }
    }

    for (int i=0; i<4; i++)
    {
        l7_update_obstacles(dt/4*obstacle_speed);
        if (l7_game_state==0) l7_update_ball(dt/4);
    }

    if (l7_game_state==0 && l7_stroke>=l7_stroke_limit && l7_speed.x==0 && l7_speed.y==0) l7_game_state = 2;
}


void l7_draw()
{
    l7_draw_ground();
    l7_draw_obstacles();
    l7_draw_ball_and_pot();
    l7_draw_rising_lava();
    draw_straight_preview(l7_ball,l7_aiming,l7_radius_ball,l7_max_speed*l7_u,l7_u);
    l7_draw_shower();
    l7_draw_hud();
}


void l7_unload()
{
    UnloadTexture(l7_rock_texture);
    UnloadTexture(l7_lava_texture);
    UnloadTexture(l7_crust_texture);
    UnloadTexture(l7_vent_texture);
    UnloadTexture(l7_eruption_texture);
    UnloadTexture(l7_steam_texture);
    UnloadTexture(l7_catwalk_texture);
    UnloadTexture(l7_tube_texture);
    UnloadTexture(l7_heart_texture);
    UnloadTexture(l7_geyser_texture);
    UnloadTexture(l7_crystal_texture);
    UnloadTexture(l7_raft_texture);
    UnloadTexture(l7_salamander_texture);
    UnloadTexture(l7_lavafall_texture);
}
