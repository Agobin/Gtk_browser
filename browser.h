#ifndef BROWSER_H
#define BROWSER_H            

#include <gtk/gtk.h>
#include <webkit/webkit.h>
#include "browser.h"
#include <string.h>


//Functions involved in building a browser
void destroy( GtkWidget* window, gpointer data);
gboolean delete_event( GtkWidget* window, GdkEvent* event, gpointer data);
void setTapUp(GtkWidget* notebook, gpointer data);
void setTapDown(GtkWidget* notebook, gpointer data);
void setTapLeft(GtkWidget* notebook, gpointer data);
void setTapRight(GtkWidget* notebook, gpointer data);
void delete_tab( GtkWidget* browserForm, gpointer data);
void addNewTab( GtkWidget* browserForm, gpointer data);
GtkWidget * setPageLayout(GtkWidget* window, GtkWidget* browserForm);
void logoClicked( GtkWidget*eventBox, GdkEvent *event, gpointer textBox);
void facebookLogoClicked( GtkWidget*eventBox, GdkEvent *event, gpointer textBox);
void skypeLogoClicked( GtkWidget*eventBox, GdkEvent *event, gpointer textBox);
void gmailLogoClicked( GtkWidget*eventBox, GdkEvent *event, gpointer textBox);
void twitterLogoClicked( GtkWidget*eventBox, GdkEvent *event, gpointer textBox);
void googleLogoClicked( GtkWidget*eventBox, GdkEvent *event, gpointer textBox);
GtkWidget * initialiseEventBox( GtkWidget *childWidget);
gboolean clearHistory(GtkWidget * button, gpointer data);
void viewHistory(GtkWidget *webViewX, gpointer button);
GtkListStore * initialiseEntryCompletionList( void);
void openFile(GtkWidget * webViewX, gpointer button);

void viewSearchedPage( GtkWidget *expander, gpointer button);

//These functions make the browser surf the internet, displaying webpages
void go(GtkWidget* webViewX, gpointer button);
void back(GtkWidget* webview, gpointer button);
void forward(GtkWidget* web, gpointer button);
void stop(GtkWidget* webview, gpointer button);
void activate(GtkWidget* web_view, gpointer searchBox);
void progress (WebKitWebView *web, gint progress,gpointer* data);
void finished (WebKitWebView  *web_view, WebKitWebFrame *frame, gpointer*  data);
void setUri( GtkWidget *entry, gpointer button);

#endif
