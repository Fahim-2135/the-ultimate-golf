//==================== LEVEL 4 ====================

//full global
int l4_width = 1920;
int l4_height = 1080;
float l4_u = 1;
int l4_stroke_base = 18;
int l4_stroke_limit = 18;
#define l4_max_speed 650

//colours
Color l4_brick = {139,58,43,255};
Color l4_brick_dark = {110,44,32,255};
Color l4_mortar = {58,51,48,255};
Color l4_steel = {90,95,102,255};
Color l4_steel_light = {138,144,153,255};
Color l4_steel_dark = {46,49,54,255};
Color l4_floor_colour = {74,74,72,255};
Color l4_hazard_yellow = {232,185,35,255};
Color l4_rust = {181,84,28,255};
Color l4_rust_dark = {107,46,14,255};
Color l4_molten_orange = {255,106,0,255};
Color l4_molten_yellow = {255,208,0,255};
Color l4_molten_dark = {139,26,0,255};
Color l4_laser_red = {255,32,48,255};
Color l4_magnet_red = {192,40,45,255};
Color l4_stone = {120,126,112,255};
Color l4_stone_dark = {84,90,78,255};
Color l4_stone_mortar = {52,58,48,255};
Color l4_moss_green = {94,127,58,255};
Color l4_jungle_dark = {24,60,32,255};
Color l4_gold = {232,184,64,255};
Color l4_gold_light = {255,224,130,255};
Color l4_river_tint = {150,215,170,255};
Color l4_wood_brown = {120,80,40,255};

//pictures
Texture2D l4_ground_texture;
Texture2D l4_temple_texture;
Texture2D l4_foliage_texture;
Texture2D l4_plank_texture;
Texture2D l4_water_texture;
Texture2D l4_foam_texture;
Texture2D l4_splash_texture;
Texture2D l4_boulder_texture;

//l4_ball and l4_pot
Vector2 l4_ball;
Vector2 l4_speed;
float l4_radius_ball;
Vector2 l4_pot;
float l4_radius_pot;
Vector2 l4_start_position;
Vector2 l4_last_shot_position;
int l4_stroke = 0;
int l4_game_state = 0;
int l4_aiming = 0;
float l4_message_timer = 0;
int l4_message_type = 0;
float l4_animation_time = 0;
float l4_hud_height;
Vector2 l4_no_speed = {0,0};

//l4_walls: outer frame, temple l4_walls, altar room l4_walls, and the 2 jungle hedges (last two)
Rectangle l4_walls[13];
Rectangle l4_temple_area;

//l4_river (fall in = chomp) and the l4_plank bridges: 0 normal one at the top, 1 risky shortcut at the bottom
Rectangle l4_river;
Rectangle l4_plank[2][5];
float l4_plank_timer[2][5];
float l4_crack_time[2];
Vector2 l4_bridge_bank[2];

//l4_mud (slow), l4_moss (slippery), l4_quicksand (slow, and a l4_ball that stays still sinks)
Rectangle l4_mud;
Rectangle l4_moss[2];
Rectangle l4_quicksand[2];
float l4_sink_timer = 0;

//boulders rolling back and forth (the level 3 asteroids)
Vector2 l4_boulder[3];
Vector2 l4_boulder_start[3];
Vector2 l4_boulder_end[3];
Vector2 l4_boulder_velocity[3];
float l4_boulder_t[3];
float l4_boulder_time[3];
float l4_boulder_radius[3];
int l4_boulder_direction[3];

//dart traps (the level 1 laser): off 1.2s, warning 0.4s, on 1.4s
Rectangle l4_dart_gate[4];
float l4_dart_clock[4];

//spinning totem (the level 1 l4_fan)
Vector2 l4_fan;
float l4_fan_angle = 0;
float l4_fan_spin = 100;
float l4_fan_blade_length;
float l4_fan_blade_thickness;
float l4_fan_hub_radius;

//mushrooms (the level 3 bumpers)
Vector2 l4_bumper[6];
float l4_bumper_radius;
float l4_bumper_hit[6];

//pressure plates (0 opens the temple gate for 6s, 1 and 2 open the altar l4_door) and the 2 l4_stone doors
Vector2 l4_plate[3];
float l4_plate_radius;
int l4_plate_down[3];
float l4_gate_timer = 0;
Rectangle l4_door[2];
float l4_door_closed_y[2];
float l4_door_open[2];
Vector2 l4_door_velocity[2];

//splash
Vector2 l4_splash_position;
float l4_splash_timer = 0;


//a rectangle in design units (a 1920 x 1080 screen), made to fit the real screen
Rectangle l4_make_rect(float x, float y, float w, float h)
{
    Rectangle rec = {x*l4_u,y*l4_u,w*l4_u,h*l4_u};
    return rec;
}

//a point in design units
Vector2 l4_make_point(float x, float y)
{
    Vector2 point = {x*l4_u,y*l4_u};
    return point;
}


