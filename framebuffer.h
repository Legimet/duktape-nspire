#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__ 1

// =================================================
// Xtase (Franck GALLIAT) code section
// @ 09/2015

		// TODO : cuttedCornerRect + circle (V)


const float __cos[] = {1.0F,0.9998477F,0.99939084F,0.9986295F,0.9975641F,0.9961947F,0.9945219F,0.99254614F,0.99026805F,0.98768836F,0.9848077F,0.98162717F,0.9781476F,0.97437006F,0.9702957F,0.9659258F,0.9612617F,0.9563047F,0.95105654F,0.94551855F,0.9396926F,0.9335804F,0.92718387F,0.92050487F,0.9135454F,0.9063078F,0.89879405F,0.8910065F,0.88294756F,0.8746197F,0.8660254F,0.8571673F,0.8480481F,0.83867055F,0.82903755F,0.81915206F,0.809017F,0.7986355F,0.7880108F,0.777146F,0.76604444F,0.7547096F,0.7431448F,0.7313537F,0.7193398F,0.70710677F,0.6946584F,0.6819984F,0.6691306F,0.656059F,0.64278764F,0.6293204F,0.6156615F,0.60181504F,0.58778524F,0.57357645F,0.5591929F,0.54463905F,0.52991927F,0.5150381F,0.5F,0.4848096F,0.46947157F,0.4539905F,0.43837115F,0.42261827F,0.40673664F,0.39073113F,0.37460658F,0.35836795F,0.34202015F,0.32556817F,0.309017F,0.2923717F,0.27563736F,0.25881904F,0.2419219F,0.22495106F,0.20791169F,0.190809F,0.17364818F,0.15643446F,0.1391731F,0.12186934F,0.104528464F,0.087155744F,0.06975647F,0.052335955F,0.034899496F,0.017452406F,6.123234E-17F,-0.017452406F,-0.034899496F,-0.052335955F,-0.06975647F,-0.087155744F,-0.104528464F,-0.12186934F,-0.1391731F,-0.15643446F,-0.17364818F,-0.190809F,-0.20791169F,-0.22495106F,-0.2419219F,-0.25881904F,-0.27563736F,-0.2923717F,-0.309017F,-0.32556817F,-0.34202015F,-0.35836795F,-0.37460658F,-0.39073113F,-0.40673664F,-0.42261827F,-0.43837115F,-0.4539905F,-0.46947157F,-0.4848096F,-0.5F,-0.5150381F,-0.52991927F,-0.54463905F,-0.5591929F,-0.57357645F,-0.58778524F,-0.60181504F,-0.6156615F,-0.6293204F,-0.64278764F,-0.656059F,-0.6691306F,-0.6819984F,-0.6946584F,-0.70710677F,-0.7193398F,-0.7313537F,-0.7431448F,-0.7547096F,-0.76604444F,-0.777146F,-0.7880108F,-0.7986355F,-0.809017F,-0.81915206F,-0.82903755F,-0.83867055F,-0.8480481F,-0.8571673F,-0.8660254F,-0.8746197F,-0.88294756F,-0.8910065F,-0.89879405F,-0.9063078F,-0.9135454F,-0.92050487F,-0.92718387F,-0.9335804F,-0.9396926F,-0.94551855F,-0.95105654F,-0.9563047F,-0.9612617F,-0.9659258F,-0.9702957F,-0.97437006F,-0.9781476F,-0.98162717F,-0.9848077F,-0.98768836F,-0.99026805F,-0.99254614F,-0.9945219F,-0.9961947F,-0.9975641F,-0.9986295F,-0.99939084F,-0.9998477F,-1.0F,-0.9998477F,-0.99939084F,-0.9986295F,-0.9975641F,-0.9961947F,-0.9945219F,-0.99254614F,-0.99026805F,-0.98768836F,-0.9848077F,-0.98162717F,-0.9781476F,-0.97437006F,-0.9702957F,-0.9659258F,-0.9612617F,-0.9563047F,-0.95105654F,-0.94551855F,-0.9396926F,-0.9335804F,-0.92718387F,-0.92050487F,-0.9135454F,-0.9063078F,-0.89879405F,-0.8910065F,-0.88294756F,-0.8746197F,-0.8660254F,-0.8571673F,-0.8480481F,-0.83867055F,-0.82903755F,-0.81915206F,-0.809017F,-0.7986355F,-0.7880108F,-0.777146F,-0.76604444F,-0.7547096F,-0.7431448F,-0.7313537F,-0.7193398F,-0.70710677F,-0.6946584F,-0.6819984F,-0.6691306F,-0.656059F,-0.64278764F,-0.6293204F,-0.6156615F,-0.60181504F,-0.58778524F,-0.57357645F,-0.5591929F,-0.54463905F,-0.52991927F,-0.5150381F,-0.5F,-0.4848096F,-0.46947157F,-0.4539905F,-0.43837115F,-0.42261827F,-0.40673664F,-0.39073113F,-0.37460658F,-0.35836795F,-0.34202015F,-0.32556817F,-0.309017F,-0.2923717F,-0.27563736F,-0.25881904F,-0.2419219F,-0.22495106F,-0.20791169F,-0.190809F,-0.17364818F,-0.15643446F,-0.1391731F,-0.12186934F,-0.104528464F,-0.087155744F,-0.06975647F,-0.052335955F,-0.034899496F,-0.017452406F,-1.8369701E-16F,0.017452406F,0.034899496F,0.052335955F,0.06975647F,0.087155744F,0.104528464F,0.12186934F,0.1391731F,0.15643446F,0.17364818F,0.190809F,0.20791169F,0.22495106F,0.2419219F,0.25881904F,0.27563736F,0.2923717F,0.309017F,0.32556817F,0.34202015F,0.35836795F,0.37460658F,0.39073113F,0.40673664F,0.42261827F,0.43837115F,0.4539905F,0.46947157F,0.4848096F,0.5F,0.5150381F,0.52991927F,0.54463905F,0.5591929F,0.57357645F,0.58778524F,0.60181504F,0.6156615F,0.6293204F,0.64278764F,0.656059F,0.6691306F,0.6819984F,0.6946584F,0.70710677F,0.7193398F,0.7313537F,0.7431448F,0.7547096F,0.76604444F,0.777146F,0.7880108F,0.7986355F,0.809017F,0.81915206F,0.82903755F,0.83867055F,0.8480481F,0.8571673F,0.8660254F,0.8746197F,0.88294756F,0.8910065F,0.89879405F,0.9063078F,0.9135454F,0.92050487F,0.92718387F,0.9335804F,0.9396926F,0.94551855F,0.95105654F,0.9563047F,0.9612617F,0.9659258F,0.9702957F,0.97437006F,0.9781476F,0.98162717F,0.9848077F,0.98768836F,0.99026805F,0.99254614F,0.9945219F,0.9961947F,0.9975641F,0.9986295F,0.99939084F,0.9998477F};

