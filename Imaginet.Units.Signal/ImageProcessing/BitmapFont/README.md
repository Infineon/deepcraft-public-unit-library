# BitmapFont Utility Unit

## Overview
`BitmapFont` is an internal utility unit that provides reusable bitmap font rendering capabilities for image processing units. This unit is hidden from the node library and serves as a dependency for other units that need to render text on images.

## Purpose
- **Centralized Font Management**: All bitmap font data is stored in one location
- **Code Reusability**: Multiple units can access the same font resources
- **Reduced Duplication**: Eliminates the need to duplicate font data across multiple units
- **Maintainability**: Font updates only need to be made in one place

## Features

### Font Sizes
The unit provides three bitmap font sizes:
- **Small (5x7)**: `font_small_5x7` - For very low resolution images (< 240p)
- **Large (8x12)**: `font_large_8x12` - For low-medium resolution images (240p - 720p)
- **Extra Large (12x16)**: `font_xlarge_12x16` - For high resolution images (720p+)

### Character Support
- ASCII characters from 32 (space) to 90 (Z)
- Automatic lowercase to uppercase conversion
- Numbers, punctuation, and special characters

### Available Fragments

#### 1. `bitmap_font_data`
Contains the raw bitmap font data arrays for all three font sizes.

#### 2. `bitmap_font_helpers`
Provides helper functions for font rendering:
- `get_font_metrics(font_size, &char_width, &char_height)` - Get dimensions for a font size
- `get_char_index(char)` - Convert ASCII character to font array index
- `write_pixel(image, idx, channels, r, g, b)` - Write a colored pixel
- `render_char(image, width, height, channels, x, y, char, font_size, r, g, b)` - Render single character
- `render_text(image, width, height, channels, x, y, text, font_size, r, g, b)` - Render text string

#### 3. `bitmap_font_text_utils`
Provides utility functions for text formatting:
- `float_to_string(value, buffer, precision)` - Convert float to string representation

## Usage

### In Other Units
To use the BitmapFont in your unit, add the following fragment dependencies to your C header file:

```c
#pragma IMAGINET_FRAGMENT_BEGIN "your_fragment_name"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "../BitmapFont/bitmap_font.h:bitmap_font_helpers"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "../BitmapFont/bitmap_font.h:bitmap_font_text_utils"

// Your code can now call font rendering functions
render_text(image, width, height, channels, x, y, "Hello", 1, 1.0f, 1.0f, 1.0f);
```

### Example: DisplayBoundingBox
The `DisplayBoundingBox` unit uses BitmapFont to render class names and confidence scores:

```c
#pragma IMAGINET_FRAGMENT_BEGIN "display_bounding_box_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "../BitmapFont/bitmap_font.h:bitmap_font_helpers"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "../BitmapFont/bitmap_font.h:bitmap_font_text_utils"

// ... later in code ...
int font_size = (image_height < 240) ? 0 : (image_height < 720) ? 1 : 2;
render_text(output, image_width, image_height, channels, 
            text_x, text_y, "Person 0.95", font_size, 1.0f, 1.0f, 1.0f);
```

## Files
- **bitmap_font.h**: C header file containing all font data and helper functions

## Configuration
This is a pure utility fragment (no `.imunit` file needed). It only provides code fragments for other units to import via `#pragma IMAGINET_FRAGMENT_DEPENDENCY`.

## Benefits
1. **Consistency**: All units use the same font rendering system
2. **Performance**: Font data is embedded once and referenced by multiple units
3. **Scalability**: New units can easily add text rendering capabilities
4. **Maintenance**: Single point of maintenance for font updates

## Future Extensions
Potential improvements:
- Add more font sizes
- Support for additional character sets (Unicode)
- Anti-aliased font rendering
- Variable-width fonts

## Related Units
- **DisplayBoundingBox**: Uses BitmapFont to render detection labels

