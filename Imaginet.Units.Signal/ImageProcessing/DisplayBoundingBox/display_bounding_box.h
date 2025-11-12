#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#include <math.h>
#include <string.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "display_bounding_box_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "../BitmapFont/bitmap_font.h:bitmap_font_helpers"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "../BitmapFont/bitmap_font.h:bitmap_font_text_utils"

// Compile-time constants for readability
#define DET_ATTRIBS 4                 // center_x, center_y, width, height
#define MAX_CLASSES 100
#define MAX_CLASS_NAME_LEN 32
#define MAX_TEXT_LEN 64
#define MAX_CONF_STR_LEN 16

// Helper function to get value from detection tensor at [row=confidence_idx, col=detection_idx]
static inline float get_detection_value(const float* restrict detections, int max_detections, int confidence_idx, int detection_idx) {
    return detections[confidence_idx * max_detections + detection_idx];
}

// Helper function to generate unique color for each class
static inline void get_class_color(int class_id, float* r, float* g, float* b) {
    // Updated color map (27 distinct colors)
    // If more than 27 classes, cycle back to the start
    const float colors[][3] = {
        {0.161f, 0.439f, 1.0f},   // BlueDark500 #2970ff
        {0.937f, 0.408f, 0.125f}, // Orange500 #ef6820
        {0.082f, 0.718f, 0.62f},  // Teal500 #15b79e
        {0.365f, 0.42f, 0.596f},  // GrayCool500 #5d6b98
        {0.831f, 0.267f, 0.945f}, // Fuchsia500 #d444f1
        {0.4f, 0.776f, 0.11f},    // GreenLight500 #66c61c
        {0.965f, 0.239f, 0.408f}, // Rose500 #f63d68
        {0.475f, 0.443f, 0.42f},  // GrayWarm500 #79716b
        {0.38f, 0.447f, 0.953f},  // Indigo500 #6172f3
        {0.969f, 0.565f, 0.035f}, // Warning500 #F79009
        {0.0f, 0.525f, 0.788f},   // BlueLight500 #0086c9
        {0.424f, 0.451f, 0.498f}, // GrayNeutral500 #6c737f
        {0.09f, 0.698f, 0.416f},  // Success500 #17B26A
        {0.478f, 0.353f, 0.973f}, // Purple500 #7a5af8
        {0.918f, 0.667f, 0.031f}, // Yellow500 #eaaa08
        {0.451f, 0.451f, 0.451f}, // GrayTrue500 #737373
        {0.529f, 0.357f, 0.969f}, // Violet500 #875bf7
        {1.0f, 0.267f, 0.02f},    // OrangeDark500 #FF4405
        {0.024f, 0.682f, 0.831f}, // Cyan500 #06aed4
        {0.439f, 0.439f, 0.463f}, // GrayIron500 #707076
        {0.941f, 0.267f, 0.22f},  // Error500 #F04438
        {0.18f, 0.565f, 0.98f},   // Blue500 #2e90fa
        {0.086f, 0.388f, 0.392f}, // Green500 #166364
        {0.412f, 0.459f, 0.525f}, // GrayModern500 #697586
        {0.231f, 0.608f, 0.569f}, // InfineonBrand500 #3B9B91
        {0.306f, 0.357f, 0.651f}, // GrayBlue500 #4e5ba6
        {0.933f, 0.275f, 0.737f}  // Pink500 #ee46bc
    };
    
    const int num_colors = 27;
    
    // Cycle through colors if class_id exceeds available colors
    int color_index = class_id % num_colors;
    
    *r = colors[color_index][0];
    *g = colors[color_index][1]; 
    *b = colors[color_index][2];
}

// Helper function to draw a horizontal line
static inline void draw_horizontal_line(float* restrict image, int image_width, int image_height, int channels,
                                      int x1, int x2, int y, int thickness, float r, float g, float b) {
    if (y < 0 || y >= image_height) return;
    
    int start_x = x1 < x2 ? x1 : x2;
    int end_x = x1 < x2 ? x2 : x1;
    
    if (start_x < 0) start_x = 0;
    if (end_x >= image_width) end_x = image_width - 1;
    
    const int use_rgb = (channels >= 3);
    const float gray_value = 0.299f * r + 0.587f * g + 0.114f * b;

    for (int t = 0; t < thickness; ++t) {
        int draw_y = y + t;
        if (draw_y >= 0 && draw_y < image_height) {
            int base_index = (draw_y * image_width + start_x) * channels;
            float* pix = image + base_index;
            if (use_rgb) {
                for (int x = start_x; x <= end_x; ++x) {
                    pix[0] = r;
                    pix[1] = g;
                    pix[2] = b;
                    pix += channels;
                }
            } else {
                for (int x = start_x; x <= end_x; ++x) {
                    pix[0] = gray_value;
                    pix += channels;
                }
            }
        }
    }
}

