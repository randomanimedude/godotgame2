#pragma once
#include "CommonLib.h"
#include <Node2D.hpp>
#include <Sprite.hpp>
#include "Piece.h"
#include "Bot.h"
#include <Label.hpp>

class Piece;
class Bot;

class FlyingMoney :
    public Node2D
{
    GODOT_CLASS(FlyingMoney, Node2D);

public:
	static void _register_methods();
	void _init();

	void _ready();
	void _physics_process(float delta);

	void SetCourse(Piece* from, Piece* to, int amount, PieceOwner owner, Bot* botOwner);

private:
	Sprite* sprite;

	Vector2 fromPos;
	Vector2 toPos;

	Piece* toPiece;

	Bot* botOwner;

	float speed = 0.01;
	float progress = 0;

	PieceOwner owner;

	int money;

	bool proceed = false;
};
