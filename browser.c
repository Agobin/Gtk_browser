#include "browser.h"      

//Global variables to keep track of the number of opened tabs
gint NUMBER_OF_TABS = 1;
//GtkWidget *webView; 

const gchar * uri;

//The destroy and delete_event functions take charge of closing the main window i.e exiting the gtk_main() loop
 void destroy( GtkWidget* window, gpointer data){
	gtk_main_quit();
}
 gboolean delete_event( GtkWidget* window, GdkEvent* event, gpointer data){
	return FALSE;
}

void setUri( GtkWidget *mainSearchBox, gpointer button){
	//Checking if the web address already has http attached to if, if not it is attached to the address
	uri = gtk_entry_get_text(GTK_ENTRY(mainSearchBox));

	//These if constructs ensure that the url is in order
	//If address is online or on user's machine
	if( (uri[0] == 'h' && uri[1] == 't' && uri[2] == 't' && uri[3] == 'p') || (uri[0] == 'f' && uri[1] == 'i' && uri[2] == 'l' && uri[3] == 'e')){
		//The address is in the correct format
		uri = gtk_entry_get_text(GTK_ENTRY(mainSearchBox));
	}
	else{

		if( uri[0] == 'w' && uri[1] == 'w' && uri[2] == 'w'){
			//address lacks http://, which is added
			gtk_entry_prepend_text(GTK_ENTRY(mainSearchBox),"http://");
			uri = gtk_entry_get_text(GTK_ENTRY(mainSearchBox));
		}
		else{
			
			if(  strstr(uri, ".com") && uri[0] != '.'){
				//if address ends with .com like youtube.com
				gtk_entry_prepend_text(GTK_ENTRY(mainSearchBox),"http://www.");
				uri = gtk_entry_get_text(GTK_ENTRY(mainSearchBox));
			}
			else{
				//if address has none of the above, google is used to search for the user's text
				gtk_entry_prepend_text(GTK_ENTRY(mainSearchBox),"http://www.google.com/");
				uri = gtk_entry_get_text(GTK_ENTRY(mainSearchBox));
			}
		}
	}
	gtk_entry_progress_pulse(GTK_ENTRY(mainSearchBox));
}

//Functions that take the browser to the web
void go(GtkWidget* webViewX, gpointer button){
	webkit_web_view_load_uri(WEBKIT_WEB_VIEW(webViewX),uri);
	gtk_window_set_title(GTK_WINDOW(webViewX->parent->parent->parent->parent->parent->parent), uri);
	system("mkdir -p History");
	//Saves uri in the history file
	FILE *historyPtr = fopen("History/History.htm","a");
	fprintf(historyPtr, "\n%s",uri);
	fclose(historyPtr);
}

void back(GtkWidget* webViewX, gpointer button){

	webkit_web_view_go_back(WEBKIT_WEB_VIEW(webViewX));
}

void forward(GtkWidget* webViewX, gpointer button){
	
	webkit_web_view_go_forward(WEBKIT_WEB_VIEW(webViewX));
}

void stop(GtkWidget* webViewX, gpointer button){

	webkit_web_view_stop_loading(WEBKIT_WEB_VIEW(webViewX));
}

void activate(GtkWidget* web_view, gpointer searchBox){

	uri = gtk_entry_get_text(GTK_ENTRY(searchBox));
	gtk_entry_progress_pulse(GTK_ENTRY(searchBox));
	webkit_web_view_load_uri(WEBKIT_WEB_VIEW(web_view),uri);
}

void progress (WebKitWebView *web, gint progress, gpointer* data){

	gtk_entry_set_progress_fraction (GTK_ENTRY(data),progress);
}

void finished (WebKitWebView  *web_view, WebKitWebFrame *frame, gpointer*  data){

	gtk_entry_set_progress_fraction (GTK_ENTRY(data),0.0);
}
//End of webkit fxns


//These set of functions vary the position of the tab
 void setTapUp(GtkWidget* notebook, gpointer data){
	gtk_notebook_set_tab_pos( GTK_NOTEBOOK(notebook), GTK_POS_TOP);
}
  void setTapDown(GtkWidget* notebook, gpointer data){
	gtk_notebook_set_tab_pos( GTK_NOTEBOOK(notebook), GTK_POS_BOTTOM);
}

