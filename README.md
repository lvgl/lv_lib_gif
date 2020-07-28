# GIF decoder for LVGL
Allow to use of GIF images in LVGL from. 

Based on https://github.com/lecram/gifdec

## Get started
- Download or clone this repository
  - [Download from GitHub](https://github.com/lvgl/lv_lib_gifdec/archive/master.zip)
  - Clone: `git clone https://github.com/lvgl/lv_lib_gifdec.git`
- Include the library: `#include "lv_lib_lgifdec/lv_gifdec.h"`

## Use GIF images from file
```c
lv_obj_t * img = lv_gif_create_from_file(parent, "S/path/to/img.gif");
```

## Use GIF images from flash
If the gif file stored in the flash as a C array: 
```c
lv_obj_t * img = lv_gif_create_from_data(parent, gif_data);
```
