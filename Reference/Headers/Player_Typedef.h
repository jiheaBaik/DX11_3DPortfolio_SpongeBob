#pragma once
class CPlayer_Typedef
{
public:
	enum PLAYER_KIND { PLAYER_SPONGEBOB, PLAYER_PATRICK, PLAYER_END };

	enum SPONGEBOB_ACTION {
		SPONGEBOB_IDLE, SPONGEBOB_WALK, SPONGEBOB_JUMPUP, SPONGEBOB_RUN, SPONGEBOB_DEATH, SPONGEBOB_ATTACK, SPONGEBOB_JUMPDW, SPONGEBOB_JUMP_LANDING_EYE,
		SPONGEBOB_BUBLE_START, SPONGEBOB_BUBLE_LANDING, SPONGEBOB_BUBLE_LOOP, SPONGEBOB_BUBLE_SPINE, SPONGEBOB_BASH_LOOP,
		SPONGEBOB_POSEIDOME_INTRO_0, SPONGEBOB_POSEIDOME_INTRO_1, SPONGEBOB_POSEIDOME_INTRO_2, SPONGEBOB_POSEIDOME_INTRO_3, SPONGEBOB_POSEIDOME_INTRO_4,
		SPONGEBOB_POSEIDOME_OUTRO0, SPONGEBOB_POSEIDOME_OUTRO1, SPONGEBOB_POSEIDOME_OUTRO2,
		SPONGEBOB_POSEIDOME_ROUND2_0, SPONGEBOB_POSEIDOME_ROUND2_1, SPONGEBOB_POSEIDOME_ROUND2_2, SPONGEBOB_POSEIDOME_ROUND2_3,
		SPONGEBOB_POSEIDOME_ROUND3_0, SPONGEBOB_POSEIDOME_ROUND3_1, SPONGEBOB_HIT, SPONGEBOB_ACTION_END
	};

	//Patrick
	enum PATRICK_ACTION {
		PATRICK_ATTACK, PATRICK_DEATH, PATRICK_HIT, PATRICK_IDLE, PATRICK_JUMP_DW, PATRICK_JUMP_UP, PATRICK_INTRO_2, PATRICK_INTRO_3, PATRICK_INTRO_7,
		PATRICK_OUTTRO_0, PATRICK_ROUND2_1, PATRICK_ROUND2_2, PATRICK_ROUND2_4, PATRICK_ROUND3_1, PATRICK_ROUND3_2,
		PATRICK_PICKUP, PATRICK_PICKUP_IDLE, PATRICK_PICKUP_JUMP_DW, PATRICK_PICKUP_JUMP_UP, PATRICK_PICKUP_WALK, PATRICK_PICKUP_RUN, PATRICK_SLAM_END, PATRICK_SLAM_LOOP, PATRICK_SLAM_START,
		PATRICK_TALK, PATRICK_THROW, PATRICK_VICTORY, PATRICK_WALK, PATRICK_WATCHING_DOME, PATRICK_PATRICK_ACTION_END
	};

};