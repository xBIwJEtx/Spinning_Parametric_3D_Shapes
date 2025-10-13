#include <bits/stdc++.h>
#include <unistd.h>

float A, B, C;
const int width = 120, height = 44;
float zBuffer[width * height];
char buffer[width * height];
int backgroundASCIICode = ' ';
int distanceFromCam = 40;
float K1 = 40;
float x, y, z;
float ooz;
int xp, yp;
int idx;
float U,V;

const char* shades = ".,-~:;=!*#$@";

void calculate(float i, float j){
    float r = 0.5;
    float x0 = 16 * sin(i) * sin(i) * sin(i) + r * cos(j);
    float y0 = 13 * cos(i) - 5 * cos(2 * i) - 2 * cos(3 * i) - cos(4 * i) + r * sin(j);
    float z0 = r * sin(j/2);

    float x1 = x0;
    float y1 = y0*cos(A) - z0*sin(A);
    float z1 = y0*sin(A) + z0*cos(A);

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

            int shade_index = std::min(10, std::max(0, int(ooz * 12)));
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
            for(V = 0; V <= 6.28; V += 0.1){
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
        usleep(8000);
    }
}