//This function deletes the current browser notebook page each time its called
  void delete_tab( GtkWidget* browserForm, gpointer data){
	gint pageNum;
	pageNum = gtk_notebook_get_current_page( GTK_NOTEBOOK(browserForm));

	//If there exist more than one tab it deletes else it notifies the user of the current status of only one tab
	if( NUMBER_OF_TABS > 1){
		gtk_notebook_remove_page( GTK_NOTEBOOK(browserForm), pageNum);
		//Number of tabs decrease by one each time a tab is deleted
		NUMBER_OF_TABS--;
	}
	else{
		GtkWidget * window, *label;
		window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

		gtk_window_set_position( GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);
		gtk_window_set_title(GTK_WINDOW(window), "Operation cannot be allowed");
		gtk_window_set_default_size( GTK_WINDOW(window), 500, 200);
		gtk_window_set_resizable( GTK_WINDOW(window), FALSE);
		gtk_widget_set_size_request( window, 500, 300);

		g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK( gtk_widget_hide), (gpointer) window);

		label = gtk_label_new("Current tab cannot be deleted\n\nThis is the only remaining tab");
		gtk_container_add( GTK_CONTAINER(window), label);
		gtk_widget_show_all(window);
	}

}

//This function adds a new tab to the browser notebook each time its called
  void addNewTab( GtkWidget* browserForm, gpointer data){
	GtkWidget *tableInitialing , *cancelButton, *addButton, *hboxHeader, *hboxLabel;
	gint currentPageNum;
	gint nextPageNum;

	//Number of tabs increase by one each time a tab is added
	NUMBER_OF_TABS++;
	
	hboxHeader = gtk_hbox_new( TRUE, 0);
	gtk_widget_set_size_request( hboxHeader, 200, 20);

	hboxLabel = gtk_label_new("New tab");
	gtk_widget_set_size_request(hboxLabel, 95, 20);
	gtk_box_pack_start_defaults( GTK_BOX(hboxHeader), hboxLabel);

	cancelButton = gtk_button_new_with_mnemonic("_x");
	gtk_button_set_relief( GTK_BUTTON( cancelButton), GTK_RELIEF_NONE);
	//Connects signal related to close button
	g_signal_connect_swapped( G_OBJECT(cancelButton), "clicked", G_CALLBACK(delete_tab), (gpointer) browserForm);
	gtk_box_pack_start_defaults( GTK_BOX(hboxHeader), cancelButton);

	addButton = gtk_button_new_with_mnemonic("_+");
	gtk_button_set_relief( GTK_BUTTON( addButton), GTK_RELIEF_NONE);
	gtk_widget_set_size_request( addButton, 20, 20);
	//Connects the addButton to its callback function when clicked
	g_signal_connect_swapped( G_OBJECT(addButton), "clicked", G_CALLBACK(addNewTab), (gpointer) browserForm);
	gtk_box_pack_start_defaults( GTK_BOX(hboxHeader), addButton);

	gtk_widget_show_all(hboxHeader);
	
	//Creates a table of 30 rows and 17 columns
	tableInitialing = setPageLayout(browserForm->parent, browserForm);

	currentPageNum = gtk_notebook_get_current_page( GTK_NOTEBOOK(browserForm));
	nextPageNum = currentPageNum + 1;
	gtk_notebook_insert_page( GTK_NOTEBOOK(browserForm), tableInitialing, hboxHeader, nextPageNum + 1);
	gtk_notebook_next_page( GTK_NOTEBOOK(browserForm));
	gtk_widget_show_all(browserForm);
}

