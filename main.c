#include "Controller/Controller.h"
#include "View/Display.h"
#include "View/SpaceObjects.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



int main(int argc, char const *argv[])
{
	if (!initiateController()) return EXIT_FAILURE;
	atexit(cleanupController);
	
	
	Point3D p1 = { 0, 0, 10 };
	Point3D p2 = { -6, 23, -5 };
	Point3D p3 = { 37, 30, -7 };
	
	Camera player = { 0 };
	
	player.field_of_view.xz = deg2rad(120);
	player.field_of_view.yz = deg2rad(80);
	player.direction.xz = deg2rad(45);
	
	
	system("clear");
	for (enum ControlKey ck = CK_NOKEY; ck != CK_QUIT; ck = readControllerInput())
	{
		if (ck == CK_NOKEY)
		{
			usleep(1000);
			continue;
		}
		else if (ck == CK_QUIT) break;
		
		
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
			
			default:
				break;
		}
		
		clearGrid();
		system("clear");
		
		renderPoint(&player, &p1);
		renderPoint(&player, &p2);
		renderPoint(&player, &p3);
		
		printf("Camera: %s\n", Point3D_toStr(&player.position));
		printf("p1: %s\n", Point3D_toStr(&p1));
		printf("p2: %s\n", Point3D_toStr(&p3));
		printf("p3: %s\n", Point3D_toStr(&p2));
		printGrid();
	}
	
	return EXIT_SUCCESS;
}
