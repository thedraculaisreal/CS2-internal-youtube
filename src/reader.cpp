#include "reader.h"

void Reader::init()
{
	client = (uintptr_t)GetModuleHandle("client.dll");

	update();
}

void Reader::update()
{
	while (true)
	{
		Sleep(5);
		std::vector<Entity*> list;

		local_player_view = (uintptr_t)(client + 0x1A5D610);
		uintptr_t local_player_addr = *(uintptr_t*)(client + 0x1861FA0);
		local_player = (Entity*)(local_player_addr);

		for (int i = 1; i < 16; ++i)
		{
			Entity* entity = new Entity;

			uintptr_t player_pawn = *(uintptr_t*)(client + 0x1861FA0 + (i * 0x10));
			if (!player_pawn) continue;

			entity = (Entity*)(player_pawn);
			if (!entity) continue;

			if (entity->health <= 0 || entity->health > 100) continue;

			list.push_back(entity);
		}

		entity_list.assign(list.begin(), list.end());
	}
}