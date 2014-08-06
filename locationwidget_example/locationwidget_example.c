
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
#include <libcaja-extension/caja-location-widget-provider.h>

typedef struct {
    GObject parent;
} LocationWidgetExample;

typedef struct {
    GObjectClass parent_class;
} LocationWidgetExampleClass;

static GObjectClass *parent_class = NULL;
static GType provider_types[1];
static GType locationwidget_example_type;


static void locationwidget_example_register_type (GTypeModule *module);
GType locationwidget_example_get_type (void);

static void locationwidget_example_class_init (LocationWidgetExampleClass *class);
static void locationwidget_example_instance_init (LocationWidgetExample *self);
static void locationwidget_provider_iface_init (CajaLocationWidgetProviderIface *iface);

static GtkWidget *
locationwidget_example_get_location_widget (CajaLocationWidgetProvider *provider,
                                            const char *uri, 
                                            GtkWidget *window);

/* Extension interface: */

void
caja_module_initialize (GTypeModule  *module) 
{
	locationwidget_example_register_type (module);

	provider_types[0] = locationwidget_example_get_type ();
}

void 
caja_module_shutdown (void)
{
	// Any module-specific shutdown:
}

void
caja_module_list_types (const GType **types,
			    int *num_types)
{
	*types = provider_types;
	*num_types = G_N_ELEMENTS (provider_types);
}


static void
locationwidget_example_register_type (GTypeModule *module)
{
        static const GTypeInfo info = {
                sizeof (LocationWidgetExampleClass),
                (GBaseInitFunc) NULL,
                (GBaseFinalizeFunc) NULL,
                (GClassInitFunc) locationwidget_example_class_init,
                NULL,
                NULL,
                sizeof (LocationWidgetExample),
                0,
                (GInstanceInitFunc) locationwidget_example_instance_init,
        };

    static const GInterfaceInfo menu_provider_iface_info = {
        (GInterfaceInitFunc)locationwidget_provider_iface_init,
        NULL,
        NULL
    };
 
	locationwidget_example_type = g_type_module_register_type (module,
							  G_TYPE_OBJECT,
							  "LocationWidgetExample",
							  &info, 0);

    // add interface:
    g_type_module_add_interface (module,
				 locationwidget_example_type,
				 CAJA_TYPE_LOCATION_WIDGET_PROVIDER,
				 &menu_provider_iface_info);
}

GType
locationwidget_example_get_type (void)
{
	return locationwidget_example_type;
}

/* Initiation functions: */

static void
locationwidget_example_class_init (LocationWidgetExampleClass *class)
{
    parent_class = g_type_class_peek_parent (class);
}

static void
locationwidget_example_instance_init (LocationWidgetExample *self)
{
  g_message ("Initializing LocationWidgetExample extension...");
}

static void locationwidget_provider_iface_init (CajaLocationWidgetProviderIface *iface)
{
    iface->get_widget = locationwidget_example_get_location_widget;
}

/* Location Widget Provider specific functions: */

static GtkWidget *
locationwidget_example_get_location_widget (CajaLocationWidgetProvider *provider,
                                            const char *uri, 
                                            GtkWidget *window)
{
    GFile *file;
    GFile *home;
    
    /* Basic example that adds a GtkLabel to the file manager when
     * the current directory is $HOME.*/
	file = g_file_new_for_uri (uri);
	home = g_file_new_for_path (g_get_home_dir ());
    
	if (!g_file_equal (home, file)) {
		g_object_unref (home);
		g_object_unref (file);
		return NULL;
	}

    g_object_unref (home);
    g_object_unref (file);
    
    GtkWidget *bar = gtk_label_new ("LocationWidget example\n");
    
    gtk_widget_show_all (bar);
    return bar;
}

