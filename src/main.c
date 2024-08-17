#include "model/arena.h"
#include "view/main_window.h"

extern arena_t* arena;

int main (int argc, char *argv[])
{
	arena = arena_create();
	GtkApplication* app;
	int status;

	app = gtk_application_new("gtk.pthreadmon",G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(ui_show_trainer_window), NULL);
	status = g_application_run(G_APPLICATION (app), argc, argv);
	g_object_unref(app);
	return status;
}
