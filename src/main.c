// NOTE: 2/10/24 2:56 am
// The use of "attacking" in this code base
//  just refers to starting an animation or 
//  in the middle of an animation. This is 
//  because I'm bad at software development 
//  and I dont want to go through and rename 
//  everything. So 'isAttacking' and 'p_attack'
//  are just the general animation triggers

#include <stdio.h>
#include <raylib.h>
#include <raymath.h>
#include <player.h>

#define SCREEN_W 1920
#define SCREEN_H 1080

#define RELEASE 0
#define DEBUG 1

#define MAX_POSTPRO_SHADERS 12
#define FX_SOBEL 0
#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION 330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION 100
#endif

//anims: should probably use an enum

//static Vector3 zero_center = {0.0f, 0.0f, 0.0f};
static Vector3 model_scale = {0.001f, 0.0007f, 0.001f};
static Vector3 cam_position = {25.0f, 10.0f, -10.0f}; 
static Vector3 cam_target = {25.0f, 2.0f, 10.0f}; 
static Vector3 center_stage = {25.0f, 0.0f, 10.0f};
static Vector3 back_stage = {25.0f, 0.0f, 20.0f};
static Vector3 up = {0.0f, 1.0f, 0.0f};
static Color TAN = {210, 180, 140, 255};
static Color HITBOX = {255, 255, 0, 175};
bool draw_poses = 1;
bool draw_once = 1;
bool wtf = 1;
int stepper = 18;
Hurtboxes p1hurtboxes = {0};
Hurtboxes p2hurtboxes = {0};

void init_game()
{
    InitWindow(SCREEN_W, SCREEN_H, "shabengine");
    SetTargetFPS(60);
}

Camera3D init_cam()
{
    Camera3D camera;
    camera.position = cam_position;
    camera.position.z -= 5.0f;
    camera.position.y += 5.0f;

    camera.up = up;
    camera.fovy = 75.0f;
    camera.target = cam_target;
    camera.projection = CAMERA_PERSPECTIVE; 
    return camera;
}

Camera3D cam_controls(Camera3D camera, int mode)
{
    if (mode == DEBUG){
        if (IsKeyDown(KEY_J)) camera.position.x += 0.25f;
        if (IsKeyDown(KEY_K)) camera.position.x -= 0.25f;
        if (IsKeyDown(KEY_I)) camera.position.y -= 0.25f;
        if (IsKeyDown(KEY_U)) camera.position.y += 0.25f;
        if (IsKeyDown(KEY_N)) camera.position.z -= 0.25f;
        if (IsKeyDown(KEY_M)) camera.position.z += 0.25f;
    }
    return camera;
}

void draw_step_3D(Player p1, Player p2)
{
        DrawPlane(center_stage, (Vector2){50.0f, 25.0f}, TAN);
        DrawCube(back_stage, 50.0f, 25.0f, 3, TAN);
        DrawModelEx(p1.playermodel, p1.pos, (Vector3){0.0f, 1.0f, 0.0f}, p1.face, model_scale, RED); 
        DrawModelEx(p2.playermodel, p2.pos, (Vector3){0.0f, 1.0f, 0.0f}, p2.face, model_scale, BLUE);
        
        p1.pos.y += 2.0f;
        p2.pos.y += 2.0f;
}

