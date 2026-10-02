//==================== LEVEL 2 ====================

//full global
int l2_width = 1920;
int l2_height = 1080;
float l2_u = 1;
int l2_stroke_base = 25;
int l2_stroke_limit = 25;
#define l2_max_speed 650

//colours
Color l2_sand_light = {245,232,200,255};
Color l2_wood = {139,94,52,255};
Color l2_wood_dark = {92,60,32,255};
Color l2_wood_light = {186,138,88,255};
Color l2_sun_yellow = {255,204,64,255};
Color l2_coral_red = {235,87,70,255};
Color l2_sea = {63,180,207,255};
Color l2_sea_deep = {26,127,168,255};

//pictures
Texture2D l2_sand_texture;
Texture2D l2_water_texture;
Texture2D l2_foam_texture;
Texture2D l2_crab_texture;
Texture2D l2_palm_texture;
Texture2D l2_splash_texture;

//l2_ball and l2_pot
Vector2 l2_ball;
Vector2 l2_speed;
float l2_radius_ball;
Vector2 l2_pot;
float l2_radius_pot;
Vector2 l2_start_position;
Vector2 l2_last_shot_position;
int l2_stroke = 0;
int l2_game_state = 0;
int l2_aiming = 0;
float l2_message_timer = 0;
float l2_animation_time = 0;
float l2_hud_height;

//ground: l2_sand is safe, everything else is water
Rectangle l2_sand[10];
Rectangle l2_soft_sand[4];
Rectangle l2_wet_sand[2];

//l2_tidal l2_sand bars, safe only at low tide (tide clock: low 3s, rising 1s, high 3s, falling 1s)
Rectangle l2_tidal[2];
Vector2 l2_tidal_push[2];
float l2_tide_timer = 0;

//rip current in shallow water (the old conveyor belt)
Rectangle l2_ford;
Vector2 l2_ford_push;

//whirlpools (the old magnet)
Vector2 l2_whirlpool[2];
float l2_whirlpool_field[2];
float l2_whirlpool_pull;

//crabs (the old crane beams)
Rectangle l2_crab[4];
float l2_crab_speed[4];
float l2_crab_left_limit[4];
float l2_crab_right_limit[4];
float l2_crab_angry[4];

//l2_palm trees and beach balls
Vector2 l2_palm[4];
float l2_trunk_radius;
Vector2 l2_beach_ball[4];
float l2_beach_ball_radius;
float l2_beach_ball_hit[4];

//splash
Vector2 l2_splash_position;
float l2_splash_timer = 0;


//a rectangle in design units (a 1920 x 1080 screen), made to fit the real screen
Rectangle l2_make_rect(float x, float y, float w, float h)
{
    Rectangle rec = {x*l2_u,y*l2_u,w*l2_u,h*l2_u};
    return rec;
}

//a point in design units
Vector2 l2_make_point(float x, float y)
{
    Vector2 point = {x*l2_u,y*l2_u};
    return point;
}


