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
	j = json{ {"gravity", g.gravity}, {"walkSpeed", g.walkSpeed}, {"sprintSpeed", g.sprintSpeed}, {"jumpSpeed", g.jumpSpeed} };
}
void from_json(const json& j, Gameplay& g)
{
	j.at("gravity").get_to(g.gravity);
	j.at("walkSpeed").get_to(g.walkSpeed);
	j.at("sprintSpeed").get_to(g.sprintSpeed);
	j.at("jumpSpeed").get_to(g.jumpSpeed);
}

void to_json(json& j, const Keybinds& k)
{
	j = json{ {"forward", k.forward}, {"backward", k.backward}, {"left", k.left}, {"right", k.right}, {"sprint", k.sprint}, {"jump", k.jump}, {"use", k.use}, {"escape", k.escape}, {"enter", k.enter} };
}
void from_json(const json& j, Keybinds& k)
{
	j.at("forward").get_to(k.forward);
	j.at("backward").get_to(k.backward);
	j.at("left").get_to(k.left);
	j.at("right").get_to(k.right);
	j.at("sprint").get_to(k.sprint);
	j.at("jump").get_to(k.jump);
	j.at("use").get_to(k.use);
	j.at("escape").get_to(k.escape);
	j.at("enter").get_to(k.enter);
}