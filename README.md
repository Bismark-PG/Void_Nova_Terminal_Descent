# 🚀 Void Nova : Terminal Descent

- **A 2D bullet-hell shooter created with DirectX 11, inspired by the classic arcade shoot 'em ups.**

This project was developed as a school assignment and serves as a spiritual successor to a previous Unity project, 'Void Nova'.\
Please keep in mind that this is a student project, and my skills are still developing. Any feedback is welcome!

## 🔑 Key Features

- **Five Unique Stages**
  + Each stage features different enemy types and a unique boss battle.

- **Dynamic Storytelling**
  + The main story unfolds based on the selected game mode, with cutscenes for major events like boss encounters.

- **Diverse Enemy Roster**
  + Face a variety of enemies with distinct patterns and challenging bosses with spectacular bullet-hell attacks.

## 🛠️ Development Environment

* **Language** : C/C++
* **Library** : DirectX 11
* **Tools Used** : Visual Studio, Adobe Photoshop
* **Development start date** : 2025.08.01

## 🎮 How To Play

***(Only code is being distributed because the currently used sound source resources are not redistributable.)***

---

## ✨ Patch And Update History
### v1.0 (2025.09.09)
- Initial Release!
   + Early access version launched. While some bugs remain, Stages 1 through 4 are fully playable.
 
### v1.1.0 (2025.10.26)
- Alpha version Release!
   + Fixing bugs in stage 5 and improving the ending sequence.

## 📝 Roadmap & Dev Notes
- **This section outlines known issues and future plans for the project.**

---

> ### ⁉️ Known Issues
> 	**As of v1.1.0, there are no fatal bugs, but some minor bugs remain.**
>   + **Story Mode Cutscene Bug**: Story cutscenes may fail to trigger in Story Mode if Stage Mode has been played previously.
>   + **Stage 2 Spawn Timing**: Enemy spawn timings in Stage 2 are misaligned due to a new spawn logic.
>
> ### 🗃️ Planned Improvements
>   + **Preparing for EXE Distribution: Researching file encryption methods for non-redistributable resources to allow for distribution.**
>   + **Audio Balancing** : Adjust the volume for item acquisition sound effects.
>   + **Difficulty Tuning** : Rebalance the overall game difficulty.
>   + **Pattern Refinement** : Rework and improve existing boss and enemy patterns.
>
> ### 📬 Received Feedback
>  + ~~***(I`m Waiting!)***~~
>
> ### ✔️ Completed
>  + ~~***Stage 5 Progression Bug : A critical bug prevents progress in Stage 5.***~~
>  + ~~**Ending Sequence** : The ending cutscene requires further debugging.~~

---

# ©️ Credits

## ■ Texture ■

### ◆ [ChatGPT](https://chatgpt.com/)
- Explosion Sprite
- BackGround Texture
- Enemy Texture
- Bullet Texture
- Ending Ally Base Texture
- UI Texture

### ◆ [Freepik](https://www.freepik.com/)
- Player Texture
- Story Commander Texture
- Character Graphic (for Bomb Effect Cutscene)

### ◆ [OpenGameArt](https://opengameart.org/)
- Explosion Sprite
  
### ◆ [Xbox One Controller Textures](https://forums.dolphin-emu.org/Thread-xbox-one-controller-textures?highlight=Xbox+One+Controller+Textures)
- Controller Button Textures

### ◆ Photoshop
- Endind Text Script
- Textures Editing
- Player Bomb Effect

## ■ BGM / SFX ■

### ◆ [DOVA-SYNDROME](https://dova-s.jp/)
- BGM ***(Producers List Editing...)***

### ◆ [効果音ラボ (Sound Effect Lab)](https://soundeffect-lab.info/)
- SFX ***(Producers List Editing...)***

## ■ Editor Tools Site ■

### ◆ [123APPS](https://123apps.com/)
- Convert MP3 to WAV
- Edit Volume

## ■ Reference Games ■

- [Galaga](https://en.wikipedia.org/wiki/Galaga) 
- [1942](https://en.wikipedia.org/wiki/1942_(video_game))
- [Strikers 1945](https://en.wikipedia.org/wiki/Strikers_1945)
- [Touhou Project](https://en.wikipedia.org/wiki/Touhou_Project)
- [Under Defeat](https://en.wikipedia.org/wiki/Under_Defeat)

---

## 💻 Code Snippet

```cpp
// Example Code in Main Menu Select Page

void Game_Select_Update()
{
	switch (Game_Select_Buffer)
	{
	case SELECT_GAME::NONE:
		if (GM.Get_Current_Main_Screen() == Main_Screen::SELECT_GAME)
			Update_Game_Select_Buffer(SELECT_GAME::INTRO_WAIT);
		break;

	case SELECT_GAME::INTRO_WAIT:
	case SELECT_GAME::BACK_WAIT:
		break;

	case SELECT_GAME::SELECT_WAIT:
		if (KeyLogger_IsTrigger(KK_W) || KeyLogger_IsTrigger(KK_UP) ||
			XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_UP))
		{
			Update_Game_Select_Buffer(SELECT_GAME::SELECT_BACK);
			SM->Play_SFX("Buffer_Move");
		}
		else if (KeyLogger_IsTrigger(KK_S) || KeyLogger_IsTrigger(KK_DOWN) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_DOWN))
		{
			Update_Game_Select_Buffer(SELECT_GAME::SELECT_NEW_GAME);
			SM->Play_SFX("Buffer_Move");
		}
		break;

	case SELECT_GAME::SELECT_NEW_GAME:
		if (KeyLogger_IsTrigger(KK_ENTER) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_A))
		{
			Set_Game_Mode(GameMode::NEW_GAME);
			Update_Game_Select_Buffer(SELECT_GAME::FADING_TO_STAGE);
			Update_Select_Stage_State(SELECT_STAGE_STATE::STAGE_SELECT);

			Update_Stage_Select_Buffer_State(STAGE_NUM::STAGE_1);

			SM->Play_SFX("Buffer_Select");
			SM->Stop_BGM("Title");
		}

		if (KeyLogger_IsTrigger(KK_S) || KeyLogger_IsTrigger(KK_DOWN) ||
			XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_DOWN))
		{
			Update_Game_Select_Buffer(SELECT_GAME::SELECT_CONTINUE);
			SM->Play_SFX("Buffer_Move");
		}
		break;

	case SELECT_GAME::SELECT_CONTINUE:
    // Omission
}
```