void l4_reset_level()
{
    //sizes
    l4_hud_height = 70*l4_u;
    l4_radius_ball = 7*l4_u;
    l4_radius_pot = 11*l4_u;

    //l4_walls
    l4_walls[0] = l4_make_rect(0,70,1920,28);        //frame top
    l4_walls[1] = l4_make_rect(0,1052,1920,28);      //frame bottom
    l4_walls[2] = l4_make_rect(0,70,28,1010);        //frame left
    l4_walls[3] = l4_make_rect(1892,70,28,1010);     //frame right
    l4_walls[4] = l4_make_rect(1152,98,28,422);      //temple outer wall, above the gate
    l4_walls[5] = l4_make_rect(1152,620,28,432);     //temple outer wall, below the gate
    l4_walls[6] = l4_make_rect(1380,300,348,28);     //altar room top
    l4_walls[7] = l4_make_rect(1380,822,348,28);     //altar room bottom
    l4_walls[8] = l4_make_rect(1380,300,28,550);     //altar room left
    l4_walls[9] = l4_make_rect(1700,300,28,220);     //altar room right, above the l4_door
    l4_walls[10] = l4_make_rect(1700,620,28,230);    //altar room right, below the l4_door
    l4_walls[11] = l4_make_rect(190,700,430,40);     //jungle hedge, low
    l4_walls[12] = l4_make_rect(28,400,430,40);      //jungle hedge, high
    l4_temple_area = l4_make_rect(1152,98,740,954);

    //l4_river and bridges
    l4_river = l4_make_rect(620,98,140,954);
    for (int i=0; i<5; i++)
    {
        l4_plank[0][i] = l4_make_rect(620+i*28,165,28,90);
        l4_plank[1][i] = l4_make_rect(620+i*28,915,28,90);
        l4_plank_timer[0][i] = 0;
        l4_plank_timer[1][i] = 0;
    }
    l4_crack_time[0] = 0.5;
    l4_crack_time[1] = 0.2;
    l4_bridge_bank[0] = l4_make_point(590,210);
    l4_bridge_bank[1] = l4_make_point(590,960);

    //ground patches
    l4_mud = l4_make_rect(250,850,180,140);
    l4_moss[0] = l4_make_rect(770,260,130,180);
    l4_moss[1] = l4_make_rect(1740,330,140,170);
    l4_quicksand[0] = l4_make_rect(850,720,200,150);
    l4_quicksand[1] = l4_make_rect(1200,120,160,160);
    l4_sink_timer = 0;

    //boulders
    l4_boulder_start[0] = l4_make_point(100,570);
    l4_boulder_end[0] = l4_make_point(560,570);
    l4_boulder_radius[0] = 30*l4_u;
    l4_boulder_time[0] = 3.0;
    l4_boulder_start[1] = l4_make_point(1070,420);
    l4_boulder_end[1] = l4_make_point(830,420);
    l4_boulder_radius[1] = 28*l4_u;
    l4_boulder_time[1] = 2.2;
    l4_boulder_start[2] = l4_make_point(1280,380);
    l4_boulder_end[2] = l4_make_point(1280,760);
    l4_boulder_radius[2] = 30*l4_u;
    l4_boulder_time[2] = 2.6;
    for (int i=0; i<3; i++)
    {
        l4_boulder_t[i] = 0;
        l4_boulder_direction[i] = 1;
        l4_boulder[i] = l4_boulder_start[i];
        l4_boulder_velocity[i] = l4_no_speed;
    }

    //dart traps
    l4_dart_gate[0] = l4_make_rect(1180,330,200,8);
    l4_dart_gate[1] = l4_make_rect(1180,820,200,8);
    l4_dart_gate[2] = l4_make_rect(1540,98,8,202);
    l4_dart_gate[3] = l4_make_rect(1540,850,8,202);
    for (int i=0; i<4; i++) l4_dart_clock[i] = i*0.75;

    //totem in front of the altar l4_door
    l4_fan = l4_make_point(1810,570);
    l4_fan_blade_length = 80*l4_u;
    l4_fan_blade_thickness = 12*l4_u;
    l4_fan_hub_radius = 12*l4_u;
    l4_fan_angle = 0;

    //mushrooms
    l4_bumper_radius = 22*l4_u;
    l4_bumper[0] = l4_make_point(250,220);
    l4_bumper[1] = l4_make_point(430,300);
    l4_bumper[2] = l4_make_point(450,820);
    l4_bumper[3] = l4_make_point(1060,960);
    l4_bumper[4] = l4_make_point(1480,470);
    l4_bumper[5] = l4_make_point(1620,690);
    for (int i=0; i<6; i++) l4_bumper_hit[i] = 0;

    //plates and doors
    l4_plate_radius = 26*l4_u;
    l4_plate[0] = l4_make_point(1000,570);
    l4_plate[1] = l4_make_point(1810,200);
    l4_plate[2] = l4_make_point(1810,950);
    for (int i=0; i<3; i++) l4_plate_down[i] = 0;
    l4_gate_timer = 0;
    l4_door[0] = l4_make_rect(1152,520,28,100);
    l4_door[1] = l4_make_rect(1700,520,28,100);
    for (int i=0; i<2; i++)
    {
        l4_door_closed_y[i] = l4_door[i].y;
        l4_door_open[i] = 0;
        l4_door_velocity[i] = l4_no_speed;
    }

    //l4_ball and l4_pot
    l4_start_position = l4_make_point(120,980);
    l4_ball = l4_start_position;
    l4_last_shot_position = l4_start_position;
    l4_speed.x = 0;
    l4_speed.y= 0;
    l4_pot = l4_make_point(1560,575);
    l4_stroke = 0;
    l4_game_state = 0;
    l4_aiming = 0;
    l4_message_timer = 0;
    l4_splash_timer = 0;
}


//bounce off a straight rectangle, rec_speed is how fast the rectangle itself is moving
int l4_bounce_off_rectangle(Rectangle rec, Vector2 rec_speed)
{
    if (CheckCollisionCircleRec(l4_ball,l4_radius_ball,rec))
    {
        Vector2 collision_point;
        collision_point.x = Clamp(l4_ball.x,rec.x,rec.x+rec.width);
        collision_point.y = Clamp(l4_ball.y,rec.y,rec.y+rec.height);
        Vector2 normal = Vector2Subtract(l4_ball,collision_point);

        //centre is inside, go out through the nearest side
        if (normal.x==0 && normal.y==0)
        {
            float left = l4_ball.x - rec.x;
            float right = rec.x + rec.width - l4_ball.x;
            float top = l4_ball.y - rec.y;
            float bottom = rec.y + rec.height - l4_ball.y;
            if (left<=right && left<=top && left<=bottom) { normal.x = -1; collision_point.x = rec.x; }
            else if (right<=top && right<=bottom) { normal.x = 1; collision_point.x = rec.x + rec.width; }
            else if (top<=bottom) { normal.y = -1; collision_point.y = rec.y; }
            else { normal.y = 1; collision_point.y = rec.y + rec.height; }
        }
        normal = Vector2Normalize(normal);
        l4_ball = Vector2Add(collision_point,Vector2Scale(normal,l4_radius_ball));

        //reflect only if the l4_ball is going into it
        Vector2 relative_speed = Vector2Subtract(l4_speed,rec_speed);
        if (Vector2DotProduct(relative_speed,normal)<0)
        {
            relative_speed = Vector2Reflect(relative_speed,normal);
            l4_speed = Vector2Add(relative_speed,rec_speed);

            //moving crab side: also knock the l4_ball out of its row, or it gets hit again and again
            if (rec_speed.x!=0 && normal.y==0)
            {
                if (l4_ball.y < rec.y+rec.height/2) l4_speed.y = l4_speed.y - fabsf(rec_speed.x)/2;
                else l4_speed.y = l4_speed.y + fabsf(rec_speed.x)/2;
            }
            l4_speed = Vector2ClampValue(l4_speed,0,l4_max_speed*l4_u);
        }
        return 1;
    }
    return 0;
}

//bounce off a round thing: bounce is 1 for normal and more for bouncy, circle_speed is how fast it moves
int l4_bounce_off_circle(Vector2 center, float radius, float bounce, Vector2 circle_speed)
{
    Vector2 normal = Vector2Subtract(l4_ball,center);
    float distance = Vector2Length(normal);
    if (distance < radius + l4_radius_ball)
    {
        if (distance==0) normal.y = -1;
        normal = Vector2Normalize(normal);
        l4_ball = Vector2Add(center,Vector2Scale(normal,radius + l4_radius_ball));
        Vector2 relative_speed = Vector2Subtract(l4_speed,circle_speed);
        if (Vector2DotProduct(relative_speed,normal)<0)
        {
            relative_speed = Vector2Scale(Vector2Reflect(relative_speed,normal),bounce);
            l4_speed = Vector2ClampValue(Vector2Add(relative_speed,circle_speed),0,l4_max_speed*l4_u);
            return 1;
        }
    }
    return 0;
}

//bounce off a turned rectangle (diamond, l4_fan blades), spin is in degrees per second
void l4_bounce_off_rotated_rectangle(Vector2 center, float rec_width, float rec_height, float angle, float spin)
{
    //look at the l4_ball as if the rectangle was not turned
    Vector2 local_ball = Vector2Rotate(Vector2Subtract(l4_ball,center),-angle*DEG2RAD);
    Rectangle rec = {-rec_width/2,-rec_height/2,rec_width,rec_height};
    if (CheckCollisionCircleRec(local_ball,l4_radius_ball,rec))
    {
        Vector2 collision_point;
        collision_point.x = Clamp(local_ball.x,rec.x,rec.x+rec.width);
        collision_point.y = Clamp(local_ball.y,rec.y,rec.y+rec.height);
        Vector2 normal = Vector2Subtract(local_ball,collision_point);

        //centre is inside, go out through the nearest side
        if (normal.x==0 && normal.y==0)
        {
            float left = local_ball.x - rec.x;
            float right = rec.x + rec.width - local_ball.x;
            float top = local_ball.y - rec.y;
            float bottom = rec.y + rec.height - local_ball.y;
            if (left<=right && left<=top && left<=bottom) { normal.x = -1; collision_point.x = rec.x; }
            else if (right<=top && right<=bottom) { normal.x = 1; collision_point.x = rec.x + rec.width; }
            else if (top<=bottom) { normal.y = -1; collision_point.y = rec.y; }
            else { normal.y = 1; collision_point.y = rec.y + rec.height; }
        }

        //turn the point and normal back to the screen
        normal = Vector2Rotate(Vector2Normalize(normal),angle*DEG2RAD);
        collision_point = Vector2Add(center,Vector2Rotate(collision_point,angle*DEG2RAD));
        l4_ball = Vector2Add(collision_point,Vector2Scale(normal,l4_radius_ball));

        //l4_speed of the blade at the point it touches the l4_ball
        Vector2 arm = Vector2Subtract(collision_point,center);
        Vector2 rec_speed = {-arm.y*spin*DEG2RAD, arm.x*spin*DEG2RAD};

        Vector2 relative_speed = Vector2Subtract(l4_speed,rec_speed);
        if (Vector2DotProduct(relative_speed,normal)<0)
        {
            relative_speed = Vector2Reflect(relative_speed,normal);
            l4_speed = Vector2ClampValue(Vector2Add(relative_speed,rec_speed),0,l4_max_speed*l4_u);
        }
    }
}