Hurtboxes get_bind_poses(Player p, int mode){ // 0 is release, 1 is debug
    Hurtboxes boxes = {0};
    Vector3 model_pos = p.pos;
    int fc = p.atk_fc;
    
    if (mode == 1){
        if (IsKeyPressed(KEY_C)) {
            stepper+=1;
            //draw_once ^= 1;
            printf("i: %d\n", stepper);
            if (stepper > p.playermodel.boneCount - 1){
                stepper = 0;
            } 
        }
    }
        //p.playermodel.boneCount - 1
        for (int i = 0; i < p.playermodel.boneCount; i++) {
            // adjust translation based on model scale
            Color col = YELLOW;
            col.a = 0;
            // using idle frame counter for point will cause segfault, fc := atk_fc
            Vector3 segfault = p.anims[p.currentanim].framePoses[fc][i].translation;
            Vector3 scaledTranslation = Vector3Multiply(segfault, model_scale);
            scaledTranslation =  Vector3RotateByAxisAngle(scaledTranslation, (Vector3){0.0f, 1.0f, 0.0f}, p.face * DEG2RAD);
            scaledTranslation = Vector3Add(scaledTranslation, model_pos);
            // 5 = left knee
            // 6 = right knee
            // 9 = right foot
            // 8 = left foot
            // 24 = r fist
            // 23 = l fist
            // 21 = l elbow
            // 22 = r elbow
            
            if (i==5 || i==6 || i==9 || i==8 ){
                col.a = 255;
            }
            if (i==24 || i==21 || i==22 || i==23){
                col = PURPLE;
                col.a = 255;
            }
            if (i==9){
                boxes.right_l = scaledTranslation;
            }
            if (i==6){
                boxes.left_l = scaledTranslation;
            }
            if (i==24){
                boxes.right_h = scaledTranslation;
            }
            if (i==23){
                boxes.left_h = scaledTranslation;
            }
            if (i==stepper){
                col = GREEN;
                col.a = 255;
            }
            if (mode == 1){ 
            DrawCube(scaledTranslation, 1.0f, 1.0f, 1.0f, col);
            }
        }
        return boxes;
        
}


void debug_draw_3d(Player p1, Player p2)
{
    //idk
    DrawBoundingBox(p1.bbox, GREEN);
    DrawBoundingBox(p2.bbox, GREEN);

    if (IsKeyPressed(KEY_V)) draw_poses ^= 1;
    p1hurtboxes = get_bind_poses(p1, draw_poses);
    p2hurtboxes = get_bind_poses(p2, draw_poses);
    
    DrawGrid(100, 3.0f);
    if (p1.phbox.active == 1){
        DrawSphere(p1.phbox.pos, p1.phbox.r, HITBOX);
        //printf("hb: %f\n", p1.phbox.pos.x);
    }
    if (p1.phbox1.active == 1){
        DrawSphere(p1.phbox1.pos, p1.phbox1.r, HITBOX);
        //printf("hb: %f\n", p1.phbox.pos.x);
    }
    if (p2.phbox.active == 1){
        DrawSphere(p2.phbox.pos, p2.phbox.r, HITBOX);
        //printf("hb: %f\n", p1.phbox.pos.x);
    }
    if (p2.phbox1.active == 1){
        DrawSphere(p2.phbox1.pos, p2.phbox1.r, HITBOX);
        //printf("hb: %f\n", p1.phbox.pos.x);
    }
}


void draw_player_hp(Player p1, Player p2)
{
    int xspacing = 50;
    int yspacing = 100;
    int width = 300;
    int height = 50;
    DrawRectangle(xspacing - 10, yspacing - 10, width + 20, height + 20, BLACK);
    DrawRectangle(SCREEN_W - width - xspacing - 10, yspacing - 10, width + 20, height + 20, BLACK);
    DrawRectangle(xspacing, yspacing, width, height, RED);
    DrawRectangle(SCREEN_W - width - xspacing, yspacing, width, height, RED);
    DrawRectangle(xspacing, yspacing, p1.health, height, GREEN);
    DrawText("P1", xspacing, yspacing, 20, RAYWHITE);
    DrawRectangle(SCREEN_W - width - xspacing, yspacing, p2.health, height, GREEN);
    DrawText("P2", SCREEN_W - width - xspacing, yspacing, 20, RAYWHITE);

}

void draw_step_2D(Player p1, Player p2, int dev_mode)
{
    draw_player_hp(p1, p2);
    if (dev_mode == DEBUG) {
        DrawText("hello, shab", SCREEN_W - 200, SCREEN_H - 100, 30, BLACK);
        DrawRectangle(20, 20, 100, 50, RAYWHITE);
        DrawFPS(30,30);
    }
}

