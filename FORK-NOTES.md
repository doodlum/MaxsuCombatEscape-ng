# Better Combat Escape - NG (vtable fix fork)

Fork of [doodlum/MaxsuCombatEscape-ng](https://github.com/doodlum/MaxsuCombatEscape-ng) (based on commit `7bcdd61`), itself a port of Maxsu's Better Combat Escape.
Licence: GPL-3.0-or-later with the modding exception, unchanged from upstream (`COPYING`, `EXCEPTIONS.md`). The DLL links alandtse's CommonLibSSE-NG (GPL-3.0 with a modding exception).

## What this fork changes

**The crash it fixes.** On Skyrim 1.7.x (and SE/AE) the upstream DLL crashed at `SkyrimSE.exe+04D76C8` during combat when the player crossed a cell border (`MaxsuCombatEscape.dll+0x2AA5`, `CombatEsc_Events.cpp:80`).

**Cause.** `include/PCH.h` had `#undef ENABLE_SKYRIM_VR` while the CommonLibSSE-NG it links is built with SE + AE + VR. With VR undefined the macro `SKYRIM_REL_VR_VIRTUAL` expands to a plain `virtual`, so the compiler fixed the vtable slots of `RE::Actor::IsInCombat()` and `StopCombat()` at `0xE4` and `0xE6` (the Skyrim VR layout). On Skyrim SE/AE the real slots are `0xE3` and `0xE5`; slot `0xE4` is `Actor::UpdateCombat()`. The plugin therefore ran a full combat update on the player and on every enemy in the HUD list inside the cell-change event, which crashed in combat pathing.

**Fix.** The `#undef` is removed (`include/PCH.h`). `ng.cmake` builds the plugin through `ng-build` (CommonLibSSE-NG with VR off), where the calls compile to `0xE3` / `0xE5`. Verified by disassembling the built DLL: virtual calls at `+0x718` (IsInCombat, 2 sites) and `+0x728` (StopCombat, 2 sites); the real slots were read from the Character vtable of a 1.7.104 `SkyrimSE.exe`.
`src/XSEPlugin.cpp` uses `SKSEPluginLoad` and the generated plugin info under `MCE_NG`; the plugin now reports its name as `MaxsuCombatEscape` instead of the placeholder `PluginName`.

Nothing else in the plugin's behaviour is changed. Known weaknesses left for a separate change: `currentProcess` is not null-checked in `ShouldExteriorEnemyStopCombat`, the HUD actor array is iterated while `StopCombat()` may modify it, and `std::stof` on a malformed ini value is not guarded.

## Build

`E:\WorkSpace\ng-build\build.cmd MaxsuCombatEscape` -> `build\release\out\MaxsuCombatEscape\MaxsuCombatEscape.dll` (Skyrim SE / AE, including 1.7.x; no VR build in this fork yet).
