#include "raylib.h"
#include <stdio.h>
#include <math.h>


int main() {
	float X[10000] = {0};
       	float Y[10000] = {0};
	float inertX[10000] = {0};
	float inertY[10000] = {0};
	float X2[10000];
	float Y2[10000];
	int count = 0;
	const int width = 700;
	const int height = 400;
	int middleX = (int)((float)width/2);
	int middleY = (int)((float)height/2);
	InitWindow(width, height, "this is a test");
	SetTargetFPS(60);
	int paused = 1;
	float zoom = 10;
	float moveX = 0;
	float moveY = 0;
	int details = 0;
	int details2 = 0;
	int details3 = 0;
	int details4 = 0;
	int details5 = 0;
	float xStart = 0;
       	float yStart = 0;
	float xEnd = 0;
	float yEnd = 0;
	int velocX = 0;
	int velocY = 0;
	int FindClusters = 0;
	int Clock = 0;
	while (!WindowShouldClose()) {
		int iint;
		for (iint = 0; iint < count; ++iint) {
			moveX += X[iint];
			moveY += Y[iint];
		}
		if ((count > 0)) {
			moveX = middleX-moveX/(count*zoom);
			moveY = middleY-moveY/(count*zoom);
		}
		//
		if (IsKeyPressed(81)*details) {
			int pos1 = GetMouseX();
			int pos2 = GetMouseY();
			if (details3) {
				//now the velocity vector
				velocX = pos1 - xStart;
				velocY = pos2 - yStart;
				details3 = 0;
				details2 = 0;
				details = 0;
				details4 = 1;
			}
			else {
				if ((1-details2)*(1-details3)) {
					//first corner
					xStart = pos1;
					yStart = pos2;
					details2 = 1;
				}
				else {
					//second corner
					xEnd = pos1;
					yEnd = pos2;
					details3 = 1;
				}
			}
			if (details4*(1-details5)) {
				xStart = ((float)(xStart));
				yStart = ((float)(yStart));
				//now modify the values to correspond
				float Xmax = fmax(xStart, xEnd);
				float Ymax = fmax(yStart, yEnd);
				float Xmin = fmin(xStart, xEnd);
				float Ymin = fmin(yStart, yEnd);
				//printf("%f, %f, %f, %f\n", Xmax, Ymax, Xmin, Ymin);
				int ill;
				for (ill = 0; ill < count; ++ill) {
					float XvaL = (X[ill]/zoom) + (float)(moveX);
					float YvaL = (Y[ill]/zoom) + (float)(moveY);
					if ((XvaL < Xmax)*(XvaL > Xmin)*(YvaL < Ymax)*(YvaL > Ymin)) {
						//printf("here\n");
						//then it falls within the bounds
						inertX[ill] += zoom*(float)velocX/(4000);
						inertY[ill] += zoom*(float)velocY/(4000);
					}
				}
				details = 0;
				details2 = 0;
				details3 = 0;
				details4 = 0;
				details5 = 0;
			}
		}
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)*(1-details)*(1-details4)) {
			int pos1 = GetMouseX();
			int pos2 = GetMouseY();
			//get distance from mouse to the last point placed for a uniform distribution of points
			float distdelta = sqrt(pow(pos1 - X[count-1], 2) + pow(pos2 - Y[count - 1], 2));
			if (distdelta > 5) {
				X[count] = (float)pos1*zoom - moveX*zoom;
				Y[count] = (float)pos2*zoom - moveY*zoom;
				count += 1;
			}
		}
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
			if (count > 0) {
				count = count - 1;
			}
		}
		if (IsKeyPressed(32)) {
			paused = 1-paused;
		}
		if (IsKeyPressed(61)) {
			zoom = zoom/1.2;
		}
		if (IsKeyPressed(45)) {
			zoom = (zoom*1.2);
		}
		if (IsKeyPressed(77)) {
			details = 1 - details;
		}
		if (IsKeyPressed(78)) {
			FindClusters = 1 - FindClusters;
		}
		if (1-paused) {
			//then do one game tick, calculate the positions of each one step forward
			int i;
			int j;
			for (i = 0; i < count; ++i) {
				X2[i] = X[i];
				Y2[i] = Y[i];
				for (j = 0; j < count; ++j) {
					if (j != i) {
						float x1 = X[i];
						float y1 = Y[i];
						float x2 = X2[j];
						float y2 = Y2[j];
						float distance = sqrt(pow(x1-x2,2) + pow(y1-y2,2));
						float dx = 0;
						float dy = 0;
						if (distance != 0) {
							if (distance > 1) {
								dx = 0.1*(x2-x1)/(pow(distance,2));
								dy = 0.1*(y2-y1)/(pow(distance,2));
							}
							else {
								dx = 0;
								dy = 0;
							}
						}
						X2[i] += dx + inertX[i];
						Y2[i] += dy + inertY[i];
						inertX[i] += dx;
						inertY[i] += dy;
					}
				}
			}
			for (i = 0; i < count; ++i) {
				X[i] = X2[i];
				Y[i] = Y2[i];
			}
		}
		
		//draw it --
		BeginDrawing();
		DrawText(TextFormat("Total used: %02i / 10000", count), 10, 10, 10, BLACK);
		DrawText("Left click to draw particles", 10, 50, 10, BLACK);
		DrawText("Right click to remove recently drawn particles", 10, 100, 10, BLACK);
		DrawText("Space to pause/unpause", 10, 150, 10, BLACK);
		DrawText("+/- to zoom in/out", 10, 200, 10, BLACK);
		//DrawText("n to enable/disable cluster analysis", 10, 300, 10, BLACK);
		if ((1 - details)) {
			DrawText("m to modify velocity of selected particles", 10, 250, 10, BLACK);
		}
		else {
			if (1-details3) {
				DrawText("select range of particles to modify using mouse", 10, 250, 10, BLUE);
			}
			else {
				DrawText("draw the velocity vector for the particles", 10, 250, 10, BLUE);
			}
		}
		//how much space left for points:
		int i;
		for (i = 0; i < count; ++i) {
			DrawPixel((int)(((float)X[i]/zoom) + moveX), (int)(((float)Y[i]/zoom) + moveY), BLACK);
			DrawPixel(1 + (int)(((float)X[i]/zoom) + moveX), (int)(((float)Y[i]/zoom) + moveY), BLACK);
			DrawPixel((int)(((float)X[i]/zoom) + moveX), (int)(((float)Y[i]/zoom) + moveY) + 1, BLACK);
			DrawPixel(1 + (int)(((float)X[i]/zoom) + moveX), (int)(((float)Y[i]/zoom) + moveY) + 1, BLACK);
		}
		if (details2*(1-details3)) {
			int pos1 = GetMouseX();
			int pos2 = GetMouseY();
			DrawLine(xStart, yStart, xStart, pos2, BLUE);
			DrawLine(xStart, yStart, pos1, yStart, BLUE);
			DrawLine(xStart, pos2, pos1, pos2, BLUE);
			DrawLine(pos1, yStart, pos1, pos2, BLUE);
		}
		if (details3) {
			int pos1 = GetMouseX();
			int pos2 = GetMouseY();
			DrawLine(xStart, yStart, pos1, pos2, BLUE);
		}
		ClearBackground(WHITE);
		EndDrawing();
		//----------
	}
}