int switch_dev_mode(int mode) {
    if (mode == RELEASE){
        mode = DEBUG;
    } else {
        mode = RELEASE;
    }
    printf("changed mode: %d\n", mode);
    return mode;
}

BoundingBox get_p_bbox(Player p)
{
    // some tomfoolery
    p.bbox.min.x = p.pos.x - 2.3f; 
    p.bbox.min.y = p.pos.y; 
    p.bbox.min.z = p.pos.z - 2.3f; 
    p.bbox.max.x = p.pos.x + 2.3f; 
    p.bbox.max.y = p.pos.y + 11.0f; 
    p.bbox.max.z = p.pos.z + 2.3f;
    return p.bbox;
}

//drawing
Player init_hitbox(Player p)
{
    if (p.pindex==1){
        p.phurtboxes = get_bind_poses(p, 0);
    }
    if (p.pindex==2){
        p.phurtboxes = get_bind_poses(p, 0);
    }    
    //printf("Vector: %f, %f, %f\n", p1hurtboxes.left_h.x, p1hurtboxes.left_h.y, p1hurtboxes.left_h.z);
    //printf("Vector: %f, %f, %f, \n", p1hurtboxes.right_h.x, p1hurtboxes.right_h.y, p1hurtboxes.right_h.z);
    //printf("Vector: %f, %f, %f, \n", p1hurtboxes.left_l.x, p1hurtboxes.left_l.y, p1hurtboxes.left_l.z);
    //printf("Vector: %f, %f, %f, \n", p1hurtboxes.right_l.x, p1hurtboxes.right_l.y, p1hurtboxes.right_l.z);   
    p.phbox.pos.z = p.pos.z;
    p.phbox.pos.x = p.pos.x;
    p.phbox.pos.y = p.pos.y;

    // THIS IS SO MUCH BETTER LETS GO
    if (p.currentanim == PUNCH){
        p.phbox.pos = p.phurtboxes.right_h;
    }
    if (p.currentanim == KICK){
        p.phbox.pos = p.phurtboxes.right_l;
    }
    if (p.currentanim == SWEEP){
        p.phbox.pos = p.phurtboxes.right_l;
        p.phbox1.pos = p.phurtboxes.left_l;
    }
    if (p.currentanim == UPCUT){
        p.phbox.pos = p.phurtboxes.left_h;

    }
    return p;
}
// this is not working properly

//frame data and radius
Player fight_logic(Player p)
{
    if (p.isAttacking){
        //printf("current anim: %d\n", p1.currentanim);
        //printf("fc: %d\n", p.atk_fc);
        int fc = p.atk_fc;
        p = init_hitbox(p);
        if (p.currentanim == PUNCH){
            p.phbox.r =  1.2f;
            int start = 13;
            int end = 27;
            if (fc > start && fc < end) {
                p.phbox.active = 1;
            } else if (fc > end) {
                p.phbox.active = 0;
                if (fc > 35) p.isAttacking = 0;
            }
        } else if (p.currentanim == KICK){
            p.phbox.r =  1.5f;
            int start = 35;
            int end = 50;
            if (fc > start && fc < end) {
                p.phbox.active = 1;
            } else if (fc > end) {
                p.phbox.active = 0;
            }
        } else if (p.currentanim == SWEEP){
            p.phbox.r = 2.0f;
            p.phbox1.r = 3.0f;
            int start = 68;
            int end = 85;
            if (fc > start && fc < end) {
                p.phbox.active = 1;
                p.phbox1.active = 1;
            } else if (fc > end) {
                p.phbox.active = 0;
                p.phbox1.active = 0;
                if (fc > 120) p.isAttacking = 0;
            }
            
        } else if (p.currentanim == UPCUT){
            p.phbox.r =  1.5f;
            int start = 28;
            int end = 42;
            if (fc > start && fc < end) {
                p.phbox.active = 1;
            } else if (fc > end) {
                p.phbox.active = 0;
                if (fc > 65) p.isAttacking = 0;
            }
        }
    } else {
        p.atk_fc = 0;
        p.phbox.active = 0;
    }
    return p;
}
//CheckCollisionBoxSphere(BoundingBox box, Vector3 center, float radius); 