// Helper function to draw a vertical line
static inline void draw_vertical_line(float* restrict image, int image_width, int image_height, int channels,
                                    int x, int y1, int y2, int thickness, float r, float g, float b) {
    if (x < 0 || x >= image_width) return;
    
    int start_y = y1 < y2 ? y1 : y2;
    int end_y = y1 < y2 ? y2 : y1;
    
    if (start_y < 0) start_y = 0;
    if (end_y >= image_height) end_y = image_height - 1;
    
    const int use_rgb = (channels >= 3);
    const float gray_value = 0.299f * r + 0.587f * g + 0.114f * b;

    for (int t = 0; t < thickness; ++t) {
        int draw_x = x + t;
        if (draw_x >= 0 && draw_x < image_width) {
            int idx = (start_y * image_width + draw_x) * channels;
            float* pix = image + idx;
            const int step = image_width * channels;
            if (use_rgb) {
                for (int y = start_y; y <= end_y; ++y) {
                    pix[0] = r;
                    pix[1] = g;
                    pix[2] = b;
                    pix += step;
                }
            } else {
                for (int y = start_y; y <= end_y; ++y) {
                    pix[0] = gray_value;
                    pix += step;
                }
            }
        }
    }
}

// Helper function to draw rectangle outline
static inline void draw_rectangle(float* restrict image, int image_width, int image_height, int channels,
                                int x1, int y1, int x2, int y2, int thickness, float r, float g, float b) {
    // Draw top and bottom horizontal lines
    draw_horizontal_line(image, image_width, image_height, channels, x1, x2, y1, thickness, r, g, b);
    draw_horizontal_line(image, image_width, image_height, channels, x1, x2, y2 - thickness + 1, thickness, r, g, b);
    
    // Draw left and right vertical lines
    draw_vertical_line(image, image_width, image_height, channels, x1, y1, y2, thickness, r, g, b);
    draw_vertical_line(image, image_width, image_height, channels, x2 - thickness + 1, y1, y2, thickness, r, g, b);
}

// Helper function to draw a filled rectangle
static inline void draw_filled_rectangle(float* restrict image, int image_width, int image_height, int channels,
                                       int x1, int y1, int x2, int y2, float r, float g, float b) {
    if (x1 > x2) { int tmp = x1; x1 = x2; x2 = tmp; }
    if (y1 > y2) { int tmp = y1; y1 = y2; y2 = tmp; }

    if (x2 < 0 || y2 < 0 || x1 >= image_width || y1 >= image_height) return;

    if (x1 < 0) x1 = 0;
    if (y1 < 0) y1 = 0;
    if (x2 >= image_width) x2 = image_width - 1;
    if (y2 >= image_height) y2 = image_height - 1;

    const int use_rgb = (channels >= 3);
    const float gray_value = 0.299f * r + 0.587f * g + 0.114f * b;

    for (int y = y1; y <= y2; ++y) {
        int base_index = (y * image_width + x1) * channels;
        float* pix = image + base_index;
        if (use_rgb) {
            for (int x = x1; x <= x2; ++x) {
                pix[0] = r;
                pix[1] = g;
                pix[2] = b;
                pix += channels;
            }
        } else {
            for (int x = x1; x <= x2; ++x) {
                pix[0] = gray_value;
                pix += channels;
            }
        }
    }
}

// Helper function to compute relative luminance for sRGB colors
static inline float compute_luminance(float r, float g, float b) {
    return 0.2126f * r + 0.7152f * g + 0.0722f * b;
}

