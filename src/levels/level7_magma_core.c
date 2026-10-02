//==================== LEVEL 7 ====================
//Magma Core: a descent into an erupting volcano. The lava climbs the whole time, so the
//bottom of the course stops existing, and the hole only opens for a few seconds after an
//eruption drains away. The one level you cannot take your time over.

//full global
int l7_width = 1920;
int l7_height = 1080;
float l7_u = 1;
int l7_stroke_base = 18;
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
Texture2D l7_geyser_texture;
Texture2D l7_crystal_texture;
Texture2D l7_raft_texture;
Texture2D l7_bubble_texture;
Texture2D l7_salamander_texture;
Texture2D l7_smoke_texture;
Texture2D l7_pumice_texture;
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
Vector2 l7_bubble[7];
float l7_bubble_stage[7];
float l7_bubble_wait[7];

//salamanders skitter along the ledges: harmless, but always moving
Vector2 l7_salamander[3];
Vector2 l7_salamander_from[3];
Vector2 l7_salamander_to[3];
float l7_salamander_travel[3];
float l7_salamander_speed[3];

//smoke plumes drifting up off the hot spots
Vector2 l7_smoke[4];
float l7_smoke_stage[4];

//pumice bobbing on the lake, and a lava fall pouring down one wall
Vector2 l7_pumice[4];
float l7_pumice_bob[4];
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

    //THE ROUTE IS A DESCENT, and it reads at a glance: along the top terrace to the
    //right, down the right-hand bridge, back along the middle terrace, down the
    //left-hand bridge, then right along the floor to the heart. The difficulty is
    //timing the vents, the crust and the lava fall, not working out where to go.
    l7_ledge[0] = l7_make_rect(60,150,1800,180);    //top terrace
    l7_ledge[1] = l7_make_rect(1560,330,300,150);   //right landing
    l7_ledge[2] = l7_make_rect(60,480,1800,180);    //middle terrace
    l7_ledge[3] = l7_make_rect(60,660,320,150);     //left landing
    l7_ledge[4] = l7_make_rect(60,810,1800,190);    //the floor, where the heart is
    for (int i=5; i<10; i++) l7_ledge[i] = l7_make_rect(-400,-400,10,10);

    //the lava climbs the screen and stops just under the floor, so you can see it
    //coming for you without the level becoming unwinnable
    l7_lava_line = 1080*l7_u;
    l7_lava_rise = 2.0*l7_u;

    //cooling crust: safe while grey, deadly while it glows. One per terrace, right on
    //the line you want to take.
    l7_crust[0] = l7_make_rect(620,170,150,140);
    l7_crust[1] = l7_make_rect(980,500,150,140);
    l7_crust[2] = l7_make_rect(760,830,150,140);
    l7_crust_clock[0] = 0;
    l7_crust_clock[1] = 2.2;
    l7_crust_clock[2] = 4;

    //the two bridges down, both crumbling
    for (int i=0; i<6; i++) l7_plank[0][i] = l7_make_rect(1596+i*46,330,46,160);
    for (int i=0; i<7; i++) l7_plank[1][i] = l7_make_rect(96+i*41,660,41,160);
    for (int b=0; b<2; b++)
    {
        for (int i=0; i<7; i++)
        {
            l7_plank_state[b][i] = 0;
            l7_plank_timer[b][i] = 0;
        }
    }

    //vents: the holes are always deadly, and now and then they flood a circle
    l7_vent_hole = 34*l7_u;
    l7_vent_reach = 140*l7_u;
    l7_vent[0] = l7_make_point(1080,240);
    l7_vent[1] = l7_make_point(560,570);
    l7_vent[2] = l7_make_point(1300,975);
    for (int i=0; i<3; i++)
    {
        l7_vent_wait[i] = 7 + i*4.5;
        l7_vent_stage[i] = 0;
        l7_vent_live[i] = 0;
    }

    //geysers throw you along the terrace, which is how you cover the long legs fast
    l7_geyser[0] = l7_make_point(340,240);
    l7_geyser_kick[0] = l7_make_point(560,0);
    l7_geyser[1] = l7_make_point(1520,570);
    l7_geyser_kick[1] = l7_make_point(-560,0);
    l7_geyser_clock[0] = 0;
    l7_geyser_clock[1] = 1.6;

    //rafts drift in the lava between the terraces: pure scenery, nothing to land on
    l7_raft[0] = l7_make_rect(500,350,170,120);
    l7_raft_from[0] = 420*l7_u;
    l7_raft_to[0] = 900*l7_u;
    l7_raft_speed[0] = 90*l7_u;
    l7_raft[1] = l7_make_rect(1100,690,170,120);
    l7_raft_from[1] = 900*l7_u;
    l7_raft_to[1] = 1400*l7_u;
    l7_raft_speed[1] = -80*l7_u;

    //the lava tube: a shortcut from the middle terrace down to the floor
    l7_tube_in = l7_make_point(1760,560);
    l7_tube_out = l7_make_point(1600,900);
    l7_tube_facing = 180;
    l7_tube_cooldown = 0;

    //steam blows sideways across two spots
    l7_steam_jet[0] = l7_make_rect(880,150,150,180);
    l7_steam_push[0] = l7_make_point(0,420);
    l7_steam_jet[1] = l7_make_rect(420,810,150,190);
    l7_steam_push[1] = l7_make_point(420,0);
    l7_steam_clock[0] = 0;
    l7_steam_clock[1] = 1.8;

    //crystals: bumpers along the terraces
    l7_crystal_radius = 30*l7_u;
    l7_crystal[0] = l7_make_point(1420,190);
    l7_crystal[1] = l7_make_point(260,570);
    l7_crystal[2] = l7_make_point(1180,570);
    l7_crystal[3] = l7_make_point(1040,900);
    for (int i=0; i<4; i++) l7_crystal_hit[i] = 0;

    l7_shower_wait = 22;
    l7_shower_state = 0;
    l7_shower_timer = 0;
    l7_shower_x = 0;
    l7_heart_open = 0;

    //bubbles pop out in the lava, well away from the terraces
    float bubble_spot[7][2] = {{300,400},{780,390},{1250,420},{520,720},{880,730},{1500,720},{1700,420}};
    for (int i=0; i<7; i++)
    {
        l7_bubble[i] = l7_make_point(bubble_spot[i][0],bubble_spot[i][1]);
        l7_bubble_stage[i] = 0;
        l7_bubble_wait[i] = 0.4 + i*0.55;
    }

    //salamanders run the terraces
    l7_salamander_from[0] = l7_make_point(200,300);
    l7_salamander_to[0] = l7_make_point(700,300);
    l7_salamander_speed[0] = 0.3;
    l7_salamander_from[1] = l7_make_point(700,630);
    l7_salamander_to[1] = l7_make_point(1300,630);
    l7_salamander_speed[1] = 0.26;
    l7_salamander_from[2] = l7_make_point(1200,970);
    l7_salamander_to[2] = l7_make_point(1800,970);
    l7_salamander_speed[2] = 0.33;
    for (int i=0; i<3; i++)
    {
        l7_salamander_travel[i] = i*0.4;
        l7_salamander[i] = l7_salamander_from[i];
    }

    //smoke drifts up off the hot spots
    l7_smoke[0] = l7_make_point(420,400);
    l7_smoke[1] = l7_make_point(1150,400);
    l7_smoke[2] = l7_make_point(700,730);
    l7_smoke[3] = l7_make_point(1560,730);
    for (int i=0; i<4; i++) l7_smoke_stage[i] = i*0.9;

    //pumice bobbing out in the lake
    l7_pumice[0] = l7_make_point(980,390);
    l7_pumice[1] = l7_make_point(260,730);
    l7_pumice[2] = l7_make_point(1420,390);
    l7_pumice[3] = l7_make_point(1700,730);
    for (int i=0; i<4; i++) l7_pumice_bob[i] = i*1.3;

    //the lava fall pours straight across the middle terrace: a curtain to time
    l7_lavafall = l7_make_rect(740,480,70,180);
    l7_lavafall_clock = 0;

    //ball and heart
    l7_start_position = l7_make_point(150,240);
    l7_ball = l7_start_position;
    l7_last_shot_position = l7_start_position;
    l7_speed.x = 0;
    l7_speed.y = 0;
    l7_pot = l7_make_point(1740,900);
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
    if (l7_lava_line < 800*l7_u) l7_lava_line = 800*l7_u;   //it stops below the heart: the level has to stay winnable

    //the lava fall pours, then stops, then pours again
    l7_lavafall_clock = l7_lavafall_clock + dt;
    if (l7_lavafall_clock>=5) l7_lavafall_clock = l7_lavafall_clock - 5;

    //bubbles: a wait, then swell and pop
    for (int i=0; i<7; i++)
    {
        if (l7_bubble_stage[i]>0)
        {
            l7_bubble_stage[i] = l7_bubble_stage[i] + dt;
            if (l7_bubble_stage[i]>1.2)
            {
                l7_bubble_stage[i] = 0;
                l7_bubble_wait[i] = 1.4 + GetRandomValue(0,30)/10.0;
            }
        }
        else
        {
            l7_bubble_wait[i] = l7_bubble_wait[i] - dt;
            if (l7_bubble_wait[i]<=0) l7_bubble_stage[i] = 0.0001;
        }
    }

    //salamanders
    for (int i=0; i<3; i++)
    {
        l7_salamander_travel[i] = l7_salamander_travel[i] + dt*l7_salamander_speed[i];
        if (l7_salamander_travel[i]>2) l7_salamander_travel[i] = l7_salamander_travel[i] - 2;
        float t = l7_salamander_travel[i];
        if (t>1) t = 2 - t;
        l7_salamander[i] = Vector2Lerp(l7_salamander_from[i],l7_salamander_to[i],t);
    }

    //smoke rises, thins, and starts again
    for (int i=0; i<4; i++)
    {
        l7_smoke_stage[i] = l7_smoke_stage[i] + dt*0.55;
        if (l7_smoke_stage[i]>3) l7_smoke_stage[i] = 0;
    }

    //pumice bobs
    for (int i=0; i<4; i++) l7_pumice_bob[i] = l7_pumice_bob[i] + dt*1.4;

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
            Rectangle shown = {r.x+shake,r.y,r.width-4*l7_u,r.height};
            DrawRectangleRec(shown,l7_rock);
            DrawRectangleLinesEx(shown,2*l7_u,l7_rock_dark);
            if (l7_plank_state[b][i]==1)
            {
                DrawLineEx((Vector2){shown.x+shown.width/2,shown.y},(Vector2){shown.x+shown.width/3,shown.y+shown.height},3*l7_u,Fade(l7_lava,0.9));
            }
        }
    }

    //cooling crust, grey or glowing, drawn from the two frames of the one picture
    for (int i=0; i<3; i++)
    {
        int frame = 0;
        if (l7_crust_clock[i]>=4) frame = 1;
        else if (l7_crust_clock[i]>=3.4 && fmod(l7_animation_time*10,2)<1) frame = 1;
        Rectangle source = {frame*256,0,256,256};
        Vector2 no_origin = {0,0};
        DrawTexturePro(l7_crust_texture,source,l7_crust[i],no_origin,0,WHITE);
    }

    //rafts
    for (int i=0; i<2; i++)
    {
        Rectangle source = {0,0,320,256};
        Vector2 no_origin = {0,0};
        DrawTexturePro(l7_raft_texture,source,l7_raft[i],no_origin,0,WHITE);
    }

    //pumice bobbing on the lake
    for (int i=0; i<4; i++)
    {
        float bob = sin(l7_pumice_bob[i])*6*l7_u;
        Rectangle source = {(i%3)*256,0,256,256};
        Rectangle dest = {l7_pumice[i].x,l7_pumice[i].y+bob,90*l7_u,90*l7_u};
        Vector2 origin = {45*l7_u,45*l7_u};
        DrawTexturePro(l7_pumice_texture,source,dest,origin,i*31,WHITE);
    }

    //bubbles swelling and popping all over the lava
    for (int i=0; i<7; i++)
    {
        if (l7_bubble_stage[i]<=0) continue;
        int frame = (int)(l7_bubble_stage[i]/1.2*4);
        if (frame>3) frame = 3;
        Rectangle source = {frame*256,0,256,256};
        Rectangle dest = {l7_bubble[i].x,l7_bubble[i].y,120*l7_u,120*l7_u};
        Vector2 origin = {60*l7_u,60*l7_u};
        DrawTexturePro(l7_bubble_texture,source,dest,origin,0,WHITE);
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

    //steam jets
    for (int i=0; i<2; i++)
    {
        if (l7_steam_clock[i]<1.4)
        {
            float strength = 1 - l7_steam_clock[i]/1.4;
            Rectangle r = l7_steam_jet[i];
            DrawRectangleRec(r,Fade(l7_steam,0.25*strength));
            for (int k=0; k<7; k++)
            {
                float t = fmod(l7_animation_time*1.6 + k*0.14,1.0);
                Vector2 puff = {r.x+r.width/2 + l7_steam_push[i].x*t*0.0016*l7_u*120,r.y+r.height/2 + l7_steam_push[i].y*t*0.0016*l7_u*120};
                DrawCircleV(puff,(10+k*3)*l7_u,Fade(l7_steam,0.3*strength*(1-t)));
            }
        }
        DrawRectangleLinesEx(l7_steam_jet[i],2*l7_u,Fade(l7_steam,0.35));
    }

    //the lava tube, in and out
    DrawCircleV(l7_tube_in,36*l7_u,l7_rock_dark);
    DrawRing(l7_tube_in,30*l7_u,36*l7_u,l7_animation_time*90,l7_animation_time*90+280,24,l7_lava);
    DrawCircleV(l7_tube_out,30*l7_u,l7_rock_dark);
    float facing = l7_tube_facing*DEG2RAD;
    Vector2 nose = {l7_tube_out.x+cos(facing)*52*l7_u,l7_tube_out.y+sin(facing)*52*l7_u};
    DrawRing(l7_tube_out,26*l7_u,32*l7_u,-l7_animation_time*70,-l7_animation_time*70+240,24,Fade(l7_ember,0.8));
    DrawPoly(nose,3,14*l7_u,l7_tube_facing,Fade(l7_ember,0.85));

    //vents: dormant, building, erupting - plus the circle of lava an eruption throws out
    for (int i=0; i<3; i++)
    {
        float flood = l7_vent_flood(i);
        if (flood>0)
        {
            DrawCircleV(l7_vent[i],flood,Fade(l7_lava,0.75));
            DrawCircleLines(l7_vent[i].x,l7_vent[i].y,flood,l7_lava_hot);
        }
        int frame = 0;
        if (l7_vent_live[i]==1 && l7_vent_stage[i]<1.5) frame = 1;
        else if (l7_vent_live[i]==1) frame = 2;
        Rectangle source = {frame*256,0,256,256};
        Rectangle dest = {l7_vent[i].x,l7_vent[i].y,120*l7_u,120*l7_u};
        Vector2 origin = {60*l7_u,60*l7_u};
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
            Rectangle source = {frame*128,0,128,128};
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
        Rectangle dest = {l7_salamander[i].x,l7_salamander[i].y,62*l7_u,62*l7_u};
        Vector2 origin = {31*l7_u,31*l7_u};
        float facing = 90;
        if (l7_salamander_travel[i]>1) facing = -90;
        DrawTexturePro(l7_salamander_texture,source,dest,origin,facing,WHITE);
    }

    //smoke drifting off the hot spots
    for (int i=0; i<4; i++)
    {
        int frame = (int)(l7_smoke_stage[i]/3*4);
        if (frame>3) frame = 3;
        float lift = l7_smoke_stage[i]*40*l7_u;
        Rectangle source = {frame*256,0,256,256};
        Rectangle dest = {l7_smoke[i].x+lift*0.3,l7_smoke[i].y-lift,220*l7_u,220*l7_u};
        Vector2 origin = {110*l7_u,110*l7_u};
        DrawTexturePro(l7_smoke_texture,source,dest,origin,0,Fade(WHITE,0.7));
    }

    //crystals
    for (int i=0; i<4; i++)
    {
        int frame = i%3;
        Rectangle source = {frame*192,0,192,192};
        float size = 86*l7_u;
        if (l7_crystal_hit[i]>0) size = size + 10*l7_u*l7_crystal_hit[i]/0.2;
        Rectangle dest = {l7_crystal[i].x,l7_crystal[i].y,size,size};
        Vector2 origin = {size/2,size/2};
        DrawTexturePro(l7_crystal_texture,source,dest,origin,i*29,WHITE);
    }
}


