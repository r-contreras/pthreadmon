#include "main_window.h"

void ui_set_trainer_1_name(GtkWidget * widget, gpointer callback_data)
{
	trainer_1_name = (gchar*) gtk_entry_buffer_get_text(trainer_1_buffer);
	//Si esta vacio entonces se va Trainer 1 por default
	if (gtk_entry_buffer_get_length(trainer_1_buffer) == 0)
		trainer_1_name = "Trainer 1";
	g_print ("Trainer 1 name: %s\n", trainer_1_name);
	arena_set_trainer_1_name(trainer_1_name);
}

void ui_set_trainer_2_name(GtkWidget * widget, gpointer callback_data)
{
	trainer_2_name = (gchar*) gtk_entry_buffer_get_text(trainer_2_buffer);
	//Si esta vacio entonces se va Trainer 2 por default
	if (gtk_entry_buffer_get_length(trainer_1_buffer) == 0)
		trainer_2_name = "Trainer 2";
	g_print ("Trainer 2 name: %s\n", trainer_2_name);
	arena_set_trainer_2_name(trainer_2_name);
}

void ui_show_trainer_window(GtkApplication* app, gpointer user_data)
{
	GtkWidget* main_window;
	GtkWidget* layout;
	GtkWidget* trainer_1_entry;
	GtkWidget* trainer_2_entry;
	GtkWidget* set_name_button_1;
	GtkWidget* set_name_button_2;
	GtkWidget* start_button;
	GtkWidget* background_image;
	GtkWidget* start_button_image;
	GtkWidget* trainer_1_image;
	GtkWidget* trainer_2_image;
	GtkWidget* welcome_animation;
	GtkWidget* trainer_1_bag_button;
	GtkWidget* trainer_2_bag_button;

	 //se crea el window
	main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (main_window), "Pthreadmon");
    gtk_window_set_default_size (GTK_WINDOW (main_window), 450, 275);
	gtk_window_set_resizable (GTK_WINDOW (main_window), FALSE);
	gtk_window_set_position(GTK_WINDOW(main_window), GTK_WIN_POS_CENTER);

	//se agrega el layout
	layout = gtk_layout_new(NULL,NULL);
	gtk_container_add (GTK_CONTAINER (main_window),layout);

	//imagenes
	background_image = gtk_image_new_from_file("sprites/background/trainer_background.png");
	trainer_1_image = gtk_image_new_from_animation (gdk_pixbuf_animation_new_from_file ("sprites/gif/trainer_1.gif",NULL));
	trainer_2_image = gtk_image_new_from_animation (gdk_pixbuf_animation_new_from_file ("sprites/gif/trainer_2.gif",NULL));
	start_button_image = gtk_image_new_from_file("sprites/misc/vs_button.png");
	//gifs
	welcome_animation = gtk_image_new_from_animation (gdk_pixbuf_animation_new_from_file ("sprites/gif/welcome_animation.gif",NULL));

	//entry with buffer
		//buffers NOTA: ambos buffers definidos en el .h
	trainer_1_buffer = gtk_entry_buffer_new(NULL,-1);
	trainer_2_buffer = gtk_entry_buffer_new(NULL,-1);
	trainer_1_entry = gtk_entry_new_with_buffer(trainer_1_buffer);
	trainer_2_entry = gtk_entry_new_with_buffer(trainer_2_buffer);
	gtk_entry_set_placeholder_text (GTK_ENTRY (trainer_1_entry), "Name");
	gtk_entry_set_placeholder_text (GTK_ENTRY (trainer_2_entry), "Name");
	gtk_entry_set_max_length(GTK_ENTRY (trainer_1_entry), 17);
	gtk_entry_set_max_length(GTK_ENTRY (trainer_2_entry), 17);
	gtk_entry_set_alignment(GTK_ENTRY (trainer_1_entry), 0.5);
	gtk_entry_set_alignment(GTK_ENTRY (trainer_2_entry), 0.5);

	//se agregan los botones
	set_name_button_1 = gtk_button_new_with_label("READY");
	set_name_button_2 = gtk_button_new_with_label("READY");
	g_signal_connect_swapped(set_name_button_1, "clicked", G_CALLBACK(ui_set_trainer_1_name),NULL);
	g_signal_connect_swapped(set_name_button_2, "clicked", G_CALLBACK(ui_set_trainer_2_name),NULL);

		//boton de start (vs)
	start_button = gtk_button_new();
	gtk_button_set_image(GTK_BUTTON(start_button),start_button_image);
	g_signal_connect(start_button, "clicked", G_CALLBACK(ui_show_fight_window),(gpointer) main_window);
		//botones de escoger pokemones
	trainer_1_bag_button = gtk_button_new();
	trainer_2_bag_button = gtk_button_new();
	gtk_button_set_image(GTK_BUTTON(trainer_1_bag_button),gtk_image_new_from_file("sprites/misc/select_pokemon.png"));
	gtk_button_set_image(GTK_BUTTON(trainer_2_bag_button),gtk_image_new_from_file("sprites/misc/select_pokemon.png"));
	gtk_button_set_relief(GTK_BUTTON(trainer_1_bag_button), GTK_RELIEF_NONE);
	gtk_button_set_relief(GTK_BUTTON(trainer_2_bag_button), GTK_RELIEF_NONE);
	gtk_widget_set_can_focus (GTK_WIDGET(trainer_1_bag_button), false);
	gtk_widget_set_can_focus (GTK_WIDGET(trainer_2_bag_button), false);

	//paquetes de informacion
	ui_pokedex_info_t* pokedex_info_1 = malloc(sizeof(ui_pokedex_info_t));
	ui_pokedex_info_t* pokedex_info_2 = malloc(sizeof(ui_pokedex_info_t));
	pokedex_info_1->parent = pokedex_info_2->parent = main_window;
	pokedex_info_1->add_call = ui_add_pokemon_team_1; //puntero a funcion
	pokedex_info_2->add_call = ui_add_pokemon_team_2;
	pokedex_info_1->clear_call = ui_clear_pokemon_team_1; //puntero a funcion
	pokedex_info_2->clear_call = ui_clear_pokemon_team_2;
	g_signal_connect(GTK_BUTTON(trainer_1_bag_button),"clicked",G_CALLBACK(ui_create_pokedex_window), (gpointer) pokedex_info_1);
	g_signal_connect(GTK_BUTTON(trainer_2_bag_button),"clicked",G_CALLBACK(ui_create_pokedex_window), (gpointer) pokedex_info_2);
	//se agregan las imagenes y botones
	gtk_layout_put(GTK_LAYOUT(layout), background_image, 0, 0);
	gtk_layout_put(GTK_LAYOUT(layout), trainer_1_image, 50, 20);
	gtk_layout_put(GTK_LAYOUT(layout), trainer_2_image, 230, 30);
	gtk_layout_put(GTK_LAYOUT(layout), welcome_animation, 197, 120);

	gtk_layout_put(GTK_LAYOUT(layout), trainer_1_entry, 5,200);
	gtk_layout_put(GTK_LAYOUT(layout), trainer_2_entry, 275,200);

	gtk_layout_put(GTK_LAYOUT(layout), set_name_button_1, 50, 237);
	gtk_layout_put(GTK_LAYOUT(layout), set_name_button_2, 320, 237);
	gtk_layout_put(GTK_LAYOUT(layout), start_button, 197, 200);
	gtk_layout_put(GTK_LAYOUT(layout), trainer_1_bag_button, 5, 130);
	gtk_layout_put(GTK_LAYOUT(layout), trainer_2_bag_button, 351, 130);
	//size de los entries y botones
	gtk_widget_set_size_request(trainer_1_entry, 50, 35);
	gtk_widget_set_size_request(trainer_2_entry, 50, 35);

	gtk_widget_set_size_request(set_name_button_1, 80, 35);
	gtk_widget_set_size_request(set_name_button_2, 80, 35);
	gtk_widget_set_size_request(start_button, 41, 35);
	gtk_widget_set_size_request(trainer_1_bag_button, 80, 57);
	gtk_widget_set_size_request(trainer_2_bag_button, 80, 57);

	g_signal_connect_swapped(G_OBJECT(main_window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_widget_show_all(main_window);

	gtk_main();
}
