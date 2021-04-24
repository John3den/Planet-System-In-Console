#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#define WIDTH 100
#define HEIGHT 50

char* light;
//move this somewhere?
typedef struct vec2{
  float x;
  float y;
} vec2;

typedef struct Body{
  float mass;
  float size;// size ranges [0;1] where 1 is fucking big
  vec2 position; // positions range ([0;1];[0;1])
  vec2 velocity;
} Body;

void drawStuff(char** canvas, Body* stuff, int amountOfStuff){
  for(int i=0;i<amountOfStuff;i++)
  {
    Body body = stuff[i];
    //checking square which captures the entire body (which itself is a circle)
    for(int ypos = HEIGHT*((float)body.position.y-(float)body.size); ypos < HEIGHT*((float)body.position.y + (float)body.size);ypos++)
    {
      for(int xpos = WIDTH*(body.position.x-body.size); xpos < WIDTH*(body.position.x + body.size);xpos++)
        {
          if( (((float)xpos)/WIDTH-body.position.x)*(((float)xpos)/WIDTH-body.position.x) +
              (((float)ypos)/HEIGHT-body.position.y)*(((float)ypos)/HEIGHT-body.position.y) < body.size*body.size)
          {
            float br;
            float mx = 0.5 - ((float)xpos)/WIDTH;
            float my = 0.5 - ((float)ypos)/HEIGHT;
            float nx = ((float)xpos)/WIDTH - body.position.x;
            float ny = ((float)ypos)/HEIGHT - body.position.y;
            float mag = sqrt(nx*nx + ny*ny);
            nx = nx / mag;
            ny = ny / mag;
            float str = (mx*mx + my*my);
            mx = mx/str;
            my = my/str;
            br = (nx*mx + ny*my)+1;
            if(br<0) br =0;

            int ind =abs((int)br);
            //printf(" %i ",ind);
            if(i==0) ind = 9;
            if(ypos>=0 && ypos<HEIGHT)
              if(xpos>=0&&xpos<WIDTH)
              if(ind>=0 && ind<10)
              {
                canvas[ypos][xpos] = light[ind];
              }
          }
        }
    }
  }
}

void physics(Body* system,int amount,float dt){
  float dx = 0;
  float dy = 0;
//apply forces to change vel
for(int i=0;i<amount;i++)
{
  for(int j=0;j<amount;j++){
    if(i==j) continue;
    vec2 nv;
    nv.x = system[i].velocity.x + system[j].mass*(system[j].position.x-system[i].position.x)*dt/system[i].mass;
    nv.y = system[i].velocity.y + system[j].mass*(system[j].position.y-system[i].position.y)*dt/system[i].mass;
    system[i].velocity = nv;
  }
}
//
for(int i=0;i<amount;i++)
{
  vec2 pos;

  pos.x = system[i].position.x + system[i].velocity.x*dt;
  pos.y = system[i].position.y + system[i].velocity.y*dt;
  if(i==0)
  {
    dx = system[i].velocity.x*dt;
    dy = system[i].velocity.y*dt;
  }
    pos.x -=dx;
    pos.y -=dy;

  system[i].position = pos;
}
}

void clearCanvas(char** canvas){
  for(int i=0;i<HEIGHT;i++){
    for(int j=0;j<WIDTH;j++){
      canvas[i][j]=' ';
    }
  }
}

void display(char** canvas)
{
  for(int i=0;i<HEIGHT;i++){
    for(int j=0;j<WIDTH;j++){
      printf("%c",canvas[i][j]);
    }
    printf("\n");
  }
}

Body makeBody(float x,float y,float size,float mass,float vx,float vy){
  Body b;
  vec2 pos;
  pos.x = x;
  pos.y = y;
  vec2 vel;
  vel.x = vx;
  vel.y = vy;
  b.position = pos;
  b.mass = mass;
  b.size = size;
  b.velocity = vel;
  return b;
}

int main(){
  light = malloc(10);
  light[0] = '.';
  light[1] = '^';
  light[2] = ':';
  light[3] = ';';
  light[4] = '?';
  light[5] = '&';
  light[6] = '$';
  light[7] = '(';
  light[8] = '#';
  light[9] = '@';

  int numOfBodies = 3;
  float dt=0.003;
  Body* bodies = (Body*)malloc(numOfBodies*sizeof(Body));
  bodies[0] = makeBody(0.5,0.5,0.1,10,0,0);
  bodies[1] = makeBody(0.2,0.2,0.07,1,0.5,-0.5);
  bodies[2] = makeBody(0.7,0.6,0.08,5,0.4,-0.4);
  //bodies[3] = makeBody(0.8,0.2,0.07,1,0,0);
  char** canvas = (char**)malloc(HEIGHT*sizeof(char*));
  for(int i=0;i<HEIGHT;i++)
  {
    canvas[i] = (char*)malloc(WIDTH*sizeof(char));
  }

  while(1)
  {
    //_sleep(dt*1000);
    physics(bodies,numOfBodies,dt*30);
    system("cls");
    clearCanvas(canvas);
    drawStuff(canvas,bodies,numOfBodies);
    display(canvas);
  }
  return 0;
}
