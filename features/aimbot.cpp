#include "aimbot.h"
#include "../src/reader.h"
#include <algorithm>

namespace Aimbot
{
	Vector3 get_closest_entity()
	{
		Vec3 closest_target = { 0, 0 , 0 };
		float closest_angle = 999999.0f;

		for (const auto& entity : read.entity_list)
		{
			Vec3 target_angle = CalcAngle(read.local_player->head, entity->head); // angle to enemy;

			if (!is_in_fov(target_angle)) continue;

			Vec3 delta_angle = target_angle - read.local_player->view_angles; // new angle - our angle;

			if (delta_angle.y > 180.0f)
			{
				delta_angle.y -= 360.0f;
			}
			if (delta_angle.y < -180.0f)
			{
				delta_angle.y += 360.0f;
			}

			float pitch_diff = target_angle.y - read.local_player->view_angles.y; // pitch, yaw, roll;

			float yaw_diff = min(abs(read.local_player->view_angles.x - target_angle.x), abs(target_angle.x - read.local_player->view_angles.x));

			float angle_magnitude = sqrt(pow(yaw_diff, 2) + pow(pitch_diff, 2)); // multiply by self so not negativo.

			if (angle_magnitude < closest_angle)
			{
				closest_angle = angle_magnitude;
				closest_target = read.local_player->view_angles + (delta_angle * 0.1); // .1 is moothing factor.
			}
		}

		return closest_target;
	}
	void do_aimbot()
	{
		while (true)
		{
			Sleep(5);
			Vector3 closest_target = get_closest_entity();

			if (closest_target.x == 0 || closest_target.y == 0) continue;

			if (!GetAsyncKeyState(VK_LBUTTON)) continue;

			*(Vec2*)(read.local_player_view) = closest_target.to_vec2();
		}
	}
	bool is_in_fov(Vector3 target_angle)
	{
		float difference_pitch = abs(target_angle.x) - abs(read.local_player->view_angles.x);
		float difference_yaw = abs(target_angle.y) - abs(read.local_player->view_angles.y);
		float fov = 20.0f;

		if (difference_pitch > fov || difference_pitch < -fov || difference_yaw > fov || difference_yaw < -fov)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}