void l2_reset_level()
{
    //sizes
    l2_hud_height = 70*l2_u;
    l2_radius_ball = 7*l2_u;
    l2_radius_pot = 11*l2_u;

    //l2_sand islands and strips, the path goes up and down from left to right
    l2_sand[0] = l2_make_rect(60,760,340,270);     //start island
    l2_sand[1] = l2_make_rect(140,330,170,440);    //strip going up
    l2_sand[2] = l2_make_rect(60,110,520,230);     //top left island
    l2_sand[3] = l2_make_rect(450,640,200,390);    //bottom middle island
    l2_sand[4] = l2_make_rect(640,920,330,110);    //its narrow neck
    l2_sand[5] = l2_make_rect(860,330,110,600);    //strip going up (rip current in the middle)
    l2_sand[6] = l2_make_rect(800,110,600,230);    //top middle island
    l2_sand[7] = l2_make_rect(1310,330,70,380);    //narrow bridge going down
    l2_sand[8] = l2_make_rect(1100,700,760,330);   //bottom right island
    l2_sand[9] = l2_make_rect(1480,110,380,280);   //last island with the l2_pot

    l2_soft_sand[0] = l2_make_rect(140,470,170,90);
    l2_soft_sand[1] = l2_make_rect(1230,820,220,150);
    l2_soft_sand[2] = l2_make_rect(1590,230,130,110);
    l2_soft_sand[3] = l2_make_rect(860,360,110,140);    //just past the rip current, so the l2_ball stops before the island
    l2_wet_sand[0] = l2_make_rect(360,118,210,95);
    l2_wet_sand[1] = l2_make_rect(1600,940,250,80);

    //l2_tidal bars, the rising tide pushes the l2_ball back towards land
    l2_tidal[0] = l2_make_rect(470,330,100,320);
    l2_tidal_push[0] = l2_make_point(0,-400);
    l2_tidal[1] = l2_make_rect(1700,380,110,330);
    l2_tidal_push[1] = l2_make_point(0,400);
    l2_tide_timer = 0;

    //rip current across the strip, pushing to the right
    l2_ford = l2_make_rect(860,520,110,220);
    l2_ford_push = l2_make_point(260,0);

    //whirlpools
    l2_whirlpool[0] = l2_make_point(730,820);
    l2_whirlpool_field[0] = 125*l2_u;
    l2_whirlpool[1] = l2_make_point(1450,560);
    l2_whirlpool_field[1] = 130*l2_u;
    l2_whirlpool_pull = 380*l2_u;

    //crabs walking left and right
    l2_crab[0] = l2_make_rect(160,625,44,30);
    l2_crab_left_limit[0] = 140*l2_u;
    l2_crab_right_limit[0] = 266*l2_u;
    l2_crab_speed[0] = 110*l2_u;
    l2_crab[1] = l2_make_rect(1100,125,44,30);
    l2_crab_left_limit[1] = 1010*l2_u;
    l2_crab_right_limit[1] = 1356*l2_u;
    l2_crab_speed[1] = 130*l2_u;
    l2_crab[2] = l2_make_rect(900,290,44,30);
    l2_crab_left_limit[2] = 880*l2_u;
    l2_crab_right_limit[2] = 1250*l2_u;
    l2_crab_speed[2] = -130*l2_u;
    l2_crab[3] = l2_make_rect(1500,745,44,30);
    l2_crab_left_limit[3] = 1450*l2_u;
    l2_crab_right_limit[3] = 1816*l2_u;
    l2_crab_speed[3] = 150*l2_u;

    //l2_palm trees (only the trunk is solid) and beach balls
    l2_trunk_radius = 14*l2_u;
    l2_palm[0] = l2_make_point(300,930);
    l2_palm[1] = l2_make_point(220,200);
    l2_palm[2] = l2_make_point(1180,225);
    l2_palm[3] = l2_make_point(1790,300);
    l2_beach_ball_radius = 24*l2_u;
    l2_beach_ball[0] = l2_make_point(330,800);
    l2_beach_ball[1] = l2_make_point(1000,225);
    l2_beach_ball[2] = l2_make_point(1700,160);
    l2_beach_ball[3] = l2_make_point(1545,330);
    for (int i=0; i<4; i++)
    {
        l2_crab_angry[i] = 0;
        l2_beach_ball_hit[i] = 0;
    }

    //l2_ball and l2_pot
    l2_start_position = l2_make_point(130,990);
    l2_ball = l2_start_position;
    l2_last_shot_position = l2_start_position;
    l2_speed.x = 0;
    l2_speed.y= 0;
    l2_pot = l2_make_point(1560,200);
    l2_stroke = 0;
    l2_game_state = 0;
    l2_aiming = 0;
    l2_message_timer = 0;
    l2_splash_timer = 0;
}

//bounce off a straight rectangle, rec_speed is how fast the rectangle itself is moving
int l2_bounce_off_rectangle(Rectangle rec, Vector2 rec_speed)
{
    if (CheckCollisionCircleRec(l2_ball,l2_radius_ball,rec))
    {
        Vector2 collision_point;
        collision_point.x = Clamp(l2_ball.x,rec.x,rec.x+rec.width);
        collision_point.y = Clamp(l2_ball.y,rec.y,rec.y+rec.height);
        Vector2 normal = Vector2Subtract(l2_ball,collision_point);

        //centre is inside, go out through the nearest side
        if (normal.x==0 && normal.y==0)
        {
            float left = l2_ball.x - rec.x;
            float right = rec.x + rec.width - l2_ball.x;
            float top = l2_ball.y - rec.y;
            float bottom = rec.y + rec.height - l2_ball.y;
            if (left<=right && left<=top && left<=bottom) { normal.x = -1; collision_point.x = rec.x; }
            else if (right<=top && right<=bottom) { normal.x = 1; collision_point.x = rec.x + rec.width; }
            else if (top<=bottom) { normal.y = -1; collision_point.y = rec.y; }
            else { normal.y = 1; collision_point.y = rec.y + rec.height; }
        }
        normal = Vector2Normalize(normal);
        l2_ball = Vector2Add(collision_point,Vector2Scale(normal,l2_radius_ball));

        //reflect only if the l2_ball is going into it
        Vector2 relative_speed = Vector2Subtract(l2_speed,rec_speed);
        if (Vector2DotProduct(relative_speed,normal)<0)
        {
            relative_speed = Vector2Reflect(relative_speed,normal);
            l2_speed = Vector2Add(relative_speed,rec_speed);

            //moving l2_crab side: also knock the l2_ball out of its row, or it gets hit again and again
            if (rec_speed.x!=0 && normal.y==0)
            {
                if (l2_ball.y < rec.y+rec.height/2) l2_speed.y = l2_speed.y - fabsf(rec_speed.x)/2;
                else l2_speed.y = l2_speed.y + fabsf(rec_speed.x)/2;
            }
            l2_speed = Vector2ClampValue(l2_speed,0,l2_max_speed*l2_u);
        }
        return 1;
    }
    return 0;
}

