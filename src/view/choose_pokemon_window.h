#ifndef CHOOSE_POKEMON_WINDOW_H
#define CHOOSE_POKEMON_WINDOW_H

#include <gtk/gtk.h>
#include <stdio.h>
#include "../model/arena.h"

#define WIDGET(NAME) GtkWidget* NAME
#define POKEMON_BUTTON(NAME, NAME_IMAGE, NUMBER)\
	WIDGET(NAME);\
	WIDGET(NAME_IMAGE);\
	NAME = gtk_button_new();\
	NAME_IMAGE = gtk_image_new_from_file("sprites/pokemon/" #NAME ".png");\
	gtk_button_set_image(GTK_BUTTON(NAME),NAME_IMAGE);\
	g_signal_connect(GTK_BUTTON(NAME),"clicked",G_CALLBACK(info->add_call), (gpointer) &pokemon_id[NUMBER]);\
	gtk_button_set_relief(GTK_BUTTON(NAME), GTK_RELIEF_NONE);\
	gtk_flow_box_insert(GTK_FLOW_BOX(pokedex), NAME, -1);

static int pokemon_id[50] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49};

typedef struct
{
	GtkWidget * parent;
	void (*add_call)(GtkWidget *, gpointer);
	void (*clear_call)(GtkWidget *, gpointer);
} ui_pokedex_info_t;

void ui_clear_pokemon_team_1(GtkWidget * widget, gpointer callback_data);

void ui_clear_pokemon_team_2(GtkWidget * widget, gpointer callback_data);

void ui_add_pokemon_team_1(GtkWidget * widget, gpointer callback_data);

void ui_add_pokemon_team_2(GtkWidget * widget, gpointer callback_data);

void ui_create_pokedex_window(GtkApplication* app, gpointer pokedex_info);

#endif
