#include <bits/stdc++.h>
#include <unistd.h>

float A, B, C;
const int width = 120, height = 55;
float zBuffer[width * height];
char buffer[width * height];
int backgroundASCIICode = ' ';
int distanceFromCam = 120;
float K1 = 80;
float x, y, z;
float ooz;
int xp, yp;
int idx;
float U, V;

const char* shades = ".,-~:;=!*#$@";

void calculate(float i, float j){
    float tR = 0.2;
    float x0 = sin(i) + 2 * sin(2 * i);
    float y0 = cos(i) - 2 * cos(2 * i);
    float z0 = -sin(3 * i);

    float xt = x0 + tR * cos(j);
    float yt = y0 + tR * sin(j);
    float zt = z0;

    float scale = 10;
    xt *= scale; yt *= scale; zt *= scale;

    float x1 = xt;
    float y1 = yt*cos(A) - zt*sin(A);
    float z1 = yt*sin(A) + zt*cos(A);

    float x2 = x1*cos(B) + z1*sin(B);
    float y2 = y1;
    float z2 = -x1*sin(B) + z1*cos(B);

    float x3 = x2*cos(C) - y2*sin(C);
    float y3 = x2*sin(C) + y2*cos(C);
    float z3 = z2;

    z = z3 + distanceFromCam;
    ooz = 1 / z;
    xp = (int)(width / 2 + K1 * ooz * x3 * 2);
    yp = (int)(height / 2 + K1 * ooz * y3);
    idx = xp + yp * width;
    if(idx >= 0 && idx < width * height){
        if(ooz > zBuffer[idx]){
            zBuffer[idx] = ooz;

            float nx = cos(j), ny = sin(j), nz = 0;
            float shade = nx*0.5 + ny*0.7 + nz*0.2;
            if(shade < 0) shade = 0;
            int shade_index = std::min(10, int(shade*10));
            buffer[idx] = shades[shade_index];
        }
    }
}

int main(){
    printf("\x1b[2J");

    while(1){
        memset(buffer, backgroundASCIICode, width * height);
        memset(zBuffer, 0, width * height * 4);

        for(U = 0; U <= 6.28; U += 0.05){
            for(V = 0; V <= 6.28; V += 0.3){
                calculate(U, V);
            }
        }
        printf("\x1b[H");
        for(int k = 0; k < width * height; k++){
            putchar(k % width ? buffer[k] : 10);
        }
        A += 0.03;
        B += 0.02;
        C += 0.01;
        usleep(5000);
    }
}