//bounce off a round thing that doesn't move, bounce is 1 for a normal one and more than 1 for a bouncy one
int l2_bounce_off_circle(Vector2 center, float radius, float bounce)
{
    Vector2 normal = Vector2Subtract(l2_ball,center);
    float distance = Vector2Length(normal);
    if (distance < radius + l2_radius_ball)
    {
        if (distance==0) normal.y = -1;
        normal = Vector2Normalize(normal);
        l2_ball = Vector2Add(center,Vector2Scale(normal,radius + l2_radius_ball));
        if (Vector2DotProduct(l2_speed,normal)<0)
        {
            l2_speed = Vector2ClampValue(Vector2Scale(Vector2Reflect(l2_speed,normal),bounce),0,l2_max_speed*l2_u);
            return 1;
        }
    }
    return 0;
}


//is this point on l2_sand (or shallow water), and not on a l2_tidal bar at high tide
int l2_on_safe_ground(Vector2 point)
{
    for (int i=0; i<10; i++)
    {
        if (CheckCollisionPointRec(point,l2_sand[i])) return 1;
    }
    if (CheckCollisionPointRec(point,l2_ford)) return 1;
    if (l2_tide_timer<3.5 || l2_tide_timer>=7.5)
    {
        for (int i=0; i<2; i++)
        {
            if (CheckCollisionPointRec(point,l2_tidal[i])) return 1;
        }
    }
    return 0;
}


void l2_update_obstacles(float dt)
{
    //crabs walking (the old crane beams)
    for (int i=0; i<4; i++)
    {
        l2_crab[i].x = l2_crab[i].x + l2_crab_speed[i]*dt;
        if (l2_crab[i].x < l2_crab_left_limit[i])
        {
            l2_crab[i].x = l2_crab_left_limit[i];
            l2_crab_speed[i] = fabsf(l2_crab_speed[i]);
        }
        if (l2_crab[i].x > l2_crab_right_limit[i])
        {
            l2_crab[i].x = l2_crab_right_limit[i];
            l2_crab_speed[i] = -fabsf(l2_crab_speed[i]);
        }
        if (l2_crab_angry[i]>0) l2_crab_angry[i] = l2_crab_angry[i] - dt;
        if (l2_beach_ball_hit[i]>0) l2_beach_ball_hit[i] = l2_beach_ball_hit[i] - dt;
    }

    //tide clock
    l2_tide_timer = l2_tide_timer + dt;
    if (l2_tide_timer>=8) l2_tide_timer = l2_tide_timer - 8;

    if (l2_splash_timer>0) l2_splash_timer = l2_splash_timer - dt;
}


//after falling in the water, the l2_ball goes back to where it was shot from
void l2_send_ball_back()
{
    l2_ball = l2_last_shot_position;

    //not right in a l2_crab's path, or it gets knocked into the water again and again
    for (int i=0; i<4; i++)
    {
        if (l2_ball.x>l2_crab_left_limit[i]-l2_radius_ball && l2_ball.x<l2_crab_right_limit[i]+l2_crab[i].width+l2_radius_ball && l2_ball.y>l2_crab[i].y-l2_radius_ball-1*l2_u && l2_ball.y<l2_crab[i].y+l2_crab[i].height+l2_radius_ball+1*l2_u)
        {
            if (l2_ball.y < l2_crab[i].y+l2_crab[i].height/2) l2_ball.y = l2_crab[i].y - l2_radius_ball - 2*l2_u;
            else l2_ball.y = l2_crab[i].y + l2_crab[i].height + l2_radius_ball + 2*l2_u;
        }
    }

    //the old spot is under water now (tide), start again
    if (l2_on_safe_ground(l2_ball)==0) l2_ball = l2_start_position;

    l2_speed.x = 0;
    l2_speed.y = 0;
}