const float __sin[] = {0.0F,0.017452406F,0.034899496F,0.052335955F,0.06975647F,0.087155744F,0.104528464F,0.12186934F,0.1391731F,0.15643446F,0.17364818F,0.190809F,0.20791169F,0.22495106F,0.2419219F,0.25881904F,0.27563736F,0.2923717F,0.309017F,0.32556817F,0.34202015F,0.35836795F,0.37460658F,0.39073113F,0.40673664F,0.42261827F,0.43837115F,0.4539905F,0.46947157F,0.4848096F,0.5F,0.5150381F,0.52991927F,0.54463905F,0.5591929F,0.57357645F,0.58778524F,0.60181504F,0.6156615F,0.6293204F,0.64278764F,0.656059F,0.6691306F,0.6819984F,0.6946584F,0.70710677F,0.7193398F,0.7313537F,0.7431448F,0.7547096F,0.76604444F,0.777146F,0.7880108F,0.7986355F,0.809017F,0.81915206F,0.82903755F,0.83867055F,0.8480481F,0.8571673F,0.8660254F,0.8746197F,0.88294756F,0.8910065F,0.89879405F,0.9063078F,0.9135454F,0.92050487F,0.92718387F,0.9335804F,0.9396926F,0.94551855F,0.95105654F,0.9563047F,0.9612617F,0.9659258F,0.9702957F,0.97437006F,0.9781476F,0.98162717F,0.9848077F,0.98768836F,0.99026805F,0.99254614F,0.9945219F,0.9961947F,0.9975641F,0.9986295F,0.99939084F,0.9998477F,1.0F,0.9998477F,0.99939084F,0.9986295F,0.9975641F,0.9961947F,0.9945219F,0.99254614F,0.99026805F,0.98768836F,0.9848077F,0.98162717F,0.9781476F,0.97437006F,0.9702957F,0.9659258F,0.9612617F,0.9563047F,0.95105654F,0.94551855F,0.9396926F,0.9335804F,0.92718387F,0.92050487F,0.9135454F,0.9063078F,0.89879405F,0.8910065F,0.88294756F,0.8746197F,0.8660254F,0.8571673F,0.8480481F,0.83867055F,0.82903755F,0.81915206F,0.809017F,0.7986355F,0.7880108F,0.777146F,0.76604444F,0.7547096F,0.7431448F,0.7313537F,0.7193398F,0.70710677F,0.6946584F,0.6819984F,0.6691306F,0.656059F,0.64278764F,0.6293204F,0.6156615F,0.60181504F,0.58778524F,0.57357645F,0.5591929F,0.54463905F,0.52991927F,0.5150381F,0.5F,0.4848096F,0.46947157F,0.4539905F,0.43837115F,0.42261827F,0.40673664F,0.39073113F,0.37460658F,0.35836795F,0.34202015F,0.32556817F,0.309017F,0.2923717F,0.27563736F,0.25881904F,0.2419219F,0.22495106F,0.20791169F,0.190809F,0.17364818F,0.15643446F,0.1391731F,0.12186934F,0.104528464F,0.087155744F,0.06975647F,0.052335955F,0.034899496F,0.017452406F,1.2246469E-16F,-0.017452406F,-0.034899496F,-0.052335955F,-0.06975647F,-0.087155744F,-0.104528464F,-0.12186934F,-0.1391731F,-0.15643446F,-0.17364818F,-0.190809F,-0.20791169F,-0.22495106F,-0.2419219F,-0.25881904F,-0.27563736F,-0.2923717F,-0.309017F,-0.32556817F,-0.34202015F,-0.35836795F,-0.37460658F,-0.39073113F,-0.40673664F,-0.42261827F,-0.43837115F,-0.4539905F,-0.46947157F,-0.4848096F,-0.5F,-0.5150381F,-0.52991927F,-0.54463905F,-0.5591929F,-0.57357645F,-0.58778524F,-0.60181504F,-0.6156615F,-0.6293204F,-0.64278764F,-0.656059F,-0.6691306F,-0.6819984F,-0.6946584F,-0.70710677F,-0.7193398F,-0.7313537F,-0.7431448F,-0.7547096F,-0.76604444F,-0.777146F,-0.7880108F,-0.7986355F,-0.809017F,-0.81915206F,-0.82903755F,-0.83867055F,-0.8480481F,-0.8571673F,-0.8660254F,-0.8746197F,-0.88294756F,-0.8910065F,-0.89879405F,-0.9063078F,-0.9135454F,-0.92050487F,-0.92718387F,-0.9335804F,-0.9396926F,-0.94551855F,-0.95105654F,-0.9563047F,-0.9612617F,-0.9659258F,-0.9702957F,-0.97437006F,-0.9781476F,-0.98162717F,-0.9848077F,-0.98768836F,-0.99026805F,-0.99254614F,-0.9945219F,-0.9961947F,-0.9975641F,-0.9986295F,-0.99939084F,-0.9998477F,-1.0F,-0.9998477F,-0.99939084F,-0.9986295F,-0.9975641F,-0.9961947F,-0.9945219F,-0.99254614F,-0.99026805F,-0.98768836F,-0.9848077F,-0.98162717F,-0.9781476F,-0.97437006F,-0.9702957F,-0.9659258F,-0.9612617F,-0.9563047F,-0.95105654F,-0.94551855F,-0.9396926F,-0.9335804F,-0.92718387F,-0.92050487F,-0.9135454F,-0.9063078F,-0.89879405F,-0.8910065F,-0.88294756F,-0.8746197F,-0.8660254F,-0.8571673F,-0.8480481F,-0.83867055F,-0.82903755F,-0.81915206F,-0.809017F,-0.7986355F,-0.7880108F,-0.777146F,-0.76604444F,-0.7547096F,-0.7431448F,-0.7313537F,-0.7193398F,-0.70710677F,-0.6946584F,-0.6819984F,-0.6691306F,-0.656059F,-0.64278764F,-0.6293204F,-0.6156615F,-0.60181504F,-0.58778524F,-0.57357645F,-0.5591929F,-0.54463905F,-0.52991927F,-0.5150381F,-0.5F,-0.4848096F,-0.46947157F,-0.4539905F,-0.43837115F,-0.42261827F,-0.40673664F,-0.39073113F,-0.37460658F,-0.35836795F,-0.34202015F,-0.32556817F,-0.309017F,-0.2923717F,-0.27563736F,-0.25881904F,-0.2419219F,-0.22495106F,-0.20791169F,-0.190809F,-0.17364818F,-0.15643446F,-0.1391731F,-0.12186934F,-0.104528464F,-0.087155744F,-0.06975647F,-0.052335955F,-0.034899496F,-0.017452406F};


