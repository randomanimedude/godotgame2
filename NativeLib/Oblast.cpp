#include "Oblast.h"

void Oblast::_register_methods()
{
	register_method("_input_event", &Oblast::_input_event);
	register_method("_ready", &Oblast::_ready);
	register_method("_physics_process", &Oblast::_physics_process);
}

void Oblast::_init()
{
}

void Oblast::_ready()
{
	gameManager = GameManager::GetSingleton();
	mainSprite = cast_to<MeshInstance2D>(get_node("Sprite"));
	//border = cast_to<Sprite>(get_node("Border"));
	collisionShape = cast_to<CollisionPolygon2D>(get_node("CollisionPolygon2D"));
	//piecesCombined = cast_to<Node2D>(get_node("Pieces"));

	currentColor = get_self_modulate();
}

void Oblast::_physics_process()
{
	switch (state)
	{
	case Hiding:
		currentColor = Color(currentColor.r, currentColor.g, currentColor.b, lerp(currentColor.a, 0, transition_t));
		if (currentColor.a < 0.01)
		{
			currentColor=Color(1, 1, 1, 0);
			state = Hidden;
			mainSprite->set_visible(false);
		}
		break;
	case Appearing:
		currentColor = Color(currentColor.r, currentColor.g, currentColor.b, lerp(currentColor.a, 1, transition_t));
		if (currentColor.a > 0.99)
		{
			currentColor=Color(1, 1, 1, 1);
			state = Visible;
		}
		break;
	case VisibleToBG:
		currentColor = lerp(currentColor, borderColor, transition_t);
		if ((currentColor.r - borderColor.r) * (currentColor.g - borderColor.g) * (currentColor.b - borderColor.b) < 0.01)
		{
			currentColor = borderColor;
			state = BG;
			gameManager->SetGameIsPlaying(true);
		}
		break;
	case BGToVisible:
		currentColor = lerp(currentColor, storedColor, transition_t);
		if ((currentColor.r - storedColor.r) * (currentColor.g - storedColor.g) * (currentColor.b - storedColor.b) < 0.01)
		{
			currentColor = storedColor;
			state = Visible;
			if (piecesCombined != nullptr)
				piecesCombined->set_visible(false);
			gameManager->SetGameIsPlaying(false);
			piecesCombined->queue_free();
			piecesCombined = nullptr;
		}
	}
	mainSprite->set_self_modulate(currentColor);
}

void Oblast::_input_event(Node* viewport, InputEventMouseButton* event, int shape_idx)
{
	if (event->is_pressed() && !get_tree()->is_input_handled() && !gameManager->IsGamePlaying())
	{
		//border->set_visible(!border->is_visible());
		//ChangeColorTo(blue, 0.1);
		//Hide();
		if(gameManager->GetSelectedOblast()==nullptr)
			gameManager->SelectOblast(this);

		get_tree()->set_input_as_handled();
	}
}

void Oblast::ChangeColorTo(Color color, float force)
{
	if ((colorChangeForce += force) > 1)
		colorChangeForce = 1;
	currentColor = def - (def - color) * colorChangeForce;
	mainSprite->set_self_modulate(currentColor);
}

Vector2 Oblast::GetSize()
{
	return mainSprite->get_texture()->get_size() * mainSprite->get_global_scale();
}

void Oblast::Hide()
{
	state = Hiding;
}

void Oblast::Show()
{
	mainSprite->set_visible(true);
	state = Appearing;
}

void Oblast::ShowPieces()
{
	Ref<PackedScene> temp = resourceLoader->load("res://Prefabs/Oblasti/" + get_name() + "/Pieces.tscn");
	piecesCombined = cast_to<Node2D>(temp->instance());
	storedColor = currentColor;
	state = VisibleToBG;
	if(piecesCombined!=nullptr)
		piecesCombined->set_visible(true);
	for (Piece* piece : pieces)
		piece->Show();
}

void Oblast::HidePieces()
{
	state = BGToVisible;
	for (Piece* piece : pieces)
		piece->Hide();
}

void Oblast::RegisterPiece(Piece* piece)
{
	pieces.push_back(piece);
}

void Oblast::UpdateSituation()
{
}

void Oblast::SelectPiece(Piece* piece)
{
	UnselectPiece();
	selectedPiece = piece;
	piece->set_scale(Vector2(1.012, 1.012));
}

void Oblast::UnselectPiece()
{
	if (selectedPiece != nullptr)
	{
		selectedPiece->set_scale(Vector2(1, 1));
		selectedPiece = nullptr;
	}
}

bool Oblast::IsPieceSelected(Piece* piece)
{
	return selectedPiece == piece;
}


