#include "worldGenerator.h"
#include "randomStuff.h"
#include <FastNoiseSIMD.h>


void generateWorld(GameMap& gameMap, int seed)
{
	const int w = 900;
	const int h = 500;

	gameMap.create(w, h);


	std::ranlux24_base rng(seed++);

	int desertStart = getRandomInt(rng, 10, w - 210);
	int desertEnd = desertStart + 100 + getRandomInt(rng, 0, 100);
	if (desertEnd > w) { desertEnd = w;  }

	std::unique_ptr<FastNoiseSIMD> dirtNoiseGenrator(FastNoiseSIMD::NewFastNoiseSIMD());
	std::unique_ptr<FastNoiseSIMD> cavesNoiseGenrator(FastNoiseSIMD::NewFastNoiseSIMD());

	dirtNoiseGenrator->SetSeed(seed++);
	cavesNoiseGenrator->SetSeed(seed++);

	dirtNoiseGenrator->SetFractalType(FastNoiseSIMD::FractalType::FBM);
	dirtNoiseGenrator->SetFractalOctaves(6);
	dirtNoiseGenrator->SetFractalGain(0.4f); // lower gain = sharper
	dirtNoiseGenrator->SetFrequency(0.01f);

	cavesNoiseGenrator->SetFractalType(FastNoiseSIMD::FractalType::FBM);
	cavesNoiseGenrator->SetFractalOctaves(3);
	cavesNoiseGenrator->SetFractalGain(0.2f);


	float* dirtNoise = FastNoiseSIMD::GetEmptySet(w);

	dirtNoiseGenrator->FillNoiseSet(dirtNoise, 0, 0, 0, w, 1, 1);

	//convert from [-1 1] to [0 1]

	for (int i = 0; i < w; i++)
	{
		dirtNoise[i] = (dirtNoise[i] + 1) / 2;
	}

	float* cavesNoise = FastNoiseSIMD::GetEmptySet(w * h);
	cavesNoiseGenrator->FillNoiseSet(cavesNoise, 0, 0, 0, h, w, 1); // Make sure you flip w and h

	for (int i = 0; i < w * h; i++)
	{
		cavesNoise[i] = (cavesNoise[i] + 1) / 2; 
	}

	auto getCaveNoise = [&](int x, int y)
		{
			return cavesNoise[x + y * w];
		};

	int dirtOffsetStart = -5;
	int dirtOffsetEnd = 35;


	int keepDirectionTimeStone = getRandomInt(rng, 5, 40);
	int directionStone = getRandomInt(rng, -2, 2);


	int stoneHeight = 90;

	for (int x = 0; x < w; x++)
	{

		bool inDesert = (x >= desertStart && x <= desertEnd);

#pragma region stone height


		keepDirectionTimeStone--;
		if (keepDirectionTimeStone <= 0)
		{
			keepDirectionTimeStone = getRandomInt(rng, 5, 40);
			directionStone = getRandomInt(rng, -2, 2);
		}

		if (directionStone == -1)
		{
			if (getRandomChance(rng, 0.25))
			{
				stoneHeight--;
			}
		}
		else if (directionStone == -2)
		{
			if (getRandomChance(rng, 0.25))
			{
				stoneHeight--;
			}

			if (getRandomChance(rng, 0.25))
			{
				stoneHeight--;
			}
		}
		else if (directionStone == 1)
		{
			if (getRandomChance(rng, 0.25))
			{
				stoneHeight++;
			}
		}
		else if (directionStone == 2)
		{
			if (getRandomChance(rng, 0.25))
			{
				stoneHeight++;
			}

			if (getRandomChance(rng, 0.25))
			{
				stoneHeight++;
			}
		}

		if (stoneHeight < 60)
		{
			stoneHeight = 60;
		}

		if (stoneHeight > 120)
		{
			stoneHeight = 120;
		}

#pragma endregion


		int dirtHeight = dirtOffsetStart + (dirtOffsetEnd - dirtOffsetStart) * dirtNoise[x];
		dirtHeight = stoneHeight - dirtHeight;

		int dirtType = Block::dirt; 
		int grassType = Block::grassBlock;
		int stoneType = Block::stone;

		if (inDesert)
		{
			dirtType = Block::sand;
			grassType = Block::sand;
			stoneType = Block::sandStone;
		}

		for (int y = 0; y < h; y++)
		{
			Block b;

			if (y > dirtHeight)
			{
				b.type = dirtType;
			}

			if (y == dirtHeight)
			{
				b.type = grassType;
			}

			if (y >= stoneHeight)
			{
				b.type = stoneType;
			}

			if (inDesert)
			{
				int desertMid = (desertStart + desertEnd) / 2; 
				int desertHalfWidth = (desertEnd - desertStart) / 2; 
				int distanceFromDesertMid = std::abs(x - desertMid);

				// This give a value from 0 at edge to 1 at center
				float desertDistance = 1 - distanceFromDesertMid / float(desertHalfWidth);

				int desertStoneStart = 10 + stoneHeight;
				int desertStoneDepth = 20 + stoneHeight; // How deep the triangle goes

				int traingleStoneY = desertStoneStart + desertDistance * desertStoneDepth;

				// Apple stone if below the triangle 
				if (y > traingleStoneY)
				{
					b.type = Block::stone;
				}
			}


			//Bigger more interesting caves
			//getCaveNoise(x, y) < 0.80 && getCaveNoise(x, y) > 0.60

			if (getCaveNoise(x, y) < 0.30)
			{
				b.type = Block::air;
			}

			gameMap.getBlocUnsafe(x, y) = b;


		}



	}




	FastNoiseSIMD::FreeNoiseSet(dirtNoise);
	FastNoiseSIMD::FreeNoiseSet(cavesNoise);

}