//is this point in the l4_river, and not on a l4_plank that is still there
int l4_in_the_river(Vector2 point)
{
    if (CheckCollisionPointRec(point,l4_river)==0) return 0;
    for (int b=0; b<2; b++)
    {
        for (int i=0; i<5; i++)
        {
            if (CheckCollisionPointRec(point,l4_plank[b][i]) && l4_plank_timer[b][i]<l4_crack_time[b]) return 0;
        }
    }
    return 1;
}


void l4_update_obstacles(float dt)
{
    //boulders rolling back and forth (the level 3 asteroids)
    for (int i=0; i<3; i++)
    {
        l4_boulder_t[i] = l4_boulder_t[i] + l4_boulder_direction[i]*dt/l4_boulder_time[i];
        if (l4_boulder_t[i]>=1)
        {
            l4_boulder_t[i] = 1;
            l4_boulder_direction[i] = -1;
        }
        if (l4_boulder_t[i]<=0)
        {
            l4_boulder_t[i] = 0;
            l4_boulder_direction[i] = 1;
        }
        l4_boulder[i] = Vector2Lerp(l4_boulder_start[i],l4_boulder_end[i],l4_boulder_t[i]);
        l4_boulder_velocity[i] = Vector2Scale(Vector2Subtract(l4_boulder_end[i],l4_boulder_start[i]),l4_boulder_direction[i]/l4_boulder_time[i]);
    }

    //totem spinning (the level 1 l4_fan)
    l4_fan_angle = l4_fan_angle + l4_fan_spin*dt;
    if (l4_fan_angle>=360) l4_fan_angle = l4_fan_angle - 360;

    //dart clocks
    for (int i=0; i<4; i++)
    {
        l4_dart_clock[i] = l4_dart_clock[i] + dt;
        if (l4_dart_clock[i]>=3.0) l4_dart_clock[i] = l4_dart_clock[i] - 3.0;
    }

    //planks: cracking, then gone for 5 seconds, then back
    for (int b=0; b<2; b++)
    {
        for (int i=0; i<5; i++)
        {
            if (l4_plank_timer[b][i]>0) l4_plank_timer[b][i] = l4_plank_timer[b][i] + dt;
            if (l4_plank_timer[b][i]>=l4_crack_time[b]+5) l4_plank_timer[b][i] = 0;
        }
    }

    //doors slide down into the wall in half a second, and never close on the l4_ball (it jams)
    if (l4_gate_timer>0) l4_gate_timer = l4_gate_timer - dt;
    for (int i=0; i<2; i++)
    {
        int want_open = 0;
        if (i==0 && l4_gate_timer>0) want_open = 1;
        if (i==1 && l4_plate_down[1]==1 && l4_plate_down[2]==1) want_open = 1;
        Rectangle doorway = {l4_door[i].x,l4_door_closed_y[i],l4_door[i].width,100*l4_u};
        l4_door_velocity[i] = l4_no_speed;
        if (want_open==1 && l4_door_open[i]<1)
        {
            l4_door_open[i] = l4_door_open[i] + 2*dt;
            if (l4_door_open[i]>1) l4_door_open[i] = 1;
            l4_door_velocity[i].y = 200*l4_u;
        }
        if (want_open==0 && l4_door_open[i]>0 && CheckCollisionCircleRec(l4_ball,l4_radius_ball+2*l4_u,doorway)==0)
        {
            l4_door_open[i] = l4_door_open[i] - 2*dt;
            if (l4_door_open[i]<0) l4_door_open[i] = 0;
            l4_door_velocity[i].y = -200*l4_u;
        }
        l4_door[i].y = l4_door_closed_y[i] + l4_door_open[i]*100*l4_u;
    }

    for (int i=0; i<6; i++)
    {
        if (l4_bumper_hit[i]>0) l4_bumper_hit[i] = l4_bumper_hit[i] - dt;
    }
    if (l4_splash_timer>0) l4_splash_timer = l4_splash_timer - dt;
}


//after the l4_river, darts or l4_quicksand, the l4_ball goes back to where it was shot from
void l4_send_ball_back()
{
    l4_ball = l4_last_shot_position;

    //not right in an l4_boulder's path, or it gets knocked off again and again
    for (int i=0; i<3; i++)
    {
        Vector2 path = Vector2Subtract(l4_boulder_end[i],l4_boulder_start[i]);
        float along = Vector2DotProduct(Vector2Subtract(l4_ball,l4_boulder_start[i]),path)/Vector2DotProduct(path,path);
        along = Clamp(along,0,1);
        Vector2 closest = Vector2Add(l4_boulder_start[i],Vector2Scale(path,along));
        Vector2 away = Vector2Subtract(l4_ball,closest);
        float gap = l4_boulder_radius[i] + l4_radius_ball + 2*l4_u;
        if (Vector2Length(away)<gap)
        {
            Vector2 side = {-path.y,path.x};
            side = Vector2Normalize(side);
            if (Vector2DotProduct(away,side)<0) side = Vector2Negate(side);
            l4_ball = Vector2Add(closest,Vector2Scale(side,gap));
        }
    }

    //not where the totem arms sweep, or it gets knocked off again and again (the level 1 piston rule)
    float sweep = l4_fan_blade_length/2 + l4_radius_ball + 2*l4_u;
    if (Vector2Distance(l4_ball,l4_fan)<sweep)
    {
        if (l4_ball.y<l4_fan.y) l4_ball.y = l4_fan.y - sweep;
        else l4_ball.y = l4_fan.y + sweep;
    }

    //not inside a dart trap (level 1 rule)
    for (int i=0; i<4; i++)
    {
        if (CheckCollisionCircleRec(l4_ball,l4_radius_ball,l4_dart_gate[i])) l4_ball = l4_start_position;
    }

    //the l4_plank it was on has fallen: back to that bridge's bank
    if (l4_in_the_river(l4_ball))
    {
        Vector2 target = l4_ball;
        l4_ball = l4_start_position;
        for (int b=0; b<2; b++)
        {
            if (target.y>=l4_plank[b][0].y && target.y<=l4_plank[b][0].y+l4_plank[b][0].height) l4_ball = l4_bridge_bank[b];
        }
    }

    //falling in or getting hit lets both altar plates back up
    l4_plate_down[1] = 0;
    l4_plate_down[2] = 0;
    l4_sink_timer = 0;

    l4_speed.x = 0;
    l4_speed.y = 0;
}


