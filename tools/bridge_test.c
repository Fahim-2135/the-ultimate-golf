//Can you actually cross the boardwalks on Frozen Peak?
//
//The segment checker walks the course between waypoints, which never proves that a
//crossing works: it can route round the water instead. This drives the ball at a
//boardwalk from the bank, at a range of speeds, and says whether it got to the far side.
//
//Build it the same way as segment_test.c and run it from the repo root.
#include "rlgl.h"
#include <stdio.h>
#define main game_main
#include "../src/the_ultimate_golf.c"
#undef main


//roll the ball across one bridge and say what happened
//  0 = made it      1 = went in the water      2 = still on the bridge when time ran out
int cross(int bridge, float from_x, float to_x, float lane_y, float speed)
{
    l5_reset_level();
    l5_ball = (Vector2){from_x*su,lane_y*su};
    l5_last_shot_position = l5_ball;
    float way = (to_x>from_x) ? 1 : -1;
    l5_speed = (Vector2){way*speed*su,0};

    int fell = 0;
    for (int frame=0; frame<600; frame++)
    {
        float dt = 1.0/60;
        l5_update_obstacles(dt);
        Vector2 before = l5_ball;
        l5_update_ball(dt);
        //update_ball sends the ball home when it goes in, so a jump back is a soaking
        if (Vector2Distance(before,l5_ball) > 300*su) fell = 1;
        if (fell) return 1;
        if (way>0 && l5_ball.x > to_x*su) return 0;
        if (way<0 && l5_ball.x < to_x*su) return 0;
        //it stopped dead somewhere
        if (Vector2Length(l5_speed) < 2*su && frame>30) break;
    }
    //where did it stop?
    if (l5_on_safe_ground(l5_ball)==0) return 1;
    return 2;
}


int main(void)
{
    InitWindow(1600,900,"bridge test");
    SetTargetFPS(60);
    screen_width = GetScreenWidth();
    screen_height = GetScreenHeight();
    su = screen_height/1080.0;
    if (screen_width/1920.0 < su) su = screen_width/1920.0;
    l5_start(screen_width,screen_height);

    //west bank -> east bank over each lead, and back again
    float run[8][4] = {
        //bridge, start x, finish x, the line the ball is on
        {0,  300,  960,  860},
        {0,  960,  300,  860},
        {0,  300,  960,  820},     //high on the planks
        {0,  300,  960,  900},     //low on the planks
        {1, 1030, 1630,  653},
        {1, 1630, 1030,  653},
        {1, 1030, 1630,  620},
        {1, 1030, 1630,  686}};
    const char *name[8] = {"south lead, west to east","south lead, east to west",
                           "south lead, along the top edge","south lead, along the bottom edge",
                           "middle lead, west to east","middle lead, east to west",
                           "middle lead, along the top edge","middle lead, along the bottom edge"};

    for (int r=0; r<8; r++)
    {
        int made = 0, drowned = 0, stuck = 0;
        printf("%s\n",name[r]);
        for (int k=0; k<12; k++)
        {
            float speed = 160 + k*45;          //a gentle roll up to a full-blooded shot
            int what = cross(run[r][0],run[r][1],run[r][2],run[r][3],speed);
            if (what==0) made++;
            else if (what==1) drowned++;
            else stuck++;
            printf("   %4.0f units/sec : %s\n",speed,
                   what==0 ? "across" : (what==1 ? "IN THE WATER" : "stopped on it"));
        }
        printf("   -> %d across, %d in, %d stopped short\n\n",made,drowned,stuck);
    }

    CloseWindow();
    return 0;
}