void l2_update_ball(float dt)
{
    int pushed = 0;

    //rip current (the old conveyor belt)
    if (CheckCollisionPointRec(l2_ball,l2_ford))
    {
        l2_speed = Vector2Add(l2_speed,Vector2Scale(l2_ford_push,dt));
        pushed = 1;
    }

    //rising tide gently pushes the l2_ball back towards land
    if (l2_tide_timer>=3 && l2_tide_timer<4)
    {
        for (int i=0; i<2; i++)
        {
            int on_sand = 0;
            for (int j=0; j<10; j++)
            {
                if (CheckCollisionPointRec(l2_ball,l2_sand[j])) on_sand = 1;
            }
            if (on_sand==0 && CheckCollisionPointRec(l2_ball,l2_tidal[i]))
            {
                if (Vector2Length(l2_speed)<150*l2_u) l2_speed = Vector2Add(l2_speed,Vector2Scale(l2_tidal_push[i],dt));
                pushed = 1;
            }
        }
    }

    //l2_whirlpool pull (the old magnet)
    for (int i=0; i<2; i++)
    {
        Vector2 to_whirlpool = Vector2Subtract(l2_whirlpool[i],l2_ball);
        float whirlpool_distance = Vector2Length(to_whirlpool);
        if (whirlpool_distance<l2_whirlpool_field[i] && whirlpool_distance>1*l2_u)
        {
            l2_speed = Vector2Add(l2_speed,Vector2Scale(Vector2Normalize(to_whirlpool),l2_whirlpool_pull*dt));
            pushed = 1;
        }
    }

    //moving
    l2_ball = Vector2Add(l2_ball,Vector2Scale(l2_speed,dt));

    //proportional deceleration (friction depends on the ground)
    float friction = 110*l2_u;
    for (int i=0; i<4; i++)
    {
        if (CheckCollisionPointRec(l2_ball,l2_soft_sand[i])) friction = 400*l2_u;
    }
    for (int i=0; i<2; i++)
    {
        if (CheckCollisionPointRec(l2_ball,l2_wet_sand[i])) friction = 20*l2_u;
    }
    if (CheckCollisionPointRec(l2_ball,l2_ford)) friction = 250*l2_u;
    float ball_speed = Vector2Length(l2_speed);
    if (ball_speed>0)
    {
        float new_speed = ball_speed - friction*dt;
        if (new_speed<0) new_speed = 0;
        l2_speed = Vector2Scale(l2_speed,new_speed/ball_speed);
    }
    if (pushed==0 && Vector2Length(l2_speed)<6*l2_u)
    {
        l2_speed.x = 0;
        l2_speed.y= 0;
    }

    //water (checked before the collisions, so if something is on the old spot it pushes the l2_ball out)
    if (l2_on_safe_ground(l2_ball)==0)
    {
        l2_splash_position = l2_ball;
        l2_splash_timer = 0.5;
        l2_send_ball_back();
        l2_message_timer = 1;
    }

    //l2_palm trunks and beach balls
    for (int i=0; i<4; i++)
    {
        l2_bounce_off_circle(l2_palm[i],l2_trunk_radius,1);
        if (l2_bounce_off_circle(l2_beach_ball[i],l2_beach_ball_radius,1.3)) l2_beach_ball_hit[i] = 0.15;
    }

    //crabs
    for (int i=0; i<4; i++)
    {
        Vector2 crab_velocity = {l2_crab_speed[i],0};
        if (l2_bounce_off_rectangle(l2_crab[i],crab_velocity)) l2_crab_angry[i] = 0.4;
    }

    //score
    if ((l2_ball.x>l2_pot.x-3*l2_radius_pot/4) && (l2_ball.x<l2_pot.x+3*l2_radius_pot/4) && (l2_ball.y>l2_pot.y-3*l2_radius_pot/4) && (l2_ball.y<l2_pot.y+3*l2_radius_pot/4))
    {
        l2_ball = l2_pot;
        l2_speed.x = 0;
        l2_speed.y= 0;
        l2_game_state = 1;
    }
}


//l2_sand picture that repeats, lined up with the screen so pieces join without seams (the picture is 2x size)
void l2_draw_sand(Rectangle rec, Color tint)
{
    Rectangle source = {rec.x/l2_u*2,rec.y/l2_u*2,rec.width/l2_u*2,rec.height/l2_u*2};
    Vector2 no_origin = {0,0};
    DrawTexturePro(l2_sand_texture,source,rec,no_origin,0,tint);
}


//foam along the 4 edges of a piece of ground, the bumpy side faces the water
void l2_draw_foam(Rectangle rec, Color tint)
{
    int frame = (int)(l2_animation_time*8)%4;
    float long_side = rec.width + 40*l2_u;
    Rectangle source = {rec.x/l2_u*2,frame*128+1,long_side/l2_u*2,126};
    Vector2 origin = {long_side/2,32*l2_u};

    //top and bottom edges (top one turned around)
    Rectangle top = {rec.x+rec.width/2,rec.y-18*l2_u,long_side,64*l2_u};
    DrawTexturePro(l2_foam_texture,source,top,origin,180,tint);
    Rectangle bottom = {rec.x+rec.width/2,rec.y+rec.height+18*l2_u,long_side,64*l2_u};
    DrawTexturePro(l2_foam_texture,source,bottom,origin,0,tint);

    //left and right edges
    long_side = rec.height + 40*l2_u;
    source.x = rec.y/l2_u*2;
    source.width = long_side/l2_u*2;
    origin.x = long_side/2;
    Rectangle left = {rec.x-18*l2_u,rec.y+rec.height/2,long_side,64*l2_u};
    DrawTexturePro(l2_foam_texture,source,left,origin,90,tint);
    Rectangle right = {rec.x+rec.width+18*l2_u,rec.y+rec.height/2,long_side,64*l2_u};
    DrawTexturePro(l2_foam_texture,source,right,origin,-90,tint);
}