//This function returns a table containing the elements of the page arranged
GtkWidget * setPageLayout(GtkWidget* window, GtkWidget* browserForm){

	GtkWidget * tableInitialing;
	GtkWidget *image;          
	GtkWidget * mainSearchBox, *miniSearchBox, *centerSearchBox; 
	GtkWidget *searchMainBut, *searchMiniBut, *searchCenterBut, *exitButton , *goBackBut, *stopBut, *frontBut;
	GtkWidget *scrolledWindow;
	GtkWidget *webView;
	GtkWidget *pageLayoutExpander;

	webView = webkit_web_view_new();
	pageLayoutExpander = gtk_expander_new_with_mnemonic("V_iew page");

	//creates a table with 20 rows and 17 columns
	tableInitialing = gtk_table_new( 30, 17, TRUE);
	
	exitButton = gtk_button_new_with_mnemonic("E_xit One click browser");
	g_signal_connect_swapped( G_OBJECT(exitButton), "clicked", G_CALLBACK(gtk_widget_destroy), (gpointer) window);
	gtk_table_attach_defaults( GTK_TABLE(tableInitialing), exitButton, 0, 17, 29,30);



	//Start of expander setting
	//declaration of variables involved in setting position of tabs
	GtkWidget *settingExpander, *expanderVBox;
	GtkWidget *setTapUpRadio, *setTapDownRadio, *clearHistoryBut, *viewHistoryBut;
	GtkWidget *facebookLogo, *twitterLogo, *gmailLogo, *skypeLogo, *googleLogo;
	GtkWidget *facebookEventBox, *twitterEventBox, *gmailEventBox, *skypeEventBox, *googleEventBox;

	//Initialising variables
	setTapUpRadio = gtk_radio_button_new_with_label(NULL,"Set tab position up");
	setTapDownRadio = gtk_radio_button_new_with_label_from_widget( GTK_RADIO_BUTTON(setTapUpRadio), "Set tab position down");
	
	//Initialises the clearHistoryBut and associate a callback to it
	clearHistoryBut = gtk_button_new_with_mnemonic("_Clear history");
	g_signal_connect(G_OBJECT(clearHistoryBut), "clicked", G_CALLBACK(clearHistory), NULL);

	viewHistoryBut = gtk_button_new_with_mnemonic("_View history");
	g_signal_connect_swapped(G_OBJECT(viewHistoryBut),"clicked", G_CALLBACK(viewHistory), NULL);


	//Initialises the expander and the vbox
	settingExpander = gtk_expander_new_with_mnemonic("_Settings");
	expanderVBox = gtk_vbox_new(TRUE, 0);

	//Adds radio buttons to the vbox
	gtk_box_pack_start_defaults( GTK_BOX(expanderVBox), setTapUpRadio);
	gtk_box_pack_start_defaults( GTK_BOX(expanderVBox), setTapDownRadio);
	gtk_box_pack_start_defaults( GTK_BOX(expanderVBox), clearHistoryBut);
	gtk_box_pack_start_defaults( GTK_BOX(expanderVBox), viewHistoryBut);

	//Linking signals of radio buttons to the respective callbacks
	g_signal_connect_swapped( G_OBJECT(setTapUpRadio), "toggled", G_CALLBACK(setTapUp), browserForm);
	g_signal_connect_swapped( G_OBJECT(setTapDownRadio), "toggled", G_CALLBACK(setTapDown), browserForm);

	gtk_container_add( GTK_CONTAINER(settingExpander), expanderVBox);
	gtk_table_attach_defaults( GTK_TABLE(tableInitialing),settingExpander, 0,3,20,24);
	//End of expander setting 


	goBackBut = gtk_button_new_with_label(" <-- ");
	gtk_widget_set_size_request(goBackBut, 10, 25);
	gtk_table_attach_defaults( GTK_TABLE(tableInitialing), goBackBut, 1, 2, 1, 2);
	g_signal_connect_swapped(goBackBut, "clicked",  G_CALLBACK(back), webView);

	stopBut = gtk_button_new_with_label(" X ");
	gtk_widget_set_size_request(stopBut, 10, 25);
	gtk_table_attach_defaults( GTK_TABLE(tableInitialing), stopBut, 2, 3, 1, 2);
	g_signal_connect_swapped(stopBut, "clicked",  G_CALLBACK(stop), webView);

	frontBut = gtk_button_new_with_label(" --> ");
	gtk_widget_set_size_request(frontBut, 10, 25);
	gtk_table_attach_defaults( GTK_TABLE(tableInitialing), frontBut, 16, 17, 1, 2);
	g_signal_connect_swapped(frontBut, "clicked",  G_CALLBACK(forward), webView);

	//This patch of code initiates autocompletion of the search box
	GtkEntryCompletion *completion;
	GtkTreeIter *iter;
	GtkListStore *store_list;
	FILE * historyPtr;

	store_list = gtk_list_store_new(1,G_TYPE_STRING);
	completion = gtk_entry_completion_new();

	if( (historyPtr = fopen("History/History.htm","r+")) == NULL)
		g_print("Error");
	
	gchar historyString[100];
	
	while( !feof(historyPtr)){
		fscanf(historyPtr,"%s",historyString);
		gtk_list_store_append(store_list, iter);
		gtk_list_store_set(store_list, iter, 0, historyString, -1);
	}
	fclose(historyPtr);
	gtk_entry_completion_set_model(completion, GTK_TREE_MODEL(store_list));
	gtk_entry_completion_set_text_column(completion, 0);

	mainSearchBox = gtk_entry_new();
	gtk_entry_set_completion(GTK_ENTRY(mainSearchBox), completion);
	gtk_widget_set_size_request(mainSearchBox, 500, 25);
	gtk_table_attach_defaults( GTK_TABLE(tableInitialing), mainSearchBox, 3, 15, 1, 2 );
	g_signal_connect_swapped(mainSearchBox, "activate",  G_CALLBACK(activate), webView);

	searchMainBut = gtk_button_new_with_mnemonic("Sear_ch");
	gtk_widget_set_size_request(searchMainBut, 10,15);
	gtk_table_attach_defaults( GTK_TABLE(tableInitialing), searchMainBut, 15, 16, 1, 2 );
	g_signal_connect_swapped(searchMainBut, "clicked", G_CALLBACK(setUri), mainSearchBox);
	g_signal_connect_swapped(searchMainBut, "clicked",  G_CALLBACK(go), webView);
	//g_signal_connect_swapped(searchMainBut, "clicked",  G_CALLBACK(initialiseEntryCompletion), completion);

	//Linking callbacks to signals emitted by webview widget
	g_signal_connect(WEBKIT_WEB_VIEW(webView),"load-progress-changed",G_CALLBACK(progress),mainSearchBox);
	g_signal_connect(WEBKIT_WEB_VIEW(webView),"load-finished",G_CALLBACK(finished),mainSearchBox);
	
	scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy( GTK_SCROLLED_WINDOW(scrolledWindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_add_with_viewport( GTK_SCROLLED_WINDOW(scrolledWindow), webView);
	gtk_widget_show_all(scrolledWindow);

	//PageLayout expander
	//gtk_widget_set_sensitive(pageLayoutExpander, FALSE);
	gtk_container_add( GTK_CONTAINER(pageLayoutExpander), scrolledWindow);
	gtk_table_attach_defaults(GTK_TABLE(tableInitialing), pageLayoutExpander, 0, 17, 2, 29);
	g_signal_connect_swapped(WEBKIT_WEB_VIEW(webView),"load-finished",G_CALLBACK(viewSearchedPage),pageLayoutExpander);

	image = gtk_image_new_from_file("images/OneClickLogo.png"); 
	GtkWidget* imageEventBox = initialiseEventBox(image);
	g_signal_connect( G_OBJECT(imageEventBox), "button_press_event", G_CALLBACK(logoClicked), mainSearchBox);
	gtk_table_attach_defaults(GTK_TABLE(tableInitialing), imageEventBox, 3, 14, 14, 24);

	//Adding pictures of some social media to the browser's home page
	facebookLogo = gtk_image_new_from_file("images/facebook.jpg");
	skypeLogo = gtk_image_new_from_file("images/skype.jpg");
	twitterLogo = gtk_image_new_from_file("images/twitter.jpg");
	gmailLogo = gtk_image_new_from_file("images/gmail.jpg");
	googleLogo = gtk_image_new_from_file("images/google.jpg");

	//Adding event handlers to images
	facebookEventBox = initialiseEventBox(facebookLogo);
	g_signal_connect( G_OBJECT(facebookEventBox), "button_press_event", G_CALLBACK(facebookLogoClicked), mainSearchBox);
	gtk_table_attach_defaults(GTK_TABLE(tableInitialing), facebookEventBox, 4, 6, 24, 28 );

	gmailEventBox = initialiseEventBox(gmailLogo);
	g_signal_connect( G_OBJECT(gmailEventBox), "button_press_event", G_CALLBACK(gmailLogoClicked), mainSearchBox);
	gtk_table_attach_defaults(GTK_TABLE(tableInitialing), gmailEventBox, 6, 8, 24, 28 );

	twitterEventBox = initialiseEventBox(twitterLogo);
	g_signal_connect( G_OBJECT(twitterEventBox), "button_press_event", G_CALLBACK(twitterLogoClicked), mainSearchBox);
	gtk_table_attach_defaults(GTK_TABLE(tableInitialing), twitterEventBox, 8, 10, 24, 28 );

	skypeEventBox = initialiseEventBox(skypeLogo);
	g_signal_connect( G_OBJECT(skypeEventBox), "button_press_event", G_CALLBACK(skypeLogoClicked), mainSearchBox);
	gtk_table_attach_defaults(GTK_TABLE(tableInitialing), skypeEventBox, 10, 12, 24, 28 );

	googleEventBox = initialiseEventBox(googleLogo);
	g_signal_connect( G_OBJECT(googleEventBox), "button_press_event", G_CALLBACK(googleLogoClicked), mainSearchBox);
	gtk_table_attach_defaults(GTK_TABLE(tableInitialing), googleEventBox, 12, 14, 24, 28 );

	//This patch of code implements the file chooser button that opens files on the user's computer
	GtkWidget *openFileChooser;
	openFileChooser = gtk_file_chooser_button_new("Select an html document to open",GTK_FILE_CHOOSER_ACTION_OPEN);
	g_signal_connect_swapped(G_OBJECT(openFileChooser), "selection_changed", G_CALLBACK(openFile), webView);
	//gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(openFileChooser), g_get_home_dir());
	gtk_table_attach_defaults(GTK_TABLE(tableInitialing), openFileChooser, 4, 12, 0, 1 );
	
	GtkFileFilter *filter;
	filter = gtk_file_filter_new();
	gtk_file_filter_set_name (filter, "HTML Files");
	gtk_file_filter_add_pattern (filter, "*.html");
	gtk_file_filter_add_pattern (filter, "*.htm");
	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (openFileChooser), filter);
	

	return tableInitialing;
}

//Opens an html or an htm document on the user's machine
void openFile(GtkWidget * webViewX, gpointer chooserBut){

	gchar *file, *url;

	file = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (chooserBut));
	//Concatenates the filename with 'file://'
	url = g_strconcat("file://", file, NULL, NULL); 
	webkit_web_view_load_uri(WEBKIT_WEB_VIEW(webViewX), url);
	gtk_window_set_title(GTK_WINDOW(webViewX->parent->parent->parent->parent->parent->parent), file);
	gtk_widget_show(webViewX);
}
//This function initialises the completion list
GtkListStore * initialiseEntryCompletionList( void){
	
	GtkTreeIter *iter;
	GtkListStore *store_list;
	FILE * historyPtr;

	store_list = gtk_list_store_new(1,G_TYPE_STRING);

	if( (historyPtr = fopen("History/History.htm","r+")) == NULL)
		g_print("Error");
	
	gchar historyString[100];
	
	while( !feof(historyPtr)){
		fscanf(historyPtr,"%s",historyString);
		gtk_list_store_append(store_list, iter);
		gtk_list_store_set(store_list, iter, 0, historyString, -1);
	}
	fclose(historyPtr);
	return store_list;
}

