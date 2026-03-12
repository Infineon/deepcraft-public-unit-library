#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#include <math.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "display_object_tracker_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "../../Drawing/BitmapFont/bitmap_font.h:bitmap_font_helpers"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "../../Drawing/BitmapFont/bitmap_font.h:bitmap_font_text_utils"

//============================================================================
// CONSTANTS AND CONFIGURATION
//============================================================================

#define MAX_TRAIL_LENGTH 12
#define MAX_TRAILS 200
#define MAX_CLASS_NAMES 50
#define TEXT_PADDING 1
#define MAX_LABEL_LENGTH 256
#define BRIGHTNESS_THRESHOLD 0.5f

//============================================================================
// DATA STRUCTURES
//============================================================================

// Trail structure for storing object paths
typedef struct {
    float x[MAX_TRAIL_LENGTH];  // Trail positions
    float y[MAX_TRAIL_LENGTH];
    int length;                 // Current trail length
    int track_id;              // Associated track ID
    int active;                // Whether trail is active
    int last_frame;            // Last frame this trail was updated
} TrackingTrail;

// Global trail storage (static to persist between calls)
static TrackingTrail trails[MAX_TRAILS];
static int trails_initialized = 0;
static int current_frame = 0;

//============================================================================
// UTILITY FUNCTIONS
//============================================================================

// Helper function to get tracked detection value from input tensor
static float get_tracked_detection_value(const float* restrict tracked_detections, int max_detections, int confidence_count, int conf_idx, int det_idx) {
    return tracked_detections[conf_idx * max_detections + det_idx];
}

// Helper function to generate unique color for each track ID
static void get_track_color(int track_id, float* r, float* g, float* b) {
    // Design system color palette with 27 distinct colors
    static const float colors[][3] = {
        {0.4f, 0.776f, 0.11f},    // GreenLight500 #66c61c
        {0.024f, 0.682f, 0.831f}, // Cyan500 #06aed4
        {0.0f, 0.525f, 0.788f},   // BlueLight500 #0086c9
        {0.18f, 0.565f, 0.98f},   // Blue500 #2e90fa
        {0.161f, 0.439f, 1.0f},   // BlueDark500 #2970ff
        {0.38f, 0.447f, 0.953f},  // Indigo500 #6172f3
        {0.529f, 0.357f, 0.969f}, // Violet500 #875bf7
        {0.478f, 0.353f, 0.973f}, // Purple500 #7a5af8
        {0.831f, 0.267f, 0.945f}, // Fuchsia500 #d444f1
        {0.933f, 0.275f, 0.737f}, // Pink500 #ee46bc
        {0.965f, 0.239f, 0.408f}, // Rose500 #f63d68
        {1.0f, 0.267f, 0.02f},    // OrangeDark500 #FF4405
        {0.937f, 0.408f, 0.125f}, // Orange500 #ef6820
        {0.918f, 0.667f, 0.031f}, // Yellow500 #eaaa08
        {0.941f, 0.267f, 0.22f},  // Error500 #F04438
        {0.365f, 0.42f, 0.596f},  // GrayCool500 #5d6b98
        {0.412f, 0.459f, 0.525f}, // GrayModern500 #697586
        {0.424f, 0.451f, 0.498f}, // GrayNeutral500 #6c737f
        {0.439f, 0.439f, 0.463f}, // GrayIron500 #707076
        {0.451f, 0.451f, 0.451f}, // GrayTrue500 #737373
        {0.475f, 0.443f, 0.42f},  // GrayWarm500 #79716b
        {0.086f, 0.388f, 0.392f}, // Green500 #166364
        {0.082f, 0.718f, 0.62f},  // Teal500 #15b79e
        {0.231f, 0.608f, 0.569f}, // InfineonBrand500 #3B9B91
        {0.969f, 0.565f, 0.035f}, // Warning500 #F79009
        {0.09f, 0.698f, 0.416f},  // Success500 #17B26A
        {0.306f, 0.357f, 0.651f}  // GrayBlue500 #4e5ba6
    };
    
    const int num_colors = 27;
    
    if (track_id > 0) {
        const int color_index = (track_id - 1) % num_colors;
        *r = colors[color_index][0];
        *g = colors[color_index][1]; 
        *b = colors[color_index][2];
    } else {
        // Default color for invalid track IDs
        *r = 0.5f; *g = 0.5f; *b = 0.5f;
    }
}