void l2_draw_ground()
{
    float t = l2_animation_time;
    Vector2 no_origin = {0,0};

    //water tiles, 2 pictures swapping for the shimmer
    int water_frame = (int)(t*2)%2;
    Rectangle water_source = {water_frame*512,0,512,512};
    int columns = l2_width/(256*l2_u) + 1;
    int rows = l2_height/(256*l2_u) + 1;
    for (int i=0; i<columns; i++)
    {
        for (int j=0; j<rows; j++)
        {
            Rectangle tile = {i*256*l2_u,j*256*l2_u,256*l2_u+1,256*l2_u+1};
            DrawTexturePro(l2_water_texture,water_source,tile,no_origin,0,WHITE);
        }
    }

    //whirlpools
    for (int i=0; i<2; i++)
    {
        Vector2 w = l2_whirlpool[i];
        DrawCircleGradient(w,l2_whirlpool_field[i],Fade(l2_sea_deep,0.7),Fade(l2_sea,0));
        for (int j=0; j<4; j++)
        {
            float ring = 18*l2_u + j*24*l2_u;
            float angle = t*(320 - j*60) + j*70;
            DrawRing(w,ring-2*l2_u,ring+2*l2_u,angle,angle+230,24,Fade(WHITE,0.55-j*0.1));
        }
        DrawCircle(w.x,w.y,10*l2_u,l2_sea_deep);
    }

    //how high the tide is, 0 low and 1 high
    float tide_level = 0;
    if (l2_tide_timer>=3 && l2_tide_timer<4) tide_level = l2_tide_timer - 3;
    if (l2_tide_timer>=4 && l2_tide_timer<7) tide_level = 1;
    if (l2_tide_timer>=7) tide_level = 8 - l2_tide_timer;

    //foam first, then the ground on top, so the foam only shows on the water side
    for (int i=0; i<10; i++) l2_draw_foam(l2_sand[i],Fade(WHITE,0.85));
    for (int i=0; i<2; i++) l2_draw_foam(l2_tidal[i],Fade(WHITE,0.85*(1-tide_level)));

    //l2_tidal bars, the water comes over them
    Color wet = {222,196,150,255};
    for (int i=0; i<2; i++)
    {
        l2_draw_sand(l2_tidal[i],wet);
        DrawRectangleRec(l2_tidal[i],Fade(l2_sea,tide_level*0.9));
    }

    for (int i=0; i<10; i++) l2_draw_sand(l2_sand[i],WHITE);

    //soft l2_sand, darker with wind ripples
    Color soft = {226,196,140,255};
    for (int i=0; i<4; i++)
    {
        Rectangle s = l2_soft_sand[i];
        l2_draw_sand(s,soft);
        int ripple_rows = s.height/(14*l2_u);
        int pieces = s.width/(24*l2_u);
        for (int j=1; j<ripple_rows; j++)
        {
            float shift = 0;
            if (j%2==1) shift = 12*l2_u;
            for (int k=0; k<pieces; k++)
            {
                Vector2 a = {s.x + k*24*l2_u + shift, s.y + j*14*l2_u};
                Vector2 b = {a.x + 14*l2_u, a.y - 3*l2_u};
                if (b.x < s.x+s.width) DrawLineEx(a,b,2*l2_u,Fade(l2_wood,0.35));
            }
        }
        DrawRectangleLinesEx(s,2*l2_u,Fade(l2_wood,0.25));
    }

    //wet l2_sand, dark and shiny
    for (int i=0; i<2; i++)
    {
        Rectangle w = l2_wet_sand[i];
        Color wet_dark = {196,160,112,255};
        l2_draw_sand(w,wet_dark);
        for (int k=0; k<14; k++)
        {
            float shine_x = w.x + fmod(k*53*l2_u,w.width);
            float shine_y = w.y + fmod(k*31*l2_u + 7*l2_u,w.height);
            DrawCircle(shine_x,shine_y,2.5*l2_u,Fade(WHITE,0.25+0.25*sin(t*3+k)));
        }
    }

    //rip current, shallow water with foam lines moving the way it pushes
    Color shallow = {170,215,210,255};
    l2_draw_sand(l2_ford,shallow);
    DrawRectangleRec(l2_ford,Fade(l2_sea,0.45));
    int current_lines = l2_ford.height/(22*l2_u);
    for (int j=0; j<current_lines; j++)
    {
        Vector2 a = {l2_ford.x + fmod(t*120*l2_u + j*37*l2_u,l2_ford.width), l2_ford.y + 11*l2_u + j*22*l2_u};
        Vector2 b = {a.x + 18*l2_u, a.y};
        if (b.x > l2_ford.x+l2_ford.width) b.x = l2_ford.x + l2_ford.width;
        DrawLineEx(a,b,2*l2_u,Fade(WHITE,0.6));
    }
}


