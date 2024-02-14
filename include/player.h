#include <raylib.h>
#include <stdio.h>

#define IDLE 1
#define WALKF 2
#define WALKB 3
#define CHSTANCE 4
#define CIDLE 5
#define UPCUT 6
#define PUNCH 7
#define KICK 8
#define SWEEP 9
#define HIT 10
#define FALL 11
#define GETUP 12
#define DANCE 13
#define GOLF 14
#define BLOCK 15
#define CBLOCK 16


static Vector3 stage_center = {25.0f, 0.0f, 10.0f};


typedef struct {
    Vector3 pos;
    float r;
    int active;
} Hitbox; // might want to add another pos or array of vector3

typedef struct {
    Vector3 right_h;
    Vector3 left_h;
    Vector3 right_l;
    Vector3 left_l;
} Hurtboxes;

typedef struct {
    Model playermodel;
    Texture2D ptexture;
    ModelAnimation* anims;
    BoundingBox bbox;
    Vector3 pos;
    Hitbox phbox;
    Hitbox phbox1;
    Hurtboxes phurtboxes;
    float face;
    int currentanim;
    int animcount;
    int health;
    int isAttacking;
    int pindex;
    int fc;
    int atk_fc;
    int blk_fc;
    int ntlh; // need to lower health
    int alh; // already lowered health
    int pha; // play hit anim
} Player;


typedef struct {
    Player player1;
    Player player2;
    int once;
} PlayerPair;



Player player_controls(Player p)
{
    if (p.pindex == 1){
        if (p.pha == 1){
            p.currentanim = HIT;
            return p;
        }
        if (p.pha == 2){
            p.currentanim = FALL;
            return p;
        } 
        if (IsKeyDown(KEY_LEFT) && !p.isAttacking) {
            p.pos.x += 0.1f;
            if(p.face == 90.0f){
                p.currentanim = WALKF; // flip anim
            } else {
                p.currentanim = WALKB;
            }
        } 
        else if (IsKeyDown(KEY_RIGHT) && !p.isAttacking) {
            p.pos.x -= 0.1f;
            if(p.face == 90.0f){
                p.currentanim = WALKB; // flip anim
            } else {
                p.currentanim = WALKF;
            }
        } 
        else if (IsKeyPressed(KEY_UP) && !p.isAttacking) {
            p.isAttacking = 1;
            p.currentanim = PUNCH;
        }
        else if (IsKeyPressed(KEY_KP_1) && !p.isAttacking) {
            p.isAttacking = 1;
            p.currentanim = UPCUT;
        }
        else if (IsKeyPressed(KEY_KP_2) && !p.isAttacking) {
            p.isAttacking = 1;
            p.currentanim = KICK;
        }
        else if (IsKeyPressed(KEY_KP_3) && !p.isAttacking) {
            p.isAttacking = 1;
            p.currentanim = DANCE;
        }
        else if (IsKeyPressed(KEY_KP_4) && !p.isAttacking) {
            p.isAttacking = 1;
            p.currentanim = BLOCK;

        }
        else if (IsKeyPressed(KEY_KP_5) && !p.isAttacking) {
            p.isAttacking = 1;
            p.currentanim = CBLOCK;

        }
        else if (IsKeyPressed(KEY_DOWN) && !p.isAttacking) {
            p.isAttacking = 1;
            p.currentanim = SWEEP;
        }
        else {
            if (!p.isAttacking){
                p.currentanim = IDLE;
                //p.pha=0;
            }
        }
    }
    if (p.pindex == 2){

        if (p.pha == 1){
            p.currentanim = HIT;
            return p;
        }
        if (p.pha == 2){
            p.currentanim = FALL;
            return p;
        } 
        if (IsKeyDown(KEY_A) && !p.isAttacking) {
            p.pos.x += 0.1f;
            if(p.face == 270.0f){
                p.currentanim = WALKB; // flip anim
            } else {
                p.currentanim = WALKF;
            }
        }
        else if (IsKeyDown(KEY_D) && !p.isAttacking) {
            p.pos.x -= 0.1f;
            if (p.face == 270.0f){
                p.currentanim = WALKF; // flip anim
            } else {
                p.currentanim = WALKB;
            }

        } 
        else if (IsKeyPressed(KEY_W) && !p.isAttacking) {
            p.isAttacking = 1;
            p.currentanim = PUNCH;

        }
        else if (IsKeyPressed(KEY_E) && !p.isAttacking) {
            p.isAttacking = 1;
            p.currentanim = UPCUT;

        }
        else if (IsKeyPressed(KEY_F) && !p.isAttacking) {
            p.isAttacking = 1;
            p.currentanim = BLOCK;

        }
        else if (IsKeyPressed(KEY_R) && !p.isAttacking) {
            p.isAttacking = 1;
            p.currentanim = CBLOCK;

        }
        else if (IsKeyPressed(KEY_Q) && !p.isAttacking) {
            p.isAttacking = 1;
            p.currentanim = KICK;

        }
        else if (IsKeyPressed(KEY_S) && !p.isAttacking) {
            p.isAttacking = 1;
            p.currentanim = SWEEP;

        }
        else if (IsKeyPressed(KEY_Z) && !p.isAttacking) {
            p.isAttacking = 1;
            p.currentanim = DANCE;

        }
        else {
            if (!p.isAttacking){
                p.currentanim = IDLE;
                //p.pha=0;
            }
        }
    }
    return p;
}


