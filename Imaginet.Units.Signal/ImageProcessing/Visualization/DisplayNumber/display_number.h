#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#include <math.h>
#pragma IMAGINET_INCLUDES_END

// =============================================================================
// FRAGMENT: display_number_common
// Shared logic for rendering numbers
// =============================================================================
#pragma IMAGINET_FRAGMENT_BEGIN "display_number_common"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "../../Drawing/BitmapFont/bitmap_font.h:bitmap_font_helpers"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "../../Drawing/BitmapFont/bitmap_font.h:bitmap_font_text_utils"

//============================================================================
// CONSTANTS AND CONFIGURATION
//============================================================================

#define DISPLAY_NUMBER_MAX_CHARS 32
#define DISPLAY_NUMBER_NUM_COLORS 9

// Background rendering constants
#define DISPLAY_NUMBER_TEXT_PADDING 3
#define BRIGHTNESS_THRESHOLD 0.5f

//============================================================================
// COLOR DEFINITIONS
//============================================================================

static const float display_number_colors[DISPLAY_NUMBER_NUM_COLORS][3] = {
    {1.0f, 1.0f, 1.0f},       // White
    {0.0f, 0.0f, 0.0f},       // Black
    {0.941f, 0.267f, 0.22f},  // Error500 #F04438 (Red)
    {0.09f, 0.698f, 0.416f},  // Success500 #17B26A (Green)
    {0.18f, 0.565f, 0.98f},   // Blue500 #2e90fa (Blue)
    {0.918f, 0.667f, 0.031f}, // Yellow500 #eaaa08 (Yellow)
    {0.937f, 0.408f, 0.125f}, // Orange500 #ef6820 (Orange)
    {0.024f, 0.682f, 0.831f}, // Cyan500 #06aed4 (Cyan)
    {0.478f, 0.353f, 0.973f}  // Purple500 #7a5af8 (Purple)
};

//============================================================================
// HELPER FUNCTIONS
//============================================================================

// Get color values for the specified color index
static void display_number_get_color(int color_index, float* r, float* g, float* b) {
    *r = display_number_colors[color_index][0];
    *g = display_number_colors[color_index][1];
    *b = display_number_colors[color_index][2];
}

// Clamp coordinates to image bounds
static void clamp_coordinates(int* x, int* y, int image_width, int image_height) {
    *x = (*x < 0) ? 0 : (*x >= image_width) ? image_width - 1 : *x;
    *y = (*y < 0) ? 0 : (*y >= image_height) ? image_height - 1 : *y;
}

// Background rendering functions
static void display_number_clamp_coords(int* x1, int* y1, int* x2, int* y2, int image_width, int image_height) {
    if (*x1 > *x2) { int temp = *x1; *x1 = *x2; *x2 = temp; }
    if (*y1 > *y2) { int temp = *y1; *y1 = *y2; *y2 = temp; }
    if (*x1 < 0) *x1 = 0;
    if (*y1 < 0) *y1 = 0;
    if (*x2 >= image_width) *x2 = image_width - 1;
    if (*y2 >= image_height) *y2 = image_height - 1;
}

// Draw a filled rectangle (for text backgrounds)
static void display_number_draw_filled_rectangle(float* restrict image, int image_width, int image_height, int channels,
                                 int x1, int y1, int x2, int y2, float r, float g, float b) {
    display_number_clamp_coords(&x1, &y1, &x2, &y2, image_width, image_height);
    
    for (int y = y1; y <= y2; y++) {
        for (int x = x1; x <= x2; x++) {
            const int idx = (y * image_width + x) * channels;
            write_pixel(image, idx, channels, r, g, b);
        }
    }
}