void l4_update_ball(float dt)
{
    int pushed = 0;

    //moving
    l4_ball = Vector2Add(l4_ball,Vector2Scale(l4_speed,dt));

    //proportional deceleration (l4_mud and l4_quicksand are slow, l4_moss is slippery)
    float friction = 110*l4_u;
    int in_quicksand = 0;
    if (CheckCollisionPointRec(l4_ball,l4_mud)) friction = 400*l4_u;
    for (int i=0; i<2; i++)
    {
        if (CheckCollisionPointRec(l4_ball,l4_moss[i])) friction = 25*l4_u;
        if (CheckCollisionPointRec(l4_ball,l4_quicksand[i]))
        {
            friction = 400*l4_u;
            in_quicksand = 1;
        }
    }
    float ball_speed = Vector2Length(l4_speed);
    if (ball_speed>0)
    {
        float new_speed = ball_speed - friction*dt;
        if (new_speed<0) new_speed = 0;
        l4_speed = Vector2Scale(l4_speed,new_speed/ball_speed);
    }
    if (pushed==0 && Vector2Length(l4_speed)<6*l4_u)
    {
        l4_speed.x = 0;
        l4_speed.y= 0;
    }

    //planks start cracking when the l4_ball rolls onto them
    for (int b=0; b<2; b++)
    {
        for (int i=0; i<5; i++)
        {
            if (l4_plank_timer[b][i]==0 && CheckCollisionPointRec(l4_ball,l4_plank[b][i])) l4_plank_timer[b][i] = 0.001;
        }
    }

    //pressure plates
    if (Vector2Distance(l4_ball,l4_plate[0])<l4_plate_radius) l4_gate_timer = 6;
    for (int i=1; i<3; i++)
    {
        if (Vector2Distance(l4_ball,l4_plate[i])<l4_plate_radius) l4_plate_down[i] = 1;
    }

    //l4_river (checked before the collisions, so if something is on the old spot it pushes the l4_ball out)
    if (l4_in_the_river(l4_ball))
    {
        l4_splash_position = l4_ball;
        l4_splash_timer = 0.5;
        l4_send_ball_back();
        l4_message_type = 1;
        l4_message_timer = 1;
    }

    //l4_quicksand: a l4_ball that stays still sinks in 3 seconds
    if (in_quicksand==1 && l4_speed.x==0 && l4_speed.y==0)
    {
        l4_sink_timer = l4_sink_timer + dt;
        if (l4_sink_timer>=3)
        {
            l4_send_ball_back();
            l4_message_type = 3;
            l4_message_timer = 1;
        }
    }
    else l4_sink_timer = 0;

    //dart traps (the level 1 laser)
    for (int i=0; i<4; i++)
    {
        if (l4_dart_clock[i]>=1.6 && CheckCollisionCircleRec(l4_ball,l4_radius_ball,l4_dart_gate[i]))
        {
            l4_send_ball_back();
            l4_message_type = 2;
            l4_message_timer = 1;
        }
    }

    //mushrooms and boulders
    for (int i=0; i<6; i++)
    {
        if (l4_bounce_off_circle(l4_bumper[i],l4_bumper_radius,1.3,l4_no_speed)) l4_bumper_hit[i] = 0.15;
    }
    for (int i=0; i<3; i++)
    {
        l4_bounce_off_circle(l4_boulder[i],l4_boulder_radius[i],1,l4_boulder_velocity[i]);
    }

    //spinning totem (the level 1 l4_fan)
    l4_bounce_off_circle(l4_fan,l4_fan_hub_radius,1,l4_no_speed);
    l4_bounce_off_rotated_rectangle(l4_fan,l4_fan_blade_length,l4_fan_blade_thickness,l4_fan_angle,l4_fan_spin);
    l4_bounce_off_rotated_rectangle(l4_fan,l4_fan_blade_length,l4_fan_blade_thickness,l4_fan_angle+90,l4_fan_spin);

    //l4_stone doors
    for (int i=0; i<2; i++)
    {
        l4_bounce_off_rectangle(l4_door[i],l4_door_velocity[i]);
    }

    //l4_walls (last, so the l4_ball never ends inside a wall)
    for (int i=0; i<13; i++)
    {
        l4_bounce_off_rectangle(l4_walls[i],l4_no_speed);
    }

    //score
    if ((l4_ball.x>l4_pot.x-3*l4_radius_pot/4) && (l4_ball.x<l4_pot.x+3*l4_radius_pot/4) && (l4_ball.y>l4_pot.y-3*l4_radius_pot/4) && (l4_ball.y<l4_pot.y+3*l4_radius_pot/4))
    {
        l4_ball = l4_pot;
        l4_speed.x = 0;
        l4_speed.y= 0;
        l4_game_state = 1;
    }
}


//yellow and black bands, across the long side
void l4_draw_hazard_stripes(Rectangle rec)
{
    DrawRectangleRec(rec,l4_hazard_yellow);
    float band = 6*l4_u;
    if (rec.width>=rec.height)
    {
        int bands = rec.width/band;
        for (int i=0; i<bands; i++)
        {
            if (i%2==1) DrawRectangle(rec.x+i*band,rec.y,band,rec.height,BLACK);
        }
    }
    else
    {
        int bands = rec.height/band;
        for (int i=0; i<bands; i++)
        {
            if (i%2==1) DrawRectangle(rec.x,rec.y+i*band,rec.width,band,BLACK);
        }
    }
}

//a picture that repeats (made at 2x size), lined up with the screen so pieces join without seams
void l4_draw_tiled(Texture2D texture, Rectangle rec, Color tint)
{
    Rectangle source = {rec.x/l4_u*2,rec.y/l4_u*2,rec.width/l4_u*2,rec.height/l4_u*2};
    Vector2 no_origin = {0,0};
    DrawTexturePro(texture,source,rec,no_origin,0,tint);
}


//foam along the 4 edges of a piece of ground, the bumpy side faces the water
void l4_draw_foam(Rectangle rec, Color tint)
{
    int frame = (int)(l4_animation_time*8)%4;
    float long_side = rec.width + 40*l4_u;
    Rectangle source = {rec.x/l4_u*2,frame*128+1,long_side/l4_u*2,126};
    Vector2 origin = {long_side/2,32*l4_u};

    //top and bottom edges (top one turned around)
    Rectangle top = {rec.x+rec.width/2,rec.y-18*l4_u,long_side,64*l4_u};
    DrawTexturePro(l4_foam_texture,source,top,origin,180,tint);
    Rectangle bottom = {rec.x+rec.width/2,rec.y+rec.height+18*l4_u,long_side,64*l4_u};
    DrawTexturePro(l4_foam_texture,source,bottom,origin,0,tint);

    //left and right edges
    long_side = rec.height + 40*l4_u;
    source.x = rec.y/l4_u*2;
    source.width = long_side/l4_u*2;
    origin.x = long_side/2;
    Rectangle left = {rec.x-18*l4_u,rec.y+rec.height/2,long_side,64*l4_u};
    DrawTexturePro(l4_foam_texture,source,left,origin,90,tint);
    Rectangle right = {rec.x+rec.width+18*l4_u,rec.y+rec.height/2,long_side,64*l4_u};
    DrawTexturePro(l4_foam_texture,source,right,origin,-90,tint);
}


