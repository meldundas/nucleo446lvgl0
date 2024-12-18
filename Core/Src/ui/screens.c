#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;
lv_obj_t *tick_value_change_obj;

void create_screen_main() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_SNAPPABLE|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff1a627f), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(obj, &lv_font_montserrat_34, LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // maincontainer
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.maincontainer = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 320, 240);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj0 = obj;
            lv_obj_set_pos(obj, 80, 28);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "ECET 260");
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffe6ee05), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // LED_R
            lv_obj_t *obj = lv_switch_create(parent_obj);
            objects.led_r = obj;
            lv_obj_set_pos(obj, 55, 95);
            lv_obj_set_size(obj, 50, 25);
            lv_obj_add_event_cb(obj, action_switch_changed, LV_EVENT_PRESSED, (void *)0);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xfff2e910), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffec1818), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // LED_G
            lv_obj_t *obj = lv_switch_create(parent_obj);
            objects.led_g = obj;
            lv_obj_set_pos(obj, 55, 135);
            lv_obj_set_size(obj, 50, 25);
            lv_obj_add_event_cb(obj, action_switch_changed, LV_EVENT_PRESSED, (void *)0);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xfff2e910), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff10e556), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // LED_B
            lv_obj_t *obj = lv_switch_create(parent_obj);
            objects.led_b = obj;
            lv_obj_set_pos(obj, 55, 176);
            lv_obj_set_size(obj, 50, 25);
            lv_obj_add_event_cb(obj, action_switch_changed, LV_EVENT_PRESSED, (void *)0);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xfff2e910), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff2844ec), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // ledButton
            lv_obj_t *obj = lv_imagebutton_create(parent_obj);
            objects.led_button = obj;
            lv_obj_set_pos(obj, 160, 95);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, 100);
            lv_imagebutton_set_src(obj, LV_IMAGEBUTTON_STATE_RELEASED, NULL, &img_led_offsr, NULL);
            lv_imagebutton_set_src(obj, LV_IMAGEBUTTON_STATE_PRESSED, NULL, &img_led_onsr, NULL);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
        }
    }
}

void tick_screen_main() {
}


void create_screens() {
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    create_screen_main();
}

typedef void (*tick_screen_func_t)();

tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
};

void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
