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
	
	Polygon poly = { .point = {
					{ 10, 10, 10 },
					{ 10, 15, 10 },
					{ 15, 10, 10 },
				}};
	
	
	Camera player = { 0 };
	
	player.field_of_view.xz = evaluateFov(1/sqrt(3));
	player.field_of_view.yz = evaluateFov(1/sqrt(3));
	
	system("clear");
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
				player.position.y++;
				break;
			
			case CK_DOWN:
				player.position.y--;
				break;
			
			case CK_RIGHT:
				player.position.x++;
				break;
			
			case CK_LEFT:
				player.position.x--;
				break;
			
			case CK_FORWARD:
				player.position.z++;
				break;
			
			case CK_BACKWARD:
				player.position.z--;
				break;
			
			case CK_LTURN:
				{
					Angle3D turn = { .xz = deg2rad(6), .yz = 0 };
					fprintf(stderr, "before = %.3f | expected = %.3f | ", rad2deg(player.direction.xz), rad2deg(player.direction.xz - turn.xz));
					subAngle3D(&player.direction, &turn);
					fprintf(stderr, "after = %.3f\n\n", rad2deg(player.direction.xz));
				}
				break;
			
			case CK_RTURN:
				{
					Angle3D turn = { .xz = deg2rad(6), .yz = 0 };
					fprintf(stderr, "before = %.3f | expected = %.3f | ", rad2deg(player.direction.xz), rad2deg(player.direction.xz + turn.xz));
					addAngle3D(&player.direction, &turn);
					fprintf(stderr, "after = %.3f\n\n", rad2deg(player.direction.xz));
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
		
		fprintf(stderr, "\nCamera: %s | %s\n", Point3D_toStr(&player.position), Angle3D_toStr(&player.direction));
		printGrid();
	}
	
	return EXIT_SUCCESS;
}