void l4_draw_ground()
{
    float t = l4_animation_time;
    Vector2 no_origin = {0,0};

    //jungle floor everywhere, temple floor inside the temple
    Rectangle everything = {0,0,l4_width,l4_height};
    l4_draw_tiled(l4_ground_texture,everything,WHITE);

    //l4_river: the beach water tinted green, foam along both banks
    int water_frame = (int)(t*2)%2;
    Rectangle water_source = {water_frame*512,0,512,512};
    BeginScissorMode(l4_river.x,l4_river.y,l4_river.width,l4_river.height);
    int rows = l4_river.height/(256*l4_u) + 1;
    for (int j=0; j<rows; j++)
    {
        Rectangle tile = {l4_river.x,l4_river.y+j*256*l4_u,256*l4_u,256*l4_u};
        DrawTexturePro(l4_water_texture,water_source,tile,no_origin,0,l4_river_tint);
    }
    EndScissorMode();
    Rectangle left_bank = l4_make_rect(28,98,592,954);
    Rectangle right_bank = l4_make_rect(760,98,392,954);
    l4_draw_foam(left_bank,Fade(WHITE,0.8));
    l4_draw_foam(right_bank,Fade(WHITE,0.8));
    l4_draw_tiled(l4_temple_texture,l4_temple_area,WHITE);

    //l4_mud, with ripples
    l4_draw_tiled(l4_ground_texture,l4_mud,GetColor(0x8C6440FF));
    for (int j=1; j<l4_mud.height/(16*l4_u); j++)
    {
        Vector2 a = {l4_mud.x+10*l4_u,l4_mud.y+j*16*l4_u};
        Vector2 b = {l4_mud.x+l4_mud.width-10*l4_u,a.y+3*l4_u};
        DrawLineEx(a,b,2*l4_u,Fade(BLACK,0.18));
    }
    DrawRectangleLinesEx(l4_mud,2*l4_u,Fade(BLACK,0.25));

    //l4_moss, green and shiny
    for (int i=0; i<2; i++)
    {
        DrawRectangleRec(l4_moss[i],Fade(l4_moss_green,0.55));
        for (int k=0; k<12; k++)
        {
            float shine_x = l4_moss[i].x + fmod(k*47*l4_u,l4_moss[i].width);
            float shine_y = l4_moss[i].y + fmod(k*31*l4_u + 9*l4_u,l4_moss[i].height);
            DrawCircle(shine_x,shine_y,2.5*l4_u,Fade(WHITE,0.2+0.2*sin(t*3+k)));
        }
    }

    //l4_quicksand, slowly turning
    for (int i=0; i<2; i++)
    {
        Rectangle q = l4_quicksand[i];
        Vector2 middle = {q.x+q.width/2,q.y+q.height/2};
        DrawRectangleRec(q,GetColor(0xC8A56EFF));
        for (int j=0; j<4; j++)
        {
            float ring = 12*l4_u + j*16*l4_u;
            float turn = t*40;
            if (j%2==1) turn = -t*40;
            DrawRing(middle,ring,ring+3*l4_u,turn+j*50,turn+j*50+220,24,Fade(GetColor(0x8A6A3AFF),0.6));
        }
        DrawRectangleLinesEx(q,3*l4_u,GetColor(0x8A6A3AFF));
    }

    //l4_plank bridges with rope rails: cracked picture while cracking, gone for 5 seconds
    for (int b=0; b<2; b++)
    {
        for (int i=0; i<5; i++)
        {
            float timer = l4_plank_timer[b][i];
            if (timer>=l4_crack_time[b]) continue;
            Rectangle p = l4_plank[b][i];
            int frame = 0;
            float shake = 0;
            if (timer>0)
            {
                frame = 1;
                shake = sin(t*60)*2*l4_u;
            }
            Rectangle source = {frame*256+8,12,240,56};
            Rectangle dest = {p.x+p.width/2+shake,p.y+p.height/2,p.height,p.width-2*l4_u};
            Vector2 origin = {p.height/2,(p.width-2*l4_u)/2};
            DrawTexturePro(l4_plank_texture,source,dest,origin,90,WHITE);
        }
        Vector2 rope1 = {l4_river.x-6*l4_u,l4_plank[b][0].y};
        Vector2 rope2 = {l4_river.x+l4_river.width+6*l4_u,l4_plank[b][0].y};
        DrawLineEx(rope1,rope2,3*l4_u,GetColor(0xC9A66BFF));
        rope1.y = rope1.y + l4_plank[b][0].height;
        rope2.y = rope2.y + l4_plank[b][0].height;
        DrawLineEx(rope1,rope2,3*l4_u,GetColor(0xC9A66BFF));
    }

    //pressure plates, l4_gold when pressed (the gate l4_plate shows how much time is left)
    for (int i=0; i<3; i++)
    {
        int down = l4_plate_down[i];
        if (i==0 && l4_gate_timer>0) down = 1;
        DrawCircle(l4_plate[i].x,l4_plate[i].y,l4_plate_radius+4*l4_u,l4_stone_mortar);
        if (down==1)
        {
            DrawCircleGradient(l4_plate[i],l4_plate_radius*2,Fade(l4_gold,0.35),BLANK);
            DrawCircle(l4_plate[i].x,l4_plate[i].y,l4_plate_radius,l4_gold);
            DrawRing(l4_plate[i],l4_plate_radius*0.45,l4_plate_radius*0.6,0,360,24,l4_gold_light);
        }
        else
        {
            DrawCircle(l4_plate[i].x,l4_plate[i].y,l4_plate_radius,l4_stone);
            DrawRing(l4_plate[i],l4_plate_radius*0.45,l4_plate_radius*0.6,0,360,24,l4_stone_dark);
        }
        if (i==0 && l4_gate_timer>0) DrawRing(l4_plate[i],l4_plate_radius+6*l4_u,l4_plate_radius+10*l4_u,-90,-90+l4_gate_timer/6*360,32,l4_gold_light);
    }
}


//l4_stone doors (drawn before the l4_walls, so the part that slid into the wall is hidden)
void l4_draw_doors()
{
    for (int i=0; i<2; i++)
    {
        DrawRectangleRec(l4_door[i],l4_stone_dark);
        DrawRectangleLinesEx(l4_door[i],3*l4_u,l4_gold);
        DrawCircle(l4_door[i].x+l4_door[i].width/2,l4_door[i].y+l4_door[i].height/2,6*l4_u,l4_gold);
    }
}


//mossy l4_stone l4_walls (the level 1 l4_brick l4_walls)
void l4_draw_walls()
{
    float brick_height = 14*l4_u;
    float brick_length = 28*l4_u;
    for (int i=0; i<13; i++)
    {
        Rectangle rec = l4_walls[i];

        //jungle hedges are leaves, not l4_stone
        if (i>=11)
        {
            DrawRectangleRec(rec,l4_jungle_dark);
            DrawRectangleLinesEx(rec,3*l4_u,Fade(BLACK,0.3));
            continue;
        }
        DrawRectangleRec(rec,l4_stone_mortar);

        //bricks, every second row moved by half a l4_brick
        int rows = rec.height/brick_height + 1;
        for (int row=0; row<rows; row++)
        {
            float y = rec.y + row*brick_height;
            float bottom = y + brick_height;
            if (bottom>rec.y+rec.height) bottom = rec.y + rec.height;
            float x = rec.x;
            if (row%2==1) x = rec.x - brick_length/2;
            int count = 0;
            while (x < rec.x+rec.width)
            {
                float left = x;
                float right = x + brick_length;
                if (left<rec.x) left = rec.x;
                if (right>rec.x+rec.width) right = rec.x + rec.width;
                if (right-left>3*l4_u && bottom-y>3*l4_u)
                {
                    Color colour = l4_stone;
                    if ((row*3 + count*7)%5==0) colour = l4_stone_dark;
                    DrawRectangle(left+1*l4_u,y+1*l4_u,right-left-2*l4_u,bottom-y-2*l4_u,colour);
                    DrawRectangle(left+1*l4_u,bottom-3*l4_u,right-left-2*l4_u,2*l4_u,Fade(BLACK,0.25));
                }
                x = x + brick_length;
                count++;
            }
        }

        //dark edge and l4_moss
        DrawRectangleLinesEx(rec,3*l4_u,Fade(BLACK,0.35));
        if (rec.height>rec.width)
        {
            int rivets = rec.height/(40*l4_u);
            for (int j=0; j<rivets; j++)
            {
                DrawCircle(rec.x+5*l4_u,rec.y+20*l4_u+j*40*l4_u,2.5*l4_u,l4_moss_green);
                DrawCircle(rec.x+rec.width-5*l4_u,rec.y+20*l4_u+j*40*l4_u,2.5*l4_u,l4_moss_green);
            }
        }
        else
        {
            int rivets = rec.width/(40*l4_u);
            for (int j=0; j<rivets; j++)
            {
                DrawCircle(rec.x+20*l4_u+j*40*l4_u,rec.y+5*l4_u,2.5*l4_u,l4_moss_green);
                DrawCircle(rec.x+20*l4_u+j*40*l4_u,rec.y+rec.height-5*l4_u,2.5*l4_u,l4_moss_green);
            }
        }
    }
}


