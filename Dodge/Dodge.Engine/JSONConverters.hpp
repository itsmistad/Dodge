#pragma once
#include "Headers.h"
#include "Audio.h"
#include "Gameplay.h"
#include "Keybinds.h"
#include "Video.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

void to_json(json& j, const Audio& a)
{
	j = json{ {"masterVolume", a.masterVolume}, {"effectsVolume", a.effectsVolume}, {"musicVolume", a.musicVolume}, {"uiVolume", a.uiVolume} };
}
void from_json(const json& j, Audio& a)
{
	j.at("masterVolume").get_to(a.masterVolume);
	j.at("effectsVolume").get_to(a.effectsVolume);
	j.at("musicVolume").get_to(a.musicVolume);
	j.at("uiVolume").get_to(a.uiVolume);
}

void to_json(json& j, const Video& v)
{
	j = json{ {"windowWidth", v.windowWidth}, {"windowHeight", v.windowHeight}, {"frameCap", v.frameCap} };
}
void from_json(const json& j, Video& v)
{
	j.at("windowWidth").get_to(v.windowWidth);
	j.at("windowHeight").get_to(v.windowHeight);
	j.at("frameCap").get_to(v.frameCap);
}

void to_json(json& j, const Gameplay& g)
{
	j = json{ {"gravity", g.gravity}, {"walkSpeed", g.walkSpeed}, {"sprintSpeed", g.sprintSpeed}, {"jumpSpeed", g.jumpSpeed}, {"mouseSensitivity", g.mouseSensitivity}, {"mouseSmoothing", g.mouseSmoothing} };
}
void from_json(const json& j, Gameplay& g)
{
	j.at("gravity").get_to(g.gravity);
	j.at("walkSpeed").get_to(g.walkSpeed);
	j.at("sprintSpeed").get_to(g.sprintSpeed);
	j.at("jumpSpeed").get_to(g.jumpSpeed);
	j.at("mouseSensitivity").get_to(g.mouseSensitivity);
	j.at("mouseSmoothing").get_to(g.mouseSmoothing);
}

void to_json(json& j, const Keybinds& k)
{
	j = json{ {"forward", k.forward->key}, {"backward", k.backward->key}, {"left", k.left->key}, {"right", k.right->key}, {"sprint", k.sprint->key}, {"jump", k.jump->key}, {"use", k.use->key}, {"escape", k.escape->key}, {"enter", k.enter->key} };
}
void from_json(const json& j, Keybinds& k)
{
	int key = 0;
	j.at("forward").get_to(key);
	k.forward->SetKey(key);
	j.at("backward").get_to(key);
	k.backward->SetKey(key);
	j.at("left").get_to(key);
	k.left->SetKey(key);
	j.at("right").get_to(key);
	k.right->SetKey(key);
	j.at("sprint").get_to(key);
	k.sprint->SetKey(key);
	j.at("jump").get_to(key);
	k.jump->SetKey(key);
	j.at("use").get_to(key);
	k.use->SetKey(key);
	j.at("escape").get_to(key);
	k.escape->SetKey(key);
	j.at("enter").get_to(key);
	k.enter->SetKey(key);
}