void l7_draw_ball_and_pot()
{
    //the heart: shut while the vent is working, open afterwards
    if (l7_heart_open>0)
    {
        DrawCircleV(l7_pot,l7_radius_pot+14*l7_u,Fade(l7_lava_hot,0.3));
        DrawCircleV(l7_pot,l7_radius_pot+3*l7_u,Fade(BLACK,0.7));
        DrawCircleV(l7_pot,l7_radius_pot,BLACK);
        DrawCircleLines(l7_pot.x,l7_pot.y,l7_radius_pot+6*l7_u,l7_lava_hot);
    }
    else
    {
        DrawCircleV(l7_pot,l7_radius_pot+10*l7_u,Fade(l7_lava,0.85));
        DrawCircleV(l7_pot,l7_radius_pot,l7_lava_hot);
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
    int columns = l7_width/(256*l7_u) + 1;
    float band = l7_height - l7_lava_line;
    int rows = band/(256*l7_u) + 1;
    for (int x=0; x<columns; x++)
    {
        for (int y=0; y<rows; y++)
        {
            Rectangle dest = {x*256*l7_u,l7_lava_line + y*256*l7_u,256*l7_u,256*l7_u};
            DrawTexturePro(l7_lava_texture,source,dest,no_origin,0,WHITE);
        }
    }
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
    if (l7_lava_line < 900*l7_u) l7_lava_line = 1080*l7_u;
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
    l7_geyser_texture = LoadTexture("assets/magma/magma_geyser.png");
    l7_crystal_texture = LoadTexture("assets/magma/magma_crystal.png");
    l7_raft_texture = LoadTexture("assets/magma/magma_raft.png");
    l7_bubble_texture = LoadTexture("assets/magma/magma_bubble.png");
    l7_salamander_texture = LoadTexture("assets/magma/magma_salamander.png");
    l7_smoke_texture = LoadTexture("assets/magma/magma_smoke.png");
    l7_pumice_texture = LoadTexture("assets/magma/magma_pumice.png");
    l7_lavafall_texture = LoadTexture("assets/magma/magma_lavafall.png");
    SetTextureWrap(l7_rock_texture,TEXTURE_WRAP_REPEAT);
    SetTextureWrap(l7_lava_texture,TEXTURE_WRAP_REPEAT);
    SetTextureFilter(l7_rock_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l7_lava_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l7_crust_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l7_vent_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l7_geyser_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l7_crystal_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l7_raft_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l7_bubble_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l7_salamander_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l7_smoke_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l7_pumice_texture,TEXTURE_FILTER_BILINEAR);
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
    UnloadTexture(l7_geyser_texture);
    UnloadTexture(l7_crystal_texture);
    UnloadTexture(l7_raft_texture);
    UnloadTexture(l7_bubble_texture);
    UnloadTexture(l7_salamander_texture);
    UnloadTexture(l7_smoke_texture);
    UnloadTexture(l7_pumice_texture);
    UnloadTexture(l7_lavafall_texture);
}