void l2_draw_obstacles()
{
    float t = l2_animation_time;

    //start towel
    Rectangle towel = {l2_start_position.x-45*l2_u,l2_start_position.y-28*l2_u,90*l2_u,56*l2_u};
    DrawRectangle(towel.x+4*l2_u,towel.y+5*l2_u,towel.width,towel.height,Fade(BLACK,0.2));
    for (int j=0; j<6; j++)
    {
        Color stripe = l2_coral_red;
        if (j%2==1) stripe = RAYWHITE;
        DrawRectangle(towel.x+j*15*l2_u,towel.y,15*l2_u,towel.height,stripe);
    }
    DrawText("START",l2_start_position.x-MeasureText("START",16*l2_u)/2,towel.y+4*l2_u,16*l2_u,l2_wood_dark);

    //splash, 5 pictures in half a second
    if (l2_splash_timer>0)
    {
        int frame = (0.5-l2_splash_timer)/0.1;
        if (frame>4) frame = 4;
        Rectangle source = {frame*192,0,192,192};
        Rectangle dest = {l2_splash_position.x,l2_splash_position.y,96*l2_u,96*l2_u};
        Vector2 origin = {48*l2_u,48*l2_u};
        DrawTexturePro(l2_splash_texture,source,dest,origin,0,WHITE);
    }

    //crabs, walking pictures 1-4, picture 5 when angry
    for (int i=0; i<4; i++)
    {
        Vector2 middle = {l2_crab[i].x+l2_crab[i].width/2,l2_crab[i].y+l2_crab[i].height/2};
        DrawCircle(middle.x+3*l2_u,middle.y+6*l2_u,18*l2_u,Fade(BLACK,0.18));
        int frame = (int)(t*8)%4;
        if (l2_crab_angry[i]>0) frame = 4;
        Rectangle source = {frame*128,0,128,128};
        Rectangle dest = {middle.x,middle.y-2*l2_u,64*l2_u,64*l2_u};
        Vector2 origin = {32*l2_u,32*l2_u};
        DrawTexturePro(l2_crab_texture,source,dest,origin,0,WHITE);
    }

    //beach balls, they get bigger for a moment when hit
    for (int i=0; i<4; i++)
    {
        Vector2 b = l2_beach_ball[i];
        float r = l2_beach_ball_radius;
        if (l2_beach_ball_hit[i]>0) r = r*(1 + l2_beach_ball_hit[i]);
        DrawCircle(b.x+3*l2_u,b.y+5*l2_u,r,Fade(BLACK,0.22));
        Color slice[6] = {l2_coral_red,RAYWHITE,l2_sea_deep,l2_sun_yellow,RAYWHITE,LIME};
        for (int j=0; j<6; j++)
        {
            DrawCircleSector(b,r,t*40+j*60,t*40+j*60+60,8,slice[j]);
        }
        DrawCircle(b.x,b.y,5*l2_u,RAYWHITE);
        DrawCircle(b.x-8*l2_u,b.y-9*l2_u,5*l2_u,Fade(WHITE,0.45));
        DrawCircleLines(b.x,b.y,r,Fade(BLACK,0.35));
    }

    //l2_palm tree shadows (the leaves are drawn after the l2_ball)
    int palm_frame = (int)(t/0.8)%2;
    for (int i=0; i<4; i++)
    {
        Rectangle source = {palm_frame*384,0,384,384};
        Rectangle dest = {l2_palm[i].x+14*l2_u,l2_palm[i].y+18*l2_u,192*l2_u,192*l2_u};
        Vector2 origin = {96*l2_u,96*l2_u};
        DrawTexturePro(l2_palm_texture,source,dest,origin,0,Fade(BLACK,0.22));
    }
}

void l2_draw_ball_and_pot()
{
    //l2_pot with striped ring
    for (int i=0; i<8; i++)
    {
        Color colour = WHITE;
        if (i%2==1) colour = l2_coral_red;
        DrawRing(l2_pot,l2_radius_pot+6*l2_u,l2_radius_pot+12*l2_u,i*45,i*45+45,6,colour);
    }
    DrawCircle(l2_pot.x,l2_pot.y,l2_radius_pot+6*l2_u,l2_sand_light);
    DrawRing(l2_pot,l2_radius_pot+1*l2_u,l2_radius_pot+5*l2_u,0,360,24,l2_wood);
    DrawCircle(l2_pot.x,l2_pot.y,l2_radius_pot,BLACK);

    //flag
    Vector2 pole_top = {l2_pot.x,l2_pot.y-60*l2_u};
    DrawLineEx(l2_pot,pole_top,3*l2_u,l2_sand_light);
    for (int i=0; i<5; i++)
    {
        float wave = sin(l2_animation_time*6 - i*0.8)*2.5*l2_u;
        DrawRectangle(l2_pot.x+1*l2_u+i*6*l2_u,pole_top.y+wave,6*l2_u,18*l2_u,l2_coral_red);
    }

    //l2_ball
    DrawCircle(l2_ball.x+3*l2_u,l2_ball.y+4*l2_u,l2_radius_ball,Fade(BLACK,0.45));
    DrawCircle(l2_ball.x,l2_ball.y,l2_radius_ball,GetColor(0xEDEDEDFF));
    DrawCircleLines(l2_ball.x,l2_ball.y,l2_radius_ball,GRAY);
    DrawCircle(l2_ball.x-2*l2_u,l2_ball.y-2*l2_u,2*l2_u,WHITE);
}


