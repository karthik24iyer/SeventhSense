#pragma once

#include <iostream>

class Polymorph {
	public:
		void polymorphic() {
			srand((unsigned int)time(0));
			int count = 0;
			for (count = 0;count < 10;count++) {
				int index = rand() % (6 - 0 + 1) + 0;
				switch (index) {

				case 0:
					__asm __volatile{
						sub eax, 3
						add eax, 1
						add eax, 2
					}

				case 1:
					__asm __volatile{
						push eax
						pop eax
					}

				case 2:
					__asm __volatile{
						inc eax
						dec eax
					}

				case 3:
					__asm __volatile{
						dec eax
						add eax, 1
					}

				case 4:
					__asm __volatile{
						pop eax
						push eax
					}

				case 5:
					__asm __volatile{
						mov eax, eax
						sub eax, 1
						add eax, 1
					}

				case 6:
					__asm __volatile{
						xor eax, eax
						mov eax, eax
					}
				}
			}
		}
};