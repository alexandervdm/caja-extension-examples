
/*
 * @file   propertypage_example.c
 * @brief  example for the caja propertypage extension provider
 *
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
#include <libcaja-extension/caja-property-page-provider.h>

typedef struct {
    GObject parent;
} PropertyPageExample;

typedef struct {
    GObjectClass parent_class;
} PropertyPageExampleClass;

static GObjectClass *parent_class = NULL;
static GType provider_types[1];
static GType propertypage_example_type;


static void propertypage_example_register_type (GTypeModule *module);
GType propertypage_example_get_type (void);

static void propertypage_example_class_init (PropertyPageExampleClass *class);
static void propertypage_example_instance_init (PropertyPageExample *self);
static void propertypage_provider_iface_init (CajaPropertyPageProviderIface *iface);

static GList *
propertypage_example_get_pages (CajaPropertyPageProvider *provider,
						 GList *files);

/* Extension interface: */

void
caja_module_initialize (GTypeModule  *module) 
{
	propertypage_example_register_type (module);

	provider_types[0] = propertypage_example_get_type ();
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
propertypage_example_register_type (GTypeModule *module)
{
        static const GTypeInfo info = {
                sizeof (PropertyPageExampleClass),
                (GBaseInitFunc) NULL,
                (GBaseFinalizeFunc) NULL,
                (GClassInitFunc) propertypage_example_class_init,
                NULL,
                NULL,
                sizeof (PropertyPageExample),
                0,
                (GInstanceInitFunc) propertypage_example_instance_init,
        };

    static const GInterfaceInfo menu_provider_iface_info = {
        (GInterfaceInitFunc)propertypage_provider_iface_init,
        NULL,
        NULL
    };
 
	propertypage_example_type = g_type_module_register_type (module,
							  G_TYPE_OBJECT,
							  "PropertyPageExample",
							  &info, 0);

    // add interface:
    g_type_module_add_interface (module,
				 propertypage_example_type,
				 CAJA_TYPE_PROPERTY_PAGE_PROVIDER,
				 &menu_provider_iface_info);
}

GType
propertypage_example_get_type (void)
{
	return propertypage_example_type;
}

/* Initiation functions: */

static void
propertypage_example_class_init (PropertyPageExampleClass *class)
{
    parent_class = g_type_class_peek_parent (class);
}

static void
propertypage_example_instance_init (PropertyPageExample *self)
{
  g_message ("Initializing PropertyPageExample extension...");
}

static void propertypage_provider_iface_init (CajaPropertyPageProviderIface *iface)
{
    iface->get_pages = propertypage_example_get_pages;
}


/* Property Page Provider specific functions: */

static GList *
propertypage_example_get_pages (CajaPropertyPageProvider *provider,
						 GList *files)
{
	GList *pages;
	CajaPropertyPage *page;
	CajaFileInfo *file;
	GtkWidget *properties;

    // only show property page when a single file is selected:
	if (!files || files->next != NULL) {
	        return NULL;
	}
	
	file = files->data;

    /* only show property page for certain file types:
	if (!caja_file_is_mime_type (file, "application/x-foo")) {
	        return NULL;
	}*/
	
	pages = NULL;
    gchar *name;
	name = caja_file_info_get_uri (file);

	properties = gtk_button_new_with_label (name);
	gtk_widget_show (properties);

	page = caja_property_page_new ("PropertyPageExample::property_page",
                                    gtk_label_new ("PropertyPageExample"),
                                    properties);
    pages = g_list_append (pages, page);
	return pages;
}