// Convert integer to string (simple implementation)
// Function removed: using shared implementation from BitmapFont/bitmap_font.h:bitmap_font_text_utils

// Parse comma-separated class names string
static int parse_class_names(const char* class_names_str, char class_names[][32], int max_classes) {
    if (class_names_str == NULL || class_names_str[0] == '\0') {
        return 0;
    }
    
    int class_count = 0;
    int str_idx = 0;
    int name_idx = 0;
    
    while (class_names_str[str_idx] != '\0' && class_count < max_classes) {
        const char c = class_names_str[str_idx];
        
        if (c == ',' || c == '\0') {
            // End of current class name
            class_names[class_count][name_idx] = '\0';
            if (name_idx > 0) {
                class_count++;
            }
            name_idx = 0;
        } else if (c != ' ' || name_idx > 0) {
            // Add character to current class name (skip leading spaces)
            if (name_idx < 31) {
                class_names[class_count][name_idx++] = c;
            }
        }
        
        str_idx++;
    }
    
    // Handle last class name if no trailing comma
    if (name_idx > 0 && class_count < max_classes) {
        class_names[class_count][name_idx] = '\0';
        class_count++;
    }
    
    return class_count;
}

//============================================================================
// PIXEL SETTING SYSTEM
//============================================================================

// Unified pixel setting function to eliminate code duplication
static void dot_set_pixel(float* restrict image, int image_width, int image_height, int channels,
                         int x, int y, float r, float g, float b) {
    if (x < 0 || x >= image_width || y < 0 || y >= image_height) return;
    
    if (channels == 1) {
        const int idx = y * image_width + x;
        image[idx] = (r + g + b) / 3.0f; // Convert to grayscale
    } else if (channels == 3) {
        const int idx = (y * image_width + x) * channels;
        image[idx] = r;
        image[idx + 1] = g;
        image[idx + 2] = b;
    } else {
        // Handle other channel layouts (e.g., CHW format)
        for (int ch = 0; ch < channels; ch++) {
            const int idx = ch * image_height * image_width + y * image_width + x;
            if (ch == 0) image[idx] = r;
            else if (ch == 1) image[idx] = g;
            else if (ch == 2) image[idx] = b;
        }
    }
}

//============================================================================
// FONT RENDERING SYSTEM
//============================================================================

//============================================================================
// DRAWING FUNCTIONS
//============================================================================

// Clamp coordinates to valid range
static void clamp_coords(int* x1, int* y1, int* x2, int* y2, int image_width, int image_height) {
    if (*x1 > *x2) { int temp = *x1; *x1 = *x2; *x2 = temp; }
    if (*y1 > *y2) { int temp = *y1; *y1 = *y2; *y2 = temp; }
    if (*x1 < 0) *x1 = 0;
    if (*y1 < 0) *y1 = 0;
    if (*x2 >= image_width) *x2 = image_width - 1;
    if (*y2 >= image_height) *y2 = image_height - 1;
}

// Draw a filled rectangle (for text backgrounds)
static void draw_filled_rectangle(float* restrict image, int image_width, int image_height, int channels,
                                 int x1, int y1, int x2, int y2, float r, float g, float b) {
    clamp_coords(&x1, &y1, &x2, &y2, image_width, image_height);
    
    for (int y = y1; y <= y2; y++) {
        for (int x = x1; x <= x2; x++) {
            dot_set_pixel(image, image_width, image_height, channels, x, y, r, g, b);
        }
    }
}

