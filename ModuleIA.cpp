#include "Globals.h"
#include "Application.h"
#include "ModuleIA.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"
#include "ModuleCollision.h"

ModuleIA::ModuleIA(bool start_enabled) : Module(start_enabled)
{
	// Create 3 rival IA cars
	cars.push_back(new ModulePlayer());
	cars.push_back(new ModulePlayer());
	cars.push_back(new ModulePlayer());

	// Define initial positions
	cars[0]->position.x = 177;
	cars[0]->position.y = 165;
	cars[1]->position.x = 195;
	cars[1]->position.y = 165;
	cars[2]->position.x = 195;
	cars[2]->position.y = 177;

	// Add checkpoints to checkpoints containers
	for (int i = 115; i < 141; i+=5)
	{
		vector<iPoint> tempCheckpoint;
		for (int j = 160; j < 179; j++)
		{
			tempCheckpoint.push_back({ i, j });
		}

		checkpointContainer1.push_back(tempCheckpoint);
	}

	for (int i = 75; i < 91; i+=5)
	{
		vector<iPoint> tempCheckpoint;
		for (int j = 112; j < 177; j++)
		{
			tempCheckpoint.push_back({ j, i });
		}

		checkpointContainer2.push_back(tempCheckpoint);
	}

	for (int i = 220; i < 241; i += 5)
	{
		vector<iPoint> tempCheckpoint;
		for (int j = 58; j < 81; j++)
		{
			tempCheckpoint.push_back({ i, j });
		}

		checkpointContainer3.push_back(tempCheckpoint);
	}

	for (int i = 35; i < 51; i += 5)
	{
		vector<iPoint> tempCheckpoint;
		for (int j = 228; j < 268; j++)
		{
			tempCheckpoint.push_back({ j, i });
		}

		checkpointContainer4.push_back(tempCheckpoint);
	}

	for (int i = 75; i < 96; i += 5)
	{
		vector<iPoint> tempCheckpoint;
		for (int j = 22; j < 44; j++)
		{
			tempCheckpoint.push_back({ i, j });
		}

		checkpointContainer5.push_back(tempCheckpoint);
	}

	for (int i = 90; i < 106; i += 5)
	{
		vector<iPoint> tempCheckpoint;
		for (int j = 26; j < 66; j++)
		{
			tempCheckpoint.push_back({ j, i });
		}

		checkpointContainer6.push_back(tempCheckpoint);
	}

	for (int i = 220; i < 241; i += 5)
	{
		vector<iPoint> tempCheckpoint;
		for (int j = 100; j < 130; j++)
		{
			tempCheckpoint.push_back({ i, j });
		}

		checkpointContainer7.push_back(tempCheckpoint);
	}

	for (int i = 150; i < 161; i += 5)
	{
		vector<iPoint> tempCheckpoint;
		for (int j = 220; j < 253; j++)
		{
			tempCheckpoint.push_back({ j, i });
		}

		checkpointContainer8.push_back(tempCheckpoint);
	}
}

ModuleIA::~ModuleIA()
{
	for each (ModulePlayer* car in cars)
	{
		delete car;
	}
}

// Load assets
bool ModuleIA::Start()
{
	LOG("Loading IA");

	graphics = App->textures->Load("general_sprites.png");

	return true;
}

// Unload assets
bool ModuleIA::CleanUp()
{
	LOG("Unloading IA");

	App->textures->Unload(graphics);

	return true;
}

// PreUpdate
update_status ModuleIA::PreUpdate()
{
	for each (ModulePlayer* car in cars)
	{
		DetectCheckpoints(car);
		OnCheckpointExit(car);

		if (car->acceleration == car->accelerationCondition)
		{
			car->repeater++;
			car->SetDirection();
			car->MoveCar();

			car->acceleration = car->initialAcceleration;
			if (car->accelerationCondition > 1 && car->repeater % 2 == 0)
			{
				car->accelerationCondition--;
			}
		}

		car->acceleration++;

		if (car->still)
		{
			App->renderer->Blit(graphics, car->position.x, car->position.y + car->shadowsOffset, &car->currentShadowsAnimation->GetCurrentStaticFrame());
			App->renderer->Blit(graphics, car->position.x, car->position.y, &car->currentAnimation->GetCurrentStaticFrame());
		}
		else
		{
			if (car->right)
			{
				App->renderer->Blit(graphics, car->position.x, car->position.y + car->shadowsOffset, &car->currentShadowsAnimation->GetCurrentFrame());
				App->renderer->Blit(graphics, car->position.x, car->position.y, &car->currentAnimation->GetCurrentFrame());
			}
			else
			{
				App->renderer->Blit(graphics, car->position.x, car->position.y + car->shadowsOffset, &car->currentShadowsAnimation->GetCurrentInverseFrame());
				App->renderer->Blit(graphics, car->position.x, car->position.y, &car->currentAnimation->GetCurrentInverseFrame());
			}
		}
	}

	return UPDATE_CONTINUE;
}

