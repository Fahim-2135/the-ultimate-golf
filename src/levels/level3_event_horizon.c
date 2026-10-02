//==================== LEVEL 3 ====================

//full global
int l3_width = 1920;
int l3_height = 1080;
float l3_u = 1;
int l3_stroke_base = 20;
int l3_stroke_limit = 20;
#define l3_max_speed 650

//colours
Color l3_brick = {139,58,43,255};
Color l3_brick_dark = {110,44,32,255};
Color l3_mortar = {58,51,48,255};
Color l3_steel = {90,95,102,255};
Color l3_steel_light = {138,144,153,255};
Color l3_steel_dark = {46,49,54,255};
Color l3_floor_colour = {74,74,72,255};
Color l3_hazard_yellow = {232,185,35,255};
Color l3_rust = {181,84,28,255};
Color l3_rust_dark = {107,46,14,255};
Color l3_molten_orange = {255,106,0,255};
Color l3_molten_yellow = {255,208,0,255};
Color l3_molten_dark = {139,26,0,255};
Color l3_laser_red = {255,32,48,255};
Color l3_magnet_red = {192,40,45,255};
Color l3_space_dark = {6,8,22,255};
Color l3_neon_cyan = {80,230,255,255};
Color l3_neon_purple = {190,90,255,255};
Color l3_neon_orange = {255,150,40,255};
Color l3_neon_green = {110,255,120,255};
Color l3_panel_blue = {50,100,210,255};

//pictures
Texture2D l3_asteroid_texture;
Texture2D l3_ufo_texture;

//l3_ball and l3_pot
Vector2 l3_ball;
Vector2 l3_speed;
float l3_radius_ball;
Vector2 l3_pot;
float l3_radius_pot;
Vector2 l3_start_position;
Vector2 l3_last_shot_position;
int l3_stroke = 0;
int l3_game_state = 0;
int l3_aiming = 0;
float l3_message_timer = 0;
int l3_message_type = 0;
float l3_animation_time = 0;
float l3_hud_height;
Vector2 l3_no_speed = {0,0};

//walkways in path order: standing on one is safe, falling off is lost in space
Rectangle l3_platform[11];

//l3_vacuum strips (almost no friction) and solar wind (the old rip current, pushes towards the void)
Rectangle l3_vacuum[2];
Rectangle l3_solar_wind;
Vector2 l3_solar_wind_push;

//gravity: planets (solid core) and the black hole, it only bends a moving l3_ball
Vector2 l3_planet[2];
float l3_planet_core[2];
float l3_planet_field;
float l3_planet_pull;
Vector2 l3_black_hole;
float l3_black_hole_field;
float l3_black_hole_pull;

//wormholes, one way from in to out, the l3_ball leaves the way the exit faces
Vector2 l3_wormhole_in[3];
Vector2 l3_wormhole_out[3];
float l3_wormhole_angle[3];
float l3_wormhole_radius;
float l3_wormhole_cooldown = 0;

//asteroids drifting back and forth between two points
Vector2 l3_asteroid[4];
Vector2 l3_asteroid_start[4];
Vector2 l3_asteroid_end[4];
Vector2 l3_asteroid_velocity[4];
float l3_asteroid_t[4];
float l3_asteroid_time[4];
float l3_asteroid_radius[4];
int l3_asteroid_direction[4];

//laser gates (from level 1): off 1.2s, warning 0.4s, on 1.4s
Rectangle l3_laser_gate[2];
float l3_laser_clock[2];

//spinning satellite (the level 1 l3_fan)
Vector2 l3_fan;
float l3_fan_angle = 0;
float l3_fan_spin = 90;
float l3_fan_blade_length;
float l3_fan_blade_thickness;
float l3_fan_hub_radius;

//energy bumpers
Vector2 l3_bumper[4];
float l3_bumper_radius;
float l3_bumper_hit[4];

//lost in space effect
Vector2 l3_lost_position;
float l3_lost_timer = 0;

//fly-through events: 0 nothing, 1 l3_comet, 2 l3_meteor shower, 3 l3_ufo (the first 1.5s of each is a warning)
int l3_event_type = 0;
float l3_event_timer = 0;
float l3_next_event = 10;
int l3_shower_done = 0;
int l3_ufo_done = 0;
Vector2 l3_event_start;
Vector2 l3_event_direction;
float l3_event_speed;
Vector2 l3_comet;
Vector2 l3_meteor[5];
Vector2 l3_ufo;
float l3_beam_timer = 0;
int l3_beam_used = 0;
int l3_abducted = 0;
float l3_carry_timer = 0;
int l3_abducted_section = 0;

//stars
Vector2 l3_star[150];
float l3_star_size[150];


//a rectangle in design units (a 1920 x 1080 screen), made to fit the real screen
Rectangle l3_make_rect(float x, float y, float w, float h)
{
    Rectangle rec = {x*l3_u,y*l3_u,w*l3_u,h*l3_u};
    return rec;
}

//a point in design units
Vector2 l3_make_point(float x, float y)
{
    Vector2 point = {x*l3_u,y*l3_u};
    return point;
}


//stars at random places, 3 sizes (bigger ones drift faster)
void l3_make_stars()
{
    for (int i=0; i<150; i++)
    {
        l3_star[i].x = GetRandomValue(0,l3_width);
        l3_star[i].y = GetRandomValue(0,l3_height);
        l3_star_size[i] = GetRandomValue(1,3)*l3_u;
    }
}


