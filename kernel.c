/*
 * ZeonOS x64 Kernel Main Entry Point
 * kernel_main64.c
 * 
 * This is the C entry point for the kernel after bootloader
 * handles long mode initialization
 */

#include "kernel.h"

/* Video memory for text output */
#define VIDEO_MEMORY 0xb8000
#define VIDEO_WIDTH 80
#define VIDEO_HEIGHT 25

/* Cursor position */
static int cursor_x = 0;
static int cursor_y = 0;

/* Color codes */
#define COLOR_BLACK 0
#define COLOR_BLUE 1
#define COLOR_GREEN 2
#define COLOR_CYAN 3
#define COLOR_RED 4
#define COLOR_MAGENTA 5
#define COLOR_BROWN 6
#define COLOR_LIGHT_GRAY 7

#define COLOR_DARK_GRAY 8
#define COLOR_LIGHT_BLUE 9
#define COLOR_LIGHT_GREEN 10
#define COLOR_LIGHT_CYAN 11
#define COLOR_LIGHT_RED 12
#define COLOR_LIGHT_MAGENTA 13
#define COLOR_YELLOW 14
#define COLOR_WHITE 15

/**
 * Make color byte from foreground and background
 */
static inline uint8_t make_color(uint8_t fg, uint8_t bg) {
    return (bg << 4) | fg;
}

/**
 * Make vga entry (character + color)
 */
static inline uint16_t make_vga_entry(char c, uint8_t color) {
    return ((uint16_t)c) | (((uint16_t)color) << 8);
}

/**
 * Put character at position
 */
static void put_char_at(char c, uint8_t color, int x, int y) {
    uint16_t* vga_buffer = (uint16_t*)VIDEO_MEMORY;
    int index = y * VIDEO_WIDTH + x;
    vga_buffer[index] = make_vga_entry(c, color);
}

/**
 * Clear screen
 */
void kernel_clear_screen(void) {
    uint8_t color = make_color(COLOR_WHITE, COLOR_BLACK);
    
    for (int y = 0; y < VIDEO_HEIGHT; y++) {
        for (int x = 0; x < VIDEO_WIDTH; x++) {
            put_char_at(' ', color, x, y);
        }
    }
    
    cursor_x = 0;
    cursor_y = 0;
}

/**
 * Scroll screen up one line
 */
static void scroll_screen(void) {
    uint8_t color = make_color(COLOR_WHITE, COLOR_BLACK);
    uint16_t* vga_buffer = (uint16_t*)VIDEO_MEMORY;
    
    for (int y = 0; y < VIDEO_HEIGHT - 1; y++) {
        for (int x = 0; x < VIDEO_WIDTH; x++) {
            int src_index = (y + 1) * VIDEO_WIDTH + x;
            int dst_index = y * VIDEO_WIDTH + x;
            vga_buffer[dst_index] = vga_buffer[src_index];
        }
    }
    
    for (int x = 0; x < VIDEO_WIDTH; x++) {
        int index = (VIDEO_HEIGHT - 1) * VIDEO_WIDTH + x;
        vga_buffer[index] = make_vga_entry(' ', color);
    }
    
    cursor_y--;
}

/**
 * Put character at current cursor position
 */
void kernel_putchar(char c) {
    uint8_t color = make_color(COLOR_WHITE, COLOR_BLACK);
    
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else if (c == '\r') {
        cursor_x = 0;
    } else if (c == '\t') {
        cursor_x += 4;
    } else {
        put_char_at(c, color, cursor_x, cursor_y);
        cursor_x++;
    }
    
    if (cursor_x >= VIDEO_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }
    
    if (cursor_y >= VIDEO_HEIGHT) {
        scroll_screen();
    }
}

/**
 * Print string
 */
void kernel_print(const char* str) {
    while (*str) {
        kernel_putchar(*str);
        str++;
    }
}

/**
 * Print string with newline
 */
void kernel_println(const char* str) {
    kernel_print(str);
    kernel_putchar('\n');
}

/**
 * Convert integer to string
 */
static void int_to_string(int num, char* str) {
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    
    int negative = num < 0;
    if (negative) num = -num;
    
    char temp[32];
    int i = 0;
    while (num > 0) {
        temp[i++] = '0' + (num % 10);
        num /= 10;
    }
    
    if (negative) {
        str[0] = '-';
        for (int j = 0; j < i; j++) {
            str[j + 1] = temp[i - j - 1];
        }
        str[i + 1] = '\0';
    } else {
        for (int j = 0; j < i; j++) {
            str[j] = temp[i - j - 1];
        }
        str[i] = '\0';
    }
}

/**
 * Print integer
 */
void kernel_print_int(int num) {
    char str[32];
    int_to_string(num, str);
    kernel_print(str);
}

/**
 * Main kernel entry point
 */
void kernel_main(void) {
    kernel_clear_screen();
    
    kernel_println("========================================");
    kernel_println("  Welcome to ZeonOS x64 Kernel v1.0");
    kernel_println("========================================");
    kernel_println("");
    
    kernel_println("* Bootloader: Complete");
    kernel_println("* Long mode: Enabled");
    kernel_println("* Video mode: VGA Text Mode 80x25");
    kernel_println("");
    
    kernel_println("System Information:");
    kernel_print("  Screen Width: ");
    kernel_print_int(VIDEO_WIDTH);
    kernel_println(" characters");
    
    kernel_print("  Screen Height: ");
    kernel_print_int(VIDEO_HEIGHT);
    kernel_println(" lines");
    kernel_println("");
    
    kernel_println("Initializing kernel components...");
    
    /* Initialize kernel subsystems */
    kernel_init_memory();
    kernel_init_interrupts();
    kernel_init_timer();
    
    kernel_println("");
    kernel_println("Kernel initialization complete!");
    kernel_println("Ready for user input...");
    kernel_println("");
    
    /* Kernel main loop */
    while (1) {
        __asm__("hlt");
    }
}

/**
 * Halt the system
 */
void kernel_halt(void) {
    kernel_println("System halted.");
    while (1) {
        __asm__("hlt");
    }
}