int ModuleIA::GetRandomCheckpoint(int lot)
{
	return rand() % lot + 0;
}

void ModuleIA::DetectCheckpoints(ModulePlayer* car)
{
	for each (iPoint checkpoint in checkpointContainer1[GetRandomCheckpoint(checkpoint1Lot)])
	{
		if (car->position.DistanceTo(checkpoint) <= distanceOffset)
		{
			car->right = true;
			car->still = false;
			car->checkpoint1 = true;
		}
	}

	for each (iPoint checkpoint in checkpointContainer2[GetRandomCheckpoint(checkpoint2Lot)])
	{
		if (car->position.DistanceTo(checkpoint) <= distanceOffset)
		{
			car->right = true;
			car->still = false;
			car->checkpoint2 = true;
		}
	}

	for each (iPoint checkpoint in checkpointContainer3[GetRandomCheckpoint(checkpoint3Lot)])
	{
		if (car->position.DistanceTo(checkpoint) <= distanceOffset)
		{
			car->right = false;
			car->still = false;
			car->checkpoint3 = true;
		}
	}

	for each (iPoint checkpoint in checkpointContainer4[GetRandomCheckpoint(checkpoint4Lot)])
	{
		if (car->position.DistanceTo(checkpoint) <= distanceOffset)
		{
			car->right = false;
			car->still = false;
			car->checkpoint4 = true;
		}
	}

	for each (iPoint checkpoint in checkpointContainer5[GetRandomCheckpoint(checkpoint5Lot)])
	{
		if (car->position.DistanceTo(checkpoint) <= distanceOffset)
		{
			car->right = false;
			car->still = false;
			car->checkpoint5 = true;
		}
	}

	for each (iPoint checkpoint in checkpointContainer6[GetRandomCheckpoint(checkpoint6Lot)])
	{
		if (car->position.DistanceTo(checkpoint) <= distanceOffset)
		{
			car->right = false;
			car->still = false;
			car->checkpoint6 = true;
		}
	}

	for each (iPoint checkpoint in checkpointContainer7[GetRandomCheckpoint(checkpoint7Lot)])
	{
		if (car->position.DistanceTo(checkpoint) <= distanceOffset)
		{
			car->right = true;
			car->still = false;
			car->checkpoint7 = true;
		}
	}

	for each (iPoint checkpoint in checkpointContainer8[GetRandomCheckpoint(checkpoint8Lot)])
	{
		if (car->position.DistanceTo(checkpoint) <= distanceOffset)
		{
			car->right = true;
			car->still = false;
			car->checkpoint8 = true;
		}
	}
}

void ModuleIA::OnCheckpointExit(ModulePlayer* car)
{
	if (car->checkpoint1)
	{
		if (&car->currentAnimation->GetCurrentStaticFrame() == &car->turn.frames[25])
		{
			car->still = true;
			car->checkpoint1 = false;
		}
	}
	else if (car->checkpoint2)
	{
		if (&car->currentAnimation->GetCurrentStaticFrame() == &car->turn.frames[0])
		{
			car->still = true;
			car->checkpoint2 = false;
		}
	}
	else if (car->checkpoint3)
	{
		if (&car->currentAnimation->GetCurrentStaticFrame() == &car->turn.frames[23])
		{
			car->still = true;
			car->checkpoint3 = false;
		}
	}
	else if (car->checkpoint4)
	{
		if (&car->currentAnimation->GetCurrentStaticFrame() == &car->turn.frames[17])
		{
			car->still = true;
			car->checkpoint4 = false;
		}
	}
	else if (car->checkpoint5)
	{
		if (&car->currentAnimation->GetCurrentStaticFrame() == &car->turn.frames[8])
		{
			car->still = true;
			car->checkpoint5 = false;
		}
	}
	else if (car->checkpoint6)
	{
		if (&car->currentAnimation->GetCurrentStaticFrame() == &car->turn.frames[0])
		{
			car->still = true;
			car->checkpoint6 = false;
		}
	}
	else if (car->checkpoint7)
	{
		if (&car->currentAnimation->GetCurrentStaticFrame() == &car->turn.frames[8])
		{
			car->still = true;
			car->checkpoint7 = false;
		}
	}
	else if (car->checkpoint8)
	{
		if (&car->currentAnimation->GetCurrentStaticFrame() == &car->turn.frames[17])
		{
			car->still = true;
			car->checkpoint8 = false;
		}
	}
}