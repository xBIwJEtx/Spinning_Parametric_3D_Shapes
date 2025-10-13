#include <bits/stdc++.h>
#include <unistd.h>

float A, B, C;
const int width = 120, height = 44;
float zBuffer[width * height];
char buffer[width * height];
int backgroundASCIICode = ' ';
int distanceFromCam = 25;
float K1 = 100;
float x, y, z;
float ooz;
int xp, yp;
int idx;
float U, V;

const char* shades = ".,-~:;=!*#$@";

void parKlein(float u, float v, float &x, float &y, float &z){
    float r = 2.0;
    x = (r + cos(u/2)*sin(v) - sin(u/2)*sin(2*v)) * cos(u);
    y = (r + cos(u/2)*sin(v) - sin(u/2)*sin(2*v)) * sin(u);
    z = sin(u/2)*sin(v) + cos(u/2)*sin(2*v);
}

void cal(float u, float v, float &nx, float &ny, float &nz){
    float du = 0.01, dv = 0.01;
    float x1, y1, z1, x2, y2, z2, x3, y3, z3;

    parKlein(u, v, x1, y1, z1);
    parKlein(u + du, v, x2, y2, z2);
    parKlein(u, v + dv, x3, y3, z3);

    float ux = x2 - x1, uy = y2 - y1, uz = z2 - z1;
    float vx = x3 - x1, vy = y3 - y1, vz = z3 - z1;

    nx = uy * vz - uz * vy;
    ny = uz * vx - ux * vz;
    nz = ux * vy - uy * vx;

    float norm = sqrt(nx*nx + ny*ny + nz*nz);
    if(norm > 0) nx /= norm, ny /= norm, nz /= norm;
}

void calculate(float i, float j){
    float r = 2.0;

    float x0 = (r + cos(i/2)*sin(j) - sin(i/2)*sin(2*j)) * cos(i);
    float y0 = (r + cos(i/2)*sin(j) - sin(i/2)*sin(2*j)) * sin(i);
    float z0 = sin(i/2)*sin(j) + cos(i/2)*sin(2*j);

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
            float nx, ny, nz;
            cal(i, j, nx, ny, nz);

            float ny1 = ny * cos(A) - nz * sin(A);
            float nz1 = ny * sin(A) + nz * cos(A);
            float nx1 = nx;
            float nz2 = -nx1 * sin(B) + nz1 * cos(B);
            float nx2 = nx1 * cos(B) + nz1 * sin(B);
            float ny2 = ny1;
            float nx3 = nx2 * cos(C) - ny2 * sin(C);
            float ny3 = nx2 * sin(C) + ny2 * cos(C);
            float nz3 = nz2;

            float lx = 0.5, ly = 1, lz = -1;
            float norm = sqrt(lx*lx + ly*ly + lz*lz);
            lx /= norm; ly /= norm; lz /= norm;

            float L = nx3*lx + ny3*ly + nz3*lz;
            if (L < 0) L = 0;

            int shade_index = (int)(L * 11);
            buffer[idx] = shades[shade_index];
            zBuffer[idx] = ooz;
        }
    }
}

int main(){
    printf("\x1b[2J");

    while(1){
        memset(buffer, backgroundASCIICode, width * height);
        memset(zBuffer, 0, width * height * 4);

        for(U = 0; U <= 6.28; U += 0.05){
            for(V = 0; V <= 6.28; V += 0.05){
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