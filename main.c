#include "Controller/Controller.h"
#include "View/Display.h"
#include "View/SpaceObjects.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main(int argc, char const *argv[])
{
	if (!initiateController()) return EXIT_FAILURE;
	atexit(cleanupController);
	
	
	Point3D floor[200] = { 0 };
	for (int i = 0; i < 50; i++)
	{
		floor[i].x = 0.8*(i - 100);
		floor[i].z = 100;
	}
	
	Polygon poly = { .point = {
					{ 0, 5, 0 },
					{ 10, 5, 10 },
					{ -20, 5, 5 },
				}};
	
	
	Camera player = { 0 };
	
	player.field_of_view.xz = evaluateFov(1/sqrt(3));
	player.field_of_view.yz = evaluateFov(1/sqrt(3));
	
	//system("clear");
	for (enum ControlKey ck = CK_NOKEY; ck != CK_QUIT; ck = readControllerInput())
	{
		if (ck == CK_NOKEY)
		{
			usleep(1000);
			continue;
		}
		else if (ck == CK_QUIT) break;
		
		system("clear");
		switch(ck)
		{
			case CK_UP:
				player.position.y += 1*cos(player.direction.yz);
				break;
			
			case CK_DOWN:
				player.position.y -= 1*cos(player.direction.yz);
				break;
			
			case CK_RIGHT:
				player.position.x += 1*cos(player.direction.xz);
				player.position.z -= 1*sin(player.direction.xz);
				break;
			
			case CK_LEFT:
				player.position.x -= 1*cos(player.direction.xz);
				player.position.z += 1*sin(player.direction.xz);
				break;
			
			case CK_FORWARD:
				player.position.z += 1*cos(player.direction.xz);
				player.position.x += 1*sin(player.direction.xz);
				break;
			
			case CK_BACKWARD:
				player.position.z -= 1*cos(player.direction.xz);
				player.position.x -= 1*sin(player.direction.xz);
				break;
			
			case CK_LTURN:
				{
					Angle3D turn = { .xz = deg2rad(6), .yz = 0 };
					if ((player.direction.xz - turn.xz) > -M_PI)
					{
						subAngle3D(&player.direction, &turn);
					}
				}
				break;
			
			case CK_RTURN:
				{
					Angle3D turn = { .xz = deg2rad(6), .yz = 0 };
					if ((player.direction.xz + turn.xz) < M_PI)
					{
						addAngle3D(&player.direction, &turn);
					}
				}
				break;
			
			default:
				break;
		}
		
		clearGrid();
		for (int i = 0; i < 3; i++)
		{
			renderPoint(&player, &poly.point[i]);
		}
		for (int i = 0; i < 50; i++)
		{
			renderPoint(&player, &floor[i]);
		}
		
		fprintf(stderr, "Camera: %s | %s\n", Point3D_toStr(&player.position), Angle3D_toStr(&player.direction));
		printGrid();
	}
	
	return EXIT_SUCCESS;
}
