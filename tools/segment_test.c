//Walks each course one stretch at a time: put the ball on a waypoint, give it a dozen
//tries to reach the next one, and report how often it gets there. A stretch that never
//works is a hole in the level, not bad luck.
#include "rlgl.h"
#define main game_main
#include "../src/the_ultimate_golf.c"
#undef main
#include <stdio.h>

static unsigned int sd = 2024;
float rnd(void) { sd = sd*1103515245 + 12345; return ((sd>>16)&0x7fff)/32767.0f; }

float way5[][2] = {{150,960},{330,900},{300,790},{250,620},{230,430},{250,300},{420,300},{560,300},
                   {700,300},{830,300},{900,480},{900,650},{900,820},{900,950},{1600,940},
                   {1750,820},{1750,650},{1750,480},{1700,300},{1780,250}};
float way6[][2] = {{150,940},{260,880},{340,700},{360,560},{400,470},{520,440},{650,460},{800,430},
                   {900,250},{1050,200},{1250,250},{1380,230},{1300,500},{1250,700},{1100,850},
                   {1000,880},{1200,760},{1380,600},{1430,590},{1560,580},{1680,580}};
float way7[][2] = {{140,920},{300,920},{430,950},{880,960},{900,820},{880,700},{960,660},
                   {1300,250},{1400,280},{1480,320},{1620,420},{1700,560},{1700,660},{1700,760}};
int way_count[3] = {20,21,14};

Vector2 way(int n, int i)
{
    float u = (n==5) ? l5_u : (n==6) ? l6_u : l7_u;
    float x = (n==5) ? way5[i][0] : (n==6) ? way6[i][0] : way7[i][0];
    float y = (n==5) ? way5[i][1] : (n==6) ? way6[i][1] : way7[i][1];
    Vector2 p = {x*u,y*u};
    return p;
}

void put_ball(int n, Vector2 p)
{
    if (n==5) { l5_ball = p; l5_last_shot_position = p; l5_speed = Vector2Zero(); }
    else if (n==6) { l6_ball = p; l6_last_shot_position = p; l6_speed = Vector2Zero(); }
    else { l7_ball = p; l7_last_shot_position = p; l7_speed = Vector2Zero(); }
}

Vector2 get_ball(int n)
{
    if (n==5) return l5_ball;
    if (n==6) return l6_ball;
    return l7_ball;
}

void shoot(int n, float angle, float power)
{
    float u = (n==5) ? l5_u : (n==6) ? l6_u : l7_u;
    Vector2 v = {cosf(angle)*power*650*u,sinf(angle)*power*650*u};
    if (n==5) l5_speed = v;
    else if (n==6) l6_speed = v;
    else l7_speed = v;
    float dt = 1.0/60;
    for (int f=0; f<420; f++)
    {
        for (int k=0; k<4; k++)
        {
            if (n==5) { l5_update_obstacles(dt/4); l5_update_ball(dt/4); }
            else if (n==6) { l6_update_obstacles(dt/4); l6_update_ball(dt/4); }
            else { l7_update_obstacles(dt/4); l7_update_ball(dt/4); }
        }
        Vector2 s = (n==5) ? l5_speed : (n==6) ? l6_speed : l7_speed;
        if (s.x==0 && s.y==0) return;
    }
}

void reset(int n)
{
    if (n==5) { l5_stroke_limit = 999; l5_reset_level(); }
    else if (n==6) { l6_stroke_limit = 999; l6_reset_level(); }
    else { l7_stroke_limit = 999; l7_reset_level(); }
}

void walk(int n)
{
    float u = (n==5) ? l5_u : (n==6) ? l6_u : l7_u;
    printf("level %d\n", n);
    int worst = 100, worst_at = -1;
    for (int i=0; i<way_count[n-5]-1; i++)
    {
        int made = 0;
        for (int t=0; t<40; t++)
        {
            reset(n);
            put_ball(n,way(n,i));
            for (int shot=0; shot<10; shot++)
            {
                Vector2 ball = get_ball(n);
                Vector2 target = way(n,i+1);
                float a = atan2f(target.y-ball.y,target.x-ball.x) + (rnd()-0.5f)*0.35f;
                float d = Vector2Distance(ball,target)/u;
                float p = sqrtf(d/1400.0f) + (rnd()-0.5f)*0.12f;
                if (p<0.12f) p = 0.12f;
                if (p>1) p = 1;
                shoot(n,a,p);
                //reaching ANY later waypoint counts: on ice, sliding past the next one is progress
                int moved_on = 0;
                for (int w=i+1; w<way_count[n-5]; w++)
                {
                    if (Vector2Distance(get_ball(n),way(n,w))<90*u) moved_on = 1;
                }
                if (moved_on==1) { made++; break; }
                for (int w=0; w<150; w++)         //two seconds of thinking time
                {
                    if (n==5) l5_update_obstacles(1.0/60);
                    else if (n==6) l6_update_obstacles(1.0/60);
                    else l7_update_obstacles(1.0/60);
                }
            }
        }
        if (made<worst) { worst = made; worst_at = i; }
        if (made<12) printf("  stretch %2d (%.0f,%.0f -> %.0f,%.0f): %d of 40\n",
                            i, way(n,i).x/u, way(n,i).y/u, way(n,i+1).x/u, way(n,i+1).y/u, made);
    }
    printf("  worst stretch %d at %.0f,%.0f (%d of 40)\n\n", worst_at, way(n,worst_at).x/u, way(n,worst_at).y/u, worst);
}

int main(void)
{
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(1920,1080,"segments");
    screen_width = GetScreenWidth();
    screen_height = GetScreenHeight();
    su = screen_height/1080.0;
    if (screen_width/1920.0 < su) su = screen_width/1920.0;
    l5_start(screen_width,screen_height);
    l6_start(screen_width,screen_height);
    l7_start(screen_width,screen_height);
    obstacle_speed = 1;
    walk(5);
    walk(6);
    walk(7);
    CloseWindow();
    return 0;
}
