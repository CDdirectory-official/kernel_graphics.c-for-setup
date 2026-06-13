/*
 * Kernel Graphics Engine for Setup (Mode 13h - 320x200, 256 Colors)
 * Pure hardware pixel manipulation. No standards, no bloat.
 * Target Memory: 0xA0000
 */

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define VGA_MEMORY (unsigned char*)0xA0000

/* Color Palette Constants (Standard VGA) */
#define BLACK        0
#define BLUE         1
#define GREEN        2
#define CYAN         3
#define RED          4
#define MAGENTA      5
#define WHITE        15
#define DARK_GRAY    8

/* 
 * Function to switch to VGA Mode 13h (Graphics Mode)
 * We use BIOS interrupt 0x10 inside the bootloader or via inline assembly.
 * Assuming the bootloader already triggered Mode 13h, we directly manipulate pixels.
 */

// Draw a single pixel at coordinate (x, y)
void put_pixel(int x, int y, unsigned char color) {
    // Prevent memory corruption / out-of-bounds writing
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        // Formula: Index = (y * 320) + x
        VGA_MEMORY[(y * SCREEN_WIDTH) + x] = color;
    }
}

// Clear the entire screen with a solid color
void clear_screen(unsigned char color) {
    int total_pixels = SCREEN_WIDTH * SCREEN_HEIGHT; // 64000 pixels
    for (int i = 0; i < total_pixels; i++) {
        VGA_MEMORY[i] = color;
    }
}

// Draw a filled rectangle (Perfect for building GUI windows, buttons, or taskbars)
void draw_rectangle(int x, int y, int width, int height, unsigned char color) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            put_pixel(x + j, y + i, color);
        }
    }
}

/* The ultimate graphical entry point */
void kernel_main() {
    // 1. Clear screen to dark gray for an elegant setup environment background
    clear_screen(DARK_GRAY);

    // 2. Draw a modern "Setup Window" centered on screen
    // draw_rectangle(x, y, width, height, color)
    draw_rectangle(40, 30, 240, 140, BLACK);   // Window Shadow/Border
    draw_rectangle(42, 32, 236, 136, WHITE);   // Window Body
    draw_rectangle(42, 32, 236, 20, BLUE);     // Title Bar (Windows 95 style!)

    // 3. Draw a "Next/Confirm" Button at the bottom right of the window
    draw_rectangle(210, 140, 50, 18, GREEN);

    // 4. Draw a small warning accent (Red dot/box) just to show off pixel control
    draw_rectangle(50, 60, 10, 10, RED);

    // Freeze CPU here, let them stare at your beautiful low-level GUI layout
    while(1);
}
