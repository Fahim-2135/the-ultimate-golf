//==================== LEVEL 1 ====================

//full global
int l1_width = 1920;
int l1_height = 1080;
float l1_u = 1;
int l1_stroke_base = 18;
int l1_stroke_limit = 18;
#define l1_max_speed 650

//colours
Color l1_brick = {139,58,43,255};
Color l1_brick_dark = {110,44,32,255};
Color l1_mortar = {58,51,48,255};
Color l1_steel = {90,95,102,255};
Color l1_steel_light = {138,144,153,255};
Color l1_steel_dark = {46,49,54,255};
Color l1_floor_colour = {74,74,72,255};
Color l1_hazard_yellow = {232,185,35,255};
Color l1_rust = {181,84,28,255};
Color l1_rust_dark = {107,46,14,255};
Color l1_molten_orange = {255,106,0,255};
Color l1_molten_yellow = {255,208,0,255};
Color l1_molten_dark = {139,26,0,255};
Color l1_laser_red = {255,32,48,255};
Color l1_magnet_red = {192,40,45,255};

//l1_ball and l1_pot
Vector2 l1_ball;
Vector2 l1_speed;
float l1_radius_ball;
Vector2 l1_pot;
float l1_radius_pot;
Vector2 l1_start_position;
Vector2 l1_last_shot_position;
int l1_stroke = 0;
int l1_game_state = 0;
int l1_aiming = 0;
float l1_message_timer = 0;
int l1_message_type = 0;
float l1_animation_time = 0;

//lanes
float l1_hud_height;
float l1_wall;
float l1_gap;
float l1_lane_width;
float l1_lane_x[6];
float l1_course_top;
float l1_course_bottom;
Rectangle l1_walls[9];
Vector2 l1_no_speed = {0,0};

//valve wheel bumpers (one in lane 1, two next to the l1_pot)
Vector2 l1_bumper[3];
float l1_bumper_radius;

//l1_steel l1_crate
Rectangle l1_crate;

//conveyor belts (0 pushes down in lane 1, 1 pushes up in lane 4)
Rectangle l1_belt[2];
float l1_belt_push[2];

//warning l1_diamond
Vector2 l1_diamond;
float l1_diamond_radius;

//l1_steel girders
Rectangle l1_girder[2];

//l1_oil slick
Vector2 l1_oil;
float l1_oil_radius;

//spinning l1_fan
Vector2 l1_fan;
float l1_fan_angle = 0;
float l1_fan_spin = 120;
float l1_fan_blade_length;
float l1_fan_blade_thickness;
float l1_fan_hub_radius;

//crane beams (0 in lane 3, 1 and 2 in lane 6)
Rectangle l1_beam[3];
float l1_beam_speed[3];
float l1_beam_left_limit[3];
float l1_beam_right_limit[3];

//l1_magnet
Vector2 l1_magnet;
float l1_magnet_core;
float l1_magnet_field;
float l1_magnet_pull;

//hydraulic l1_piston
Rectangle l1_piston;
float l1_piston_head;
float l1_piston_length = 0;
float l1_piston_max_length;
float l1_piston_speed = 0;
float l1_piston_timer = 0;
int l1_piston_state = 0;

//molten pits
Vector2 l1_pit[3];
float l1_pit_radius;

//l1_laser gate
Rectangle l1_laser;
float l1_laser_timer = 0;


void l1_reset_level()
{
    //sizes
    l1_hud_height = 70*l1_u;
    l1_wall = 28*l1_u;
    l1_gap = 230*l1_u;
    l1_lane_width = (l1_width - 7*l1_wall)/6;
    l1_course_top = l1_hud_height + l1_wall;
    l1_course_bottom = l1_height - l1_wall;
    l1_radius_ball = 7*l1_u;
    l1_radius_pot = 11*l1_u;

    float centre[6];
    for (int i=0; i<6; i++)
    {
        l1_lane_x[i] = l1_wall + i*(l1_lane_width + l1_wall);
        centre[i] = l1_lane_x[i] + l1_lane_width/2;
    }

    //frame l1_walls
    Rectangle top_wall = {0,l1_hud_height,l1_width,l1_wall};
    Rectangle bottom_wall = {0,l1_height-l1_wall,l1_width,l1_wall};
    Rectangle left_wall = {0,l1_hud_height,l1_wall,l1_height-l1_hud_height};
    Rectangle right_wall = {l1_width-l1_wall,l1_hud_height,l1_wall,l1_height-l1_hud_height};
    l1_walls[0] = top_wall;
    l1_walls[1] = bottom_wall;
    l1_walls[2] = left_wall;
    l1_walls[3] = right_wall;

    //divider l1_walls, the turn l1_gap is at the top after lane 1, 3, 5 and at the bottom after lane 2, 4
    for (int i=0; i<5; i++)
    {
        Rectangle divider = {l1_lane_x[i]+l1_lane_width,l1_course_top,l1_wall,l1_course_bottom-l1_course_top-l1_gap};
        if (i%2==0) divider.y = l1_course_top + l1_gap;
        l1_walls[4+i] = divider;
    }

    //l1_ball and l1_pot
    l1_start_position.x = centre[0];
    l1_start_position.y = l1_course_bottom - 70*l1_u;
    l1_ball = l1_start_position;
    l1_last_shot_position = l1_start_position;
    l1_speed.x = 0;
    l1_speed.y= 0;
    l1_pot.x = centre[5];
    l1_pot.y = l1_course_bottom - 110*l1_u;
    l1_stroke = 0;
    l1_game_state = 0;
    l1_aiming = 0;
    l1_message_timer = 0;

    //lane 1: l1_crate, valve wheel, conveyor pushing down
    l1_crate.x = centre[0] - 68*l1_u;
    l1_crate.y = l1_course_bottom - 233*l1_u;
    l1_crate.width = 46*l1_u;
    l1_crate.height = 46*l1_u;
    l1_bumper_radius = 26*l1_u;
    l1_bumper[0].x = centre[0] + 55*l1_u;
    l1_bumper[0].y = l1_course_bottom - 360*l1_u;
    l1_belt[0].x = l1_lane_x[0];
    l1_belt[0].y = l1_course_top + 330*l1_u;
    l1_belt[0].width = l1_lane_width;
    l1_belt[0].height = 64*l1_u;
    l1_belt_push[0] = 750*l1_u;

    //lane 2: l1_diamond, two girders, l1_oil slick
    l1_diamond_radius = 34*l1_u;
    l1_diamond.x = centre[1];
    l1_diamond.y = l1_course_top + 330*l1_u;
    l1_girder[0].x = l1_lane_x[1];
    l1_girder[0].y = l1_course_top + 500*l1_u;
    l1_girder[0].width = 150*l1_u;
    l1_girder[0].height = 18*l1_u;
    l1_girder[1].x = l1_lane_x[1] + l1_lane_width - 150*l1_u;
    l1_girder[1].y = l1_course_top + 640*l1_u;
    l1_girder[1].width = 150*l1_u;
    l1_girder[1].height = 18*l1_u;
    l1_oil_radius = 70*l1_u;
    l1_oil.x = centre[1];
    l1_oil.y = l1_course_bottom - 170*l1_u;

    //lane 3: crane l1_beam, spinning l1_fan
    l1_beam[0].x = l1_lane_x[2] + 42*l1_u;
    l1_beam[0].y = l1_course_bottom - 330*l1_u;
    l1_fan_blade_length = 190*l1_u;
    l1_fan_blade_thickness = 12*l1_u;
    l1_fan_hub_radius = 16*l1_u;
    l1_fan.x = centre[2];
    l1_fan.y = l1_course_top + 380*l1_u;
    l1_fan_angle = 0;

    //lane 4: conveyor pushing up, l1_piston, l1_magnet
    l1_belt[1].x = l1_lane_x[3];
    l1_belt[1].y = l1_course_top + 250*l1_u;
    l1_belt[1].width = l1_lane_width;
    l1_belt[1].height = 64*l1_u;
    l1_belt_push[1] = -750*l1_u;
    l1_piston_head = 30*l1_u;
    l1_piston.x = l1_lane_x[3];
    l1_piston.y = l1_course_top + 435*l1_u;
    l1_piston.width = l1_piston_head;
    l1_piston.height = 70*l1_u;
    l1_piston_max_length = l1_lane_width - l1_piston_head - 42*l1_u;
    l1_piston_length = 0;
    l1_piston_speed = 0;
    l1_piston_timer = 0;
    l1_piston_state = 0;
    l1_magnet_core = 22*l1_u;
    l1_magnet_field = 170*l1_u;
    l1_magnet_pull = 450*l1_u;
    l1_magnet.x = centre[3];
    l1_magnet.y = l1_course_top + 720*l1_u;

    //lane 5: two molten pits, l1_laser gate
    l1_pit_radius = 38*l1_u;
    l1_pit[0].x = centre[4] - 55*l1_u;
    l1_pit[0].y = l1_course_bottom - 330*l1_u;
    l1_pit[1].x = centre[4] + 55*l1_u;
    l1_pit[1].y = l1_course_top + 400*l1_u;
    l1_laser.x = l1_lane_x[4];
    l1_laser.y = l1_course_top + 256*l1_u;
    l1_laser.width = l1_lane_width;
    l1_laser.height = 8*l1_u;
    l1_laser_timer = 0;

    //lane 6: two crane beams, molten l1_pit, two bumpers guarding the l1_pot
    l1_beam[1].x = l1_lane_x[5] + 42*l1_u;
    l1_beam[1].y = l1_course_top + 300*l1_u;
    l1_beam[2].x = l1_lane_x[5] + l1_lane_width - 42*l1_u - 110*l1_u;
    l1_beam[2].y = l1_course_top + 440*l1_u;
    l1_pit[2].x = centre[5];
    l1_pit[2].y = l1_course_bottom - 300*l1_u;
    l1_bumper[1].x = centre[5] - 75*l1_u;
    l1_bumper[1].y = l1_course_bottom - 190*l1_u;
    l1_bumper[2].x = centre[5] + 75*l1_u;
    l1_bumper[2].y = l1_course_bottom - 190*l1_u;

    //same size and l1_speed for all crane beams
    for (int i=0; i<3; i++)
    {
        l1_beam[i].width = 110*l1_u;
        l1_beam[i].height = 18*l1_u;
        int lane = 2;
        if (i>0) lane = 5;
        l1_beam_left_limit[i] = l1_lane_x[lane] + 42*l1_u;
        l1_beam_right_limit[i] = l1_lane_x[lane] + l1_lane_width - 42*l1_u - l1_beam[i].width;
    }
    l1_beam_speed[0] = 110*l1_u;
    l1_beam_speed[1] = 110*l1_u;
    l1_beam_speed[2] = -110*l1_u;
}