// Render text with colored background and optimal contrast
static void render_text_with_background(float* restrict image, int image_width, int image_height, int channels,
                                       int x, int y, const char* text, int font_size, float bg_r, float bg_g, float bg_b) {
    if (text[0] == '\0') return; // Empty text
    
    // Get font metrics for the selected font size
    int char_width, char_height;
    get_font_metrics(font_size, &char_width, &char_height);
    
    // Calculate text dimensions
    int text_len = 0;
    while (text[text_len] != '\0' && text_len < MAX_LABEL_LENGTH) text_len++;
    const int text_width = text_len * char_width;
    
    // Add padding around text
    const int bg_x1 = x - TEXT_PADDING;
    const int bg_y1 = y - TEXT_PADDING;
    const int bg_x2 = x + text_width - 1 + TEXT_PADDING;
    const int bg_y2 = y + char_height - 2 + TEXT_PADDING;
    
    // Draw background rectangle
    draw_filled_rectangle(image, image_width, image_height, channels, bg_x1, bg_y1, bg_x2, bg_y2, bg_r, bg_g, bg_b);
    
    // Calculate perceived brightness using standard luminance formula
    const float brightness = 0.299f * bg_r + 0.587f * bg_g + 0.114f * bg_b;
    
    // Choose text color based on background brightness for optimal contrast
    const float text_color = (brightness > BRIGHTNESS_THRESHOLD) ? 0.0f : 1.0f;
    
    // Draw text with optimal contrast color using BitmapFont
    render_text(image, image_width, image_height, channels, x, y, text, font_size, text_color, text_color, text_color);
}