void l3_reset_level()
{
    //sizes
    l3_hud_height = 70*l3_u;
    l3_radius_ball = 7*l3_u;
    l3_radius_pot = 11*l3_u;

    //walkways, spiralling in to the black hole
    l3_platform[0] = l3_make_rect(60,940,1800,100);    //outer ring bottom (start)
    l3_platform[1] = l3_make_rect(1760,110,100,930);   //outer ring right
    l3_platform[2] = l3_make_rect(60,110,1800,100);    //outer ring top
    l3_platform[3] = l3_make_rect(60,110,100,750);     //outer ring left
    l3_platform[4] = l3_make_rect(60,760,1620,100);    //inner ring bottom
    l3_platform[5] = l3_make_rect(1580,290,100,570);   //inner ring right
    l3_platform[6] = l3_make_rect(240,290,1440,100);   //inner ring top
    l3_platform[7] = l3_make_rect(240,290,100,390);    //inner ring left
    l3_platform[8] = l3_make_rect(240,470,560,210);    //centre left
    l3_platform[9] = l3_make_rect(800,470,320,60);     //narrow bridge past the black hole
    l3_platform[10] = l3_make_rect(1120,470,380,210);  //l3_pot l3_platform

    l3_vacuum[0] = l3_make_rect(500,940,350,100);
    l3_vacuum[1] = l3_make_rect(500,290,350,100);
    l3_solar_wind = l3_make_rect(60,400,100,180);
    l3_solar_wind_push = l3_make_point(-280,0);

    //gravity
    l3_planet[0] = l3_make_point(600,720);
    l3_planet[1] = l3_make_point(1450,250);
    l3_planet_core[0] = 28*l3_u;
    l3_planet_core[1] = 28*l3_u;
    l3_planet_field = 200*l3_u;
    l3_planet_pull = 700*l3_u;
    l3_black_hole = l3_make_point(960,640);
    l3_black_hole_field = 240*l3_u;
    l3_black_hole_pull = 500*l3_u;

    //wormholes: purple shortcut, cyan to the l3_pot, orange trap
    l3_wormhole_radius = 26*l3_u;
    l3_wormhole_in[0] = l3_make_point(1840,700);
    l3_wormhole_out[0] = l3_make_point(1300,340);
    l3_wormhole_angle[0] = 180;
    l3_wormhole_in[1] = l3_make_point(700,600);
    l3_wormhole_out[1] = l3_make_point(1170,600);
    l3_wormhole_angle[1] = 0;
    l3_wormhole_in[2] = l3_make_point(1300,810);
    l3_wormhole_out[2] = l3_make_point(960,250);
    l3_wormhole_angle[2] = 90;
    l3_wormhole_cooldown = 0;

    //asteroids
    l3_asteroid_start[0] = l3_make_point(700,50);
    l3_asteroid_end[0] = l3_make_point(700,250);
    l3_asteroid_radius[0] = 30*l3_u;
    l3_asteroid_time[0] = 2.4;
    l3_asteroid_start[1] = l3_make_point(1250,250);
    l3_asteroid_end[1] = l3_make_point(1250,50);
    l3_asteroid_radius[1] = 26*l3_u;
    l3_asteroid_time[1] = 2.0;
    l3_asteroid_start[2] = l3_make_point(1000,250);
    l3_asteroid_end[2] = l3_make_point(1000,430);
    l3_asteroid_radius[2] = 22*l3_u;
    l3_asteroid_time[2] = 1.8;
    l3_asteroid_start[3] = l3_make_point(420,430);
    l3_asteroid_end[3] = l3_make_point(420,720);
    l3_asteroid_radius[3] = 30*l3_u;
    l3_asteroid_time[3] = 3.0;
    for (int i=0; i<4; i++)
    {
        l3_asteroid_t[i] = 0;
        l3_asteroid_direction[i] = 1;
        l3_asteroid[i] = l3_asteroid_start[i];
        l3_asteroid_velocity[i] = l3_no_speed;
        l3_bumper_hit[i] = 0;
    }

    //laser gates across the two right hand walkways
    l3_laser_gate[0] = l3_make_rect(1760,450,100,8);
    l3_laser_gate[1] = l3_make_rect(1580,600,100,8);
    l3_laser_clock[0] = 0;
    l3_laser_clock[1] = 1.5;

    //satellite
    l3_fan = l3_make_point(1200,990);
    l3_fan_blade_length = 150*l3_u;
    l3_fan_blade_thickness = 12*l3_u;
    l3_fan_hub_radius = 16*l3_u;
    l3_fan_angle = 0;

    //energy bumpers
    l3_bumper_radius = 20*l3_u;
    l3_bumper[0] = l3_make_point(110,700);
    l3_bumper[1] = l3_make_point(1630,420);
    l3_bumper[2] = l3_make_point(1330,520);
    l3_bumper[3] = l3_make_point(1330,640);

    //fly-through events
    l3_event_type = 0;
    l3_next_event = 10;
    l3_shower_done = 0;
    l3_ufo_done = 0;
    l3_abducted = 0;
    l3_beam_timer = 0;
    l3_beam_used = 0;

    //l3_ball and l3_pot
    l3_start_position = l3_make_point(140,990);
    l3_ball = l3_start_position;
    l3_last_shot_position = l3_start_position;
    l3_speed.x = 0;
    l3_speed.y= 0;
    l3_pot = l3_make_point(1400,575);
    l3_stroke = 0;
    l3_game_state = 0;
    l3_aiming = 0;
    l3_message_timer = 0;
    l3_lost_timer = 0;
}


//bounce off a round thing: bounce is 1 for normal and more for bouncy, circle_speed is how fast it moves
int l3_bounce_off_circle(Vector2 center, float radius, float bounce, Vector2 circle_speed)
{
    Vector2 normal = Vector2Subtract(l3_ball,center);
    float distance = Vector2Length(normal);
    if (distance < radius + l3_radius_ball)
    {
        if (distance==0) normal.y = -1;
        normal = Vector2Normalize(normal);
        l3_ball = Vector2Add(center,Vector2Scale(normal,radius + l3_radius_ball));
        Vector2 relative_speed = Vector2Subtract(l3_speed,circle_speed);
        if (Vector2DotProduct(relative_speed,normal)<0)
        {
            relative_speed = Vector2Scale(Vector2Reflect(relative_speed,normal),bounce);
            l3_speed = Vector2ClampValue(Vector2Add(relative_speed,circle_speed),0,l3_max_speed*l3_u);
            return 1;
        }
    }
    return 0;
}


//bounce off a turned rectangle (diamond, l3_fan blades), spin is in degrees per second
void l3_bounce_off_rotated_rectangle(Vector2 center, float rec_width, float rec_height, float angle, float spin)
{
    //look at the l3_ball as if the rectangle was not turned
    Vector2 local_ball = Vector2Rotate(Vector2Subtract(l3_ball,center),-angle*DEG2RAD);
    Rectangle rec = {-rec_width/2,-rec_height/2,rec_width,rec_height};
    if (CheckCollisionCircleRec(local_ball,l3_radius_ball,rec))
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
        l3_ball = Vector2Add(collision_point,Vector2Scale(normal,l3_radius_ball));

        //l3_speed of the blade at the point it touches the l3_ball
        Vector2 arm = Vector2Subtract(collision_point,center);
        Vector2 rec_speed = {-arm.y*spin*DEG2RAD, arm.x*spin*DEG2RAD};

        Vector2 relative_speed = Vector2Subtract(l3_speed,rec_speed);
        if (Vector2DotProduct(relative_speed,normal)<0)
        {
            relative_speed = Vector2Reflect(relative_speed,normal);
            l3_speed = Vector2ClampValue(Vector2Add(relative_speed,rec_speed),0,l3_max_speed*l3_u);
        }
    }
}


//pull towards a centre, stronger the closer the l3_ball is
Vector2 l3_gravity_pull(Vector2 center, float field, float strength, float dt)
{
    Vector2 pull = {0,0};
    Vector2 to_center = Vector2Subtract(center,l3_ball);
    float distance = Vector2Length(to_center);
    if (distance<field && distance>1*l3_u)
    {
        float closeness = 1 - distance/field;
        pull = Vector2Scale(Vector2Normalize(to_center),strength*closeness*closeness*dt);
    }
    return pull;
}


//is this point on a walkway
int l3_on_safe_ground(Vector2 point)
{
    for (int i=0; i<11; i++)
    {
        if (CheckCollisionPointRec(point,l3_platform[i])) return 1;
    }
    return 0;
}


//a random point on a walkway, 20 units inside its edges
Vector2 l3_random_point_on(Rectangle rec)
{
    Vector2 point;
    point.x = rec.x + 20*l3_u + GetRandomValue(0,1000)/1000.0*(rec.width-40*l3_u);
    point.y = rec.y + 20*l3_u + GetRandomValue(0,1000)/1000.0*(rec.height-40*l3_u);
    return point;
}


//which walkway a point is on (the later one if two overlap)
int l3_section_of(Vector2 point)
{
    int section = 0;
    for (int i=0; i<11; i++)
    {
        if (CheckCollisionPointRec(point,l3_platform[i])) section = i;
    }
    return section;
}


//a spot where a dropped l3_ball won't land in trouble
int l3_spot_is_clear(Vector2 spot)
{
    if (l3_on_safe_ground(spot)==0) return 0;
    for (int i=0; i<2; i++)
    {
        if (Vector2Distance(spot,l3_planet[i]) < l3_planet_core[i]+30*l3_u) return 0;
        if (CheckCollisionCircleRec(spot,30*l3_u,l3_laser_gate[i])) return 0;
    }
    for (int i=0; i<4; i++)
    {
        if (Vector2Distance(spot,l3_bumper[i]) < l3_bumper_radius+30*l3_u) return 0;
        if (Vector2Distance(spot,l3_asteroid[i]) < l3_asteroid_radius[i]+30*l3_u) return 0;
    }
    for (int i=0; i<3; i++)
    {
        if (Vector2Distance(spot,l3_wormhole_in[i]) < l3_wormhole_radius+30*l3_u) return 0;
    }
    if (Vector2Distance(spot,l3_fan) < l3_fan_blade_length/2+30*l3_u) return 0;
    if (CheckCollisionPointRec(spot,l3_solar_wind)) return 0;
    if (Vector2Distance(spot,l3_pot) < 40*l3_u) return 0;
    return 1;
}


