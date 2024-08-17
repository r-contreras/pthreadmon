#ifndef FIGHT_WINDOW_H
#define FIGHT_WINDOW_H

#include <gtk/gtk.h>
#include <string.h>
#include "../model/arena.h"

//MACROS
#define PATH_START "sprites/pokemon/"
#define PATH_END ".png"
#define PATH(TEMP_POINTER, POKE_STRING)\
    char TEMP_POINTER[50];\
    sprintf(TEMP_POINTER, "%s%s%s",PATH_START,POKE_STRING,PATH_END);
//endofmacros

//imagenes
GtkWidget* team_1_attacker_image;
GtkWidget* team_2_attacker_image;
//labels
GtkWidget* team_1_name;
GtkWidget* team_2_name;
//barras de vida
GtkWidget* team_1_health_bar;
GtkWidget* team_2_health_bar;
//barras de energia
GtkWidget* team_1_energy_bar;
GtkWidget* team_2_energy_bar;

static int fight_call_id; //se necesita para parar el hilo de dibujo.

static gboolean ui_draw_fight(GtkWidget *widget, GdkEventExpose* event, gpointer data);

void* ui_stop_fight(void* args);

void ui_start_button(GtkApplication* app, gpointer user_data);

void ui_show_fight_window(GtkApplication* app, gpointer user_data);
#endif
