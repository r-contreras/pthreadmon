#include "fight_window.h"

static gboolean ui_draw_fight(GtkWidget *widget, GdkEventExpose* event, gpointer data)
{
	if(arena->is_running)
	{
		//nombres de los pokemones
		gtk_label_set_text(GTK_LABEL(team_1_name),arena->team_1_attacker->species_name);
		gtk_label_set_text(GTK_LABEL(team_2_name),arena->team_2_attacker->species_name);
		//imagenes de los pokemones
		char team_1_image_path[50];
		char team_2_image_path[50];
		sprintf(team_1_image_path,"%s%s%s","sprites/pokemon/",arena->team_1_attacker->species_name,".png");
		sprintf(team_2_image_path,"%s%s%s","sprites/pokemon/",arena->team_2_attacker->species_name,".png");
		gtk_image_set_from_file(GTK_IMAGE(team_1_attacker_image), team_1_image_path);
		gtk_image_set_from_file(GTK_IMAGE(team_2_attacker_image), team_2_image_path);
		//barras de vida
		gtk_level_bar_set_value(GTK_LEVEL_BAR(team_1_health_bar), arena->team_1_attacker->hp);
		gtk_level_bar_set_value(GTK_LEVEL_BAR(team_2_health_bar), arena->team_2_attacker->hp);
		//barras de energia
		gtk_level_bar_set_max_value(GTK_LEVEL_BAR(team_1_energy_bar), arena->team_1_attacker->charged_attack.energy);
		gtk_level_bar_set_max_value(GTK_LEVEL_BAR(team_2_energy_bar), arena->team_2_attacker->charged_attack.energy);
		gtk_level_bar_set_value(GTK_LEVEL_BAR(team_1_energy_bar), arena->team_1_attacker->current_energy);
		gtk_level_bar_set_value(GTK_LEVEL_BAR(team_2_energy_bar), arena->team_2_attacker->current_energy);
	}
	if(arena->winner)
	{
		sleep(0.2); //para que se vea quien perdio
		//show winner window
		GtkWidget* fight_window = (GtkWidget*) data;
		GtkWidget* stats_window;
		GtkWidget* team_1_label;
		GtkWidget* team_2_label;
		GtkWidget* content_area;
		GtkWidget* close_button;
		GtkWidget* grid;

		GtkWidget** team_1_images = (GtkWidget**) malloc(3*sizeof(GtkWidget*));
		GtkWidget** team_2_images = (GtkWidget**) malloc(3*sizeof(GtkWidget*));

		stats_window = gtk_dialog_new();
		char winner_text[50];
		sprintf(winner_text,"%s's team wins!",arena->winner);
		gtk_window_set_title (GTK_WINDOW (stats_window), winner_text);
		gtk_window_set_resizable (GTK_WINDOW (stats_window), FALSE);

		//boton de salida
		close_button = gtk_dialog_add_button (GTK_DIALOG(stats_window), "END SIMULATION", 0);
		g_signal_connect_swapped(close_button, "clicked", G_CALLBACK(gtk_widget_destroy),(gpointer)stats_window);

		//caja de contenido del dialog window
		content_area = gtk_dialog_get_content_area(GTK_DIALOG(stats_window));
		//grid en donde se pone la informacion
		grid = gtk_grid_new();
		gtk_grid_set_row_spacing(GTK_GRID(grid),10);
		gtk_grid_set_column_spacing(GTK_GRID(grid),5);
		//strings de los teams para presentar los datos
		char trainer_1_string[50];
		char trainer_2_string[50];
		sprintf(trainer_1_string,"%s's team:",arena->trainer_1);
		sprintf(trainer_2_string,"%s's team:",arena->trainer_2);
		team_1_label = gtk_label_new(trainer_1_string);
		team_2_label = gtk_label_new(trainer_2_string);
			//se agregan los labels al grid
		gtk_grid_attach(GTK_GRID(grid), team_1_label,0,0,3,1);
		gtk_grid_attach(GTK_GRID(grid), team_2_label,0,4,3,1);
		for(int pokemon = 0; pokemon < TEAM_SIZE; ++pokemon)
		{
			char team_1_image_path[50];
			char team_2_image_path[50];
			sprintf(team_1_image_path,"%s%s%s","sprites/pokemon/",arena->pokemon_data_list[pokemon].pokemon->species_name,".png");
			sprintf(team_2_image_path,"%s%s%s","sprites/pokemon/",arena->pokemon_data_list[pokemon+3].pokemon->species_name,".png");
			team_1_images[pokemon] = gtk_image_new_from_file(team_1_image_path);
			team_2_images[pokemon] = gtk_image_new_from_file(team_2_image_path);
			gtk_grid_attach(GTK_GRID(grid), team_1_images[pokemon],pokemon,1,1,1);
			gtk_grid_attach(GTK_GRID(grid), team_2_images[pokemon],pokemon,5,1,1);
			char pokemon_1_battle_time[50];
			char pokemon_2_battle_time[50];
			sprintf(pokemon_1_battle_time, "%.2lfs", arena->pokemon_data_list[pokemon].pokemon->battle_time);
			sprintf(pokemon_2_battle_time, "%.2lfs", arena->pokemon_data_list[pokemon+3].pokemon->battle_time);
			GtkWidget* team_1_pokemon_stats = gtk_label_new(pokemon_1_battle_time);
			GtkWidget* team_2_pokemon_stats = gtk_label_new(pokemon_2_battle_time);
			gtk_grid_attach(GTK_GRID(grid), team_1_pokemon_stats,pokemon,2,1,1);
			gtk_grid_attach(GTK_GRID(grid), team_2_pokemon_stats,pokemon,6,1,1);
		}
		//label del tiempo total de batalla
		char arena_battle_string[50];
		sprintf(arena_battle_string, "TOTAL BATTLE TIME: %.2lfs", arena->total_battle_time);
		GtkWidget* arena_battle_time_label = gtk_label_new(arena_battle_string);
		gtk_grid_attach(GTK_GRID(grid), arena_battle_time_label, 0,8,3,1);

		//se agrega el grid al contenido del dialog window
		gtk_box_pack_end(GTK_BOX(content_area),grid,FALSE,FALSE,0);
		arena->winner = NULL;
		//conectar el destoy de esta ventana con la de simulacion
		g_signal_connect_swapped(stats_window, "destroy", G_CALLBACK(gtk_widget_destroy),(gpointer)fight_window);
		gtk_widget_show_all(stats_window);
		return FALSE; //para matar al hilo
	}
	return TRUE;

}