void l3_update_obstacles(float dt)
{
    //asteroids drifting back and forth
    for (int i=0; i<4; i++)
    {
        l3_asteroid_t[i] = l3_asteroid_t[i] + l3_asteroid_direction[i]*dt/l3_asteroid_time[i];
        if (l3_asteroid_t[i]>=1)
        {
            l3_asteroid_t[i] = 1;
            l3_asteroid_direction[i] = -1;
        }
        if (l3_asteroid_t[i]<=0)
        {
            l3_asteroid_t[i] = 0;
            l3_asteroid_direction[i] = 1;
        }
        l3_asteroid[i] = Vector2Lerp(l3_asteroid_start[i],l3_asteroid_end[i],l3_asteroid_t[i]);
        l3_asteroid_velocity[i] = Vector2Scale(Vector2Subtract(l3_asteroid_end[i],l3_asteroid_start[i]),l3_asteroid_direction[i]/l3_asteroid_time[i]);
        if (l3_bumper_hit[i]>0) l3_bumper_hit[i] = l3_bumper_hit[i] - dt;
    }

    //l3_fan spinning
    l3_fan_angle = l3_fan_angle + l3_fan_spin*dt;
    if (l3_fan_angle>=360) l3_fan_angle = l3_fan_angle - 360;

    //laser clocks
    for (int i=0; i<2; i++)
    {
        l3_laser_clock[i] = l3_laser_clock[i] + dt;
        if (l3_laser_clock[i]>=3.0) l3_laser_clock[i] = l3_laser_clock[i] - 3.0;
    }

    if (l3_wormhole_cooldown>0) l3_wormhole_cooldown = l3_wormhole_cooldown - dt;
    if (l3_lost_timer>0) l3_lost_timer = l3_lost_timer - dt;
}


//after falling off or getting zapped, the l3_ball goes back to where it was shot from
void l3_send_ball_back()
{
    l3_ball = l3_last_shot_position;

    //not right in an l3_asteroid's path, or it gets knocked off again and again
    for (int i=0; i<4; i++)
    {
        Vector2 path = Vector2Subtract(l3_asteroid_end[i],l3_asteroid_start[i]);
        float along = Vector2DotProduct(Vector2Subtract(l3_ball,l3_asteroid_start[i]),path)/Vector2DotProduct(path,path);
        along = Clamp(along,0,1);
        Vector2 closest = Vector2Add(l3_asteroid_start[i],Vector2Scale(path,along));
        Vector2 away = Vector2Subtract(l3_ball,closest);
        float gap = l3_asteroid_radius[i] + l3_radius_ball + 2*l3_u;
        if (Vector2Length(away)<gap)
        {
            Vector2 side = {-path.y,path.x};
            side = Vector2Normalize(side);
            if (Vector2DotProduct(away,side)<0) side = Vector2Negate(side);
            l3_ball = Vector2Add(closest,Vector2Scale(side,gap));
        }
    }

    //not where the satellite blades sweep, or it gets knocked off again and again (the level 1 piston rule)
    float sweep = l3_fan_blade_length/2 + l3_radius_ball + 2*l3_u;
    if (Vector2Distance(l3_ball,l3_fan)<sweep)
    {
        if (l3_ball.x<l3_fan.x) l3_ball.x = l3_fan.x - sweep;
        else l3_ball.x = l3_fan.x + sweep;
    }

    //not inside a laser gate (level 1 rule), not off a walkway
    for (int i=0; i<2; i++)
    {
        if (CheckCollisionCircleRec(l3_ball,l3_radius_ball,l3_laser_gate[i])) l3_ball = l3_start_position;
    }
    if (l3_on_safe_ground(l3_ball)==0) l3_ball = l3_start_position;

    l3_speed.x = 0;
    l3_speed.y = 0;
}


//a l3_comet or l3_meteor hits the l3_ball and knocks it along
void l3_knock_ball(Vector2 center, float radius)
{
    if (l3_game_state!=0 || l3_abducted==1) return;
    Vector2 normal = Vector2Subtract(l3_ball,center);
    float distance = Vector2Length(normal);
    if (distance < radius + l3_radius_ball)
    {
        if (distance==0) normal = l3_event_direction;
        normal = Vector2Normalize(normal);
        l3_ball = Vector2Add(center,Vector2Scale(normal,radius + l3_radius_ball));
        l3_speed = Vector2ClampValue(Vector2Add(Vector2Scale(l3_event_direction,400*l3_u),Vector2Scale(normal,150*l3_u)),0,l3_max_speed*l3_u);
    }
}


//the l3_ufo lets go: a random clear spot on the walkway before or after the one the l3_ball was on
void l3_drop_ball()
{
    int section = l3_abducted_section - 1;
    if (GetRandomValue(0,1)==1) section = l3_abducted_section + 1;
    if (section<0) section = 1;
    if (section>10) section = 9;
    if (section==9) section = 8 + 2*GetRandomValue(0,1);    //never the narrow bridge
    l3_ball = l3_last_shot_position;
    for (int tries=0; tries<30; tries++)
    {
        Vector2 spot = l3_random_point_on(l3_platform[section]);
        if (l3_spot_is_clear(spot))
        {
            l3_ball = spot;
            break;
        }
    }
    l3_speed.x = 0;
    l3_speed.y = 0;
    l3_abducted = 0;
    l3_beam_timer = 0;
}


//pick a new fly-through, the l3_meteor shower and the l3_ufo only come once
void l3_start_event()
{
    l3_event_type = GetRandomValue(1,3);
    if (l3_event_type==2 && l3_shower_done==1) l3_event_type = 1;
    if (l3_event_type==3 && l3_ufo_done==1) l3_event_type = 1;
    l3_event_timer = 0;

    if (l3_event_type==3)
    {
        //the l3_ufo flies straight across, over one of the long walkways
        l3_ufo_done = 1;
        l3_beam_timer = 0;
        l3_beam_used = 0;
        int lane = GetRandomValue(0,3)*2;
        l3_event_start.y = l3_platform[lane].y + l3_platform[lane].height/2;
        l3_event_start.x = -120*l3_u;
        l3_event_direction.x = 1;
        l3_event_direction.y = 0;
        if (GetRandomValue(0,1)==1)
        {
            l3_event_start.x = l3_width + 120*l3_u;
            l3_event_direction.x = -1;
        }
        l3_event_speed = 170*l3_u;
        l3_ufo = l3_event_start;
        return;
    }

    //l3_comet or l3_meteor shower: aimed at a random walkway spot from a random side, starting just off the screen
    if (l3_event_type==2) l3_shower_done = 1;
    Vector2 target = l3_random_point_on(l3_platform[GetRandomValue(0,10)]);
    float angle = GetRandomValue(0,359)*DEG2RAD;
    l3_event_direction.x = cos(angle);
    l3_event_direction.y = sin(angle);
    float back_x = 10000;
    float back_y = 10000;
    if (l3_event_direction.x>0.01) back_x = target.x/l3_event_direction.x;
    if (l3_event_direction.x<-0.01) back_x = (l3_width-target.x)/(-l3_event_direction.x);
    if (l3_event_direction.y>0.01) back_y = target.y/l3_event_direction.y;
    if (l3_event_direction.y<-0.01) back_y = (l3_height-target.y)/(-l3_event_direction.y);
    float back = back_x;
    if (back_y<back) back = back_y;
    l3_event_start = Vector2Subtract(target,Vector2Scale(l3_event_direction,back+60*l3_u));
    l3_event_speed = 900*l3_u;
    if (l3_event_type==2) l3_event_speed = 650*l3_u;
    l3_comet = l3_event_start;
}