//bounce off a straight rectangle, rec_speed is how fast the rectangle itself is moving
void l1_bounce_off_rectangle(Rectangle rec, Vector2 rec_speed)
{
    if (CheckCollisionCircleRec(l1_ball,l1_radius_ball,rec))
    {
        Vector2 collision_point;
        collision_point.x = Clamp(l1_ball.x,rec.x,rec.x+rec.width);
        collision_point.y = Clamp(l1_ball.y,rec.y,rec.y+rec.height);
        Vector2 normal = Vector2Subtract(l1_ball,collision_point);

        //centre is inside, go out through the nearest side
        if (normal.x==0 && normal.y==0)
        {
            float left = l1_ball.x - rec.x;
            float right = rec.x + rec.width - l1_ball.x;
            float top = l1_ball.y - rec.y;
            float bottom = rec.y + rec.height - l1_ball.y;
            if (left<=right && left<=top && left<=bottom) { normal.x = -1; collision_point.x = rec.x; }
            else if (right<=top && right<=bottom) { normal.x = 1; collision_point.x = rec.x + rec.width; }
            else if (top<=bottom) { normal.y = -1; collision_point.y = rec.y; }
            else { normal.y = 1; collision_point.y = rec.y + rec.height; }
        }
        normal = Vector2Normalize(normal);
        l1_ball = Vector2Add(collision_point,Vector2Scale(normal,l1_radius_ball));

        //reflect only if the l1_ball is going into it
        Vector2 relative_speed = Vector2Subtract(l1_speed,rec_speed);
        if (Vector2DotProduct(relative_speed,normal)<0)
        {
            relative_speed = Vector2Reflect(relative_speed,normal);
            l1_speed = Vector2Add(relative_speed,rec_speed);

            //moving l1_beam or l1_piston side: also knock the l1_ball out of its row, or it gets hit again and again
            if (rec_speed.x!=0 && normal.y==0)
            {
                if (l1_ball.y < rec.y+rec.height/2) l1_speed.y = l1_speed.y - fabsf(rec_speed.x)/2;
                else l1_speed.y = l1_speed.y + fabsf(rec_speed.x)/2;
            }
            l1_speed = Vector2ClampValue(l1_speed,0,l1_max_speed*l1_u);
        }
    }
}

//bounce off a round thing that doesn't move
void l1_bounce_off_circle(Vector2 center, float radius)
{
    Vector2 normal = Vector2Subtract(l1_ball,center);
    float distance = Vector2Length(normal);
    if (distance < radius + l1_radius_ball)
    {
        if (distance==0) normal.y = -1;
        normal = Vector2Normalize(normal);
        l1_ball = Vector2Add(center,Vector2Scale(normal,radius + l1_radius_ball));
        if (Vector2DotProduct(l1_speed,normal)<0) l1_speed = Vector2Reflect(l1_speed,normal);
    }
}

