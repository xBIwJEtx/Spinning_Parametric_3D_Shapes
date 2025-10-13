#include <bits/stdc++.h>
#include <unistd.h>

float A, B, C;
const int circle_r = 5;
const int torus_r = 20;
const int width = 80, height = 44;
float zBuffer[width * height];
char buffer[width * height];
int backgroundASCIICode = ' ';
int distanceFromCam = 100;
float K1 = 40;
float x, y, z;
float ooz;
int xp, yp;
int idx;
float theta, phi;

float calculateX(float i, float j){
    return (torus_r + circle_r * cos(j)) * cos(i) * (cos(B) * cos(C)) + (torus_r + circle_r * cos(j)) * sin(i) * (sin(A) * sin(B) * cos(C) - cos(A) * sin(C)) + circle_r * sin(j) * (cos(A) * sin(B) * cos(C) + sin(A) * sin(C));
}

float calculateY(float i, float j){
    return (torus_r + circle_r * cos(j)) * cos(i) * (cos(B) * sin(C)) + (torus_r + circle_r * cos(j)) * sin(i) * (sin(A) * sin(B) * sin(C) + cos(A) * cos(C)) + circle_r * sin(j) * (cos(A) * sin(B) * sin(C) - sin(A) * cos(C));
}

float calculateZ(float i, float j){
    return -(torus_r + circle_r * cos(j)) * cos(i) * sin(B) + (torus_r + circle_r * cos(j)) * sin(i) * (sin(A) * cos(B)) + circle_r * sin(j) * (cos(A) * cos(B));
}

void calculate(float i, float j, int ch){
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
            buffer[idx] = ch;
        }
    }
}

int main(){
    printf("\x1b[2J");

    while(1){
        memset(buffer, backgroundASCIICode, width * height);
        memset(zBuffer, 0, width * height * 4);

        for(phi = 0; phi < 6.28; phi += 0.1){
            for(theta = 0; theta < 6.28; theta += 0.1){
                calculate(phi, theta, '@');
                calculate(-phi, theta, '$');
                calculate(-phi, -theta, '#');
                calculate(phi, -theta, '+');
                calculate(theta, phi, '%');
            }
        }
        printf("\x1b[H");
        for(int k = 0; k < width * height; k++){
            putchar(k % width ? buffer[k] : 10);
        }
        A += 0.05;
        B += 0.05;
        C += 0.01;
        usleep(8000);
    }
}