#pragma once
#include "../Include/Quregister.h"


static complex_t UxArr[4][4][4] = {
	{
		{ -1, 0, 0, 0 },
		{ 0,  1, 0, 0 },
		{ 0,  0, 1, 0 },
		{ 0,  0, 0, 1 }
	},

	{
		{ 1, 0,  0, 0 },
		{ 0, -1, 0, 0 },
		{ 0, 0,  1, 0 },
		{ 0, 0,  0, 1 }
	},

	{
		{ 1, 0, 0,  0 },
		{ 0, 1, 0,  0 },
		{ 0, 0, -1, 0 },
		{ 0, 0, 0,  1 }
	},

	{
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, -1 }
	},
};

Matrix2& U(int num);

int findNum(Matrix2& Ux);