// FrameBuffer virtual device
typedef struct FbDev {
	int width;
	int height;
	uint16_t* fb;
	int fb_size; // width * height || nb of pixels (but not nb of bytes)
} FbDev;


#define _setPixel(x,y,rgb, fbDev) fbDev->fb[(y*fbDev->width)+x]=rgb
#define _getPixel(x,y    , fbDev) fbDev->fb[(y*fbDev->width)+x]

#define _getAddr(x, y, fbDev) ((y * fbDev->width) + x)


#define Color_t uint16_t


#include "floodfill.h"


	void __floodFill(int x, int y, Color_t color, FbDev* fb) {
		/* -makes a stack overflow -
		 if ( x >= 0 && x < fb->width && y >= 0 && y < fb->height && _getPixel(x,y,fb) != color) {
			 _setPixel(x,y,color,fb);
			 __floodFill(x - 1, y, color, fb);
			 __floodFill(x, y - 1, color, fb);
			 __floodFill(x, y + 1, color, fb);
			 __floodFill(x + 1, y, color, fb);
		 }
		 */ 
		 flood_fill(fb->fb, fb->width, fb->height, x, y, color);
	}

	// not the real scan lines impl.
	// yes it's a dirty impl. ;(
	void __scanFill(int xMin, int yMin, int xMax, int yMax, int* xys, int nbPts, Color_t color, FbDev* fb) {
		//int xpts[20]; // max 20 sommets par lignes
		int xptsCnt = 0;

		int insideX = -999;
		int insideY = -999;

		for (int y = yMin; y <= yMax; y++) {
			xptsCnt = 0;
			for (int i = 0; i < nbPts; i++) {
				if (xys[(i * 2) + 1] == y) {
					//xpts[xptsCnt++] = (i * 2) + 0;
					xptsCnt++;
				}
			}

			bool inside = false;
			int startX = -999, stopX = -999;
			for (int x = xMin; x <= xMax; x++) {
				if (xptsCnt > 0) {
					// noting
					break;
				} else {
					if (_getPixel(x, y, fb) == color) {
						inside = !inside;
					}
					if (inside && startX == -999) {
						startX = x;
					}

					if (!inside && x == startX + 1) {
						break;
					}

					if (!inside && startX > -999 && x > startX + 1) {
						stopX = x;
					}

					if (startX > -999 && stopX > -999 && stopX - startX > 2) {
						insideX = startX + 1;
						break;
					}

				}
			}
			if (insideX > -999) {
				insideY = y;
				break;
			}
		}

		if (insideX > -999 && insideY > -999) {
			__floodFill(insideX, insideY, color, fb);
		}

	}

  // just a Color for during the fill Op
  // const Color_t TEMP_FILL_COLOR = (15 << 11 | 15 << 5 | 15);
  const Color_t TEMP_FILL_COLOR = (1 << 11 | 1 << 5 | 1);

	void __replaceByFillColor(int startAddr, int endAddr, Color_t color, FbDev* fb) {
			if (startAddr < 0) {
				startAddr = 0;
			}
			if (endAddr >= fb->fb_size) {
				endAddr = fb->fb_size - 1;
			}
			for (int i = startAddr; i <= endAddr; i++) {
				if (fb->fb[i] == TEMP_FILL_COLOR)
					fb->fb[i] = color;
			}
	}
	
	// ========================================

		void setPixel(int x, int y, Color_t color, FbDev* fb) {
			if (x < 0) {
				return;
			} else if (x >= fb->width) {
				return;
			}

			if (y < 0) {
				return;
			} else if (y >= fb->height) {
				return;
			}

			_setPixel(x,y,color,fb);
		}

		int getPixel(int x, int y, FbDev* fb) {
			return _getPixel(x,y,fb);
		}

		// ====================
		// where a is float
		#define __round(a) ( (int) (a + 0.5F) )
		// where a is int
		#define __fabs(a) ( a >= 0 ? a : -a )

		void drawHorizLine(int x1, int x2, int y, Color_t rgb, FbDev* fb) {
			if (x2 < x1) {
				int tmp = x1;
				x1 = x2;
				x2 = tmp;
			}
			// int row = (y * SCREEN_WIDTH);
			for (int i = x1; i <= x2; i++) {
				setPixel(i, y, rgb, fb);
				// fb[row + i] = rgb;
			}
		}

		void drawVertLine(int y1, int y2, int x, Color_t rgb, FbDev* fb) {
			if (y2 < y1) {
				int tmp = y1;
				y1 = y2;
				y2 = tmp;
			}
			for (int i = y1; i <= y2; i++) {
				setPixel(x, i, rgb, fb);
			}
		}

		void drawLine(int x1, int y1, int x2, int y2, Color_t rgb, FbDev* fb) {
			if (y1 == y2) {
				drawHorizLine(x1, x2, y1, rgb, fb);
			} else if (x1 == x2) {
				drawVertLine(y1, y2, x1, rgb, fb);
			} else {
				int dx = x2 - x1, dy = y2 - y1, steps, k;
				float xIncrement, yIncrement, x = x1, y = y1;

				if (__fabs(dx) > __fabs(dy)) {
					steps = __fabs(dx);
				} else {
					steps = __fabs(dy);
				}
				xIncrement = (float) dx / (float) steps;
				yIncrement = (float) dy / (float) steps;
				setPixel(__round(x), __round(y), rgb, fb);
				for (k = 0; k < steps; k++) {
					x += xIncrement;
					y += yIncrement;
					setPixel(__round(x), __round(y), rgb, fb);
				}
			}
		}


	  // ============


		void drawRect(int x, int y, int w, int h, Color_t color, FbDev* fb) {
			drawHorizLine(x, x + w, y, color, fb);
			drawHorizLine(x, x + w, y + h, color, fb);
			drawVertLine(y, y + h, x, color, fb);
			drawVertLine(y, y + h, x + w, color, fb);
		}

		void fillRect(int x, int y, int w, int h, Color_t color, FbDev* fb) {
			drawRect(x, y, w, h, TEMP_FILL_COLOR, fb);
			__floodFill(x + (w >> 1), y + (h >> 1), TEMP_FILL_COLOR, fb);
			// /!\ let () around (y+h) Cf defined macro & args
			__replaceByFillColor(_getAddr(x, y, fb), _getAddr( (x + w), (y + h), fb), color, fb);
		}


		// ========================================

		void drawPolyLines(int* xys, int nbPts, Color_t color, FbDev* fb) {
			for (int i = 0; i < nbPts - 1; i++) {
				drawLine(xys[(2 * i) + 0], xys[(2 * i) + 1], xys[(2 * i) + 2], xys[(2 * i) + 3], color, fb);
			}
		}

		void fillPolygon(int* xys, int nbPts, Color_t color, FbDev* fb) {
			int x1, x2, y1, y2, xMin = SCREEN_WIDTH + 10, xMax = -10, yMin = SCREEN_HEIGHT + 10, yMax = -10;

			// drawPolyLines(xys, nbPts, TEMP_FILL_COLOR);
			for (int i = 0; i < nbPts - 1; i++) {
				x1 = xys[(2 * i) + 0];
				y1 = xys[(2 * i) + 1];
				x2 = xys[(2 * i) + 2];
				y2 = xys[(2 * i) + 3];

				if (x1 < xMin) {
					xMin = x1;
				}
				if (x1 > xMax) {
					xMax = x1;
				}

				if (x2 < xMin) {
					xMin = x2;
				}
				if (x2 > xMax) {
					xMax = x2;
				}

				if (y1 < yMin) {
					yMin = y1;
				}
				if (y1 > yMax) {
					yMax = y1;
				}

				if (y2 < yMin) {
					yMin = y2;
				}
				if (y2 > yMax) {
					yMax = y2;
				}

				drawLine(x1, y1, x2, y2, TEMP_FILL_COLOR, fb);
			}

			// if polygon isn't closed : close it
			if (!(xys[0] == xys[(nbPts * 2) - 2] && xys[1] == xys[(nbPts * 2) - 1])) {
				x1 = xys[0];
				y1 = xys[1];
				x2 = xys[(nbPts * 2) - 2];
				y2 = xys[(nbPts * 2) - 1];

				if (x1 < xMin) {
					xMin = x1;
				}
				if (x1 > xMax) {
					xMax = x1;
				}

				if (x2 < xMin) {
					xMin = x2;
				}
				if (x2 > xMax) {
					xMax = x2;
				}

				if (y1 < yMin) {
					yMin = y1;
				}
				if (y1 > yMax) {
					yMax = y1;
				}

				if (y2 < yMin) {
					yMin = y2;
				}
				if (y2 > yMax) {
					yMax = y2;
				}

				drawLine(x1, y1, x2, y2, TEMP_FILL_COLOR, fb);
			}

			__scanFill(xMin, yMin, xMax, yMax, xys, nbPts, TEMP_FILL_COLOR, fb);
			__replaceByFillColor(_getAddr(xMin, yMin, fb), _getAddr(xMax, yMax, fb), color, fb);
		}


		// ============

		void drawCircle(int x, int y, int radius, Color_t color, FbDev* fb) {
			int startAngle = 0, stopAngle = 90;
			float fradius = (float) radius;
			for (int a = startAngle; a <= stopAngle; a++) {
				int xc = (int) (fradius * __cos[a]);
				int ys = (int) (fradius * __sin[a]);
				setPixel(x + xc, y + ys, color, fb);
				setPixel(x - xc, y + ys, color, fb);
				setPixel(x + xc, y - ys, color, fb);
				setPixel(x - xc, y - ys, color, fb);
			}
		}

		void fillCircle(int x, int y, int radius, Color_t color, FbDev* fb) {
			drawCircle(x, y, radius, TEMP_FILL_COLOR, fb);
			__floodFill(x, y, TEMP_FILL_COLOR, fb);
			__replaceByFillColor(_getAddr( (x - radius), (y - radius), fb), _getAddr( (x + radius), (y + radius), fb), color, fb);
		}

#endif