//l2_palm leaves go over the l2_ball, so it hides under them (the aim line still shows)
void l2_draw_palm_leaves()
{
    int palm_frame = (int)(l2_animation_time/0.8)%2;
    for (int i=0; i<4; i++)
    {
        Rectangle source = {palm_frame*384,0,384,384};
        Rectangle dest = {l2_palm[i].x,l2_palm[i].y,192*l2_u,192*l2_u};
        Vector2 origin = {96*l2_u,96*l2_u};
        DrawTexturePro(l2_palm_texture,source,dest,origin,0,WHITE);
    }

    //aim line
    if (l2_aiming==1 && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        Vector2 mouse = {GetMouseX(),GetMouseY()};
        DrawLineEx(l2_ball,mouse,4*l2_u,Fade(WHITE,0.75));
        DrawCircle(mouse.x,mouse.y,5*l2_u,Fade(WHITE,0.75));
    }
}


void l2_draw_hud()
{
    //dark edges
    DrawRectangleGradientH(0,l2_hud_height,90*l2_u,l2_height-l2_hud_height,Fade(BLACK,0.25),BLANK);
    DrawRectangleGradientH(l2_width-90*l2_u,l2_hud_height,90*l2_u,l2_height-l2_hud_height,BLANK,Fade(BLACK,0.25));
    DrawRectangleGradientV(0,l2_height-70*l2_u,l2_width,70*l2_u,BLANK,Fade(BLACK,0.25));

    //wooden bar
    DrawRectangleGradientV(0,0,l2_width,l2_hud_height,l2_wood_light,l2_wood);
    DrawRectangle(0,l2_hud_height-4*l2_u,l2_width,4*l2_u,l2_wood_dark);
    int rivets = l2_width/(40*l2_u);
    for (int i=0; i<rivets; i++)
    {
        DrawCircle(20*l2_u+i*40*l2_u,8*l2_u,3*l2_u,l2_wood_dark);
        DrawCircle(20*l2_u+i*40*l2_u,l2_hud_height-12*l2_u,3*l2_u,l2_wood_dark);
    }

    //text
    DrawText("LEVEL 2 - SHORELINE",32*l2_u,20*l2_u,36*l2_u,BLACK);
    DrawText("LEVEL 2 - SHORELINE",30*l2_u,18*l2_u,36*l2_u,l2_sun_yellow);
    DrawText(TextFormat("STROKES %d / %d",l2_stroke,l2_stroke_limit),l2_width/2-298*l2_u,21*l2_u,32*l2_u,BLACK);
    DrawText(TextFormat("STROKES %d / %d",l2_stroke,l2_stroke_limit),l2_width/2-300*l2_u,19*l2_u,32*l2_u,WHITE);
    for (int i=0; i<l2_stroke_limit; i++)
    {
        Color pip = l2_wood_dark;
        if (i<l2_stroke) pip = l2_coral_red;
        DrawCircle(l2_width/2+10*l2_u+i*22*l2_u,35*l2_u,7*l2_u,pip);
        DrawCircleLines(l2_width/2+10*l2_u+i*22*l2_u,35*l2_u,7*l2_u,BLACK);
    }
    int help_width = MeasureText("R restart   ESC menu",26*l2_u);
    DrawText("R restart   ESC menu",l2_width-help_width-28*l2_u,24*l2_u,26*l2_u,BLACK);
    DrawText("R restart   ESC menu",l2_width-help_width-30*l2_u,22*l2_u,26*l2_u,WHITE);

    //splash message
    if (l2_message_timer>0)
    {
        DrawText("SPLASH!",l2_width/2-MeasureText("SPLASH!",90*l2_u)/2+4*l2_u,l2_height/2-41*l2_u,90*l2_u,Fade(l2_sea_deep,l2_message_timer));
        DrawText("SPLASH!",l2_width/2-MeasureText("SPLASH!",90*l2_u)/2,l2_height/2-45*l2_u,90*l2_u,Fade(WHITE,l2_message_timer));
    }

    //level clear or failed
    if (l2_game_state!=0)
    {
        DrawRectangle(0,0,l2_width,l2_height,Fade(BLACK,0.6));
        Rectangle panel = {l2_width/2-340*l2_u,l2_height/2-170*l2_u,680*l2_u,340*l2_u};
        DrawRectangleRec(panel,l2_wood_dark);
        Rectangle panel_band = {panel.x,panel.y,panel.width,24*l2_u};
        DrawRectangleRec(panel_band,l2_sun_yellow);
        DrawRectangleLinesEx(panel,6*l2_u,l2_sand_light);
        if (l2_game_state==1)
        {
            DrawText("LEVEL CLEAR!",l2_width/2-MeasureText("LEVEL CLEAR!",80*l2_u)/2,panel.y+60*l2_u,80*l2_u,l2_sun_yellow);
        }
        else
        {
            DrawText("FAILED",l2_width/2-MeasureText("FAILED",80*l2_u)/2,panel.y+60*l2_u,80*l2_u,l2_coral_red);
        }
        int strokes_width = MeasureText(TextFormat("Strokes: %d / %d",l2_stroke,l2_stroke_limit),40*l2_u);
        DrawText(TextFormat("Strokes: %d / %d",l2_stroke,l2_stroke_limit),l2_width/2-strokes_width/2,panel.y+170*l2_u,40*l2_u,WHITE);
        DrawText("R play again   ESC menu",l2_width/2-MeasureText("R play again   ESC menu",30*l2_u)/2,panel.y+250*l2_u,30*l2_u,l2_sand_light);
    }
}


