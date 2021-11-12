#pragma once

struct GlowStruct {
	float red, green, blue, alpha;

	uint8_t buffer1[8];
	float unknown = 1.f;
	uint8_t buffer2[4];
	BYTE renderOccluded = true;
	BYTE renderUnoccluded = false;
	BYTE fullBloom = false;

	void setup(char c) {
		switch (c) {
		case 'r':
			this->red = 1.f;
			this->green = 0.f;
			this->blue = 0.f;
			break;

		case 'g':
			this->red = 0.f;
			this->green = 1.f;
			this->blue = 0.f;
			break;

		case 'b':
			this->red = 0.f;
			this->green = 0.f;
			this->blue = 1.f;
			break;

		case 'y':
			this->red = 1.f;
			this->green = 1.f;
			this->blue = 0.f;
			break;

		case 'p':
			this->red = 1.f;
			this->green = 0.f;
			this->blue = 1.f;
			break;

		case 'l':
			this->red = 0.f;
			this->green = 1.f;
			this->blue = 1.f;
			break;
		}
		setAlpha(0.6f);
	}

	void setAlpha(float alpha) {
		this->alpha = alpha;
	}

}glowStruct;

struct DefaultChamStruct {
	BYTE r, g, b;

	void setup(BYTE r, BYTE g, BYTE b) {
		this->r = r;
		this->g = g;
		this->b = b;
	}

}defChamStruct;