void l4_draw_darts()
{
    float t = l4_animation_time;

    //dart traps: l4_stone heads at both ends, eyes blink red before firing, darts fly while on
    for (int i=0; i<4; i++)
    {
        Rectangle d = l4_dart_gate[i];
        int across = 0;
        if (d.width>d.height) across = 1;
        Vector2 head1 = {d.x+d.width/2,d.y-10*l4_u};
        Vector2 head2 = {d.x+d.width/2,d.y+d.height+10*l4_u};
        if (across==1)
        {
            head1.x = d.x-10*l4_u;
            head1.y = d.y+d.height/2;
            head2.x = d.x+d.width+10*l4_u;
            head2.y = d.y+d.height/2;
        }
        Color eye = Fade(BLACK,0.7);
        if (l4_dart_clock[i]>=1.2 && l4_dart_clock[i]<1.6 && fmod(t*10,2)<1) eye = RED;
        if (l4_dart_clock[i]>=1.6) eye = RED;
        DrawCircle(head1.x,head1.y,13*l4_u,l4_stone_dark);
        DrawCircle(head2.x,head2.y,13*l4_u,l4_stone_dark);
        DrawCircleLines(head1.x,head1.y,13*l4_u,l4_gold);
        DrawCircleLines(head2.x,head2.y,13*l4_u,l4_gold);
        DrawCircle(head1.x,head1.y,4*l4_u,eye);
        DrawCircle(head2.x,head2.y,4*l4_u,eye);

        if (l4_dart_clock[i]>=1.6)
        {
            DrawRectangleRec(d,Fade(RED,0.15));
            float length = d.height;
            if (across==1) length = d.width;
            for (int k=0; k<4; k++)
            {
                float along = fmod(t*600*l4_u + k*length/4,length);
                Vector2 tip = {d.x+d.width/2,d.y+along};
                Vector2 tail = {tip.x,tip.y-14*l4_u};
                if (across==1)
                {
                    tip.x = d.x+along;
                    tip.y = d.y+d.height/2;
                    tail.x = tip.x-14*l4_u;
                    tail.y = tip.y;
                }
                DrawLineEx(tail,tip,2*l4_u,l4_wood_brown);
                DrawCircle(tail.x,tail.y,3*l4_u,RED);
            }
        }
    }
}


//one jungle plant clump, picture and size picked from k
void l4_draw_clump(float x, float y, int k)
{
    Rectangle source = {(k%3)*320,0,320,320};
    float size = (60 + (k*37)%25)*l4_u;
    Rectangle dest = {x,y,size,size};
    Vector2 origin = {size/2,size/2};
    DrawTexturePro(l4_foliage_texture,source,dest,origin,(k*53)%360,WHITE);
}


void l4_draw_foliage()
{
    int k = 0;

    //along the hedges
    for (int i=11; i<13; i++)
    {
        int clumps = l4_walls[i].width/(70*l4_u);
        for (int j=0; j<=clumps; j++)
        {
            l4_draw_clump(l4_walls[i].x+j*70*l4_u,l4_walls[i].y+l4_walls[i].height/2,k);
            k++;
        }
    }

    //along the l4_river banks and the temple's outer wall, not on the bridges or the gate
    for (int j=0; j<11; j++)
    {
        float y = (130 + j*92)*l4_u;
        int near_bridge = 0;
        for (int b=0; b<2; b++)
        {
            if (y>l4_plank[b][0].y-40*l4_u && y<l4_plank[b][0].y+l4_plank[b][0].height+40*l4_u) near_bridge = 1;
        }
        if (near_bridge==0)
        {
            l4_draw_clump(606*l4_u,y,k);
            l4_draw_clump(774*l4_u,y+40*l4_u,k+1);
        }
        if (y<l4_door_closed_y[0]-50*l4_u || y>l4_door_closed_y[0]+150*l4_u) l4_draw_clump(1150*l4_u,y,k+2);
        k = k + 3;
    }
}


void l4_draw_obstacles()
{
    float t = l4_animation_time;

    //golden altar under the l4_pot
    DrawCircleGradient(l4_pot,110*l4_u,Fade(l4_gold,0.35),BLANK);
    DrawRing(l4_pot,34*l4_u,40*l4_u,0,360,48,l4_gold);
    DrawRing(l4_pot,48*l4_u,50*l4_u,t*30,t*30+300,48,Fade(l4_gold_light,0.7));
    for (int j=0; j<6; j++)
    {
        Vector2 arm = {60*l4_u,0};
        Vector2 sparkle = Vector2Add(l4_pot,Vector2Rotate(arm,(t*50+j*60)*DEG2RAD));
        DrawCircle(sparkle.x,sparkle.y,(2+sin(t*5+j))*l4_u,l4_gold_light);
    }

    //boulders (the big asteroid picture, tinted brown), rolling
    for (int i=0; i<3; i++)
    {
        float size = l4_boulder_radius[i]*2.4;
        Rectangle source = {0,0,192,192};
        Rectangle dest = {l4_boulder[i].x,l4_boulder[i].y,size,size};
        Vector2 origin = {size/2,size/2};
        DrawCircle(l4_boulder[i].x+5*l4_u,l4_boulder[i].y+7*l4_u,l4_boulder_radius[i],Fade(BLACK,0.3));
        DrawTexturePro(l4_boulder_texture,source,dest,origin,l4_boulder_t[i]*360,GetColor(0xC89A6AFF));
    }

    //mushrooms, they get bigger for a moment when hit
    for (int i=0; i<6; i++)
    {
        Vector2 m = l4_bumper[i];
        float r = l4_bumper_radius*(1 + l4_bumper_hit[i]*1.5);
        DrawCircle(m.x+3*l4_u,m.y+5*l4_u,r,Fade(BLACK,0.3));
        DrawCircle(m.x,m.y,r,GetColor(0xC0392BFF));
        DrawCircle(m.x-6*l4_u,m.y-6*l4_u,r*0.3,WHITE);
        DrawCircle(m.x+8*l4_u,m.y+2*l4_u,r*0.2,WHITE);
        DrawCircle(m.x-2*l4_u,m.y+9*l4_u,r*0.15,WHITE);
        DrawCircleLines(m.x,m.y,r,GetColor(0x5E1A12FF));
    }

    //spinning totem (the level 1 l4_fan, with wooden arms)
    Rectangle blade = {l4_fan.x,l4_fan.y,l4_fan_blade_length,l4_fan_blade_thickness};
    Vector2 blade_origin = {l4_fan_blade_length/2,l4_fan_blade_thickness/2};
    DrawRectanglePro(blade,blade_origin,l4_fan_angle-24,Fade(l4_wood_brown,0.12));
    DrawRectanglePro(blade,blade_origin,l4_fan_angle+90-24,Fade(l4_wood_brown,0.12));
    DrawRectanglePro(blade,blade_origin,l4_fan_angle-12,Fade(l4_wood_brown,0.25));
    DrawRectanglePro(blade,blade_origin,l4_fan_angle+90-12,Fade(l4_wood_brown,0.25));
    DrawRectanglePro(blade,blade_origin,l4_fan_angle,l4_wood_brown);
    DrawRectanglePro(blade,blade_origin,l4_fan_angle+90,l4_wood_brown);
    DrawCircle(l4_fan.x,l4_fan.y,l4_fan_hub_radius,l4_steel_dark);
    DrawCircleLines(l4_fan.x,l4_fan.y,l4_fan_hub_radius,BLACK);
    Vector2 bolt_arm = {9*l4_u,0};
    for (int j=0; j<4; j++)
    {
        Vector2 bolt = Vector2Add(l4_fan,Vector2Rotate(bolt_arm,(l4_fan_angle+45+j*90)*DEG2RAD));
        DrawCircle(bolt.x,bolt.y,2.5*l4_u,l4_gold);
    }

    //splash, 5 pictures in half a second
    if (l4_splash_timer>0)
    {
        int frame = (0.5-l4_splash_timer)/0.1;
        if (frame>4) frame = 4;
        Rectangle source = {frame*192,0,192,192};
        Rectangle dest = {l4_splash_position.x,l4_splash_position.y,96*l4_u,96*l4_u};
        Vector2 origin = {48*l4_u,48*l4_u};
        DrawTexturePro(l4_splash_texture,source,dest,origin,0,l4_river_tint);
    }
}


