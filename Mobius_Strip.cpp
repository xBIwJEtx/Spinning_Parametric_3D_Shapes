#include <bits/stdc++.h>
#include <unistd.h>

float A, B, C;
const int width = 80, height = 44;
float zBuffer[width * height];
char buffer[width * height];
int backgroundASCIICode = ' ';
int distanceFromCam = 5;
float K1 = 60;
float x, y, z;
float ooz;
int xp, yp;
int idx;
float U, V;

const char* shades = ".,-~:;=!*#$@";

float calculateX(float u, float v){
    return ((1+v/2*cos(u/2))*cos(u)*cos(B)+v/2*sin(u/2)*sin(B))*cos(C)-(1+v/2*cos(u/2))*sin(u)*cos(A)-(v/2*sin(u/2)*sin(A))*sin(C);
}

float calculateY(float u, float v){
    return ((1+v/2*cos(u/2))*cos(u)*cos(B)+v/2*sin(u/2)*sin(B))*sin(C)+((1+v/2*cos(u/2))*sin(u)*cos(A)-v/2*sin(u/2)*sin(A))*cos(C);
}

float calculateZ(float u, float v){
    return -(1+v/2*cos(u/2))*cos(u)*sin(B)+((1+v/2*cos(u/2))*sin(u)*sin(A)+v/2*sin(u/2)*cos(A));
}

void calculate(float i, float j){
    x = calculateX(i, j);
    y = calculateY(i, j);
    z = calculateZ(i, j) + distanceFromCam;
    ooz = 1 / z;
    xp = (int)(width / 2 + K1 * ooz * x * 2);
    yp = (int)(height / 2 + K1 * ooz * y);
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
            for(V = -0.5; V <= 0.5; V += 0.05){
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