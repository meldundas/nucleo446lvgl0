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
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj0 = obj;
            lv_obj_set_pos(obj, 79, 12);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "ECET 260");
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffe6ee05), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // swred
            lv_obj_t *obj = lv_switch_create(parent_obj);
            objects.swred = obj;
            lv_obj_set_pos(obj, 64, 85);
            lv_obj_set_size(obj, 50, 25);
            lv_obj_add_event_cb(obj, action_switch_changed, LV_EVENT_PRESSED, (void *)0);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xfff2e910), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffec1818), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // swgreen
            lv_obj_t *obj = lv_switch_create(parent_obj);
            objects.swgreen = obj;
            lv_obj_set_pos(obj, 64, 137);
            lv_obj_set_size(obj, 50, 25);
            lv_obj_add_event_cb(obj, action_switch_changed, LV_EVENT_PRESSED, (void *)0);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xfff2e910), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff10e556), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // swblue
            lv_obj_t *obj = lv_switch_create(parent_obj);
            objects.swblue = obj;
            lv_obj_set_pos(obj, 64, 187);
            lv_obj_set_size(obj, 50, 25);
            lv_obj_add_event_cb(obj, action_switch_changed, LV_EVENT_PRESSED, (void *)0);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xfff2e910), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff2844ec), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // blueled
            lv_obj_t *obj = lv_led_create(parent_obj);
            objects.blueled = obj;
            lv_obj_set_pos(obj, 12, 183);
            lv_obj_set_size(obj, 32, 32);
            lv_led_set_color(obj, lv_color_hex(0xff0000ff));
            lv_led_set_brightness(obj, 0);
        }
        {
            // Temperature
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.temperature = obj;
            lv_obj_set_pos(obj, 185, 118);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "");
            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffe6ee05), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // greenled
            lv_obj_t *obj = lv_led_create(parent_obj);
            objects.greenled = obj;
            lv_obj_set_pos(obj, 12, 133);
            lv_obj_set_size(obj, 32, 32);
            lv_led_set_color(obj, lv_color_hex(0xff00ff00));
            lv_led_set_brightness(obj, 0);
        }
        {
            // redled
            lv_obj_t *obj = lv_led_create(parent_obj);
            objects.redled = obj;
            lv_obj_set_pos(obj, 12, 81);
            lv_obj_set_size(obj, 32, 32);
            lv_led_set_color(obj, lv_color_hex(0xffff0000));
            lv_led_set_brightness(obj, 0);
        }
        {
            // arc
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.arc = obj;
            lv_obj_set_pos(obj, 144, 65);
            lv_obj_set_size(obj, 150, 150);
            lv_arc_set_range(obj, 0, 50);
            lv_arc_set_value(obj, 25);
            lv_arc_set_bg_end_angle(obj, 60);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj1 = obj;
            lv_obj_set_pos(obj, 181, 207);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "0");
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffe6ee05), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj2 = obj;
            lv_obj_set_pos(obj, 245, 207);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "50");
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffe6ee05), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
}

void tick_screen_main() {
    {
        const char *new_val = get_var_therm_temp();
        const char *cur_val = lv_label_get_text(objects.temperature);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.temperature;
            lv_label_set_text(objects.temperature, new_val);
            tick_value_change_obj = NULL;
        }
    }
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