//bounce off a turned rectangle (l1_diamond, l1_fan blades), spin is in degrees per second
void l1_bounce_off_rotated_rectangle(Vector2 center, float rec_width, float rec_height, float angle, float spin)
{
    //look at the l1_ball as if the rectangle was not turned
    Vector2 local_ball = Vector2Rotate(Vector2Subtract(l1_ball,center),-angle*DEG2RAD);
    Rectangle rec = {-rec_width/2,-rec_height/2,rec_width,rec_height};
    if (CheckCollisionCircleRec(local_ball,l1_radius_ball,rec))
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
        l1_ball = Vector2Add(collision_point,Vector2Scale(normal,l1_radius_ball));

        //l1_speed of the blade at the point it touches the l1_ball
        Vector2 arm = Vector2Subtract(collision_point,center);
        Vector2 rec_speed = {-arm.y*spin*DEG2RAD, arm.x*spin*DEG2RAD};

        Vector2 relative_speed = Vector2Subtract(l1_speed,rec_speed);
        if (Vector2DotProduct(relative_speed,normal)<0)
        {
            relative_speed = Vector2Reflect(relative_speed,normal);
            l1_speed = Vector2ClampValue(Vector2Add(relative_speed,rec_speed),0,l1_max_speed*l1_u);
        }
    }
}


void l1_update_obstacles(float dt)
{
    //crane beams moving
    for (int i=0; i<3; i++)
    {
        l1_beam[i].x = l1_beam[i].x + l1_beam_speed[i]*dt;
        if (l1_beam[i].x < l1_beam_left_limit[i])
        {
            l1_beam[i].x = l1_beam_left_limit[i];
            l1_beam_speed[i] = fabsf(l1_beam_speed[i]);
        }
        if (l1_beam[i].x > l1_beam_right_limit[i])
        {
            l1_beam[i].x = l1_beam_right_limit[i];
            l1_beam_speed[i] = -fabsf(l1_beam_speed[i]);
        }
    }

    //l1_fan spinning
    l1_fan_angle = l1_fan_angle + l1_fan_spin*dt;
    if (l1_fan_angle>=360) l1_fan_angle = l1_fan_angle - 360;

    //l1_laser: off 1.2s, warning 0.4s, on 1.4s
    l1_laser_timer = l1_laser_timer + dt;
    if (l1_laser_timer>=3.0) l1_laser_timer = l1_laser_timer - 3.0;

    //l1_piston: 0 waiting, 1 going out, 2 holding, 3 going back
    l1_piston_timer = l1_piston_timer + dt;
    l1_piston_speed = 0;
    if (l1_piston_state==0)
    {
        if (l1_piston_timer>=1.2)
        {
            l1_piston_state = 1;
            l1_piston_timer = 0;
        }
    }
    else if (l1_piston_state==1)
    {
        l1_piston_length = l1_piston_length + 900*l1_u*dt;
        l1_piston_speed = 900*l1_u;
        if (l1_piston_length>=l1_piston_max_length)
        {
            l1_piston_length = l1_piston_max_length;
            l1_piston_state = 2;
            l1_piston_timer = 0;
        }
    }
    else if (l1_piston_state==2)
    {
        if (l1_piston_timer>=0.3)
        {
            l1_piston_state = 3;
            l1_piston_timer = 0;
        }
    }
    else if (l1_piston_state==3)
    {
        l1_piston_length = l1_piston_length - 250*l1_u*dt;
        l1_piston_speed = -250*l1_u;
        if (l1_piston_length<=0)
        {
            l1_piston_length = 0;
            l1_piston_state = 0;
            l1_piston_timer = 0;
        }
    }
    l1_piston.width = l1_piston_head + l1_piston_length;
}


//after lava or l1_laser, the l1_ball goes back to where it was shot from
void l1_send_ball_back()
{
    l1_ball = l1_last_shot_position;

    //not inside the l1_laser
    if (CheckCollisionCircleRec(l1_ball,l1_radius_ball,l1_laser)) l1_ball = l1_start_position;

    //not right next to the l1_piston's path, or it gets hit into the lava again and again
    if (l1_ball.x>l1_piston.x && l1_ball.x<l1_piston.x+l1_piston_head+l1_piston_max_length+l1_radius_ball && l1_ball.y>l1_piston.y-l1_radius_ball-1*l1_u && l1_ball.y<l1_piston.y+l1_piston.height+l1_radius_ball+1*l1_u)
    {
        if (l1_ball.y < l1_piston.y+l1_piston.height/2) l1_ball.y = l1_piston.y - l1_radius_ball - 2*l1_u;
        else l1_ball.y = l1_piston.y + l1_piston.height + l1_radius_ball + 2*l1_u;
    }

    l1_speed.x = 0;
    l1_speed.y = 0;
}


void l1_update_ball(float dt)
{
    int pushed = 0;

    //conveyor belts
    for (int i=0; i<2; i++)
    {
        if (l1_ball.x>l1_belt[i].x && l1_ball.x<l1_belt[i].x+l1_belt[i].width && l1_ball.y>l1_belt[i].y && l1_ball.y<l1_belt[i].y+l1_belt[i].height)
        {
            l1_speed.y = l1_speed.y + l1_belt_push[i]*dt;
            pushed = 1;
        }
    }

    //l1_magnet pull
    Vector2 to_magnet = Vector2Subtract(l1_magnet,l1_ball);
    float magnet_distance = Vector2Length(to_magnet);
    if (magnet_distance<l1_magnet_field && magnet_distance>l1_magnet_core+l1_radius_ball+1*l1_u)
    {
        l1_speed = Vector2Add(l1_speed,Vector2Scale(Vector2Normalize(to_magnet),l1_magnet_pull*dt));
        pushed = 1;
    }

    //moving
    l1_ball = Vector2Add(l1_ball,Vector2Scale(l1_speed,dt));

    //proportional deceleration (slows the whole l1_speed, so the direction stays the same)
    float friction = 110*l1_u;
    if (CheckCollisionPointCircle(l1_ball,l1_oil,l1_oil_radius)) friction = 20*l1_u;
    float ball_speed = Vector2Length(l1_speed);
    if (ball_speed>0)
    {
        float new_speed = ball_speed - friction*dt;
        if (new_speed<0) new_speed = 0;
        l1_speed = Vector2Scale(l1_speed,new_speed/ball_speed);
    }
    if (pushed==0 && Vector2Length(l1_speed)<6*l1_u)
    {
        l1_speed.x = 0;
        l1_speed.y= 0;
    }

    //molten pits (checked before the collisions, so if a moving thing is on the old spot it pushes the l1_ball out)
    for (int i=0; i<3; i++)
    {
        if (Vector2Length(Vector2Subtract(l1_ball,l1_pit[i])) < l1_pit_radius - 5*l1_u)
        {
            l1_send_ball_back();
            l1_message_type = 1;
            l1_message_timer = 1;
        }
    }

    //l1_laser gate
    if (l1_laser_timer>=1.6 && CheckCollisionCircleRec(l1_ball,l1_radius_ball,l1_laser))
    {
        l1_send_ball_back();
        l1_message_type = 2;
        l1_message_timer = 1;
    }

    //collision with things that don't move
    for (int i=0; i<3; i++)
    {
        l1_bounce_off_circle(l1_bumper[i],l1_bumper_radius);
    }
    l1_bounce_off_rectangle(l1_crate,l1_no_speed);
    for (int i=0; i<2; i++)
    {
        l1_bounce_off_rectangle(l1_girder[i],l1_no_speed);
    }
    l1_bounce_off_rotated_rectangle(l1_diamond,l1_diamond_radius*sqrt(2),l1_diamond_radius*sqrt(2),45,0);

    //collision with things that move
    l1_bounce_off_circle(l1_fan,l1_fan_hub_radius);
    l1_bounce_off_rotated_rectangle(l1_fan,l1_fan_blade_length,l1_fan_blade_thickness,l1_fan_angle,l1_fan_spin);
    l1_bounce_off_rotated_rectangle(l1_fan,l1_fan_blade_length,l1_fan_blade_thickness,l1_fan_angle+90,l1_fan_spin);
    for (int i=0; i<3; i++)
    {
        Vector2 beam_velocity = {l1_beam_speed[i],0};
        l1_bounce_off_rectangle(l1_beam[i],beam_velocity);
    }
    Vector2 piston_velocity = {l1_piston_speed,0};
    l1_bounce_off_rectangle(l1_piston,piston_velocity);

    //l1_magnet core, the l1_ball sticks to it
    Vector2 away_from_magnet = Vector2Subtract(l1_ball,l1_magnet);
    float distance = Vector2Length(away_from_magnet);
    if (distance < l1_magnet_core + l1_radius_ball)
    {
        if (distance==0) away_from_magnet.y = -1;
        away_from_magnet = Vector2Normalize(away_from_magnet);
        l1_ball = Vector2Add(l1_magnet,Vector2Scale(away_from_magnet,l1_magnet_core + l1_radius_ball));
        if (Vector2DotProduct(l1_speed,away_from_magnet)<0)
        {
            l1_speed.x = 0;
            l1_speed.y = 0;
        }
    }

    //l1_wall bounce (last, so the l1_ball never ends inside a l1_wall)
    for (int i=0; i<9; i++)
    {
        l1_bounce_off_rectangle(l1_walls[i],l1_no_speed);
    }

    //score
    if ((l1_ball.x>l1_pot.x-3*l1_radius_pot/4) && (l1_ball.x<l1_pot.x+3*l1_radius_pot/4) && (l1_ball.y>l1_pot.y-3*l1_radius_pot/4) && (l1_ball.y<l1_pot.y+3*l1_radius_pot/4))
    {
        l1_ball = l1_pot;
        l1_speed.x = 0;
        l1_speed.y= 0;
        l1_game_state = 1;
    }
}