void l4_draw_ball_and_pot()
{
    //start l4_plate
    Rectangle l4_plate = {l4_start_position.x-50*l4_u,l4_start_position.y-30*l4_u,100*l4_u,60*l4_u};
    DrawRectangleRec(l4_plate,l4_steel);
    DrawRectangleLinesEx(l4_plate,3*l4_u,l4_steel_dark);
    Rectangle plate_band = {l4_plate.x,l4_plate.y+l4_plate.height-10*l4_u,l4_plate.width,10*l4_u};
    l4_draw_hazard_stripes(plate_band);
    DrawText("START",l4_start_position.x-MeasureText("START",18*l4_u)/2,l4_plate.y+5*l4_u,18*l4_u,WHITE);

    //l4_pot with hazard ring
    for (int i=0; i<8; i++)
    {
        Color colour = l4_hazard_yellow;
        if (i%2==1) colour = BLACK;
        DrawRing(l4_pot,l4_radius_pot+6*l4_u,l4_radius_pot+12*l4_u,i*45,i*45+45,6,colour);
    }
    DrawCircle(l4_pot.x,l4_pot.y,l4_radius_pot+6*l4_u,l4_steel_light);
    DrawRing(l4_pot,l4_radius_pot+1*l4_u,l4_radius_pot+5*l4_u,0,360,24,l4_steel);
    DrawCircle(l4_pot.x,l4_pot.y,l4_radius_pot,BLACK);

    //flag
    Vector2 pole_top = {l4_pot.x,l4_pot.y-60*l4_u};
    DrawLineEx(l4_pot,pole_top,3*l4_u,l4_steel_light);
    for (int i=0; i<5; i++)
    {
        float wave = sin(l4_animation_time*6 - i*0.8)*2.5*l4_u;
        DrawRectangle(l4_pot.x+1*l4_u+i*6*l4_u,pole_top.y+wave,6*l4_u,18*l4_u,l4_laser_red);
    }

    //l4_ball (it shrinks while sinking in l4_quicksand)
    float ball_size = l4_radius_ball*(1 - l4_sink_timer/4);
    DrawCircle(l4_ball.x+3*l4_u,l4_ball.y+4*l4_u,ball_size,Fade(BLACK,0.45));
    DrawCircle(l4_ball.x,l4_ball.y,ball_size,GetColor(0xEDEDEDFF));
    DrawCircleLines(l4_ball.x,l4_ball.y,ball_size,GRAY);
    DrawCircle(l4_ball.x-2*l4_u,l4_ball.y-2*l4_u,2*l4_u,WHITE);

    //aim line
    if (l4_aiming==1 && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        Vector2 mouse = {GetMouseX(),GetMouseY()};
        DrawLineEx(l4_ball,mouse,4*l4_u,Fade(WHITE,0.75));
        DrawCircle(mouse.x,mouse.y,5*l4_u,Fade(WHITE,0.75));
    }
}