void l3_update_events(float dt)
{
    //waiting for the next one
    if (l3_event_type==0)
    {
        l3_next_event = l3_next_event - dt;
        if (l3_next_event<=0) l3_start_event();
        return;
    }

    l3_event_timer = l3_event_timer + dt;
    float flying = l3_event_timer - 1.5;
    if (flying<0) return;

    //l3_comet
    if (l3_event_type==1)
    {
        l3_comet = Vector2Add(l3_event_start,Vector2Scale(l3_event_direction,l3_event_speed*flying));
        l3_knock_ball(l3_comet,12*l3_u);
    }

    //l3_meteor shower, 5 rocks side by side, a quarter of a second apart
    if (l3_event_type==2)
    {
        for (int i=0; i<5; i++)
        {
            Vector2 side = {-l3_event_direction.y*(i-2)*45*l3_u, l3_event_direction.x*(i-2)*45*l3_u};
            float travelled = l3_event_speed*(flying - i*0.25);
            l3_meteor[i] = Vector2Add(Vector2Add(l3_event_start,side),Vector2Scale(l3_event_direction,travelled));
            if (travelled>0) l3_knock_ball(l3_meteor[i],10*l3_u);
        }
    }

    //l3_ufo, beams up a l3_ball that sits still under it
    if (l3_event_type==3)
    {
        l3_ufo = Vector2Add(l3_event_start,Vector2Scale(l3_event_direction,l3_event_speed*flying));
        if (l3_abducted==0 && l3_beam_used==0 && l3_game_state==0)
        {
            if (l3_speed.x==0 && l3_speed.y==0 && Vector2Distance(l3_ball,l3_ufo)<80*l3_u)
            {
                l3_beam_timer = l3_beam_timer + dt;
                if (l3_beam_timer>=0.8)
                {
                    l3_abducted = 1;
                    l3_beam_used = 1;
                    l3_carry_timer = 1;
                    l3_aiming = 0;
                    l3_abducted_section = l3_section_of(l3_ball);
                    l3_message_type = 3;
                    l3_message_timer = 1;
                }
            }
            else l3_beam_timer = 0;
        }
        if (l3_abducted==1)
        {
            l3_ball = l3_ufo;
            l3_speed.x = 0;
            l3_speed.y = 0;
            l3_carry_timer = l3_carry_timer - dt;
            if (l3_carry_timer<=0) l3_drop_ball();
        }
    }

    //far enough to be off the screen: over, wait for the next one
    float travelled = l3_event_speed*flying;
    float needed = l3_width + l3_height + 400*l3_u;
    if (l3_event_type==2) travelled = l3_event_speed*(flying - 1);
    if (l3_event_type==3) needed = l3_width + 240*l3_u;
    if (travelled>needed)
    {
        if (l3_abducted==1) l3_drop_ball();
        l3_event_type = 0;
        l3_next_event = GetRandomValue(12,22);
    }
}


void l3_update_ball(float dt)
{
    int pushed = 0;

    //solar wind (the old rip current)
    if (CheckCollisionPointRec(l3_ball,l3_solar_wind))
    {
        l3_speed = Vector2Add(l3_speed,Vector2Scale(l3_solar_wind_push,dt));
        pushed = 1;
    }

    //gravity from the planets and the black hole, only bends a moving l3_ball
    if (l3_speed.x!=0 || l3_speed.y!=0)
    {
        for (int i=0; i<2; i++)
        {
            l3_speed = Vector2Add(l3_speed,l3_gravity_pull(l3_planet[i],l3_planet_field,l3_planet_pull,dt));
        }
        l3_speed = Vector2Add(l3_speed,l3_gravity_pull(l3_black_hole,l3_black_hole_field,l3_black_hole_pull,dt));
    }

    //moving
    l3_ball = Vector2Add(l3_ball,Vector2Scale(l3_speed,dt));

    //proportional deceleration (almost no friction in a l3_vacuum strip)
    float friction = 110*l3_u;
    for (int i=0; i<2; i++)
    {
        if (CheckCollisionPointRec(l3_ball,l3_vacuum[i])) friction = 25*l3_u;
    }
    float ball_speed = Vector2Length(l3_speed);
    if (ball_speed>0)
    {
        float new_speed = ball_speed - friction*dt;
        if (new_speed<0) new_speed = 0;
        l3_speed = Vector2Scale(l3_speed,new_speed/ball_speed);
    }
    if (pushed==0 && Vector2Length(l3_speed)<6*l3_u)
    {
        l3_speed.x = 0;
        l3_speed.y= 0;
    }

    //wormholes, one way, the l3_ball comes out the way the exit faces
    if (l3_wormhole_cooldown<=0)
    {
        for (int i=0; i<3; i++)
        {
            if (Vector2Distance(l3_ball,l3_wormhole_in[i])<l3_wormhole_radius)
            {
                Vector2 facing = {cos(l3_wormhole_angle[i]*DEG2RAD),sin(l3_wormhole_angle[i]*DEG2RAD)};
                float ball_speed = Vector2Length(l3_speed);
                if (ball_speed<80*l3_u) ball_speed = 80*l3_u;
                l3_ball = Vector2Add(l3_wormhole_out[i],Vector2Scale(facing,l3_wormhole_radius + l3_radius_ball + 2*l3_u));
                l3_speed = Vector2Scale(facing,ball_speed);
                l3_wormhole_cooldown = 0.5;
                break;
            }
        }
    }

    //lost in space (checked before the collisions, so if something is on the old spot it pushes the l3_ball out)
    if (l3_on_safe_ground(l3_ball)==0)
    {
        l3_lost_position = l3_ball;
        l3_lost_timer = 0.5;
        l3_send_ball_back();
        l3_message_type = 1;
        l3_message_timer = 1;
    }

    //laser gates (from level 1)
    for (int i=0; i<2; i++)
    {
        if (l3_laser_clock[i]>=1.6 && CheckCollisionCircleRec(l3_ball,l3_radius_ball,l3_laser_gate[i]))
        {
            l3_send_ball_back();
            l3_message_type = 2;
            l3_message_timer = 1;
        }
    }

    //l3_planet cores, energy bumpers and asteroids
    for (int i=0; i<2; i++)
    {
        l3_bounce_off_circle(l3_planet[i],l3_planet_core[i],1,l3_no_speed);
    }
    for (int i=0; i<4; i++)
    {
        if (l3_bounce_off_circle(l3_bumper[i],l3_bumper_radius,1.3,l3_no_speed)) l3_bumper_hit[i] = 0.15;
        l3_bounce_off_circle(l3_asteroid[i],l3_asteroid_radius[i],1,l3_asteroid_velocity[i]);
    }

    //spinning satellite (the level 1 l3_fan)
    l3_bounce_off_circle(l3_fan,l3_fan_hub_radius,1,l3_no_speed);
    l3_bounce_off_rotated_rectangle(l3_fan,l3_fan_blade_length,l3_fan_blade_thickness,l3_fan_angle,l3_fan_spin);
    l3_bounce_off_rotated_rectangle(l3_fan,l3_fan_blade_length,l3_fan_blade_thickness,l3_fan_angle+90,l3_fan_spin);

    //score
    if ((l3_ball.x>l3_pot.x-3*l3_radius_pot/4) && (l3_ball.x<l3_pot.x+3*l3_radius_pot/4) && (l3_ball.y>l3_pot.y-3*l3_radius_pot/4) && (l3_ball.y<l3_pot.y+3*l3_radius_pot/4))
    {
        l3_ball = l3_pot;
        l3_speed.x = 0;
        l3_speed.y= 0;
        l3_game_state = 1;
    }
}