//yellow and black bands, across the long side
void l1_draw_hazard_stripes(Rectangle rec)
{
    DrawRectangleRec(rec,l1_hazard_yellow);
    float band = 6*l1_u;
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


void l1_draw_background()
{
    ClearBackground(l1_steel_dark);

    //lane floor
    DrawRectangle(l1_wall,l1_course_top,l1_width-2*l1_wall,l1_course_bottom-l1_course_top,l1_floor_colour);

    //l1_diamond plate tread marks
    int columns = (l1_width-2*l1_wall)/(34*l1_u);
    int rows = (l1_course_bottom-l1_course_top)/(34*l1_u);
    for (int i=0; i<=columns; i++)
    {
        for (int j=0; j<=rows; j++)
        {
            float x = l1_wall + i*34*l1_u + 17*l1_u;
            float y = l1_course_top + j*34*l1_u + 17*l1_u;
            Vector2 mark_start = {x-6*l1_u,y-6*l1_u};
            Vector2 mark_end = {x+6*l1_u,y+6*l1_u};
            if ((i+j)%2==1)
            {
                mark_start.y = y+6*l1_u;
                mark_end.y = y-6*l1_u;
            }
            DrawLineEx(mark_start,mark_end,4*l1_u,Fade(BLACK,0.15));
            mark_start.y = mark_start.y - 1*l1_u;
            mark_end.y = mark_end.y - 1*l1_u;
            DrawLineEx(mark_start,mark_end,2*l1_u,Fade(l1_steel_light,0.25));
        }
    }

    //painted centre dashes and shadows next to the l1_walls
    for (int i=0; i<6; i++)
    {
        float centre = l1_lane_x[i] + l1_lane_width/2;
        int dashes = (l1_course_bottom-l1_course_top)/(60*l1_u);
        for (int j=0; j<dashes; j++)
        {
            DrawRectangle(centre-2*l1_u,l1_course_top+j*60*l1_u+17*l1_u,4*l1_u,26*l1_u,Fade(l1_hazard_yellow,0.18));
        }
        DrawRectangleGradientH(l1_lane_x[i],l1_course_top,20*l1_u,l1_course_bottom-l1_course_top,Fade(BLACK,0.35),BLANK);
        DrawRectangleGradientH(l1_lane_x[i]+l1_lane_width-20*l1_u,l1_course_top,20*l1_u,l1_course_bottom-l1_course_top,BLANK,Fade(BLACK,0.35));
    }
    DrawRectangleGradientV(l1_wall,l1_course_top,l1_width-2*l1_wall,20*l1_u,Fade(BLACK,0.35),BLANK);
}


void l1_draw_walls()
{
    float brick_height = 14*l1_u;
    float brick_length = 28*l1_u;
    for (int i=0; i<9; i++)
    {
        Rectangle rec = l1_walls[i];
        DrawRectangleRec(rec,l1_mortar);

        //bricks, every second row moved by half a l1_brick
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
                if (right-left>3*l1_u && bottom-y>3*l1_u)
                {
                    Color colour = l1_brick;
                    if ((row*3 + count*7)%5==0) colour = l1_brick_dark;
                    DrawRectangle(left+1*l1_u,y+1*l1_u,right-left-2*l1_u,bottom-y-2*l1_u,colour);
                    DrawRectangle(left+1*l1_u,bottom-3*l1_u,right-left-2*l1_u,2*l1_u,Fade(BLACK,0.25));
                }
                x = x + brick_length;
                count++;
            }
        }

        //l1_steel edge and rivets
        DrawRectangleLinesEx(rec,3*l1_u,l1_steel_dark);
        if (rec.height>rec.width)
        {
            int rivets = rec.height/(40*l1_u);
            for (int j=0; j<rivets; j++)
            {
                DrawCircle(rec.x+5*l1_u,rec.y+20*l1_u+j*40*l1_u,2.5*l1_u,l1_steel_light);
                DrawCircle(rec.x+rec.width-5*l1_u,rec.y+20*l1_u+j*40*l1_u,2.5*l1_u,l1_steel_light);
            }
        }
        else
        {
            int rivets = rec.width/(40*l1_u);
            for (int j=0; j<rivets; j++)
            {
                DrawCircle(rec.x+20*l1_u+j*40*l1_u,rec.y+5*l1_u,2.5*l1_u,l1_steel_light);
                DrawCircle(rec.x+20*l1_u+j*40*l1_u,rec.y+rec.height-5*l1_u,2.5*l1_u,l1_steel_light);
            }
        }
    }

    //hazard caps at the end of each divider
    for (int i=0; i<5; i++)
    {
        Rectangle cap = {l1_walls[4+i].x,l1_walls[4+i].y,l1_wall,22*l1_u};
        if (i%2==1) cap.y = l1_walls[4+i].y + l1_walls[4+i].height - 22*l1_u;
        l1_draw_hazard_stripes(cap);
        DrawRectangleLinesEx(cap,2*l1_u,BLACK);
    }
}


