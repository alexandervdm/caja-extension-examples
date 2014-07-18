
/*
 * Copyright (C) 2014 Alexander van der Meij <alexandervdm@gliese.me>
 * All Rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <libcaja-extension/caja-extension-types.h>
#include <libcaja-extension/caja-menu-provider.h>


typedef struct {
    GObject parent;
} MenuExample;

typedef struct {
    GObjectClass parent_class;
} MenuExampleClass;


static GObjectClass *parent_class = NULL;
static GType provider_types[1];
static GType menu_example_type;


static void menu_example_register_type (GTypeModule *module);
GType menu_example_get_type (void);

static void menu_example_class_init (MenuExampleClass *class);
static void menu_example_instance_init (MenuExample *self);
static void menu_provider_iface_init (CajaMenuProviderIface *iface);

static GList * menu_example_get_file_items (CajaMenuProvider *provider,
			      GtkWidget *window,
			      GList *files);
static GList * menu_example_get_background_items (CajaMenuProvider *provider, 
                                GtkWidget *window,
                                CajaFileInfo *current_folder);
static GList *
menu_example_get_toolbar_items (CajaMenuProvider *provider, 
                             GtkWidget *window,
                             CajaFileInfo *current_folder);

/* Extension interface: */

void
caja_module_initialize (GTypeModule  *module) 
{
	menu_example_register_type (module);
	provider_types[0] = menu_example_get_type ();
}

void 
caja_module_shutdown (void) 
{
	// Any module-specific shutdown:
}

void
caja_module_list_types (const GType **types, int *num_types) 
{
	*types = provider_types;
	*num_types = G_N_ELEMENTS (provider_types);
}

static void
menu_example_register_type (GTypeModule *module) 
{
    static const GTypeInfo info = {
        sizeof (MenuExampleClass),
        (GBaseInitFunc) NULL,
        (GBaseFinalizeFunc) NULL,
        (GClassInitFunc) menu_example_class_init,
        NULL,
        NULL,
        sizeof (MenuExample),
        0,
        (GInstanceInitFunc) menu_example_instance_init,
    };

    static const GInterfaceInfo menu_provider_iface_info = {
        (GInterfaceInitFunc)menu_provider_iface_init,
        NULL,
        NULL
    };
 
	menu_example_type = g_type_module_register_type (module,
                                G_TYPE_OBJECT,
                                "MenuExample",
                                &info, 0);

    // add interface:
    g_type_module_add_interface (module,
				 menu_example_type,
				 CAJA_TYPE_MENU_PROVIDER,
				 &menu_provider_iface_info);
}

GType
menu_example_get_type (void)
{
	return menu_example_type;
}

/* Initiation functions: */

static void
menu_example_class_init (MenuExampleClass *class)
{
    parent_class = g_type_class_peek_parent (class);
}

static void
menu_example_instance_init (MenuExample *self)
{
  g_message ("Initializing MenuExample extension..");
}

static void menu_provider_iface_init (CajaMenuProviderIface *iface)
{
    iface->get_file_items = menu_example_get_file_items;
    iface->get_background_items = menu_example_get_background_items;
    iface->get_toolbar_items = menu_example_get_toolbar_items;
}

/* Menu Provider specific functions: */

static void
menu_example_activate (CajaMenuItem *item, gpointer user_data)
{
    GtkWidget *dialog;
    
    dialog = gtk_message_dialog_new_with_markup (NULL, 0,
                    GTK_MESSAGE_ERROR,
                    GTK_BUTTONS_CLOSE,
                    "You have succesfully activated\n"
                    "the MenuExample signal.");
    gtk_dialog_run (GTK_DIALOG(dialog));
    gtk_widget_destroy (dialog);
}

CajaMenuItem *
menu_example_create_item (CajaMenuProvider *provider, 
                          char *name, char *label, char *tooltip)
{
	CajaMenuItem *item;
	item = caja_menu_item_new (name, label, tooltip, "gtk-floppy");
                       
	g_signal_connect (item, "activate", 
                      G_CALLBACK (menu_example_activate), provider); 
    return item;
}

static GList *
menu_example_get_file_items (CajaMenuProvider *provider,
                             GtkWidget *window,
                             GList *files)
{
    /* Place the 'per-file' GUI elements here. These elements are
     * shown in the file context menu and the Edit menu. */
    GList *items = NULL;
    CajaMenuItem *item;
    
    item = menu_example_create_item (provider, 
                        "MenuExampleFile::do_stuff",
                        "MenuExample per-file",
                        "Do MenuExample stuff to the selected files");
                      
    items = g_list_prepend (items, item);
    return items;
}

static GList *
menu_example_get_background_items (CajaMenuProvider *provider, 
                                   GtkWidget *window,
                                   CajaFileInfo *current_folder)
{
    /* Place the 'per-folder GUI elements here. These elements are
     * shown in the background context menu and the File menu. */
    GList *items = NULL;
    CajaMenuItem *item;

	item = menu_example_create_item (provider, 
                        "MenuExampleFolder::do_stuff",
                        "MenuExample per-folder",
                        "Do MenuExample stuff to the selected files");

    items = g_list_prepend (items, item);
    return items;
}

static GList *
menu_example_get_toolbar_items (CajaMenuProvider *provider, 
                                GtkWidget *window,
                                CajaFileInfo *current_folder)
{
    /* Place the 'toolbar' GUI elements here. These elements are
     * shown in the main toolbar. */
    GList *items = NULL;
    CajaMenuItem *item;

	item = menu_example_create_item (provider, 
                        "MenuExampleToolbar::do_stuff",
                        "MenuExample toolbar",
                        "Do MenuExample stuff to the selected files");
                       
    items = g_list_prepend (items, item);
    return items;
}