//yellow and black bands, across the long side
void l3_draw_hazard_stripes(Rectangle rec)
{
    DrawRectangleRec(rec,l3_hazard_yellow);
    float band = 6*l3_u;
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

void l3_draw_space()
{
    float t = l3_animation_time;
    DrawRectangleGradientV(0,0,l3_width,l3_height,l3_space_dark,GetColor(0x140A2AFF));

    //nebula clouds
    DrawCircleGradient(l3_make_point(420,300),420*l3_u,Fade(l3_neon_purple,0.12),BLANK);
    DrawCircleGradient(l3_make_point(1500,820),480*l3_u,Fade(l3_panel_blue,0.14),BLANK);
    DrawCircleGradient(l3_make_point(1650,200),300*l3_u,Fade(l3_neon_cyan,0.07),BLANK);
    DrawCircleGradient(l3_make_point(900,950),350*l3_u,Fade(PINK,0.06),BLANK);

    //stars drifting slowly to the left (bigger ones faster), all twinkling
    for (int i=0; i<150; i++)
    {
        float x = fmod(l3_star[i].x - t*l3_star_size[i]*6 + l3_width*100,l3_width);
        float twinkle = 0.5 + 0.5*sin(t*2 + i);
        DrawCircle(x,l3_star[i].y,l3_star_size[i]*0.7,Fade(WHITE,0.3+0.6*twinkle));
    }

    //black hole: glow, spinning bright rings, dark middle
    DrawCircleGradient(l3_black_hole,l3_black_hole_field,Fade(l3_neon_purple,0.22),BLANK);
    for (int j=0; j<5; j++)
    {
        float ring = 34*l3_u + j*13*l3_u;
        float angle = t*(220 - j*35) + j*60;
        Color ring_colour = l3_neon_orange;
        if (j%2==1) ring_colour = l3_neon_purple;
        DrawRing(l3_black_hole,ring-2*l3_u,ring+2*l3_u,angle,angle+250,32,Fade(ring_colour,0.8-j*0.12));
    }
    DrawCircleGradient(l3_black_hole,40*l3_u,BLACK,Fade(BLACK,0));
    DrawCircle(l3_black_hole.x,l3_black_hole.y,26*l3_u,BLACK);
    DrawRing(l3_black_hole,26*l3_u,28*l3_u,0,360,48,Fade(WHITE,0.5));
}


void l3_draw_walkways()
{
    float t = l3_animation_time;

    //glow, shadow and edge lights first, then the walkways on top, so they only show on the outside
    for (int i=0; i<11; i++)
    {
        Rectangle p = l3_platform[i];
        Rectangle glow = {p.x-6*l3_u,p.y-6*l3_u,p.width+12*l3_u,p.height+12*l3_u};
        DrawRectangleRec(glow,Fade(l3_neon_cyan,0.18));
        Rectangle shadow = {p.x+8*l3_u,p.y+10*l3_u,p.width,p.height};
        DrawRectangleRec(shadow,Fade(BLACK,0.5));
        int lights_x = p.width/(60*l3_u);
        int lights_y = p.height/(60*l3_u);
        for (int j=0; j<=lights_x; j++)
        {
            float blink = 0.3 + 0.7*(sin(t*3 - j*0.7 - i)>0.6);
            DrawCircle(p.x+j*60*l3_u,p.y-4*l3_u,2.5*l3_u,Fade(l3_neon_cyan,blink));
            DrawCircle(p.x+j*60*l3_u,p.y+p.height+4*l3_u,2.5*l3_u,Fade(l3_neon_cyan,blink));
        }
        for (int j=0; j<=lights_y; j++)
        {
            float blink = 0.3 + 0.7*(sin(t*3 - j*0.7 - i)>0.6);
            DrawCircle(p.x-4*l3_u,p.y+j*60*l3_u,2.5*l3_u,Fade(l3_neon_cyan,blink));
            DrawCircle(p.x+p.width+4*l3_u,p.y+j*60*l3_u,2.5*l3_u,Fade(l3_neon_cyan,blink));
        }
    }

    //l3_steel walkways with panel lines lined up with the screen, so joins match
    for (int i=0; i<11; i++)
    {
        Rectangle p = l3_platform[i];
        DrawRectangleRec(p,l3_steel_dark);
        int first_column = p.x/(50*l3_u) + 1;
        int first_row = p.y/(50*l3_u) + 1;
        for (int j=first_column; j*50*l3_u<p.x+p.width; j++)
        {
            DrawLine(j*50*l3_u,p.y,j*50*l3_u,p.y+p.height,Fade(l3_steel,0.5));
        }
        for (int j=first_row; j*50*l3_u<p.y+p.height; j++)
        {
            DrawLine(p.x,j*50*l3_u,p.x+p.width,j*50*l3_u,Fade(l3_steel,0.5));
        }
    }

    //l3_vacuum strips: darker with little sparkles
    for (int i=0; i<2; i++)
    {
        Rectangle v = l3_vacuum[i];
        DrawRectangleRec(v,Fade(BLACK,0.45));
        for (int k=0; k<16; k++)
        {
            float x = v.x + fmod(k*67*l3_u + t*20*l3_u,v.width);
            float y = v.y + fmod(k*29*l3_u + 11*l3_u,v.height);
            DrawCircle(x,y,1.5*l3_u,Fade(l3_neon_cyan,0.3+0.3*sin(t*4+k)));
        }
        DrawRectangleLinesEx(v,2*l3_u,Fade(l3_neon_cyan,0.35));
    }

    //solar wind: yellow streaks blowing the way it pushes
    DrawRectangleRec(l3_solar_wind,Fade(l3_neon_orange,0.12));
    int streaks = l3_solar_wind.height/(18*l3_u);
    for (int j=0; j<streaks; j++)
    {
        float x = l3_solar_wind.x + l3_solar_wind.width - fmod(t*140*l3_u + j*31*l3_u,l3_solar_wind.width);
        Vector2 a = {x, l3_solar_wind.y + 9*l3_u + j*18*l3_u};
        Vector2 b = {x + 16*l3_u, a.y};
        if (b.x > l3_solar_wind.x+l3_solar_wind.width) b.x = l3_solar_wind.x + l3_solar_wind.width;
        DrawLineEx(a,b,2*l3_u,Fade(l3_hazard_yellow,0.6));
    }
}


void l3_draw_obstacles()
{
    float t = l3_animation_time;

    //planets: glow, shaded l3_ball, thin ring
    Color planet_light[2] = {{255,190,120,255},{120,240,220,255}};
    Color planet_dark[2] = {{150,60,30,255},{20,90,110,255}};
    for (int i=0; i<2; i++)
    {
        DrawCircleGradient(l3_planet[i],l3_planet_field*0.5,Fade(planet_light[i],0.12),BLANK);
        DrawCircleGradient(l3_planet[i],l3_planet_core[i],planet_light[i],planet_dark[i]);
        DrawCircle(l3_planet[i].x-8*l3_u,l3_planet[i].y-9*l3_u,6*l3_u,Fade(WHITE,0.35));
        DrawRing(l3_planet[i],l3_planet_core[i]+8*l3_u,l3_planet_core[i]+11*l3_u,0,360,48,Fade(planet_light[i],0.5));
    }

    //wormholes: spinning rings at the way in, a dimmer ring and an arrow at the way out
    Color wormhole_colour[3] = {l3_neon_purple,l3_neon_cyan,l3_neon_orange};
    for (int i=0; i<3; i++)
    {
        Color c = wormhole_colour[i];
        DrawCircleGradient(l3_wormhole_in[i],l3_wormhole_radius*1.6,Fade(c,0.35),BLANK);
        for (int j=0; j<3; j++)
        {
            float ring = l3_wormhole_radius*(0.4 + j*0.25);
            float angle = t*(260 + j*80) + j*120;
            DrawRing(l3_wormhole_in[i],ring,ring+3*l3_u,angle,angle+200,24,Fade(c,0.9-j*0.2));
        }
        DrawCircle(l3_wormhole_in[i].x,l3_wormhole_in[i].y,l3_wormhole_radius*0.3,BLACK);

        DrawRing(l3_wormhole_out[i],l3_wormhole_radius*0.7,l3_wormhole_radius*0.8,-t*120,-t*120+270,24,Fade(c,0.6));
        Vector2 facing = {cos(l3_wormhole_angle[i]*DEG2RAD),sin(l3_wormhole_angle[i]*DEG2RAD)};
        Vector2 arrow = Vector2Add(l3_wormhole_out[i],Vector2Scale(facing,l3_wormhole_radius+6*l3_u));
        DrawPoly(arrow,3,9*l3_u,l3_wormhole_angle[i],Fade(c,0.85));
    }

    //energy bumpers, they get bigger for a moment when hit
    for (int i=0; i<4; i++)
    {
        float r = l3_bumper_radius*(1 + l3_bumper_hit[i]*1.5);
        float pulse = 0.5 + 0.5*sin(t*4 + i);
        DrawCircleGradient(l3_bumper[i],r*1.8,Fade(l3_neon_green,0.1+0.25*pulse),BLANK);
        DrawCircle(l3_bumper[i].x,l3_bumper[i].y,r,GetColor(0x0E2A1AFF));
        DrawRing(l3_bumper[i],r-4*l3_u,r,0,360,32,l3_neon_green);
        DrawCircle(l3_bumper[i].x,l3_bumper[i].y,r*0.35,Fade(WHITE,0.6+0.3*pulse));
    }

    //asteroids, turning slowly
    for (int i=0; i<4; i++)
    {
        float size = l3_asteroid_radius[i]*2.4;
        Rectangle source = {(i%3)*192,0,192,192};
        Rectangle dest = {l3_asteroid[i].x,l3_asteroid[i].y,size,size};
        Vector2 origin = {size/2,size/2};
        float spin = t*35;
        if (i%2==1) spin = -t*28;
        DrawTexturePro(l3_asteroid_texture,source,dest,origin,spin,WHITE);
    }

    //lost in space: rings shrinking into the spot
    if (l3_lost_timer>0)
    {
        for (int j=0; j<3; j++)
        {
            float ring = l3_lost_timer*80*l3_u + j*10*l3_u;
            DrawRing(l3_lost_position,ring,ring+2*l3_u,0,360,32,Fade(l3_neon_purple,l3_lost_timer*2));
        }
    }

    //spinning satellite (the level 1 l3_fan, with solar panels)
    Rectangle blade = {l3_fan.x,l3_fan.y,l3_fan_blade_length,l3_fan_blade_thickness};
    Vector2 blade_origin = {l3_fan_blade_length/2,l3_fan_blade_thickness/2};
    DrawRectanglePro(blade,blade_origin,l3_fan_angle-24,Fade(l3_panel_blue,0.12));
    DrawRectanglePro(blade,blade_origin,l3_fan_angle+90-24,Fade(l3_panel_blue,0.12));
    DrawRectanglePro(blade,blade_origin,l3_fan_angle-12,Fade(l3_panel_blue,0.25));
    DrawRectanglePro(blade,blade_origin,l3_fan_angle+90-12,Fade(l3_panel_blue,0.25));
    DrawRectanglePro(blade,blade_origin,l3_fan_angle,l3_panel_blue);
    DrawRectanglePro(blade,blade_origin,l3_fan_angle+90,l3_panel_blue);
    DrawCircle(l3_fan.x,l3_fan.y,l3_fan_hub_radius,l3_steel_dark);
    DrawCircleLines(l3_fan.x,l3_fan.y,l3_fan_hub_radius,BLACK);
    Vector2 bolt_arm = {9*l3_u,0};
    for (int j=0; j<4; j++)
    {
        Vector2 bolt = Vector2Add(l3_fan,Vector2Rotate(bolt_arm,(l3_fan_angle+45+j*90)*DEG2RAD));
        DrawCircle(bolt.x,bolt.y,2.5*l3_u,l3_steel_light);
    }

    //laser gates (from level 1)
    for (int i=0; i<2; i++)
    {
        Rectangle laser = l3_laser_gate[i];
        float laser_timer = l3_laser_clock[i];
        float wall = 24*l3_u;
        float beam_y = laser.y + laser.height/2;
        Rectangle left_emitter = {laser.x-wall+4*l3_u,beam_y-14*l3_u,wall-4*l3_u,28*l3_u};
        Rectangle right_emitter = {laser.x+laser.width,beam_y-14*l3_u,wall-4*l3_u,28*l3_u};
        DrawRectangleRec(left_emitter,l3_steel_dark);
        DrawRectangleRec(right_emitter,l3_steel_dark);
        DrawRectangleLinesEx(left_emitter,2*l3_u,BLACK);
        DrawRectangleLinesEx(right_emitter,2*l3_u,BLACK);
        Color laser_lamp = GetColor(0x4A3A20FF);
        int dashes = laser.width/(16*l3_u);
        if (laser_timer<1.6)
        {
            float dash_alpha = 0.3;
            if (laser_timer>=1.2)
            {
                dash_alpha = 0.55;
                if (fmod(t*10,2)<1) laser_lamp = l3_hazard_yellow;
            }
            for (int j=0; j<dashes; j++)
            {
                DrawRectangle(laser.x+j*16*l3_u+4*l3_u,beam_y-1*l3_u,8*l3_u,2*l3_u,Fade(l3_laser_red,dash_alpha));
            }
        }
        else
        {
            laser_lamp = l3_laser_red;
            DrawRectangle(laser.x,beam_y-14*l3_u,laser.width,28*l3_u,Fade(l3_laser_red,0.15));
            DrawRectangle(laser.x,beam_y-7*l3_u,laser.width,14*l3_u,Fade(l3_laser_red,0.3));
            DrawRectangleRec(laser,l3_laser_red);
            DrawRectangle(laser.x,beam_y-1*l3_u,laser.width,2*l3_u,WHITE);
        }
        DrawCircle(left_emitter.x+left_emitter.width/2,beam_y,5*l3_u,laser_lamp);
        DrawCircle(right_emitter.x+right_emitter.width/2,beam_y,5*l3_u,laser_lamp);
    }
}

void l3_draw_ball_and_pot()
{
    //start plate
    Rectangle plate = {l3_start_position.x-50*l3_u,l3_start_position.y-30*l3_u,100*l3_u,60*l3_u};
    DrawRectangleRec(plate,l3_steel);
    DrawRectangleLinesEx(plate,3*l3_u,l3_steel_dark);
    Rectangle plate_band = {plate.x,plate.y+plate.height-10*l3_u,plate.width,10*l3_u};
    l3_draw_hazard_stripes(plate_band);
    DrawText("START",l3_start_position.x-MeasureText("START",18*l3_u)/2,plate.y+5*l3_u,18*l3_u,WHITE);

    //l3_pot with hazard ring
    for (int i=0; i<8; i++)
    {
        Color colour = l3_hazard_yellow;
        if (i%2==1) colour = BLACK;
        DrawRing(l3_pot,l3_radius_pot+6*l3_u,l3_radius_pot+12*l3_u,i*45,i*45+45,6,colour);
    }
    DrawCircle(l3_pot.x,l3_pot.y,l3_radius_pot+6*l3_u,l3_steel_light);
    DrawRing(l3_pot,l3_radius_pot+1*l3_u,l3_radius_pot+5*l3_u,0,360,24,l3_steel);
    DrawCircle(l3_pot.x,l3_pot.y,l3_radius_pot,BLACK);

    //flag
    Vector2 pole_top = {l3_pot.x,l3_pot.y-60*l3_u};
    DrawLineEx(l3_pot,pole_top,3*l3_u,l3_steel_light);
    for (int i=0; i<5; i++)
    {
        float wave = sin(l3_animation_time*6 - i*0.8)*2.5*l3_u;
        DrawRectangle(l3_pot.x+1*l3_u+i*6*l3_u,pole_top.y+wave,6*l3_u,18*l3_u,l3_laser_red);
    }

    //l3_ball
    DrawCircle(l3_ball.x+3*l3_u,l3_ball.y+4*l3_u,l3_radius_ball,Fade(BLACK,0.45));
    DrawCircle(l3_ball.x,l3_ball.y,l3_radius_ball,GetColor(0xEDEDEDFF));
    DrawCircleLines(l3_ball.x,l3_ball.y,l3_radius_ball,GRAY);
    DrawCircle(l3_ball.x-2*l3_u,l3_ball.y-2*l3_u,2*l3_u,WHITE);

    //aim line
    if (l3_aiming==1 && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        Vector2 mouse = {GetMouseX(),GetMouseY()};
        DrawLineEx(l3_ball,mouse,4*l3_u,Fade(WHITE,0.75));
        DrawCircle(mouse.x,mouse.y,5*l3_u,Fade(WHITE,0.75));
    }
}

//short preview of the shot (0.3 seconds), gravity bend included
void l3_draw_aim_preview()
{
    if (preview_seconds<=0) return;
    if (l3_aiming==0 || IsMouseButtonDown(MOUSE_BUTTON_LEFT)==0) return;
    Vector2 mouse = {GetMouseX(),GetMouseY()};
    Vector2 drag = Vector2Subtract(l3_ball,mouse);
    if (Vector2Length(drag)<2*l3_radius_ball) return;

    //remember everything l3_update_ball can change
    Vector2 saved_ball = l3_ball;
    Vector2 saved_speed = l3_speed;
    Vector2 saved_last_shot = l3_last_shot_position;
    Vector2 saved_lost_position = l3_lost_position;
    float saved_lost_timer = l3_lost_timer;
    float saved_message_timer = l3_message_timer;
    int saved_message_type = l3_message_type;
    int saved_game_state = l3_game_state;
    float saved_cooldown = l3_wormhole_cooldown;
    float saved_hit[4];
    for (int i=0; i<4; i++) saved_hit[i] = l3_bumper_hit[i];

    //try the shot for 72 tiny steps, a dot every 6, stop at a hazard or a wormhole jump
    l3_speed = Vector2ClampValue(Vector2Scale(drag,3.125),0,l3_max_speed*l3_u);
    l3_message_timer = -1;
    for (int i=1; i<=preview_seconds*240; i++)
    {
        Vector2 before = l3_ball;
        l3_update_ball(1.0/240);
        if (l3_message_timer==1 || l3_game_state!=0 || Vector2Distance(before,l3_ball)>20*l3_u) break;
        if (i%6==0) DrawCircle(l3_ball.x,l3_ball.y,3*l3_u,Fade(WHITE,1-i/(preview_seconds*240+18)));
        if (l3_speed.x==0 && l3_speed.y==0) break;
    }

    //put everything back
    l3_ball = saved_ball;
    l3_speed = saved_speed;
    l3_last_shot_position = saved_last_shot;
    l3_lost_position = saved_lost_position;
    l3_lost_timer = saved_lost_timer;
    l3_message_timer = saved_message_timer;
    l3_message_type = saved_message_type;
    l3_game_state = saved_game_state;
    l3_wormhole_cooldown = saved_cooldown;
    for (int i=0; i<4; i++) l3_bumper_hit[i] = saved_hit[i];
}


void l3_draw_events()
{
    float t = l3_animation_time;
    if (l3_event_type==0) return;

    //warning: blinking red arrow at the edge where it comes in
    if (l3_event_timer<1.5)
    {
        Vector2 sign = l3_event_start;
        sign.x = Clamp(sign.x,40*l3_u,l3_width-40*l3_u);
        sign.y = Clamp(sign.y,l3_hud_height+40*l3_u,l3_height-40*l3_u);
        float angle = atan2(l3_event_direction.y,l3_event_direction.x)*RAD2DEG;
        if (fmod(t*6,2)<1.3)
        {
            DrawCircle(sign.x,sign.y,26*l3_u,Fade(RED,0.35));
            DrawPoly(sign,3,18*l3_u,angle,RED);
        }
        return;
    }

    //l3_comet: bright head and a fading tail
    if (l3_event_type==1)
    {
        for (int j=12; j>0; j--)
        {
            Vector2 bit = Vector2Subtract(l3_comet,Vector2Scale(l3_event_direction,j*14*l3_u));
            DrawCircle(bit.x,bit.y,(12-j*0.8)*l3_u,Fade(l3_neon_cyan,0.5-j*0.035));
        }
        DrawCircleGradient(l3_comet,26*l3_u,Fade(WHITE,0.6),BLANK);
        DrawCircle(l3_comet.x,l3_comet.y,12*l3_u,WHITE);
    }

    //l3_meteor shower: small rocks with orange tails
    if (l3_event_type==2)
    {
        for (int i=0; i<5; i++)
        {
            Vector2 tail = Vector2Subtract(l3_meteor[i],Vector2Scale(l3_event_direction,40*l3_u));
            DrawLineEx(tail,l3_meteor[i],6*l3_u,Fade(l3_neon_orange,0.5));
            float size = 29*l3_u;
            Rectangle source = {(i%3)*192,0,192,192};
            Rectangle dest = {l3_meteor[i].x,l3_meteor[i].y,size,size};
            Vector2 origin = {size/2,size/2};
            DrawTexturePro(l3_asteroid_texture,source,dest,origin,t*200,WHITE);
        }
    }

    //l3_ufo: green beam while it takes the l3_ball, chasing lights otherwise
    if (l3_event_type==3)
    {
        int frame = (int)(t*6)%3;
        if (l3_beam_timer>0 || l3_abducted==1)
        {
            frame = 3;
            DrawCircleGradient(l3_ufo,80*l3_u,Fade(l3_neon_green,0.45),Fade(l3_neon_green,0.05));
            DrawRing(l3_ufo,76*l3_u,80*l3_u,0,360,48,Fade(l3_neon_green,0.6));
        }
        DrawCircle(l3_ufo.x+20*l3_u,l3_ufo.y+26*l3_u,70*l3_u,Fade(BLACK,0.3));
        Rectangle source = {frame*320,0,320,320};
        Rectangle dest = {l3_ufo.x,l3_ufo.y,150*l3_u,150*l3_u};
        Vector2 origin = {75*l3_u,75*l3_u};
        DrawTexturePro(l3_ufo_texture,source,dest,origin,0,WHITE);
    }
}


void l3_draw_hud()
{
    //dark edges
    DrawRectangleGradientH(0,l3_hud_height,90*l3_u,l3_height-l3_hud_height,Fade(BLACK,0.45),BLANK);
    DrawRectangleGradientH(l3_width-90*l3_u,l3_hud_height,90*l3_u,l3_height-l3_hud_height,BLANK,Fade(BLACK,0.45));
    DrawRectangleGradientV(0,l3_height-70*l3_u,l3_width,70*l3_u,BLANK,Fade(BLACK,0.45));

    //l3_steel bar
    DrawRectangleGradientV(0,0,l3_width,l3_hud_height,l3_steel_light,l3_steel_dark);
    DrawRectangle(0,l3_hud_height-4*l3_u,l3_width,4*l3_u,BLACK);
    int rivets = l3_width/(40*l3_u);
    for (int i=0; i<rivets; i++)
    {
        DrawCircle(20*l3_u+i*40*l3_u,8*l3_u,3*l3_u,l3_steel_dark);
        DrawCircle(20*l3_u+i*40*l3_u,l3_hud_height-12*l3_u,3*l3_u,l3_steel_dark);
    }

    //text
    DrawText("LEVEL 3 - EVENT HORIZON",32*l3_u,20*l3_u,36*l3_u,BLACK);
    DrawText("LEVEL 3 - EVENT HORIZON",30*l3_u,18*l3_u,36*l3_u,l3_hazard_yellow);
    DrawText(TextFormat("STROKES %d / %d",l3_stroke,l3_stroke_limit),l3_width/2-298*l3_u,21*l3_u,32*l3_u,BLACK);
    DrawText(TextFormat("STROKES %d / %d",l3_stroke,l3_stroke_limit),l3_width/2-300*l3_u,19*l3_u,32*l3_u,WHITE);
    for (int i=0; i<l3_stroke_limit; i++)
    {
        Color pip = l3_steel_dark;
        if (i<l3_stroke) pip = l3_laser_red;
        DrawCircle(l3_width/2+10*l3_u+i*22*l3_u,35*l3_u,7*l3_u,pip);
        DrawCircleLines(l3_width/2+10*l3_u+i*22*l3_u,35*l3_u,7*l3_u,BLACK);
    }
    int help_width = MeasureText("R restart   ESC menu",26*l3_u);
    DrawText("R restart   ESC menu",l3_width-help_width-28*l3_u,24*l3_u,26*l3_u,BLACK);
    DrawText("R restart   ESC menu",l3_width-help_width-30*l3_u,22*l3_u,26*l3_u,WHITE);

    //lost, zapped or beamed up message
    if (l3_message_timer>0)
    {
        if (l3_message_type==1)
        {
            DrawText("LOST IN SPACE!",l3_width/2-MeasureText("LOST IN SPACE!",90*l3_u)/2+4*l3_u,l3_height/2-41*l3_u,90*l3_u,Fade(BLACK,l3_message_timer));
            DrawText("LOST IN SPACE!",l3_width/2-MeasureText("LOST IN SPACE!",90*l3_u)/2,l3_height/2-45*l3_u,90*l3_u,Fade(l3_neon_purple,l3_message_timer));
        }
        else if (l3_message_type==2)
        {
            DrawText("ZAPPED!",l3_width/2-MeasureText("ZAPPED!",90*l3_u)/2+4*l3_u,l3_height/2-41*l3_u,90*l3_u,Fade(BLACK,l3_message_timer));
            DrawText("ZAPPED!",l3_width/2-MeasureText("ZAPPED!",90*l3_u)/2,l3_height/2-45*l3_u,90*l3_u,Fade(l3_laser_red,l3_message_timer));
        }
        else
        {
            DrawText("BEAMED UP!",l3_width/2-MeasureText("BEAMED UP!",90*l3_u)/2+4*l3_u,l3_height/2-41*l3_u,90*l3_u,Fade(BLACK,l3_message_timer));
            DrawText("BEAMED UP!",l3_width/2-MeasureText("BEAMED UP!",90*l3_u)/2,l3_height/2-45*l3_u,90*l3_u,Fade(l3_neon_green,l3_message_timer));
        }
    }

    //level clear or failed
    if (l3_game_state!=0)
    {
        DrawRectangle(0,0,l3_width,l3_height,Fade(BLACK,0.6));
        Rectangle panel = {l3_width/2-340*l3_u,l3_height/2-170*l3_u,680*l3_u,340*l3_u};
        DrawRectangleRec(panel,l3_steel_dark);
        Rectangle panel_band = {panel.x,panel.y,panel.width,24*l3_u};
        l3_draw_hazard_stripes(panel_band);
        DrawRectangleLinesEx(panel,6*l3_u,l3_steel_light);
        if (l3_game_state==1)
        {
            DrawText("LEVEL CLEAR!",l3_width/2-MeasureText("LEVEL CLEAR!",80*l3_u)/2,panel.y+60*l3_u,80*l3_u,l3_hazard_yellow);
        }
        else
        {
            DrawText("FAILED",l3_width/2-MeasureText("FAILED",80*l3_u)/2,panel.y+60*l3_u,80*l3_u,l3_laser_red);
        }
        int strokes_width = MeasureText(TextFormat("Strokes: %d / %d",l3_stroke,l3_stroke_limit),40*l3_u);
        DrawText(TextFormat("Strokes: %d / %d",l3_stroke,l3_stroke_limit),l3_width/2-strokes_width/2,panel.y+170*l3_u,40*l3_u,WHITE);
        DrawText("R play again   ESC menu",l3_width/2-MeasureText("R play again   ESC menu",30*l3_u)/2,panel.y+250*l3_u,30*l3_u,l3_steel_light);
    }
}


//level 3: keep the obstacles moving with no ball (intro and menu)
void l3_background_step(float dt)
{
    l3_animation_time = l3_animation_time + dt;
    l3_update_obstacles(dt);
}


//level 3: draw everything except the scoreboard (intro and menu)
void l3_draw_scene()
{

    l3_draw_space();
    l3_draw_walkways();
    l3_draw_obstacles();
    l3_draw_ball_and_pot();
    l3_draw_aim_preview();
    l3_draw_events();

}


//level 3: set the screen size, load pictures, start the level
void l3_start(int screen_width, int screen_height)
{
    l3_width = screen_width;
    l3_height = screen_height;

    //pictures (made at 2x size, so they stay sharp)
    l3_asteroid_texture = LoadTexture("assets/space/space_asteroid.png");
    l3_ufo_texture = LoadTexture("assets/space/space_ufo.png");
    SetTextureFilter(l3_asteroid_texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(l3_ufo_texture,TEXTURE_FILTER_BILINEAR);

    //every size is N*l3_u, so it looks the same on any screen
    l3_u = l3_height/1080.0;
    if (l3_width/1920.0 < l3_u) l3_u = l3_width/1920.0;
    l3_make_stars();
    l3_reset_level();

}


//level 3: one frame of input and movement
void l3_update()
{
    float dt = GetFrameTime();
    if (dt>1.0/30) dt = 1.0/30;
    l3_animation_time = l3_animation_time + dt;
    if (l3_message_timer>0) l3_message_timer = l3_message_timer - dt;

    //restart
    if (IsKeyPressed(KEY_R)) l3_reset_level();

    //shooting (the click has to start while the l3_ball is still)
    int ball_stopped = 0;
    if (l3_speed.x==0 && l3_speed.y==0) ball_stopped = 1;
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && ball_stopped==1 && l3_game_state==0 && l3_abducted==0) l3_aiming = 1;
    if (l3_game_state!=0) l3_aiming = 0;
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && l3_aiming==1)
    {
        l3_aiming = 0;
        Vector2 mouse = {GetMouseX(),GetMouseY()};
        Vector2 drag = Vector2Subtract(l3_ball,mouse);
        if (ball_stopped==1 && l3_stroke<l3_stroke_limit && Vector2Length(drag)>=2*l3_radius_ball)
        {
            l3_last_shot_position = l3_ball;
            l3_speed = Vector2ClampValue(Vector2Scale(drag,3.125),0,l3_max_speed*l3_u);
            l3_stroke++;
        }
    }

    //moving everything in 4 small steps, so nothing jumps through anything
    for (int i=0; i<4; i++)
    {
        l3_update_obstacles(dt/4*obstacle_speed);
        l3_update_events(dt/4*obstacle_speed);
        if (l3_game_state==0 && l3_abducted==0) l3_update_ball(dt/4);
    }

    //out of strokes
    if (l3_game_state==0 && l3_stroke>=l3_stroke_limit && l3_speed.x==0 && l3_speed.y==0) l3_game_state = 2;


}


//level 3: one frame of drawing
void l3_draw()
{

    l3_draw_space();
    l3_draw_walkways();
    l3_draw_obstacles();
    l3_draw_ball_and_pot();
    l3_draw_aim_preview();
    l3_draw_events();
    l3_draw_hud();

}


void l3_unload()
{
    UnloadTexture(l3_asteroid_texture);
    UnloadTexture(l3_ufo_texture);
}