void* ui_stop_fight(void* args)
{
	g_source_remove(fight_call_id);
	arena->is_running = false;
	arena->winner = NULL;
	if(arena->pokemon_data_list)
	{
		arena_delete_pokemons();
	}
}

void ui_start_button(GtkApplication* app, gpointer user_data)
{
	GtkWidget* button = (GtkWidget*) user_data;
	gtk_widget_destroy(GTK_WIDGET(button));
	//LLAMAR INICIAR BATALLA
    GThread* fight_thread = g_thread_new("fight_thread",arena_start_fight,NULL);
}

void ui_show_fight_window(GtkApplication* app, gpointer user_data)
{
	if(arena_ready())
	{
		GtkWidget* parent = (GtkWidget*) user_data;
		//se esconde al main window
		gtk_widget_hide(GTK_WIDGET(parent));

		GtkWidget* fight_window;
		GtkWidget* layout;
		GtkWidget* background_image;
		GtkWidget* trainer_1_image;
		GtkWidget* trainer_2_image;
		GtkWidget* start_button;
		GtkWidget* end_button;

		fight_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		gtk_window_set_title (GTK_WINDOW (fight_window), "Pthreadmon - Battle");
	    gtk_window_set_default_size (GTK_WINDOW (fight_window), 512, 288);
		gtk_window_set_resizable (GTK_WINDOW (fight_window), FALSE);
		gtk_window_set_position(GTK_WINDOW(fight_window), GTK_WIN_POS_CENTER);

		//layout
		layout = gtk_layout_new(NULL,NULL);
		gtk_container_add (GTK_CONTAINER (fight_window),layout);
		//imagenes
		background_image = gtk_image_new_from_file("sprites/background/fight_background.png");
		trainer_1_image = gtk_image_new_from_animation (gdk_pixbuf_animation_new_from_file ("sprites/gif/trainer_1.gif",NULL));
		trainer_2_image = gtk_image_new_from_animation (gdk_pixbuf_animation_new_from_file ("sprites/gif/trainer_2.gif",NULL));

		//boton
		start_button = gtk_button_new_with_label("START");
		g_signal_connect_swapped(GTK_BUTTON(start_button), "clicked", G_CALLBACK(ui_start_button), (gpointer) start_button);
		end_button = gtk_button_new_with_label("STOP");
		g_signal_connect(GTK_BUTTON(end_button), "clicked", G_CALLBACK(ui_stop_fight), NULL);
		g_signal_connect_swapped(GTK_BUTTON(end_button), "clicked", G_CALLBACK(gtk_widget_destroy),(gpointer)fight_window);
		g_signal_connect_swapped(GTK_WINDOW(fight_window), "destroy", G_CALLBACK(gtk_widget_destroy),(gpointer)end_button);
		//se agregan las imagenes y boton
		gtk_layout_put(GTK_LAYOUT(layout), background_image, 0, 0);
		gtk_layout_put(GTK_LAYOUT(layout), trainer_1_image, -50, 120);
		gtk_layout_put(GTK_LAYOUT(layout), trainer_2_image, 400, 0);
			//botones
		gtk_layout_put(GTK_LAYOUT(layout), start_button, 342, 248);
		gtk_layout_put(GTK_LAYOUT(layout), end_button, 427, 248);

		//size request de los botones
		gtk_widget_set_size_request(start_button, 80, 35);
		gtk_widget_set_size_request(end_button, 80, 35);

		//para el fight draw
		PATH(pokemon_1_path, get_pokemon_species_name(arena->team_1[0]) );
    	PATH(pokemon_2_path, get_pokemon_species_name(arena->team_2[0]) );
		//imagenes de los pokemones
		team_1_attacker_image = gtk_image_new_from_file(pokemon_1_path);
		team_2_attacker_image = gtk_image_new_from_file(pokemon_2_path);
		//nombres de los pokemones
		team_1_name = gtk_label_new(get_pokemon_species_name(arena->team_1[0]));
		team_2_name = gtk_label_new(get_pokemon_species_name(arena->team_2[0]));
		//vida de los pokemones
		team_1_health_bar = gtk_level_bar_new_for_interval(0,1500);
		team_2_health_bar = gtk_level_bar_new_for_interval(0,1500);
		gtk_level_bar_set_value(GTK_LEVEL_BAR(team_1_health_bar), 1500);
		gtk_level_bar_set_value(GTK_LEVEL_BAR(team_2_health_bar), 1500);
		gtk_widget_set_size_request(team_1_health_bar, 100, 7);
		gtk_widget_set_size_request(team_2_health_bar, 100, 7);
		//energia de los pokemones
		team_1_energy_bar = gtk_level_bar_new_for_interval(0,100);
		team_2_energy_bar = gtk_level_bar_new_for_interval(0,100);
		gtk_level_bar_set_value(GTK_LEVEL_BAR(team_1_energy_bar), 0);
		gtk_level_bar_set_value(GTK_LEVEL_BAR(team_2_energy_bar), 0);
		gtk_widget_set_size_request(team_1_energy_bar, 70, 4);
		gtk_widget_set_size_request(team_2_energy_bar, 70, 4);
		//se ponen en el layout
		gtk_layout_put(GTK_LAYOUT(layout), team_1_attacker_image, 175, 240);
		gtk_layout_put(GTK_LAYOUT(layout), team_2_attacker_image, 350, 120);
		gtk_layout_put(GTK_LAYOUT(layout), team_1_health_bar, 155, 230);
		gtk_layout_put(GTK_LAYOUT(layout), team_2_health_bar, 330, 110);
		gtk_layout_put(GTK_LAYOUT(layout), team_1_energy_bar, 155, 220);
		gtk_layout_put(GTK_LAYOUT(layout), team_2_energy_bar, 330, 100);
		gtk_layout_put(GTK_LAYOUT(layout), team_1_name, 155, 200);
		gtk_layout_put(GTK_LAYOUT(layout), team_2_name, 330, 80);
		//llamar a la funcion de draw, se guarda el ID para matar el proceso.
		fight_call_id = g_timeout_add(33, (GSourceFunc)ui_draw_fight, (gpointer) fight_window);

		gtk_widget_show_all(fight_window);
		g_signal_connect_swapped(G_OBJECT(fight_window), "destroy", G_CALLBACK(gtk_widget_show), GTK_WIDGET(parent));
	}
	else //mensaje de error
	{
		GtkWidget* not_ready_window;
		GtkWidget* close_button;
		GtkWidget* content_area;
		GtkWidget* not_ready_label;
		not_ready_window = gtk_dialog_new();
		gtk_window_set_title (GTK_WINDOW (not_ready_window), "Pthreadmon - Trainers not ready!");
		gtk_window_set_resizable (GTK_WINDOW (not_ready_window), FALSE);

		close_button = gtk_dialog_add_button (GTK_DIALOG(not_ready_window), "OK", 0);
		g_signal_connect_swapped(close_button, "clicked", G_CALLBACK(gtk_widget_destroy),(gpointer)not_ready_window);

		content_area = gtk_dialog_get_content_area(GTK_DIALOG(not_ready_window));

		not_ready_label = gtk_label_new("\n\n	Both trainers must choose 3 pokemons to start the battle.	\n\n");
		gtk_label_set_justify(GTK_LABEL(not_ready_label), GTK_JUSTIFY_CENTER);
		gtk_box_pack_end(GTK_BOX(content_area),not_ready_label,FALSE,FALSE,0);
		gtk_widget_show_all(not_ready_window);
	}

}