void l1_draw_path_arrows()
{
    float spacing = 90*l1_u;
    float offset = fmod(l1_animation_time*45*l1_u,spacing);
    Color arrow_colour = Fade(l1_hazard_yellow,0.3);

    //along the lanes
    int arrows = (l1_course_bottom-l1_course_top-l1_gap)/spacing;
    for (int i=0; i<6; i++)
    {
        Vector2 arrow;
        arrow.x = l1_lane_x[i] + l1_lane_width/2;
        for (int j=0; j<arrows; j++)
        {
            if (i%2==0)
            {
                arrow.y = l1_course_bottom - 115*l1_u - j*spacing - offset;
                DrawPoly(arrow,3,10*l1_u,270,arrow_colour);
            }
            else
            {
                arrow.y = l1_course_top + 115*l1_u + j*spacing + offset;
                DrawPoly(arrow,3,10*l1_u,90,arrow_colour);
            }
        }
    }

    //across the turns
    for (int i=0; i<5; i++)
    {
        Vector2 arrow;
        arrow.y = l1_course_top + 115*l1_u;
        if (i%2==1) arrow.y = l1_course_bottom - 115*l1_u;
        for (int j=0; j<3; j++)
        {
            arrow.x = l1_lane_x[i] + l1_lane_width/2 + 30*l1_u + j*spacing + offset;
            DrawPoly(arrow,3,10*l1_u,0,arrow_colour);
        }
    }
}