// hit and health logic
PlayerPair p_hit(PlayerPair pair){
    if (pair.player1.phbox.active == 1){
        if (CheckCollisionBoxSphere(pair.player2.bbox, pair.player1.phbox.pos, pair.player1.phbox.r)){
            pair.player2.ntlh = 1;
            if (pair.player2.ntlh==1 && pair.player2.alh==0){
                pair.player2.alh = 1;
                printf("p1 hit p2\n");
                if (pair.player1.currentanim == PUNCH) {pair.player2.health -= 25;}
                if (pair.player1.currentanim == KICK) {pair.player2.health -= 50;}
                if (pair.player1.currentanim == SWEEP) {
                    pair.player2.health -= 40;
                    pair.player2.pha = 2;
                    }
                if (pair.player1.currentanim == UPCUT) {pair.player2.health -= 30;}
                if (pair.player1.currentanim != SWEEP) {pair.player2.pha = 1;}
            }   
        } else if (CheckCollisionBoxSphere(pair.player2.bbox, pair.player1.phbox1.pos, pair.player1.phbox1.r)) {
            pair.player2.ntlh = 1;
            if (pair.player2.ntlh==1 && pair.player2.alh==0){
                pair.player2.alh = 1;
                printf("p1 hit p2\n");
                if (pair.player1.currentanim == PUNCH) {pair.player2.health -= 25;}
                if (pair.player1.currentanim == KICK) {pair.player2.health -= 50;}
                if (pair.player1.currentanim == SWEEP) {
                    pair.player2.health -= 40;
                    pair.player2.pha = 2;
                }
                if (pair.player1.currentanim == UPCUT) {pair.player2.health -= 30;}
                if (pair.player1.currentanim != SWEEP) {pair.player2.pha = 1;}
            }
        }
    } else {
        pair.player2.ntlh = 0;
        pair.player2.alh = 0;

    }
    if (pair.player2.phbox.active == 1){
        if (CheckCollisionBoxSphere(pair.player1.bbox, pair.player2.phbox.pos, pair.player2.phbox.r)){
            pair.player1.ntlh = 1;
            if (pair.player1.ntlh==1 && pair.player1.alh==0){
                pair.player1.alh = 1;
                printf("p2 hit p1\n");
                if (pair.player2.currentanim == PUNCH) {pair.player1.health -= 25;}
                if (pair.player2.currentanim == KICK) {pair.player1.health -= 50;}
                if (pair.player2.currentanim == SWEEP) {
                    pair.player1.health -= 40;
                    pair.player1.pha = 2;
                    }
                if (pair.player2.currentanim == UPCUT) {pair.player1.health -= 30;}
                if (pair.player2.currentanim != SWEEP)  { pair.player1.pha = 1; } 
            }  
        } else if (CheckCollisionBoxSphere(pair.player1.bbox, pair.player2.phbox1.pos, pair.player2.phbox1.r)) {
            pair.player1.ntlh = 1;
            if (pair.player1.ntlh==1 && pair.player1.alh==0){
                pair.player1.alh = 1;
                printf("p2 hit p1\n");
                if (pair.player2.currentanim == PUNCH) {pair.player1.health -= 25;}
                if (pair.player2.currentanim == KICK) {pair.player1.health -= 50;}
                if (pair.player2.currentanim == SWEEP) {
                    pair.player1.health -= 40;
                    pair.player1.pha = 2;
                    }
                if (pair.player2.currentanim == UPCUT) {pair.player1.health -= 30;}
                if (pair.player2.currentanim != SWEEP) { pair.player1.pha = 1; } 

            }
        }
    } else {
        pair.player1.ntlh = 0;
        pair.player1.alh = 0;
    }
    return pair;
}


