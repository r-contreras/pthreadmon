#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <gtk/gtk.h>
#include "../model/arena.h"
#include "choose_pokemon_window.h"
#include "fight_window.h"

static gchar* trainer_1_name; //unica manera de que se pueda cambiar el nombre dentro de las funciones (?)
static gchar* trainer_2_name;

static GtkEntryBuffer* trainer_1_buffer; //buffer para que el usuario ingrese su nombre
static GtkEntryBuffer* trainer_2_buffer;

void ui_start_fight(GtkWidget * widget, gpointer callback_data);
void ui_set_trainer_1_name(GtkWidget * widget, gpointer callback_data);
void ui_set_trainer_2_name(GtkWidget * widget, gpointer callback_data);

void ui_show_trainer_window(GtkApplication* app, gpointer user_data);

#endif