// Generic line drawing using Bresenham's algorithm
static void draw_line(float* restrict image, int image_width, int image_height, int channels,
                     int x1, int y1, int x2, int y2, float r, float g, float b) {
    const int dx = abs(x2 - x1);
    const int dy = abs(y2 - y1);
    const int sx = (x1 < x2) ? 1 : -1;
    const int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    
    int x = x1, y = y1;
    
    while (1) {
        dot_set_pixel(image, image_width, image_height, channels, x, y, r, g, b);
        
        if (x == x2 && y == y2) break;
        
        const int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
}

// Draw thick line by rendering multiple parallel lines
static void draw_thick_line(float* restrict image, int image_width, int image_height, int channels,
                           int x1, int y1, int x2, int y2, int thickness, float r, float g, float b) {
    if (thickness <= 1) {
        draw_line(image, image_width, image_height, channels, x1, y1, x2, y2, r, g, b);
        return;
    }
    
    const int dx = x2 - x1;
    const int dy = y2 - y1;
    const float length = sqrtf((float)(dx * dx + dy * dy));
    
    if (length < 0.001f) {
        // Draw a thick point if line is too short
        const int start_offset = -thickness / 2;
        for (int i = 0; i < thickness; i++) {
            for (int j = 0; j < thickness; j++) {
                dot_set_pixel(image, image_width, image_height, channels, 
                            x1 + start_offset + i, y1 + start_offset + j, r, g, b);
            }
        }
        return;
    }
    
    // Calculate perpendicular unit vector
    const float perp_x = -(float)dy / length;
    const float perp_y = (float)dx / length;
    
    // Calculate starting offset to center the lines
    // Integer division truncates towards zero, providing consistent centering
    const int start_offset = -thickness / 2;
    
    // Draw 'thickness' number of parallel lines
    // Using integer loop ensures contiguous coverage for orthogonal lines
    for (int i = 0; i < thickness; i++) {
        const int offset = start_offset + i;
        
        // Use roundf to map to nearest integer coordinates
        const int offset_x1 = x1 + (int)roundf(perp_x * offset);
        const int offset_y1 = y1 + (int)roundf(perp_y * offset);
        const int offset_x2 = x2 + (int)roundf(perp_x * offset);
        const int offset_y2 = y2 + (int)roundf(perp_y * offset);
        
        draw_line(image, image_width, image_height, channels, 
                 offset_x1, offset_y1, offset_x2, offset_y2, r, g, b);
    }
}

//============================================================================
// TRACKING TRAIL SYSTEM
//============================================================================

// Initialize trail storage system
static void init_trails() {
    if (!trails_initialized) {
        for (int i = 0; i < MAX_TRAILS; i++) {
            trails[i].active = 0;
            trails[i].track_id = -1;
            trails[i].length = 0;
            trails[i].last_frame = 0;
        }
        trails_initialized = 1;
        current_frame = 0;
    }
}

// Update trail for a track
static void update_trail(int track_id, float x, float y) {
    // Find existing trail or create new one
    int trail_idx = -1;
    for (int i = 0; i < MAX_TRAILS; i++) {
        if (trails[i].active && trails[i].track_id == track_id) {
            trail_idx = i;
            break;
        }
    }
    
    if (trail_idx == -1) {
        // Create new trail
        for (int i = 0; i < MAX_TRAILS; i++) {
            if (!trails[i].active) {
                trail_idx = i;
                trails[i].active = 1;
                trails[i].track_id = track_id;
                trails[i].length = 0;
                trails[i].last_frame = current_frame;
                break;
            }
        }
    } else {
        // Check if this trail is stale (ID reuse detection)
        // If trail hasn't been updated in 10+ frames, it's likely a reused ID
        const int frame_gap = current_frame - trails[trail_idx].last_frame;
        if (frame_gap > 10) {
            // Stale trail detected - clear it and start fresh
            trails[trail_idx].length = 0;
        }
        trails[trail_idx].last_frame = current_frame;
    }
    
    if (trail_idx >= 0) {
        // Shift trail positions if at maximum length
        if (trails[trail_idx].length >= MAX_TRAIL_LENGTH) {
            for (int i = 0; i < MAX_TRAIL_LENGTH - 1; i++) {
                trails[trail_idx].x[i] = trails[trail_idx].x[i + 1];
                trails[trail_idx].y[i] = trails[trail_idx].y[i + 1];
            }
            trails[trail_idx].x[MAX_TRAIL_LENGTH - 1] = x;
            trails[trail_idx].y[MAX_TRAIL_LENGTH - 1] = y;
        } else {
            trails[trail_idx].x[trails[trail_idx].length] = x;
            trails[trail_idx].y[trails[trail_idx].length] = y;
            trails[trail_idx].length++;
        }
    }
}

// Draw trail for a track
static void draw_trail(float* restrict image, int image_width, int image_height, int channels,
                      int track_id, int thickness, float r, float g, float b) {
    for (int i = 0; i < MAX_TRAILS; i++) {
        if (trails[i].active && trails[i].track_id == track_id && trails[i].length > 1) {
            for (int j = 1; j < trails[i].length; j++) {
                const float alpha = (float)j / trails[i].length; // Fade out older points
                draw_thick_line(image, image_width, image_height, channels,
                               (int)trails[i].x[j-1], (int)trails[i].y[j-1],
                               (int)trails[i].x[j], (int)trails[i].y[j],
                               thickness, r * alpha, g * alpha, b * alpha);
            }
            break;
        }
    }
}

//============================================================================
// MAIN DISPLAY FUNCTION
//============================================================================

// Main function to display tracked objects on image
static void display_object_tracker_f32(
    const float* restrict image,
    const float* restrict tracked_detections,
    float* restrict output,
    int image_height,
    int image_width,
    int channels,
    int max_detections,
    int confidence_count,
    int font_size,
    int show_track_id,
    int show_class_name,
    int show_confidence,
    int show_tracking_confidence,
    int show_tracking_trail,
    int trail_thickness,
    const char* class_names_str)
{
    // Step 1: Copy input image to output buffer
    const int total_pixels = image_height * image_width * channels;
    for (int i = 0; i < total_pixels; i++) {
        output[i] = image[i];
    }
    
    // Step 2: Initialize tracking trail system
    init_trails();
    
    // Step 3: Parse class names from input string
    char class_names[MAX_CLASS_NAMES][32];
    const int num_class_names = parse_class_names(class_names_str, class_names, MAX_CLASS_NAMES);
    
    // Process each tracked detection
    for (int i = 0; i < max_detections; i++) {
        // Get basic detection data (center_x, center_y, width, height)
        const float center_x = get_tracked_detection_value(tracked_detections, max_detections, confidence_count, 0, i);
        const float center_y = get_tracked_detection_value(tracked_detections, max_detections, confidence_count, 1, i);
        const float width = get_tracked_detection_value(tracked_detections, max_detections, confidence_count, 2, i);
        const float height = get_tracked_detection_value(tracked_detections, max_detections, confidence_count, 3, i);
        
        // Skip empty detections
        if (width <= 0 || height <= 0) continue;
        
        // Find class with highest confidence (skip first 4 values: x, y, w, h)
        float max_conf = 0.0f;
        int class_id = -1;
        for (int j = 4; j < confidence_count - 2; j++) { // -2 for track_id and tracking_confidence
            const float class_conf = get_tracked_detection_value(tracked_detections, max_detections, confidence_count, j, i);
            if (class_conf > max_conf) {
                max_conf = class_conf;
                class_id = j - 4;
            }
        }
        
        // Get tracking information
        const int track_id = (int)get_tracked_detection_value(tracked_detections, max_detections, confidence_count, confidence_count - 2, i);
        const float tracking_confidence = get_tracked_detection_value(tracked_detections, max_detections, confidence_count, confidence_count - 1, i);
        
        // Skip detections without valid track ID
        if (track_id <= 0) continue;
        
        // Get color for this track
        float r, g, b;
        get_track_color(track_id, &r, &g, &b);
        
        // Convert coordinates to pixel space
        int pixel_center_x, pixel_center_y, pixel_width, pixel_height;
        pixel_center_x = (int)(center_x * image_width);
        pixel_center_y = (int)(center_y * image_height);
        pixel_width = (int)(width * image_width);
        pixel_height = (int)(height * image_height);

        
        // Update and draw trail
        if (show_tracking_trail) {
            update_trail(track_id, (float)pixel_center_x, (float)pixel_center_y);
            draw_trail(output, image_width, image_height, channels, track_id, trail_thickness, r, g, b);
        }
        
        // Calculate rectangle corners for label positioning
        const int x1 = pixel_center_x - pixel_width / 2;
        const int y1 = pixel_center_y - pixel_height / 2;
        
        // Build and render label text
        char label_text[MAX_LABEL_LENGTH] = "";
        int label_pos = 0;
        
        // Add track ID
        if (show_track_id) {
            char track_str[16];
            int_to_string(track_id, track_str);
            for (int k = 0; track_str[k] != '\0' && label_pos < MAX_LABEL_LENGTH - 1; k++) {
                label_text[label_pos++] = track_str[k];
            }
        }
        
        // Add class name
        if (show_class_name) {
            if (show_track_id && label_pos > 0 && label_pos < MAX_LABEL_LENGTH - 1) {
                label_text[label_pos++] = ' ';
            }
            
            const char* class_name = (class_id >= 0 && class_id < num_class_names) ? 
                                    class_names[class_id] : "Unknown";
            for (int k = 0; class_name[k] != '\0' && label_pos < MAX_LABEL_LENGTH - 1; k++) {
                label_text[label_pos++] = class_name[k];
            }
        }
        
        // Add confidence scores
        if (show_confidence) {
            if (label_pos > 0 && label_pos < MAX_LABEL_LENGTH - 1) label_text[label_pos++] = ' ';
            label_text[label_pos++] = 'C'; label_text[label_pos++] = ':';
            char conf_str[16];
            float_to_string(max_conf, conf_str, 2);
            for (int k = 0; conf_str[k] != '\0' && label_pos < MAX_LABEL_LENGTH - 1; k++) {
                label_text[label_pos++] = conf_str[k];
            }
        }
        
        if (show_tracking_confidence) {
            if (label_pos > 0 && label_pos < MAX_LABEL_LENGTH - 1) label_text[label_pos++] = ' ';
            label_text[label_pos++] = 'T'; label_text[label_pos++] = ':';
            char track_conf_str[16];
            float_to_string(tracking_confidence, track_conf_str, 2);
            for (int k = 0; track_conf_str[k] != '\0' && label_pos < MAX_LABEL_LENGTH - 1; k++) {
                label_text[label_pos++] = track_conf_str[k];
            }
        }
        
        label_text[label_pos] = '\0';
        
        // Render text with background
        if (label_pos > 0) {
            int text_x = (x1 < 0) ? 0 : x1;
            int text_y = (y1 - 10 < 0) ? y1 + 8 : y1 - 10;
            if (text_x >= image_width - 50) text_x = image_width - 50;
            if (text_y >= image_height - 10) text_y = image_height - 10;
            
            render_text_with_background(output, image_width, image_height, channels, text_x, text_y, label_text, font_size, r, g, b);
        }
    }
    
    // Increment frame counter for next call
    current_frame++;
}

#pragma IMAGINET_FRAGMENT_END 