//This function attaches event to one click logo
void logoClicked( GtkWidget*eventBox, GdkEvent *event, gpointer textBox){
	gtk_entry_set_text( GTK_ENTRY(textBox), "www.oneclick.com");
	gtk_widget_show(textBox);
}

void facebookLogoClicked( GtkWidget*eventBox, GdkEvent *event, gpointer textBox){
	gtk_entry_set_text( GTK_ENTRY(textBox), "www.facebook.com");
	gtk_widget_show(textBox);
}
void twitterLogoClicked( GtkWidget*eventBox, GdkEvent *event, gpointer textBox){
	gtk_entry_set_text( GTK_ENTRY(textBox), "www.twitter.com");
	gtk_widget_show(textBox);
}
void gmailLogoClicked( GtkWidget*eventBox, GdkEvent *event, gpointer textBox){
	gtk_entry_set_text( GTK_ENTRY(textBox), "www.gmail.com");
	gtk_widget_show(textBox);
}
void skypeLogoClicked( GtkWidget*eventBox, GdkEvent *event, gpointer textBox){
	gtk_entry_set_text( GTK_ENTRY(textBox), "www.skype.com");
	gtk_widget_show(textBox);
}
void googleLogoClicked( GtkWidget*eventBox, GdkEvent *event, gpointer textBox){
	gtk_entry_set_text( GTK_ENTRY(textBox), "www.google.com");
	gtk_widget_show(textBox);
}

GtkWidget * initialiseEventBox( GtkWidget *childWidget){
	GtkWidget* eventBox;
	eventBox = gtk_event_box_new();

	gtk_event_box_set_above_child( GTK_EVENT_BOX(eventBox), FALSE);
	gtk_widget_set_events(eventBox, GDK_BUTTON_PRESS_MASK);
	gtk_container_add( GTK_CONTAINER(eventBox), childWidget);

	return eventBox;
}
void viewSearchedPage( GtkWidget *expander, gpointer button){
	gtk_expander_set_expanded( GTK_EXPANDER(expander), TRUE);
}
//This function clears history of the browser
gboolean clearHistory(GtkWidget * button, gpointer data){
	system("mkdir -p History");
	FILE * historyPtr = fopen("History/History.htm","w");
	fclose(historyPtr);
}

void viewHistory(GtkWidget *webViewX, gpointer button){
	system("gedit History/History.htm");
}