int main(void)
{
    int dev_mode = 1;
    int framestep = 0;
    
    init_game();
    Camera3D camera = init_cam();
    
    Player p1;
    Player p2; 
    p1.pindex = 1;
    p2.pindex = 2;
    p1 = init_player(p1, p1.pindex);
    p2 = init_player(p2, p2.pindex);

    // shader things i do not understand
    Shader shader[MAX_POSTPRO_SHADERS] =  { 0 };
    shader[FX_SOBEL] = LoadShader(0, TextFormat("../resources/shaders/glsl%i/sobel.fs", GLSL_VERSION));
    shader[DEBUG] = LoadShader(0, TextFormat("../resources/shaders/glsl%i/base.fs", GLSL_VERSION));
    RenderTexture2D render_target = LoadRenderTexture(SCREEN_W, SCREEN_H);
    int currentShader = DEBUG;
    PlayerPair player_pair;
    player_pair = (PlayerPair){ p1 , p2 , 0 };


    while(!WindowShouldClose())
    {

        player_pair.player1 = p1;
        player_pair.player2 = p2;
        
        framestep++;
        if (framestep > 59){
            framestep = 0;
        }
        //printf("framestep: %d\n", framestep);
        
        BeginTextureMode(render_target); 
            ClearBackground(GRAY);
            /*******MODE 3D******/
            BeginMode3D(camera);
                draw_step_3D(p1, p2);
                if (dev_mode == DEBUG) {
                    debug_draw_3d(p1, p2);
                }
            EndMode3D();
        EndTextureMode();

            /*******MODE 2D******/
        BeginDrawing();
            BeginShaderMode(shader[currentShader]);
            DrawTextureRec(render_target.texture, (Rectangle) { 0, 
                                                                0, 
                                                                (float) render_target.texture.width, 
                                                                (float) -render_target.texture.height }, 
                                                                (Vector2){ 0, 0 }, 
                                                                WHITE);
            EndShaderMode();
            draw_step_2D(p1, p2, dev_mode);
        EndDrawing();

        //int x = GetKeyPressed();
        //if (x > 0) printf("keypress: %d\n", x);
        

        if (IsKeyPressed(KEY_B)){
            dev_mode = switch_dev_mode(dev_mode);
            currentShader = dev_mode;
        }
        camera = cam_controls(camera, dev_mode);
        p1 = player_controls(p1);
        p2 = player_controls(p2);

        p1 = p_face(p1, p2.pos.x);
        p2 = p_face(p2, p1.pos.x);
        p1.bbox = get_p_bbox(p1);
        p2.bbox = get_p_bbox(p2);
        p1 = fight_logic(p1);
        p2 = fight_logic(p2);
        player_pair = p_hit(player_pair);
        p1.pha = player_pair.player1.pha;
        p2.pha = player_pair.player2.pha;
        p1.health = player_pair.player1.health;
        p2.health = player_pair.player2.health;
        p1.ntlh = player_pair.player1.ntlh;
        p1.alh = player_pair.player1.alh;
        p2.ntlh = player_pair.player2.ntlh;
        p2.alh = player_pair.player2.alh;
        p1 = anim_controller(p1);
        p2 = anim_controller(p2);
        

        if (p1.pha > 0){
            printf("p1.pha: %d\n", p1.pha);
        }
        if (p2.pha > 0){
            printf("p2.pha: %d\n", p2.pha);
        }
        
        
        //p1 = hit_anim(p1);
        //p2 = hit_anim(p2);
        
    }

    UnloadRenderTexture(render_target);
    UnloadTexture(p1.ptexture);
    UnloadTexture(p2.ptexture);
    UnloadModel(p1.playermodel);
    UnloadModel(p2.playermodel);
    CloseWindow();
}