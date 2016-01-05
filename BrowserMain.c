#include "browser.h"
       
int main(int argc, char* argv[]){

	//Declaration of variables
	GtkWidget * window, *tableInitialing, *hboxHeader, *hboxLabel;
	GtkWidget * browserForm, *cancelButton, *addButton, *addButtonTest;


	//This function parses the command line arguments, removing the wants it recognizes
	gtk_init( &argc, &argv);

	//includes the resource file to modify GtkWidgets
	gtk_rc_parse("resourceFile.gtkrc");

	//initialises the form notebook
	browserForm = gtk_notebook_new();
	
	//This line of code sets the properties of the main window
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size( GTK_WINDOW(window), 1000, 700);
	gtk_widget_set_size_request( window, 900, 600);
	gtk_container_set_border_width( GTK_CONTAINER(window), 10);        
	gtk_window_set_title(GTK_WINDOW(window), "One click browser   Made to ease browsing");
	gtk_window_set_resizable( GTK_WINDOW(window), TRUE);
	gtk_window_set_icon_from_file( GTK_WINDOW(window), "images/OneClickLogo.png", NULL);

	//Connects signals relating to the window
	g_signal_connect( G_OBJECT(window), "destroy", G_CALLBACK(destroy), NULL);
	g_signal_connect( G_OBJECT(window), "delete_event", G_CALLBACK(delete_event), NULL);

	//Initialising the addButton and linking it with its callback function when clicked
	addButton = gtk_button_new_with_mnemonic("_+");
	gtk_button_set_relief( GTK_BUTTON(addButton), GTK_RELIEF_NONE);
	g_signal_connect_swapped( G_OBJECT(addButton), "clicked", G_CALLBACK(addNewTab), (gpointer) browserForm);


	//Initialises the hbox-It contains the addButton, cancelButton and the hboxLabel
	hboxHeader = gtk_hbox_new( TRUE, 0);
	gtk_widget_set_size_request( hboxHeader, 200, 20);

	hboxLabel = gtk_label_new("New tab");
	gtk_widget_set_size_request( hboxLabel, 95, 20);

	
	//Initialising the cancelButton and linking it with its callback function when clicked
	cancelButton = gtk_button_new_with_mnemonic("_x");
	gtk_button_set_relief( GTK_BUTTON(cancelButton), GTK_RELIEF_NONE);
	g_signal_connect_swapped( G_OBJECT(cancelButton), "clicked", G_CALLBACK(delete_tab), (gpointer) browserForm);

	//Adding widgets to the hbox
	gtk_box_pack_start_defaults( GTK_BOX(hboxHeader), hboxLabel);
	gtk_box_pack_start_defaults( GTK_BOX(hboxHeader), cancelButton);
	gtk_box_pack_start_defaults( GTK_BOX(hboxHeader), addButton);
	gtk_widget_show_all(hboxHeader);
 
 	//setPageLayout initialises the table and then table is then added to the browser notebook
	tableInitialing = setPageLayout(window, browserForm);
	gtk_notebook_append_page( GTK_NOTEBOOK(browserForm), tableInitialing, hboxHeader);

	gtk_notebook_set_scrollable( GTK_NOTEBOOK(browserForm), TRUE);
	gtk_container_add(GTK_CONTAINER(window), browserForm);

	gtk_widget_show_all(window); 
	gtk_main();
	return 0;
} //End of main function