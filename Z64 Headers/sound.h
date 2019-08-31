#pragma once

typedef enum EnvironmentSound {
	SFX_ENV_NIGHT,
	SFX_ENV_VILLAGE_NIGHT,
	SFX_ENV_QUIET_STORM,
	SFX_ENV_HEAVY_WIND_CROWS,
	SFX_ENV_WIND_BIRDS,
	SFX_ENV_WIND_CRICKETS,
	SFX_ENV_WIND,
	SFX_ENV_HEAVY_WIND,
	SFX_ENV_WIND_CRICKETS2,
	SFX_ENV_WIND_CRICKETS3,
	SFX_ENV_HEAVY_WIND2,
	SFX_ENV_HEAVY_WIND3,
	SFX_ENV_WIND2,
	SFX_ENV_USE_MUSIC = 19,
	SFX_ENV_RAIN = 28
} EnvironmentSound;

extern void PlayActorSfx();
extern void PlayActorSfx2();
extern void PlayRangedPositionalSfx();
extern void PlayPositionalSfx();
extern void PlaySoundEffect();