// Render text with colored background and optimal contrast
static void display_number_render_text_with_background(float* restrict image, int image_width, int image_height, int channels,
                                      int x, int y, const char* text, int font_size, 
                                      float bg_r, float bg_g, float bg_b) {
    // Get font metrics for the selected font size
    int char_width, char_height;
    get_font_metrics(font_size, &char_width, &char_height);
    
    // Calculate text dimensions
    int text_len = 0;
    while (text[text_len] != '\0' && text_len < DISPLAY_NUMBER_MAX_CHARS) text_len++;
    const int text_width = text_len * char_width;
    
    // Add padding around text
    const int bg_x1 = x - DISPLAY_NUMBER_TEXT_PADDING;
    const int bg_y1 = y - DISPLAY_NUMBER_TEXT_PADDING;
    const int bg_x2 = x + text_width + DISPLAY_NUMBER_TEXT_PADDING;
    const int bg_y2 = y + char_height + DISPLAY_NUMBER_TEXT_PADDING;
    
    // Draw background rectangle
    display_number_draw_filled_rectangle(image, image_width, image_height, channels, bg_x1, bg_y1, bg_x2, bg_y2, bg_r, bg_g, bg_b);
    
    // Calculate perceived brightness using standard luminance formula
    const float brightness = 0.299f * bg_r + 0.587f * bg_g + 0.114f * bg_b;
    
    // Choose text color based on background brightness for optimal contrast
    const float text_color = (brightness > BRIGHTNESS_THRESHOLD) ? 0.0f : 1.0f;
    
    // Draw text with optimal contrast color using BitmapFont
    render_text(image, image_width, image_height, channels, x, y, text, font_size, text_color, text_color, text_color);
}

//============================================================================
// GENERIC DISPLAY FUNCTION
//============================================================================

static void display_number(
    const float* restrict image,
    float* restrict output,
    int image_height,
    int image_width,
    int channels,
    double x_position,
    double y_position,
    int font_size,
    int text_color,
    const char* number_str,
    int show_background)
{
    // Copy image to output
    const int total_pixels = image_height * image_width * channels;
    for (int i = 0; i < total_pixels; i++) {
        output[i] = image[i];
    }
    
    // Convert normalized coordinates to pixel coordinates
    int pixel_x = (int)(x_position * image_width);
    int pixel_y = (int)(y_position * image_height);
    clamp_coordinates(&pixel_x, &pixel_y, image_width, image_height);
    
    if (show_background) {
        // Get background color values from text_color parameter
        float bg_r, bg_g, bg_b;
        display_number_get_color(text_color, &bg_r, &bg_g, &bg_b);
        
        // Render text with background and automatic contrast
        display_number_render_text_with_background(output, image_width, image_height, channels, 
                                   pixel_x, pixel_y, number_str, font_size, bg_r, bg_g, bg_b);
    } else {
        // Get text color values
        float r, g, b;
        display_number_get_color(text_color, &r, &g, &b);
        
        // Render the string without background using BitmapFont
        render_text(output, image_width, image_height, channels, 
                   pixel_x, pixel_y, number_str, font_size, r, g, b);
    }
}

#pragma IMAGINET_FRAGMENT_END

// =============================================================================
// FRAGMENT: display_number_f32
// Float implementation
// =============================================================================
#pragma IMAGINET_FRAGMENT_BEGIN "display_number_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "display_number_common"

// Main function for float values
static void display_number_f32(
    const float* restrict image,
    const float* restrict value,
    float* restrict output,
    int image_height,
    int image_width,
    int channels,
    double x_position,
    double y_position,
    int font_size,
    int text_color,
    int decimal_places,
    int show_background)
{
    char number_str[DISPLAY_NUMBER_MAX_CHARS];
    float_to_string(*value, number_str, decimal_places);
    display_number(image, output, image_height, image_width, channels,
                          x_position, y_position, font_size, text_color, number_str, show_background);
}

#pragma IMAGINET_FRAGMENT_END

// =============================================================================
// FRAGMENT: display_number_i32
// Integer implementation
// =============================================================================
#pragma IMAGINET_FRAGMENT_BEGIN "display_number_i32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "display_number_common"

// Main function for integer values
static void display_number_i32(
    const float* restrict image,
    const int* restrict value,
    float* restrict output,
    int image_height,
    int image_width,
    int channels,
    double x_position,
    double y_position,
    int font_size,
    int text_color,
    int decimal_places,
    int show_background)
{
    char number_str[DISPLAY_NUMBER_MAX_CHARS];
    int_to_string(*value, number_str);
    display_number(image, output, image_height, image_width, channels,
                          x_position, y_position, font_size, text_color, number_str, show_background);
}

#pragma IMAGINET_FRAGMENT_END