void l1_draw_obstacles()
{
    float t = l1_animation_time;

    //l1_oil slick
    DrawCircle(l1_oil.x,l1_oil.y,l1_oil_radius*0.75,Fade(BLACK,0.55));
    DrawCircle(l1_oil.x-30*l1_u,l1_oil.y+15*l1_u,l1_oil_radius*0.5,Fade(BLACK,0.5));
    DrawCircle(l1_oil.x+32*l1_u,l1_oil.y-12*l1_u,l1_oil_radius*0.5,Fade(BLACK,0.5));
    DrawCircle(l1_oil.x+8*l1_u,l1_oil.y+35*l1_u,l1_oil_radius*0.45,Fade(BLACK,0.45));
    DrawCircle(l1_oil.x-18*l1_u,l1_oil.y-35*l1_u,l1_oil_radius*0.4,Fade(BLACK,0.45));
    DrawRing(l1_oil,l1_oil_radius*0.3,l1_oil_radius*0.34,t*25,t*25+110,16,Fade(PURPLE,0.4));
    DrawRing(l1_oil,l1_oil_radius*0.5,l1_oil_radius*0.54,90-t*18,200-t*18,16,Fade(SKYBLUE,0.3));
    DrawRing(l1_oil,l1_oil_radius*0.66,l1_oil_radius*0.69,200+t*12,290+t*12,16,Fade(VIOLET,0.35));

    //conveyor belts
    for (int i=0; i<2; i++)
    {
        Rectangle b = l1_belt[i];
        DrawRectangleRec(b,GetColor(0x1E1E1EFF));
        float stripe = 16*l1_u;
        float move = fmod(t*80*l1_u,stripe);
        if (l1_belt_push[i]<0) move = stripe - move;
        int stripes = b.height/stripe;
        for (int j=0; j<stripes; j++)
        {
            float y = b.y + j*stripe + move;
            if (y+4*l1_u < b.y+b.height) DrawRectangle(b.x,y,b.width,4*l1_u,GetColor(0x3C3C3CFF));
        }
        //rollers
        DrawRectangleGradientV(b.x,b.y-4*l1_u,b.width,8*l1_u,l1_steel_light,l1_steel_dark);
        DrawRectangleGradientV(b.x,b.y+b.height-4*l1_u,b.width,8*l1_u,l1_steel_light,l1_steel_dark);
        //arrows
        for (int j=1; j<=3; j++)
        {
            Vector2 arrow = {b.x + j*b.width/4, b.y + b.height/2};
            if (l1_belt_push[i]>0) DrawPoly(arrow,3,12*l1_u,90,l1_hazard_yellow);
            else DrawPoly(arrow,3,12*l1_u,270,l1_hazard_yellow);
        }
    }

    //l1_steel l1_crate
    DrawRectangle(l1_crate.x+4*l1_u,l1_crate.y+5*l1_u,l1_crate.width,l1_crate.height,Fade(BLACK,0.4));
    DrawRectangleRec(l1_crate,l1_rust);
    DrawRectangleLinesEx(l1_crate,5*l1_u,l1_rust_dark);
    Vector2 crate_corner1 = {l1_crate.x+5*l1_u,l1_crate.y+5*l1_u};
    Vector2 crate_corner2 = {l1_crate.x+l1_crate.width-5*l1_u,l1_crate.y+l1_crate.height-5*l1_u};
    Vector2 crate_corner3 = {l1_crate.x+l1_crate.width-5*l1_u,l1_crate.y+5*l1_u};
    Vector2 crate_corner4 = {l1_crate.x+5*l1_u,l1_crate.y+l1_crate.height-5*l1_u};
    DrawLineEx(crate_corner1,crate_corner2,4*l1_u,l1_rust_dark);
    DrawLineEx(crate_corner3,crate_corner4,4*l1_u,l1_rust_dark);
    DrawCircle(crate_corner1.x,crate_corner1.y,2.5*l1_u,l1_steel_light);
    DrawCircle(crate_corner2.x,crate_corner2.y,2.5*l1_u,l1_steel_light);
    DrawCircle(crate_corner3.x,crate_corner3.y,2.5*l1_u,l1_steel_light);
    DrawCircle(crate_corner4.x,crate_corner4.y,2.5*l1_u,l1_steel_light);

    //valve wheel bumpers
    for (int i=0; i<3; i++)
    {
        Vector2 b = l1_bumper[i];
        DrawCircle(b.x+3*l1_u,b.y+4*l1_u,l1_bumper_radius,Fade(BLACK,0.4));
        DrawCircle(b.x,b.y,l1_bumper_radius,l1_steel);
        DrawRing(b,l1_bumper_radius-6*l1_u,l1_bumper_radius,0,360,32,l1_magnet_red);
        Vector2 spoke = {l1_bumper_radius-6*l1_u,0};
        for (int j=0; j<4; j++)
        {
            Vector2 turned = Vector2Rotate(spoke,j*45*DEG2RAD);
            DrawLineEx(Vector2Add(b,turned),Vector2Subtract(b,turned),4*l1_u,l1_steel_dark);
        }
        DrawCircle(b.x,b.y,7*l1_u,l1_steel_dark);
        DrawCircle(b.x,b.y,3*l1_u,l1_steel_light);
    }

    //warning l1_diamond
    Vector2 diamond_shadow = {l1_diamond.x+3*l1_u,l1_diamond.y+4*l1_u};
    DrawPoly(diamond_shadow,4,l1_diamond_radius,0,Fade(BLACK,0.4));
    DrawPoly(l1_diamond,4,l1_diamond_radius,0,l1_hazard_yellow);
    DrawPolyLinesEx(l1_diamond,4,l1_diamond_radius,0,4*l1_u,BLACK);
    DrawText("!",l1_diamond.x-MeasureText("!",30*l1_u)/2,l1_diamond.y-14*l1_u,30*l1_u,BLACK);

    //l1_steel girders
    for (int i=0; i<2; i++)
    {
        Rectangle g = l1_girder[i];
        DrawRectangle(g.x,g.y+4*l1_u,g.width,g.height,Fade(BLACK,0.35));
        DrawRectangleRec(g,l1_steel);
        DrawRectangle(g.x,g.y,g.width,4*l1_u,l1_steel_light);
        DrawRectangle(g.x,g.y+g.height-4*l1_u,g.width,4*l1_u,l1_steel_dark);
        int rivets = g.width/(20*l1_u);
        for (int j=1; j<rivets; j++)
        {
            DrawCircle(g.x+j*20*l1_u,g.y+g.height/2,2*l1_u,l1_steel_light);
        }
        Rectangle girder_end = {g.x+g.width-20*l1_u,g.y,20*l1_u,g.height};
        if (i==1) girder_end.x = g.x;
        l1_draw_hazard_stripes(girder_end);
        DrawRectangleLinesEx(g,2*l1_u,BLACK);
    }

    //crane beams
    for (int i=0; i<3; i++)
    {
        DrawRectangle(l1_beam[i].x+5*l1_u,l1_beam[i].y+6*l1_u,l1_beam[i].width,l1_beam[i].height,Fade(BLACK,0.4));
        l1_draw_hazard_stripes(l1_beam[i]);
        DrawRectangleLinesEx(l1_beam[i],3*l1_u,BLACK);
        DrawCircle(l1_beam[i].x+l1_beam[i].width/2,l1_beam[i].y+l1_beam[i].height/2,4*l1_u,l1_steel_dark);
    }

    //spinning l1_fan
    DrawCircle(l1_fan.x,l1_fan.y,l1_fan_blade_length/2+10*l1_u,Fade(BLACK,0.3));
    DrawRing(l1_fan,l1_fan_blade_length/2+4*l1_u,l1_fan_blade_length/2+10*l1_u,0,360,48,l1_steel_dark);
    DrawRing(l1_fan,l1_fan_blade_length/2+8*l1_u,l1_fan_blade_length/2+10*l1_u,0,360,48,l1_steel_light);
    Rectangle blade = {l1_fan.x,l1_fan.y,l1_fan_blade_length,l1_fan_blade_thickness};
    Vector2 blade_origin = {l1_fan_blade_length/2,l1_fan_blade_thickness/2};
    DrawRectanglePro(blade,blade_origin,l1_fan_angle-24,Fade(l1_steel_light,0.12));
    DrawRectanglePro(blade,blade_origin,l1_fan_angle+90-24,Fade(l1_steel_light,0.12));
    DrawRectanglePro(blade,blade_origin,l1_fan_angle-12,Fade(l1_steel_light,0.25));
    DrawRectanglePro(blade,blade_origin,l1_fan_angle+90-12,Fade(l1_steel_light,0.25));
    DrawRectanglePro(blade,blade_origin,l1_fan_angle,l1_steel_light);
    DrawRectanglePro(blade,blade_origin,l1_fan_angle+90,l1_steel_light);
    DrawCircle(l1_fan.x,l1_fan.y,l1_fan_hub_radius,l1_steel_dark);
    DrawCircleLines(l1_fan.x,l1_fan.y,l1_fan_hub_radius,BLACK);
    Vector2 bolt_arm = {9*l1_u,0};
    for (int j=0; j<4; j++)
    {
        Vector2 bolt = Vector2Add(l1_fan,Vector2Rotate(bolt_arm,(l1_fan_angle+45+j*90)*DEG2RAD));
        DrawCircle(bolt.x,bolt.y,2.5*l1_u,l1_steel_light);
    }

    //hydraulic l1_piston
    Rectangle housing = {l1_piston.x-l1_wall,l1_piston.y-8*l1_u,l1_wall,l1_piston.height+16*l1_u};
    DrawRectangleRec(housing,l1_steel_dark);
    for (int j=0; j<4; j++)
    {
        DrawRectangle(housing.x,housing.y+8*l1_u+j*20*l1_u,housing.width,5*l1_u,l1_steel);
    }
    DrawRectangleLinesEx(housing,2*l1_u,BLACK);
    DrawRectangleGradientV(l1_piston.x,l1_piston.y,l1_piston_length,l1_piston.height,l1_steel,l1_steel_dark);
    DrawRectangleGradientV(l1_piston.x,l1_piston.y+l1_piston.height/2-10*l1_u,l1_piston_length,20*l1_u,RAYWHITE,l1_steel_light);
    Rectangle head = {l1_piston.x+l1_piston_length,l1_piston.y,l1_piston_head,l1_piston.height};
    l1_draw_hazard_stripes(head);
    DrawRectangleLinesEx(head,3*l1_u,BLACK);
    DrawRectangleLinesEx(l1_piston,2*l1_u,BLACK);
    Color lamp = GetColor(0x4A3A20FF);
    if (l1_piston_state==0 && l1_piston_timer>=0.8 && fmod(t*8,2)<1) lamp = l1_hazard_yellow;
    if (l1_piston_state==1 || l1_piston_state==2) lamp = l1_laser_red;
    DrawCircle(housing.x+l1_wall/2,housing.y-12*l1_u,9*l1_u,Fade(lamp,0.35));
    DrawCircle(housing.x+l1_wall/2,housing.y-12*l1_u,6*l1_u,lamp);

    //molten pits
    for (int i=0; i<3; i++)
    {
        Vector2 p = l1_pit[i];
        float pulse = sin(t*3 + i*2);
        DrawCircleGradient(p,l1_pit_radius*1.9,Fade(l1_molten_orange,0.3+0.1*pulse),BLANK);
        DrawCircle(p.x,p.y,l1_pit_radius+5*l1_u,GetColor(0x2A1A12FF));
        DrawCircleGradient(p,l1_pit_radius,l1_molten_orange,l1_molten_dark);
        DrawCircleGradient(p,l1_pit_radius*(0.55+0.08*pulse),l1_molten_yellow,Fade(l1_molten_orange,0));
        Vector2 lump_arm = {l1_pit_radius+1*l1_u,0};
        for (int j=0; j<8; j++)
        {
            Vector2 lump = Vector2Add(p,Vector2Rotate(lump_arm,(j*45+i*20)*DEG2RAD));
            DrawCircle(lump.x,lump.y,3.5*l1_u,GetColor(0x3B2418FF));
        }
        //bubbles
        for (int j=0; j<4; j++)
        {
            float life = fmod(t*0.8 + j*0.25 + i*0.13,1.0);
            float bubble_x = p.x + sin(j*2.1 + i)*l1_pit_radius*0.5;
            float bubble_y = p.y + cos(j*1.7 + i)*l1_pit_radius*0.5;
            DrawCircle(bubble_x,bubble_y,(1.5+life*3.5)*l1_u,Fade(l1_molten_yellow,(1-life)*0.9));
        }
    }

    //l1_laser gate
    float beam_y = l1_laser.y + l1_laser.height/2;
    Rectangle left_emitter = {l1_laser.x-l1_wall+4*l1_u,beam_y-14*l1_u,l1_wall-4*l1_u,28*l1_u};
    Rectangle right_emitter = {l1_laser.x+l1_laser.width,beam_y-14*l1_u,l1_wall-4*l1_u,28*l1_u};
    DrawRectangleRec(left_emitter,l1_steel_dark);
    DrawRectangleRec(right_emitter,l1_steel_dark);
    DrawRectangleLinesEx(left_emitter,2*l1_u,BLACK);
    DrawRectangleLinesEx(right_emitter,2*l1_u,BLACK);
    Color laser_lamp = GetColor(0x4A3A20FF);
    int dashes = l1_laser.width/(16*l1_u);
    if (l1_laser_timer<1.6)
    {
        float dash_alpha = 0.3;
        if (l1_laser_timer>=1.2)
        {
            dash_alpha = 0.55;
            if (fmod(t*10,2)<1) laser_lamp = l1_hazard_yellow;
        }
        for (int j=0; j<dashes; j++)
        {
            DrawRectangle(l1_laser.x+j*16*l1_u+4*l1_u,beam_y-1*l1_u,8*l1_u,2*l1_u,Fade(l1_laser_red,dash_alpha));
        }
    }
    else
    {
        laser_lamp = l1_laser_red;
        DrawRectangle(l1_laser.x,beam_y-14*l1_u,l1_laser.width,28*l1_u,Fade(l1_laser_red,0.15));
        DrawRectangle(l1_laser.x,beam_y-7*l1_u,l1_laser.width,14*l1_u,Fade(l1_laser_red,0.3));
        DrawRectangleRec(l1_laser,l1_laser_red);
        DrawRectangle(l1_laser.x,beam_y-1*l1_u,l1_laser.width,2*l1_u,WHITE);
    }
    DrawCircle(left_emitter.x+left_emitter.width/2,beam_y,5*l1_u,laser_lamp);
    DrawCircle(right_emitter.x+right_emitter.width/2,beam_y,5*l1_u,laser_lamp);

    //l1_magnet field rings
    for (int j=0; j<3; j++)
    {
        float ring = l1_magnet_field - fmod(t*50*l1_u + j*(l1_magnet_field-l1_magnet_core)/3,l1_magnet_field-l1_magnet_core);
        DrawRing(l1_magnet,ring-1.5*l1_u,ring+1.5*l1_u,0,360,48,Fade(SKYBLUE,0.05+0.35*(1-ring/l1_magnet_field)));
    }
    DrawCircle(l1_magnet.x,l1_magnet.y,l1_magnet_field,Fade(SKYBLUE,0.04));

    //l1_magnet
    Vector2 magnet_shadow = {l1_magnet.x+3*l1_u,l1_magnet.y+4*l1_u};
    DrawRing(magnet_shadow,12*l1_u,28*l1_u,180,360,24,Fade(BLACK,0.4));
    DrawRing(l1_magnet,12*l1_u,28*l1_u,180,360,24,l1_magnet_red);
    DrawRectangle(l1_magnet.x-28*l1_u,l1_magnet.y,16*l1_u,8*l1_u,l1_magnet_red);
    DrawRectangle(l1_magnet.x+12*l1_u,l1_magnet.y,16*l1_u,8*l1_u,l1_magnet_red);
    DrawRectangle(l1_magnet.x-28*l1_u,l1_magnet.y+8*l1_u,16*l1_u,10*l1_u,l1_steel_light);
    DrawRectangle(l1_magnet.x+12*l1_u,l1_magnet.y+8*l1_u,16*l1_u,10*l1_u,l1_steel_light);
}


