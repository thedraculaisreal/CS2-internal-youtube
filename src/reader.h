#pragma once
#include <Windows.h>
#include <iostream>
#include <cstdint>
#include <vector>
#include "../math/geom.h"

class Entity
{
public:
	char pad_0000[832]; //0x0000
	uint32_t max_health; //0x0340
	uint32_t health; //0x0344
	char pad_0348[152]; //0x0348
	int16_t N000000F8; //0x03E0
	int8_t N00000483; //0x03E2
	uint8_t team; //0x03E3
	char pad_03E4[2516]; //0x03E4
	Vector3 feet; //0x0DB8
	char pad_0DC4[1472]; //0x0DC4
	Vector3 head; //0x1384
	char pad_1390[168]; //0x1390
	Vector3 view_angles; //0x1438
};

class Reader
{
private:

	uintptr_t client;

	void update();

public:

	uintptr_t local_player_view;

	Entity* local_player;

	std::vector<Entity*> entity_list;

	void init();

};

inline Reader read;