//level 2: keep the obstacles moving with no ball (intro and menu)
void l2_background_step(float dt)
{
    l2_animation_time = l2_animation_time + dt;
    l2_update_obstacles(dt);
}


//level 2: draw everything except the scoreboard (intro and menu)
void l2_draw_scene()
{

    l2_draw_ground();
    l2_draw_obstacles();
    l2_draw_ball_and_pot();
    l2_draw_palm_leaves();

}


//level 2: set the screen size, load pictures, start the level
void l2_start(int screen_width, int screen_height)
{
    l2_width = screen_width;
    l2_height = screen_height;

    //pictures (made at 2x size, so they stay sharp)
    l2_sand_texture = LoadTexture("assets/beach/beach_sand_tile.png");
    l2_water_texture = LoadTexture("assets/beach/beach_water_tile.png");
    l2_foam_texture = LoadTexture("assets/beach/beach_foam_strip.png");
    l2_crab_texture = LoadTexture("assets/beach/beach_crab.png");
    l2_palm_texture = LoadTexture("assets/beach/beach_palm.png");
    l2_splash_texture = LoadTexture("assets/beach/beach_splash.png");
    SetTextureWrap(l2_sand_texture,TEXTURE_WRAP_REPEAT);
    SetTextureWrap(l2_foam_texture,TEXTURE_WRAP_REPEAT);
    SetTextureFilter(l2_sand_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l2_water_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l2_foam_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l2_crab_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l2_palm_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l2_splash_texture,TEXTURE_FILTER_BILINEAR);

    //every size is N*l2_u, so it looks the same on any screen
    l2_u = l2_height/1080.0;
    if (l2_width/1920.0 < l2_u) l2_u = l2_width/1920.0;
    l2_reset_level();

}


//level 2: one frame of input and movement
void l2_update()
{
    float dt = GetFrameTime();
    if (dt>1.0/30) dt = 1.0/30;
    l2_animation_time = l2_animation_time + dt;
    if (l2_message_timer>0) l2_message_timer = l2_message_timer - dt;

    //restart
    if (IsKeyPressed(KEY_R)) l2_reset_level();

    //shooting (the click has to start while the l2_ball is still)
    int ball_stopped = 0;
    if (l2_speed.x==0 && l2_speed.y==0) ball_stopped = 1;
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && ball_stopped==1 && l2_game_state==0) l2_aiming = 1;
    if (l2_game_state!=0) l2_aiming = 0;
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && l2_aiming==1)
    {
        l2_aiming = 0;
        Vector2 mouse = {GetMouseX(),GetMouseY()};
        Vector2 drag = Vector2Subtract(l2_ball,mouse);
        if (ball_stopped==1 && l2_stroke<l2_stroke_limit && Vector2Length(drag)>=2*l2_radius_ball)
        {
            l2_last_shot_position = l2_ball;
            l2_speed = Vector2ClampValue(Vector2Scale(drag,3.125),0,l2_max_speed*l2_u);
            l2_stroke++;
        }
    }

    //moving everything in 4 small steps, so nothing jumps through anything
    for (int i=0; i<4; i++)
    {
        l2_update_obstacles(dt/4*obstacle_speed);
        if (l2_game_state==0) l2_update_ball(dt/4);
    }

    //out of strokes
    if (l2_game_state==0 && l2_stroke>=l2_stroke_limit && l2_speed.x==0 && l2_speed.y==0) l2_game_state = 2;


}


//level 2: one frame of drawing
void l2_draw()
{

    l2_draw_ground();
    l2_draw_obstacles();
    l2_draw_ball_and_pot();
    l2_draw_palm_leaves();
    draw_straight_preview(l2_ball,l2_aiming,l2_radius_ball,l2_max_speed*l2_u,l2_u);
    l2_draw_hud();

}


void l2_unload()
{
    UnloadTexture(l2_sand_texture);
    UnloadTexture(l2_water_texture);
    UnloadTexture(l2_foam_texture);
    UnloadTexture(l2_crab_texture);
    UnloadTexture(l2_palm_texture);
    UnloadTexture(l2_splash_texture);
}