// Helper function to parse class names from comma-separated string
static inline int parse_class_names(const char* class_names_str, char class_names[][32], int max_classes) {
    if (class_names_str == NULL || class_names_str[0] == '\0') {
        return 0; // Empty string, use default names
    }

    int class_count = 0;
    int char_idx = 0;

    for (int i = 0; class_names_str[i] != '\0' && class_count < max_classes; ++i) {
        char c = class_names_str[i];

        if (c == ',') {
            // Terminate current token
            if (char_idx > 0) {
                class_names[class_count][char_idx] = '\0';
                class_count++;
                char_idx = 0;
            }
            // Skip: allow empty tokens and trim spaces automatically by ignoring leading spaces next
            continue;
        }

        // Skip leading spaces/tabs before a token starts
        if (char_idx == 0 && (c == ' ' || c == '\t')) {
            continue;
        }

        // Append character
        if (char_idx < 31) {
            class_names[class_count][char_idx++] = c;
        }
    }

    // Flush last token
    if (char_idx > 0 && class_count < max_classes) {
        class_names[class_count][char_idx] = '\0';
        class_count++;
    }

    return class_count;
}

static inline void display_bounding_box_f32(
    const float* restrict image,
    const float* restrict detections,
    float* restrict output,
    int image_height,
    int image_width,
    int channels,
    int max_detections,
    int confidence_count,
    int box_thickness,
    int show_class_name,
    int show_confidence,
    const char* class_names_str)
{
    // First, copy input image to output
    int total_pixels = image_height * image_width * channels;
    memcpy(output, image, (size_t)total_pixels * sizeof(float));

    // Parse custom class names if provided
    char custom_class_names[MAX_CLASSES][MAX_CLASS_NAME_LEN];
    int custom_class_count = parse_class_names(class_names_str, custom_class_names, MAX_CLASSES);
    
    // Determine font size based on image resolution
    // Use small font for very low res, large font for medium res, extra large font for high res
    int font_size;
    if (image_height < 240 || image_width < 320) {
        font_size = 0; // Small font 5x7 for very low resolution (sub-QVGA)
    } else if (image_height < 720 || image_width < 1280) {
        font_size = 1; // Large font 8x12 for low-medium resolution (up to 720p)
    } else {
        font_size = 2; // Extra large font 12x16 for high resolution (720p+)
    }
    
            // Process each detection
        for (int det = 0; det < max_detections; ++det) {
            // Get bounding box coordinates (center_x, center_y, width, height)
            float center_x = get_detection_value(detections, max_detections, 0, det);
            float center_y = get_detection_value(detections, max_detections, 1, det);
            float width = get_detection_value(detections, max_detections, 2, det);
            float height = get_detection_value(detections, max_detections, 3, det);
            
            // Skip if no detection (width or height is 0)
            if (width <= 0.0f || height <= 0.0f) continue;
            
            // Find the highest confidence class (starting from index 4)
            float max_confidence = 0.0f;
            int best_class = -1;
            
            for (int c = 4; c < confidence_count; ++c) {
                float conf = get_detection_value(detections, max_detections, c, det);
                if (conf > max_confidence) {
                    max_confidence = conf;
                    best_class = c - 4; // Adjust for class indexing
                }
            }
            
            // Skip if no valid class found
            if (best_class < 0) continue;
        
        // Get unique color for this class
        float box_color_r, box_color_g, box_color_b;
        get_class_color(best_class, &box_color_r, &box_color_g, &box_color_b);
        
        // Convert normalized coordinates to pixel coordinates
        int pixel_center_x = (int)(center_x * image_width);
        int pixel_center_y = (int)(center_y * image_height);
        int pixel_width = (int)(width * image_width);
        int pixel_height = (int)(height * image_height);
        
        // Calculate rectangle corners
        int x1 = pixel_center_x - pixel_width / 2;
        int y1 = pixel_center_y - pixel_height / 2;
        int x2 = pixel_center_x + pixel_width / 2;
        int y2 = pixel_center_y + pixel_height / 2;
        
        // Ensure coordinates are within image bounds
        if (x1 < 0) x1 = 0;
        if (y1 < 0) y1 = 0;
        if (x2 >= image_width) x2 = image_width - 1;
        if (y2 >= image_height) y2 = image_height - 1;
        
        // Skip if rectangle is completely outside image bounds
        if (x1 >= x2 || y1 >= y2) continue;
        
        // Draw the bounding box rectangle
        draw_rectangle(output, image_width, image_height, channels,
                      x1, y1, x2, y2, box_thickness,
                      box_color_r, box_color_g, box_color_b);
        
        // Render text above the bounding box
        if (show_class_name || show_confidence) {
            
            char text_buffer[MAX_TEXT_LEN];
            int text_pos = 0;
            
            // Add class name
            if (show_class_name) {
                // Use custom class name if available
                if (custom_class_count > 0 && best_class < custom_class_count) {
                    // Copy the custom class name to the text buffer
                    for (int i = 0; custom_class_names[best_class][i] != '\0' && text_pos < 50; i++) {
                        text_buffer[text_pos++] = custom_class_names[best_class][i];
                    }
                } else {
                    // Generate default class name like "Class0", "Class1", etc.
                    const char* class_prefix = "Class";
                    for (int i = 0; class_prefix[i] != '\0' && text_pos < 50; i++) {
                        text_buffer[text_pos++] = class_prefix[i];
                    }
                    
                    // Convert class number to string
                    char class_num_str[MAX_CONF_STR_LEN];
                    int class_num = best_class;
                    int num_digits = 0;
                    int temp = class_num;
                    
                    // Count digits
                    if (temp == 0) {
                        num_digits = 1;
                    } else {
                        while (temp > 0) {
                            temp /= 10;
                            num_digits++;
                        }
                    }
                    
                    // Convert number to string
                    for (int i = num_digits - 1; i >= 0; i--) {
                        class_num_str[i] = '0' + (class_num % 10);
                        class_num /= 10;
                    }
                    class_num_str[num_digits] = '\0';
                    
                    // Append number to text buffer
                    for (int i = 0; class_num_str[i] != '\0' && text_pos < 50; i++) {
                        text_buffer[text_pos++] = class_num_str[i];
                    }
                }
                
                if (show_confidence && text_pos < 60) {
                    text_buffer[text_pos++] = ' ';
                }
            }
            
            // Add confidence score
            if (show_confidence) {
                char conf_str[16];
                float_to_string(max_confidence, conf_str, 2);
                for (int i = 0; conf_str[i] != '\0' && text_pos < 60; i++) {
                    text_buffer[text_pos++] = conf_str[i];
                }
            }
            
            text_buffer[text_pos] = '\0';
            
            // Compute text dimensions
            int char_width, char_height;
            get_font_metrics(font_size, &char_width, &char_height);

            int text_len = 0;
            while (text_buffer[text_len] != '\0' && text_len < 64) text_len++;
            int text_pixel_width = text_len * char_width;

            const int padding = 3; // expand background three pixels on all sides
            int label_width = text_pixel_width + 2 * padding;
            int label_height = char_height + 2 * padding;

            // Prefer placing above; account for covering the border thickness when computing fit
            int effective_border = (box_thickness > 0) ? box_thickness : 1;
            int can_place_above = (y1 + effective_border - label_height) >= 0;
            int bg_y1, bg_y2;
            if (can_place_above) {
                // Cover exactly the top border thickness for symmetry without extra overlap
                bg_y2 = y1 + effective_border - 1;
                bg_y1 = bg_y2 - label_height + 1;
                if (bg_y1 < 0) {
                    bg_y1 = 0;
                    bg_y2 = bg_y1 + label_height - 1;
                }
            } else {
                // Place inside: start the background at the top edge so it touches the box
                bg_y1 = y1;
                bg_y2 = y1 + label_height - 1;
                if (bg_y2 >= image_height) bg_y2 = image_height - 1;
            }

            // Ensure background does not go further left than the bounding box's left edge
            int bg_x1 = x1;
            int bg_x2 = bg_x1 + label_width - 1;
            if (bg_x2 >= image_width) bg_x2 = image_width - 1;

            // Draw background with box color so it appears as one entity with the bounding box
            draw_filled_rectangle(output, image_width, image_height, channels,
                                  bg_x1, bg_y1, bg_x2, bg_y2,
                                  box_color_r, box_color_g, box_color_b);

            // Compute luminance to pick contrasting text color
            float lum = compute_luminance(box_color_r, box_color_g, box_color_b);
            float text_r = lum > 0.5f ? 0.0f : 1.0f;
            float text_g = lum > 0.5f ? 0.0f : 1.0f;
            float text_b = lum > 0.5f ? 0.0f : 1.0f;

            // Render text within the background (respect left/top padding)
            int text_x = bg_x1 + padding;
            int text_y = bg_y1 + padding;
            render_text(output, image_width, image_height, channels,
                        text_x, text_y, text_buffer, font_size, text_r, text_g, text_b);
        }
    }
}

#pragma IMAGINET_FRAGMENT_END 