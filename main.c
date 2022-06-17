#include "CodeTools.h"
#include "Controller/Controller.h"
#include "Math/AngleMath.h"
#include "View/Display.h"
#include "View/Camera.h"
#include "Math/MathLib.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>



static void assert_vec4(void)
{
	Vec4 v = {{0,0,0,0}};
	assert(&v.x == &v.tuple[0]);
	assert(&v.y == &v.tuple[1]);
	assert(&v.z == &v.tuple[2]);
	assert(&v.w == &v.tuple[3]);
}


static double getWallTime()
{
	struct timeval time = { 0 };
	
	if (gettimeofday(&time, NULL))
	{
		return 0; //unhandled error
	}
	return (double)time.tv_sec + (double)time.tv_usec/1000000;
}


static void printCameraStatistics(Camera* cam)
{
	fprintf(stderr, "Camera: %s \t", Vec4_ToStr(&cam->position));
	fprintf(stderr, "fov=(%+.2f;%+.2f)   |   ", rad2deg(cam->x_fov), rad2deg(cam->y_fov));
	fprintf(stderr, "up = %+.2f, left = %+.2f   \n", rad2deg(cam->view.up), rad2deg(cam->view.left));
}





static Camera player = { 0 };
// static Polygon poly = {
// 		{{5,0,10,1}},
// 		{{0,10,10,1}},
// 		{{-5,0,10,1}},
// };

static Polygon L1 = {
		{{-10,-1,5,1}},
		{{-10,5,5,1}},
		{{-10,-1,100,1}},
};
static Polygon L2 = {
		{{-10,5,100,1}},
		{{-10,-1,100,1}},
		{{-10,-1,5,1}},
};

static Polygon R1 = {
		{{+10,-1,5,1}},
		{{+10,5,5,1}},
		{{+10,-1,100,1}},
};
static Polygon R2 = {
		{{+10,5,100,1}},
		{{+10,-1,100,1}},
		{{+10,5,5,1}},
};


static bool loop(enum ControlKey ctrlKey, double deltaTime);

int main(int argc, char const *argv[])
{
	assert_vec4();
	if (!initiateController()) return EXIT_FAILURE;
	atexit(cleanupController);
	
	
	zoomCamera(&player, -deg2rad(120), -deg2rad(60));
	player.position.w = 1;
	
	DISPLAY_CLEAR();
	double prevTime = getWallTime();
	for (enum ControlKey ck = CK_NOKEY; ck != CK_QUIT; ck = readControllerInput())
	{
		if (ck == CK_QUIT) break;
		usleep(5000);
		double deltaTime = getWallTime() - prevTime;
		
		DISPLAY_GOTOXY(0, 0);
		clearGrid();
		
		printCameraStatistics(&player);
		fprintf(stderr, "Elapsed time: %d[us]\n", (int)(deltaTime*1000000));
		
		if (!loop(ck, deltaTime)) break;
		printGrid();
		prevTime = getWallTime();
	}
	DISPLAY_CLEAR();
	
	return EXIT_SUCCESS;
}




static bool loop(enum ControlKey ctrlKey, double deltaTime)
{
	renderPolygon(&player, L1);
	renderPolygon(&player, L2);
	renderPolygon(&player, R1);
	renderPolygon(&player, R2);
	
	if (player.position.y > 0)
	{
		player.position.y -= 0.01;
		if (player.position.y < 0)
		{
			player.position.y = 0;
		}
	}
	
	const int WALK_SPEED = 200;
	if (ctrlKey == CK_NOKEY) return true;
	switch(ctrlKey)
	{
		case CK_UP:
			if (player.position.y == 0)
			{
				player.position.y += 2;
			}
			break;
		
		case CK_RIGHT:
			player.position.x += cos(player.view.left) * WALK_SPEED*deltaTime*2;
			player.position.z -= sin(player.view.left) * WALK_SPEED*deltaTime*2;
			break;
		
		case CK_LEFT:
			player.position.x -= cos(player.view.left) * WALK_SPEED*deltaTime*2;
			player.position.z += sin(player.view.left) * WALK_SPEED*deltaTime*2;
			break;
		
		case CK_FORWARD:
			player.position.z += cos(player.view.left) * WALK_SPEED*deltaTime;
			player.position.x -= sin(player.view.left) * WALK_SPEED*deltaTime;
			break;
		
		case CK_BACKWARD:
			player.position.z -= cos(player.view.left) * WALK_SPEED*deltaTime;
			player.position.x += sin(player.view.left) * WALK_SPEED*deltaTime;
			break;
		
		case CK_LTURN:
			ViewDirection_add(&player.view, &(ViewDirection){ .up = 0, .left = deg2rad(2) });
			break;
		
		case CK_RTURN:
			ViewDirection_sub(&player.view, &(ViewDirection){ .up = 0, .left = deg2rad(2) });
			break;
		
		
		case CK_XINZOOM:
			zoomCamera(&player, deg2rad(0.5), 0);
			break;
		
		case CK_XOUTZOOM:
			zoomCamera(&player, -deg2rad(0.5), 0);
			break;
		
		case CK_YINZOOM:
			zoomCamera(&player, 0, deg2rad(0.5));
			break;
		
		case CK_YOUTZOOM:
			zoomCamera(&player, 0, -deg2rad(0.5));
			break;
		
		
		default:
			break;
	}
	
	return true;
}