Player p_face (Player p, float otherpos)
{
    // on right
    if (p.pos.x > otherpos){p.face = 270.0f;} else {p.face = 90.0f;}
    return p;
}

Player p_neutral(Player p)
{
    p.fc++;
    UpdateModelAnimation(p.playermodel,  p.anims[p.currentanim], p.fc);  
    if (p.fc >= p.anims[p.currentanim].frameCount) {
        p.pha = 0;
        p.fc = 0;
    }
    return p; 
}

Player p_attack(Player p)
{
    p.atk_fc++;
    UpdateModelAnimation(p.playermodel,  p.anims[p.currentanim], p.atk_fc);  
    if (p.atk_fc >= p.anims[p.currentanim].frameCount) {
        p.isAttacking = 0;
        p.atk_fc = 0;
    }
    return p;
}
Player hit_anim(Player p){

    p.blk_fc++;
    if (p.pha == 1) {p.currentanim = HIT;}
    if (p.pha == 2) {p.currentanim = FALL;}
    UpdateModelAnimation(p.playermodel,  p.anims[p.currentanim], p.blk_fc);  
    if (p.blk_fc >= p.anims[p.currentanim].frameCount) {
        p.blk_fc = 0;
        p.pha = 0;
    }
    return p;
}

Player anim_controller(Player p)
{   
     
    if (!p.isAttacking && p.pha < 1){
        p.pha = 0;
        p = p_neutral(p);
        return p;
    }
    else if (p.pha > 0){
        p = hit_anim(p);
        return p;
    }
    else if (p.isAttacking){
        p = p_attack(p);
        return p;
    }
    return p;
}

Player init_player(Player p, int i)
{   
    p.playermodel = LoadModel("../anims/portale2.iqm");
    p.ptexture = LoadTexture("../anims/portale.png");
    SetMaterialTexture(&p.playermodel.materials[0], MATERIAL_MAP_DIFFUSE, p.ptexture);
    p.animcount = 0;
    p.anims = LoadModelAnimations("../anims/portale2.iqm", &p.animcount);
    p.fc = 0;
    p.atk_fc = 0;
    p.blk_fc = 0;
    p.pos = stage_center;
    p.currentanim = 0;
    p.health = 300;
    p.isAttacking = 0;
    p.ntlh = 0;
    p.alh = 0;
    p.pha = 0;

    if (i==1){
        p.pos.x -= 10.0f;
        p.face = 90.0f;
    }
    if(i==2){
        p.pos.x += 10.0f;
        p.face = 270.0f;
    }
    return p;
}
