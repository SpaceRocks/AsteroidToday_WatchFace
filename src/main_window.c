#include <pebble.h>
#include "main_window.h"

#define ASTEROID_COUNT 0
#define KEY_TEMPERATURE 1
#define KEY_CONDITIONS 2
#define KEY_CLOUDINESS 3

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_roboto_bold_subset_49;
static GFont s_res_gothic_18_bold;
static GFont s_res_gothic_14;
static GFont s_res_gothic_24_bold;
static TextLayer *s_timelayer_main;
static TextLayer *s_asteroidlayer_main;
static TextLayer *s_asteroidlabel_1;
static TextLayer *s_weather_main;
static TextLayer *s_cloudLabel_2;
static TextLayer *s_cloudyness_main;
static TextLayer *s_cloudyLabel_1;
static BitmapLayer *moon_image_layer;

static void initialise_ui(void) {
  s_window = window_create();
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, 1);
  #endif
  
  s_res_roboto_bold_subset_49 = fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49);
  s_res_gothic_18_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  s_res_gothic_14 = fonts_get_system_font(FONT_KEY_GOTHIC_14);
  s_res_gothic_24_bold = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
  // s_timelayer_main
  s_timelayer_main = text_layer_create(GRect(3, 46, 139, 54));
  text_layer_set_text(s_timelayer_main, "00:00");
  text_layer_set_text_alignment(s_timelayer_main, GTextAlignmentCenter);
  text_layer_set_font(s_timelayer_main, s_res_roboto_bold_subset_49);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_timelayer_main);
  
  // s_asteroidlayer_main
  s_asteroidlayer_main = text_layer_create(GRect(23, 119, 29, 24));
  text_layer_set_text(s_asteroidlayer_main, "-");
  text_layer_set_text_alignment(s_asteroidlayer_main, GTextAlignmentCenter);
  text_layer_set_font(s_asteroidlayer_main, s_res_gothic_24_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_asteroidlayer_main);
  
  // s_asteroidlabel_1
  s_asteroidlabel_1 = text_layer_create(GRect(5, 99, 76, 17));
  text_layer_set_text(s_asteroidlabel_1, "AsteroidToday");
  text_layer_set_font(s_asteroidlabel_1, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_asteroidlabel_1);
  
  // s_weather_main
  s_weather_main = text_layer_create(GRect(2, 2, 134, 24));
  text_layer_set_text(s_weather_main, "-");
  text_layer_set_text_alignment(s_weather_main, GTextAlignmentRight);
  text_layer_set_font(s_weather_main, s_res_gothic_24_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_weather_main);
  
  // s_cloudLabel_2
  s_cloudLabel_2 = text_layer_create(GRect(93, 26, 42, 19));
  text_layer_set_text(s_cloudLabel_2, "Cloudy");
  text_layer_set_text_alignment(s_cloudLabel_2, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_cloudLabel_2);
  
  // s_cloudyness_main
  s_cloudyness_main = text_layer_create(GRect(58, 26, 20, 18));
  text_layer_set_text(s_cloudyness_main, "-");
  text_layer_set_text_alignment(s_cloudyness_main, GTextAlignmentCenter);
  text_layer_set_font(s_cloudyness_main, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_cloudyness_main);
  
  // s_cloudyLabel_1
  s_cloudyLabel_1 = text_layer_create(GRect(76, 26, 15, 18));
  text_layer_set_text(s_cloudyLabel_1, "%");
  text_layer_set_text_alignment(s_cloudyLabel_1, GTextAlignmentRight);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_cloudyLabel_1);
  
  // moon_image_layer
  moon_image_layer = bitmap_layer_create(GRect(84, 101, 50, 49));
  layer_add_child(window_get_root_layer(s_window), (Layer *)moon_image_layer);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(s_timelayer_main);
  text_layer_destroy(s_asteroidlayer_main);
  text_layer_destroy(s_asteroidlabel_1);
  text_layer_destroy(s_weather_main);
  text_layer_destroy(s_cloudLabel_2);
  text_layer_destroy(s_cloudyness_main);
  text_layer_destroy(s_cloudyLabel_1);
  bitmap_layer_destroy(moon_image_layer);
}
// END AUTO-GENERATED UI CODE

static GBitmap *s_bitmap;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, 
           sizeof(s_buffer),
           clock_is_24h_style() ?
           "%H:%M" : "%I:%M", tick_time);
  text_layer_set_text(s_timelayer_main, s_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  static char asteroid_buffer[32];
  static char temperature_buffer[8];
  static char conditions_buffer[32];
  static char weather_layer_buffer[32];
  static char cloudy_buffer[8];
  
  Tuple *asteroid_tuple = dict_find(iterator, ASTEROID_COUNT);
  if(asteroid_tuple) {
     snprintf(asteroid_buffer,
              sizeof(asteroid_buffer),
              "%d",
               (int)asteroid_tuple->value->int32);
     APP_LOG(APP_LOG_LEVEL_INFO, "data received with value %d", (int)asteroid_tuple->value->int32);
  }
  text_layer_set_text(s_asteroidlayer_main, asteroid_buffer);

  Tuple *temp_tuple = dict_find(iterator, KEY_TEMPERATURE);
  Tuple *conditions_tuple = dict_find(iterator, KEY_CONDITIONS);
  if(temp_tuple && conditions_tuple) {
    snprintf(temperature_buffer, sizeof(temperature_buffer), "%d°", (int)temp_tuple->value->int32);
    snprintf(conditions_buffer, sizeof(conditions_buffer), "%s", conditions_tuple->value->cstring);

    // Assemble full string and display
    snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%s %s", temperature_buffer, conditions_buffer);
    text_layer_set_text(s_weather_main, weather_layer_buffer);
  }
  
  Tuple *cloudy_tuple = dict_find(iterator, KEY_CLOUDINESS);
  if(cloudy_tuple) {
    snprintf(cloudy_buffer,
              sizeof(cloudy_buffer),
              "%d",
               (int)cloudy_tuple->value->int32);
  }
  text_layer_set_text(s_cloudyness_main, cloudy_buffer);
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}


static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_main_window(void) {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  
  window_stack_push(s_window, true);
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  update_time();

  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
  s_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MOON_FULL);
  bitmap_layer_set_bitmap(moon_image_layer, s_bitmap);

}
void hide_main_window(void) {
  window_stack_remove(s_window, true);
}

int main(void) {
  show_main_window();
  app_event_loop();
  hide_main_window();
}