void l4_draw_hud()
{
    //dark edges
    DrawRectangleGradientH(0,l4_hud_height,90*l4_u,l4_height-l4_hud_height,Fade(BLACK,0.45),BLANK);
    DrawRectangleGradientH(l4_width-90*l4_u,l4_hud_height,90*l4_u,l4_height-l4_hud_height,BLANK,Fade(BLACK,0.45));
    DrawRectangleGradientV(0,l4_height-70*l4_u,l4_width,70*l4_u,BLANK,Fade(BLACK,0.45));

    //l4_steel bar
    DrawRectangleGradientV(0,0,l4_width,l4_hud_height,l4_steel_light,l4_steel_dark);
    DrawRectangle(0,l4_hud_height-4*l4_u,l4_width,4*l4_u,BLACK);
    int rivets = l4_width/(40*l4_u);
    for (int i=0; i<rivets; i++)
    {
        DrawCircle(20*l4_u+i*40*l4_u,8*l4_u,3*l4_u,l4_steel_dark);
        DrawCircle(20*l4_u+i*40*l4_u,l4_hud_height-12*l4_u,3*l4_u,l4_steel_dark);
    }

    //text
    DrawText("LEVEL 4 - LOST TEMPLE",32*l4_u,20*l4_u,36*l4_u,BLACK);
    DrawText("LEVEL 4 - LOST TEMPLE",30*l4_u,18*l4_u,36*l4_u,l4_hazard_yellow);
    DrawText(TextFormat("STROKES %d / %d",l4_stroke,l4_stroke_limit),l4_width/2-298*l4_u,21*l4_u,32*l4_u,BLACK);
    DrawText(TextFormat("STROKES %d / %d",l4_stroke,l4_stroke_limit),l4_width/2-300*l4_u,19*l4_u,32*l4_u,WHITE);
    for (int i=0; i<l4_stroke_limit; i++)
    {
        Color pip = l4_steel_dark;
        if (i<l4_stroke) pip = l4_laser_red;
        DrawCircle(l4_width/2+10*l4_u+i*22*l4_u,35*l4_u,7*l4_u,pip);
        DrawCircleLines(l4_width/2+10*l4_u+i*22*l4_u,35*l4_u,7*l4_u,BLACK);
    }
    int help_width = MeasureText("R restart   ESC menu",26*l4_u);
    DrawText("R restart   ESC menu",l4_width-help_width-28*l4_u,24*l4_u,26*l4_u,BLACK);
    DrawText("R restart   ESC menu",l4_width-help_width-30*l4_u,22*l4_u,26*l4_u,WHITE);

    //chomp, darted or sunk message
    if (l4_message_timer>0)
    {
        if (l4_message_type==1)
        {
            DrawText("CHOMP!",l4_width/2-MeasureText("CHOMP!",90*l4_u)/2+4*l4_u,l4_height/2-41*l4_u,90*l4_u,Fade(BLACK,l4_message_timer));
            DrawText("CHOMP!",l4_width/2-MeasureText("CHOMP!",90*l4_u)/2,l4_height/2-45*l4_u,90*l4_u,Fade(GREEN,l4_message_timer));
        }
        else if (l4_message_type==2)
        {
            DrawText("DARTED!",l4_width/2-MeasureText("DARTED!",90*l4_u)/2+4*l4_u,l4_height/2-41*l4_u,90*l4_u,Fade(BLACK,l4_message_timer));
            DrawText("DARTED!",l4_width/2-MeasureText("DARTED!",90*l4_u)/2,l4_height/2-45*l4_u,90*l4_u,Fade(l4_laser_red,l4_message_timer));
        }
        else
        {
            DrawText("SUNK!",l4_width/2-MeasureText("SUNK!",90*l4_u)/2+4*l4_u,l4_height/2-41*l4_u,90*l4_u,Fade(BLACK,l4_message_timer));
            DrawText("SUNK!",l4_width/2-MeasureText("SUNK!",90*l4_u)/2,l4_height/2-45*l4_u,90*l4_u,Fade(l4_gold,l4_message_timer));
        }
    }

    //level clear or failed
    if (l4_game_state!=0)
    {
        DrawRectangle(0,0,l4_width,l4_height,Fade(BLACK,0.6));
        Rectangle panel = {l4_width/2-340*l4_u,l4_height/2-170*l4_u,680*l4_u,340*l4_u};
        DrawRectangleRec(panel,l4_steel_dark);
        Rectangle panel_band = {panel.x,panel.y,panel.width,24*l4_u};
        l4_draw_hazard_stripes(panel_band);
        DrawRectangleLinesEx(panel,6*l4_u,l4_steel_light);
        if (l4_game_state==1)
        {
            DrawText("LEVEL CLEAR!",l4_width/2-MeasureText("LEVEL CLEAR!",80*l4_u)/2,panel.y+60*l4_u,80*l4_u,l4_hazard_yellow);
        }
        else
        {
            DrawText("FAILED",l4_width/2-MeasureText("FAILED",80*l4_u)/2,panel.y+60*l4_u,80*l4_u,l4_laser_red);
        }
        int strokes_width = MeasureText(TextFormat("Strokes: %d / %d",l4_stroke,l4_stroke_limit),40*l4_u);
        DrawText(TextFormat("Strokes: %d / %d",l4_stroke,l4_stroke_limit),l4_width/2-strokes_width/2,panel.y+170*l4_u,40*l4_u,WHITE);
        DrawText("R play again   ESC menu",l4_width/2-MeasureText("R play again   ESC menu",30*l4_u)/2,panel.y+250*l4_u,30*l4_u,l4_steel_light);
    }
}


//level 4: keep the obstacles moving with no ball (intro and menu)
void l4_background_step(float dt)
{
    l4_animation_time = l4_animation_time + dt;
    l4_update_obstacles(dt);
}


//level 4: draw everything except the scoreboard (intro and menu)
void l4_draw_scene()
{

    l4_draw_ground();
    l4_draw_doors();
    l4_draw_walls();
    l4_draw_darts();
    l4_draw_foliage();
    l4_draw_obstacles();
    l4_draw_ball_and_pot();

}


//level 4: set the screen size, load pictures, start the level
void l4_start(int screen_width, int screen_height)
{
    l4_width = screen_width;
    l4_height = screen_height;

    //pictures (made at 2x size; water, foam and splash come from the beach level, boulders from space)
    l4_ground_texture = LoadTexture("assets/jungle/jungle_ground.png");
    l4_temple_texture = LoadTexture("assets/jungle/jungle_temple_floor.png");
    l4_foliage_texture = LoadTexture("assets/jungle/jungle_foliage.png");
    l4_plank_texture = LoadTexture("assets/jungle/jungle_plank.png");
    l4_water_texture = LoadTexture("assets/beach/beach_water_tile.png");
    l4_foam_texture = LoadTexture("assets/beach/beach_foam_strip.png");
    l4_splash_texture = LoadTexture("assets/beach/beach_splash.png");
    l4_boulder_texture = LoadTexture("assets/space/space_asteroid.png");
    SetTextureWrap(l4_ground_texture,TEXTURE_WRAP_REPEAT);
    SetTextureWrap(l4_temple_texture,TEXTURE_WRAP_REPEAT);
    SetTextureWrap(l4_foam_texture,TEXTURE_WRAP_REPEAT);
    SetTextureFilter(l4_ground_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l4_temple_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l4_foliage_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l4_plank_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l4_water_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l4_foam_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l4_splash_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l4_boulder_texture,TEXTURE_FILTER_BILINEAR);

    //every size is N*l4_u, so it looks the same on any screen
    l4_u = l4_height/1080.0;
    if (l4_width/1920.0 < l4_u) l4_u = l4_width/1920.0;
    l4_reset_level();

}


//level 4: one frame of input and movement
void l4_update()
{
    float dt = GetFrameTime();
    if (dt>1.0/30) dt = 1.0/30;
    l4_animation_time = l4_animation_time + dt;
    if (l4_message_timer>0) l4_message_timer = l4_message_timer - dt;

    //restart
    if (IsKeyPressed(KEY_R)) l4_reset_level();

    //shooting (the click has to start while the l4_ball is still)
    int ball_stopped = 0;
    if (l4_speed.x==0 && l4_speed.y==0) ball_stopped = 1;
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && ball_stopped==1 && l4_game_state==0) l4_aiming = 1;
    if (l4_game_state!=0) l4_aiming = 0;
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && l4_aiming==1)
    {
        l4_aiming = 0;
        Vector2 mouse = {GetMouseX(),GetMouseY()};
        Vector2 drag = Vector2Subtract(l4_ball,mouse);
        if (ball_stopped==1 && l4_stroke<l4_stroke_limit && Vector2Length(drag)>=2*l4_radius_ball)
        {
            l4_last_shot_position = l4_ball;
            l4_speed = Vector2ClampValue(Vector2Scale(drag,3.125),0,l4_max_speed*l4_u);
            l4_stroke++;
        }
    }

    //moving everything in 4 small steps, so nothing jumps through anything
    for (int i=0; i<4; i++)
    {
        l4_update_obstacles(dt/4*obstacle_speed);
        if (l4_game_state==0) l4_update_ball(dt/4);
    }

    //out of strokes
    if (l4_game_state==0 && l4_stroke>=l4_stroke_limit && l4_speed.x==0 && l4_speed.y==0) l4_game_state = 2;


}


//level 4: one frame of drawing
void l4_draw()
{

    l4_draw_ground();
    l4_draw_doors();
    l4_draw_walls();
    l4_draw_darts();
    l4_draw_foliage();
    l4_draw_obstacles();
    l4_draw_ball_and_pot();
    draw_straight_preview(l4_ball,l4_aiming,l4_radius_ball,l4_max_speed*l4_u,l4_u);
    l4_draw_hud();

}


void l4_unload()
{
    UnloadTexture(l4_ground_texture);
    UnloadTexture(l4_temple_texture);
    UnloadTexture(l4_foliage_texture);
    UnloadTexture(l4_plank_texture);
    UnloadTexture(l4_water_texture);
    UnloadTexture(l4_foam_texture);
    UnloadTexture(l4_splash_texture);
    UnloadTexture(l4_boulder_texture);
}