void l1_draw_ball_and_pot()
{
    //start plate
    Rectangle plate = {l1_start_position.x-50*l1_u,l1_start_position.y-30*l1_u,100*l1_u,60*l1_u};
    DrawRectangleRec(plate,l1_steel);
    DrawRectangleLinesEx(plate,3*l1_u,l1_steel_dark);
    Rectangle plate_band = {plate.x,plate.y+plate.height-10*l1_u,plate.width,10*l1_u};
    l1_draw_hazard_stripes(plate_band);
    DrawText("START",l1_start_position.x-MeasureText("START",18*l1_u)/2,plate.y+5*l1_u,18*l1_u,WHITE);

    //l1_pot with hazard ring
    for (int i=0; i<8; i++)
    {
        Color colour = l1_hazard_yellow;
        if (i%2==1) colour = BLACK;
        DrawRing(l1_pot,l1_radius_pot+6*l1_u,l1_radius_pot+12*l1_u,i*45,i*45+45,6,colour);
    }
    DrawCircle(l1_pot.x,l1_pot.y,l1_radius_pot+6*l1_u,l1_steel_light);
    DrawRing(l1_pot,l1_radius_pot+1*l1_u,l1_radius_pot+5*l1_u,0,360,24,l1_steel);
    DrawCircle(l1_pot.x,l1_pot.y,l1_radius_pot,BLACK);

    //flag
    Vector2 pole_top = {l1_pot.x,l1_pot.y-60*l1_u};
    DrawLineEx(l1_pot,pole_top,3*l1_u,l1_steel_light);
    for (int i=0; i<5; i++)
    {
        float wave = sin(l1_animation_time*6 - i*0.8)*2.5*l1_u;
        DrawRectangle(l1_pot.x+1*l1_u+i*6*l1_u,pole_top.y+wave,6*l1_u,18*l1_u,l1_laser_red);
    }

    //l1_ball
    DrawCircle(l1_ball.x+3*l1_u,l1_ball.y+4*l1_u,l1_radius_ball,Fade(BLACK,0.45));
    DrawCircle(l1_ball.x,l1_ball.y,l1_radius_ball,GetColor(0xEDEDEDFF));
    DrawCircleLines(l1_ball.x,l1_ball.y,l1_radius_ball,GRAY);
    DrawCircle(l1_ball.x-2*l1_u,l1_ball.y-2*l1_u,2*l1_u,WHITE);

    //aim line
    if (l1_aiming==1 && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        Vector2 mouse = {GetMouseX(),GetMouseY()};
        DrawLineEx(l1_ball,mouse,4*l1_u,Fade(WHITE,0.75));
        DrawCircle(mouse.x,mouse.y,5*l1_u,Fade(WHITE,0.75));
    }
}


