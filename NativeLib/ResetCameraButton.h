#pragma once
#include "CommonLib.h"
#include <TextureButton.hpp>
#include "CameraController.h"

class ResetCameraButton :
    public TextureButton
{
	GODOT_CLASS(ResetCameraButton, TextureButton);

public:
	static void _register_methods();
	void _init();

	void _on_ResetCameraButton_pressed();
};
