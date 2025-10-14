#include <bits/stdc++.h>
#include <unistd.h>

float A, B, C;
const int width = 120, height = 44;
float zBuffer[width * height];
char buffer[width * height];
int backgroundASCIICode = ' ';
int distanceFromCam = 15;
float K1 = 60;
float x, y, z;
float ooz;
int xp, yp;
int idx;
float U,V;

const char* shades = ".,-~:;=!*#$@";

void calculate(float i, float j){
    float x0 = i - (i * i * i)/3 + i * j * j;
    float y0 = -j + (j * j * j)/3 - j * i * i;
    float z0 = i * i - j * j;

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

            int shade_index = std::min(10, std::max(0, int(ooz * 20)));
            buffer[idx] = shades[shade_index];
        }
    }
}

int main(){
    printf("\x1b[2J");

    while(1){
        memset(buffer, backgroundASCIICode, width * height);
        memset(zBuffer, 0, width * height * 4);

        for(U = -1.5; U <= 1.5; U += 0.05){
            for(V = -1.5; V <= 1.5; V += 0.05){
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
