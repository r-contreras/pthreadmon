#include "choose_pokemon_window.h"

void ui_clear_pokemon_team_1 (GtkWidget * widget, gpointer callback_data)
{
	arena_clear_team_1();
}

void ui_clear_pokemon_team_2 (GtkWidget * widget, gpointer callback_data)
{
	arena_clear_team_2();
}

void ui_add_pokemon_team_1(GtkWidget * widget, gpointer callback_data)
{
	int* id = (int*) callback_data;
	bool added = arena_add_pokemon_team_1(*id);
	if(!added)
	{
		GtkWidget* dialog_window = gtk_dialog_new();
		GtkWidget* content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog_window));
		GtkWidget* label = gtk_label_new("	This pokemon has already been added to your team.	");
		GtkWidget* close_button = gtk_dialog_add_button (GTK_DIALOG(dialog_window), "OK", 0);

		gtk_window_set_title (GTK_WINDOW (dialog_window), "Can't repeat pokemons!");
		g_signal_connect_swapped(close_button, "clicked", G_CALLBACK(gtk_widget_destroy),(gpointer)dialog_window);

		gtk_box_pack_end(GTK_BOX(content_area),label,FALSE,FALSE,0);
		gtk_widget_show_all(dialog_window);
	}
}

void ui_add_pokemon_team_2(GtkWidget * widget, gpointer callback_data)
{
	int* id = (int*) callback_data;
	bool added = arena_add_pokemon_team_2(*id);
	if(!added)
	{
		GtkWidget* dialog_window = gtk_dialog_new();
		GtkWidget* content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog_window));
		GtkWidget* label = gtk_label_new("	This pokemon has already been added to your team.	");
		GtkWidget* close_button = gtk_dialog_add_button (GTK_DIALOG(dialog_window), "OK", 0);

		gtk_window_set_title (GTK_WINDOW (dialog_window), "Can't repeat pokemons!");
		g_signal_connect_swapped(close_button, "clicked", G_CALLBACK(gtk_widget_destroy),(gpointer)dialog_window);

		gtk_box_pack_end(GTK_BOX(content_area),label,FALSE,FALSE,0);
		gtk_widget_show_all(dialog_window);
	}
}

void ui_create_pokedex_window(GtkApplication* app, gpointer pokedex_info)
{
	ui_pokedex_info_t* info = (ui_pokedex_info_t*) pokedex_info;
	GtkWidget* parent = info->parent;

	GtkWidget* pokedex_window;
	GtkWidget* pokedex;
	GtkWidget* content_area;

	pokedex_window = gtk_dialog_new();
	gtk_window_set_title (GTK_WINDOW (pokedex_window), "Choose your Pokemons!");
	gtk_window_set_resizable (GTK_WINDOW (pokedex_window), FALSE);
	GtkWidget * clear_button = gtk_dialog_add_button (GTK_DIALOG(pokedex_window), "CLEAR TEAM", 0);
	g_signal_connect_swapped(clear_button, "clicked", G_CALLBACK(info->clear_call),NULL);
	GtkWidget * close_button = gtk_dialog_add_button (GTK_DIALOG(pokedex_window), "CLOSE BAG", 0);
	g_signal_connect_swapped(close_button, "clicked", G_CALLBACK(gtk_widget_destroy),(gpointer)pokedex_window);

	content_area = gtk_dialog_get_content_area(GTK_DIALOG(pokedex_window));

	pokedex = gtk_flow_box_new();
	gtk_flow_box_set_min_children_per_line (GTK_FLOW_BOX(pokedex), 10);
	gtk_flow_box_set_max_children_per_line(GTK_FLOW_BOX(pokedex), 10);
	gtk_flow_box_set_selection_mode(GTK_FLOW_BOX(pokedex), GTK_SELECTION_NONE);

	//botones definidos con macros
	POKEMON_BUTTON(venusaur,venusaur_,0);
	POKEMON_BUTTON(wigglytuff,wigglytuff_,1);
	POKEMON_BUTTON(gloom,gloom_,2);
	POKEMON_BUTTON(primeape,primeape_,3);
	POKEMON_BUTTON(machamp,machamp_,4);
	POKEMON_BUTTON(victreebel,victreebel_,5);
	POKEMON_BUTTON(dewgong,dewgong_,6);
	POKEMON_BUTTON(lickitung,lickitung_,7);
	POKEMON_BUTTON(lapras,lapras_,8);
	POKEMON_BUTTON(snorlax,snorlax_,9);
	POKEMON_BUTTON(zapdos,zapdos_,10);
	POKEMON_BUTTON(mew,mew_,11);
	POKEMON_BUTTON(meganium,meganium_,12);
	POKEMON_BUTTON(lanturn,lanturn_,13);
	POKEMON_BUTTON(bellossom,bellossom_,14);
	POKEMON_BUTTON(azumarill,azumarill_,15);
	POKEMON_BUTTON(politoed,politoed_,16);
	POKEMON_BUTTON(umbreon,umbreon_,17);
	POKEMON_BUTTON(mantine,mantine_,18);
	POKEMON_BUTTON(skarmory,skarmory_,19);
	POKEMON_BUTTON(hitmontop,hitmontop_,20);
	POKEMON_BUTTON(raikou,raikou_,21);
	POKEMON_BUTTON(swampert,swampert_,22);
	POKEMON_BUTTON(shiftry,shiftry_,23);
	POKEMON_BUTTON(pelipper,pelipper_,24);
	POKEMON_BUTTON(vigoroth,vigoroth_,25);
	POKEMON_BUTTON(medicham,medicham_,26);
	POKEMON_BUTTON(flygon,flygon_,27);
	POKEMON_BUTTON(altaria,altaria_,28);
	POKEMON_BUTTON(whiscash,whiscash_,29);
	POKEMON_BUTTON(castform,castform_,30);
	POKEMON_BUTTON(tropius,tropius_,31);
	POKEMON_BUTTON(regirock,regirock_,32);
	POKEMON_BUTTON(registeel,registeel_,33);
	POKEMON_BUTTON(deoxys,deoxys_,34);
	POKEMON_BUTTON(grotle,grotle_,35);
	POKEMON_BUTTON(bastiodon,bastiodon_,36);
	POKEMON_BUTTON(munchlax,munchlax_,37);
	POKEMON_BUTTON(abomasnow,abomasnow_,38);
	POKEMON_BUTTON(magnezone,magnezone_,39);
	POKEMON_BUTTON(cresselia,cresselia_,40);
	POKEMON_BUTTON(leavanny,leavanny_,41);
	POKEMON_BUTTON(whimsicott,whimsicott_,42);
	POKEMON_BUTTON(scrafty,scrafty_,43);
	POKEMON_BUTTON(alomomola,alomomola_,44);
	POKEMON_BUTTON(galvantula,galvantula_,45);
	POKEMON_BUTTON(ferrothorn,ferrothorn_,46);
	POKEMON_BUTTON(stunfisk,stunfisk_,47);
	POKEMON_BUTTON(zweilous,zweilous_,48);
	POKEMON_BUTTON(obstagoon,obstagoon_,49);

	gtk_box_pack_end(GTK_BOX(content_area),pokedex,FALSE,FALSE,0);

	gtk_widget_show_all(pokedex_window);
}