void l1_draw_hud()
{
    //dark edges
    DrawRectangleGradientH(0,l1_hud_height,90*l1_u,l1_height-l1_hud_height,Fade(BLACK,0.45),BLANK);
    DrawRectangleGradientH(l1_width-90*l1_u,l1_hud_height,90*l1_u,l1_height-l1_hud_height,BLANK,Fade(BLACK,0.45));
    DrawRectangleGradientV(0,l1_height-70*l1_u,l1_width,70*l1_u,BLANK,Fade(BLACK,0.45));

    //l1_steel bar
    DrawRectangleGradientV(0,0,l1_width,l1_hud_height,l1_steel_light,l1_steel_dark);
    DrawRectangle(0,l1_hud_height-4*l1_u,l1_width,4*l1_u,BLACK);
    int rivets = l1_width/(40*l1_u);
    for (int i=0; i<rivets; i++)
    {
        DrawCircle(20*l1_u+i*40*l1_u,8*l1_u,3*l1_u,l1_steel_dark);
        DrawCircle(20*l1_u+i*40*l1_u,l1_hud_height-12*l1_u,3*l1_u,l1_steel_dark);
    }

    //text
    DrawText("LEVEL 1 - FOUNDRY",32*l1_u,20*l1_u,36*l1_u,BLACK);
    DrawText("LEVEL 1 - FOUNDRY",30*l1_u,18*l1_u,36*l1_u,l1_hazard_yellow);
    DrawText(TextFormat("STROKES %d / %d",l1_stroke,l1_stroke_limit),l1_width/2-298*l1_u,21*l1_u,32*l1_u,BLACK);
    DrawText(TextFormat("STROKES %d / %d",l1_stroke,l1_stroke_limit),l1_width/2-300*l1_u,19*l1_u,32*l1_u,WHITE);
    for (int i=0; i<l1_stroke_limit; i++)
    {
        Color pip = l1_steel_dark;
        if (i<l1_stroke) pip = l1_laser_red;
        DrawCircle(l1_width/2+10*l1_u+i*22*l1_u,35*l1_u,7*l1_u,pip);
        DrawCircleLines(l1_width/2+10*l1_u+i*22*l1_u,35*l1_u,7*l1_u,BLACK);
    }
    int help_width = MeasureText("R restart   ESC menu",26*l1_u);
    DrawText("R restart   ESC menu",l1_width-help_width-28*l1_u,24*l1_u,26*l1_u,BLACK);
    DrawText("R restart   ESC menu",l1_width-help_width-30*l1_u,22*l1_u,26*l1_u,WHITE);

    //melted or zapped message
    if (l1_message_timer>0)
    {
        if (l1_message_type==1)
        {
            DrawText("MELTED!",l1_width/2-MeasureText("MELTED!",90*l1_u)/2+4*l1_u,l1_height/2-41*l1_u,90*l1_u,Fade(BLACK,l1_message_timer));
            DrawText("MELTED!",l1_width/2-MeasureText("MELTED!",90*l1_u)/2,l1_height/2-45*l1_u,90*l1_u,Fade(l1_molten_orange,l1_message_timer));
        }
        else
        {
            DrawText("ZAPPED!",l1_width/2-MeasureText("ZAPPED!",90*l1_u)/2+4*l1_u,l1_height/2-41*l1_u,90*l1_u,Fade(BLACK,l1_message_timer));
            DrawText("ZAPPED!",l1_width/2-MeasureText("ZAPPED!",90*l1_u)/2,l1_height/2-45*l1_u,90*l1_u,Fade(l1_laser_red,l1_message_timer));
        }
    }

    //level clear or failed
    if (l1_game_state!=0)
    {
        DrawRectangle(0,0,l1_width,l1_height,Fade(BLACK,0.6));
        Rectangle panel = {l1_width/2-340*l1_u,l1_height/2-170*l1_u,680*l1_u,340*l1_u};
        DrawRectangleRec(panel,l1_steel_dark);
        Rectangle panel_band = {panel.x,panel.y,panel.width,24*l1_u};
        l1_draw_hazard_stripes(panel_band);
        DrawRectangleLinesEx(panel,6*l1_u,l1_steel_light);
        if (l1_game_state==1)
        {
            DrawText("LEVEL CLEAR!",l1_width/2-MeasureText("LEVEL CLEAR!",80*l1_u)/2,panel.y+60*l1_u,80*l1_u,l1_hazard_yellow);
        }
        else
        {
            DrawText("FAILED",l1_width/2-MeasureText("FAILED",80*l1_u)/2,panel.y+60*l1_u,80*l1_u,l1_laser_red);
        }
        int strokes_width = MeasureText(TextFormat("Strokes: %d / %d",l1_stroke,l1_stroke_limit),40*l1_u);
        DrawText(TextFormat("Strokes: %d / %d",l1_stroke,l1_stroke_limit),l1_width/2-strokes_width/2,panel.y+170*l1_u,40*l1_u,WHITE);
        DrawText("R play again   ESC menu",l1_width/2-MeasureText("R play again   ESC menu",30*l1_u)/2,panel.y+250*l1_u,30*l1_u,l1_steel_light);
    }
}


//level 1: keep the obstacles moving with no ball (intro and menu)
void l1_background_step(float dt)
{
    l1_animation_time = l1_animation_time + dt;
    l1_update_obstacles(dt);
}


//level 1: draw everything except the scoreboard (intro and menu)
void l1_draw_scene()
{

    l1_draw_background();
    l1_draw_path_arrows();
    l1_draw_walls();
    l1_draw_obstacles();
    l1_draw_ball_and_pot();

}


//level 1: set the screen size, load pictures, start the level
void l1_start(int screen_width, int screen_height)
{
    l1_width = screen_width;
    l1_height = screen_height;

    //every size is N*l1_u, so it looks the same on any screen
    l1_u = l1_height/1080.0;
    if (l1_width/1920.0 < l1_u) l1_u = l1_width/1920.0;
    l1_reset_level();

}


//level 1: one frame of input and movement
void l1_update()
{
    float dt = GetFrameTime();
    if (dt>1.0/30) dt = 1.0/30;
    l1_animation_time = l1_animation_time + dt;
    if (l1_message_timer>0) l1_message_timer = l1_message_timer - dt;

    //restart
    if (IsKeyPressed(KEY_R)) l1_reset_level();

    //shooting (the click has to start while the l1_ball is still)
    int ball_stopped = 0;
    if (l1_speed.x==0 && l1_speed.y==0) ball_stopped = 1;
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && ball_stopped==1 && l1_game_state==0) l1_aiming = 1;
    if (l1_game_state!=0) l1_aiming = 0;
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && l1_aiming==1)
    {
        l1_aiming = 0;
        Vector2 mouse = {GetMouseX(),GetMouseY()};
        Vector2 drag = Vector2Subtract(l1_ball,mouse);
        if (ball_stopped==1 && l1_stroke<l1_stroke_limit && Vector2Length(drag)>=2*l1_radius_ball)
        {
            l1_last_shot_position = l1_ball;
            l1_speed = Vector2ClampValue(Vector2Scale(drag,3.125),0,l1_max_speed*l1_u);
            l1_stroke++;
        }
    }

    //moving everything in 4 small steps, so nothing jumps through anything
    for (int i=0; i<4; i++)
    {
        l1_update_obstacles(dt/4*obstacle_speed);
        if (l1_game_state==0) l1_update_ball(dt/4);
    }

    //out of strokes
    if (l1_game_state==0 && l1_stroke>=l1_stroke_limit && l1_speed.x==0 && l1_speed.y==0) l1_game_state = 2;


}


//level 1: one frame of drawing
void l1_draw()
{

    l1_draw_background();
    l1_draw_path_arrows();
    l1_draw_walls();
    l1_draw_obstacles();
    l1_draw_ball_and_pot();
    draw_straight_preview(l1_ball,l1_aiming,l1_radius_ball,l1_max_speed*l1_u,l1_u);
    l1_draw_hud();

}


void l1_unload()
{
}
