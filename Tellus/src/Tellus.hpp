#pragma once
#include "Application.hpp"
#include "Layers/Layer.hpp"

#include "Input/KeyCodes.hpp"
#include "Input/MouseCodes.hpp"
#include "Input/Input.hpp"
#include <FastNoise/FastNoise.h>

#include "Renderer/Renderer.hpp"

#include "Renderer/Buffer.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/Texture.hpp"

#include "Renderer/Camera.hpp"

#ifdef TS_MAIN
#    include "Entry.hpp"
#    undef TS_MAIN
#endif